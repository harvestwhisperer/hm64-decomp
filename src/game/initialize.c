#include "common.h"

#include "game/initialize.h"

#include "ld_symbols.h"

#include "data/animationScripts/animationScripts.h"
#include "data/audio/sfx.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/memory.h"
#include "system/message.h"

#include "game/animals.h" 
#include "game/cutscenes.h"
#include "game/fieldObjects.h"
#include "data/fieldTileMaps/fieldTiles.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameFile.h"
#include "game/gameStart.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h" 
#include "game/fieldObjects.h"
#include "game/namingScreen.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/title.h"
#include "game/transition.h"
#include "game/tv.h"
#include "game/time.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "buffers/buffers.h"

#include "assetIndices/dialogues.h"
#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sequences.h"
#include "assetIndices/sprites.h"
#include "assetIndices/texts.h"

// forward declarations
void initializeEntityAssets(void);
void initializeDialogueSystem(void);
void setDialogueBytecodeAddresses(void);
void initializeGameVariables(void);
void initializeGameVariableStrings(void);
void loadMapAddresses(void);
void registerMainLoopCallbacks(void);
void initializeDialogueVariables(void);
void initializeTextAddresses(void);
void initializeMainMessageBoxes(void);

static inline void setStartingTime() {
    gYear = 1;
    gDayOfWeek = 3;
    gSeason = 1;
    gDayOfMonth = 3;
    gHour = 6;
    gMinutes = 0;
    gSeconds = 0;
}

static inline void initializeTV() {

    tvContext.channelIndex = 0;
    tvContext.mode = 0;
    tvContext.varietyShowEpisodeCounters[0] = 1;
    tvContext.varietyShowEpisodeCounters[1] = 1;
    tvContext.varietyShowEpisodeCounters[2] = 0;
    tvContext.varietyShowEpisodeCounters[3] = 0;
    tvContext.varietyShowEpisodeCounters[4] = 0;
    tvContext.varietyShowEpisodeCounters[5] = 0;
    tvContext.varietyShowEpisodeCounters[6] = 0;
    
} 

static inline void initializePlayer() {

    gPlayer.fatigueCounter = 0;
    gPlayer.staminaExhaustionLevel = 0;
    gPlayer.fatigueThreshold= 0;
    gPlayer.shopItemIndex = 0;
    gPlayer.direction = SOUTHWEST;
    gPlayer.groundObjectIndex = 0;
    gPlayer.actionHandler = 0;
    gPlayer.animationHandler = 0;
    gPlayer.actionPhaseFrameCounter = 0;
    gPlayer.actionPhase = 0;
    gPlayer.fishingSpotType = 0;
    gPlayer.flags = 0;
    gPlayer.velocity = 0.0f;
    gPlayer.toolHeldCounter = 0;
    gPlayer.currentToolLevel = 0;
    gPlayer.bottleContents = 0;
    gPlayer.toolLevels[0] = 0;
    gPlayer.toolSlots[0] = 2;
    gPlayer.toolSlots[3] = 4;
    gPlayer.toolLevels[1] = 0;
    gPlayer.toolLevels[2] = 0;
    gPlayer.toolLevels[3] = 0;
    gPlayer.toolLevels[4] = 0;
    
    gPlayer.toolUseCounters[0] = 0;
    gPlayer.toolUseCounters[1] = 0;
    gPlayer.toolUseCounters[2] = 0;
    gPlayer.toolUseCounters[3] = 0;
    gPlayer.toolUseCounters[4] = 0;
    gPlayer.toolSlots[1] = 1;
    gPlayer.toolSlots[2] = 3;
    gPlayer.toolSlots[4] = 5;
    gPlayer.toolSlots[5] = 0;
    gPlayer.toolSlots[6] = 0;
    gPlayer.toolSlots[7] = 0;
    gPlayer.currentTool = 0;
    
    gPlayer.belongingsSlots[0] = 0;
    gPlayer.belongingsSlots[1] = 0;
    gPlayer.belongingsSlots[2] = 0;
    gPlayer.belongingsSlots[3] = 0;
    gPlayer.belongingsSlots[4] = 0;
    gPlayer.belongingsSlots[5] = 0;
    gPlayer.belongingsSlots[6] = 0;
    gPlayer.belongingsSlots[7] = 0;

    gPlayer.heldItem = 0;

    gPlayer.keyItemSlots[0] = 0;
    gPlayer.keyItemSlots[1] = 0;
    gPlayer.keyItemSlots[2] = 0;
    gPlayer.keyItemSlots[3] = 0;
    gPlayer.keyItemSlots[4] = 0;
    gPlayer.keyItemSlots[5] = 0;
    gPlayer.keyItemSlots[6] = 0;
    gPlayer.keyItemSlots[7] = 0;
    gPlayer.keyItemSlots[8] = 0;
    gPlayer.keyItemSlots[9] = 0;
    gPlayer.keyItemSlots[10] = 0;
    gPlayer.keyItemSlots[11] = 0;
    gPlayer.keyItemSlots[12] = 0;
    gPlayer.keyItemSlots[13] = 0;
    gPlayer.keyItemSlots[14] = 0;
    gPlayer.keyItemSlots[15] = 0;
    gPlayer.keyItemSlots[16] = 0;
    gPlayer.keyItemSlots[17] = 0;
    gPlayer.keyItemSlots[18] = 0;
    gPlayer.keyItemSlots[19] = 0;
    gPlayer.keyItemSlots[20] = 0;

}

static inline void setInitialSpecialDialogueBits() {

    setSpecialDialogueBit(0);
    setSpecialDialogueBit(0xBC);
    setSpecialDialogueBit(0xBD);
    setSpecialDialogueBit(0xBE);
    setSpecialDialogueBit(0xBF);
    setSpecialDialogueBit(0xC0);
    setSpecialDialogueBit(0xC1);
    setSpecialDialogueBit(0xC2);
    setSpecialDialogueBit(0xC3);
    setSpecialDialogueBit(0xC4);
    setSpecialDialogueBit(0xC5);
    setSpecialDialogueBit(0xC6);
    setSpecialDialogueBit(0xC7);
    setSpecialDialogueBit(0xC8);
    setSpecialDialogueBit(0xC9);
    setSpecialDialogueBit(0xCA);
    setSpecialDialogueBit(0xCB);
    setSpecialDialogueBit(0xCC);
    setSpecialDialogueBit(0xCD);
    setSpecialDialogueBit(0xCE);
    setSpecialDialogueBit(0xCF);
    setSpecialDialogueBit(0xD0);
    setSpecialDialogueBit(0xD1);
    setSpecialDialogueBit(0xD2);
    setSpecialDialogueBit(0xD3);
    setSpecialDialogueBit(0xD4);
    setSpecialDialogueBit(0xD5);
    setSpecialDialogueBit(0xD6);
    setSpecialDialogueBit(0xD7);
    setSpecialDialogueBit(0xDF);
    setSpecialDialogueBit(0xD8);
    setSpecialDialogueBit(0xD9);
    setSpecialDialogueBit(0xDA);
    setSpecialDialogueBit(0xDB);
    setSpecialDialogueBit(0xDC);
    setSpecialDialogueBit(0xDD);
    setSpecialDialogueBit(0xDE);
    setSpecialDialogueBit(0xDF);
    setSpecialDialogueBit(0xE0);

}

// TODO: experiment with loop unrolling
static inline void initializeToolboxSlots() {

    gToolboxSlots[0] = 0;
    gToolboxSlots[1] = 0;
    gToolboxSlots[2] = 0;
    gToolboxSlots[3] = 0;
    gToolboxSlots[4] = 0;
    gToolboxSlots[5] = 0;
    gToolboxSlots[6] = 0;
    gToolboxSlots[7] = 0;
    gToolboxSlots[8] = 0;
    gToolboxSlots[9] = 0;
    gToolboxSlots[10] = 0;
    gToolboxSlots[11] = 0;
    gToolboxSlots[12] = 0;
    gToolboxSlots[13] = 0;
    gToolboxSlots[14] = 0;
    gToolboxSlots[15] = 0;
    gToolboxSlots[16] = 0;
    gToolboxSlots[17] = 0;
    gToolboxSlots[18] = 0;
    gToolboxSlots[19] = 0;
    gToolboxSlots[20] = 0;
    gToolboxSlots[21] = 0;    
    gToolboxSlots[22] = 0;
    gToolboxSlots[23] = 0;
    gToolboxSlots[24] = 0;
    gToolboxSlots[25] = 0;
    gToolboxSlots[26] = 0;
    gToolboxSlots[27] = 0;
       
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeAll);

void initializeAll(void) {
    
    volatile musConfig c;
    
    c.fxs = sfxList;
    c.priority = musPriorityList;
     
    initializeAudio(&c);

    initializeGameVariables();

    gCurrentGameIndex = 0; 
    
    registerMainLoopCallbacks();
    initializeEntityAssets();
    loadMapAddresses();
    initializeMainMessageBoxes();
    initializeDialogueSystem();
    setEntitiesRGBA(0, 0, 0, 0);
    setMapControllerRGBA(MAIN_MAP_INDEX, 0, 0, 0, 0);

    setMainLoopCallbackFunctionIndex(MAIN_GAME);
    
    initializeWaveTable(0);

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeGameVariables);

// TODO: there might be some static inlines for each group being initialized
void initializeGameVariables(void) {

    u8 i, j, k;

    gBaseMapIndex = 0xFF;
    gCurrentAudioSequenceIndex = 0xFF;
    
    gAudioSequenceVolume = 128;
    
    previousMapIndex = 0xFF;
    gSpawnPointIndex = 0xFF;
    previousSpawnPoint = 0xFF;
    
    demoCutsceneIndex = 0;
    
    gWife = 0xFF;

    resetGlobalLighting();

    setStartingTime();
    
    gGold = 300;
    
    dailyShippingBinValue = 0;
    
    gWeather = SUNNY;
    gForecast = SUNNY;
    gSeasonTomorrow = SPRING;

    gameLoopContext.dialogueMenuIndex = 0;
    gameLoopContext.frameCount = 0;
    gameLoopContext.delayedFramesCount = 0;

    gCutsceneBytecodeSegmentIndex = 0xFFFF;
    gItemBeingHeld = 0xFF;

    gCutsceneFlags = 0;

    gMaximumStamina = 100;
    gPlayer.currentStamina = 100;

    gHarvestKing = 6;
    gHarvestCoinFinder = 0xFF;
    gFlowerFestivalGoddess = 0xFF;
    gCowFestivalEnteredCowIndex= 0xFF;
    gVoteForFlowerFestivalGoddess = 0xFF;
    spiritFestivalAssistant1 = 0;
    spiritFestivalAssistant2 = 0;
    
    spiritFestivalAssistant3 = 0;

    gHappiness = 0;

    blueMistFlowerPlot = 0;

    gSickDays = 0;

    deadAnimalCount = 0;

    flowerShopPoints = 0;
    bakeryCardPoints = 0;

    playerIdleCounter = 0;

    initializePlayer();
    
    toolUse.unk_0 = 0;
    toolUse.stepIndex = 0;
    toolUse.stumpHitCounter = 0;
    toolUse.stumpHitPositionX = 0;
    toolUse.stumpHitPositionZ = 0;
    toolUse.boulderHitCounter = 0;
    toolUse.boulderHitPositionX = 0;
    toolUse.boulderHitPositionZ = 0;
    toolUse.toolUseState = 0;

    for (i = 0; i < MAX_NPCS; i++) {
        npcs[i].entityIndex = 0;
        npcs[i].wanderRadiusX = 0;
        npcs[i].wanderRadiusZ = 0;
        npcs[i].levelIndex = 0;
        npcs[i].animationMode = 0;
        npcs[i].defaultAnimationMode = 0;
        npcs[i].direction = SOUTHWEST;
        npcs[i].speed = 0;
        npcs[i].animationState = 0;
        npcs[i].animationTimer = 0;
        npcs[i].location = 0;
    }

    for (i = 0; i < MAX_NPCS; i++) {
        npcAffection[i] = 0;
    }

    dogInfo.coordinates.x = 0;
    dogInfo.coordinates.y = 0;
    dogInfo.coordinates.z = 0;
    dogInfo.location = FARM;
    dogInfo.actionState = 0;
    dogInfo.direction = SOUTHWEST;
    dogInfo.speed = 0;
    dogInfo.stateTimer = 0;
    dogInfo.unk_1B = 0;
    dogInfo.name[0] = 0xF6;
    dogInfo.name[1] = 0xF6;
    dogInfo.name[2] = 0xF6;
    dogInfo.name[3] = 0xF6;
    dogInfo.name[4] = 0xF6;
    dogInfo.name[5] = 0xF6;
    dogInfo.bestRacePlacement = 0xFF;
    dogInfo.flags = 0;
    
    initializeDog();

    horseInfo.coordinates.x = 0;
    horseInfo.coordinates.y = 0;
    horseInfo.coordinates.z = 0;
    horseInfo.location = FARM;
    horseInfo.actionState = 0;
    horseInfo.direction = SOUTHWEST;
    horseInfo.speed = 0;
    horseInfo.stateTimer = 0;
    horseInfo.unk_1B = 0;
    horseInfo.age = 0;
    horseInfo.name[0] = 0xF6;
    horseInfo.name[1] = 0xF6;
    horseInfo.name[2] = 0xF6;
    horseInfo.name[3] = 0xF6;
    horseInfo.name[4] = 0xF6;
    horseInfo.name[5] = 0xF6;
    horseInfo.bestRacePlacement = 0xFF;
    horseInfo.flags = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        gChickens[i].location = 0;
        gChickens[i].actionState = 0;
        gChickens[i].direction = SOUTHWEST;
        gChickens[i].speed = 0;
        gChickens[i].stateTimer = 0;
        gChickens[i].unk_1B = 0;
        gChickens[i].type = 0;
        gChickens[i].condition = 0;
        gChickens[i].typeCounter = 0;
        gChickens[i].conditionCounter = 0;

        gChickens[i].coordinates.x = 0;
        gChickens[i].coordinates.y = 0;
        gChickens[i].coordinates.z = 0;
        
        gChickens[i].flags = 0;

        gChickens[i].name[0] = 0xFF;
        gChickens[i].name[1] = 0xFF;
        gChickens[i].name[2] = 0xFF;
        gChickens[i].name[3] = 0xFF;
        gChickens[i].name[4] = 0xFF;
        gChickens[i].name[5] = 0xFF;

    }

    for (i = 0; i < MAX_FARM_ANIMALS; i++) {
         
        gFarmAnimals[i].location = 0;
         
        gFarmAnimals[i].actionState = 0;
        gFarmAnimals[i].direction = 0;
        gFarmAnimals[i].speed = 0;
        gFarmAnimals[i].stateTimer = 0;
        gFarmAnimals[i].unk_1E = 0;
         
        gFarmAnimals[i].type = 0;
        gFarmAnimals[i].condition = 0;
        gFarmAnimals[i].typeCounter = 0;
        gFarmAnimals[i].conditionCounter = 0;
         
        gFarmAnimals[i].coordinates.x = 0;
        gFarmAnimals[i].coordinates.y = 0;
        gFarmAnimals[i].coordinates.z = 0;
         
        gFarmAnimals[i].flags = 0;
         
        gFarmAnimals[i].name[0] = 0;
        gFarmAnimals[i].name[1] = 0;
        gFarmAnimals[i].name[2] = 0;
        gFarmAnimals[i].name[3] = 0;
        gFarmAnimals[i].name[4] = 0;
        gFarmAnimals[i].name[5] = 0;
          
        gFarmAnimals[i].motherName[0] = 0xFF;
        gFarmAnimals[i].motherName[1] = 0xFF;
        gFarmAnimals[i].motherName[2] = 0xFF;
        gFarmAnimals[i].motherName[3] = 0xFF;
        gFarmAnimals[i].motherName[4] = 0xFF;
        gFarmAnimals[i].motherName[5] = 0xFF;
         
        gFarmAnimals[i].birthdaySeason = 0;
        gFarmAnimals[i].birthdayDayOfMonth = 0;
         
        gFarmAnimals[i].milkType = 0xFF;
    }

    for (i = 0; i < MAX_MISC_ANIMALS; i++) {

        gMiscAnimals[i].animalType = 0;
        gMiscAnimals[i].mapIndex = 0;
        gMiscAnimals[i].actionState = 0;
        gMiscAnimals[i].direction = 0;
        gMiscAnimals[i].zDisplacement = 0;
        gMiscAnimals[i].timer = 0;
        gMiscAnimals[i].unk_14 = 0;
        gMiscAnimals[i].coordinates.x = 0;
        gMiscAnimals[i].coordinates.y = 0;
        gMiscAnimals[i].coordinates.z = 0;
        gMiscAnimals[i].flags = 0;

    }

   // clearAllItemContextSlots from itemHandlers.c 

    for (i = 0; i < 10; i++) {

        itemInfo[i].position.x = 0;
        itemInfo[i].position.y = 0;
        itemInfo[i].position.z = 0;
        
        itemInfo[i].movement.x = 0;
        itemInfo[i].movement.y = 0;
        itemInfo[i].movement.z = 0;
        
        itemInfo[i].attachmentOffset.x = 0;
        itemInfo[i].attachmentOffset.y = 0;
        itemInfo[i].attachmentOffset.z = 0;
        
        itemInfo[i].unk_24 = 0;
        itemInfo[i].itemAnimationFrameCounter = 0;
        itemInfo[i].heldItemIndex = 0;
        
        itemInfo[i].stateIndex = 0;
        itemInfo[i].flags = 0;

    }
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.unk_3 = 0;
    overlayScreenTable.screenState = 0;

    initializeTV();

    turnipSeedsQuantity = 0;
    potatoSeedsQuantity = 0;
    eggplantSeedsQuantity = 0;
    cabbageSeedsQuantity = 0;
    strawberrySeedsQuantity = 0;
    tomatoSeedsQuantity = 0;
    cornSeedsQuantity = 0;
    grassSeedsQuantity = 0;
    moondropSeedsQuantity = 0;
    pinkCatMintSeedsQuantity = 0;
    blueMistSeedsQuantity = 0;
    wateringCanUses = 0;
    gLumber = 0;
    fodderQuantity = 0;
    chickenFeedQuantity = 0;

    initializeToolboxSlots();

    // freezer pages 1-4
    D_80237420[0] = 0;
    D_80237420[1] = 0;
    D_80237420[2] = 0;
    D_80237420[3] = 0;
    D_80237420[4] = 0;
    D_80237420[5] = 0;
    D_80237420[6] = 0;
    D_80237420[7] = 0;
    D_80237420[8] = 0;
    D_80237420[9] = 0;
    D_80237420[10] = 0;
    D_80237420[11] = 0;
    D_80237420[12] = 0;
    D_80237420[13] = 0;
    D_80237420[14] = 0;
    D_80237420[15] = 0;
    D_80237420[16] = 0;
    D_80237420[17] = 0;
    D_80237420[18] = 0;
    D_80237420[19] = 0;
    D_80237420[20] = 0;
    D_80237420[21] = 0;
    D_80237420[22] = 0;
    D_80237420[23] = 0;
    D_80237420[24] = 0;
    D_80237420[25] = 0;
    D_80237420[26] = 0;
    D_80237420[27] = 0;
    D_80237420[28] = 0;
    D_80237420[29] = 0;
    D_80237420[30] = 0;
    D_80237420[31] = 0;

    D_801890E8[0] = 0;
    D_801890E8[1] = 0;
    D_801890E8[2] = 0;
    D_801890E8[3] = 0;
    D_801890E8[4] = 0;
    D_801890E8[5] = 0;
    D_801890E8[6] = 0;
    D_801890E8[7] = 0;
    D_801890E8[8] = 0;
    D_801890E8[9] = 0;
    D_801890E8[10] = 0;
    D_801890E8[11] = 0;
    D_801890E8[12] = 0;
    D_801890E8[13] = 0;
    D_801890E8[14] = 0;
    D_801890E8[15] = 0;
    D_801890E8[16] = 0;
    D_801890E8[17] = 0;
    D_801890E8[18] = 0;
    D_801890E8[19] = 0;    
    D_801890E8[20] = 0;
    D_801890E8[21] = 0;
    D_801890E8[22] = 0;
    D_801890E8[23] = 0;
    D_801890E8[24] = 0;
    D_801890E8[25] = 0;
    D_801890E8[26] = 0;
    D_801890E8[27] = 0;
    D_801890E8[28] = 0;
    D_801890E8[29] = 0;
    D_801890E8[30] = 0;
    D_801890E8[31] = 0;

    for (j = 0; j < 5; j++) {
        for (k = 0; k < 7; k++) {
            D_80189108[j][k] = 0xFF;
        }
    }

    recipesBits[0] = 0;

    // unused
    D_8016FB00 = 0;

    albumBits = 0;

    for (i = 0; i < 32; i++) {
        lifeEventBits[i] = 0;
    }

    for (i = 0; i < 32; i++) {
        dailyEventBits[i] = 0;
    }
    
    for (i = 0; i < 16; i++) {
        specialDialogueBits[i] = 0;
    }    

    setInitialSpecialDialogueBits();
    
    memcpy(farmFieldTiles , D_80113580, FIELD_WIDTH * FIELD_HEIGHT);
    memcpy(greenhouseFieldTiles, D_80113760, FIELD_WIDTH * FIELD_HEIGHT);
        
    albumBits |= PHOTO_GRANDPA;

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", registerMainLoopCallbacks);

void registerMainLoopCallbacks(void) {

    // game.c
    registerMainLoopCallback(MAIN_GAME, mainGameLoopCallback);
    registerMainLoopCallback(MAP_LOAD, mapLoadCallback);
    registerMainLoopCallback(DREAM_CUTSCENES, dreamCutscenesCallback); 
    registerMainLoopCallback(SET_AUDIO_AND_LIGHTING, setMapAudioAndLighting);
    registerMainLoopCallback(LEVEL_LOAD, levelLoadCallback);
    registerMainLoopCallback(EXIT_LEVEL, exitLevelCallback);
    registerMainLoopCallback(OVERLAY_SCREEN_LOAD, setOverlayScreenCallbackWithDelay);
    registerMainLoopCallback(ROTATING, handleRotationCallback);
    registerMainLoopCallback(DIALOGUE, handleDialogueCallback);
    registerMainLoopCallback(MESSAGE_BOX, messageBoxCallback);
    registerMainLoopCallback(END_CUTSCENE, endCutsceneCallback);
    registerMainLoopCallback(DIALOGUE_SELECTION, pinkOverlayMenuCallback);
    registerMainLoopCallback(END_OF_DAY_1, endOfDayCallback1);
    registerMainLoopCallback(END_OF_FESTIVAL_DAY_1, endOfFestivalDayCallback1);
    registerMainLoopCallback(END_OF_FESTIVAL_DAY_2, endOfFestivalDayCallback2);
    registerMainLoopCallback(END_OF_DAY_2, endOfDayCallback2);
    registerMainLoopCallback(WAIT_AUDIO_FINISH, waitForAudioFinishCallback);

    // tv
    registerMainLoopCallback(TV, tvMainLoopCallback);
    
    // shop
    registerMainLoopCallback(SHOP_DIALOGUE, shopDialogueCallback); 

    // overlay screens
    registerMainLoopCallback(PAUSE_SCREEN_LOAD, loadPauseScreenCallback);
    registerMainLoopCallback(TOOLBOX_LOAD, loadToolboxScreenCallback);
    registerMainLoopCallback(FREEZER_LOAD, loadFreezerScreenCallback);
    registerMainLoopCallback(CABINET_LOAD, loadCabinetScreenCallback);
    registerMainLoopCallback(HOME_EXTENSIONS_SELECT_LOAD, loadHouseExtensionsSelectionCallback);
    registerMainLoopCallback(ESTIMATE_LOAD, loadEstimateScreenCallback);
    registerMainLoopCallback(KITCHEN_PICTURE_LOAD, loadKitchenPictureCallback);
    registerMainLoopCallback(CALENDAR_LOAD, loadCalendarScreenCallback);
    registerMainLoopCallback(RECIPE_BOOK_LOAD, loadRecipeBookScreenCallback);
    registerMainLoopCallback(ALBUM_LOAD, loadAlbumScreenCallback);
    registerMainLoopCallback(PAUSE_SCREEN_MAP_LOAD, loadPauseScreenMapScreenCallback);
    registerMainLoopCallback(RACE_BETTING_LOAD, loadRaceBettingScreenCallback);
    registerMainLoopCallback(RACE_RESULTS_LOAD, loadRaceResultsScreenCallback);
    registerMainLoopCallback(RACE_GIFTS_LOAD, loadRaceGiftsScreenCallback);
    registerMainLoopCallback(LOTTERY_LOAD, loadLotteryScreenCallback);
    registerMainLoopCallback(PAUSE_SCREEN, pauseScreenCallback);
    registerMainLoopCallback(TOOLBOX, toolboxScreenCallback);
    registerMainLoopCallback(FREEZER, freezerScreenCallback);
    registerMainLoopCallback(CABINET, cabinetScreenCallback);
    registerMainLoopCallback(HOME_EXTENSIONS_SELECT, houseExtensionsSelectionScreenCallback);
    registerMainLoopCallback(ESTIMATE, estimateScreenCallback);
    registerMainLoopCallback(KITCHEN_PICTURE, kitchenPictureScreenCallback);
    registerMainLoopCallback(CALENDAR, calendarScreenCallback);
    registerMainLoopCallback(RECIPE_BOOK, recipeBookScreenCallback);
    registerMainLoopCallback(ALBUM, albumScreenCallback);
    registerMainLoopCallback(PAUSE_SCREEN_MAP, pauseScreenMapCallback);
    registerMainLoopCallback(HORSE_RACE_BETTING, raceBettingScreenCallback);
    registerMainLoopCallback(HORSE_RACE_RESULTS, raceResultsScreenCallback);
    registerMainLoopCallback(HORSE_RACE_GIFTS, raceGiftsScreenCallback);
    registerMainLoopCallback(LOTTERY, lotteryScreenCallback);

    // title
    registerMainLoopCallback(TITLE_SCREEN, titleScreenMainLoopCallback);

    // naming screen
    registerMainLoopCallback(NAMING_SCREEN_LOAD, loadNamingScreenCallback);
    registerMainLoopCallback(NAMING_SCREEN, namingScreenCallback);

    // game file
    registerMainLoopCallback(SELECT_GAME, gameSelectCallback);
    registerMainLoopCallback(FARM_RANKING_SCREEN, farmRankingScreenCallback);

    // game status
    // empty funtions
    registerMainLoopCallback(55, func_800657B4);
    registerMainLoopCallback(56, func_800657C4);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeEntityAssets);

void initializeEntityAssets(void) {
    
    initializeShadowSprite(0, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsAssetsIndexSegmentRomStart, &_shadowsAssetsIndexSegmentRomEnd, (u8*)SHADOW_TEXTURE_BUFFER, (u16*)SHADOW_PALETTE_BUFFER, (AnimationFrameMetadata*)SHADOW_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHADOW_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 0);
    initializeShadowSprite(1, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsAssetsIndexSegmentRomStart, &_shadowsAssetsIndexSegmentRomEnd, (u8*)SHADOW_TEXTURE_BUFFER, (u16*)SHADOW_PALETTE_BUFFER, (AnimationFrameMetadata*)SHADOW_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHADOW_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 1, 0);
    initializeShadowSprite(2, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsAssetsIndexSegmentRomStart, &_shadowsAssetsIndexSegmentRomEnd, (u8*)SHADOW_TEXTURE_BUFFER, (u16*)SHADOW_PALETTE_BUFFER, (AnimationFrameMetadata*)SHADOW_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHADOW_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 2, 0);
 
    initializeEntityAsset(ENTITY_ASSET_PLAYER, &_playerTextureSegmentRomStart, &_playerTextureSegmentRomEnd, &_playerAssetsIndexSegmentRomStart, &_playerAssetsIndexSegmentRomEnd, &_playerSpritesheetIndexSegmentRomStart, &_playerSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_PLAYER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_PLAYER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MARIA, &_mariaTextureSegmentRomStart, &_mariaTextureSegmentRomEnd, &_mariaAssetsIndexSegmentRomStart, &_mariaAssetsIndexSegmentRomEnd, &_mariaSpritesheetIndexSegmentRomStart, &_mariaSpritesheetIndexSegmentRomEnd, 1, 0, &mariaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MARIA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MARIA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_POPURI, &_popuriTextureSegmentRomStart, &_popuriTextureSegmentRomEnd, &_popuriAssetsIndexSegmentRomStart, &_popuriAssetsIndexSegmentRomEnd, &_popuriSpritesheetIndexSegmentRomStart, &_popuriSpritesheetIndexSegmentRomEnd, 1, 0, &popuriAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_POPURI, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_POPURI, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ELLI, &_elliTextureSegmentRomStart, &_elliTextureSegmentRomEnd, &_elliAssetsIndexSegmentRomStart, &_elliAssetsIndexSegmentRomEnd, &_elliSpritesheetIndexSegmentRomStart, &_elliSpritesheetIndexSegmentRomEnd, 1, 0, &elliAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ELLI, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ELLI, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ANN, &_annTextureSegmentRomStart, &_annTextureSegmentRomEnd, &_annAssetsIndexSegmentRomStart, &_annAssetsIndexSegmentRomEnd, &_annSpritesheetIndexSegmentRomStart, &_annSpritesheetIndexSegmentRomEnd, 1, 0, &annAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ANN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ANN, 12, 12); 
    
    initializeEntityAsset(ENTITY_ASSET_KAREN, &_karenTextureSegmentRomStart, &_karenTextureSegmentRomEnd, &_karenAssetsIndexSegmentRomStart, &_karenAssetsIndexSegmentRomEnd, &_karenSpritesheetIndexSegmentRomStart, &_karenSpritesheetIndexSegmentRomEnd, 1, 0, &karenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_KAREN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_KAREN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_BABY, &_babyTextureSegmentRomStart, &_babyTextureSegmentRomEnd, &_babyAssetsIndexSegmentRomStart, &_babyAssetsIndexSegmentRomEnd, &_babySpritesheetIndexSegmentRomStart, &_babySpritesheetIndexSegmentRomEnd, 1, 0, &babyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_BABY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_BABY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HARRIS, &_harrisTextureSegmentRomStart, &_harrisTextureSegmentRomEnd, &_harrisAssetsIndexSegmentRomStart, &_harrisAssetsIndexSegmentRomEnd, &_harrisSpritesheetIndexSegmentRomStart, &_harrisSpritesheetIndexSegmentRomEnd, 1, 0, &harrisAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HARRIS, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HARRIS, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_GRAY, &_grayTextureSegmentRomStart, &_grayTextureSegmentRomEnd, &_grayAssetsIndexSegmentRomStart, &_grayAssetsIndexSegmentRomEnd, &_graySpritesheetIndexSegmentRomStart, &_graySpritesheetIndexSegmentRomEnd, 1, 0, &grayAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_GRAY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_GRAY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_JEFF, &_jeffTextureSegmentRomStart, &_jeffTextureSegmentRomEnd, &_jeffAssetsIndexSegmentRomStart, &_jeffAssetsIndexSegmentRomEnd, &_jeffSpritesheetIndexSegmentRomStart, &_jeffSpritesheetIndexSegmentRomEnd, 1, 0, &jeffAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_JEFF, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_JEFF, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CLIFF, &_cliffTextureSegmentRomStart, &_cliffTextureSegmentRomEnd, &_cliffAssetsIndexSegmentRomStart, &_cliffAssetsIndexSegmentRomEnd, &_cliffSpritesheetIndexSegmentRomStart, &_cliffSpritesheetIndexSegmentRomEnd, 1, 0, &cliffAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CLIFF, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CLIFF, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_KAI, &_kaiTextureSegmentRomStart, &_kaiTextureSegmentRomEnd, &_kaiAssetsIndexSegmentRomStart, &_kaiAssetsIndexSegmentRomEnd, &_kaiSpritesheetIndexSegmentRomStart, &_kaiSpritesheetIndexSegmentRomEnd, 1, 0, &kaiAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_KAI, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_KAI, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MAYOR, &_mayorTextureSegmentRomStart, &_mayorTextureSegmentRomEnd, &_mayorAssetsIndexSegmentRomStart, &_mayorAssetsIndexSegmentRomEnd, &_mayorSpritesheetIndexSegmentRomStart, &_mayorSpritesheetIndexSegmentRomEnd, 1, 0, &mayorAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MAYOR, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MAYOR, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MAYOR_WIFE, &_mayorWifeTextureSegmentRomStart, &_mayorWifeTextureSegmentRomEnd, &_mayorWifeAssetsIndexSegmentRomStart, &_mayorWifeAssetsIndexSegmentRomEnd, &_mayorWifeSpritesheetIndexSegmentRomStart, &_mayorWifeSpritesheetIndexSegmentRomEnd, 1, 0, &mayorWifeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MAYOR_WIFE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MAYOR_WIFE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_LILLIA, &_lilliaTextureSegmentRomStart, &_lilliaTextureSegmentRomEnd, &_lilliaAssetsIndexSegmentRomStart, &_lilliaAssetsIndexSegmentRomEnd, &_lilliaSpritesheetIndexSegmentRomStart, &_lilliaSpritesheetIndexSegmentRomEnd, 1, 0, &lilliaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_LILLIA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_LILLIA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_BASIL, &_basilTextureSegmentRomStart, &_basilTextureSegmentRomEnd, &_basilAssetsIndexSegmentRomStart, &_basilAssetsIndexSegmentRomEnd, &_basilSpritesheetIndexSegmentRomStart, &_basilSpritesheetIndexSegmentRomEnd, 1, 0, &basilAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_BASIL, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_BASIL, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ELLEN, &_ellenTextureSegmentRomStart, &_ellenTextureSegmentRomEnd, &_ellenAssetsIndexSegmentRomStart, &_ellenAssetsIndexSegmentRomEnd, &_ellenSpritesheetIndexSegmentRomStart, &_ellenSpritesheetIndexSegmentRomEnd, 1, 0, &ellenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ELLEN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ELLEN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_DOUG, &_dougTextureSegmentRomStart, &_dougTextureSegmentRomEnd, &_dougAssetsIndexSegmentRomStart, &_dougAssetsIndexSegmentRomEnd, &_dougSpritesheetIndexSegmentRomStart, &_dougSpritesheetIndexSegmentRomEnd, 1, 0, &dougAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_DOUG, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_DOUG, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_GOTZ, &_gotzTextureSegmentRomStart, &_gotzTextureSegmentRomEnd, &_gotzAssetsIndexSegmentRomStart, &_gotzAssetsIndexSegmentRomEnd, &_gotzSpritesheetIndexSegmentRomStart, &_gotzSpritesheetIndexSegmentRomEnd, 1, 0, &gotzAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_GOTZ, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_GOTZ, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SASHA, &_sashaTextureSegmentRomStart, &_sashaTextureSegmentRomEnd, &_sashaAssetsIndexSegmentRomStart, &_sashaAssetsIndexSegmentRomEnd, &_sashaSpritesheetIndexSegmentRomStart, &_sashaSpritesheetIndexSegmentRomEnd, 1, 0, &sashaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SASHA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SASHA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_POTION_SHOP_DEALER, &_potionShopDealerTextureSegmentRomStart, &_potionShopDealerTextureSegmentRomEnd, &_potionShopDealerAssetsIndexSegmentRomStart, &_potionShopDealerAssetsIndexSegmentRomEnd, &_potionShopDealerSpritesheetIndexSegmentRomStart, &_potionShopDealerSpritesheetIndexSegmentRomEnd, 1, 0, &potionShopDealerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_POTION_SHOP_DEALER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_POTION_SHOP_DEALER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_KENT, &_kentTextureSegmentRomStart, &_kentTextureSegmentRomEnd, &_kentAssetsIndexSegmentRomStart, &_kentAssetsIndexSegmentRomEnd, &_kentSpritesheetIndexSegmentRomStart, &_kentSpritesheetIndexSegmentRomEnd, 1, 0, &kentAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_KENT, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_KENT, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_STU, &_stuTextureSegmentRomStart, &_stuTextureSegmentRomEnd, &_stuAssetsIndexSegmentRomStart, &_stuAssetsIndexSegmentRomEnd, &_stuSpritesheetIndexSegmentRomStart, &_stuSpritesheetIndexSegmentRomEnd, 1, 0, &stuAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_STU, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_STU, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MIDWIFE, &_midwifeTextureSegmentRomStart, &_midwifeTextureSegmentRomEnd, &_midwifeAssetsIndexSegmentRomStart, &_midwifeAssetsIndexSegmentRomEnd, &_midwifeSpritesheetIndexSegmentRomStart, &_midwifeSpritesheetIndexSegmentRomEnd, 1, 0, &midwifeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MIDWIFE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MIDWIFE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MAY, &_mayTextureSegmentRomStart, &_mayTextureSegmentRomEnd, &_mayAssetsIndexSegmentRomStart, &_mayAssetsIndexSegmentRomEnd, &_maySpritesheetIndexSegmentRomStart, &_maySpritesheetIndexSegmentRomEnd, 1, 0, &mayAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MAY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MAY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_RICK, &_rickTextureSegmentRomStart, &_rickTextureSegmentRomEnd, &_rickAssetsIndexSegmentRomStart, &_rickAssetsIndexSegmentRomEnd, &_rickSpritesheetIndexSegmentRomStart, &_rickSpritesheetIndexSegmentRomEnd, 1, 0, &rickAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_RICK, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_RICK, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_PASTOR, &_pastorTextureSegmentRomStart, &_pastorTextureSegmentRomEnd, &_pastorAssetsIndexSegmentRomStart, &_pastorAssetsIndexSegmentRomEnd, &_pastorSpritesheetIndexSegmentRomStart, &_pastorSpritesheetIndexSegmentRomEnd, 1, 0, &pastorAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_PASTOR, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_PASTOR, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SHIPPER, &_shipperTextureSegmentRomStart, &_shipperTextureSegmentRomEnd, &_shipperAssetsIndexSegmentRomStart, &_shipperAssetsIndexSegmentRomEnd, &_shipperSpritesheetIndexSegmentRomStart, &_shipperSpritesheetIndexSegmentRomEnd, 1, 0, &shipperAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SHIPPER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SHIPPER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SAIBARA, &_saibaraTextureSegmentRomStart, &_saibaraTextureSegmentRomEnd, &_saibaraAssetsIndexSegmentRomStart, &_saibaraAssetsIndexSegmentRomEnd, &_saibaraSpritesheetIndexSegmentRomStart, &_saibaraSpritesheetIndexSegmentRomEnd, 1, 0, &saibaraAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SAIBARA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SAIBARA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_DUKE, &_dukeTextureSegmentRomStart, &_dukeTextureSegmentRomEnd, &_dukeAssetsIndexSegmentRomStart, &_dukeAssetsIndexSegmentRomEnd, &_dukeSpritesheetIndexSegmentRomStart, &_dukeSpritesheetIndexSegmentRomEnd, 1, 0, &dukeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_DUKE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_DUKE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_GREG, &_gregTextureSegmentRomStart, &_gregTextureSegmentRomEnd, &_gregAssetsIndexSegmentRomStart, &_gregAssetsIndexSegmentRomEnd, &_gregSpritesheetIndexSegmentRomStart, &_gregSpritesheetIndexSegmentRomEnd, 1, 0, &gregAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_GREG, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_GREG, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ASSISTANT_CARPENTER, &_assistantCarpenterTextureSegmentRomStart, &_assistantCarpenterTextureSegmentRomEnd, &_assistantCarpenterAssetsIndexSegmentRomStart, &_assistantCarpenterAssetsIndexSegmentRomEnd, &_assistantCarpenterSpritesheetIndexSegmentRomStart, &_assistantCarpenterSpritesheetIndexSegmentRomEnd, 1, 0, &assistantCarpenterAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ASSISTANT_CARPENTER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ASSISTANT_CARPENTER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MASTER_CARPENTER, &_masterCarpenterTextureSegmentRomStart, &_masterCarpenterTextureSegmentRomEnd, &_masterCarpenterAssetsIndexSegmentRomStart, &_masterCarpenterAssetsIndexSegmentRomEnd, &_masterCarpenterSpritesheetIndexSegmentRomStart, &_masterCarpenterSpritesheetIndexSegmentRomEnd, 1, 0, &masterCarpenterAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MASTER_CARPENTER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MASTER_CARPENTER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HARVEST_SPRITE, &_harvestSpriteTextureSegmentRomStart, &_harvestSpriteTextureSegmentRomEnd, &_harvestSpriteAssetsIndexSegmentRomStart, &_harvestSpriteAssetsIndexSegmentRomEnd, &_harvestSpriteSpritesheetIndexSegmentRomStart, &_harvestSpriteSpritesheetIndexSegmentRomEnd, 1, 0, &harvestSpriteAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HARVEST_SPRITE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HARVEST_SPRITE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_BARLEY, &_barleyTextureSegmentRomStart, &_barleyTextureSegmentRomEnd, &_barleyAssetsIndexSegmentRomStart, &_barleyAssetsIndexSegmentRomEnd, &_barleySpritesheetIndexSegmentRomStart, &_barleySpritesheetIndexSegmentRomEnd, 1, 0, &barleyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_BARLEY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_BARLEY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SYDNEY, &_sydneyTextureSegmentRomStart, &_sydneyTextureSegmentRomEnd, &_sydneyAssetsIndexSegmentRomStart, &_sydneyAssetsIndexSegmentRomEnd, &_sydneySpritesheetIndexSegmentRomStart, &_sydneySpritesheetIndexSegmentRomEnd, 1, 0, &sydneyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SYDNEY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SYDNEY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ENTOMOLOGIST, &_entomologistTextureSegmentRomStart, &_entomologistTextureSegmentRomEnd, &_entomologistAssetsIndexSegmentRomStart, &_entomologistAssetsIndexSegmentRomEnd, &_entomologistSpritesheetIndexSegmentRomStart, &_entomologistSpritesheetIndexSegmentRomEnd, 1, 0, &entomologistAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ENTOMOLOGIST, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ENTOMOLOGIST, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_GOURMET_JUDGE, &_gourmetJudgeTextureSegmentRomStart, &_gourmetJudgeTextureSegmentRomEnd, &_gourmetJudgeAssetsIndexSegmentRomStart, &_gourmetJudgeAssetsIndexSegmentRomEnd, &_gourmetJudgeSpritesheetIndexSegmentRomStart, &_gourmetJudgeSpritesheetIndexSegmentRomEnd, 1, 0, &gourmetJudgeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_GOURMET_JUDGE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_GOURMET_JUDGE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_PHOTOGRAPHER, &_photographerTextureSegmentRomStart, &_photographerTextureSegmentRomEnd, &_photographerAssetsIndexSegmentRomStart, &_photographerAssetsIndexSegmentRomEnd, &_photographerSpritesheetIndexSegmentRomStart, &_photographerSpritesheetIndexSegmentRomEnd, 1, 0, &photographerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_PHOTOGRAPHER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_PHOTOGRAPHER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SHADY_SALESMAN, &_shadySalesmanTextureSegmentRomStart, &_shadySalesmanTextureSegmentRomEnd, &_shadySalesmanAssetsIndexSegmentRomStart, &_shadySalesmanAssetsIndexSegmentRomEnd, &_shadySalesmanSpritesheetIndexSegmentRomStart, &_shadySalesmanSpritesheetIndexSegmentRomEnd, 1, 0, &shadySalesmanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SHADY_SALESMAN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SHADY_SALESMAN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MRS_MANA_SON, &_mrsManaSonTextureSegmentRomStart, &_mrsManaSonTextureSegmentRomEnd, &_mrsManaSonAssetsIndexSegmentRomStart, &_mrsManaSonAssetsIndexSegmentRomEnd, &_mrsManaSonSpritesheetIndexSegmentRomStart, &_mrsManaSonSpritesheetIndexSegmentRomEnd, 1, 0, &mrsManaSonAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MRS_MANA_SON, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MRS_MANA_SON, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MRS_MANA, &_mrsManaTextureSegmentRomStart, &_mrsManaTextureSegmentRomEnd, &_mrsManaAssetsIndexSegmentRomStart, &_mrsManaAssetsIndexSegmentRomEnd, &_mrsManaSpritesheetIndexSegmentRomStart, &_mrsManaSpritesheetIndexSegmentRomEnd, 1, 0, &mrsManaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MRS_MANA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MRS_MANA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_OLD_MAN, &_oldManTextureSegmentRomStart, &_oldManTextureSegmentRomEnd, &_oldManAssetsIndexSegmentRomStart, &_oldManAssetsIndexSegmentRomEnd, &_oldManSpritesheetIndexSegmentRomStart, &_oldManSpritesheetIndexSegmentRomEnd, 1, 0, &oldManAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_OLD_MAN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_OLD_MAN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_OLD_WOMAN, &_oldWomanTextureSegmentRomStart, &_oldWomanTextureSegmentRomEnd, &_oldWomanAssetsIndexSegmentRomStart, &_oldWomanAssetsIndexSegmentRomEnd, &_oldWomanSpritesheetIndexSegmentRomStart, &_oldWomanSpritesheetIndexSegmentRomEnd, 1, 0, &oldWomanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_OLD_WOMAN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_OLD_WOMAN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_TOURIST_COUPLE_MAN, &_touristCoupleManTextureSegmentRomStart, &_touristCoupleManTextureSegmentRomEnd, &_touristCoupleManAssetsIndexSegmentRomStart, &_touristCoupleManAssetsIndexSegmentRomEnd, &_touristCoupleManSpritesheetIndexSegmentRomStart, &_touristCoupleManSpritesheetIndexSegmentRomEnd, 1, 0, &touristCoupleManAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_TOURIST_COUPLE_MAN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_TOURIST_COUPLE_MAN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_TOURIST_COUPLE_WOMAN, &_touristCoupleWomanTextureSegmentRomStart, &_touristCoupleWomanTextureSegmentRomEnd, &_touristCoupleWomanAssetsIndexSegmentRomStart, &_touristCoupleWomanAssetsIndexSegmentRomEnd, &_touristCoupleWomanSpritesheetIndexSegmentRomStart, &_touristCoupleWomanSpritesheetIndexSegmentRomEnd, 1, 0, &touristCoupleWomanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_TOURIST_COUPLE_WOMAN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_TOURIST_COUPLE_WOMAN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_FESTIVAL_GIRL1, &_festivalGirl1TextureSegmentRomStart, &_festivalGirl1TextureSegmentRomEnd, &_festivalGirl1AssetsIndexSegmentRomStart, &_festivalGirl1AssetsIndexSegmentRomEnd, &_festivalGirl1SpritesheetIndexSegmentRomStart, &_festivalGirl1SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl1AnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_FESTIVAL_GIRL1, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_FESTIVAL_GIRL1, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_FESTIVAL_GIRL2, &_festivalGirl2TextureSegmentRomStart, &_festivalGirl2TextureSegmentRomEnd, &_festivalGirl2AssetsIndexSegmentRomStart, &_festivalGirl2AssetsIndexSegmentRomEnd, &_festivalGirl2SpritesheetIndexSegmentRomStart, &_festivalGirl2SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl2AnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_FESTIVAL_GIRL2, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_FESTIVAL_GIRL2, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_FESTIVAL_GIRL3, &_festivalGirl3TextureSegmentRomStart, &_festivalGirl3TextureSegmentRomEnd, &_festivalGirl3AssetsIndexSegmentRomStart, &_festivalGirl3AssetsIndexSegmentRomEnd, &_festivalGirl3SpritesheetIndexSegmentRomStart, &_festivalGirl3SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl3AnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_FESTIVAL_GIRL3, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_FESTIVAL_GIRL3, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HARVEST_GODDESS, &_harvestGoddessTextureSegmentRomStart, &_harvestGoddessTextureSegmentRomEnd, &_harvestGoddessAssetsIndexSegmentRomStart, &_harvestGoddessAssetsIndexSegmentRomEnd, &_harvestGoddessSpritesheetIndexSegmentRomStart, &_harvestGoddessSpritesheetIndexSegmentRomEnd, 1, 0, &harvestGoddessAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HARVEST_GODDESS, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HARVEST_GODDESS, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SPIRIT_FESTIVAL_OCARINA_PERFORMER, &_spiritFestivalOcarinaPerformerTextureSegmentRomStart, &_spiritFestivalOcarinaPerformerTextureSegmentRomEnd, &_spiritFestivalOcarinaPerformerAssetsIndexSegmentRomStart, &_spiritFestivalOcarinaPerformerAssetsIndexSegmentRomEnd, &_spiritFestivalOcarinaPerformerSpritesheetIndexSegmentRomStart, &_spiritFestivalOcarinaPerformerSpritesheetIndexSegmentRomEnd, 1, 0, &spiritFestivalOcarinaPerformerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SPIRIT_FESTIVAL_OCARINA_PERFORMER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SPIRIT_FESTIVAL_OCARINA_PERFORMER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_KAPPA, &_kappaTextureSegmentRomStart, &_kappaTextureSegmentRomEnd, &_kappaAssetsIndexSegmentRomStart, &_kappaAssetsIndexSegmentRomEnd, &_kappaSpritesheetIndexSegmentRomStart, &_kappaSpritesheetIndexSegmentRomEnd, 1, 0, &kappaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_KAPPA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_KAPPA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_PRINCESS, &_princessTextureSegmentRomStart, &_princessTextureSegmentRomEnd, &_princessAssetsIndexSegmentRomStart, &_princessAssetsIndexSegmentRomEnd, &_princessSpritesheetIndexSegmentRomStart, &_princessSpritesheetIndexSegmentRomEnd, 1, 0, &princessAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_PRINCESS, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_PRINCESS, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_EMPTY_NPC_SPRITE, &_emptyNPCTextureSegmentRomStart, &_emptyNPCTextureSegmentRomEnd, &_emptyNPCAssetsIndexSegmentRomStart, &_emptyNPCAssetsIndexSegmentRomEnd, &_emptyNPCSpritesheetIndexSegmentRomStart, &_emptyNPCSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_EMPTY_NPC_SPRITE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_EMPTY_NPC_SPRITE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_BUNNY, &_bunnyTextureSegmentRomStart, &_bunnyTextureSegmentRomEnd, &_bunnyAssetsIndexSegmentRomStart, &_bunnyAssetsIndexSegmentRomEnd, &_bunnySpritesheetIndexSegmentRomStart, &_bunnySpritesheetIndexSegmentRomEnd, 1, 0, &bunnyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_BUNNY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_BUNNY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MOUSE, &_mouseTextureSegmentRomStart, &_mouseTextureSegmentRomEnd, &_mouseAssetsIndexSegmentRomStart, &_mouseAssetsIndexSegmentRomEnd, &_mouseSpritesheetIndexSegmentRomStart, &_mouseSpritesheetIndexSegmentRomEnd, 1, 0, &mouseAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MOUSE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MOUSE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_FOX, &_foxTextureSegmentRomStart, &_foxTextureSegmentRomEnd, &_foxAssetsIndexSegmentRomStart, &_foxAssetsIndexSegmentRomEnd, &_foxSpritesheetIndexSegmentRomStart, &_foxSpritesheetIndexSegmentRomEnd, 1, 0, &foxAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_FOX, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_FOX, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MONKEY, &_monkeyTextureSegmentRomStart, &_monkeyTextureSegmentRomEnd, &_monkeyAssetsIndexSegmentRomStart, &_monkeyAssetsIndexSegmentRomEnd, &_monkeySpritesheetIndexSegmentRomStart, &_monkeySpritesheetIndexSegmentRomEnd, 1, 0, &monkeyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MONKEY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MONKEY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HAWK, &_hawkTextureSegmentRomStart, &_hawkTextureSegmentRomEnd, &_hawkAssetsIndexSegmentRomStart, &_hawkAssetsIndexSegmentRomEnd, &_hawkSpritesheetIndexSegmentRomStart, &_hawkSpritesheetIndexSegmentRomEnd, 1, 0, &hawkAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HAWK, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HAWK, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MOLE, &_moleTextureSegmentRomStart, &_moleTextureSegmentRomEnd, &_moleAssetsIndexSegmentRomStart, &_moleAssetsIndexSegmentRomEnd, &_moleSpritesheetIndexSegmentRomStart, &_moleSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MOLE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MOLE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SQUIRREL, &_squirrelTextureSegmentRomStart, &_squirrelTextureSegmentRomEnd, &_squirrelAssetsIndexSegmentRomStart, &_squirrelAssetsIndexSegmentRomEnd, &_squirrelSpritesheetIndexSegmentRomStart, &_squirrelSpritesheetIndexSegmentRomEnd, 1, 0, &squirrelAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SQUIRREL, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SQUIRREL, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_BIRD, &_birdTextureSegmentRomStart, &_birdTextureSegmentRomEnd, &_birdAssetsIndexSegmentRomStart, &_birdAssetsIndexSegmentRomEnd, &_birdSpritesheetIndexSegmentRomStart, &_birdSpritesheetIndexSegmentRomEnd, 1, 0, &birdAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_BIRD, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_BIRD, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CAT, &_catTextureSegmentRomStart, &_catTextureSegmentRomEnd, &_catAssetsIndexSegmentRomStart, &_catAssetsIndexSegmentRomEnd, &_catSpritesheetIndexSegmentRomStart, &_catSpritesheetIndexSegmentRomEnd, 1, 0, &catAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CAT, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CAT, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_DOG_VILLAGE, &_dogVillageTextureSegmentRomStart, &_dogVillageTextureSegmentRomEnd, &_dogVillageAssetsIndexSegmentRomStart, &_dogVillageAssetsIndexSegmentRomEnd, &_dogVillageSpritesheetIndexSegmentRomStart, &_dogVillageSpritesheetIndexSegmentRomEnd, 1, 2, &dogVillageAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_DOG_VILLAGE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_DOG_VILLAGE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_PLAYER_DOG, &_dogTextureSegmentRomStart, &_dogTextureSegmentRomEnd, &_dogAssetsIndexSegmentRomStart, &_dogAssetsIndexSegmentRomEnd, &_dogSpritesheetIndexSegmentRomStart, &_dogSpritesheetIndexSegmentRomEnd, 1, 0, &dogAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_PLAYER_DOG, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_PLAYER_DOG, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HORSE_PONY, &_horsePonyTextureSegmentRomStart, &_horsePonyTextureSegmentRomEnd, &_horsePonyAssetsIndexSegmentRomStart, &_horsePonyAssetsIndexSegmentRomEnd, &_horsePonySpritesheetIndexSegmentRomStart, &_horsePonySpritesheetIndexSegmentRomEnd, 1, 0, &horsePonyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HORSE_PONY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HORSE_PONY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HORSE_GROWN, &_horseGrownTextureSegmentRomStart, &_horseGrownTextureSegmentRomEnd, &_horseGrownAssetsIndexSegmentRomStart, &_horseGrownAssetsIndexSegmentRomEnd, &_horseGrownSpritesheetIndexSegmentRomStart, &_horseGrownSpritesheetIndexSegmentRomEnd, 1, 0, &horseGrownAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HORSE_GROWN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HORSE_GROWN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CHICK, &_chickTextureSegmentRomStart, &_chickTextureSegmentRomEnd, &_chickAssetsIndexSegmentRomStart, &_chickAssetsIndexSegmentRomEnd, &_chickSpritesheetIndexSegmentRomStart, &_chickSpritesheetIndexSegmentRomEnd, 1, 2, &chickAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CHICK, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CHICK, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CHICKEN, &_chickenTextureSegmentRomStart, &_chickenTextureSegmentRomEnd, &_chickenAssetsIndexSegmentRomStart, &_chickenAssetsIndexSegmentRomEnd, &_chickenSpritesheetIndexSegmentRomStart, &_chickenSpritesheetIndexSegmentRomEnd, 1, 0, &chickenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CHICKEN, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CHICKEN, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CALF, &_calfTextureSegmentRomStart, &_calfTextureSegmentRomEnd, &_calfAssetsIndexSegmentRomStart, &_calfAssetsIndexSegmentRomEnd, &_calfSpritesheetIndexSegmentRomStart, &_calfSpritesheetIndexSegmentRomEnd, 1, 0, &calfAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CALF, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CALF, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_YOUNG_COW, &_youngCowTextureSegmentRomStart, &_youngCowTextureSegmentRomEnd, &_youngCowAssetsIndexSegmentRomStart, &_youngCowAssetsIndexSegmentRomEnd, &_youngCowSpritesheetIndexSegmentRomStart, &_youngCowSpritesheetIndexSegmentRomEnd, 1, 1, &youngCowAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_YOUNG_COW, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_YOUNG_COW, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_COW, &_cowTextureSegmentRomStart, &_cowTextureSegmentRomEnd, &_cowAssetsIndexSegmentRomStart, &_cowAssetsIndexSegmentRomEnd, &_cowSpritesheetIndexSegmentRomStart, &_cowSpritesheetIndexSegmentRomEnd, 1, 1, &cowAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_COW, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_COW, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_LAMB, &_lambTextureSegmentRomStart, &_lambTextureSegmentRomEnd, &_lambAssetsIndexSegmentRomStart, &_lambAssetsIndexSegmentRomEnd, &_lambSpritesheetIndexSegmentRomStart, &_lambSpritesheetIndexSegmentRomEnd, 1, 0, &lambAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_LAMB, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_LAMB, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SHEEP, &_sheepTextureSegmentRomStart, &_sheepTextureSegmentRomEnd, &_sheepAssetsIndexSegmentRomStart, &_sheepAssetsIndexSegmentRomEnd, &_sheepSpritesheetIndexSegmentRomStart, &_sheepSpritesheetIndexSegmentRomEnd, 1, 0, &sheepAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SHEEP, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SHEEP, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_FROG, &_frogTextureSegmentRomStart, &_frogTextureSegmentRomEnd, &_frogAssetsIndexSegmentRomStart, &_frogAssetsIndexSegmentRomEnd, &_frogSpritesheetIndexSegmentRomStart, &_frogSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_FROG, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_FROG, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CRAB, &_crabTextureSegmentRomStart, &_crabTextureSegmentRomEnd, &_crabAssetsIndexSegmentRomStart, &_crabAssetsIndexSegmentRomEnd, &_crabSpritesheetIndexSegmentRomStart, &_crabSpritesheetIndexSegmentRomEnd, 1, 0, &crabAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CRAB, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CRAB, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HORNED_BEETLE, &_hornedBeetleTextureSegmentRomStart, &_hornedBeetleTextureSegmentRomEnd, &_hornedBeetleAssetsIndexSegmentRomStart, &_hornedBeetleAssetsIndexSegmentRomEnd, &_hornedBeetleSpritesheetIndexSegmentRomStart, &_hornedBeetleSpritesheetIndexSegmentRomEnd, 1, 0xFF, &hornedBeetleAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HORNED_BEETLE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HORNED_BEETLE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_STAG_BEETLE, &_stagBeetleTextureSegmentRomStart, &_stagBeetleTextureSegmentRomEnd, &_stagBeetleAssetsIndexSegmentRomStart, &_stagBeetleAssetsIndexSegmentRomEnd, &_stagBeetleSpritesheetIndexSegmentRomStart, &_stagBeetleSpritesheetIndexSegmentRomEnd, 1, 0xFF, &stagBeetleAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_STAG_BEETLE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_STAG_BEETLE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_BELL_RING_CRICKET, &_cricketTextureSegmentRomStart, &_cricketTextureSegmentRomEnd, &_cricketAssetsIndexSegmentRomStart, &_cricketAssetsIndexSegmentRomEnd, &_cricketSpritesheetIndexSegmentRomStart, &_cricketSpritesheetIndexSegmentRomEnd, 1, 0xFF, &cricketAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_BELL_RING_CRICKET, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_BELL_RING_CRICKET, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_CICADA, &_cicadaTextureSegmentRomStart, &_cicadaTextureSegmentRomEnd, &_cicadaAssetsIndexSegmentRomStart, &_cicadaAssetsIndexSegmentRomEnd, &_cicadaSpritesheetIndexSegmentRomStart, &_cicadaSpritesheetIndexSegmentRomEnd, 1, 0xFF, &cicadaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_CICADA, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_CICADA, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_WHITE_BUTTERFLY, &_whiteButterflyTextureSegmentRomStart, &_whiteButterflyTextureSegmentRomEnd, &_whiteButterflyAssetsIndexSegmentRomStart, &_whiteButterflyAssetsIndexSegmentRomEnd, &_whiteButterflySpritesheetIndexSegmentRomStart, &_whiteButterflySpritesheetIndexSegmentRomEnd, 1, 2, &whiteButterflyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_WHITE_BUTTERFLY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_WHITE_BUTTERFLY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_LADYBUG, &_ladybugTextureSegmentRomStart, &_ladybugTextureSegmentRomEnd, &_ladybugAssetsIndexSegmentRomStart, &_ladybugAssetsIndexSegmentRomEnd, &_ladybugSpritesheetIndexSegmentRomStart, &_ladybugSpritesheetIndexSegmentRomEnd, 1, 0xFF, &ladybugAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_LADYBUG, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_LADYBUG, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_DRAGONFLY, &_dragonflyTextureSegmentRomStart, &_dragonflyTextureSegmentRomEnd, &_dragonflyAssetsIndexSegmentRomStart, &_dragonflyAssetsIndexSegmentRomEnd, &_dragonflySpritesheetIndexSegmentRomStart, &_dragonflySpritesheetIndexSegmentRomEnd, 1, 2, &dragonflyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_DRAGONFLY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_DRAGONFLY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_GRASSHOPPER, &_grasshopperTextureSegmentRomStart, &_grasshopperTextureSegmentRomEnd, &_grasshopperAssetsIndexSegmentRomStart, &_grasshopperAssetsIndexSegmentRomEnd, &_grasshopperSpritesheetIndexSegmentRomStart, &_grasshopperSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_GRASSHOPPER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_GRASSHOPPER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_SNAKE, &_snakeTextureSegmentRomStart, &_snakeTextureSegmentRomEnd, &_snakeAssetsIndexSegmentRomStart, &_snakeAssetsIndexSegmentRomEnd, &_snakeSpritesheetIndexSegmentRomStart, &_snakeSpritesheetIndexSegmentRomEnd, 1, 2, &snakeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_SNAKE, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_SNAKE, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_PURPLE_BUTTERFLY, &_purpleButterflyTextureSegmentRomStart, &_purpleButterflyTextureSegmentRomEnd, &_purpleButterflyAssetsIndexSegmentRomStart, &_purpleButterflyAssetsIndexSegmentRomEnd, &_purpleButterflySpritesheetIndexSegmentRomStart, &_purpleButterflySpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_PURPLE_BUTTERFLY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_PURPLE_BUTTERFLY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_DAD, &_dadTextureSegmentRomStart, &_dadTextureSegmentRomEnd, &_dadAssetsIndexSegmentRomStart, &_dadAssetsIndexSegmentRomEnd, &_dadSpritesheetIndexSegmentRomStart, &_dadSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_DAD, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_DAD, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_COW_INDOORS, &_cowIndoorsTextureSegmentRomStart, &_cowIndoorsTextureSegmentRomEnd, &_cowIndoorsAssetsIndexSegmentRomStart, &_cowIndoorsAssetsIndexSegmentRomEnd, &_cowIndoorsSpritesheetIndexSegmentRomStart, &_cowIndoorsSpritesheetIndexSegmentRomEnd, 1, 1, &cowIndoorsAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_COW_INDOORS, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_COW_INDOORS, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HORSE_UNBRIDLED, &_horseUnbridledTextureSegmentRomStart, &_horseUnbridledTextureSegmentRomEnd, &_horseUnbridledAssetsIndexSegmentRomStart, &_horseUnbridledAssetsIndexSegmentRomEnd, &_horseUnbridledSpritesheetIndexSegmentRomStart, &_horseUnbridledSpritesheetIndexSegmentRomEnd, 1, 0, &horseUnbridledAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HORSE_UNBRIDLED, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HORSE_UNBRIDLED, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HORSE_RACER, &_horseRacerTextureSegmentRomStart, &_horseRacerTextureSegmentRomEnd, &_horseRacerAssetsIndexSegmentRomStart, &_horseRacerAssetsIndexSegmentRomEnd, &_horseRacerSpritesheetIndexSegmentRomStart, &_horseRacerSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HORSE_RACER, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HORSE_RACER, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_EMPTY_ENTITY, &_emptyEntityTextureSegmentRomStart, &_emptyEntityTextureSegmentRomEnd, &_emptyEntityAssetsIndexSegmentRomStart, &_emptyEntityAssetsIndexSegmentRomEnd, &_emptyEntitySpritesheetIndexSegmentRomStart, &_emptyEntitySpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_EMPTY_ENTITY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_EMPTY_ENTITY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HOLDABLE_ITEMS_1, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItems1AnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HOLDABLE_ITEMS_1, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HOLDABLE_ITEMS_1, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_EXTRA_FESTIVAL_ITEMS, &_extraFestivalTextureSegmentRomStart, &_extraFestivalTextureSegmentRomEnd, &_extraFestivalAssetsIndexSegmentRomStart, &_extraFestivalAssetsIndexSegmentRomEnd, &_extraFestivalSpritesheetIndexSegmentRomStart, &_extraFestivalSpritesheetIndexSegmentRomEnd, 1, 0, &homeItemsAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_EXTRA_FESTIVAL_ITEMS, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_EXTRA_FESTIVAL_ITEMS, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HOLDABLE_ITEMS_2, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 2, &shopItemsAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HOLDABLE_ITEMS_2, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HOLDABLE_ITEMS_2, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HOLDABLE_ITEMS_3, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItems2AnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HOLDABLE_ITEMS_3, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HOLDABLE_ITEMS_3, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_TOOLS, &_toolsTextureSegmentRomStart, &_toolsTextureSegmentRomEnd, &_toolsAssetsIndexSegmentRomStart, &_toolsAssetsIndexSegmentRomEnd, &_toolsSpritesheetIndexSegmentRomStart, &_toolsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &toolsAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_TOOLS, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_TOOLS, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_HOLDABLE_ITEMS_4, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItems3AnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_HOLDABLE_ITEMS_4, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_HOLDABLE_ITEMS_4, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_MARIA_HARRIS_BABY, &_mariaHarrisBabyTextureSegmentRomStart, &_mariaHarrisBabyTextureSegmentRomEnd, &_mariaHarrisBabyAssetsIndexSegmentRomStart, &_mariaHarrisBabyAssetsIndexSegmentRomEnd, &_mariaHarrisBabySpritesheetIndexSegmentRomStart, &_mariaHarrisBabySpritesheetIndexSegmentRomEnd, 1, 0, &mariaHarrisBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_MARIA_HARRIS_BABY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_MARIA_HARRIS_BABY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_POPURI_GRAY_BABY, &_popuriGrayBabyTextureSegmentRomStart, &_popuriGrayBabyTextureSegmentRomEnd, &_popuriGrayBabyAssetsIndexSegmentRomStart, &_popuriGrayBabyAssetsIndexSegmentRomEnd, &_popuriGrayBabySpritesheetIndexSegmentRomStart, &_popuriGrayBabySpritesheetIndexSegmentRomEnd, 1, 0, &popuriGrayBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_POPURI_GRAY_BABY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_POPURI_GRAY_BABY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ELLI_JEFF_BABY, &_elliJeffBabyTextureSegmentRomStart, &_elliJeffBabyTextureSegmentRomEnd, &_elliJeffBabyAssetsIndexSegmentRomStart, &_elliJeffBabyAssetsIndexSegmentRomEnd, &_elliJeffBabySpritesheetIndexSegmentRomStart, &_elliJeffBabySpritesheetIndexSegmentRomEnd, 1, 0, &elliJeffBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ELLI_JEFF_BABY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ELLI_JEFF_BABY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_ANN_CLIFF_BABY, &_annCliffBabyTextureSegmentRomStart, &_annCliffBabyTextureSegmentRomEnd, &_annCliffBabyAssetsIndexSegmentRomStart, &_annCliffBabyAssetsIndexSegmentRomEnd, &_annCliffBabySpritesheetIndexSegmentRomStart, &_annCliffBabySpritesheetIndexSegmentRomEnd, 1, 0, &annCliffBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_ANN_CLIFF_BABY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_ANN_CLIFF_BABY, 12, 12);
    
    initializeEntityAsset(ENTITY_ASSET_KAREN_KAI_BABY, &_karenKaiBabyTextureSegmentRomStart, &_karenKaiBabyTextureSegmentRomEnd, &_karenKaiBabyAssetsIndexSegmentRomStart, &_karenKaiBabyAssetsIndexSegmentRomEnd, &_karenKaiBabySpritesheetIndexSegmentRomStart, &_karenKaiBabySpritesheetIndexSegmentRomEnd, 1, 0, &karenKaiBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSET_KAREN_KAI_BABY, 15, 15);
    setEntitySpriteDimensions(ENTITY_ASSET_KAREN_KAI_BABY, 12, 12);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", loadMapAddresses);

void loadMapAddresses(void) {
    
    setMapDataAddresses(RANCH_SPRING, &_ranchSpringMapSegmentRomStart, &_ranchSpringMapSegmentRomEnd);
    setMapDataAddresses(RANCH_SUMMER, &_ranchSummerMapSegmentRomStart, &_ranchSummerMapSegmentRomEnd);
    setMapDataAddresses(RANCH_AUTUMN, &_ranchFallMapSegmentRomStart, &_ranchFallMapSegmentRomEnd);
    setMapDataAddresses(RANCH_WINTER, &_ranchWinterMapSegmentRomStart, &_ranchWinterMapSegmentRomEnd);
 
    setMapDataAddresses(ANN_ROOM, &_annsRoomMapSegmentRomStart, &_annsRoomMapSegmentRomEnd);

    setMapDataAddresses(RANCH_BARN, &_ranchBarnMapSegmentRomStart, &_ranchBarnMapSegmentRomEnd);
    setMapDataAddresses(RANCH_STORE, &_ranchStoreMapSegmentRomStart, &_ranchStoreMapSegmentRomEnd);
    setMapDataAddresses(RANCH_HOUSE, &_ranchHouseMapSegmentRomStart, &_ranchHouseMapSegmentRomEnd);

    setMapDataAddresses(EMPTY_MAP_1, &_emptyMap1SegmentRomStart, &_emptyMap1SegmentRomEnd);
    
    setMapDataAddresses(BEACH_SPRING, &_beachSpringMapSegmentRomStart, &_beachSpringMapSegmentRomEnd);
    setMapDataAddresses(BEACH_SUMMER, &_beachSummerMapSegmentRomStart, &_beachSummerMapSegmentRomEnd);
    setMapDataAddresses(BEACH_AUTUMN, &_beachFallMapSegmentRomStart, &_beachFallMapSegmentRomEnd);
    setMapDataAddresses(BEACH_WINTER, &_beachWinterMapSegmentRomStart, &_beachWinterMapSegmentRomEnd);

    setMapDataAddresses(RACE_TRACK_SPRING, &_raceTrackSpringMapSegmentRomStart, &_raceTrackSpringMapSegmentRomEnd);
    // no race track summer: 0xE
    // empty 4 words in rom
    setMapDataAddresses(RACE_TRACK_AUTUMN, &_raceTrackFallMapSegmentRomStart, &_raceTrackFallMapSegmentRomEnd);
    setMapDataAddresses(RACE_TRACK_WINTER, &_raceTrackWinterMapSegmentRomStart, &_raceTrackWinterMapSegmentRomEnd);

    setMapDataAddresses(MOUNTAIN_1_SPRING, &_mountain1SpringMapSegmentRomStart, &_mountain1SpringMapSegmentRomEnd);
    setMapDataAddresses(MOUNTAIN_1_SUMMER, &_mountain1SummerMapSegmentRomStart, &_mountain1SummerMapSegmentRomEnd);
    setMapDataAddresses(MOUNTAIN_1_AUTUMN, &_mountain1FallMapSegmentRomStart, &_mountain1FallMapSegmentRomEnd);
    setMapDataAddresses(MOUNTAIN_1_WINTER, &_mountain1WinterMapSegmentRomStart, &_mountain1WinterMapSegmentRomEnd);

    setMapDataAddresses(MOUNTAIN_2_SPRING, &_mountain2SpringMapSegmentRomStart, &_mountain2SpringMapSegmentRomEnd);
    setMapDataAddresses(MOUNTAIN_2_SUMMER, &_mountain2SummerMapSegmentRomStart, &_mountain2SummerMapSegmentRomEnd);
    setMapDataAddresses(MOUNTAIN_2_AUTUMN, &_mountain2FallMapSegmentRomStart, &_mountain2FallMapSegmentRomEnd);
    setMapDataAddresses(MOUNTAIN_2_WINTER, &_mountain2WinterMapSegmentRomStart, &_mountain2WinterMapSegmentRomEnd);

    setMapDataAddresses(TOP_OF_MOUNTAIN_1_SPRING, &_topOfMountain1SpringMapSegmentRomStart, &_topOfMountain1SpringMapSegmentRomEnd);
    setMapDataAddresses(TOP_OF_MOUNTAIN_1_SUMMER, &_topOfMountain1SummerMapSegmentRomStart, &_topOfMountain1SummerMapSegmentRomEnd);
    setMapDataAddresses(TOP_OF_MOUNTAIN_1_AUTUMN, &_topOfMountain1FallMapSegmentRomStart, &_topOfMountain1FallMapSegmentRomEnd);
    setMapDataAddresses(TOP_OF_MOUNTAIN_1_WINTER, &_topOfMountain1WinterMapSegmentRomStart, &_topOfMountain1WinterMapSegmentRomEnd);

    setMapDataAddresses(MOON_MOUNTAIN_SPRING, &_moonMountainSpringMapSegmentRomStart, &_moonMountainSpringMapSegmentRomEnd);
    setMapDataAddresses(MOON_MOUNTAIN_SUMMER, &_moonMountainSummerMapSegmentRomStart, &_moonMountainSummerMapSegmentRomEnd);
    setMapDataAddresses(MOON_MOUNTAIN_AUTUMN, &_moonMountainFallMapSegmentRomStart, &_moonMountainFallMapSegmentRomEnd);
    setMapDataAddresses(MOON_MOUNTAIN_WINTER, &_moonMountainWinterMapSegmentRomStart, &_moonMountainWinterMapSegmentRomEnd);

    setMapDataAddresses(CARPENTER_HUT, &_carpentersHutMapSegmentRomStart, &_carpentersHutMapSegmentRomEnd);
    
    setMapDataAddresses(DUMPLING_HOUSE, &_dumplingHouseMapSegmentRomStart, &_dumplingHouseMapSegmentRomEnd);

    setMapDataAddresses(POND_SPRING, &_pondSpringMapSegmentRomStart, &_pondSpringMapSegmentRomEnd);
    setMapDataAddresses(POND_SUMMER, &_pondSummerMapSegmentRomStart, &_pondSummerMapSegmentRomEnd);
    setMapDataAddresses(POND_AUTUMN, &_pondFallMapSegmentRomStart, &_pondFallMapSegmentRomEnd);
    setMapDataAddresses(POND_WINTER, &_pondWinterMapSegmentRomStart, &_pondWinterMapSegmentRomEnd);

    setMapDataAddresses(ELLI_ROOM, &_ellisRoomMapSegmentRomStart, &_ellisRoomMapSegmentRomEnd);

    setMapDataAddresses(BAKERY, &_bakeryMapSegmentRomStart, &_bakeryMapSegmentRomEnd);

    setMapDataAddresses(VILLAGE_1_SPRING, &_village1SpringMapSegmentRomStart, &_village1SpringMapSegmentRomEnd);
    setMapDataAddresses(VILLAGE_1_SUMMER, &_village1SummerMapSegmentRomStart, &_village1SummerMapSegmentRomEnd);
    setMapDataAddresses(VILLAGE_1_AUTUMN, &_village1FallMapSegmentRomStart, &_village1FallMapSegmentRomEnd);
    setMapDataAddresses(VILLAGE_1_WINTER, &_village1WinterMapSegmentRomStart, &_village1WinterMapSegmentRomEnd);

    setMapDataAddresses(VILLAGE_2_SPRING, &_village2SpringMapSegmentRomStart, &_village2SpringMapSegmentRomEnd);
    setMapDataAddresses(VILLAGE_2_SUMMER, &_village2SummerMapSegmentRomStart, &_village2SummerMapSegmentRomEnd);
    setMapDataAddresses(VILLAGE_2_AUTUMN, &_village2FallMapSegmentRomStart, &_village2FallMapSegmentRomEnd);
    setMapDataAddresses(VILLAGE_2_WINTER, &_village2WinterMapSegmentRomStart, &_village2WinterMapSegmentRomEnd);

    setMapDataAddresses(POPURI_ROOM, &_popurisRoomMapSegmentRomStart, &_popurisRoomMapSegmentRomEnd);

    setMapDataAddresses(FLOWER_SHOP, &_flowerShopMapSegmentRomStart, &_flowerShopMapSegmentRomEnd);

    setMapDataAddresses(CHURCH, &_churchMapSegmentRomStart, &_churchMapSegmentRomEnd);

    setMapDataAddresses(SOUVENIR_SHOP, &_souvenirShopMapSegmentRomStart, &_souvenirShopMapSegmentRomEnd);

    setMapDataAddresses(SQUARE_SPRING, &_squareSpringMapSegmentRomStart, &_squareSpringMapSegmentRomEnd);
    setMapDataAddresses(SQUARE_SUMMER, &_squareSummerMapSegmentRomStart, &_squareSummerMapSegmentRomEnd);
    setMapDataAddresses(SQUARE_AUTUMN, &_squareFallMapSegmentRomStart, &_squareFallMapSegmentRomEnd);
    setMapDataAddresses(SQUARE_WINTER, &_squareWinterMapSegmentRomStart, &_squareWinterMapSegmentRomEnd);

    setMapDataAddresses(RICK_STORE, &_ricksShopMapSegmentRomStart, &_ricksShopMapSegmentRomEnd);

    setMapDataAddresses(MIDWIFE_HOUSE, &_midwifesHouseMapSegmentRomStart, &_midwifesHouseMapSegmentRomEnd);
    
    setMapDataAddresses(TAVERN, &_tavernMapSegmentRomStart, &_tavernMapSegmentRomEnd);
    
    setMapDataAddresses(LIBRARY, &_libraryMapSegmentRomStart, &_libraryMapSegmentRomEnd);

    setMapDataAddresses(MARIA_ROOM, &_mariasRoomMapSegmentRomStart, &_mariasRoomMapSegmentRomEnd);
    
    setMapDataAddresses(MAYOR_HOUSE, &_mayorsHouseMapSegmentRomStart, &_mayorsHouseMapSegmentRomEnd);

    setMapDataAddresses(POTION_SHOP_BEDROOM, &_potionShopBackroomMapSegmentRomStart, &_potionShopBackroomMapSegmentRomEnd);
    setMapDataAddresses(POTION_SHOP, &_potionShopMapSegmentRomStart, &_potionShopMapSegmentRomEnd);

    // empty, referenced in getCantEnterTextIndex 
    setMapDataAddresses(EMPTY_MAP_2, &_emptyMap2SegmentRomStart, &_emptyMap2SegmentRomEnd);
    
    setMapDataAddresses(HARVEST_SPRITE_CAVE, &_spriteCaveMapSegmentRomStart, &_spriteCaveMapSegmentRomEnd);
    setMapDataAddresses(CAVE, &_caveMapSegmentRomStart, &_caveMapSegmentRomEnd);    
    setMapDataAddresses(MINE, &_emptyMineMapSegmentRomStart, &_emptyMineMapSegmentRomEnd);
    setMapDataAddresses(MINE_2, &_mineMapSegmentRomStart, &_mineMapSegmentRomEnd);

    setMapDataAddresses(KAREN_ROOM, &_karensRoomMapSegmentRomStart, &_karensRoomMapSegmentRomEnd);

    setMapDataAddresses(VINEYARD_SPRING, &_vineyardSpringMapSegmentRomStart, &_vineyardSpringMapSegmentRomEnd);
    setMapDataAddresses(VINEYARD_SUMMER, &_vineyardSummerMapSegmentRomStart, &_vineyardSummerMapSegmentRomEnd);
    setMapDataAddresses(VINEYARD_AUTUMN, &_vineyardFallMapSegmentRomStart, &_vineyardFallMapSegmentRomEnd);
    setMapDataAddresses(VINEYARD_WINTER, &_vineyardWinterMapSegmentRomStart, &_vineyardWinterMapSegmentRomEnd);

    setMapDataAddresses(VINEYARD_HOUSE, &_vineyardHouseMapSegmentRomStart, &_vineyardHouseMapSegmentRomEnd);

    setMapDataAddresses(VINEYARD_CELLAR, &_vineyardCellar1MapSegmentRomStart, &_vineyardCellar1MapSegmentRomEnd);
    setMapDataAddresses(VINEYARD_CELLAR_BASEMENT, &_vineyardCellar2MapSegmentRomStart, &_vineyardCellar2MapSegmentRomEnd);

    setMapDataAddresses(ROAD_SPRING, &_roadSpringMapSegmentRomStart, &_roadSpringMapSegmentRomEnd);
    setMapDataAddresses(ROAD_SUMMER, &_roadSummerMapSegmentRomStart, &_roadSummerMapSegmentRomEnd);
    setMapDataAddresses(ROAD_AUTUMN, &_roadFallMapSegmentRomStart, &_roadFallMapSegmentRomEnd);
    setMapDataAddresses(ROAD_WINTER, &_roadWinterMapSegmentRomStart, &_roadWinterMapSegmentRomEnd);

    setMapDataAddresses(FARM_SPRING, &_farmSpringMapSegmentRomStart, &_farmSpringMapSegmentRomEnd);
    setMapDataAddresses(FARM_SUMMER, &_farmSummerMapSegmentRomStart, &_farmSummerMapSegmentRomEnd);
    setMapDataAddresses(FARM_AUTUMN, &_farmFallMapSegmentRomStart, &_farmFallMapSegmentRomEnd);
    setMapDataAddresses(FARM_WINTER, &_farmWinterMapSegmentRomStart, &_farmWinterMapSegmentRomEnd);

    setMapDataAddresses(GREENHOUSE, &_greenhouseMapSegmentRomStart, &_greenhouseMapSegmentRomEnd);
    
    setMapDataAddresses(HOUSE, &_houseMapSegmentRomStart, &_houseMapSegmentRomEnd);

    setMapDataAddresses(BARN, &_barnMapSegmentRomStart, &_barnMapSegmentRomEnd);
    setMapDataAddresses(COOP, &_coopMapSegmentRomStart, &_coopMapSegmentRomEnd);
    
    setMapDataAddresses(KITCHEN, &_kitchenMapSegmentRomStart, &_kitchenMapSegmentRomEnd);
    setMapDataAddresses(BATHROOM, &_bathroomMapSegmentRomStart, &_bathroomMapSegmentRomEnd);
    
    initializeMapController(MAIN_MAP_INDEX, 0, MAP_DATA_BUFFER);

}

// INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeMainMessageBoxes);

void initializeMainMessageBoxes(void) {

    u32 ptr = &_fontTextureSegmentRomStart;
    u32 ptr2 = &_fontTextureSegmentRomEnd;
    u32 ptr3 = &_fontPalette1SegmentRomStart;
    u32 ptr4 = &_fontPalette1SegmentRomEnd;
    u32 ptr5 = &_fontPalette2SegmentRomStart;
    u32 ptr6 = &_fontPalette2SegmentRomEnd;

    initializeTextAddresses();
    initializeGameVariableStrings();
    setCharacterAvatarAnimationsPtr(characterAvatarCodesToAnimations);

    nuPiReadRom(ptr, (u8*)FONT_TEXTURE_BUFFER, ptr2 - ptr);
    nuPiReadRom(ptr3, (u16*)FONT_PALETTE_1_BUFFER, ptr4 - ptr3);
    nuPiReadRom(ptr5, (u16*)FONT_PALETTE_2_BUFFER, ptr6 - ptr5);

    setDialogueWindowSprite(0, 0x76, &_messageBoxTextureSegmentRomStart, &_messageBoxTextureSegmentRomEnd, &_messageBoxAssetsIndexSegmentRomStart, &_messageBoxAssetsIndexSegmentRomEnd, (u8*)MESSAGE_BOX_TEXTURE_BUFFER, (u16*)MESSAGE_BOX_PALETTE_BUFFER, (u32*)MESSAGE_BOX_ANIMATION_FRAME_METADATA_BUFFER, (u32*)MESSAGE_BOX_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 0, 1, -24.0f, 0, 0);
    setDialogueWindowSprite(1, 0x76, &_messageBoxTextureSegmentRomStart, &_messageBoxTextureSegmentRomEnd, &_messageBoxAssetsIndexSegmentRomStart, &_messageBoxAssetsIndexSegmentRomEnd, (u8*)MESSAGE_BOX_TEXTURE_BUFFER, (u16*)MESSAGE_BOX_PALETTE_BUFFER, (u32*)MESSAGE_BOX_ANIMATION_FRAME_METADATA_BUFFER, (u32*)MESSAGE_BOX_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 0, 0, 0, 0, 0);
    setDialogueWindowSprite(2, 0x77, &_messageBoxTextureSegmentRomStart, &_messageBoxTextureSegmentRomEnd, &_messageBoxAssetsIndexSegmentRomStart, &_messageBoxAssetsIndexSegmentRomEnd, (u8*)MESSAGE_BOX_TEXTURE_BUFFER, (u16*)MESSAGE_BOX_PALETTE_BUFFER, (u32*)MESSAGE_BOX_ANIMATION_FRAME_METADATA_BUFFER, (u32*)MESSAGE_BOX_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, 0, 0, 0);
  
    setOverlayIconSprite(0, 0x78, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 4, 0xFE, 106.0f, -15.0f, 0.0f);
    setOverlayIconSprite(1, 0x78, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 0xD, 0xFE, 106.0f, -15.0f, 0.0f);
    
    setCharacterAvatarSprite(0, 0x75, &_characterAvatarsTextureSegmentRomStart, &_characterAvatarsTextureSegmentRomEnd, &_characterAvatarsAssetsIndexSegmentRomStart, &_characterAvatarsAssetsIndexSegmentRomEnd, &_characterAvatarsSpritesheetIndexSegmentRomStart, &_characterAvatarsSpritesheetIndexSegmentRomEnd, (u8*)CHARACTER_AVATAR_TEXTURE_1_BUFFER, (u8*)CHARACTER_AVATAR_TEXTURE_2_BUFFER, (u16*)CHARACTER_AVATAR_PALETTE_BUFFER, (AnimationFrameMetadata*)CHARACTER_AVATAR_ANIMATION_FRAME_METADATA_BUFFER, (u32*)CHARACTER_AVATAR_SPRITESHEET_INDEX_BUFFER, (u32*)CHARACTER_AVATAR_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, -139.0f, 1.0f, 0);
 
    initializeEmptyMessageBox(MAIN_MESSAGE_BOX_INDEX, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(MAIN_MESSAGE_BOX_INDEX, 24.0f, -64.0f, 352.0f);
    setMessageBoxLineAndRowSizes(MAIN_MESSAGE_BOX_INDEX, 0x10, 3);
    setMessageBoxSpacing(MAIN_MESSAGE_BOX_INDEX, 0, 2);
    setMessageBoxFont(MAIN_MESSAGE_BOX_INDEX, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(MAIN_MESSAGE_BOX_INDEX, -4, 0);
    setMessageBoxSpriteIndices(MAIN_MESSAGE_BOX_INDEX, 0, 0, 0);
    setMessageBoxSfx(MAIN_MESSAGE_BOX_INDEX, 0x57, 8, 1);
    setMessageBoxButtonMask(MAIN_MESSAGE_BOX_INDEX, (BUTTON_B | BUTTON_A));
    setMessageBoxScrollSpeed(MAIN_MESSAGE_BOX_INDEX, 1);
    
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, 64.0f, 32.0f, 352.0f);
    setMessageBoxSpacing(1, 0, 2);
    setMessageBoxFont(1, 14, 14, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_2_BUFFER);
    setMessageBoxLineAndRowSizes(1, 0xB, 4);
    setMessageBoxInterpolationWithFlags(1, -4, 3);
    setMessageBoxButtonMask(1, (BUTTON_B | BUTTON_A));
    setMessageBoxScrollSpeed(1, 2);
    setMessageBoxSpriteIndices(1, 2, 0xFF, 0xFF);
    setMessageBoxSfx(1, 0xFF, 0xFF, 0xFF);
    
}
    
//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeTextAddresses);

void initializeTextAddresses(void) {

    setTextAddresses(TEXT_1_TEXT_INDEX, &_text1TextIndexSegmentRomStart, &_text1TextIndexSegmentRomEnd, &_text1TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(LIBRARY_TEXT_INDEX, &_libraryTextIndexSegmentRomStart, &_libraryTextIndexSegmentRomEnd, &_libraryTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(DIARY_TEXT_INDEX, &_diaryTextIndexSegmentRomStart, &_diaryTextIndexSegmentRomEnd, &_diaryTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(RECIPES_JAPANESE_TEXT_INDEX, &_recipesJapaneseTextIndexSegmentRomStart, &_recipesJapaneseTextIndexSegmentRomEnd, &_recipesJapaneseTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(FESTIVALS_TEXT_INDEX, &_festivalOverlaySelectionsTextIndexSegmentRomStart, &_festivalOverlaySelectionsTextIndexSegmentRomEnd, &_festivalOverlaySelectionsTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(LETTERS_TEXT_INDEX, &_lettersTextIndexSegmentRomStart, &_lettersTextIndexSegmentRomEnd, &_lettersTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(SHOP_TEXT_INDEX, &_shopTextIndexSegmentRomStart, &_shopTextIndexSegmentRomEnd, &_shopTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ANIMAL_INTERACTIONS_TEXT_INDEX, &_animalInteractionsTextIndexSegmentRomStart, &_animalInteractionsTextIndexSegmentRomEnd, &_animalInteractionsTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TV_TEXT_INDEX, &_tvTextIndexSegmentRomStart, &_tvTextIndexSegmentRomEnd, &_tvTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_10_TEXT_INDEX, &_text10TextIndexSegmentRomStart, &_text10TextIndexSegmentRomEnd, &_text10TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(NAMING_SCREEN_TEXT_INDEX, &_namingScreenTextIndexSegmentRomStart, &_namingScreenTextIndexSegmentRomEnd, &_namingScreenTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);

    setTextAddresses(MARIA_TEXT_INDEX, &_mariaTextIndexSegmentRomStart, &_mariaTextIndexSegmentRomEnd, &_mariaTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(POPURI_TEXT_INDEX, &_popuriTextIndexSegmentRomStart, &_popuriTextIndexSegmentRomEnd, &_popuriTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ELLI_TEXT_INDEX, &_elliTextIndexSegmentRomStart, &_elliTextIndexSegmentRomEnd, &_elliTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ANN_TEXT_INDEX, &_annTextIndexSegmentRomStart, &_annTextIndexSegmentRomEnd, &_annTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(KAREN_TEXT_INDEX, &_karenTextIndexSegmentRomStart, &_karenTextIndexSegmentRomEnd, &_karenTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(HARRIS_TEXT_INDEX, &_harrisTextIndexSegmentRomStart, &_harrisTextIndexSegmentRomEnd, &_harrisTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(GRAY_TEXT_INDEX, &_grayTextIndexSegmentRomStart, &_grayTextIndexSegmentRomEnd, &_grayTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(JEFF_TEXT_INDEX, &_jeffTextIndexSegmentRomStart, &_jeffTextIndexSegmentRomEnd, &_jeffTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(CLIFF_TEXT_INDEX, &_cliffTextIndexSegmentRomStart, &_cliffTextIndexSegmentRomEnd, &_cliffTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(KAI_TEXT_INDEX, &_kaiTextIndexSegmentRomStart, &_kaiTextIndexSegmentRomEnd, &_kaiTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MAYOR_TEXT_INDEX, &_mayorTextIndexSegmentRomStart, &_mayorTextIndexSegmentRomEnd, &_mayorTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MAYOR_WIFE_TEXT_INDEX, &_mayorWifeTextIndexSegmentRomStart, &_mayorWifeTextIndexSegmentRomEnd, &_mayorWifeTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(LILLIA_TEXT_INDEX, &_lilliaTextIndexSegmentRomStart, &_lilliaTextIndexSegmentRomEnd, &_lilliaTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(BASIL_TEXT_INDEX, &_basilTextIndexSegmentRomStart, &_basilTextIndexSegmentRomEnd, &_basilTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ELLEN_TEXT_INDEX, &_ellenTextIndexSegmentRomStart, &_ellenTextIndexSegmentRomEnd, &_ellenTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(DOUG_TEXT_INDEX, &_dougTextIndexSegmentRomStart, &_dougTextIndexSegmentRomEnd, &_dougTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(GOTZ_TEXT_INDEX, &_gotzTextIndexSegmentRomStart, &_gotzTextIndexSegmentRomEnd, &_gotzTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(SASHA_TEXT_INDEX, &_sashaTextIndexSegmentRomStart, &_sashaTextIndexSegmentRomEnd, &_sashaTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(POTION_SHOP_DEALER_TEXT_INDEX, &_potionShopDealerTextIndexSegmentRomStart, &_potionShopDealerTextIndexSegmentRomEnd, &_potionShopDealerTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(KENT_TEXT_INDEX, &_kentTextIndexSegmentRomStart, &_kentTextIndexSegmentRomEnd, &_kentTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(STU_TEXT_INDEX, &_stuTextIndexSegmentRomStart, &_stuTextIndexSegmentRomEnd, &_stuTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MIDWIFE_TEXT_INDEX, &_midwifeTextIndexSegmentRomStart, &_midwifeTextIndexSegmentRomEnd, &_midwifeTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MAY_TEXT_INDEX, &_mayTextIndexSegmentRomStart, &_mayTextIndexSegmentRomEnd, &_mayTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(RICK_TEXT_INDEX, &_rickTextIndexSegmentRomStart, &_rickTextIndexSegmentRomEnd, &_rickTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(PASTOR_TEXT_INDEX, &_pastorTextIndexSegmentRomStart, &_pastorTextIndexSegmentRomEnd, &_pastorTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(SHIPPER_TEXT_INDEX, &_shipperTextIndexSegmentRomStart, &_shipperTextIndexSegmentRomEnd, &_shipperTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(SAIBARA_TEXT_INDEX, &_saibaraTextIndexSegmentRomStart, &_saibaraTextIndexSegmentRomEnd, &_saibaraTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(DUKE_TEXT_INDEX, &_dukeTextIndexSegmentRomStart, &_dukeTextIndexSegmentRomEnd, &_dukeTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(GREG_TEXT_INDEX, &_gregTextIndexSegmentRomStart, &_gregTextIndexSegmentRomEnd, &_gregTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ASSISTANT_CARPENTERS_TEXT_INDEX, &_assistantCarpentersTextIndexSegmentRomStart, &_assistantCarpentersTextIndexSegmentRomEnd, &_assistantCarpentersTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ASSISTANT_CARPENTERS_TEXT_INDEX, &_assistantCarpentersTextIndexSegmentRomStart, &_assistantCarpentersTextIndexSegmentRomEnd, &_assistantCarpentersTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MASTER_CARPENTER_TEXT_INDEX, &_masterCarpenterTextIndexSegmentRomStart, &_masterCarpenterTextIndexSegmentRomEnd, &_masterCarpenterTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(HARVEST_SPRITES_TEXT_INDEX, &_harvestSpritesTextIndexSegmentRomStart, &_harvestSpritesTextIndexSegmentRomEnd, &_harvestSpritesTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(HARVEST_SPRITES_TEXT_INDEX, &_harvestSpritesTextIndexSegmentRomStart, &_harvestSpritesTextIndexSegmentRomEnd, &_harvestSpritesTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(HARVEST_SPRITES_TEXT_INDEX, &_harvestSpritesTextIndexSegmentRomStart, &_harvestSpritesTextIndexSegmentRomEnd, &_harvestSpritesTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(BARLEY_TEXT_INDEX, &_barleyTextIndexSegmentRomStart, &_barleyTextIndexSegmentRomEnd, &_barleyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(SYDNEY_TEXT_INDEX, &_sydneyTextIndexSegmentRomStart, &_sydneyTextIndexSegmentRomEnd, &_sydneyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MRS_MANA_AND_JOHN_TEXT_INDEX, &_mrsManaAndJohnTextIndexSegmentRomStart, &_mrsManaAndJohnTextIndexSegmentRomEnd, &_mrsManaAndJohnTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MRS_MANA_AND_JOHN_TEXT_INDEX, &_mrsManaAndJohnTextIndexSegmentRomStart, &_mrsManaAndJohnTextIndexSegmentRomEnd, &_mrsManaAndJohnTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(ADDITIONAL_NPCS_TEXT_INDEX, &_additionalNpcsTextIndexSegmentRomStart, &_additionalNpcsTextIndexSegmentRomEnd, &_additionalNpcsTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    
    // unnecessary setting for player baby and each NPC baby
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    
    // unnecessary additional load
    setTextAddresses(ADDITIONAL_NPCS_TEXT_INDEX, &_additionalNpcsTextIndexSegmentRomStart, &_additionalNpcsTextIndexSegmentRomEnd, &_additionalNpcsTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    
    // cutscenes and festivals
    setTextAddresses(FARM_VISITS_TEXT_INDEX, &_farmVisitsTextIndexSegmentRomStart, &_farmVisitsTextIndexSegmentRomEnd, &_farmVisitsTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_65_TEXT_INDEX, &_text65TextIndexSegmentRomStart, &_text65TextIndexSegmentRomEnd, &_text65TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(CUTSCENES_1_TEXT_INDEX, &_cutscenes1TextIndexSegmentRomStart, &_cutscenes1TextIndexSegmentRomEnd, &_cutscenes1TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_54_TEXT_INDEX, &_text54TextIndexSegmentRomStart, &_text54TextIndexSegmentRomEnd, &_text54TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(MARRIED_DIALOGUES_TEXT_INDEX, &_marriedDialoguesTextIndexSegmentRomStart, &_marriedDialoguesTextIndexSegmentRomEnd, &_marriedDialoguesTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(VILLAGE_2_CUTSCENES_TEXT_INDEX, &_text61TextIndexSegmentRomStart, &_text61TextIndexSegmentRomEnd, &_text61TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(VINEYARD_CUTSCENES_TEXT_INDEX, &_vineyardCutscenesTextIndexSegmentRomStart, &_vineyardCutscenesTextIndexSegmentRomEnd, &_vineyardCutscenesTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_63_TEXT_INDEX, &_text63TextIndexSegmentRomStart, &_text63TextIndexSegmentRomEnd, &_text63TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(BEACH_CUTSCENES_TEXT_INDEX, &_beachCutscenesTextIndexSegmentRomStart, &_beachCutscenesTextIndexSegmentRomEnd, &_beachCutscenesTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_55_TEXT_INDEX, &_text55TextIndexSegmentRomStart, &_text55TextIndexSegmentRomEnd, &_text55TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(HORSE_RACE_TEXT_INDEX, &_horseRaceTextIndexSegmentRomStart, &_horseRaceTextIndexSegmentRomEnd, &_horseRaceTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_49_TEXT_INDEX, &_text49TextIndexSegmentRomStart, &_text49TextIndexSegmentRomEnd, &_text49TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(VEGETABLE_FESTIVAL_TEXT_INDEX, &_vegetableFestivalTextIndexSegmentRomStart, &_vegetableFestivalTextIndexSegmentRomEnd, &_vegetableFestivalTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_48_TEXT_INDEX, &_text48TextIndexSegmentRomStart, &_text48TextIndexSegmentRomEnd, &_text48TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_52_TEXT_INDEX, &_text52TextIndexSegmentRomStart, &_text52TextIndexSegmentRomEnd, &_text52TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_50_TEXT_INDEX, &_text50TextIndexSegmentRomStart, &_text50TextIndexSegmentRomEnd, &_text50TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_51_TEXT_INDEX, &_text51TextIndexSegmentRomStart, &_text51TextIndexSegmentRomEnd, &_text51TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(TEXT_56_TEXT_INDEX, &_text56TextIndexSegmentRomStart, &_text56TextIndexSegmentRomEnd, &_text56TextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(EGG_FESTIVAL_TEXT_INDEX, &_eggFestivalTextIndexSegmentRomStart, &_eggFestivalTextIndexSegmentRomEnd, &_eggFestivalTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(DOG_RACE_TEXT_INDEX, &_dogRaceTextIndexSegmentRomStart, &_dogRaceTextIndexSegmentRomEnd, &_dogRaceTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(SPIRIT_FESTIVAL_TEXT_INDEX, &_spiritFestivalTextIndexSegmentRomStart, &_spiritFestivalTextIndexSegmentRomEnd, &_spiritFestivalTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(NEW_YEAR_FESTIVAL_TEXT_INDEX, &_newYearFestivalTextIndexSegmentRomStart, &_newYearFestivalTextIndexSegmentRomEnd, &_newYearFestivalTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    setTextAddresses(FUNERAL_INTRO_TEXT_INDEX, &_funeralIntroTextIndexSegmentRomStart, &_funeralIntroTextIndexSegmentRomEnd, &_funeralIntroTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);
    
    setTextAddresses(HOW_TO_PLAY_TEXT_INDEX, &_howToPlayTextIndexSegmentRomStart, &_howToPlayTextIndexSegmentRomEnd, &_howToPlayTextSegmentRomStart, (u32*)TEXT_ADDRESSES_INDEX_BUFFER);

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeGameVariableStrings);

void initializeGameVariableStrings(void) {
    
    // also set as current naming screen name
    setGameVariableString(0, gPlayer.name, 6);
    setGameVariableString(1, gFarmName, 6);
    setGameVariableString(2, dogInfo.name, 6);
    setGameVariableString(3, horseInfo.name, 6);
    setGameVariableString(4, gBabyName, 6); 
     
    setGameVariableString(5, gFarmAnimals[0].name, 6);
    setGameVariableString(6, gFarmAnimals[1].name, 6);
    setGameVariableString(7, gFarmAnimals[2].name, 6);
    setGameVariableString(8, gFarmAnimals[3].name, 6);
    setGameVariableString(9, gFarmAnimals[4].name, 6);
    setGameVariableString(0xA, gFarmAnimals[5].name, 6);
    setGameVariableString(0xB, gFarmAnimals[6].name, 6);
    setGameVariableString(0xC, gFarmAnimals[7].name, 6);

    // 1-15 repurposed by gameFile.c
    
    // 0xD = chicken or farm animal name

    // cow name
    setGameVariableString(0xE, D_80237380, 6);
    
    setGameVariableString(0xF, gWifeName, 6);
    
    // unused
    setGameVariableString(0x10, D_801FB5C4, 6);

    // watering can use/seed use counter
    setGameVariableString(0x11, D_801594E0, 3);
    // price
    setGameVariableString(0x12, D_801A8B50, 8);

    setGameVariableString(0x13, gDeadAnimalName, 6);

    setGameVariableString(0x14, gCurrentSeasonName, 6);
    
    // anniversary date; animal birth countdown; animal death dat
    setGameVariableString(0x15, D_801FC152, 2);

    setGameVariableString(0x16, gWifeName, 6);
    
    // fodder amount
    setGameVariableString(0x17, D_80205210, 3);
    // lumber amount
    setGameVariableString(0x18, D_80237418, 3);
    
    setGameVariableString(0x19, gChickens[0].name, 6);
    setGameVariableString(0x1A, gChickens[1].name, 6);
    setGameVariableString(0x1B, gChickens[2].name, 6);
    setGameVariableString(0x1C, gChickens[3].name, 6);
    setGameVariableString(0x1D, gChickens[4].name, 6);
    setGameVariableString(0x1E, gChickens[5].name, 6);
    
    // horse race medal pieces
    setGameVariableString(0x1F, D_8020563B, 4);

    setGameVariableString(0x20, gHarvestKingName, 6);

    setGameVariableString(0x21, D_801806C8, 2);

    // unused
    setGameVariableString(0x22, D_80170268, 6);
    setGameVariableString(0x23, D_801FC156, 6);
    setGameVariableString(0x24, D_80204B3C, 6);

    setGameVariableString(0x25, gMilkTypeString, 1);

    // 0x26 = farm animal mother name

    // unused
    setGameVariableString(0x27, D_801886D4, 6);

    // 0x2A-0x2F set by overlay screens (horse/dog race)

    // flower card points
    setGameVariableString(0x30, D_801594E6, 3);
    // bakery card points
    setGameVariableString(0x31, D_8016F6E0, 3);

    // girls' and rivals' affection
    setGameVariableString(0x32, D_80182D90[0], 3);
    setGameVariableString(0x33, D_80182D90[1], 3);
    setGameVariableString(0x34, D_80182D90[2], 3);
    setGameVariableString(0x35, D_80182D90[3], 3);
    setGameVariableString(0x36, D_80182D90[4], 3);
    setGameVariableString(0x37, D_80182D90[5], 3);
    setGameVariableString(0x38, D_80182D90[6], 3);
    setGameVariableString(0x39, D_80182D90[7], 3);
    setGameVariableString(0x3A, D_80182D90[8], 3);
    setGameVariableString(0x3B, D_80182D90[9], 3);
    setGameVariableString(0x3C, D_80182D90[10], 3);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeDialogueSystem);

void initializeDialogueSystem(void) {

    setDialogueBytecodeAddresses();

    initializeDialogueVariables();

    setSpecialDialogueBitsPointer(specialDialogueBits);

    initializeDialogueSessionManager(0, 0, 1);

    setDialogueButtonIcon1(0, DIALOGUE_ICONS_1, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, DIALOGUE_ICON_TEXTURE_BUFFER, DIALOGUE_ICON_PALETTE_BUFFER, DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 3, 0, 8.0f, -16.0f, 0);
    setDialogueButtonIcon2(0, DIALOGUE_ICONS_2, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, DIALOGUE_ICON_TEXTURE_BUFFER, DIALOGUE_ICON_PALETTE_BUFFER, DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0xA, 0xFE, 0, 40.0f, 0);
    setDialogueButtonIcon3(0, DIALOGUE_ICONS_3, &_dialogueButtonIconsTextureSegmentRomStart, &_dialogueButtonIconsTextureSegmentRomEnd, &_dialogueButtonIconsAssetsIndexSegmentRomStart, &_dialogueButtonIconsAssetsIndexSegmentRomEnd, DIALOGUE_ICON_TEXTURE_BUFFER, DIALOGUE_ICON_PALETTE_BUFFER, DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0xB, 0xFE, 0, -40.0f, 0);
   
    setDialogueSfxIndices(0, 2, 0, 8);

}
 
//INCLUDE_ASM("asm/nonmatchings/game/initialize", setDialogueBytecodeAddresses);

void setDialogueBytecodeAddresses(void) {
    
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT1, TEXT_1_TEXT_INDEX, TEXT_1_TEXT_INDEX, &_text1DialogueIndexSegmentRomStart, &_text1DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text1DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_DIARY, DIARY_TEXT_INDEX, DIARY_TEXT_INDEX, &_diaryDialogueIndexSegmentRomStart, &_diaryDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_diaryDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_SHOP, SHOP_TEXT_INDEX, SHOP_TEXT_INDEX, &_shopDialogueIndexSegmentRomStart, &_shopDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_shopDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_FESTIVAL_OVERLAY_SELECTIONS, FESTIVALS_TEXT_INDEX, FESTIVALS_TEXT_INDEX, &_festivalOverlaySelectionsDialogueIndexSegmentRomStart, &_festivalOverlaySelectionsDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_festivalOverlaySelectionsDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_LIBRARY, LIBRARY_TEXT_INDEX, LIBRARY_TEXT_INDEX, &_libraryDialogueIndexSegmentRomStart, &_libraryDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_libraryDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MARIA, MARIA_TEXT_INDEX, MARIA_TEXT_INDEX, &_mariaDialogueIndexSegmentRomStart, &_mariaDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_mariaDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_POPURI, POPURI_TEXT_INDEX, POPURI_TEXT_INDEX, &_popuriDialogueIndexSegmentRomStart, &_popuriDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_popuriDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ELLI, ELLI_TEXT_INDEX, ELLI_TEXT_INDEX, &_elliDialogueIndexSegmentRomStart, &_elliDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_elliDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ANN, ANN_TEXT_INDEX, ANN_TEXT_INDEX, &_annDialogueIndexSegmentRomStart, &_annDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_annDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_KAREN, KAREN_TEXT_INDEX, KAREN_TEXT_INDEX, &_karenDialogueIndexSegmentRomStart, &_karenDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_karenDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_BABY, BABY_TEXT_INDEX, BABY_TEXT_INDEX, &_babyDialogueIndexSegmentRomStart, &_babyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_babyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_HARRIS, HARRIS_TEXT_INDEX, HARRIS_TEXT_INDEX, &_harrisDialogueIndexSegmentRomStart, &_harrisDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_harrisDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_GRAY, GRAY_TEXT_INDEX, GRAY_TEXT_INDEX, &_grayDialogueIndexSegmentRomStart, &_grayDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_grayDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_JEFF, JEFF_TEXT_INDEX, JEFF_TEXT_INDEX, &_jeffDialogueIndexSegmentRomStart, &_jeffDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_jeffDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_CLIFF, CLIFF_TEXT_INDEX, CLIFF_TEXT_INDEX, &_cliffDialogueIndexSegmentRomStart, &_cliffDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_cliffDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_KAI, KAI_TEXT_INDEX, KAI_TEXT_INDEX, &_kaiDialogueIndexSegmentRomStart, &_kaiDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_kaiDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MAYOR, MAYOR_TEXT_INDEX, MAYOR_TEXT_INDEX, &_mayorDialogueIndexSegmentRomStart, &_mayorDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_mayorDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MAYOR_WIFE, MAYOR_WIFE_TEXT_INDEX, MAYOR_WIFE_TEXT_INDEX, &_mayorWifeDialogueIndexSegmentRomStart, &_mayorWifeDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_mayorWifeDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_LILLIA, LILLIA_TEXT_INDEX, LILLIA_TEXT_INDEX, &_lilliaDialogueIndexSegmentRomStart, &_lilliaDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_lilliaDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_BASIL, BASIL_TEXT_INDEX, BASIL_TEXT_INDEX, &_basilDialogueIndexSegmentRomStart, &_basilDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_basilDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ELLEN, ELLEN_TEXT_INDEX, ELLEN_TEXT_INDEX, &_ellenDialogueIndexSegmentRomStart, &_ellenDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_ellenDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_DOUG, DOUG_TEXT_INDEX, DOUG_TEXT_INDEX, &_dougDialogueIndexSegmentRomStart, &_dougDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_dougDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_GOTZ, GOTZ_TEXT_INDEX, GOTZ_TEXT_INDEX, &_gotzDialogueIndexSegmentRomStart, &_gotzDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_gotzDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_SASHA, SASHA_TEXT_INDEX, SASHA_TEXT_INDEX, &_sashaDialogueIndexSegmentRomStart, &_sashaDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_sashaDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_POTION_SHOP_DEALER, POTION_SHOP_DEALER_TEXT_INDEX, POTION_SHOP_DEALER_TEXT_INDEX, &_potionShopDealerDialogueIndexSegmentRomStart, &_potionShopDealerDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_potionShopDealerDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_KENT, KENT_TEXT_INDEX, KENT_TEXT_INDEX, &_kentDialogueIndexSegmentRomStart, &_kentDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_kentDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_STU, STU_TEXT_INDEX, STU_TEXT_INDEX, &_stuDialogueIndexSegmentRomStart, &_stuDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_stuDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MIDWIFE, MIDWIFE_TEXT_INDEX, MIDWIFE_TEXT_INDEX, &_midwifeDialogueIndexSegmentRomStart, &_midwifeDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_midwifeDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MAY, MAY_TEXT_INDEX, MAY_TEXT_INDEX, &_mayDialogueIndexSegmentRomStart, &_mayDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_mayDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_RICK, RICK_TEXT_INDEX, RICK_TEXT_INDEX, &_rickDialogueIndexSegmentRomStart, &_rickDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_rickDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_PASTOR, PASTOR_TEXT_INDEX, PASTOR_TEXT_INDEX, &_pastorDialogueIndexSegmentRomStart, &_pastorDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_pastorDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_SHIPPER, SHIPPER_TEXT_INDEX, SHIPPER_TEXT_INDEX, &_shipperDialogueIndexSegmentRomStart, &_shipperDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_shipperDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_SAIBARA, SAIBARA_TEXT_INDEX, SAIBARA_TEXT_INDEX, &_saibaraDialogueIndexSegmentRomStart, &_saibaraDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_saibaraDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_DUKE, DUKE_TEXT_INDEX, DUKE_TEXT_INDEX, &_dukeDialogueIndexSegmentRomStart, &_dukeDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_dukeDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_GREG, GREG_TEXT_INDEX, GREG_TEXT_INDEX, &_gregDialogueIndexSegmentRomStart, &_gregDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_gregDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ASSISTANT_CARPENTERS_1, ASSISTANT_CARPENTERS_TEXT_INDEX, ASSISTANT_CARPENTERS_TEXT_INDEX, &_assistantCarpenters1DialogueIndexSegmentRomStart, &_assistantCarpenters1DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_assistantCarpenters1DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ASSISTANT_CARPENTERS_2, ASSISTANT_CARPENTERS_TEXT_INDEX, ASSISTANT_CARPENTERS_TEXT_INDEX, &_assistantCarpenters2DialogueIndexSegmentRomStart, &_assistantCarpenters2DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_assistantCarpenters2DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MASTER_CARPENTER, MASTER_CARPENTER_TEXT_INDEX, MASTER_CARPENTER_TEXT_INDEX, &_masterCarpenterDialogueIndexSegmentRomStart, &_masterCarpenterDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_masterCarpenterDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_HARVEST_SPRITE_1, HARVEST_SPRITES_TEXT_INDEX, HARVEST_SPRITES_TEXT_INDEX, &_harvestSprites1DialogueIndexSegmentRomStart, &_harvestSprites1DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_harvestSprites1DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_HARVEST_SPRITE_2, HARVEST_SPRITES_TEXT_INDEX, HARVEST_SPRITES_TEXT_INDEX, &_harvestSprites2DialogueIndexSegmentRomStart, &_harvestSprites2DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_harvestSprites2DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_HARVEST_SPRITE_3, HARVEST_SPRITES_TEXT_INDEX, HARVEST_SPRITES_TEXT_INDEX, &_harvestSprites3DialogueIndexSegmentRomStart, &_harvestSprites3DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_harvestSprites3DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_BARLEY, BARLEY_TEXT_INDEX, BARLEY_TEXT_INDEX, &_barleyDialogueIndexSegmentRomStart, &_barleyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_barleyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_SYDNEY, SYDNEY_TEXT_INDEX, SYDNEY_TEXT_INDEX, &_sydneyDialogueIndexSegmentRomStart, &_sydneyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_sydneyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MRS_MANA_AND_JOHN_1, MRS_MANA_AND_JOHN_TEXT_INDEX, MRS_MANA_AND_JOHN_TEXT_INDEX, &_mrsManaAndJohn1DialogueIndexSegmentRomStart, &_mrsManaAndJohn1DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_mrsManaAndJohn1DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MRS_MANA_AND_JOHN_2, MRS_MANA_AND_JOHN_TEXT_INDEX, MRS_MANA_AND_JOHN_TEXT_INDEX, &_mrsManaAndJohn2DialogueIndexSegmentRomStart, &_mrsManaAndJohn2DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_mrsManaAndJohn2DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ADDITIONAL_NPCS_1, ADDITIONAL_NPCS_TEXT_INDEX, ADDITIONAL_NPCS_TEXT_INDEX, &_additionalNpcs1DialogueIndexSegmentRomStart, &_additionalNpcs1DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_additionalNpcs1DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NPC_BABY, NPC_BABY_TEXT_INDEX, NPC_BABY_TEXT_INDEX, &_npcBabyDialogueIndexSegmentRomStart, &_npcBabyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_npcBabyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NPC_BABY, NPC_BABY_TEXT_INDEX, NPC_BABY_TEXT_INDEX, &_npcBabyDialogueIndexSegmentRomStart, &_npcBabyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_npcBabyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NPC_BABY, NPC_BABY_TEXT_INDEX, NPC_BABY_TEXT_INDEX, &_npcBabyDialogueIndexSegmentRomStart, &_npcBabyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_npcBabyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NPC_BABY, NPC_BABY_TEXT_INDEX, NPC_BABY_TEXT_INDEX, &_npcBabyDialogueIndexSegmentRomStart, &_npcBabyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_npcBabyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NPC_BABY, NPC_BABY_TEXT_INDEX, NPC_BABY_TEXT_INDEX, &_npcBabyDialogueIndexSegmentRomStart, &_npcBabyDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_npcBabyDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_ADDITIONAL_NPCS_2, ADDITIONAL_NPCS_TEXT_INDEX, ADDITIONAL_NPCS_TEXT_INDEX, &_additionalNpcs2DialogueIndexSegmentRomStart, &_additionalNpcs2DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_additionalNpcs2DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_FARM_VISITS, FARM_VISITS_TEXT_INDEX, FARM_VISITS_TEXT_INDEX, &_farmVisitsDialogueIndexSegmentRomStart, &_farmVisitsDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_farmVisitsDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_65, TEXT_65_TEXT_INDEX, TEXT_65_TEXT_INDEX, &_text65DialogueIndexSegmentRomStart, &_text65DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text65DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_CUTSCENES_1, CUTSCENES_1_TEXT_INDEX, CUTSCENES_1_TEXT_INDEX, &_cutscenes1DialogueIndexSegmentRomStart, &_cutscenes1DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_cutscenes1DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_54, TEXT_54_TEXT_INDEX, TEXT_54_TEXT_INDEX, &_text54DialogueIndexSegmentRomStart, &_text54DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text54DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_MARRIED, MARRIED_DIALOGUES_TEXT_INDEX, MARRIED_DIALOGUES_TEXT_INDEX, &_marriedDialogueIndexSegmentRomStart, &_marriedDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_marriedDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_61, VILLAGE_2_CUTSCENES_TEXT_INDEX, VILLAGE_2_CUTSCENES_TEXT_INDEX, &_text61DialogueIndexSegmentRomStart, &_text61DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text61DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_VINEYARD_CUTSCENES, VINEYARD_CUTSCENES_TEXT_INDEX, VINEYARD_CUTSCENES_TEXT_INDEX, &_vineyardCutscenesDialogueIndexSegmentRomStart, &_vineyardCutscenesDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_vineyardCutscenesDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_63, TEXT_63_TEXT_INDEX, TEXT_63_TEXT_INDEX, &_text63DialogueIndexSegmentRomStart, &_text63DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text63DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_BEACH_CUTSCENES, BEACH_CUTSCENES_TEXT_INDEX, BEACH_CUTSCENES_TEXT_INDEX, &_beachCutscenesDialogueIndexSegmentRomStart, &_beachCutscenesDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_beachCutscenesDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_55, TEXT_55_TEXT_INDEX, TEXT_55_TEXT_INDEX, &_text55DialogueIndexSegmentRomStart, &_text55DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text55DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_HORSE_RACE, HORSE_RACE_TEXT_INDEX, HORSE_RACE_TEXT_INDEX, &_horseRaceDialogueIndexSegmentRomStart, &_horseRaceDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_horseRaceDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_49, TEXT_49_TEXT_INDEX, TEXT_49_TEXT_INDEX, &_text49DialogueIndexSegmentRomStart, &_text49DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text49DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_VEGETABLE_FESTIVAL, VEGETABLE_FESTIVAL_TEXT_INDEX, VEGETABLE_FESTIVAL_TEXT_INDEX, &_vegetableFestivalDialogueIndexSegmentRomStart, &_vegetableFestivalDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_vegetableFestivalDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_48, TEXT_48_TEXT_INDEX, TEXT_48_TEXT_INDEX, &_text48DialogueIndexSegmentRomStart, &_text48DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text48DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_52, TEXT_52_TEXT_INDEX, TEXT_52_TEXT_INDEX, &_text52DialogueIndexSegmentRomStart, &_text52DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text52DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_50, TEXT_50_TEXT_INDEX, TEXT_50_TEXT_INDEX, &_text50DialogueIndexSegmentRomStart, &_text50DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text50DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_51, TEXT_51_TEXT_INDEX, TEXT_51_TEXT_INDEX, &_text51DialogueIndexSegmentRomStart, &_text51DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text51DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_TEXT_56, TEXT_56_TEXT_INDEX, TEXT_56_TEXT_INDEX, &_text56DialogueIndexSegmentRomStart, &_text56DialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_text56DialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_EGG_FESTIVAL, EGG_FESTIVAL_TEXT_INDEX, EGG_FESTIVAL_TEXT_INDEX, &_eggFestivalDialogueIndexSegmentRomStart, &_eggFestivalDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_eggFestivalDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_DOG_RACE, DOG_RACE_TEXT_INDEX, DOG_RACE_TEXT_INDEX, &_dogRaceDialogueIndexSegmentRomStart, &_dogRaceDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_dogRaceDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NEW_YEAR_FESTIVAL, NEW_YEAR_FESTIVAL_TEXT_INDEX, NEW_YEAR_FESTIVAL_TEXT_INDEX, &_newYearFestivalDialogueIndexSegmentRomStart, &_newYearFestivalDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_newYearFestivalDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);
    setDialogueBytecodeAddressInfo(DIALOGUE_NAMING_SCREEN, NAMING_SCREEN_TEXT_INDEX, NAMING_SCREEN_TEXT_INDEX, &_namingScreenDialogueIndexSegmentRomStart, &_namingScreenDialogueIndexSegmentRomEnd, (u32*)DIALOGUE_BYTECODE_INDEX_BUFFER, &_namingScreenDialogueSegmentRomStart, (void*)DIALOGUE_BYTECODE_BUFFER);

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeDialogueVariables);

void initializeDialogueVariables(void) {

    // alcohol tolerance is first lol
    setDialogueVariable(0, &gAlcoholTolerance, UNSIGNED_CHAR, MAX_ALCOHOL_TOLERANCE);

    setDialogueVariable(1, &gSeason, UNSIGNED_CHAR, 4);
    setDialogueVariable(2, &gHour, UNSIGNED_CHAR, 23);
    setDialogueVariable(3, &gWeather, UNSIGNED_CHAR, 5);
    setDialogueVariable(4, &gDayOfWeek, UNSIGNED_CHAR, 6);
    setDialogueVariable(5, &gYear, UNSIGNED_CHAR, 99);
    
    setDialogueVariable(6, &gItemBeingHeld, UNSIGNED_CHAR, TOTAL_ITEMS);

    setDialogueVariable(7, &npcAffection[MARIA], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(8, &npcAffection[POPURI], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(9, &npcAffection[ELLI], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0xA, &npcAffection[ANN], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0xB, &npcAffection[KAREN], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0xC, &npcAffection[HARRIS], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0xD, &npcAffection[GRAY], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0xE, &npcAffection[JEFF], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0xF, &npcAffection[CLIFF], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x10, &npcAffection[KAI], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x11, &npcAffection[MAYOR], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x12, &npcAffection[MAYOR_WIFE], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x13, &npcAffection[LILLIA], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x14, &npcAffection[BASIL], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x15, &npcAffection[ELLEN], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x16, &npcAffection[PASTOR], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x17, &npcAffection[RICK], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x18, &npcAffection[SAIBARA], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x19, &npcAffection[POTION_SHOP_DEALER], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x1A, &npcAffection[KENT], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x1B, &npcAffection[STU], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x1C, &npcAffection[MIDWIFE], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x1D, &npcAffection[MAY], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x1E, &npcAffection[CARPENTER_1], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x1F, &npcAffection[CARPENTER_2], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x20, &npcAffection[MASTER_CARPENTER], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x21, &npcAffection[HARVEST_SPRITE_1], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x22, &npcAffection[SYDNEY], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x23, &npcAffection[BARLEY], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x24, &npcAffection[GREG], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x25, &npcAffection[BABY], UNSIGNED_CHAR, MAX_AFFECTION);

    // sum of girls' affection
    setDialogueVariable(0x26, &D_80215DF0, UNSIGNED_SHORT, MAX_AFFECTION * 5);

    setDialogueVariable(0x27, &gDayOfMonth, UNSIGNED_CHAR, 30);

    setDialogueVariable(0x28, &gGold, UNSIGNED_INT, MAX_GOLD);

    setDialogueVariable(0x29, &gTotalGrassTiles, UNSIGNED_SHORT, 1000);
    setDialogueVariable(0x2A, &gTotalPinkCatMintFlowersGrowing, UNSIGNED_SHORT, 1000);

    // crops shipped
    setDialogueVariable(0x2B, &totalTurnipsShipped, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2C, &totalPotatoesShipped, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2D, &totalCabbageShipped, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2E, &totalTomatoesShipped, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2F, &totalCornShipped, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x30, &totalEggplantsShipped, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);

    setDialogueVariable(0x31, &npcAffection[DOUG], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x32, &npcAffection[GOTZ], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x33, &npcAffection[SASHA], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x34, &npcAffection[SHIPPER], UNSIGNED_CHAR, MAX_AFFECTION);
    setDialogueVariable(0x35, &npcAffection[DUKE], UNSIGNED_CHAR, MAX_AFFECTION);

    setDialogueVariable(0x36, &horseInfo.affection, UNSIGNED_CHAR, MAX_AFFECTION);
    
    setDialogueVariable(0x37, &gFarmAnimals[0].affection, UNSIGNED_CHAR, MAX_AFFECTION);

    setDialogueVariable(0x38, &dogInfo.affection, UNSIGNED_CHAR, MAX_AFFECTION);

    setDialogueVariable(0x39, &gElliGrievingCounter, UNSIGNED_CHAR, 30);

    setDialogueVariable(0x3A, &gBabyAge, UNSIGNED_CHAR, 120);
    setDialogueVariable(0x3B, &gWifeConceptionCounter, UNSIGNED_CHAR, 120);
    setDialogueVariable(0x3C, &gWifeConceptionCounter, UNSIGNED_CHAR, 120);
    setDialogueVariable(0x3D, &gBabyAge, UNSIGNED_CHAR, 120);
    
    // unused
    setDialogueVariable(0x3E, &D_801890D8, UNSIGNED_CHAR, 30);
    
    setDialogueVariable(0x3F, &gBabyAge, UNSIGNED_CHAR, 120);
    
    // unused
    setDialogueVariable(0x40, &D_801C4214, UNSIGNED_CHAR, 30);
    
    setDialogueVariable(0x41, &gBabyAge, UNSIGNED_CHAR, 120);
    setDialogueVariable(0x42, &gBabyAge, UNSIGNED_CHAR, 120);
    
    // unsued
    setDialogueVariable(0x43, &D_80189A48, UNSIGNED_CHAR, 30);
    // unused
    setDialogueVariable(0x44, &D_80180712, UNSIGNED_CHAR, 30);

    setDialogueVariable(0x45, &gWifePregnancyCounter, UNSIGNED_CHAR, 60);
    setDialogueVariable(0x46, &gWifePregnancyCounter, UNSIGNED_CHAR, 60);
    
    // unused
    setDialogueVariable(0x47, &D_8017044B, UNSIGNED_CHAR, 255);
    // unused
    setDialogueVariable(0x48, &D_801C3B64, UNSIGNED_CHAR, 255);

    setDialogueVariable(0x49, &gWifePregnancyCounter, UNSIGNED_CHAR, 60);
    setDialogueVariable(0x4A, &gWifePregnancyCounter, UNSIGNED_CHAR, 60);
    setDialogueVariable(0x4B, &gWifePregnancyCounter, UNSIGNED_CHAR, 60);

}