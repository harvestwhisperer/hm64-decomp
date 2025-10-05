#include "common.h"

#include "game/initialize.h"

#include "ld_symbols.h"

#include "data/animationScripts/animationScripts.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/memory.h"
#include "system/message.h"

#include "game/animals.h" 
#include "game/evaluation.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStart.h"
#include "game/gameStatus.h"
#include "game/transition.h"
#include "game/itemHandlers.h"
#include "game/level.h" 
#include "game/loadGameScreen.h"
#include "mainLoop.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/setCutscenes.h"
#include "game/shop.h"
#include "game/spriteInfo.h"
#include "game/title.h"
#include "game/tv.h"
#include "game/updateGame.h"
#include "game/weather.h"

// unused variables
extern u8 D_8017044B;
extern u8 D_80180712;
extern u8 D_801890D8;
extern u8 D_80189A48;
extern u8 D_801C3B64;
extern u8 D_801C4214;

// unused strings
extern u8 D_801594E0[3];
extern u8 D_801594E6[3];
extern u8 D_8016F6E0[3];
extern u8 D_80170268[6];
extern u8 D_801806C8[2];
extern u8 D_80182D90[10][3];
extern u8 D_801A8B50[8];
extern u8 D_801FB5C4[6];
extern u8 D_801FC152[2];
extern u8 D_801FC156[6];
extern u8 D_80204B3C[6];
extern u8 D_80205210[2];
extern u8 D_8020563B[4];
extern u8 D_80237380[6];
extern u8 D_80237418[3];

// forward declarations
void func_8004F768(void);
void func_80054550(void);
void func_80054734(void);
void initializeGameVariables(void);
void initializeGameVariableStrings(void);
void loadMapAddresses(void);
void registerMainLoopCallbacks(void);
void initializeDialogueVariables(void);
void func_800535DC(void);

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
    gPlayer.unk_60 = 0.0f;
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

// experiment with loop unrolling
static inline void initializeToolchestSlots() {

    gToolchestSlots[0] = 0;
    gToolchestSlots[1] = 0;
    gToolchestSlots[2] = 0;
    gToolchestSlots[3] = 0;
    gToolchestSlots[4] = 0;
    gToolchestSlots[5] = 0;
    gToolchestSlots[6] = 0;
    gToolchestSlots[7] = 0;
    gToolchestSlots[8] = 0;
    gToolchestSlots[9] = 0;
    gToolchestSlots[10] = 0;
    gToolchestSlots[11] = 0;
    gToolchestSlots[12] = 0;
    gToolchestSlots[13] = 0;
    gToolchestSlots[14] = 0;
    gToolchestSlots[15] = 0;
    gToolchestSlots[16] = 0;
    gToolchestSlots[17] = 0;
    gToolchestSlots[18] = 0;
    gToolchestSlots[19] = 0;
    gToolchestSlots[20] = 0;
    gToolchestSlots[21] = 0;    
    gToolchestSlots[22] = 0;
    gToolchestSlots[23] = 0;
    gToolchestSlots[24] = 0;
    gToolchestSlots[25] = 0;
    gToolchestSlots[26] = 0;
    gToolchestSlots[27] = 0;
       
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", func_8004E160);

void func_8004E160(void) {
    
    volatile musConfig c;
    
    c.fxs = sfxList;
    c.priority = musPriorityList;
     
    initializeAudio(&c);

    initializeGameVariables();

    gCurrentGameIndex = 0; 
    
    registerMainLoopCallbacks();
    func_8004F768();
    loadMapAddresses();
    func_80053088();
    func_80054550();
    setEntitiesColor(0, 0, 0, 0);
    func_8003BE98(MAIN_MAP_INDEX, 0, 0, 0, 0);

    setMainLoopCallbackFunctionIndex(MAIN_GAME);
    
    initializeWaveTable(0);

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeGameVariables);

// TODO: there might be some static inlines for each group being initialized
void initializeGameVariables(void) {

    u8 i, j, k;

    gBaseMapIndex = 0xFF;
    gCurrentSongIndex = 0xFF;
    
    gSongVolume = 128;
    
    previousMapIndex = 0xFF;
    gEntranceIndex = 0xFF;
    previousEntranceIndex = 0xFF;
    
    demoCutsceneIndex = 0;
    
    gWife = 0xFF;

    resetGlobalLighting();

    setStartingTime();
    
    gGold = 300;
    
    dailyShippingBinValue = 0;
    
    gWeather = SUNNY;
    gForecast = SUNNY;
    gSeasonTomorrow = SPRING;

    gameLoopContext.unk_6 = 0;
    gameLoopContext.unk_2 = 0;
    gameLoopContext.unk_4 = 0;

    D_8018981C = 0xFFFF;
    gItemBeingHeld = 0xFF;

    gCutsceneFlags = 0;

    gMaximumStamina = 100;
    gPlayer.currentStamina = 100;

    gHarvestKing = 6;
    gHarvestCoinFinder = 0xFF;
    gFlowerFestivalGoddess = 0xFF;
    D_80189054 = 0xFF;
    gVoteForFlowerFestivalGoddess = 0xFF;
    spiritFestivalAssistant1 = 0;
    spiritFestivalAssistant2 = 0;
    
    spiritFestivalAssistant3 = 0;

    gHappiness = 0;

    blueMistFlowerPlot = 0;

    gSickDays = 0;

    D_8013DC2E = 0;

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
        npcs[i].unk_1A = 0;
        npcs[i].unk_1B = 0;
        npcs[i].levelIndex = 0;
        npcs[i].movingFlag = 0;
        npcs[i].unk_1E = 0;
        npcs[i].direction = SOUTHWEST;
        npcs[i].speed = 0;
        npcs[i].unk_21 = 0;
        npcs[i].unk_22 = 0;
        npcs[i].location = 0;
    }

    for (i = 0; i < MAX_NPCS; i++) {
        npcAffection[i] = 0;
    }

    dogInfo.coordinates.x = 0;
    dogInfo.coordinates.y = 0;
    dogInfo.coordinates.z = 0;
    dogInfo.location = FARM;
    dogInfo.unk_17 = 0;
    dogInfo.direction = SOUTHWEST;
    dogInfo.speed = 0;
    dogInfo.unk_1A = 0;
    dogInfo.unk_1B = 0;
    dogInfo.name[0] = 0xF6;
    dogInfo.name[1] = 0xF6;
    dogInfo.name[2] = 0xF6;
    dogInfo.name[3] = 0xF6;
    dogInfo.name[4] = 0xF6;
    dogInfo.name[5] = 0xF6;
    dogInfo.unk_1C = 0xFF;
    dogInfo.flags = 0;
    
    initializeDog();

    horseInfo.coordinates.x = 0;
    horseInfo.coordinates.y = 0;
    horseInfo.coordinates.z = 0;
    horseInfo.location = FARM;
    horseInfo.unk_17 = 0;
    horseInfo.direction = SOUTHWEST;
    horseInfo.speed = 0;
    horseInfo.unk_1A = 0;
    horseInfo.unk_1B = 0;
    horseInfo.age = 0;
    horseInfo.name[0] = 0xF6;
    horseInfo.name[1] = 0xF6;
    horseInfo.name[2] = 0xF6;
    horseInfo.name[3] = 0xF6;
    horseInfo.name[4] = 0xF6;
    horseInfo.name[5] = 0xF6;
    horseInfo.unk_1E = 0xFF;
    horseInfo.flags = 0;

    for (i = 0; i < MAX_CHICKENS; i++) {

        gChickens[i].location = 0;
        gChickens[i].unk_17 = 0;
        gChickens[i].direction = SOUTHWEST;
        gChickens[i].unk_19 = 0;
        gChickens[i].unk_1A = 0;
        gChickens[i].unk_1B = 0;
        gChickens[i].type = 0;
        gChickens[i].condition = 0;
        gChickens[i].unk_1E = 0;
        gChickens[i].starvedCounter = 0;

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
         
        gFarmAnimals[i].unk_1B = 0;
        gFarmAnimals[i].direction = 0;
        gFarmAnimals[i].unk_14 = 0;
        gFarmAnimals[i].unk_1D = 0;
        gFarmAnimals[i].unk_1E = 0;
         
        gFarmAnimals[i].type = 0;
        gFarmAnimals[i].condition = 0;
        gFarmAnimals[i].age = 0;
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
          
        gFarmAnimals[i].unk_23[0] = 0xFF;
        gFarmAnimals[i].unk_23[1] = 0xFF;
        gFarmAnimals[i].unk_23[2] = 0xFF;
        gFarmAnimals[i].unk_23[3] = 0xFF;
        gFarmAnimals[i].unk_23[4] = 0xFF;
        gFarmAnimals[i].unk_23[5] = 0xFF;
         
        gFarmAnimals[i].birthdaySeason = 0;
        gFarmAnimals[i].birthdayDayOfMonth = 0;
         
        gFarmAnimals[i].goldenMilk = 0xFF;
    }

    for (i = 0; i < MAX_CHICKEN_EGGS; i++) {

        gBirds[i].unk_15 = 0;
        gBirds[i].mapIndex = 0;
        gBirds[i].unk_F = 0;
        gBirds[i].direction = 0;
        gBirds[i].zDisplacement = 0;
        gBirds[i].unk_13 = 0;
        gBirds[i].unk_14 = 0;
        gBirds[i].coordinates.x = 0;
        gBirds[i].coordinates.y = 0;
        gBirds[i].coordinates.z = 0;
        gBirds[i].flags = 0;

    }

   // func_800D51B0 from itemHandlers.c 

    for (i = 0; i < 10; i++) {

        itemInfo[i].unk_0.x = 0;
        itemInfo[i].unk_0.y = 0;
        itemInfo[i].unk_0.z = 0;
        
        itemInfo[i].unk_C.x = 0;
        itemInfo[i].unk_C.y = 0;
        itemInfo[i].unk_C.z = 0;
        
        itemInfo[i].unk_18.x = 0;
        itemInfo[i].unk_18.y = 0;
        itemInfo[i].unk_18.z = 0;
        
        itemInfo[i].unk_24 = 0;
        itemInfo[i].itemAnimationFrameCounter = 0;
        itemInfo[i].heldItemIndex = 0;
        
        itemInfo[i].unk_2A = 0;
        itemInfo[i].flags = 0;

    }
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.unk_3 = 0;
    overlayScreenTable.subscreen = 0;

    initializeTV();

    D_801FC154 = 0;
    D_80204DF4 = 0;
    D_80237458 = 0;
    D_8018A725 = 0;
    D_802373E8 = 0;
    D_8013DC2C = 0;
    D_801FAD91 = 0;
    D_801C3E28 = 0;
    D_801C3F70 = 0;
    D_80205636 = 0;
    D_802373A8 = 0;
    wateringCanUses = 0;
    gLumber = 0;
    fodderQuantity = 0;
    chickenFeedQuantity = 0;

    initializeToolchestSlots();

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

    for (i = 0; i < 0x20; i++) {
        lifeEventBits[i] = 0;
    }

    for (i = 0; i < 0x20; i++) {
        dailyEventBits[i] = 0;
    }
    
    for (i = 0; i < 0x10; i++) {
        specialDialogueBits[i] = 0;
    }    

    setInitialSpecialDialogueBits();
    
    memcpy(farmFieldTiles , D_80113580, 0x1E0);

    memcpy(greenhouseFieldTiles, D_80113760, 0x1E0);
        
    albumBits |= 1;

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", registerMainLoopCallbacks);

void registerMainLoopCallbacks(void) {

    registerMainLoopCallback(MAIN_GAME, mainGameLoopCallback);
    registerMainLoopCallback(MAP_LOAD, func_8005D0BC);
    // cutscene prep
    registerMainLoopCallback(3, func_80060490); 
    registerMainLoopCallback(4, func_8005C00C);
    registerMainLoopCallback(LEVEL_LOAD_1, func_8005CA64);
    registerMainLoopCallback(LEVEL_LOAD_2, func_8005CAA8);
    registerMainLoopCallback(OVERLAY_SCREEN_LOAD, func_8005CB50);
    registerMainLoopCallback(ROTATING, func_8005CBA4);
    registerMainLoopCallback(DIALOGUE, func_8005CBF0);
    registerMainLoopCallback(TEXT, func_8005CEFC);
    registerMainLoopCallback(0xB, func_8005CF4C);
    registerMainLoopCallback(PINK_OVERLAY_TEXT, func_8005D2B0);
    registerMainLoopCallback(END_OF_DAY_1, func_80060624);
    registerMainLoopCallback(END_OF_FESTIVAL_DAY_1, func_800604B0);
    registerMainLoopCallback(END_OF_FESTIVAL_DAY_2, func_800605F0);
    registerMainLoopCallback(END_OF_DAY_2, func_80060838);
    registerMainLoopCallback(WAIT_AUDIO_FINISH, func_80060454);
    registerMainLoopCallback(TV, tvMainLoopCallback);
    registerMainLoopCallback(SHOP_DIALOGUE, func_800DCAB8); 
    registerMainLoopCallback(PAUSE_SCREEN_LOAD, func_800B881C);
    registerMainLoopCallback(TOOLBOX_LOAD, func_800BADD0);
    registerMainLoopCallback(FREEZER_LOAD, func_800BCA9C);
    registerMainLoopCallback(CABINET_LOAD, func_800BE808);
    registerMainLoopCallback(HOME_EXTENSIONS_SELECT_LOAD, func_800B82AC);
    registerMainLoopCallback(ESTIMATE_LOAD, func_800B8018);
    registerMainLoopCallback(KITCHEN_PICTURE_LOAD, func_800B815C);
    registerMainLoopCallback(CALENDAR_LOAD, func_800B83F0);
    registerMainLoopCallback(RECIPE_BOOK_LOAD, func_800B8554);
    registerMainLoopCallback(0x1D, func_800B86B8);
    registerMainLoopCallback(0x1E, func_800C8424);
    registerMainLoopCallback(0x1F, func_800C88F4);
    registerMainLoopCallback(HORSE_RACE_RESULTS_LOAD, func_800CC518);
    registerMainLoopCallback(HORSE_RACE_GIFTS_LOAD, func_800CD928);
    registerMainLoopCallback(LOTTERY_LOAD, func_800CE930);
    registerMainLoopCallback(PAUSE_SCREEN, func_800B9D3C);
    registerMainLoopCallback(TOOLBOX, func_800BBEC0);
    registerMainLoopCallback(FREEZER, func_800BDB24);
    registerMainLoopCallback(CABINET, func_800BF990);
    registerMainLoopCallback(HOME_EXTENSIONS_SELECT, func_800C1124);
    registerMainLoopCallback(ESTIMATE, func_800C224C);
    registerMainLoopCallback(KITCHEN_PICTURE, func_800C2B8C);
    registerMainLoopCallback(CALENDAR, func_800C3D20);
    registerMainLoopCallback(RECIPE_BOOK, func_800C53C0);
    registerMainLoopCallback(ALBUM, func_800C7058);
    registerMainLoopCallback(0x2D, func_800C8784);
    registerMainLoopCallback(HORSE_RACE_BETTING, func_800CA808);
    registerMainLoopCallback(HORSE_RACE_RESULTS, func_800CD750);
    registerMainLoopCallback(HORSE_RACE_GIFTS, func_800CE068);
    registerMainLoopCallback(LOTTERY_2, func_800CEDF0);
    registerMainLoopCallback(TITLE_SCREEN, titleScreenMainLoopCallback);
    registerMainLoopCallback(NAMING_SCREEN, func_8005CF94);
    registerMainLoopCallback(0x34, func_800ED974);
    registerMainLoopCallback(SELECT_GAME, func_800E1FAC);
    registerMainLoopCallback(ESTIMATE_SCREEN, func_800E8F08);
    registerMainLoopCallback(0x37, func_800657B4);
    registerMainLoopCallback(0x38, func_800657C4);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", func_8004F768);

void func_8004F768(void) {
    
    initializeShadowSprite(0, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsIndexSegmentRomStart, &_shadowsIndexSegmentRomEnd, (void*)SHADOW_TEXTURE_VADDR_START, (void*)SHADOW_TEXTURE_VADDR_END, (void*)SHADOW_ASSETS_INDEX_VADDR_START, (void*)SHADOW_ASSETS_INDEX_VADDR_END, 0, 0);
    initializeShadowSprite(1, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsIndexSegmentRomStart, &_shadowsIndexSegmentRomEnd, (void*)SHADOW_TEXTURE_VADDR_START, (void*)SHADOW_TEXTURE_VADDR_END, (void*)SHADOW_ASSETS_INDEX_VADDR_START, (void*)SHADOW_ASSETS_INDEX_VADDR_END, 1, 0);
    initializeShadowSprite(2, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsIndexSegmentRomStart, &_shadowsIndexSegmentRomEnd, (void*)SHADOW_TEXTURE_VADDR_START, (void*)SHADOW_TEXTURE_VADDR_END, (void*)SHADOW_ASSETS_INDEX_VADDR_START, (void*)SHADOW_ASSETS_INDEX_VADDR_END, 2, 0);
 
    initializeEntityAsset(ENTITY_PLAYER, &_playerTextureSegmentRomStart, &_playerTextureSegmentRomEnd, &_playerAssetsIndexSegmentRomStart, &_playerAssetsIndexSegmentRomEnd, &_playerSpritesheetIndexSegmentRomStart, &_playerSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PLAYER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_PLAYER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MARIA, &_mariaTextureSegmentRomStart, &_mariaTextureSegmentRomEnd, &_mariaAssetsIndexSegmentRomStart, &_mariaAssetsIndexSegmentRomEnd, &_mariaSpritesheetIndexSegmentRomStart, &_mariaSpritesheetIndexSegmentRomEnd, 1, 0, &mariaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MARIA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MARIA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_POPURI, &_popuriTextureSegmentRomStart, &_popuriTextureSegmentRomEnd, &_popuriAssetsIndexSegmentRomStart, &_popuriAssetsIndexSegmentRomEnd, &_popuriSpritesheetIndexSegmentRomStart, &_popuriSpritesheetIndexSegmentRomEnd, 1, 0, &popuriAnimationScripts);
    setEntityCollisionBuffers(ENTITY_POPURI, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_POPURI, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ELLI, &_elliTextureSegmentRomStart, &_elliTextureSegmentRomEnd, &_elliAssetsIndexSegmentRomStart, &_elliAssetsIndexSegmentRomEnd, &_elliSpritesheetIndexSegmentRomStart, &_elliSpritesheetIndexSegmentRomEnd, 1, 0, &elliAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ELLI, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ELLI, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ANN, &_annTextureSegmentRomStart, &_annTextureSegmentRomEnd, &_annAssetsIndexSegmentRomStart, &_annAssetsIndexSegmentRomEnd, &_annSpritesheetIndexSegmentRomStart, &_annSpritesheetIndexSegmentRomEnd, 1, 0, &annAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ANN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ANN, 0xC, 0xC); 
    
    initializeEntityAsset(ENTITY_KAREN, &_karenTextureSegmentRomStart, &_karenTextureSegmentRomEnd, &_karenAssetsIndexSegmentRomStart, &_karenAssetsIndexSegmentRomEnd, &_karenSpritesheetIndexSegmentRomStart, &_karenSpritesheetIndexSegmentRomEnd, 1, 0, &karenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KAREN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_KAREN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BABY, &_babyTextureSegmentRomStart, &_babyTextureSegmentRomEnd, &_babyAssetsIndexSegmentRomStart, &_babyAssetsIndexSegmentRomEnd, &_babySpritesheetIndexSegmentRomStart, &_babySpritesheetIndexSegmentRomEnd, 1, 0, &babyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BABY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_BABY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HARRIS, &_harrisTextureSegmentRomStart, &_harrisTextureSegmentRomEnd, &_harrisAssetsIndexSegmentRomStart, &_harrisAssetsIndexSegmentRomEnd, &_harrisSpritesheetIndexSegmentRomStart, &_harrisSpritesheetIndexSegmentRomEnd, 1, 0, &harrisAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HARRIS, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HARRIS, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GRAY, &_grayTextureSegmentRomStart, &_grayTextureSegmentRomEnd, &_grayAssetsIndexSegmentRomStart, &_grayAssetsIndexSegmentRomEnd, &_graySpritesheetIndexSegmentRomStart, &_graySpritesheetIndexSegmentRomEnd, 1, 0, &grayAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GRAY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_GRAY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_JEFF, &_jeffTextureSegmentRomStart, &_jeffTextureSegmentRomEnd, &_jeffAssetsIndexSegmentRomStart, &_jeffAssetsIndexSegmentRomEnd, &_jeffSpritesheetIndexSegmentRomStart, &_jeffSpritesheetIndexSegmentRomEnd, 1, 0, &jeffAnimationScripts);
    setEntityCollisionBuffers(ENTITY_JEFF, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_JEFF, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CLIFF, &_cliffTextureSegmentRomStart, &_cliffTextureSegmentRomEnd, &_cliffAssetsIndexSegmentRomStart, &_cliffAssetsIndexSegmentRomEnd, &_cliffSpritesheetIndexSegmentRomStart, &_cliffSpritesheetIndexSegmentRomEnd, 1, 0, &cliffAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CLIFF, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CLIFF, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAI, &_kaiTextureSegmentRomStart, &_kaiTextureSegmentRomEnd, &_kaiAssetsIndexSegmentRomStart, &_kaiAssetsIndexSegmentRomEnd, &_kaiSpritesheetIndexSegmentRomStart, &_kaiSpritesheetIndexSegmentRomEnd, 1, 0, &kaiAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KAI, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_KAI, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MAYOR, &_mayorTextureSegmentRomStart, &_mayorTextureSegmentRomEnd, &_mayorAssetsIndexSegmentRomStart, &_mayorAssetsIndexSegmentRomEnd, &_mayorSpritesheetIndexSegmentRomStart, &_mayorSpritesheetIndexSegmentRomEnd, 1, 0, &mayorAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MAYOR, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MAYOR, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MAYOR_WIFE, &_mayorWifeTextureSegmentRomStart, &_mayorWifeTextureSegmentRomEnd, &_mayorWifeAssetsIndexSegmentRomStart, &_mayorWifeAssetsIndexSegmentRomEnd, &_mayorWifeSpritesheetIndexSegmentRomStart, &_mayorWifeSpritesheetIndexSegmentRomEnd, 1, 0, &mayorWifeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MAYOR_WIFE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MAYOR_WIFE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_LILIA, &_liliaTextureSegmentRomStart, &_liliaTextureSegmentRomEnd, &_liliaAssetsIndexSegmentRomStart, &_liliaAssetsIndexSegmentRomEnd, &_liliaSpritesheetIndexSegmentRomStart, &_liliaSpritesheetIndexSegmentRomEnd, 1, 0, &liliaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_LILIA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_LILIA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BASIL, &_basilTextureSegmentRomStart, &_basilTextureSegmentRomEnd, &_basilAssetsIndexSegmentRomStart, &_basilAssetsIndexSegmentRomEnd, &_basilSpritesheetIndexSegmentRomStart, &_basilSpritesheetIndexSegmentRomEnd, 1, 0, &basilAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BASIL, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_BASIL, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ELLEN, &_ellenTextureSegmentRomStart, &_ellenTextureSegmentRomEnd, &_ellenAssetsIndexSegmentRomStart, &_ellenAssetsIndexSegmentRomEnd, &_ellenSpritesheetIndexSegmentRomStart, &_ellenSpritesheetIndexSegmentRomEnd, 1, 0, &ellenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ELLEN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ELLEN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOUG, &_dougTextureSegmentRomStart, &_dougTextureSegmentRomEnd, &_dougAssetsIndexSegmentRomStart, &_dougAssetsIndexSegmentRomEnd, &_dougSpritesheetIndexSegmentRomStart, &_dougSpritesheetIndexSegmentRomEnd, 1, 0, &dougAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DOUG, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DOUG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GOTZ, &_gotzTextureSegmentRomStart, &_gotzTextureSegmentRomEnd, &_gotzAssetsIndexSegmentRomStart, &_gotzAssetsIndexSegmentRomEnd, &_gotzSpritesheetIndexSegmentRomStart, &_gotzSpritesheetIndexSegmentRomEnd, 1, 0, &gotzAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GOTZ, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_GOTZ, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SASHA, &_sashaTextureSegmentRomStart, &_sashaTextureSegmentRomEnd, &_sashaAssetsIndexSegmentRomStart, &_sashaAssetsIndexSegmentRomEnd, &_sashaSpritesheetIndexSegmentRomStart, &_sashaSpritesheetIndexSegmentRomEnd, 1, 0, &sashaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SASHA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SASHA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_POTION_SHOP_DEALER, &_potionShopDealerTextureSegmentRomStart, &_potionShopDealerTextureSegmentRomEnd, &_potionShopDealerAssetsIndexSegmentRomStart, &_potionShopDealerAssetsIndexSegmentRomEnd, &_potionShopDealerSpritesheetIndexSegmentRomStart, &_potionShopDealerSpritesheetIndexSegmentRomEnd, 1, 0, &potionShopDealerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_POTION_SHOP_DEALER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_POTION_SHOP_DEALER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KENT, &_kentTextureSegmentRomStart, &_kentTextureSegmentRomEnd, &_kentAssetsIndexSegmentRomStart, &_kentAssetsIndexSegmentRomEnd, &_kentSpritesheetIndexSegmentRomStart, &_kentSpritesheetIndexSegmentRomEnd, 1, 0, &kentAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KENT, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_KENT, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_STU, &_stuTextureSegmentRomStart, &_stuTextureSegmentRomEnd, &_stuAssetsIndexSegmentRomStart, &_stuAssetsIndexSegmentRomEnd, &_stuSpritesheetIndexSegmentRomStart, &_stuSpritesheetIndexSegmentRomEnd, 1, 0, &stuAnimationScripts);
    setEntityCollisionBuffers(ENTITY_STU, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_STU, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MIDWIFE, &_midwifeTextureSegmentRomStart, &_midwifeTextureSegmentRomEnd, &_midwifeAssetsIndexSegmentRomStart, &_midwifeAssetsIndexSegmentRomEnd, &_midwifeSpritesheetIndexSegmentRomStart, &_midwifeSpritesheetIndexSegmentRomEnd, 1, 0, &midwifeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MIDWIFE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MIDWIFE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MAY, &_mayTextureSegmentRomStart, &_mayTextureSegmentRomEnd, &_mayAssetsIndexSegmentRomStart, &_mayAssetsIndexSegmentRomEnd, &_maySpritesheetIndexSegmentRomStart, &_maySpritesheetIndexSegmentRomEnd, 1, 0, &mayAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MAY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MAY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_RICK, &_rickTextureSegmentRomStart, &_rickTextureSegmentRomEnd, &_rickAssetsIndexSegmentRomStart, &_rickAssetsIndexSegmentRomEnd, &_rickSpritesheetIndexSegmentRomStart, &_rickSpritesheetIndexSegmentRomEnd, 1, 0, &rickAnimationScripts);
    setEntityCollisionBuffers(ENTITY_RICK, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_RICK, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PASTOR, &_pastorTextureSegmentRomStart, &_pastorTextureSegmentRomEnd, &_pastorAssetsIndexSegmentRomStart, &_pastorAssetsIndexSegmentRomEnd, &_pastorSpritesheetIndexSegmentRomStart, &_pastorSpritesheetIndexSegmentRomEnd, 1, 0, &pastorAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PASTOR, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_PASTOR, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SHIPPER, &_shipperTextureSegmentRomStart, &_shipperTextureSegmentRomEnd, &_shipperAssetsIndexSegmentRomStart, &_shipperAssetsIndexSegmentRomEnd, &_shipperSpritesheetIndexSegmentRomStart, &_shipperSpritesheetIndexSegmentRomEnd, 1, 0, &shipperAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SHIPPER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SHIPPER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SAIBARA, &_saibaraTextureSegmentRomStart, &_saibaraTextureSegmentRomEnd, &_saibaraAssetsIndexSegmentRomStart, &_saibaraAssetsIndexSegmentRomEnd, &_saibaraSpritesheetIndexSegmentRomStart, &_saibaraSpritesheetIndexSegmentRomEnd, 1, 0, &saibaraAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SAIBARA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SAIBARA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DUKE, &_dukeTextureSegmentRomStart, &_dukeTextureSegmentRomEnd, &_dukeAssetsIndexSegmentRomStart, &_dukeAssetsIndexSegmentRomEnd, &_dukeSpritesheetIndexSegmentRomStart, &_dukeSpritesheetIndexSegmentRomEnd, 1, 0, &dukeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DUKE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DUKE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GREG, &_gregTextureSegmentRomStart, &_gregTextureSegmentRomEnd, &_gregAssetsIndexSegmentRomStart, &_gregAssetsIndexSegmentRomEnd, &_gregSpritesheetIndexSegmentRomStart, &_gregSpritesheetIndexSegmentRomEnd, 1, 0, &gregAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GREG, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_GREG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ASSISTANT_CARPENTER, &_assistantCarpenterTextureSegmentRomStart, &_assistantCarpenterTextureSegmentRomEnd, &_assistantCarpenterAssetsIndexSegmentRomStart, &_assistantCarpenterAssetsIndexSegmentRomEnd, &_assistantCarpenterSpritesheetIndexSegmentRomStart, &_assistantCarpenterSpritesheetIndexSegmentRomEnd, 1, 0, &assistantCarpenterAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSISTANT_CARPENTER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ASSISTANT_CARPENTER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MASTER_CARPENTER, &_masterCarpenterTextureSegmentRomStart, &_masterCarpenterTextureSegmentRomEnd, &_masterCarpenterAssetsIndexSegmentRomStart, &_masterCarpenterAssetsIndexSegmentRomEnd, &_masterCarpenterSpritesheetIndexSegmentRomStart, &_masterCarpenterSpritesheetIndexSegmentRomEnd, 1, 0, &masterCarpenterAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MASTER_CARPENTER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MASTER_CARPENTER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HARVEST_SPRITE, &_harvestSpriteTextureSegmentRomStart, &_harvestSpriteTextureSegmentRomEnd, &_harvestSpriteAssetsIndexSegmentRomStart, &_harvestSpriteAssetsIndexSegmentRomEnd, &_harvestSpriteSpritesheetIndexSegmentRomStart, &_harvestSpriteSpritesheetIndexSegmentRomEnd, 1, 0, &harvestSpriteAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HARVEST_SPRITE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HARVEST_SPRITE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BARLEY, &_barleyTextureSegmentRomStart, &_barleyTextureSegmentRomEnd, &_barleyAssetsIndexSegmentRomStart, &_barleyAssetsIndexSegmentRomEnd, &_barleySpritesheetIndexSegmentRomStart, &_barleySpritesheetIndexSegmentRomEnd, 1, 0, &barleyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BARLEY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_BARLEY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SYDNEY, &_sydneyTextureSegmentRomStart, &_sydneyTextureSegmentRomEnd, &_sydneyAssetsIndexSegmentRomStart, &_sydneyAssetsIndexSegmentRomEnd, &_sydneySpritesheetIndexSegmentRomStart, &_sydneySpritesheetIndexSegmentRomEnd, 1, 0, &sydneyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SYDNEY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SYDNEY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ENTOMOLOGIST, &_entomologistTextureSegmentRomStart, &_entomologistTextureSegmentRomEnd, &_entomologistAssetsIndexSegmentRomStart, &_entomologistAssetsIndexSegmentRomEnd, &_entomologistSpritesheetIndexSegmentRomStart, &_entomologistSpritesheetIndexSegmentRomEnd, 1, 0, &entomologistAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ENTOMOLOGIST, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ENTOMOLOGIST, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GOURMET_JUDGE, &_gourmetJudgeTextureSegmentRomStart, &_gourmetJudgeTextureSegmentRomEnd, &_gourmetJudgeAssetsIndexSegmentRomStart, &_gourmetJudgeAssetsIndexSegmentRomEnd, &_gourmetJudgeSpritesheetIndexSegmentRomStart, &_gourmetJudgeSpritesheetIndexSegmentRomEnd, 1, 0, &gourmetJudgeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GOURMET_JUDGE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_GOURMET_JUDGE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PHOTOGRAPHER, &_photographerTextureSegmentRomStart, &_photographerTextureSegmentRomEnd, &_photographerAssetsIndexSegmentRomStart, &_photographerAssetsIndexSegmentRomEnd, &_photographerSpritesheetIndexSegmentRomStart, &_photographerSpritesheetIndexSegmentRomEnd, 1, 0, &photographerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PHOTOGRAPHER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_PHOTOGRAPHER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SHADY_SALESMAN, &_shadySalesmanTextureSegmentRomStart, &_shadySalesmanTextureSegmentRomEnd, &_shadySalesmanAssetsIndexSegmentRomStart, &_shadySalesmanAssetsIndexSegmentRomEnd, &_shadySalesmanSpritesheetIndexSegmentRomStart, &_shadySalesmanSpritesheetIndexSegmentRomEnd, 1, 0, &shadySalesmanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SHADY_SALESMAN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SHADY_SALESMAN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MRS_MANA_SON, &_mrsManaSonTextureSegmentRomStart, &_mrsManaSonTextureSegmentRomEnd, &_mrsManaSonAssetsIndexSegmentRomStart, &_mrsManaSonAssetsIndexSegmentRomEnd, &_mrsManaSonSpritesheetIndexSegmentRomStart, &_mrsManaSonSpritesheetIndexSegmentRomEnd, 1, 0, &mrsManaSonAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MRS_MANA_SON, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MRS_MANA_SON, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MRS_MANA, &_mrsManaTextureSegmentRomStart, &_mrsManaTextureSegmentRomEnd, &_mrsManaAssetsIndexSegmentRomStart, &_mrsManaAssetsIndexSegmentRomEnd, &_mrsManaSpritesheetIndexSegmentRomStart, &_mrsManaSpritesheetIndexSegmentRomEnd, 1, 0, &mrsManaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MRS_MANA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MRS_MANA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_OLD_MAN, &_oldManTextureSegmentRomStart, &_oldManTextureSegmentRomEnd, &_oldManAssetsIndexSegmentRomStart, &_oldManAssetsIndexSegmentRomEnd, &_oldManSpritesheetIndexSegmentRomStart, &_oldManSpritesheetIndexSegmentRomEnd, 1, 0, &oldManAnimationScripts);
    setEntityCollisionBuffers(ENTITY_OLD_MAN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_OLD_MAN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_OLD_WOMAN, &_oldWomanTextureSegmentRomStart, &_oldWomanTextureSegmentRomEnd, &_oldWomanAssetsIndexSegmentRomStart, &_oldWomanAssetsIndexSegmentRomEnd, &_oldWomanSpritesheetIndexSegmentRomStart, &_oldWomanSpritesheetIndexSegmentRomEnd, 1, 0, &oldWomanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_OLD_WOMAN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_OLD_WOMAN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_TOURIST_COUPLE_MAN, &_touristCoupleManTextureSegmentRomStart, &_touristCoupleManTextureSegmentRomEnd, &_touristCoupleManAssetsIndexSegmentRomStart, &_touristCoupleManAssetsIndexSegmentRomEnd, &_touristCoupleManSpritesheetIndexSegmentRomStart, &_touristCoupleManSpritesheetIndexSegmentRomEnd, 1, 0, &touristCoupleManAnimationScripts);
    setEntityCollisionBuffers(ENTITY_TOURIST_COUPLE_MAN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_TOURIST_COUPLE_MAN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_TOURIST_COUPLE_WOMAN, &_touristCoupleWomanTextureSegmentRomStart, &_touristCoupleWomanTextureSegmentRomEnd, &_touristCoupleWomanAssetsIndexSegmentRomStart, &_touristCoupleWomanAssetsIndexSegmentRomEnd, &_touristCoupleWomanSpritesheetIndexSegmentRomStart, &_touristCoupleWomanSpritesheetIndexSegmentRomEnd, 1, 0, &touristCoupleWomanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_TOURIST_COUPLE_WOMAN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_TOURIST_COUPLE_WOMAN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FESTIVAL_GIRL1, &_festivalGirl1TextureSegmentRomStart, &_festivalGirl1TextureSegmentRomEnd, &_festivalGirl1AssetsIndexSegmentRomStart, &_festivalGirl1AssetsIndexSegmentRomEnd, &_festivalGirl1SpritesheetIndexSegmentRomStart, &_festivalGirl1SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl1AnimationScripts);
    setEntityCollisionBuffers(ENTITY_FESTIVAL_GIRL1, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_FESTIVAL_GIRL1, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FESTIVAL_GIRL2, &_festivalGirl2TextureSegmentRomStart, &_festivalGirl2TextureSegmentRomEnd, &_festivalGirl2AssetsIndexSegmentRomStart, &_festivalGirl2AssetsIndexSegmentRomEnd, &_festivalGirl2SpritesheetIndexSegmentRomStart, &_festivalGirl2SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl2AnimationScripts);
    setEntityCollisionBuffers(ENTITY_FESTIVAL_GIRL2, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_FESTIVAL_GIRL2, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FESTIVAL_GIRL3, &_festivalGirl3TextureSegmentRomStart, &_festivalGirl3TextureSegmentRomEnd, &_festivalGirl3AssetsIndexSegmentRomStart, &_festivalGirl3AssetsIndexSegmentRomEnd, &_festivalGirl3SpritesheetIndexSegmentRomStart, &_festivalGirl3SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl3AnimationScripts);
    setEntityCollisionBuffers(ENTITY_FESTIVAL_GIRL3, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_FESTIVAL_GIRL3, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HARVEST_GODDESS, &_harvestGoddessTextureSegmentRomStart, &_harvestGoddessTextureSegmentRomEnd, &_harvestGoddessAssetsIndexSegmentRomStart, &_harvestGoddessAssetsIndexSegmentRomEnd, &_harvestGoddessSpritesheetIndexSegmentRomStart, &_harvestGoddessSpritesheetIndexSegmentRomEnd, 1, 0, &harvestGoddessAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HARVEST_GODDESS, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HARVEST_GODDESS, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SPIRIT_FESTIVAL_OCARINA_PERFORMER, &_spiritFestivalOcarinaPerformerTextureSegmentRomStart, &_spiritFestivalOcarinaPerformerTextureSegmentRomEnd, &_spiritFestivalOcarinaPerformerAssetsIndexSegmentRomStart, &_spiritFestivalOcarinaPerformerAssetsIndexSegmentRomEnd, &_spiritFestivalOcarinaPerformerSpritesheetIndexSegmentRomStart, &_spiritFestivalOcarinaPerformerSpritesheetIndexSegmentRomEnd, 1, 0, &spiritFestivalOcarinaPerformerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SPIRIT_FESTIVAL_OCARINA_PERFORMER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SPIRIT_FESTIVAL_OCARINA_PERFORMER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAPPA, &_kappaTextureSegmentRomStart, &_kappaTextureSegmentRomEnd, &_kappaAssetsIndexSegmentRomStart, &_kappaAssetsIndexSegmentRomEnd, &_kappaSpritesheetIndexSegmentRomStart, &_kappaSpritesheetIndexSegmentRomEnd, 1, 0, &kappaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KAPPA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_KAPPA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PRINCESS, &_princessTextureSegmentRomStart, &_princessTextureSegmentRomEnd, &_princessAssetsIndexSegmentRomStart, &_princessAssetsIndexSegmentRomEnd, &_princessSpritesheetIndexSegmentRomStart, &_princessSpritesheetIndexSegmentRomEnd, 1, 0, &princessAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PRINCESS, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_PRINCESS, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_EMPTY_NPC_SPRITE, &_emptyNPCTextureSegmentRomStart, &_emptyNPCTextureSegmentRomEnd, &_emptyNPCAssetsIndexSegmentRomStart, &_emptyNPCAssetsIndexSegmentRomEnd, &_emptyNPCSpritesheetIndexSegmentRomStart, &_emptyNPCSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_EMPTY_NPC_SPRITE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_EMPTY_NPC_SPRITE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BUNNY, &_bunnyTextureSegmentRomStart, &_bunnyTextureSegmentRomEnd, &_bunnyAssetsIndexSegmentRomStart, &_bunnyAssetsIndexSegmentRomEnd, &_bunnySpritesheetIndexSegmentRomStart, &_bunnySpritesheetIndexSegmentRomEnd, 1, 0, &bunnyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BUNNY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_BUNNY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MOUSE, &_mouseTextureSegmentRomStart, &_mouseTextureSegmentRomEnd, &_mouseAssetsIndexSegmentRomStart, &_mouseAssetsIndexSegmentRomEnd, &_mouseSpritesheetIndexSegmentRomStart, &_mouseSpritesheetIndexSegmentRomEnd, 1, 0, &mouseAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MOUSE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MOUSE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FOX, &_foxTextureSegmentRomStart, &_foxTextureSegmentRomEnd, &_foxAssetsIndexSegmentRomStart, &_foxAssetsIndexSegmentRomEnd, &_foxSpritesheetIndexSegmentRomStart, &_foxSpritesheetIndexSegmentRomEnd, 1, 0, &foxAnimationScripts);
    setEntityCollisionBuffers(ENTITY_FOX, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_FOX, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MONKEY, &_monkeyTextureSegmentRomStart, &_monkeyTextureSegmentRomEnd, &_monkeyAssetsIndexSegmentRomStart, &_monkeyAssetsIndexSegmentRomEnd, &_monkeySpritesheetIndexSegmentRomStart, &_monkeySpritesheetIndexSegmentRomEnd, 1, 0, &monkeyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MONKEY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MONKEY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HAWK, &_hawkTextureSegmentRomStart, &_hawkTextureSegmentRomEnd, &_hawkAssetsIndexSegmentRomStart, &_hawkAssetsIndexSegmentRomEnd, &_hawkSpritesheetIndexSegmentRomStart, &_hawkSpritesheetIndexSegmentRomEnd, 1, 0, &hawkAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HAWK, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HAWK, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MOLE, &_moleTextureSegmentRomStart, &_moleTextureSegmentRomEnd, &_moleAssetsIndexSegmentRomStart, &_moleAssetsIndexSegmentRomEnd, &_moleSpritesheetIndexSegmentRomStart, &_moleSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MOLE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MOLE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SQUIRREL, &_squirrelTextureSegmentRomStart, &_squirrelTextureSegmentRomEnd, &_squirrelAssetsIndexSegmentRomStart, &_squirrelAssetsIndexSegmentRomEnd, &_squirrelSpritesheetIndexSegmentRomStart, &_squirrelSpritesheetIndexSegmentRomEnd, 1, 0, &squirrelAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SQUIRREL, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SQUIRREL, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOG_RACE, &_dogRaceTextureSegmentRomStart, &_dogRaceTextureSegmentRomEnd, &_dogRaceAssetsIndexSegmentRomStart, &_dogRaceAssetsIndexSegmentRomEnd, &_dogRaceSpritesheetIndexSegmentRomStart, &_dogRaceSpritesheetIndexSegmentRomEnd, 1, 0, &dogRaceAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DOG_RACE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DOG_RACE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CAT, &_catTextureSegmentRomStart, &_catTextureSegmentRomEnd, &_catAssetsIndexSegmentRomStart, &_catAssetsIndexSegmentRomEnd, &_catSpritesheetIndexSegmentRomStart, &_catSpritesheetIndexSegmentRomEnd, 1, 0, &catAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CAT, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CAT, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOG, &_dogTextureSegmentRomStart, &_dogTextureSegmentRomEnd, &_dogAssetsIndexSegmentRomStart, &_dogAssetsIndexSegmentRomEnd, &_dogSpritesheetIndexSegmentRomStart, &_dogSpritesheetIndexSegmentRomEnd, 1, 2, &dogAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DOG, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DOG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOG_TITLE, &_dogTitleTextureSegmentRomStart, &_dogTitleTextureSegmentRomEnd, &_dogTitleAssetsIndexSegmentRomStart, &_dogTitleAssetsIndexSegmentRomEnd, &_dogTitleSpritesheetIndexSegmentRomStart, &_dogTitleSpritesheetIndexSegmentRomEnd, 1, 0, &dogTitleAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DOG_TITLE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DOG_TITLE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_PONY, &_horsePonyTextureSegmentRomStart, &_horsePonyTextureSegmentRomEnd, &_horsePonyAssetsIndexSegmentRomStart, &_horsePonyAssetsIndexSegmentRomEnd, &_horsePonySpritesheetIndexSegmentRomStart, &_horsePonySpritesheetIndexSegmentRomEnd, 1, 0, &horsePonyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HORSE_PONY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HORSE_PONY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_GROWN, &_horseGrownTextureSegmentRomStart, &_horseGrownTextureSegmentRomEnd, &_horseGrownAssetsIndexSegmentRomStart, &_horseGrownAssetsIndexSegmentRomEnd, &_horseGrownSpritesheetIndexSegmentRomStart, &_horseGrownSpritesheetIndexSegmentRomEnd, 1, 0, &horseGrownAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HORSE_GROWN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HORSE_GROWN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CHICK, &_chickTextureSegmentRomStart, &_chickTextureSegmentRomEnd, &_chickAssetsIndexSegmentRomStart, &_chickAssetsIndexSegmentRomEnd, &_chickSpritesheetIndexSegmentRomStart, &_chickSpritesheetIndexSegmentRomEnd, 1, 2, &chickAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CHICK, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CHICK, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CHICKEN, &_chickenTextureSegmentRomStart, &_chickenTextureSegmentRomEnd, &_chickenAssetsIndexSegmentRomStart, &_chickenAssetsIndexSegmentRomEnd, &_chickenSpritesheetIndexSegmentRomStart, &_chickenSpritesheetIndexSegmentRomEnd, 1, 0, &chickenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CHICKEN, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CHICKEN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CALF, &_calfTextureSegmentRomStart, &_calfTextureSegmentRomEnd, &_calfAssetsIndexSegmentRomStart, &_calfAssetsIndexSegmentRomEnd, &_calfSpritesheetIndexSegmentRomStart, &_calfSpritesheetIndexSegmentRomEnd, 1, 0, &calfAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CALF, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CALF, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_YOUNG_COW, &_youngCowTextureSegmentRomStart, &_youngCowTextureSegmentRomEnd, &_youngCowAssetsIndexSegmentRomStart, &_youngCowAssetsIndexSegmentRomEnd, &_youngCowSpritesheetIndexSegmentRomStart, &_youngCowSpritesheetIndexSegmentRomEnd, 1, 1, &youngCowAnimationScripts);
    setEntityCollisionBuffers(ENTITY_YOUNG_COW, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_YOUNG_COW, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_COW, &_cowTextureSegmentRomStart, &_cowTextureSegmentRomEnd, &_cowAssetsIndexSegmentRomStart, &_cowAssetsIndexSegmentRomEnd, &_cowSpritesheetIndexSegmentRomStart, &_cowSpritesheetIndexSegmentRomEnd, 1, 1, &cowAnimationScripts);
    setEntityCollisionBuffers(ENTITY_COW, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_COW, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_LAMB, &_lambTextureSegmentRomStart, &_lambTextureSegmentRomEnd, &_lambAssetsIndexSegmentRomStart, &_lambAssetsIndexSegmentRomEnd, &_lambSpritesheetIndexSegmentRomStart, &_lambSpritesheetIndexSegmentRomEnd, 1, 0, &lambAnimationScripts);
    setEntityCollisionBuffers(ENTITY_LAMB, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_LAMB, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SHEEP, &_sheepTextureSegmentRomStart, &_sheepTextureSegmentRomEnd, &_sheepAssetsIndexSegmentRomStart, &_sheepAssetsIndexSegmentRomEnd, &_sheepSpritesheetIndexSegmentRomStart, &_sheepSpritesheetIndexSegmentRomEnd, 1, 0, &sheepAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SHEEP, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SHEEP, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FROG, &_frogTextureSegmentRomStart, &_frogTextureSegmentRomEnd, &_frogAssetsIndexSegmentRomStart, &_frogAssetsIndexSegmentRomEnd, &_frogSpritesheetIndexSegmentRomStart, &_frogSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_FROG, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_FROG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CRAB, &_crabTextureSegmentRomStart, &_crabTextureSegmentRomEnd, &_crabAssetsIndexSegmentRomStart, &_crabAssetsIndexSegmentRomEnd, &_crabSpritesheetIndexSegmentRomStart, &_crabSpritesheetIndexSegmentRomEnd, 1, 0, &crabAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CRAB, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CRAB, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORNED_BEETLE, &_hornedBeetleTextureSegmentRomStart, &_hornedBeetleTextureSegmentRomEnd, &_hornedBeetleAssetsIndexSegmentRomStart, &_hornedBeetleAssetsIndexSegmentRomEnd, &_hornedBeetleSpritesheetIndexSegmentRomStart, &_hornedBeetleSpritesheetIndexSegmentRomEnd, 1, 0xFF, &hornedBeetleAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HORNED_BEETLE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HORNED_BEETLE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_STAG_BEETLE, &_stagBeetleTextureSegmentRomStart, &_stagBeetleTextureSegmentRomEnd, &_stagBeetleAssetsIndexSegmentRomStart, &_stagBeetleAssetsIndexSegmentRomEnd, &_stagBeetleSpritesheetIndexSegmentRomStart, &_stagBeetleSpritesheetIndexSegmentRomEnd, 1, 0xFF, &stagBeetleAnimationScripts);
    setEntityCollisionBuffers(ENTITY_STAG_BEETLE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_STAG_BEETLE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BELL_RING_CRICKET, &_bellRingCricketTextureSegmentRomStart, &_bellRingCricketTextureSegmentRomEnd, &_bellRingCricketAssetsIndexSegmentRomStart, &_bellRingCricketAssetsIndexSegmentRomEnd, &_bellRingCricketSpritesheetIndexSegmentRomStart, &_bellRingCricketSpritesheetIndexSegmentRomEnd, 1, 0xFF, &bellRingCricketAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BELL_RING_CRICKET, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_BELL_RING_CRICKET, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CICADA, &_cicadaTextureSegmentRomStart, &_cicadaTextureSegmentRomEnd, &_cicadaAssetsIndexSegmentRomStart, &_cicadaAssetsIndexSegmentRomEnd, &_cicadaSpritesheetIndexSegmentRomStart, &_cicadaSpritesheetIndexSegmentRomEnd, 1, 0xFF, &cicadaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CICADA, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_CICADA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BIRD, &_birdTextureSegmentRomStart, &_birdTextureSegmentRomEnd, &_birdAssetsIndexSegmentRomStart, &_birdAssetsIndexSegmentRomEnd, &_birdSpritesheetIndexSegmentRomStart, &_birdSpritesheetIndexSegmentRomEnd, 1, 2, &birdAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BIRD, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_BIRD, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_LADYBUG, &_ladybugTextureSegmentRomStart, &_ladybugTextureSegmentRomEnd, &_ladybugAssetsIndexSegmentRomStart, &_ladybugAssetsIndexSegmentRomEnd, &_ladybugSpritesheetIndexSegmentRomStart, &_ladybugSpritesheetIndexSegmentRomEnd, 1, 0xFF, &ladybugAnimationScripts);
    setEntityCollisionBuffers(ENTITY_LADYBUG, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_LADYBUG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DRAGONFLY, &_dragonflyTextureSegmentRomStart, &_dragonflyTextureSegmentRomEnd, &_dragonflyAssetsIndexSegmentRomStart, &_dragonflyAssetsIndexSegmentRomEnd, &_dragonflySpritesheetIndexSegmentRomStart, &_dragonflySpritesheetIndexSegmentRomEnd, 1, 2, &dragonflyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DRAGONFLY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DRAGONFLY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GRASSHOPPER, &_grasshopperTextureSegmentRomStart, &_grasshopperTextureSegmentRomEnd, &_grasshopperAssetsIndexSegmentRomStart, &_grasshopperAssetsIndexSegmentRomEnd, &_grasshopperSpritesheetIndexSegmentRomStart, &_grasshopperSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GRASSHOPPER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_GRASSHOPPER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SNAKE, &_snakeTextureSegmentRomStart, &_snakeTextureSegmentRomEnd, &_snakeAssetsIndexSegmentRomStart, &_snakeAssetsIndexSegmentRomEnd, &_snakeSpritesheetIndexSegmentRomStart, &_snakeSpritesheetIndexSegmentRomEnd, 1, 2, &snakeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SNAKE, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_SNAKE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PURPLE_BUTTERFLY, &_purpleButterflyTextureSegmentRomStart, &_purpleButterflyTextureSegmentRomEnd, &_purpleButterflyAssetsIndexSegmentRomStart, &_purpleButterflyAssetsIndexSegmentRomEnd, &_purpleButterflySpritesheetIndexSegmentRomStart, &_purpleButterflySpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PURPLE_BUTTERFLY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_PURPLE_BUTTERFLY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DAD, &_dadTextureSegmentRomStart, &_dadTextureSegmentRomEnd, &_dadAssetsIndexSegmentRomStart, &_dadAssetsIndexSegmentRomEnd, &_dadSpritesheetIndexSegmentRomStart, &_dadSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DAD, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_DAD, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_COW_INDOORS, &_cowIndoorsTextureSegmentRomStart, &_cowIndoorsTextureSegmentRomEnd, &_cowIndoorsAssetsIndexSegmentRomStart, &_cowIndoorsAssetsIndexSegmentRomEnd, &_cowIndoorsSpritesheetIndexSegmentRomStart, &_cowIndoorsSpritesheetIndexSegmentRomEnd, 1, 1, &cowIndoorsAnimationScripts);
    setEntityCollisionBuffers(ENTITY_COW_INDOORS, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_COW_INDOORS, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_UNBRIDLED, &_horseUnbridledTextureSegmentRomStart, &_horseUnbridledTextureSegmentRomEnd, &_horseUnbridledAssetsIndexSegmentRomStart, &_horseUnbridledAssetsIndexSegmentRomEnd, &_horseUnbridledSpritesheetIndexSegmentRomStart, &_horseUnbridledSpritesheetIndexSegmentRomEnd, 1, 0, &horseUnbridledAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HORSE_UNBRIDLED, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HORSE_UNBRIDLED, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_RACER, &_horseRacerTextureSegmentRomStart, &_horseRacerTextureSegmentRomEnd, &_horseRacerAssetsIndexSegmentRomStart, &_horseRacerAssetsIndexSegmentRomEnd, &_horseRacerSpritesheetIndexSegmentRomStart, &_horseRacerSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HORSE_RACER, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_HORSE_RACER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_EMPTY_ENTITY, &_emptyEntityTextureSegmentRomStart, &_emptyEntityTextureSegmentRomEnd, &_emptyEntityAssetsIndexSegmentRomStart, &_emptyEntityAssetsIndexSegmentRomEnd, &_emptyEntitySpritesheetIndexSegmentRomStart, &_emptyEntitySpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_EMPTY_ENTITY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_EMPTY_ENTITY, 0xC, 0xC);
    
    initializeEntityAsset(0x5B, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItemsAnimationScripts1);
    setEntityCollisionBuffers(0x5B, 0xF, 0xF);
    setEntitySpriteDimensions(0x5B, 0xC, 0xC);
    
    initializeEntityAsset(0x5C, &_sowingFestivalTextureSegmentRomStart, &_sowingFestivalTextureSegmentRomEnd, &_sowingFestivalAssetsIndexSegmentRomStart, &_sowingFestivalAssetsIndexSegmentRomEnd, &_sowingFestivalSpritesheetIndexSegmentRomStart, &_sowingFestivalSpritesheetIndexSegmentRomEnd, 1, 0, &homeItemsAnimationScripts);
    setEntityCollisionBuffers(0x5C, 0xF, 0xF);
    setEntitySpriteDimensions(0x5C, 0xC, 0xC);
    
    initializeEntityAsset(0x5D, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 2, &shopItemsAnimationScripts);
    setEntityCollisionBuffers(0x5D, 0xF, 0xF);
    setEntitySpriteDimensions(0x5D, 0xC, 0xC);
    
    initializeEntityAsset(0x5E, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItemsAnimationScripts2);
    setEntityCollisionBuffers(0x5E, 0xF, 0xF);
    setEntitySpriteDimensions(0x5E, 0xC, 0xC);
    
    initializeEntityAsset(0x5F, &_toolsTextureSegmentRomStart, &_toolsTextureSegmentRomEnd, &_toolsAssetsIndexSegmentRomStart, &_toolsAssetsIndexSegmentRomEnd, &_toolsSpritesheetIndexSegmentRomStart, &_toolsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &toolsAnimationScripts);
    setEntityCollisionBuffers(0x5F, 0xF, 0xF);
    setEntitySpriteDimensions(0x5F, 0xC, 0xC);
    
    initializeEntityAsset(0x60, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItemsAnimationScripts3);
    setEntityCollisionBuffers(0x60, 0xF, 0xF);
    setEntitySpriteDimensions(0x60, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MARIA_HARRIS_BABY, &_mariaHarrisBabyTextureSegmentRomStart, &_mariaHarrisBabyTextureSegmentRomEnd, &_mariaHarrisBabyAssetsIndexSegmentRomStart, &_mariaHarrisBabyAssetsIndexSegmentRomEnd, &_mariaHarrisBabySpritesheetIndexSegmentRomStart, &_mariaHarrisBabySpritesheetIndexSegmentRomEnd, 1, 0, &mariaHarrisBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MARIA_HARRIS_BABY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_MARIA_HARRIS_BABY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_POPURI_GRAY_BABY, &_popuriGrayBabyTextureSegmentRomStart, &_popuriGrayBabyTextureSegmentRomEnd, &_popuriGrayBabyAssetsIndexSegmentRomStart, &_popuriGrayBabyAssetsIndexSegmentRomEnd, &_popuriGrayBabySpritesheetIndexSegmentRomStart, &_popuriGrayBabySpritesheetIndexSegmentRomEnd, 1, 0, &popuriGrayBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_POPURI_GRAY_BABY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_POPURI_GRAY_BABY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ELLI_JEFF_BABY, &_elliJeffBabyTextureSegmentRomStart, &_elliJeffBabyTextureSegmentRomEnd, &_elliJeffBabyAssetsIndexSegmentRomStart, &_elliJeffBabyAssetsIndexSegmentRomEnd, &_elliJeffBabySpritesheetIndexSegmentRomStart, &_elliJeffBabySpritesheetIndexSegmentRomEnd, 1, 0, &elliJeffBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ELLI_JEFF_BABY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ELLI_JEFF_BABY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ANN_CLIFF_BABY, &_annCliffBabyTextureSegmentRomStart, &_annCliffBabyTextureSegmentRomEnd, &_annCliffBabyAssetsIndexSegmentRomStart, &_annCliffBabyAssetsIndexSegmentRomEnd, &_annCliffBabySpritesheetIndexSegmentRomStart, &_annCliffBabySpritesheetIndexSegmentRomEnd, 1, 0, &annCliffBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ANN_CLIFF_BABY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_ANN_CLIFF_BABY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAREN_KAI_BABY, &_karenKaiBabyTextureSegmentRomStart, &_karenKaiBabyTextureSegmentRomEnd, &_karenKaiBabyAssetsIndexSegmentRomStart, &_karenKaiBabyAssetsIndexSegmentRomEnd, &_karenKaiBabySpritesheetIndexSegmentRomStart, &_karenKaiBabySpritesheetIndexSegmentRomEnd, 1, 0, &karenKaiBabyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KAREN_KAI_BABY, 0xF, 0xF);
    setEntitySpriteDimensions(ENTITY_KAREN_KAI_BABY, 0xC, 0xC);
    
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

    setMapDataAddresses(POND_SPRING, &_springSpringMapSegmentRomStart, &_springSpringMapSegmentRomEnd);
    setMapDataAddresses(POND_SUMMER, &_springSummerMapSegmentRomStart, &_springSummerMapSegmentRomEnd);
    setMapDataAddresses(POND_AUTUMN, &_springFallMapSegmentRomStart, &_springFallMapSegmentRomEnd);
    setMapDataAddresses(POND_WINTER, &_springWinterMapSegmentRomStart, &_springWinterMapSegmentRomEnd);

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

    // empty, referenced in func_80074C50 
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
    
    initializeMapController(MAIN_MAP_INDEX, 0, MAP_DATA_VADDR);

}

// INCLUDE_ASM("asm/nonmatchings/game/initialize", func_80053088);

void func_80053088(void) {

    u32 ptr = &_fontTextureSegmentRomStart;
    u32 ptr2 = &_fontTextureSegmentRomEnd;
    u32 ptr3 = &_fontPalette1SegmentRomStart;
    u32 ptr4 = &_fontPalette1SegmentRomEnd;
    u32 ptr5 = &_fontPalette2SegmentRomStart;
    u32 ptr6 = &_fontPalette2SegmentRomEnd;

    func_800535DC();
    initializeGameVariableStrings();
    func_8003FAE8((u8*)&characterAvatarAnimationScripts);

    nuPiReadRom(ptr, (void*)COMPRESSED_FONT_VADDR, ptr2 - ptr);
    nuPiReadRom(ptr3, (u16*)FONT_PALETTE_1_VADDR, ptr4 - ptr3);
    nuPiReadRom(ptr5, (u16*)FONT_PALETTE_2_VADDR, ptr6 - ptr5);

    func_8003F80C(0, 0x76, &_dialogueWindowTextureSegmentRomStart, &_dialogueWindowTextureSegmentRomEnd, &_dialogueWindowAssetsIndexSegmentRomStart, &_dialogueWindowAssetsIndexSegmentRomEnd, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 0, 1, -24.0f, 0, 0);
    func_8003F80C(1, 0x76, &_dialogueWindowTextureSegmentRomStart, &_dialogueWindowTextureSegmentRomEnd, &_dialogueWindowAssetsIndexSegmentRomStart, &_dialogueWindowAssetsIndexSegmentRomEnd, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 0, 0, 0, 0, 0);
    func_8003F80C(2, 0x77, &_dialogueWindowTextureSegmentRomStart, &_dialogueWindowTextureSegmentRomEnd, &_dialogueWindowAssetsIndexSegmentRomStart, &_dialogueWindowAssetsIndexSegmentRomEnd, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 1, 0, 0, 0, 0);
  
    func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_START, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_END, (void* )DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, 4, 0xFE, 106.0f, -15.0f, 0.0f);
    func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_START, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_END, (void* )DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, 0xD, 0xFE, 106.0f, -15.0f, 0.0f);
    func_8003FA1C(0, 0x75, &_characterDialogueIconsTextureSegmentRomStart, &_characterDialogueIconsTextureSegmentRomEnd, &_characterDialogueIconsAssetsIndexSegmentRomStart, &_characterDialogueIconsAssetsIndexSegmentRomEnd, &_characterDialogueIconsSpritesheetIndexSegmentRomStart, &_characterDialogueIconsSpritesheetIndexSegmentRomEnd, (void*)0x8023D300, (void*)0x8023DB00, (void*)0x8023E300, (void*)0x8023EF00, (void*)0x8023FF00, (void*)0x80240000, -139.0f, 1.0f, 0);
 
    initializeEmptyMessageBox(0, 0x8030B000);
    setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
    func_8003F5D0(0, 0x10, 3);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_1_VADDR);

    func_8003F360(0, -4, 0);
    setMessageBoxSpriteIndices(0, 0, 0, 0);
    setMessageBoxSfx(0, 0x57, 8, 1);
    setMessageBoxButtonMask(0, (BUTTON_B | BUTTON_A));
    func_8003FB4C(0, 1);
    initializeEmptyMessageBox(1, 0x8030B400);

    setMessageBoxViewSpacePosition(1, 64.0f, 32.0f, 352.0f);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, (u8*)COMPRESSED_FONT_VADDR, (u16*)FONT_PALETTE_2_VADDR);

    func_8003F5D0(1, 0xB, 4);
    func_8003F360(1, -4, 3);
    setMessageBoxButtonMask(1, (BUTTON_B | BUTTON_A));
    func_8003FB4C(1, 2);
    setMessageBoxSpriteIndices(1, 2, 0xFF, 0xFF);
    setMessageBoxSfx(1, 0xFF, 0xFF, 0xFF);
    
}
    
//INCLUDE_ASM("asm/nonmatchings/game/initialize", func_800535DC);

void func_800535DC(void) {

    setTextAddresses(DIALOGUE_1_TEXT_INDEX, &_dialogue1TextIndexSegmentRomStart, &_dialogue1TextIndexSegmentRomEnd, &_dialogue1TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(LIBRARY_TEXT_INDEX, &_libraryTextIndexSegmentRomStart, &_libraryTextIndexSegmentRomEnd, &_libraryTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIARY_TEXT_INDEX, &_diaryTextIndexSegmentRomStart, &_diaryTextIndexSegmentRomEnd, &_diaryTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    
    // Japanese text
    setTextAddresses(DIALOGUE_4_TEXT_INDEX, &_dialogue4TextIndexSegmentRomStart, &_dialogue4TextIndexSegmentRomEnd, &_dialogue4TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);

    setTextAddresses(FESTIVAL_OVERLAY_SELECTIONS_TEXT_INDEX, &_festivalOverlaySelectionsTextIndexSegmentRomStart, &_festivalOverlaySelectionsTextIndexSegmentRomEnd, &_festivalOverlaySelectionsTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(LETTERS_TEXT_INDEX, &_lettersTextIndexSegmentRomStart, &_lettersTextIndexSegmentRomEnd, &_lettersTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(SHOP_TEXT_INDEX, &_shopTextIndexSegmentRomStart, &_shopTextIndexSegmentRomEnd, &_shopTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ANIMAL_INTERACTIONS_TEXT_INDEX, &_animalInteractionsTextIndexSegmentRomStart, &_animalInteractionsTextIndexSegmentRomEnd, &_animalInteractionsTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(TV_TEXT_INDEX, &_tvTextIndexSegmentRomStart, &_tvTextIndexSegmentRomEnd, &_tvTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_10_TEXT_INDEX, &_dialogue10TextIndexSegmentRomStart, &_dialogue10TextIndexSegmentRomEnd, &_dialogue10TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(NAMING_SCREEN_TEXT_INDEX, &_namingScreenTextIndexSegmentRomStart, &_namingScreenTextIndexSegmentRomEnd, &_namingScreenTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);

    setTextAddresses(MARIA_TEXT_INDEX, &_mariaTextIndexSegmentRomStart, &_mariaTextIndexSegmentRomEnd, &_mariaTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(POPURI_TEXT_INDEX, &_popuriTextIndexSegmentRomStart, &_popuriTextIndexSegmentRomEnd, &_popuriTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ELLI_TEXT_INDEX, &_elliTextIndexSegmentRomStart, &_elliTextIndexSegmentRomEnd, &_elliTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ANN_TEXT_INDEX, &_annTextIndexSegmentRomStart, &_annTextIndexSegmentRomEnd, &_annTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(KAREN_TEXT_INDEX, &_karenTextIndexSegmentRomStart, &_karenTextIndexSegmentRomEnd, &_karenTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(HARRIS_TEXT_INDEX, &_harrisTextIndexSegmentRomStart, &_harrisTextIndexSegmentRomEnd, &_harrisTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(GRAY_TEXT_INDEX, &_grayTextIndexSegmentRomStart, &_grayTextIndexSegmentRomEnd, &_grayTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(JEFF_TEXT_INDEX, &_jeffTextIndexSegmentRomStart, &_jeffTextIndexSegmentRomEnd, &_jeffTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(CLIFF_TEXT_INDEX, &_cliffTextIndexSegmentRomStart, &_cliffTextIndexSegmentRomEnd, &_cliffTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(KAI_TEXT_INDEX, &_kaiTextIndexSegmentRomStart, &_kaiTextIndexSegmentRomEnd, &_kaiTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MAYOR_TEXT_INDEX, &_mayorTextIndexSegmentRomStart, &_mayorTextIndexSegmentRomEnd, &_mayorTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MAYOR_WIFE_TEXT_INDEX, &_mayorWifeTextIndexSegmentRomStart, &_mayorWifeTextIndexSegmentRomEnd, &_mayorWifeTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(LILIA_TEXT_INDEX, &_liliaTextIndexSegmentRomStart, &_liliaTextIndexSegmentRomEnd, &_liliaTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(BASIL_TEXT_INDEX, &_basilTextIndexSegmentRomStart, &_basilTextIndexSegmentRomEnd, &_basilTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ELLEN_TEXT_INDEX, &_ellenTextIndexSegmentRomStart, &_ellenTextIndexSegmentRomEnd, &_ellenTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DOUG_TEXT_INDEX, &_dougTextIndexSegmentRomStart, &_dougTextIndexSegmentRomEnd, &_dougTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(GOTZ_TEXT_INDEX, &_gotzTextIndexSegmentRomStart, &_gotzTextIndexSegmentRomEnd, &_gotzTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(SASHA_TEXT_INDEX, &_sashaTextIndexSegmentRomStart, &_sashaTextIndexSegmentRomEnd, &_sashaTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(POTION_SHOP_DEALER_TEXT_INDEX, &_potionShopDealerTextIndexSegmentRomStart, &_potionShopDealerTextIndexSegmentRomEnd, &_potionShopDealerTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(KENT_TEXT_INDEX, &_kentTextIndexSegmentRomStart, &_kentTextIndexSegmentRomEnd, &_kentTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(STU_TEXT_INDEX, &_stuTextIndexSegmentRomStart, &_stuTextIndexSegmentRomEnd, &_stuTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MIDWIFE_TEXT_INDEX, &_midwifeTextIndexSegmentRomStart, &_midwifeTextIndexSegmentRomEnd, &_midwifeTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MAY_TEXT_INDEX, &_mayTextIndexSegmentRomStart, &_mayTextIndexSegmentRomEnd, &_mayTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(RICK_TEXT_INDEX, &_rickTextIndexSegmentRomStart, &_rickTextIndexSegmentRomEnd, &_rickTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(PASTOR_TEXT_INDEX, &_pastorTextIndexSegmentRomStart, &_pastorTextIndexSegmentRomEnd, &_pastorTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(SHIPPER_TEXT_INDEX, &_shipperTextIndexSegmentRomStart, &_shipperTextIndexSegmentRomEnd, &_shipperTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(SAIBARA_TEXT_INDEX, &_saibaraTextIndexSegmentRomStart, &_saibaraTextIndexSegmentRomEnd, &_saibaraTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DUKE_TEXT_INDEX, &_dukeTextIndexSegmentRomStart, &_dukeTextIndexSegmentRomEnd, &_dukeTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(GREG_TEXT_INDEX, &_gregTextIndexSegmentRomStart, &_gregTextIndexSegmentRomEnd, &_gregTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ASSISTANT_CARPENTERS_TEXT_INDEX, &_assistantCarpentersTextIndexSegmentRomStart, &_assistantCarpentersTextIndexSegmentRomEnd, &_assistantCarpentersTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ASSISTANT_CARPENTERS_TEXT_INDEX, &_assistantCarpentersTextIndexSegmentRomStart, &_assistantCarpentersTextIndexSegmentRomEnd, &_assistantCarpentersTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MASTER_CARPENTER_TEXT_INDEX, &_masterCarpenterTextIndexSegmentRomStart, &_masterCarpenterTextIndexSegmentRomEnd, &_masterCarpenterTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(HARVEST_SPRITES_TEXT_INDEX, &_harvestSpritesTextIndexSegmentRomStart, &_harvestSpritesTextIndexSegmentRomEnd, &_harvestSpritesTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(HARVEST_SPRITES_TEXT_INDEX, &_harvestSpritesTextIndexSegmentRomStart, &_harvestSpritesTextIndexSegmentRomEnd, &_harvestSpritesTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(HARVEST_SPRITES_TEXT_INDEX, &_harvestSpritesTextIndexSegmentRomStart, &_harvestSpritesTextIndexSegmentRomEnd, &_harvestSpritesTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(BARLEY_TEXT_INDEX, &_barleyTextIndexSegmentRomStart, &_barleyTextIndexSegmentRomEnd, &_barleyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(SYDNEY_TEXT_INDEX, &_sydneyTextIndexSegmentRomStart, &_sydneyTextIndexSegmentRomEnd, &_sydneyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MRS_MANA_AND_JOHN_TEXT_INDEX, &_mrsManaAndJohnTextIndexSegmentRomStart, &_mrsManaAndJohnTextIndexSegmentRomEnd, &_mrsManaAndJohnTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MRS_MANA_AND_JOHN_TEXT_INDEX, &_mrsManaAndJohnTextIndexSegmentRomStart, &_mrsManaAndJohnTextIndexSegmentRomEnd, &_mrsManaAndJohnTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(ADDITIONAL_NPCS_TEXT_INDEX, &_additionalNpcsTextIndexSegmentRomStart, &_additionalNpcsTextIndexSegmentRomEnd, &_additionalNpcsTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    
    // unnecessary setting for player baby and each NPC baby
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(NPC_BABY_TEXT_INDEX, &_babyTextIndexSegmentRomStart, &_babyTextIndexSegmentRomEnd, &_babyTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    
    // unnecessary additional load
    setTextAddresses(ADDITIONAL_NPCS_TEXT_INDEX, &_additionalNpcsTextIndexSegmentRomStart, &_additionalNpcsTextIndexSegmentRomEnd, &_additionalNpcsTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    
    // cutscenes and festivals
    setTextAddresses(DIALOGUE_64_TEXT_INDEX, &_dialogue64TextIndexSegmentRomStart, &_dialogue64TextIndexSegmentRomEnd, &_dialogue64TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_65_TEXT_INDEX, &_dialogue65TextIndexSegmentRomStart, &_dialogue65TextIndexSegmentRomEnd, &_dialogue65TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(CUTSCENES_1_TEXT_INDEX, &_cutscenes1TextIndexSegmentRomStart, &_cutscenes1TextIndexSegmentRomEnd, &_cutscenes1TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_54_TEXT_INDEX, &_dialogue54TextIndexSegmentRomStart, &_dialogue54TextIndexSegmentRomEnd, &_dialogue54TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(MARRIED_DIALOGUES_TEXT_INDEX, &_marriedDialoguesTextIndexSegmentRomStart, &_marriedDialoguesTextIndexSegmentRomEnd, &_marriedDialoguesTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_61_TEXT_INDEX, &_dialogue61TextIndexSegmentRomStart, &_dialogue61TextIndexSegmentRomEnd, &_dialogue61TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(KAREN_CUTSCENES_1_TEXT_INDEX, &_karenCutscenes1TextIndexSegmentRomStart, &_karenCutscenes1TextIndexSegmentRomEnd, &_karenCutscenes1TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_63_TEXT_INDEX, &_dialogue63TextIndexSegmentRomStart, &_dialogue63TextIndexSegmentRomEnd, &_dialogue63TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(KAREN_CUTSCENES_2_TEXT_INDEX, &_karenCutscenes2TextIndexSegmentRomStart, &_karenCutscenes2TextIndexSegmentRomEnd, &_karenCutscenes2TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_55_TEXT_INDEX, &_dialogue55TextIndexSegmentRomStart, &_dialogue55TextIndexSegmentRomEnd, &_dialogue55TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(HORSE_RACE_TEXT_INDEX, &_horseRaceTextIndexSegmentRomStart, &_horseRaceTextIndexSegmentRomEnd, &_horseRaceTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_49_TEXT_INDEX, &_dialogue49TextIndexSegmentRomStart, &_dialogue49TextIndexSegmentRomEnd, &_dialogue49TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(VEGETABLE_FESTIVAL_TEXT_INDEX, &_vegetableFestivalTextIndexSegmentRomStart, &_vegetableFestivalTextIndexSegmentRomEnd, &_vegetableFestivalTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_48_TEXT_INDEX, &_dialogue48TextIndexSegmentRomStart, &_dialogue48TextIndexSegmentRomEnd, &_dialogue48TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_52_TEXT_INDEX, &_dialogue52TextIndexSegmentRomStart, &_dialogue52TextIndexSegmentRomEnd, &_dialogue52TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_50_TEXT_INDEX, &_dialogue50TextIndexSegmentRomStart, &_dialogue50TextIndexSegmentRomEnd, &_dialogue50TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_51_TEXT_INDEX, &_dialogue51TextIndexSegmentRomStart, &_dialogue51TextIndexSegmentRomEnd, &_dialogue51TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DIALOGUE_56_TEXT_INDEX, &_dialogue56TextIndexSegmentRomStart, &_dialogue56TextIndexSegmentRomEnd, &_dialogue56TextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(EGG_FESTIVAL_TEXT_INDEX, &_eggFestivalTextIndexSegmentRomStart, &_eggFestivalTextIndexSegmentRomEnd, &_eggFestivalTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(DOG_RACE_TEXT_INDEX, &_dogRaceTextIndexSegmentRomStart, &_dogRaceTextIndexSegmentRomEnd, &_dogRaceTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(SPIRIT_FESTIVAL_TEXT_INDEX, &_spiritFestivalTextIndexSegmentRomStart, &_spiritFestivalTextIndexSegmentRomEnd, &_spiritFestivalTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(NEW_YEAR_FESTIVAL_TEXT_INDEX, &_newYearFestivalTextIndexSegmentRomStart, &_newYearFestivalTextIndexSegmentRomEnd, &_newYearFestivalTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    setTextAddresses(FUNERAL_INTRO_TEXT_INDEX, &_funeralIntroTextIndexSegmentRomStart, &_funeralIntroTextIndexSegmentRomEnd, &_funeralIntroTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);
    
    setTextAddresses(HOW_TO_PLAY_TEXT_INDEX, &_howToPlayTextIndexSegmentRomStart, &_howToPlayTextIndexSegmentRomEnd, &_howToPlayTextSegmentRomStart, (void*)DIALOGUE_TEXT_VADDR);

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeGameVariableStrings);

void initializeGameVariableStrings(void) {
    
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
    
    setGameVariableString(0xE, D_80237380, 6);
    
    setGameVariableString(0xF, gWifeName, 6);
    
    setGameVariableString(0x10, D_801FB5C4, 6);
    setGameVariableString(0x11, D_801594E0, 3);
    // prices
    setGameVariableString(0x12, D_801A8B50, 8);

    // animal name
    setGameVariableString(0x13, D_8018985C, 6);

    setGameVariableString(0x14, gCurrentSeasonName, 6);
    
    // current day of the month
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
    
    // unsued
    setGameVariableString(0x1F, D_8020563B, 4);

    setGameVariableString(0x20, harvestKingName, 6);

    // unused
    setGameVariableString(0x21, D_801806C8, 2);
    setGameVariableString(0x22, D_80170268, 6);
    setGameVariableString(0x23, D_801FC156, 6);
    setGameVariableString(0x24, D_80204B3C, 6);
    setGameVariableString(0x25, D_8016FBCC, 1);
    setGameVariableString(0x27, D_801886D4, 6);

    // unused
    setGameVariableString(0x30, D_801594E6, 3);
    setGameVariableString(0x31, D_8016F6E0, 3);
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

//INCLUDE_ASM("asm/nonmatchings/game/initialize", func_80054550);

void func_80054550(void) {

    func_80054734();

    initializeDialogueVariables();

    setSpecialDialogueBitsPointer(specialDialogueBits);

    func_80042FEC(0, 0, 1);

    func_8004318C(0, DIALOGUE_ICONS_1, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, DIALOGUE_ICONS_TEXTURE_VADDR_START, DIALOGUE_ICONS_TEXTURE_VADDR_END, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, NULL, 3, 0, 8.0f, -16.0f, 0);
    func_80043260(0, DIALOGUE_ICONS_2, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, DIALOGUE_ICONS_TEXTURE_VADDR_START, DIALOGUE_ICONS_TEXTURE_VADDR_END, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, NULL, 0xA, 0xFE, 0, 40.0f, 0);
    func_80043334(0, DIALOGUE_ICONS_3, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, DIALOGUE_ICONS_TEXTURE_VADDR_START, DIALOGUE_ICONS_TEXTURE_VADDR_END, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, NULL, 0xB, 0xFE, 0, -40.0f, 0);
   
    func_80043148(0, 2, 0, 8);

}
 
//INCLUDE_ASM("asm/nonmatchings/game/initialize", func_80054734);

void func_80054734(void) {

    setDialogueBytecodeAddressInfo(0, DIALOGUE_1_TEXT_INDEX, 0, &_dialogueBytecodeSegment1IndexSegmentRomStart, &_dialogueBytecodeSegment1IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment1SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(1, DIARY_TEXT_INDEX, 2, &_dialogueBytecodeSegment2IndexSegmentRomStart, &_dialogueBytecodeSegment2IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment2SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x3D, SHOP_TEXT_INDEX, 6, &_dialogueBytecodeSegment62IndexSegmentRomStart, &_dialogueBytecodeSegment62IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment62SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x3E, FESTIVAL_OVERLAY_SELECTIONS_TEXT_INDEX, 4, &_dialogueBytecodeSegment63IndexSegmentRomStart, &_dialogueBytecodeSegment63IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment63SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x44, LIBRARY_TEXT_INDEX, 1, &_dialogueBytecodeSegment69IndexSegmentRomStart, &_dialogueBytecodeSegment69IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment69SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(9, MARIA_TEXT_INDEX, 0x15, &_dialogueBytecodeSegment10IndexSegmentRomStart, &_dialogueBytecodeSegment10IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment10SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(8, POPURI_TEXT_INDEX, 0x14, &_dialogueBytecodeSegment9IndexSegmentRomStart, &_dialogueBytecodeSegment9IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment9SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(2, ELLI_TEXT_INDEX, 0xB, &_dialogueBytecodeSegment3IndexSegmentRomStart, &_dialogueBytecodeSegment3IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment3SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0xA, ANN_TEXT_INDEX, 0x17, &_dialogueBytecodeSegment11IndexSegmentRomStart, &_dialogueBytecodeSegment11IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment11SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(4, KAREN_TEXT_INDEX, 0xD, &_dialogueBytecodeSegment5IndexSegmentRomStart, &_dialogueBytecodeSegment5IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment5SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x3F, BABY_TEXT_INDEX, 0x44, &_dialogueBytecodeSegment64IndexSegmentRomStart, &_dialogueBytecodeSegment64IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment64SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(7, HARRIS_TEXT_INDEX, 0x13, &_dialogueBytecodeSegment8IndexSegmentRomStart, &_dialogueBytecodeSegment8IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment8SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0xB, GRAY_TEXT_INDEX, 0x19, &_dialogueBytecodeSegment12IndexSegmentRomStart, &_dialogueBytecodeSegment12IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment12SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(6, JEFF_TEXT_INDEX, 0x11, &_dialogueBytecodeSegment7IndexSegmentRomStart, &_dialogueBytecodeSegment7IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment7SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(5, CLIFF_TEXT_INDEX, 0x10, &_dialogueBytecodeSegment6IndexSegmentRomStart, &_dialogueBytecodeSegment6IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment6SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(3, KAI_TEXT_INDEX, 0xC, &_dialogueBytecodeSegment4IndexSegmentRomStart, &_dialogueBytecodeSegment4IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment4SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x1F, MAYOR_TEXT_INDEX, 0x21, &_dialogueBytecodeSegment32IndexSegmentRomStart, &_dialogueBytecodeSegment32IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment32SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x24, MAYOR_WIFE_TEXT_INDEX, 0x27, &_dialogueBytecodeSegment37IndexSegmentRomStart, &_dialogueBytecodeSegment37IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment37SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x14, LILIA_TEXT_INDEX, 0x1B, &_dialogueBytecodeSegment21IndexSegmentRomStart, &_dialogueBytecodeSegment21IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment21SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x13, BASIL_TEXT_INDEX, 0x1A, &_dialogueBytecodeSegment20IndexSegmentRomStart, &_dialogueBytecodeSegment20IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment20SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0xC, ELLEN_TEXT_INDEX, 0x28, &_dialogueBytecodeSegment13IndexSegmentRomStart, &_dialogueBytecodeSegment13IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment13SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x12, DOUG_TEXT_INDEX, 0x18, &_dialogueBytecodeSegment19IndexSegmentRomStart, &_dialogueBytecodeSegment19IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment19SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0xD, GOTZ_TEXT_INDEX, 0xE, &_dialogueBytecodeSegment14IndexSegmentRomStart, &_dialogueBytecodeSegment14IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment14SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0xE, SASHA_TEXT_INDEX, 0xF, &_dialogueBytecodeSegment15IndexSegmentRomStart, &_dialogueBytecodeSegment15IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment15SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x26, POTION_SHOP_DEALER_TEXT_INDEX, 0x26, &_dialogueBytecodeSegment39IndexSegmentRomStart, &_dialogueBytecodeSegment39IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment39SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0xF, KENT_TEXT_INDEX, 0x12, &_dialogueBytecodeSegment16IndexSegmentRomStart, &_dialogueBytecodeSegment16IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment16SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x11, STU_TEXT_INDEX, 0x29, &_dialogueBytecodeSegment18IndexSegmentRomStart, &_dialogueBytecodeSegment18IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment18SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x16, MIDWIFE_TEXT_INDEX, 0x2A, &_dialogueBytecodeSegment23IndexSegmentRomStart, &_dialogueBytecodeSegment23IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment23SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x10, MAY_TEXT_INDEX, 0x16, &_dialogueBytecodeSegment17IndexSegmentRomStart, &_dialogueBytecodeSegment17IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment17SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x21, RICK_TEXT_INDEX, 0x23, &_dialogueBytecodeSegment34IndexSegmentRomStart, &_dialogueBytecodeSegment34IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment34SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x25, PASTOR_TEXT_INDEX, 0x2B, &_dialogueBytecodeSegment38IndexSegmentRomStart, &_dialogueBytecodeSegment38IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment38SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x18, SHIPPER_TEXT_INDEX, 0x1D, &_dialogueBytecodeSegment25IndexSegmentRomStart, &_dialogueBytecodeSegment25IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment25SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x15, SAIBARA_TEXT_INDEX, 0x2C, &_dialogueBytecodeSegment22IndexSegmentRomStart, &_dialogueBytecodeSegment22IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment22SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x17, DUKE_TEXT_INDEX, 0x1C, &_dialogueBytecodeSegment24IndexSegmentRomStart, &_dialogueBytecodeSegment24IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment24SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x20, GREG_TEXT_INDEX, 0x22, &_dialogueBytecodeSegment33IndexSegmentRomStart, &_dialogueBytecodeSegment33IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment33SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x1C, ASSISTANT_CARPENTERS_TEXT_INDEX, 0x1F, &_dialogueBytecodeSegment29IndexSegmentRomStart, &_dialogueBytecodeSegment29IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment29SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x1D, ASSISTANT_CARPENTERS_TEXT_INDEX, 0x1F, &_dialogueBytecodeSegment30IndexSegmentRomStart, &_dialogueBytecodeSegment30IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment30SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x1E, MASTER_CARPENTER_TEXT_INDEX, 0x20, &_dialogueBytecodeSegment31IndexSegmentRomStart, &_dialogueBytecodeSegment31IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment31SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x19, HARVEST_SPRITES_TEXT_INDEX, 0x1E, &_dialogueBytecodeSegment26IndexSegmentRomStart, &_dialogueBytecodeSegment26IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment26SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x1A, HARVEST_SPRITES_TEXT_INDEX, 0x1E, &_dialogueBytecodeSegment27IndexSegmentRomStart, &_dialogueBytecodeSegment27IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment27SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x1B, HARVEST_SPRITES_TEXT_INDEX, 0x1E, &_dialogueBytecodeSegment28IndexSegmentRomStart, &_dialogueBytecodeSegment28IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment28SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x22, BARLEY_TEXT_INDEX, 0x24, &_dialogueBytecodeSegment35IndexSegmentRomStart, &_dialogueBytecodeSegment35IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment35SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x23, SYDNEY_TEXT_INDEX, 0x25, &_dialogueBytecodeSegment36IndexSegmentRomStart, &_dialogueBytecodeSegment36IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment36SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x40, MRS_MANA_AND_JOHN_TEXT_INDEX, 0x45, &_dialogueBytecodeSegment65IndexSegmentRomStart, &_dialogueBytecodeSegment65IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment65SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x41, MRS_MANA_AND_JOHN_TEXT_INDEX, 0x45, &_dialogueBytecodeSegment66IndexSegmentRomStart, &_dialogueBytecodeSegment66IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment66SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x42, ADDITIONAL_NPCS_TEXT_INDEX, 0x46, &_dialogueBytecodeSegment67IndexSegmentRomStart, &_dialogueBytecodeSegment67IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment67SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x43, NPC_BABY_TEXT_INDEX, 0x47, &_dialogueBytecodeSegment68IndexSegmentRomStart, &_dialogueBytecodeSegment68IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment68SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x43, NPC_BABY_TEXT_INDEX, 0x47, &_dialogueBytecodeSegment68IndexSegmentRomStart, &_dialogueBytecodeSegment68IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment68SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x43, NPC_BABY_TEXT_INDEX, 0x47, &_dialogueBytecodeSegment68IndexSegmentRomStart, &_dialogueBytecodeSegment68IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment68SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x43, NPC_BABY_TEXT_INDEX, 0x47, &_dialogueBytecodeSegment68IndexSegmentRomStart, &_dialogueBytecodeSegment68IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment68SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x43, NPC_BABY_TEXT_INDEX, 0x47, &_dialogueBytecodeSegment68IndexSegmentRomStart, &_dialogueBytecodeSegment68IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment68SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x45, ADDITIONAL_NPCS_TEXT_INDEX, 0x46, &_dialogueBytecodeSegment70IndexSegmentRomStart, &_dialogueBytecodeSegment70IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment70SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x35, DIALOGUE_64_TEXT_INDEX, 0x3F, &_dialogueBytecodeSegment54IndexSegmentRomStart, &_dialogueBytecodeSegment54IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment54SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x36, DIALOGUE_65_TEXT_INDEX, 0x40, &_dialogueBytecodeSegment55IndexSegmentRomStart, &_dialogueBytecodeSegment55IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment55SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x27, CUTSCENES_1_TEXT_INDEX, 0x2D, &_dialogueBytecodeSegment40IndexSegmentRomStart, &_dialogueBytecodeSegment40IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment40SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x2F, DIALOGUE_54_TEXT_INDEX, 0x35, &_dialogueBytecodeSegment48IndexSegmentRomStart, &_dialogueBytecodeSegment48IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment48SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x38, MARRIED_DIALOGUES_TEXT_INDEX, 0x3B, &_dialogueBytecodeSegment57IndexSegmentRomStart, &_dialogueBytecodeSegment57IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment57SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x39, DIALOGUE_61_TEXT_INDEX, 0x3C, &_dialogueBytecodeSegment58IndexSegmentRomStart, &_dialogueBytecodeSegment58IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment58SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x33, KAREN_CUTSCENES_1_TEXT_INDEX, 0x3D, &_dialogueBytecodeSegment52IndexSegmentRomStart, &_dialogueBytecodeSegment52IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment52SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x34, DIALOGUE_63_TEXT_INDEX, 0x3E, &_dialogueBytecodeSegment53IndexSegmentRomStart, &_dialogueBytecodeSegment53IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment53SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x3C, KAREN_CUTSCENES_2_TEXT_INDEX, 0x42, &_dialogueBytecodeSegment61IndexSegmentRomStart, &_dialogueBytecodeSegment61IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment61SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x30, DIALOGUE_55_TEXT_INDEX, 0x36, &_dialogueBytecodeSegment49IndexSegmentRomStart, &_dialogueBytecodeSegment49IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment49SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x32, HORSE_RACE_TEXT_INDEX, 0x3A, &_dialogueBytecodeSegment51IndexSegmentRomStart, &_dialogueBytecodeSegment51IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment51SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x29, DIALOGUE_49_TEXT_INDEX, 0x30, &_dialogueBytecodeSegment42IndexSegmentRomStart, &_dialogueBytecodeSegment42IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment42SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x2E, VEGETABLE_FESTIVAL_TEXT_INDEX, 0x43, &_dialogueBytecodeSegment47IndexSegmentRomStart, &_dialogueBytecodeSegment47IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment47SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x28, DIALOGUE_48_TEXT_INDEX, 0x2F, &_dialogueBytecodeSegment41IndexSegmentRomStart, &_dialogueBytecodeSegment41IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment41SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x2C, DIALOGUE_52_TEXT_INDEX, 0x33, &_dialogueBytecodeSegment45IndexSegmentRomStart, &_dialogueBytecodeSegment45IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment45SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x2A, DIALOGUE_50_TEXT_INDEX, 0x31, &_dialogueBytecodeSegment43IndexSegmentRomStart, &_dialogueBytecodeSegment43IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment43SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x2B, DIALOGUE_51_TEXT_INDEX, 0x32, &_dialogueBytecodeSegment44IndexSegmentRomStart, &_dialogueBytecodeSegment44IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment44SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x31, DIALOGUE_56_TEXT_INDEX, 0x37, &_dialogueBytecodeSegment50IndexSegmentRomStart, &_dialogueBytecodeSegment50IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment50SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x37, EGG_FESTIVAL_TEXT_INDEX, 0x41, &_dialogueBytecodeSegment56IndexSegmentRomStart, &_dialogueBytecodeSegment56IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment56SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x2D, DOG_RACE_TEXT_INDEX, 0x34, &_dialogueBytecodeSegment46IndexSegmentRomStart, &_dialogueBytecodeSegment46IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment46SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x3B, NEW_YEAR_FESTIVAL_TEXT_INDEX, 0x38, &_dialogueBytecodeSegment60IndexSegmentRomStart, &_dialogueBytecodeSegment60IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment60SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);
    setDialogueBytecodeAddressInfo(0x3A, NAMING_SCREEN_TEXT_INDEX, 0xA, &_dialogueBytecodeSegment59IndexSegmentRomStart, &_dialogueBytecodeSegment59IndexSegmentRomEnd, (void*)DIALOGUE_BYTECODE_INDEX_VADDR, &_dialogueBytecodeSegment59SegmentRomStart, (void*)DIALOGUE_BYTECODE_VADDR);

}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeDialogueVariables);

void initializeDialogueVariables(void) {

    // alcohol tolerance is first lol
    setDialogueVariable(0, &gAlcoholTolerance, UNSIGNED_CHAR, MAX_ALCOHOL_TOLERANCE );

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
    setDialogueVariable(0x2B, &D_801654F4, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2C, &D_80237414, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2D, &D_801C3F80, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2E, &D_80188F60, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x2F, &D_801FB5D0, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);
    setDialogueVariable(0x30, &D_801FB6FC, UNSIGNED_INT, MAX_ITEM_SHIPPING_VALUE);

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