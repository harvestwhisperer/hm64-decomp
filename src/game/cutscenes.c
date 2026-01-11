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
      { &_familyCutscenesSegmentRomStart, &_familyCutscenesSegmentRomEnd },
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

static inline void transitionCutscenes(u16 cutsceneIndex, u16 entranceIndex) {
    setEntrance(entranceIndex);
    gCutsceneIndex = cutsceneIndex;
    loadCutscene(FALSE);
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
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!(gCutsceneFlags & 1)) {
        
        gCutsceneFlags &= ~2;

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
        gCutsceneFlags |= 2;
        loadCutscene(morningVisit);
        set = TRUE;
    }

    if (gEntranceIndex != 0x6F) {
        
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
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == SPRING && gDayOfMonth == 16 && (5 < gHour && gHour < 15) && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
                setDailyEventBit(FARM_MORNING_VISIT);
                gCutsceneIndex = DOUG_FARM_HORSE_RACE;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(FARM_MORNING_VISIT) && gSeason == AUTUMN && gDayOfMonth == 27 && (5 < gHour && gHour < 15) && (horseInfo.flags & 1) && horseInfo.grown == TRUE) {
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
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[POPURI] >= 50 && gSeason == SPRING && gDayOfMonth == 22 && (5 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 156;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ELLI] >= 50 && gSeason == AUTUMN && gDayOfMonth == 11 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 168;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ANN] >= 80 && gSeason == SPRING && gDayOfMonth == 16 && (6 < gHour && gHour < 9) && !checkLifeEventBit(MARRIED) && checkLifeEventBit(0x51) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 179;
                loadCutscene(morningVisit);
                set = TRUE;
            }
        }

        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && npcAffection[MARIA] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = MARIA_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[POPURI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = POPURI_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ELLI] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = ELLI_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ANN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = ANN_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[KAREN] >= 50 && gSeason == SUMMER && gDayOfMonth == 1 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = KAREN_FIREWORKS_INVITATION;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }
        
        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && npcAffection[MARIA] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = MARIA_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[POPURI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = POPURI_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ELLI] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = ELLI_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ANN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = ANN_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }    

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[KAREN] >= 140 && gSeason == WINTER && gDayOfMonth == 10 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = KAREN_THANKSGIVING_GIFT;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && npcAffection[MARIA] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 8 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 149;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[POPURI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 10 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 160;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }        

        if (!set) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ELLI] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 12 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 172;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[ANN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 14 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 183;
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }       

        if (!set) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && npcAffection[KAREN] >= 100 && gSeason == WINTER && gDayOfMonth == 24 && gHour == 16 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
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
                                    setLifeEventBit(0x3F);
                                    gCutsceneIndex = 153;
                                } else {
                                    gCutsceneIndex = 152;
                                }
                                
                                gCutsceneFlags |= 2;
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
                                
                                gCutsceneFlags |= 2;
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
                                
                                gCutsceneFlags |= 2;
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
                                
                                gCutsceneFlags |= 2;
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
                                
                                gCutsceneFlags |= 2;
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
                
                loadCutscene(morningVisit);
                set = TRUE;
            }            
        }

        if (!set) {
            if (checkLifeEventBit(BASIL_IN_TOWN) && ((gSeason == AUTUMN && gDayOfMonth >= 5) || gSeason == WINTER) && (5 < gHour && gHour < 17)) {
                toggleLifeEventBit(BASIL_IN_TOWN);
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
            if (checkLifeEventBit(0x41) && !checkLifeEventBit(0x42) && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 4 && gHarvestCoinFinder != 4) {
                
                if (checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[CLIFF] >= 150 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = 213;
                    loadCutscene(morningVisit);
                    set = TRUE;                    
                }
                
                if (npcAffection[CLIFF] < 51 && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {              
                    setLifeEventBit(0x42);
                    setLifeEventBit(0x2D);
                    gCutsceneIndex = CLIFF_LEAVING_DEFAULT;
                    loadCutscene(morningVisit);
                    set = TRUE;                                
                } 
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x33) && checkDailyEventBit(0x22) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x33);
                gCutsceneIndex = 105;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x35) && checkDailyEventBit(0x23) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x35);
                gCutsceneIndex = 106;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }
        
        if (!set) {

            if (!checkLifeEventBit(0x37) && checkDailyEventBit(0x24) && npcAffection[KENT] >= 5 && gWeather == SUNNY && (5 < gHour && gHour < 9)) {
                
                setLifeEventBit(0x37);

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
            if (!checkLifeEventBit(0x39) && checkDailyEventBit(0x25) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x39);
                gCutsceneIndex = 111;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }            
        
        if (!set) {
            if (!checkLifeEventBit(0x3B) && checkDailyEventBit(0x26) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x3B);
                gCutsceneIndex = 109;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x3D) && checkDailyEventBit(0x27) && npcAffection[KENT] >= 5 && (5 < gHour && gHour < 9)) {
                setLifeEventBit(0x3D);
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

                if (checkDailyEventBit(0x3F)) {           
                    setDailyEventBit(FARM_MORNING_VISIT);
                    gCutsceneIndex = 221;
                    loadCutscene(morningVisit);
                    set = TRUE;                               
                }
            }
        }    

        if (!set) {
            if (!checkLifeEventBit(0x8E) && getTotalFarmAnimalsCount() < 6 && gYear == 1 && gSeason == AUTUMN && gDayOfMonth == 30 && (5 < gHour && gHour < 24)) {
                setLifeEventBit(0x8E);
                gCutsceneIndex = DOUG_FARM_FAVOR;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x8F) && checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 1 && (5 < gHour && gHour < 10) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8F);
                gCutsceneIndex = DOUG_DROPPING_OFF_COWS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (checkLifeEventBit(0x94) && gSeason == WINTER && gDayOfMonth == 5 && (5 < gHour && gHour < 10) && gEntranceIndex == HOUSE_EXIT) {
                toggleLifeEventBit(0x94);
                gCutsceneIndex = MISS_MANA_PICKUP_COWS;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x68) && npcAffection[KENT] >= 50 && gSeason == SUMMER && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x68);
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
            if (!checkLifeEventBit(0x6A) && getFarmMoondropFlowerCount() >= 8 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6A);
                gCutsceneIndex = 114;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }        
        
        if (!set) {
            if (!checkLifeEventBit(0x6B) && getFarmPinkCatMintFlowersCount() && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6B);
                gCutsceneIndex = 115;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }        

        if (!set) {
            if (!checkLifeEventBit(0x6C) && getFarmPinkCatMintFlowersCount() >= 16 && npcAffection[STU] >= 80 && gWeather == SUNNY && (gSeason && gSeason < 3) && (5 < gHour && gHour < 12)) {
                setLifeEventBit(0x6C);
                gCutsceneIndex = 116;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x6D) && npcAffection[MARIA] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != MARIA)) {
                setLifeEventBit(0x6D);
                gCutsceneIndex = MARIA_FARM_GIFT_CHOCOLATE;
                loadCutscene(morningVisit);
                set = TRUE;    
            }
        }               
        
        if (!set) {
            if (!checkLifeEventBit(0x6E) && npcAffection[MARIA] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6E);
                gCutsceneIndex = MARIA_FARM_GIFT_RICE_BALLS;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     

        if (!set) {
            if (!checkLifeEventBit(0x6F) && npcAffection[MARIA] >= 200 && gWeather == SUNNY && gSeason == SUMMER && NIGHTTIME && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
                setLifeEventBit(0x6F);
                gCutsceneIndex = 147;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }     
        
        if (!set) {
            if (!checkLifeEventBit(0x70) && npcAffection[POPURI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != POPURI)) {
                setLifeEventBit(0x70);
                gCutsceneIndex = POPURI_FARM_GIFT_TEA;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }          
        
        if (!set) {
            if (!checkLifeEventBit(0x71) && npcAffection[POPURI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
                setLifeEventBit(0x71);
                gCutsceneIndex = POPURI_FARM_GIFT_COOKIES;
                loadCutscene(morningVisit);
                set = TRUE;                               
            }
        }          
        if (!set) {
            
            if (!checkLifeEventBit(0x72) && blueMistFlowerPlot >= 0xC2 && npcAffection[POPURI] >= 200 && gWeather == SUNNY && gSeason == SUMMER && gDayOfMonth == 30 && (5 < gHour && gHour < 18) 
                    && ((checkLifeEventBit(MARRIED) && gWife == POPURI) || (!checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)))) {
                
                blueMistFlowerPlot = 0;
                pondFieldTiles[5][3] = 0;
                setLifeEventBit(0x72);
                gCutsceneIndex = 158;
                loadCutscene(morningVisit);
                set = TRUE;           
                
            }
        } 

        if (!set) {
            if (!checkLifeEventBit(0x73) && npcAffection[ELLI] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x73);
                gCutsceneIndex = 165;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x74) && npcAffection[ELLI] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x74);
                gCutsceneIndex = 166;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x75) && npcAffection[ELLI] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 9 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
                setLifeEventBit(0x75);
                gCutsceneIndex = 169;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }      

        if (!set) {
            if (!checkLifeEventBit(0x76) && getChickenEggCount() >= 6 && gSeason == WINTER && gDayOfMonth == 9 && (5 < gHour && gHour < 24) && (!checkLifeEventBit(MARRIED) || gWife != ELLI)) {
                setLifeEventBit(0x76);
                gCutsceneIndex = 170;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x77) && npcAffection[ANN] >= 50 && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != ANN)) {
                setLifeEventBit(0x77);
                gCutsceneIndex = 177;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x78) && npcAffection[ANN] >= 80 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x78);
                gCutsceneIndex = 178;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x79) && npcAffection[ANN] >= 200 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
                setLifeEventBit(0x79);
                gCutsceneIndex = 181;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7A) && npcAffection[ANN] >= 50 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7A);
                gCutsceneIndex = 188;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7B) && npcAffection[ANN] >= 80 && (5 < gHour && gHour < 12) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(0x3C)) {
                setLifeEventBit(0x7B);
                gCutsceneIndex = 189;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  

        if (!set) {
            if (!checkLifeEventBit(0x7C) && npcAffection[KAREN] >= 60 && (1 < gSeason && gSeason < 4) && (14 < gHour && gHour < 17) && (!checkLifeEventBit(MARRIED) || gWife != KAREN)) {
                setLifeEventBit(0x7C);
                gCutsceneIndex = 190;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7D) && npcAffection[KAREN] >= 100 && (14 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7D);
                gCutsceneIndex = 191;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            if (!checkLifeEventBit(0x7E) && checkLifeEventBit(0x96) && npcAffection[KAREN] >= 200 && gWeather == SUNNY && gSeason == AUTUMN && gDayOfMonth == 7 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x7E);
                gCutsceneIndex = 193;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }  
        
        if (!set) {
            
            if (!checkLifeEventBit(0x7F) && checkLifeEventBit(0x96) && gSeason == WINTER && (6 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 18)) {
                
                setLifeEventBit(0x7F);
                setLifeEventBit(0x20);

                if (npcAffection[KAREN] >= npcAffection[KAI]) {
                    
                    if (!checkLifeEventBit(0x2F)) {
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
            if (!checkLifeEventBit(0x80) && npcAffection[HARRIS] >= 50 && gSeason == AUTUMN && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x80);
                gCutsceneIndex = 204;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x81) && npcAffection[CLIFF] >= 50 && (14 < gHour && gHour < 17)) {
                setLifeEventBit(0x81);
                gCutsceneIndex = 210;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x83) && checkLifeEventBit(0x2A) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x83);
                gCutsceneIndex = 218;
                loadCutscene(morningVisit);
                set = TRUE;                      
            }
        }    

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x84) && checkLifeEventBit(6) && npcAffection[RICK] >= 120 && gWeather == SUNNY && (gSeason && gSeason < 4) && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x84);
                gCutsceneIndex = 220;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set) {
            if (!checkLifeEventBit(0x85) && (5 < gHour && gHour < 12) && getTotalChickenCount() >= 5 && chickenFeedQuantity < 990) {
                setLifeEventBit(0x85);
                gCutsceneIndex = 224;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set) {
            if (!checkLifeEventBit(0x86) && (gSeason && gSeason < 4) && (5 < gHour && gHour < 12) && gTotalCropsShipped >= 900 && chickenFeedQuantity < 990) {
                setLifeEventBit(0x86);
                gCutsceneIndex = 225;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x87) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x87);
                gCutsceneIndex = 228;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x88) && getFarmGrassTilesSum() >= 90 && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x88);
                gCutsceneIndex = 230;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x89) && checkLifeEventBit(0x20) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
                setLifeEventBit(0x89);
                gCutsceneIndex = 231;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x8A) && checkLifeEventBit(0x53) && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8A);
                gCutsceneIndex = 232;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x8B) && checkLifeEventBit(0x30) && gWeather == SUNNY && getTotalCowsCount() >= 3 && gSeason == WINTER && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8B);
                gCutsceneIndex = 233;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }
        
        if (!set && morningVisit == FALSE) {
            // get cow + grass count
            if (!checkLifeEventBit(0x8C) && checkLifeEventBit(0x30) && func_8009B828(1) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8C);
                gCutsceneIndex = 234;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }

        if (!set && morningVisit == FALSE) {
            if (!checkLifeEventBit(0x8D) && (getFarmMoondropFlowerCount() + getFarmPinkCatMintFlowersCount()) >= 24 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gEntranceIndex == HOUSE_EXIT) {
                setLifeEventBit(0x8D);
                gCutsceneIndex = 236;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }        

        if (!set) {
            if (checkLifeEventBit(0x90) && npcAffection[DOUG] >= 60 && (16 < gHour && gHour < 21) && ((gSeason == SPRING && gDayOfMonth == 18) || gSeason == AUTUMN && gDayOfMonth == 29)) {
                toggleLifeEventBit(0x90);
                gCutsceneIndex = 129;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (checkLifeEventBit(0x91) && npcAffection[DOUG] >= 60 && gSeason == AUTUMN && gDayOfMonth == 5 && (16 < gHour && gHour < 21)) {
                toggleLifeEventBit(0x91);
                gCutsceneIndex = 130;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }          

        if (!set) {
            if (checkLifeEventBit(0x92) && npcAffection[BASIL] >= 60 && gSeason == SUMMER && gDayOfMonth == 10 && (16 < gHour && gHour < 21)) {
                toggleLifeEventBit(0x92);
                gCutsceneIndex = 139;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }       
        
        if (!set) {
            if (!checkLifeEventBit(0x95) && checkLifeEventBit(0x2F) && npcAffection[KAI] <= 80 && gSeason == WINTER && (5 < gHour && gHour < 18) && gHarvestKing != 5 && gHarvestCoinFinder != 5 && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
                setLifeEventBit(0x95);
                setLifeEventBit(0x2E);
                setSpecialDialogueBit(0x24);
                gCutsceneIndex = 219;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }      
        
        if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[POTION_SHOP_DEALER] >= 50 && gWeather == SUNNY && gYear == 1 && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 201;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        } 

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[HARRIS] >= 100 && gHarvestKing == 1 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 202;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    
        
      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[GRAY] >= 100 && gHarvestKing == 2 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 205;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    

      if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[JEFF] >= 100 && gHarvestKing == 3 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 207;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }    
        
    if (!set) {
        if (!checkDailyEventBit(0x51) && npcAffection[CLIFF] >= 100 && gHarvestKing == 4 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
            setDailyEventBit(0x51);
            gCutsceneIndex = 211;
            loadCutscene(morningVisit);
            set = TRUE;                             
        }
    }        
        
       if (!set) {
            if (!checkDailyEventBit(0x51) && npcAffection[KAI] >= 100 && gHarvestKing == 5 && gWeather == SUNNY && gSeason == WINTER && gDayOfMonth == 27 && (5 < gHour && gHour < 9)) {
                setDailyEventBit(0x51);
                gCutsceneIndex = 216;
                loadCutscene(morningVisit);
                set = TRUE;                             
            }
        }      
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 239;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 240;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 241;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 242;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 1 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 243;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 244;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 245;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 246;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 247;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 24 && gWeather == SUNNY && (17 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 248;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 150;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 161;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 173;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 184;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gSeason == SUMMER && gDayOfMonth == 7 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 197;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 220) {
                setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 151;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }       
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 220) {
                setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 162;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }      
    
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 220) {
                setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 174;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }        
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 220) {
                setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 185;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
                set = TRUE;                             
            }
        }     
            
        if (!set && morningVisit == FALSE) {
            if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gSeason == AUTUMN && gDayOfMonth == 9 && gWeather == SUNNY && (16 < gHour && gHour < 21) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 220) {
                setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
                gCutsceneIndex = 198;
                loadCutscene(morningVisit);
                gCutsceneFlags |= 2;
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
        gCutsceneFlags |= 2;
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set) {
        if (checkDailyEventBit(1)) {
            bytecodeSegmentIndex = 1;
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
            bytecodeSegmentIndex = 2;
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

            bytecodeSegmentIndex = 3;
            set = TRUE;
            gCutsceneFlags |= 2;
        
        }

    }

    // enter cow festival
    if (!set) {
        if (checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E)) {
            bytecodeSegmentIndex = 33;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    // cow festival take cow
    if (!set) {
        if (checkDailyEventBit(0x20)) {
            bytecodeSegmentIndex = 34;
            set = TRUE;
            gCutsceneIndex = 1;
            gCutsceneFlags |= 2;
        }
    }

    // house construction estimate
    if (!set) {
        if (checkDailyEventBit(8)) {
            gCutsceneIndex = handlePurchaseHouseExtension(gHouseExtensionSelection);
            gCutsceneFlags |= 2;
            bytecodeSegmentIndex = 4;
            set = TRUE;
        }
    }

    // house construction
    if (!set) {
        if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION) && checkDailyEventBit(0xA) && !checkDailyEventBit(0xC)) {
            gCutsceneIndex = 1;
            bytecodeSegmentIndex = 5;
            set = TRUE;
        }
    }

    // house construction completed
    if (!set) {
        if (checkDailyEventBit(0xB)) {
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

    if (!checkLifeEventBit(0x48) && gSeason == AUTUMN && (22 < gDayOfMonth && gDayOfMonth < 28) && (5 < gHour && gHour < 15)) {
        setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
        if (checkDailyEventBit(0x2C)) {
            gCutsceneIndex = 325;
        } else {
            gCutsceneIndex = 324;
            gCutsceneFlags |= (2 | 4);
        }
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0x4C) && gSeason == WINTER && (11 < gDayOfMonth && gDayOfMonth < 17) && (5 < gHour && gHour < 15)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            if (checkDailyEventBit(0x2C)) {
                gCutsceneIndex = 325;
            } else {
                gCutsceneIndex = 328;
                gCutsceneFlags |= (2 | 4);
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
        if (!checkLifeEventBit(0xA4) && npcAffection[JEFF] >= 180 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(0xA4);
            gCutsceneIndex = 308;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA6) && npcAffection[MARIA] >= 150 && npcAffection[ELLI] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && gEntranceIndex == TOP_OF_MOUNTAIN_1_SUMMER) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(0xA6);
            gCutsceneIndex = 310;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xA9) && npcAffection[POPURI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && (5 < gHour && gHour < 18) && gEntranceIndex == 0x1A) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(0xA9);
            gCutsceneIndex = 314;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
      if (!checkLifeEventBit(0xAD) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ELLI] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
            setDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY);
            setLifeEventBit(0xAD);
            setDailyEventBit(0x58);
            gCutsceneIndex = ELLI_SPRAINED_ANKLE;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
        }
    } 
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMountain2Cutscenes);

u16 setMountain2Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(0xA0) && checkLifeEventBit(0x64) && checkLifeEventBit(0x48) && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0xA0);
        gCutsceneIndex = 301;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA1) && !checkDailyEventBit(0x30) && npcAffection[POPURI] >= 180 && gWeather == SUNNY && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0xA1);
        setDailyEventBit(0x57);
        gCutsceneIndex = 302;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(0xA2) && checkLifeEventBit(0x66) && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gEntranceIndex == 0x1D) { 
        setLifeEventBit(0xA2);
        gCutsceneIndex = 306;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA3) && npcAffection[ANN] >= 80 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18) {
        setLifeEventBit(0xA3);
        gCutsceneIndex = 307;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA5) && npcAffection[CLIFF] >= 150 && gWeather == SUNNY && gSeason != WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && gEntranceIndex == 0x1D) {
        setLifeEventBit(0xA5);
        gCutsceneIndex = 309;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set && !checkLifeEventBit(0xA7) && npcAffection[MARIA] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18 && gEntranceIndex == 0x1D) {
        setLifeEventBit(0xA7);
        gCutsceneIndex = 311;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }
    
    if (!set && !checkLifeEventBit(0xAA) && npcAffection[STU] >= 50 && npcAffection[MAY] >= 50 && gWeather == SUNNY && gSeason < AUTUMN && gDayOfWeek == SATURDAY && 5 < gHour && gHour < 18) {
        setLifeEventBit(0xAA);
        gCutsceneIndex = 338;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }


    if (!set && !checkLifeEventBit(0x98) && !checkLifeEventBit(MARRIED) && npcAffection[POPURI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(0x98);
        gCutsceneIndex = 342;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setTopOfMountain1Cutscenes);

// TODO: label; top of mountain 1
u16 setTopOfMountain1Cutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(0xA8) && checkLifeEventBit(0x4C) && npcAffection[POPURI] >= 150 && npcAffection[JEFF] >= 120 && gWeather == SUNNY && gSeason >= AUTUMN && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {
        setLifeEventBit(0xA8);
        gCutsceneIndex = 312;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xAB) && checkLifeEventBit(0x4C) && gWeather == SUNNY && gSeason == AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(0xAB);
            gCutsceneIndex = 339;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xAC) && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && checkLifeEventBit(HAVE_KITCHEN) && checkLifeEventBit(HAVE_BATHROOM) && checkLifeEventBit(HAVE_STAIRS) && checkLifeEventBit(HAVE_GREENHOUSE) && checkLifeEventBit(HAVE_LOG_TERRACE) && checkLifeEventBit(HAVE_BABY_BED) && checkLifeEventBit(HOT_SPRINGS_COMPLETED)) {
            setLifeEventBit(0xAC);
            gCutsceneIndex = 340;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
        }
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMoonMountainCutscenes);

// mountain 2
u16 setMoonMountainCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        gCutsceneIndex = MOUNTAIN_FIREWORKS;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
            gCutsceneIndex = STARRY_NIGHT_MOON_MOUNTAIN;
            loadCutscene(FALSE);
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setPondCutscenes);

u16 setPondCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(0x9F) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
        setLifeEventBit(0x9F);
        setDailyEventBit(HARVEST_GODDESS_INTERACTION);
        gCutsceneIndex = 315;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xD5) && checkLifeEventBit(0x9F) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(0xD5);
            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
            gCutsceneIndex = 316;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
         if (!checkLifeEventBit(0xD6) && checkLifeEventBit(0xD5) && blueMistFlowerPlot >= 0xC2 && gSeason < AUTUMN && (5 < gHour && gHour < 18)) {
            setLifeEventBit(0xD6);
            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
            gCutsceneIndex = 317;
            gCutsceneFlags |= (2 | 4);
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
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
    }

    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setCaveCutscenes);

u16 setCaveCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    if (gSeason == WINTER && (7 < gDayOfMonth && gDayOfMonth < 31) && (5 < gHour && gHour < 16)) {
        gCutsceneIndex = 346;
        gCutsceneFlags |= 2;
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
        
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= 2;
        
    }

    if (!set) {
        if (checkLifeEventBit(0x60) && !checkLifeEventBit(0x61) && !checkLifeEventBit(MARRIED) && npcAffection[KENT] >= 160 && !checkLifeEventBit(MARRIED)) {
            gCutsceneIndex = 1;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x62) && npcAffection[CLIFF] >= 30 && gWeather == SUNNY && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
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
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = POPURI_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ELLI_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = ANN_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkDailyEventBit(FARM_MORNING_VISIT) && !checkLifeEventBit(WIFE_LEFT) && (5 < gHour && gHour < 18) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] < 100) {
            setDailyEventBit(FARM_MORNING_VISIT);
            setLifeEventBit(WIFE_LEFT);
            gCutsceneIndex = KAREN_LEAVES_MARRIAGE;
            loadCutscene(FALSE);
            gCutsceneFlags |= 2;
            set = TRUE;
        }
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setKitchenCutscenes);

u16 setKitchenCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == MARIA && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
        
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == POPURI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ELLI && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ANN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == KAREN && babyBirthdaySeason == gSeason && babyBirthdayDate == gDayOfMonth) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_BABY_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && npcAffection[MARIA] >= 100) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && npcAffection[POPURI] >= 100) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && npcAffection[ELLI] >= 100) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && npcAffection[ANN] >= 100) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gPlayerBirthdaySeason == gSeason && gDayOfMonth == 25 && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && npcAffection[KAREN] >= 100) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_MARRIED_PLAYER_BIRTHDAY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == MARIA && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == POPURI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ELLI && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == ANN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && (16 < gHour && gHour < 24) && checkLifeEventBit(MARRIED) && gWife == KAREN && anniversarySeason == gSeason && anniversaryDate == gDayOfMonth) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_ANNIVERSARY;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }
    
    if (!set && !checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == MARIA) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = MARIA_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }
        
    if (!set && !checkDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == POPURI) {
        setDailyEventBit(POPURI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = POPURI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ELLI) {
        setDailyEventBit(ELLI_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ELLI_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == ANN) {
        setDailyEventBit(ANN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = ANN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }

    if (!set && !checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (16 < gHour && gHour < 20) && checkLifeEventBit(MARRIED) && gWife == KAREN) {
        setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = KAREN_MARRIED_THANKSGIVING;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= 2;
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRanchCutscenes);

u16 setRanchCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (gYear == 1 && gSeason == SPRING && gDayOfMonth >= 3 && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(HAVE_HORSE)) {

        if (!checkLifeEventBit(0xE)) {
            gCutsceneIndex = 650;
        } else {
            gCutsceneIndex = 665;
        }

        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {

        gCutsceneIndex = 659;
        loadCutscene(FALSE);
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
        
        gCutsceneIndex = 1150;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }

    if (!set && !checkDailyEventBit(0x1B) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {

        setDailyEventBit(0x1B);
        gCutsceneIndex = 662;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    
    }
    
    if (!set && !checkLifeEventBit(0xBE) && npcAffection[GRAY] >= 150 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 17 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 652;
            setLifeEventBit(0xBE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 653;
            setLifeEventBit(0xBE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(0xBF) && npcAffection[GRAY] >= 180 && gWeather == SUNNY && gSeason < WINTER && 17 < gHour && gHour < 24 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 654;
            setLifeEventBit(0xBF);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 655;
            setLifeEventBit(0xBF);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }

    if (!set && !checkLifeEventBit(0xC0) && npcAffection[POPURI] >= 150 && npcAffection[ANN] >= 150 && gWeather == SUNNY && gSeason == AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {

        if (gEntranceIndex == 0x29) {
            gCutsceneIndex = 657;
            setLifeEventBit(0xC0);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
        if (gEntranceIndex == 0x2A) {
            gCutsceneIndex = 658;
            setLifeEventBit(0xC0);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
        
    }
    
    if (!set && !checkLifeEventBit(0xC1) && npcAffection[CLIFF] >= 100 && gWeather == SUNNY && gSeason < WINTER && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(0x1D)) {

        setLifeEventBit(0xC1);
        gCutsceneIndex = 660;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;

    }

    if (!set && !checkLifeEventBit(0x9B) && !checkLifeEventBit(MARRIED) && npcAffection[ANN] >= 250 && gWeather == SUNNY && 14 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1D) && gEntranceIndex == 0x29) {

        setLifeEventBit(0x9B);
        gCutsceneIndex = 661;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;

    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRanchStoreCutscenes);

u16 setRanchStoreCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
     
    if (!(checkLifeEventBit(ANN_SICK_VISIT)) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ANN] >= 120 && (2 <= gWeather && gWeather < 4) && gDayOfWeek == SUNDAY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED)) {
        setLifeEventBit(ANN_SICK_VISIT);
        setDailyEventBit(0x59);
        gCutsceneIndex = ANN_SICK_VISIT_CUTSCENE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }
     
    if (checkLifeEventBit(MARRIED) && gWife == ANN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = ANN_REPAIR_MARRIAGE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }

    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardCutscenes);

u16 setVineyardCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0xC3) && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != 4) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(0xC3);
        gCutsceneIndex = 700;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xC4) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[KAREN] >= 180 && gWeather == SUNNY && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0xC4);
            setDailyEventBit(0x5A);
            gCutsceneIndex = 701;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xC5) && npcAffection[ANN] >= 120 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == ROAD) {
            setLifeEventBit(0xC5);
            gCutsceneIndex = 702;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0xC6) && npcAffection[KAI] >= 50 && npcAffection[KAREN] >= 50 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == ROAD) {
            setLifeEventBit(0xC6);
            gCutsceneIndex = 703;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
        }
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardHouseCutscenes);

u16 setVineyardHouseCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(KAREN_SICK_VISIT) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[KAREN] >= 120U && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_SICK_VISIT);
        setDailyEventBit(0x5A);
        gCutsceneIndex = KAREN_SICK;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == KAREN && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 712;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVineyardCellarCutscenes);

u16 setVineyardCellarCutscenes(void) {
    
    bool set;
    u16 bytecodeSegmentIndex = 0xFFFF;

    set = FALSE;
    
    if (!checkLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE) && checkLifeEventBit(0x67) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD) {
               
        setLifeEventBit(KAREN_YELLOW_HEART_EVENT_CUTSCENE);
        setDailyEventBit(0x4F);

        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_1;
        gCutsceneFlags |= (2 | 4);

        loadCutscene(FALSE);

        set = TRUE;

    }
    
    if (!set && checkDailyEventBit(0x4F) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED) && previousMapIndex == VINEYARD_CELLAR_BASEMENT) {
        gCutsceneIndex = KAREN_YELLOW_HEART_EVENT_4;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setVillage1Cutscenes);

u16 setVillage1Cutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(0x9E) && checkLifeEventBit(2) && checkHaveTool(0x13)) {
        setLifeEventBit(0x9E);
        gCutsceneIndex = 410;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    } 

    if (!set && gSeason == SUMMER && gDayOfMonth == 9 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 1000;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 20 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 1250;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 

    if (!set && !checkDailyEventBit(0x19) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(0x19);
        gCutsceneIndex = 424;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    } 


    if (!set && !checkLifeEventBit(0xAE) && npcAffection[MARIA] >= 150 && npcAffection[POPURI] >= 150 && gWeather == SUNNY && gSeason == SPRING && 5 < gHour && gHour < 18) {

        if (previousMapIndex == ROAD) {
            
            gCutsceneIndex = 412;
            setLifeEventBit(0xAE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
        if (previousMapIndex == VILLAGE_2) {

            if (gEntranceIndex == 0x33) {
                gCutsceneIndex = 413;
            } else {
                gCutsceneIndex = 414;
            }

            setLifeEventBit(0xAE);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
    } 
    
    if (!set && !checkLifeEventBit(0xAF) && npcAffection[GRAY] >= 120 && gWeather == SUNNY && gSeason < AUTUMN && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && !checkLifeEventBit(0x1B)) {
        setLifeEventBit(0xAF);
        gCutsceneIndex = 415;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    } 

    if (!set && !checkLifeEventBit(0x31) && npcAffection[ELLEN] >= 30 && gWeather == SUNNY && gYear == 2 && gSeason == SPRING && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 416;
        gCutsceneFlags |= (2 | 4);
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

    if (!checkLifeEventBit(0xB6) && (npcAffection[MAYOR] + npcAffection[MAYOR_WIFE] >= 200) && npcAffection[MARIA] >= 150 && 5 < gHour && gHour < 18) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(0xB6);
            gCutsceneIndex = 505;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 506;
            } else {
                gCutsceneIndex = 507;
            }

            setLifeEventBit(0xB6);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
        
    }

    if (!set && !checkLifeEventBit(0xB7) && npcAffection[HARRIS] >= 160 && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(0x1A)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(0xB7);
            gCutsceneIndex = 508;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 509;
            } else {
                gCutsceneIndex = 510;
            }

            setLifeEventBit(0xB7);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xB8) && npcAffection[HARRIS] >= 180 && gSeason < AUTUMN && gWeather == SUNNY && 5 < gHour && gHour < 18 && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && !checkLifeEventBit(0x1A)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(0xB8);
            gCutsceneIndex = 511;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 512;
            } else {
                gCutsceneIndex = 513;
            }

            setLifeEventBit(0xB8);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xB9) && npcAffection[MARIA] >= 150 && npcAffection[ANN] >= 150 && gSeason == AUTUMN && 5 < gHour && gHour < 18) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(0xB9);
            gCutsceneIndex = 514;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 515;
            } else {
                gCutsceneIndex = 516;
            }

            setLifeEventBit(0xB9);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xBA) && checkLifeEventBit(0x65) && gWeather == SUNNY && 5 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        if (previousMapIndex == ROAD) {

            setLifeEventBit(0xBA);
            gCutsceneIndex = 518;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }

        if (previousMapIndex == VILLAGE_1) {

            if (gEntranceIndex == 0x3F) {
                gCutsceneIndex = 519;
            } else {
                gCutsceneIndex = 520;
            }

            setLifeEventBit(0xBA);
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
            
        }
    
    }

    if (!set && !checkLifeEventBit(0xD0) && checkLifeEventBit(0x1F) && gWife == MARIA && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD0);
        gCutsceneIndex = 500;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD1) && checkLifeEventBit(0x1F) && gWife == POPURI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD1);
        gCutsceneIndex = 501;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD2) && checkLifeEventBit(0x1F) && gWife == ELLI && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD2);
        gCutsceneIndex = 502;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD3) && checkLifeEventBit(0x1F) && gWife == ANN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD3);
        gCutsceneIndex = 503;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xD4) && checkLifeEventBit(0x1F) && gWife == KAREN && npcAffection[MIDWIFE] >= 80 && gWifePregnancyCounter == 9 && gWeather == SUNNY && 5 < gHour && gHour < 18) {

        setLifeEventBit(0xD4);
        gCutsceneIndex = 504;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0x9A) && !checkLifeEventBit(MARRIED) && npcAffection[MARIA] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && previousMapIndex == LIBRARY) {
        setLifeEventBit(0x9A);
        gCutsceneIndex = 517;
        gCutsceneFlags |= (2 | 4);
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
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
    }
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setFlowerShopCutscenes);

u16 setFlowerShopCutscenes(void) {
    
    u16 bytecodeSegmentIndex = 0xFFFF;
    
    if (!checkLifeEventBit(POPURI_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[POPURI] >= 120 && (2 <= gWeather && gWeather < 4) && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED)) {
        setLifeEventBit(POPURI_SICK_CUTSCENE);
        setDailyEventBit(0x57);
        gCutsceneIndex = POPURI_SICK;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }
    
    if (checkLifeEventBit(MARRIED) && gWife == POPURI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 444;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }
    
    return bytecodeSegmentIndex;

}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setBakeryCutscenes);

u16 setBakeryCutscenes(void) {

    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        if (!checkLifeEventBit(0x31)) {
            gCutsceneIndex = 418;
        } else {
            gCutsceneIndex = 420;
        }

        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkDailyEventBit(0x1A) && gSeason == WINTER && gDayOfMonth == 10 && 5 < gHour && gHour < 18 && !checkLifeEventBit(MARRIED)) {

        setDailyEventBit(0x1A);
        gCutsceneIndex = 427;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkLifeEventBit(0xB2) && npcAffection[ELLI] >= 150 && npcAffection[ANN] >= 150 && 5 < gHour && gHour < 18 && (gSeason == AUTUMN && gDayOfMonth >= 15 || gSeason == WINTER && gDayOfMonth < 10)) {

        setLifeEventBit(0xB2);
        gCutsceneIndex = 432;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
        
    }

    if (!set && !checkLifeEventBit(0xB3) && !checkDailyEventBit(0x30) && npcAffection[JEFF] >= 150 && 5 < gHour && gHour < 18  && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && !checkLifeEventBit(0x1C) && previousMapIndex == VILLAGE_1) {

        setLifeEventBit(0xB3);
        
        if (gEntranceIndex == 0x59) {
            gCutsceneIndex = 433;
        } else {
            gCutsceneIndex = 434;
        }
        
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }
    
    if (!set && !checkLifeEventBit(0x97) && !checkDailyEventBit(0x30) && npcAffection[ELLI] >= 120 && (SUNNY < gWeather && gWeather < 4) && 5 < gHour && gHour < 18  && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        setLifeEventBit(0x97);
        setDailyEventBit(0x58);
        gCutsceneIndex = 442;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        
    }

    if (!set && !checkLifeEventBit(0x99) && !checkLifeEventBit(MARRIED) && npcAffection[ELLI] >= 250 && gWeather == SUNNY && 11 < gHour && gHour < 17 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1C)) {

        setLifeEventBit(0x99);
        gCutsceneIndex = 426;
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        
    }

    if (checkLifeEventBit(MARRIED) && gWife == ELLI && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = 445;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }

    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setMayorHouseCutscenes);

u16 setMayorHouseCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
        setDailyEventBit(MARIA_MARRIAGE_CUTSCENE_DAILY);
        gCutsceneIndex = THANKSGIVING_MAYOR_HOUSE;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(MARIA_SICK_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[MARIA] >= 120 && (SUNNY < gWeather && gWeather < 4) && gDayOfWeek == MONDAY && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SICK_CUTSCENE);
        setDailyEventBit(MARIA_ABSENT);
        gCutsceneIndex = MARIA_SICK;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }

    if (checkLifeEventBit(MARRIED) && gWife == MARIA && checkLifeEventBit(WIFE_LEFT)) {
        gCutsceneIndex = MARIA_REPAIR_MARRIAGE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }

    return bytecodeSegmentIndex;

}    

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setLibraryCutscenes);

u16 setLibraryCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (!checkLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[MARIA] >= 180 && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
        setLifeEventBit(MARIA_SPRAINED_ANKLE_CUTSCENE);
        setDailyEventBit(MARIA_ABSENT);
        gCutsceneIndex = MARIA_SPRAINED_ANKLE;
        set = TRUE;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (!checkLifeEventBit(0xBD) && checkLifeEventBit(0x63) && (8 < gHour && gHour < 17) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED)) {
            setLifeEventBit(0xBD);
            gCutsceneIndex = 527;
            loadCutscene(FALSE);
            gCutsceneFlags |= (2 | 4);
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
        gCutsceneFlags |= (2 | 4);
    } 
    
    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 30 && NIGHTTIME) {
            gCutsceneIndex = CHURCH_NEW_YEARS;
            loadCutscene(FALSE);
            gCutsceneFlags |= (2 | 4);
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
        gCutsceneFlags |= (2 | 4);
    }

    if (!set && !checkLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE) && npcAffection[KAREN] >= 80 && npcAffection[KAI] >= 80 && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
        setLifeEventBit(KAREN_KAI_TAVERN_CUTSCENE);
        setSpecialDialogueBit(KAREN_KAI_TAVERN_CUTSCENE_DIALOGUE);
        gCutsceneIndex = KAREN_KAI_TAVERN;
        gCutsceneFlags |= (2 | 4);
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
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 23 && 5 < gHour && gHour < 18) {

        gCutsceneIndex = 950;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == SUMMER && gDayOfMonth == 1 && 17 < gHour && gHour < 24 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(0x1A)) {

        gCutsceneIndex = 600;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == AUTUMN && gDayOfMonth == 12 && 5 < gHour && gHour < 18) {

        setDailyEventBit(0x5D);
        setHarvestKingName(gHarvestCoinFinder);
        
        gCutsceneIndex = 1200;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 27 && 17 < gHour && gHour < 24) {

        if (gHarvestKing == PLAYER) {
            gCutsceneIndex = 1350;
        } else {
            if (!checkDailyEventBit(0x50)) {
                gCutsceneIndex = 1351;
            } else {
                gCutsceneIndex = 1352;
            }
        }
    
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
        
    }


    if (!set && gSeason == SPRING && gDayOfMonth == 1 && 5 < gHour && gHour < 18) {
        gCutsceneIndex = 1400;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }

    if (!set && gSeason == SPRING && gDayOfMonth == 17 && 7 < gHour && gHour < 18) {
        updateHorseRaceContext();
        gCutsceneIndex = 900;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }
    
    if (!set && gSeason == AUTUMN && gDayOfMonth == 28 && 7 < gHour && gHour < 18) {
        updateHorseRaceContext();
        gCutsceneIndex = 900;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
        set = TRUE;
    }

    if (!set && gSeason == WINTER && gDayOfMonth == 19 && 7 < gHour && gHour < 18) {
        updateDogRaceContext();
        gCutsceneIndex = 1300;
        loadCutscene(FALSE);
        gCutsceneFlags |= (2 | 4);
    }
    
    return bytecodeSegmentIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRaceTrackCutscenes);

u16 setRaceTrackCutscenes(void) {

    u16 bytecodeSegmentIndex = 0xFFFF;
    bool set = FALSE;

    if (checkDailyEventBit(0x40) && checkDailyEventBit(0x41)) {
        updateHorseRaceContext();
        toggleDailyEventBit(0x40);
        gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (checkDailyEventBit(0x40) && checkDailyEventBit(0x42)) {
            updateDogRaceContext();
            toggleDailyEventBit(0x40);
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (gSeason == SPRING && gDayOfMonth == 17 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) { 
        if (gSeason == AUTUMN && gDayOfMonth == 28 && (7 < gHour && gHour < 18)) {
            updateHorseRaceContext();
            gCutsceneIndex = HORSE_RACE_STARTING_ANNOUNCEMENT;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (gSeason == WINTER && gDayOfMonth == 19 && (7 < gHour && gHour < 18)) {
            updateDogRaceContext();
            gCutsceneIndex = DOG_RACE_AFTER_RACE;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
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
        gCutsceneFlags |= (2 | 4);
    }

    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 24 && (5 < gHour && gHour < 18)) {
            gCutsceneIndex = SEA_FESTIVAL;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) {
        if (gSeason == SUMMER && gDayOfMonth == 1 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = BEACH_FIREWORKS;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }
    
    if (!set) {
        if (!checkDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY) && gSeason == WINTER && gDayOfMonth == 10 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED)) {
            setDailyEventBit(KAREN_MARRIAGE_CUTSCENE_DAILY);
            gCutsceneIndex = 753;
            loadCutscene(FALSE);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
        }
    }

    if (!set) {
        if (gSeason == WINTER && gDayOfMonth == 24 && NIGHTTIME && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 756;
            loadCutscene(FALSE);
            gCutsceneFlags |= (2 | 4);
            set = TRUE;
        }
    }
    
    if (!set) {
        if (!checkLifeEventBit(0xCA) && npcAffection[KAREN] < 101 && (5 < gHour && gHour < 18) && (gYear >= 3 || (gYear == 2 && gSeason == WINTER)) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED) && gFlowerFestivalGoddess != 4) {
            setLifeEventBit(0xCA);
            setLifeEventBit(0x2F);
            setSpecialDialogueBit(0x23);
            gCutsceneIndex = 752;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE) && npcAffection[ELLI] >= 150 && npcAffection[KAREN] >= 150 && gWeather == SUNNY && gSeason == SUMMER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(KAREN_AND_ELLI_BEACH_CUTSCENE);
            gCutsceneIndex = KAREN_AND_ELLI;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9C) && !checkLifeEventBit(MARRIED) && npcAffection[KAREN] >= 250 && gWeather == SUNNY && (14 < gHour && gHour < 24) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9C);
            gCutsceneIndex = KAREN_PINK_HEART_EVENT;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);     
            set = TRUE;
        }
    }

    if (!set) {
        if (!checkLifeEventBit(0x9D) && checkLifeEventBit(0x20) && npcAffection[KAI] >= 180 && gWeather == SUNNY && NIGHTTIME && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            setLifeEventBit(0x9D);
            gCutsceneIndex = 751;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
        }
    }
    
    return bytecodeSegmentIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/cutscenes", setRoadCutscenes);

u16 setRoadCutscenes(void) {
    
    bool set = FALSE;
    u16 bytecodeSegmentIndex = 0xFFFF;

    if (!checkLifeEventBit(0xCC) && npcAffection[CLIFF] >= 200 && gSeason == WINTER && (5 < gHour && gHour < 18) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
        setLifeEventBit(0xCC);
        gCutsceneIndex = 800;
        gCutsceneFlags |= (2 | 4);
        loadCutscene(FALSE);
        set = TRUE;
    }

    if (!set) {
        if (!checkLifeEventBit(0xCD) && bakeryCardPoints >= 6 && npcAffection[ANN] >= 150 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == RANCH) {
            setLifeEventBit(0xCD);
            gCutsceneIndex = 801;
            gCutsceneFlags |= (2 | 4);
            loadCutscene(FALSE);
            set = TRUE;
        }    
    }

    if (!set) {
        if (!checkLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE) && checkLifeEventBit(0xB6) && (5 < gHour && gHour < 18)) {

            if (previousMapIndex == VILLAGE_1) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_1;
                gCutsceneFlags |= (2 | 4);
                loadCutscene(FALSE);
                set = TRUE;
            }

            if (previousMapIndex == VILLAGE_2) {
                setLifeEventBit(SHADY_SALESMAN_MARIA_CUTSCENE);
                gCutsceneIndex = SHADY_SALESMAN_AND_MARIA_2;
                gCutsceneFlags |= (2 | 4);
                loadCutscene(FALSE);
                set = TRUE;
            }
        }
    }

    if (!set) {
        if (!checkLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2) && npcAffection[ANN] >= 180 && (5 < gHour && gHour < 18) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && previousMapIndex == FARM) {
            setLifeEventBit(ANN_SPRAINED_ANKLE_CUTSCENE);
            setDailyEventBit(0x59);
            gCutsceneIndex = ANN_SPRAINED_ANKLE;
            loadCutscene(FALSE);
            gCutsceneFlags |= (2 | 4); 
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
            
            if (!checkDailyEventBit(0x1D) && !checkDailyEventBit(0x1E) && gSeason == AUTUMN && gDayOfMonth == 3 && (5 < gHour && gHour < 15) && getTotalFarmAnimalsByType(2)) {
               
                setDailyEventBit(0x1D);
                
                if (gBaseMapIndex == FARM) {
                    bytecodeSegmentIndex = 33;
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
        
        if (bytecodeSegmentIndex != 0xFFFF) {
            initializeCutscene(bytecodeSegmentIndex);
        }
    }

    return bytecodeSegmentIndex;
    
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
            loadCutscene(FALSE);
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
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);

        }

        // rivals' weddings
        if (!checkDailyEventBit(FESTIVAL_DAY_TYPE_2)) { 
            
            if (!set && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != MARIA) && npcAffection[HARRIS] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[MARIA] < 120))  {
                
                setLifeEventBit(MARIA_HARRIS_MARRIED);
                setSpecialDialogueBit(0xB);
                gCutsceneIndex = 203;
                setEntrance(0);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);
                
            }

            if (!set && !checkLifeEventBit(POPURI_GRAY_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != POPURI) && npcAffection[GRAY] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[POPURI] < 120)) {
               
                setLifeEventBit(POPURI_GRAY_MARRIED);
                setSpecialDialogueBit(0xC);
                gCutsceneIndex = 206;
                setEntrance(0);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }

            if (!set && !checkLifeEventBit(ELLI_JEFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ELLI) && npcAffection[JEFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ELLI] < 120)) {
                
                setLifeEventBit(ELLI_JEFF_MARRIED);
                setSpecialDialogueBit(0xD);
                gCutsceneIndex = 208;
                setEntrance(0);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }

            if (!set && !checkLifeEventBit(ANN_CLIFF_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != ANN) && npcAffection[CLIFF] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[ANN] < 120)) {
                
                setLifeEventBit(ANN_CLIFF_MARRIED);
                setSpecialDialogueBit(0xE);
                gCutsceneIndex = 214;
                setEntrance(0);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }
            
            if (!set && !checkLifeEventBit(KAREN_KAI_MARRIED) && (!checkLifeEventBit(MARRIED) || gWife != KAREN) && npcAffection[KAI] >= 200 && (checkLifeEventBit(MARRIED) || npcAffection[KAREN] < 120) && checkLifeEventBit(0x96) && !checkLifeEventBit(KAI_GONE) && !checkLifeEventBit(KAREN_GONE)) {
                
                setLifeEventBit(KAREN_KAI_MARRIED);
                setSpecialDialogueBit(0xF);
                gCutsceneIndex = 217;
                setEntrance(0);
                loadCutscene(FALSE);
                set = TRUE;
                gCutsceneFlags |= (2 | 4);

            }
            
        }

        // animal funeral
        if (!set && checkLifeEventBit(ANIMAL_DIED) && !checkDailyEventBit(FESTIVAL_DAY_TYPE_2)) {
            
            toggleLifeEventBit(ANIMAL_DIED);
            setLifeEventBit(0x50);
            setEntrance(0x3E);
            gCutsceneIndex = 523;
            loadCutscene(FALSE);
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
            gCutsceneFlags |= 2;
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

    if (!checkLifeEventBit(0x96) && checkSpecialDialogueBit(0x14B) && gSeason == AUTUMN && gDayOfMonth == 6) {
        gCutsceneIndex = 40;
        loadCutscene(FALSE);
        set = TRUE;
        gCutsceneFlags |= 2;
    }

    if (!set) {
        if (!checkLifeEventBit(0x63) && npcAffection[MARIA] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 41;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x64) && npcAffection[POPURI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 42;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }
    
    if (!set) { 
        if (!checkLifeEventBit(0x65) && npcAffection[ELLI] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 43;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x66) && npcAffection[ANN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 44;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0x67) && npcAffection[KAREN] >= 160 && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(MARIA_HARRIS_MARRIED) && !checkLifeEventBit(POPURI_GRAY_MARRIED) && !checkLifeEventBit(ELLI_JEFF_MARRIED) && !checkLifeEventBit(ANN_CLIFF_MARRIED) && !checkLifeEventBit(KAREN_KAI_MARRIED)) {
            gCutsceneIndex = 45;
            loadCutscene(FALSE);
            set = TRUE;
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == MARIA && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 9;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == POPURI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 16;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ELLI && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 23;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == ANN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 30;
            loadCutscene(FALSE);
            set = TRUE;
            gCutsceneFlags |= (2 | 4);
        }
    }

    if (!set) { 
        if (!checkLifeEventBit(0xD8) && checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY) && !checkDailyEventBit(BIRTH_EVENT_DAILY) && gWife == KAREN && (29 < gBabyAge && gBabyAge < 120) && !getRandomNumberInRange(0, 10)) {
            setLifeEventBit(0xD8);
            gCutsceneIndex = 37;
            loadCutscene(FALSE);
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

                        case 170:
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            handleHatchChicken();
                            break;    
                        
                        case 113:
                            setEntrance(0);
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

                case 9:
                    
                    switch (gCutsceneIndex) {           

                        case 1:                           
                        case 2:                           
                            setLifeEventBit(0x61);
                            break;
                        case 38:                          
                        case 39:                          
                            setLifeEventBit(0x62);
                            break;
                        case 40:                          
                            setLifeEventBit(0x96);
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
                        case 41:                          
                            setLifeEventBit(0x63);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        case 42:                          
                            setLifeEventBit(0x64);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 43:                          
                            setLifeEventBit(0x65);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 44:                          
                            setLifeEventBit(0x66);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 45:                          
                            setLifeEventBit(0x67);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);                             
                            break;
                        case 4:                           
                        case 11:                           
                        case 18:                          
                        case 25:                          
                        case 32:                          
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

                        case 302:
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

                        case MOUNTAIN_FIREWORKS:
                        case STARRY_NIGHT_MOON_MOUNTAIN:
                            setMainLoopCallbackFunctionIndex(END_OF_FESTIVAL_DAY_1);
                            break;

                        case 318:                         
                            storeTool(FISHING_POLE);
                            break;
                        
                        case 324:
                        case 328:
                            setDailyEventBit(0x2C);
                            break;

                        case 327:
                            if (gSeason == AUTUMN && gDayOfMonth == 27) {
                                setLifeEventBit(BRIDGE_COMPLETED);
                            }
                            break;

                        case 330:                         
                            if (gSeason == WINTER && gDayOfMonth == 16) {
                                setLifeEventBit(HOT_SPRINGS_COMPLETED);
                                setDailyEventBit(0x5B);
                            }
                            clearForagableObjects(TOP_OF_MOUNTAIN_1);
                            break;
                        
                        case 336:                         
                            toggleDailyEventBit(KAPPA_FISH_OFFERING_DAILY);
                            setLifeEventBit(MET_KAPPA);
                            break;
                        
                        case 337:                         
                            toggleDailyEventBit(KAPPA_LARGE_FISH_OFFERING_DAILY);
                            setLifeEventBit(KAPPA_POWER_NUT);
                            break;
                        
                        case 341:                         
                        case 344:                         
                            toggleDailyEventBit(HARVEST_GODDESS_OFFERING);
                            setDailyEventBit(HARVEST_GODDESS_INTERACTION);
                            break;
                        
                        case 345:                         
                            setSpecialDialogueBit(0x14B);
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
                            gNamingScreenIndex = 3;
                            handleExitLevel(1, NAMING_SCREEN_LOAD);
                            setLifeEventBit(HAVE_HORSE);
                            break;
                        case 651:                         
                            loadLevel(0);
                            setMainLoopCallbackFunctionIndex(SET_AUDIO_AND_LIGHTING);
                            initializeHorse();
                            break;
                        case 663:                         
                            setEntrance(0x4C);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                        case 664:                         
                            npcAffection[ANN] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                        }

                    break;

                case 13:
                    
                    switch (gCutsceneIndex) {          

                        case 701:                         
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setEntrance(0x2F);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;
                        
                        case 704:                         
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
                        
                        case 710:                         
                            npcAffection[KAREN] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
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
                            setEntrance(0x1F);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            blueMistSeedsQuantity += adjustValue(blueMistSeedsQuantity, -1, 1);
                            removeTool(BLUE_MIST_SEEDS);
                            blueMistFlowerPlot = 0xB3;
                            pondFieldTiles[5][3] = 0xB3;
                            break;
                            
                        case 416:                        
                            setLifeEventBit(0x31);
                            setSpecialDialogueBit(0x43);
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
                            // flower shop entrance
                            setEntrance(0x57);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);               
                            break;
                
                        case 438:                        
                            setEntrance(0x5A);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);               
                            break;
                
                        case 439:                        
                            npcAffection[POPURI] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
                            setMainLoopCallbackFunctionIndex(END_OF_DAY_1);
                            break;
                
                        case 440:                        
                            npcAffection[ELLI] = 120;
                            toggleLifeEventBit(WIFE_LEFT);
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
                            setEntrance(0x66);
                            setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            break;
                            
                        case 526:                         
                            if (gCutsceneCompletionFlags & 0x40000000) {
                                gHour += 2;
                                setEntrance(0x41);
                                setMainLoopCallbackFunctionIndex(MAP_LOAD);
                            }
                            break;

                        case 529:                         
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
                        setMessageBoxFont(0, 0xE, 0xE, FONT_TEXTURE_BUFFER, FONT_PALETTE_1_BUFFER);
                        setMessageBoxInterpolationWithFlags(0, -4, 0);
                        setMessageBoxSfx(0, 0x57, 8, 1);
 
                        if (D_80180710 == 0) {
                            // show pink overlay message
                            showPinkOverlayText(23);
                        } else {
                            launchIntroCutscene(FUNERAL, 0x61, 1);

                        }
                    }

                    break;
                
                case 32:
                    if (gCutsceneIndex == DEMO_CUTSCENE_2) {
                        launchIntroCutscene(1502, 0x1A, 1);
                    } else {
                        memcpy(farmFieldTiles, D_80113580, FIELD_HEIGHT * FIELD_WIDTH);
                        launchIntroCutscene(FUNERAL, 0x61, 1);
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
                        transitionCutscenes(2, 0x3E);
                        break;

                    case 38:         
                        transitionCutscenes(39, 0);
                        gCutsceneFlags &= ~0x10;
                        gCutsceneFlags |= 8;
                        break;
                    
                    case 147:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(250, SPRING_ENTER);
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
                        transitionCutscenes(251, 0x1A);
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
                        transitionCutscenes(252, 0x25);
                        break;
                
                    case 158:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(253, SPRING_ENTER);
                        break;

                    case 169:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(254, SPRING_ENTER);
                        break;
                    
                    case 181:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(255, 0x48);
                        break;
                    
                    case 193:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(256, 0x2E);
                        break;
                    
                    case 203:          
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(405, 0x32);
                        break;

                    case 206:
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(406, 0x32); 
                        break;
                    
                    case 208:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(407, 0x32);
                        break;

                    case 214:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(408, 0x32);
                        break;
                    
                    case 217:      
                        deactivateSprites();
                        deactivateGlobalSprites();
                        initializeCutsceneExecutors();
                        deactivateNPCEntities();
                        deactivateAnimalEntities();
                        clearAllItemContextSlots();
                        initializeEntityInstances(1);
                        transitionCutscenes(409, 0x32);
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
                        transitionCutscenes(STARRY_NIGHT_FESTIVAL, 0x5F);
                        break;
                    
                    case 245:      
                        transitionCutscenes(305, 0x25);
                        break;
                    
                    case 248:      
                        transitionCutscenes(756, 0x27);
                        break;
                    
                    case 324:
                        transitionCutscenes(326, 0x23);
                        break;

                    case 326:     
                        transitionCutscenes(327, 0x1A);
                        break;
                    
                    case 328:     
                        transitionCutscenes(329, 0x23);
                        break;
                    
                    case 329:     
                        transitionCutscenes(330, 0x1A);
                        break;
                    
                    case 344:     
                        transitionCutscenes(345, 0x2E);
                        break;

                    case 346:     
                        // set cutscene wild card/score variables
                        D_801886D2 = getRandomNumberInRange(0, 9);
                        D_80180710 = getRandomNumberInRange(0, 9);
                        clearForagableObjects(MINE_2);
                        transitionCutscenes(347, 0x71);
                        break;

                    case 347:
                    case 348:
                        
                        if (gHour >= 17 || gPlayer.currentStamina < 2) {
                            gCutsceneIndex = 349;
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

                            gCutsceneIndex = 348;
                            tempEntrance = 0x71;
                            
                            clearForagableObjects(MINE_2);
                 
                        }
                        
                        setEntrance(tempEntrance);
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(tempEntrance);

                        break;

                    case 410:     
                        transitionCutscenes(411, SPRING_ENTER);
                        break;
                    
                    case 426:     
                        transitionCutscenes(443, 0x32);
                        break;

                    case STARRY_NIGHT_FESTIVAL:

                        setEntrance(0x61);
    
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
                        loadLevelFromEntrance(0x61);
                        break;
                    
                    case 441:     
                    case 444:
                        setEntrance(0x58);
                        if (gCutsceneIndex == 441) {
                            gCutsceneIndex = 437;
                        } else {
                            gCutsceneIndex = 439;
                        }
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x58);
                        break;
                    
                    case 442:     
                    case 445:     
                        setEntrance(0x5C);
                        if (gCutsceneIndex == 442) {
                            gCutsceneIndex = 438;
                        } else {    
                            gCutsceneIndex = 440;
                        }
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x5C);
                        break;

                    case 525:
                        setEntrance(0x3E);
                        gCutsceneIndex = 526;
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x3E);
                        break;
                    
                    case 526:     
                        setEntrance(0x41);
                        loadLevelFromEntrance(0x41);
                        break;
                    
                    case 527:
                        transitionCutscenes(528, 0x3E);
                        break;   

                    case 530:
                    case 531:
                        setEntrance(0x67);
                        if (gCutsceneIndex == 530) {
                            gCutsceneIndex = 524;
                        } else {
                            gCutsceneIndex = 529;
                        }
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x67);
                        break;

                    case ANN_SICK_VISIT_CUTSCENE:
                    case 667:
                        setEntrance(0x4D);
                        if (gCutsceneIndex == ANN_SICK_VISIT_CUTSCENE) {
                            gCutsceneIndex = 663;
                        } else {
                            gCutsceneIndex = 664;
                        }
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x4D);
                        break;
                    
                    case KAREN_YELLOW_HEART_EVENT_1:
                        gCutsceneIndex = 706;
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x55);
                        break;

                    case 706:
                        transitionCutscenes(707, 0x53);
                        break;

                    case 707:
                    case 708:
                        toggleDailyEventBit(0x4F);
                        transitionCutscenes(709, 0x2E);
                        break;
                    
                    case 711:
                    case 712:
                        setEntrance(0x52);
                        if (gCutsceneIndex == 711) {
                            gCutsceneIndex = 704;
                        } else {
                            gCutsceneIndex = 710;
                        }
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x52);
                        break;
                    
                    case ANN_SPRAINED_ANKLE:
                        setEntrance(0x2B);
                        loadLevelFromEntrance(0x2B);
                        break;
                    
                    case 900:
                    case HORSE_RACE_STARTING_ANNOUNCEMENT:
                         
                        if (gHour >= 17) {
                            transitionCutscenes(904, 0x61);
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
                        transitionCutscenes(VEGETABLE_FESTIVAL_SQUARE, 0x61);
                        break;

                    case 1300:
                    case 1301:
                        if (gHour >= 17) {
                            transitionCutscenes(1303, 0x61);
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
                        setupPlayerEntity(0x6C, 1);
                        handlePlayerAnimation();
                        setEntitiesRGBA(0, 0, 0, 0);
                        setEntrance(0x6C);
                        gCutsceneIndex = 1301;
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(0x6C);
                        break;

                    case FUNERAL:             
                        transitionCutscenes(1451, 0);
                        break;

                    case 1451:             
                        transitionCutscenes(1452, 0x14);
                        break;

                    case MAYOR_VILLAGE_TOUR:
                        transitionCutscenes(1454, 0x3E);
                        break;
                    
                    case 1454:            
                        transitionCutscenes(1455, 0);
                        gCutsceneFlags &= ~0x10;
                        gCutsceneFlags |= 8;
                        break;

                    case 1501:
                        break;

                    case EVALUATION:             
                        transitionCutscenes(1551, 0x1A);
                        break;

                    case 1551:             
                        transitionCutscenes(1552, 0xA);
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
                            tempEntrance = 0;
                            gCutsceneIndex = 1601;
                            
                            
                        } else if (D_80189824 == 1) {

                            gSeason = SPRING;

                            tempEntrance = 0;
                            
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
                            tempEntrance = 0;
                            
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
                                    tempEntrance = 0x11;
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

                        setEntrance(tempEntrance);
                        loadCutscene(FALSE);
                        loadLevelFromEntrance(tempEntrance);

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
                    previousLightingRGBA.a = 0.0f;
                    previousLightingRGBA.b = 0.0f;
                    previousLightingRGBA.g = 0.0f;
                    previousLightingRGBA.r = 0.0f;
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

                            if (!(acquiredPowerNutBits & 0x20) && gItemBeingHeld == 0xFF && !(getRandomNumberInRange(0, 100))) {
                                gItemBeingHeld = 254;
                                acquiredPowerNutBits |= 0x20;
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
                    turnipSeedsQuantity += adjustValue(turnipSeedsQuantity, 1, 20);
                    return;
                case 1:                                 
                    storeTool(0xB);
                    potatoSeedsQuantity += adjustValue(potatoSeedsQuantity, 1, 20);
                    return;
                case 2:                                 
                    storeTool(0xC);
                    cabbageSeedsQuantity += adjustValue(cabbageSeedsQuantity, 1, 20);
                    return;
                case 3:                                 
                    storeTool(0xD);
                    tomatoSeedsQuantity += adjustValue(tomatoSeedsQuantity, 1, 20);
                    return;
                case 4:                                 
                    storeTool(0xE);
                    cornSeedsQuantity += adjustValue(cornSeedsQuantity, 1, 20);
                    return;
                case 5:                                 
                    storeTool(0xF);
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
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        bytecodeSegmentIndex = 35;
        
    } else if (gCutsceneIndex >= 1550) {

        // evaluation/credits
    
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        bytecodeSegmentIndex = 31;        
    
    } else if (gCutsceneIndex >= 1500) {

        // demos
             
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);

        memcpy(farmFieldTiles, D_80114E50, FIELD_HEIGHT * FIELD_WIDTH);

        bytecodeSegmentIndex = 32;
        
    } else if (gCutsceneIndex >= 1450) {

        // opening
        // mayor tour
        
        gCameraRotationOffset = 0;
        setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        
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
                    setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
                    break;
                case 228 ... 238:
                    gCameraRotationOffset = 0;
                    setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
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
            setInitialMapRotation(MAIN_MAP_INDEX, SOUTH);
        } 
        
    }

    initializeCutscene(bytecodeSegmentIndex);
    
    switch (gCutsceneIndex) {
        case STARRY_NIGHT_MOON_MOUNTAIN:
        case MARIA_WEDDING ... KAREN_WEDDING:
        case STARRY_NIGHT_FESTIVAL:
        case 523:
        case 756:
        case SOWING_FESTIVAL_POTION_SHOP_DEALER ... SOWING_FESTIVAL_HARRIS:
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
            setDailyEventBit(0x4B);
            break;
    }
    
}