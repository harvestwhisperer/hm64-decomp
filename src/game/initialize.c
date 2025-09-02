#include "common.h"

#include "game/initialize.h"

#include "ld_symbols.h"

#include "data/animationScripts/animationScripts.h"

#include "system/audio.h"
#include "system/cutscene.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/memory.h"
#include "system/message.h"
#include "system/mapContext.h"

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
#include "game/mapObjects.h"
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

// todo: relocate
// character sprite data
extern void* D_8011BD18;

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

// forward declaration
void func_8004F768(void);
void func_80054550(void);
bool func_80054734(void);
void initializeGameVariables(void);
void loadMapAddresses(void);
void registerMainLoopCallbacks(void);
void initializeDialogueVariables(void);


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

    gPlayer.fatigue.counter = 0;
    gPlayer.fatigue.level = 0;
    gPlayer.fatigue.unk_2 = 0;
    gPlayer.shopItemIndex = 0;
    gPlayer.direction = NORTH;
    gPlayer.unk_70 = 0;
    gPlayer.currentAction = 0;
    gPlayer.nextAction = 0;
    gPlayer.actionProgress = 0;
    gPlayer.animationState = 0;
    gPlayer.fatigue.unk_3 = 0;
    gPlayer.flags = 0;
    gPlayer.unk_60 = 0.0f;
    gPlayer.toolHeldCounter = 0;
    gPlayer.staminaLevelForCurrentToolUse = 0;
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

// initialize bss game variables
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

    // cutscene location
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
    toolUse.unk_2 = 0;
    toolUse.unk_3 = 0;
    toolUse.unk_4 = 0;
    toolUse.unk_6 = 0;
    toolUse.unk_8 = 0;
    toolUse.unk_A = 0;
    toolUse.unk_C = 0;
    toolUse.unk_E = 0;

    for (i = 0; i < TOTAL_NPCS; i++) {
        npcInfoArray[i].entityIndex = 0;
        npcInfoArray[i].unk_1A = 0;
        npcInfoArray[i].unk_1B = 0;
        npcInfoArray[i].levelIndex = 0;
        npcInfoArray[i].movingFlag = 0;
        npcInfoArray[i].unk_1E = 0;
        npcInfoArray[i].direction = NORTH;
        npcInfoArray[i].unk_20 = 0;
        npcInfoArray[i].unk_21 = 0;
        npcInfoArray[i].unk_22 = 0;
        npcInfoArray[i].location = 0;
    }

    for (i = 0; i < TOTAL_NPCS; i++) {
        npcAffection[i] = 0;
    }

    dogInfo.coordinates.x = 0;
    dogInfo.coordinates.y = 0;
    dogInfo.coordinates.z = 0;
    dogInfo.location = 0x52;
    dogInfo.speed = 0;
    dogInfo.direction = NORTH;
    dogInfo.unk_19 = 0;
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
    horseInfo.location = 0x52;
    horseInfo.speed = 0;
    horseInfo.direction = NORTH;
    horseInfo.unk_19 = 0;
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
        gChickens[i].direction = NORTH;
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

        gChickenEggs[i].unk_15 = 0;
        gChickenEggs[i].mapIndex = 0;
        gChickenEggs[i].unk_F = 0;
        gChickenEggs[i].direction = 0;
        gChickenEggs[i].zDisplacement = 0;
        gChickenEggs[i].unk_13 = 0;
        gChickenEggs[i].unk_14 = 0;
        gChickenEggs[i].unk_0.x = 0;
        gChickenEggs[i].unk_0.y = 0;
        gChickenEggs[i].unk_0.z = 0;
        gChickenEggs[i].flags = 0;

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
        itemInfo[i].unk_26 = 0;
        itemInfo[i].unk_28 = 0;
        
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
    D_8016FBCD = 0;
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
    
    readMemory((u32)D_80113580, (u32)farmFieldTiles, 0x1E0);

    readMemory((u32)D_80113760, (u32)D_80182BA8, 0x1E0);
        
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

// TODO: finish adding entity name macros to function calls
void func_8004F768(void) {
    
    initializeShadowSprite(0, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsIndexSegmentRomStart, &_shadowsIndexSegmentRomEnd, (void*)SHADOW_TEXTURE_VADDR_START, (void*)SHADOW_TEXTURE_VADDR_END, (void*)SHADOW_ASSETS_INDEX_VADDR_START, (void*)SHADOW_ASSETS_INDEX_VADDR_END, 0, 0);
    initializeShadowSprite(1, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsIndexSegmentRomStart, &_shadowsIndexSegmentRomEnd, (void*)SHADOW_TEXTURE_VADDR_START, (void*)SHADOW_TEXTURE_VADDR_END, (void*)SHADOW_ASSETS_INDEX_VADDR_START, (void*)SHADOW_ASSETS_INDEX_VADDR_END, 1, 0);
    initializeShadowSprite(2, &_shadowsTextureSegmentRomStart, &_shadowsTextureSegmentRomEnd, &_shadowsIndexSegmentRomStart, &_shadowsIndexSegmentRomEnd, (void*)SHADOW_TEXTURE_VADDR_START, (void*)SHADOW_TEXTURE_VADDR_END, (void*)SHADOW_ASSETS_INDEX_VADDR_START, (void*)SHADOW_ASSETS_INDEX_VADDR_END, 2, 0);
 
    initializeEntityAsset(ENTITY_PLAYER, &_playerTextureSegmentRomStart, &_playerTextureSegmentRomEnd, &_playerAssetsIndexSegmentRomStart, &_playerAssetsIndexSegmentRomEnd, &_playerSpritesheetIndexSegmentRomStart, &_playerSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PLAYER, 0xF, 0xF);
    func_8002FFF4(ENTITY_PLAYER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MARIA, &_mariaTextureSegmentRomStart, &_mariaTextureSegmentRomEnd, &_mariaAssetsIndexSegmentRomStart, &_mariaAssetsIndexSegmentRomEnd, &_mariaSpritesheetIndexSegmentRomStart, &_mariaSpritesheetIndexSegmentRomEnd, 1, 0, &mariaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MARIA, 0xF, 0xF);
    func_8002FFF4(ENTITY_MARIA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_POPURI, &_popuriTextureSegmentRomStart, &_popuriTextureSegmentRomEnd, &_popuriAssetsIndexSegmentRomStart, &_popuriAssetsIndexSegmentRomEnd, &_popuriSpritesheetIndexSegmentRomStart, &_popuriSpritesheetIndexSegmentRomEnd, 1, 0, &popuriAnimationScripts);
    setEntityCollisionBuffers(ENTITY_POPURI, 0xF, 0xF);
    func_8002FFF4(ENTITY_POPURI, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ELLI, &_elliTextureSegmentRomStart, &_elliTextureSegmentRomEnd, &_elliAssetsIndexSegmentRomStart, &_elliAssetsIndexSegmentRomEnd, &_elliSpritesheetIndexSegmentRomStart, &_elliSpritesheetIndexSegmentRomEnd, 1, 0, &elliAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ELLI, 0xF, 0xF);
    func_8002FFF4(ENTITY_ELLI, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ANN, &_annTextureSegmentRomStart, &_annTextureSegmentRomEnd, &_annAssetsIndexSegmentRomStart, &_annAssetsIndexSegmentRomEnd, &_annSpritesheetIndexSegmentRomStart, &_annSpritesheetIndexSegmentRomEnd, 1, 0, &annAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ANN, 0xF, 0xF);
    func_8002FFF4(ENTITY_ANN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAREN, &_karenTextureSegmentRomStart, &_karenTextureSegmentRomEnd, &_karenAssetsIndexSegmentRomStart, &_karenAssetsIndexSegmentRomEnd, &_karenSpritesheetIndexSegmentRomStart, &_karenSpritesheetIndexSegmentRomEnd, 1, 0, &karenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KAREN, 0xF, 0xF);
    func_8002FFF4(ENTITY_KAREN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BABY, &_babyTextureSegmentRomStart, &_babyTextureSegmentRomEnd, &_babyAssetsIndexSegmentRomStart, &_babyAssetsIndexSegmentRomEnd, &_babySpritesheetIndexSegmentRomStart, &_babySpritesheetIndexSegmentRomEnd, 1, 0, &babyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BABY, 0xF, 0xF);
    func_8002FFF4(ENTITY_BABY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HARRIS, &_harrisTextureSegmentRomStart, &_harrisTextureSegmentRomEnd, &_harrisAssetsIndexSegmentRomStart, &_harrisAssetsIndexSegmentRomEnd, &_harrisSpritesheetIndexSegmentRomStart, &_harrisSpritesheetIndexSegmentRomEnd, 1, 0, &harrisAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HARRIS, 0xF, 0xF);
    func_8002FFF4(ENTITY_HARRIS, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GRAY, &_grayTextureSegmentRomStart, &_grayTextureSegmentRomEnd, &_grayAssetsIndexSegmentRomStart, &_grayAssetsIndexSegmentRomEnd, &_graySpritesheetIndexSegmentRomStart, &_graySpritesheetIndexSegmentRomEnd, 1, 0, &grayAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GRAY, 0xF, 0xF);
    func_8002FFF4(ENTITY_GRAY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_JEFF, &_jeffTextureSegmentRomStart, &_jeffTextureSegmentRomEnd, &_jeffAssetsIndexSegmentRomStart, &_jeffAssetsIndexSegmentRomEnd, &_jeffSpritesheetIndexSegmentRomStart, &_jeffSpritesheetIndexSegmentRomEnd, 1, 0, &jeffAnimationScripts);
    setEntityCollisionBuffers(ENTITY_JEFF, 0xF, 0xF);
    func_8002FFF4(ENTITY_JEFF, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CLIFF, &_cliffTextureSegmentRomStart, &_cliffTextureSegmentRomEnd, &_cliffAssetsIndexSegmentRomStart, &_cliffAssetsIndexSegmentRomEnd, &_cliffSpritesheetIndexSegmentRomStart, &_cliffSpritesheetIndexSegmentRomEnd, 1, 0, &cliffAnimationScripts);
    setEntityCollisionBuffers(ENTITY_CLIFF, 0xF, 0xF);
    func_8002FFF4(ENTITY_CLIFF, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAI, &_kaiTextureSegmentRomStart, &_kaiTextureSegmentRomEnd, &_kaiAssetsIndexSegmentRomStart, &_kaiAssetsIndexSegmentRomEnd, &_kaiSpritesheetIndexSegmentRomStart, &_kaiSpritesheetIndexSegmentRomEnd, 1, 0, &kaiAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KAI, 0xF, 0xF);
    func_8002FFF4(ENTITY_KAI, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MAYOR, &_mayorTextureSegmentRomStart, &_mayorTextureSegmentRomEnd, &_mayorAssetsIndexSegmentRomStart, &_mayorAssetsIndexSegmentRomEnd, &_mayorSpritesheetIndexSegmentRomStart, &_mayorSpritesheetIndexSegmentRomEnd, 1, 0, &mayorAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MAYOR, 0xF, 0xF);
    func_8002FFF4(ENTITY_MAYOR, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MAYOR_WIFE, &_mayorWifeTextureSegmentRomStart, &_mayorWifeTextureSegmentRomEnd, &_mayorWifeAssetsIndexSegmentRomStart, &_mayorWifeAssetsIndexSegmentRomEnd, &_mayorWifeSpritesheetIndexSegmentRomStart, &_mayorWifeSpritesheetIndexSegmentRomEnd, 1, 0, &mayorWifeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MAYOR_WIFE, 0xF, 0xF);
    func_8002FFF4(ENTITY_MAYOR_WIFE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_LILIA, &_liliaTextureSegmentRomStart, &_liliaTextureSegmentRomEnd, &_liliaAssetsIndexSegmentRomStart, &_liliaAssetsIndexSegmentRomEnd, &_liliaSpritesheetIndexSegmentRomStart, &_liliaSpritesheetIndexSegmentRomEnd, 1, 0, &liliaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_LILIA, 0xF, 0xF);
    func_8002FFF4(ENTITY_LILIA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BASIL, &_basilTextureSegmentRomStart, &_basilTextureSegmentRomEnd, &_basilAssetsIndexSegmentRomStart, &_basilAssetsIndexSegmentRomEnd, &_basilSpritesheetIndexSegmentRomStart, &_basilSpritesheetIndexSegmentRomEnd, 1, 0, &basilAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BASIL, 0xF, 0xF);
    func_8002FFF4(ENTITY_BASIL, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ELLEN, &_ellenTextureSegmentRomStart, &_ellenTextureSegmentRomEnd, &_ellenAssetsIndexSegmentRomStart, &_ellenAssetsIndexSegmentRomEnd, &_ellenSpritesheetIndexSegmentRomStart, &_ellenSpritesheetIndexSegmentRomEnd, 1, 0, &ellenAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ELLEN, 0xF, 0xF);
    func_8002FFF4(ENTITY_ELLEN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOUG, &_dougTextureSegmentRomStart, &_dougTextureSegmentRomEnd, &_dougAssetsIndexSegmentRomStart, &_dougAssetsIndexSegmentRomEnd, &_dougSpritesheetIndexSegmentRomStart, &_dougSpritesheetIndexSegmentRomEnd, 1, 0, &dougAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DOUG, 0xF, 0xF);
    func_8002FFF4(ENTITY_DOUG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GOTZ, &_gotzTextureSegmentRomStart, &_gotzTextureSegmentRomEnd, &_gotzAssetsIndexSegmentRomStart, &_gotzAssetsIndexSegmentRomEnd, &_gotzSpritesheetIndexSegmentRomStart, &_gotzSpritesheetIndexSegmentRomEnd, 1, 0, &gotzAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GOTZ, 0xF, 0xF);
    func_8002FFF4(ENTITY_GOTZ, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SASHA, &_sashaTextureSegmentRomStart, &_sashaTextureSegmentRomEnd, &_sashaAssetsIndexSegmentRomStart, &_sashaAssetsIndexSegmentRomEnd, &_sashaSpritesheetIndexSegmentRomStart, &_sashaSpritesheetIndexSegmentRomEnd, 1, 0, &sashaAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SASHA, 0xF, 0xF);
    func_8002FFF4(ENTITY_SASHA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_POTION_SHOP_DEALER, &_potionShopDealerTextureSegmentRomStart, &_potionShopDealerTextureSegmentRomEnd, &_potionShopDealerAssetsIndexSegmentRomStart, &_potionShopDealerAssetsIndexSegmentRomEnd, &_potionShopDealerSpritesheetIndexSegmentRomStart, &_potionShopDealerSpritesheetIndexSegmentRomEnd, 1, 0, &potionShopDealerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_POTION_SHOP_DEALER, 0xF, 0xF);
    func_8002FFF4(ENTITY_POTION_SHOP_DEALER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KENT, &_kentTextureSegmentRomStart, &_kentTextureSegmentRomEnd, &_kentAssetsIndexSegmentRomStart, &_kentAssetsIndexSegmentRomEnd, &_kentSpritesheetIndexSegmentRomStart, &_kentSpritesheetIndexSegmentRomEnd, 1, 0, &kentAnimationScripts);
    setEntityCollisionBuffers(ENTITY_KENT, 0xF, 0xF);
    func_8002FFF4(ENTITY_KENT, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_STU, &_stuTextureSegmentRomStart, &_stuTextureSegmentRomEnd, &_stuAssetsIndexSegmentRomStart, &_stuAssetsIndexSegmentRomEnd, &_stuSpritesheetIndexSegmentRomStart, &_stuSpritesheetIndexSegmentRomEnd, 1, 0, &stuAnimationScripts);
    setEntityCollisionBuffers(ENTITY_STU, 0xF, 0xF);
    func_8002FFF4(ENTITY_STU, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MIDWIFE, &_midwifeTextureSegmentRomStart, &_midwifeTextureSegmentRomEnd, &_midwifeAssetsIndexSegmentRomStart, &_midwifeAssetsIndexSegmentRomEnd, &_midwifeSpritesheetIndexSegmentRomStart, &_midwifeSpritesheetIndexSegmentRomEnd, 1, 0, &midwifeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MIDWIFE, 0xF, 0xF);
    func_8002FFF4(ENTITY_MIDWIFE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MAY, &_mayTextureSegmentRomStart, &_mayTextureSegmentRomEnd, &_mayAssetsIndexSegmentRomStart, &_mayAssetsIndexSegmentRomEnd, &_maySpritesheetIndexSegmentRomStart, &_maySpritesheetIndexSegmentRomEnd, 1, 0, &mayAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MAY, 0xF, 0xF);
    func_8002FFF4(ENTITY_MAY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_RICK, &_rickTextureSegmentRomStart, &_rickTextureSegmentRomEnd, &_rickAssetsIndexSegmentRomStart, &_rickAssetsIndexSegmentRomEnd, &_rickSpritesheetIndexSegmentRomStart, &_rickSpritesheetIndexSegmentRomEnd, 1, 0, &rickAnimationScripts);
    setEntityCollisionBuffers(ENTITY_RICK, 0xF, 0xF);
    func_8002FFF4(ENTITY_RICK, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PASTOR, &_pastorTextureSegmentRomStart, &_pastorTextureSegmentRomEnd, &_pastorAssetsIndexSegmentRomStart, &_pastorAssetsIndexSegmentRomEnd, &_pastorSpritesheetIndexSegmentRomStart, &_pastorSpritesheetIndexSegmentRomEnd, 1, 0, &pastorAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PASTOR, 0xF, 0xF);
    func_8002FFF4(ENTITY_PASTOR, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SHIPPER, &_shipperTextureSegmentRomStart, &_shipperTextureSegmentRomEnd, &_shipperAssetsIndexSegmentRomStart, &_shipperAssetsIndexSegmentRomEnd, &_shipperSpritesheetIndexSegmentRomStart, &_shipperSpritesheetIndexSegmentRomEnd, 1, 0, &shipperAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SHIPPER, 0xF, 0xF);
    func_8002FFF4(ENTITY_SHIPPER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SAIBARA, &_saibaraTextureSegmentRomStart, &_saibaraTextureSegmentRomEnd, &_saibaraAssetsIndexSegmentRomStart, &_saibaraAssetsIndexSegmentRomEnd, &_saibaraSpritesheetIndexSegmentRomStart, &_saibaraSpritesheetIndexSegmentRomEnd, 1, 0, &saibaraAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SAIBARA, 0xF, 0xF);
    func_8002FFF4(ENTITY_SAIBARA, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DUKE, &_dukeTextureSegmentRomStart, &_dukeTextureSegmentRomEnd, &_dukeAssetsIndexSegmentRomStart, &_dukeAssetsIndexSegmentRomEnd, &_dukeSpritesheetIndexSegmentRomStart, &_dukeSpritesheetIndexSegmentRomEnd, 1, 0, &dukeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_DUKE, 0xF, 0xF);
    func_8002FFF4(ENTITY_DUKE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GREG, &_gregTextureSegmentRomStart, &_gregTextureSegmentRomEnd, &_gregAssetsIndexSegmentRomStart, &_gregAssetsIndexSegmentRomEnd, &_gregSpritesheetIndexSegmentRomStart, &_gregSpritesheetIndexSegmentRomEnd, 1, 0, &gregAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GREG, 0xF, 0xF);
    func_8002FFF4(ENTITY_GREG, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ASSISTANT_CARPENTER, &_assistantCarpenterTextureSegmentRomStart, &_assistantCarpenterTextureSegmentRomEnd, &_assistantCarpenterAssetsIndexSegmentRomStart, &_assistantCarpenterAssetsIndexSegmentRomEnd, &_assistantCarpenterSpritesheetIndexSegmentRomStart, &_assistantCarpenterSpritesheetIndexSegmentRomEnd, 1, 0, &assistantCarpenterAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ASSISTANT_CARPENTER, 0xF, 0xF);
    func_8002FFF4(ENTITY_ASSISTANT_CARPENTER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MASTER_CARPENTER, &_masterCarpenterTextureSegmentRomStart, &_masterCarpenterTextureSegmentRomEnd, &_masterCarpenterAssetsIndexSegmentRomStart, &_masterCarpenterAssetsIndexSegmentRomEnd, &_masterCarpenterSpritesheetIndexSegmentRomStart, &_masterCarpenterSpritesheetIndexSegmentRomEnd, 1, 0, &masterCarpenterAnimationScripts);
    setEntityCollisionBuffers(ENTITY_MASTER_CARPENTER, 0xF, 0xF);
    func_8002FFF4(ENTITY_MASTER_CARPENTER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HARVEST_SPRITE, &_harvestSpriteTextureSegmentRomStart, &_harvestSpriteTextureSegmentRomEnd, &_harvestSpriteAssetsIndexSegmentRomStart, &_harvestSpriteAssetsIndexSegmentRomEnd, &_harvestSpriteSpritesheetIndexSegmentRomStart, &_harvestSpriteSpritesheetIndexSegmentRomEnd, 1, 0, &harvestSpriteAnimationScripts);
    setEntityCollisionBuffers(ENTITY_HARVEST_SPRITE, 0xF, 0xF);
    func_8002FFF4(ENTITY_HARVEST_SPRITE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BARLEY, &_barleyTextureSegmentRomStart, &_barleyTextureSegmentRomEnd, &_barleyAssetsIndexSegmentRomStart, &_barleyAssetsIndexSegmentRomEnd, &_barleySpritesheetIndexSegmentRomStart, &_barleySpritesheetIndexSegmentRomEnd, 1, 0, &barleyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_BARLEY, 0xF, 0xF);
    func_8002FFF4(ENTITY_BARLEY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SYDNEY, &_sydneyTextureSegmentRomStart, &_sydneyTextureSegmentRomEnd, &_sydneyAssetsIndexSegmentRomStart, &_sydneyAssetsIndexSegmentRomEnd, &_sydneySpritesheetIndexSegmentRomStart, &_sydneySpritesheetIndexSegmentRomEnd, 1, 0, &sydneyAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SYDNEY, 0xF, 0xF);
    func_8002FFF4(ENTITY_SYDNEY, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ENTOMOLOGIST, &_entomologistTextureSegmentRomStart, &_entomologistTextureSegmentRomEnd, &_entomologistAssetsIndexSegmentRomStart, &_entomologistAssetsIndexSegmentRomEnd, &_entomologistSpritesheetIndexSegmentRomStart, &_entomologistSpritesheetIndexSegmentRomEnd, 1, 0, &entomologistAnimationScripts);
    setEntityCollisionBuffers(ENTITY_ENTOMOLOGIST, 0xF, 0xF);
    func_8002FFF4(ENTITY_ENTOMOLOGIST, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GOURMET_JUDGE, &_gourmetJudgeTextureSegmentRomStart, &_gourmetJudgeTextureSegmentRomEnd, &_gourmetJudgeAssetsIndexSegmentRomStart, &_gourmetJudgeAssetsIndexSegmentRomEnd, &_gourmetJudgeSpritesheetIndexSegmentRomStart, &_gourmetJudgeSpritesheetIndexSegmentRomEnd, 1, 0, &gourmetJudgeAnimationScripts);
    setEntityCollisionBuffers(ENTITY_GOURMET_JUDGE, 0xF, 0xF);
    func_8002FFF4(ENTITY_GOURMET_JUDGE, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PHOTOGRAPHER, &_photographerTextureSegmentRomStart, &_photographerTextureSegmentRomEnd, &_photographerAssetsIndexSegmentRomStart, &_photographerAssetsIndexSegmentRomEnd, &_photographerSpritesheetIndexSegmentRomStart, &_photographerSpritesheetIndexSegmentRomEnd, 1, 0, &photographerAnimationScripts);
    setEntityCollisionBuffers(ENTITY_PHOTOGRAPHER, 0xF, 0xF);
    func_8002FFF4(ENTITY_PHOTOGRAPHER, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SHADY_SALESMAN, &_shadySalesmanTextureSegmentRomStart, &_shadySalesmanTextureSegmentRomEnd, &_shadySalesmanAssetsIndexSegmentRomStart, &_shadySalesmanAssetsIndexSegmentRomEnd, &_shadySalesmanSpritesheetIndexSegmentRomStart, &_shadySalesmanSpritesheetIndexSegmentRomEnd, 1, 0, &shadySalesmanAnimationScripts);
    setEntityCollisionBuffers(ENTITY_SHADY_SALESMAN, 0xF, 0xF);
    func_8002FFF4(ENTITY_SHADY_SALESMAN, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MRS_MANA_SON, &_mrsManaSonTextureSegmentRomStart, &_mrsManaSonTextureSegmentRomEnd, &_mrsManaSonAssetsIndexSegmentRomStart, &_mrsManaSonAssetsIndexSegmentRomEnd, &_mrsManaSonSpritesheetIndexSegmentRomStart, &_mrsManaSonSpritesheetIndexSegmentRomEnd, 1, 0, &mrsManaSonAnimationScripts);
    setEntityCollisionBuffers(0x28, 0xF, 0xF);
    func_8002FFF4(0x28, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MRS_MANA, &_mrsManaTextureSegmentRomStart, &_mrsManaTextureSegmentRomEnd, &_mrsManaAssetsIndexSegmentRomStart, &_mrsManaAssetsIndexSegmentRomEnd, &_mrsManaSpritesheetIndexSegmentRomStart, &_mrsManaSpritesheetIndexSegmentRomEnd, 1, 0, &mrsManaAnimationScripts);
    setEntityCollisionBuffers(0x29, 0xF, 0xF);
    func_8002FFF4(0x29, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_OLD_MAN, &_oldManTextureSegmentRomStart, &_oldManTextureSegmentRomEnd, &_oldManAssetsIndexSegmentRomStart, &_oldManAssetsIndexSegmentRomEnd, &_oldManSpritesheetIndexSegmentRomStart, &_oldManSpritesheetIndexSegmentRomEnd, 1, 0, &oldManAnimationScripts);
    setEntityCollisionBuffers(0x2A, 0xF, 0xF);
    func_8002FFF4(0x2A, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_OLD_WOMAN, &_oldWomanTextureSegmentRomStart, &_oldWomanTextureSegmentRomEnd, &_oldWomanAssetsIndexSegmentRomStart, &_oldWomanAssetsIndexSegmentRomEnd, &_oldWomanSpritesheetIndexSegmentRomStart, &_oldWomanSpritesheetIndexSegmentRomEnd, 1, 0, &oldWomanAnimationScripts);
    setEntityCollisionBuffers(0x2B, 0xF, 0xF);
    func_8002FFF4(0x2B, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_TOURIST_COUPLE_MAN, &_touristCoupleManTextureSegmentRomStart, &_touristCoupleManTextureSegmentRomEnd, &_touristCoupleManAssetsIndexSegmentRomStart, &_touristCoupleManAssetsIndexSegmentRomEnd, &_touristCoupleManSpritesheetIndexSegmentRomStart, &_touristCoupleManSpritesheetIndexSegmentRomEnd, 1, 0, &touristCoupleManAnimationScripts);
    setEntityCollisionBuffers(0x2C, 0xF, 0xF);
    func_8002FFF4(0x2C, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_TOURIST_COUPLE_WOMAN, &_touristCoupleWomanTextureSegmentRomStart, &_touristCoupleWomanTextureSegmentRomEnd, &_touristCoupleWomanAssetsIndexSegmentRomStart, &_touristCoupleWomanAssetsIndexSegmentRomEnd, &_touristCoupleWomanSpritesheetIndexSegmentRomStart, &_touristCoupleWomanSpritesheetIndexSegmentRomEnd, 1, 0, &touristCoupleWomanAnimationScripts);
    setEntityCollisionBuffers(0x2D, 0xF, 0xF);
    func_8002FFF4(0x2D, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FESTIVAL_GIRL1, &_festivalGirl1TextureSegmentRomStart, &_festivalGirl1TextureSegmentRomEnd, &_festivalGirl1AssetsIndexSegmentRomStart, &_festivalGirl1AssetsIndexSegmentRomEnd, &_festivalGirl1SpritesheetIndexSegmentRomStart, &_festivalGirl1SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl1AnimationScripts);
    setEntityCollisionBuffers(0x2E, 0xF, 0xF);
    func_8002FFF4(0x2E, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FESTIVAL_GIRL2, &_festivalGirl2TextureSegmentRomStart, &_festivalGirl2TextureSegmentRomEnd, &_festivalGirl2AssetsIndexSegmentRomStart, &_festivalGirl2AssetsIndexSegmentRomEnd, &_festivalGirl2SpritesheetIndexSegmentRomStart, &_festivalGirl2SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl2AnimationScripts);
    setEntityCollisionBuffers(0x2F, 0xF, 0xF);
    func_8002FFF4(0x2F, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FESTIVAL_GIRL3, &_festivalGirl3TextureSegmentRomStart, &_festivalGirl3TextureSegmentRomEnd, &_festivalGirl3AssetsIndexSegmentRomStart, &_festivalGirl3AssetsIndexSegmentRomEnd, &_festivalGirl3SpritesheetIndexSegmentRomStart, &_festivalGirl3SpritesheetIndexSegmentRomEnd, 1, 0, &festivalGirl3AnimationScripts);
    setEntityCollisionBuffers(0x30, 0xF, 0xF);
    func_8002FFF4(0x30, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HARVEST_GODDESS, &_harvestGoddessTextureSegmentRomStart, &_harvestGoddessTextureSegmentRomEnd, &_harvestGoddessAssetsIndexSegmentRomStart, &_harvestGoddessAssetsIndexSegmentRomEnd, &_harvestGoddessSpritesheetIndexSegmentRomStart, &_harvestGoddessSpritesheetIndexSegmentRomEnd, 1, 0, &harvestGoddessAnimationScripts);
    setEntityCollisionBuffers(0x31, 0xF, 0xF);
    func_8002FFF4(0x31, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SPIRIT_FESTIVAL_OCARINA_PERFORMER, &_spiritFestivalOcarinaPerformerTextureSegmentRomStart, &_spiritFestivalOcarinaPerformerTextureSegmentRomEnd, &_spiritFestivalOcarinaPerformerAssetsIndexSegmentRomStart, &_spiritFestivalOcarinaPerformerAssetsIndexSegmentRomEnd, &_spiritFestivalOcarinaPerformerSpritesheetIndexSegmentRomStart, &_spiritFestivalOcarinaPerformerSpritesheetIndexSegmentRomEnd, 1, 0, &spiritFestivalOcarinaPerformerAnimationScripts);
    setEntityCollisionBuffers(0x32, 0xF, 0xF);
    func_8002FFF4(0x32, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAPPA, &_kappaTextureSegmentRomStart, &_kappaTextureSegmentRomEnd, &_kappaAssetsIndexSegmentRomStart, &_kappaAssetsIndexSegmentRomEnd, &_kappaSpritesheetIndexSegmentRomStart, &_kappaSpritesheetIndexSegmentRomEnd, 1, 0, &kappaAnimationScripts);
    setEntityCollisionBuffers(0x33, 0xF, 0xF);
    func_8002FFF4(0x33, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PRINCESS, &_princessTextureSegmentRomStart, &_princessTextureSegmentRomEnd, &_princessAssetsIndexSegmentRomStart, &_princessAssetsIndexSegmentRomEnd, &_princessSpritesheetIndexSegmentRomStart, &_princessSpritesheetIndexSegmentRomEnd, 1, 0, &princessAnimationScripts);
    setEntityCollisionBuffers(0x34, 0xF, 0xF);
    func_8002FFF4(0x34, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_EMPTY_NPC_SPRITE, &_emptyNPCTextureSegmentRomStart, &_emptyNPCTextureSegmentRomEnd, &_emptyNPCAssetsIndexSegmentRomStart, &_emptyNPCAssetsIndexSegmentRomEnd, &_emptyNPCSpritesheetIndexSegmentRomStart, &_emptyNPCSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x35, 0xF, 0xF);
    func_8002FFF4(0x35, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BUNNY, &_bunnyTextureSegmentRomStart, &_bunnyTextureSegmentRomEnd, &_bunnyAssetsIndexSegmentRomStart, &_bunnyAssetsIndexSegmentRomEnd, &_bunnySpritesheetIndexSegmentRomStart, &_bunnySpritesheetIndexSegmentRomEnd, 1, 0, &bunnyAnimationScripts);
    setEntityCollisionBuffers(0x36, 0xF, 0xF);
    func_8002FFF4(0x36, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MOUSE, &_mouseTextureSegmentRomStart, &_mouseTextureSegmentRomEnd, &_mouseAssetsIndexSegmentRomStart, &_mouseAssetsIndexSegmentRomEnd, &_mouseSpritesheetIndexSegmentRomStart, &_mouseSpritesheetIndexSegmentRomEnd, 1, 0, &mouseAnimationScripts);
    setEntityCollisionBuffers(0x37, 0xF, 0xF);
    func_8002FFF4(0x37, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FOX, &_foxTextureSegmentRomStart, &_foxTextureSegmentRomEnd, &_foxAssetsIndexSegmentRomStart, &_foxAssetsIndexSegmentRomEnd, &_foxSpritesheetIndexSegmentRomStart, &_foxSpritesheetIndexSegmentRomEnd, 1, 0, &foxAnimationScripts);
    setEntityCollisionBuffers(0x38, 0xF, 0xF);
    func_8002FFF4(0x38, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MONKEY, &_monkeyTextureSegmentRomStart, &_monkeyTextureSegmentRomEnd, &_monkeyAssetsIndexSegmentRomStart, &_monkeyAssetsIndexSegmentRomEnd, &_monkeySpritesheetIndexSegmentRomStart, &_monkeySpritesheetIndexSegmentRomEnd, 1, 0, &monkeyAnimationScripts);
    setEntityCollisionBuffers(0x39, 0xF, 0xF);
    func_8002FFF4(0x39, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HAWK, &_hawkTextureSegmentRomStart, &_hawkTextureSegmentRomEnd, &_hawkAssetsIndexSegmentRomStart, &_hawkAssetsIndexSegmentRomEnd, &_hawkSpritesheetIndexSegmentRomStart, &_hawkSpritesheetIndexSegmentRomEnd, 1, 0, &hawkAnimationScripts);
    setEntityCollisionBuffers(0x3A, 0xF, 0xF);
    func_8002FFF4(0x3A, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MOLE, &_moleTextureSegmentRomStart, &_moleTextureSegmentRomEnd, &_moleAssetsIndexSegmentRomStart, &_moleAssetsIndexSegmentRomEnd, &_moleSpritesheetIndexSegmentRomStart, &_moleSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x3B, 0xF, 0xF);
    func_8002FFF4(0x3B, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SQUIRREL, &_squirrelTextureSegmentRomStart, &_squirrelTextureSegmentRomEnd, &_squirrelAssetsIndexSegmentRomStart, &_squirrelAssetsIndexSegmentRomEnd, &_squirrelSpritesheetIndexSegmentRomStart, &_squirrelSpritesheetIndexSegmentRomEnd, 1, 0, &squirrelAnimationScripts);
    setEntityCollisionBuffers(0x3C, 0xF, 0xF);
    func_8002FFF4(0x3C, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOG_RACE, &_dogRaceTextureSegmentRomStart, &_dogRaceTextureSegmentRomEnd, &_dogRaceAssetsIndexSegmentRomStart, &_dogRaceAssetsIndexSegmentRomEnd, &_dogRaceSpritesheetIndexSegmentRomStart, &_dogRaceSpritesheetIndexSegmentRomEnd, 1, 0, &dogRaceAnimationScripts);
    setEntityCollisionBuffers(0x3D, 0xF, 0xF);
    func_8002FFF4(0x3D, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CAT, &_catTextureSegmentRomStart, &_catTextureSegmentRomEnd, &_catAssetsIndexSegmentRomStart, &_catAssetsIndexSegmentRomEnd, &_catSpritesheetIndexSegmentRomStart, &_catSpritesheetIndexSegmentRomEnd, 1, 0, &catAnimationScripts);
    setEntityCollisionBuffers(0x3E, 0xF, 0xF);
    func_8002FFF4(0x3E, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOG, &_dogTextureSegmentRomStart, &_dogTextureSegmentRomEnd, &_dogAssetsIndexSegmentRomStart, &_dogAssetsIndexSegmentRomEnd, &_dogSpritesheetIndexSegmentRomStart, &_dogSpritesheetIndexSegmentRomEnd, 1, 2, &dogAnimationScripts);
    setEntityCollisionBuffers(0x3F, 0xF, 0xF);
    func_8002FFF4(0x3F, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DOG_TITLE, &_dogTitleTextureSegmentRomStart, &_dogTitleTextureSegmentRomEnd, &_dogTitleAssetsIndexSegmentRomStart, &_dogTitleAssetsIndexSegmentRomEnd, &_dogTitleSpritesheetIndexSegmentRomStart, &_dogTitleSpritesheetIndexSegmentRomEnd, 1, 0, &dogTitleAnimationScripts);
    setEntityCollisionBuffers(0x40, 0xF, 0xF);
    func_8002FFF4(0x40, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_PONY, &_horsePonyTextureSegmentRomStart, &_horsePonyTextureSegmentRomEnd, &_horsePonyAssetsIndexSegmentRomStart, &_horsePonyAssetsIndexSegmentRomEnd, &_horsePonySpritesheetIndexSegmentRomStart, &_horsePonySpritesheetIndexSegmentRomEnd, 1, 0, &horsePonyAnimationScripts);
    setEntityCollisionBuffers(0x41, 0xF, 0xF);
    func_8002FFF4(0x41, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_GROWN, &_horseGrownTextureSegmentRomStart, &_horseGrownTextureSegmentRomEnd, &_horseGrownAssetsIndexSegmentRomStart, &_horseGrownAssetsIndexSegmentRomEnd, &_horseGrownSpritesheetIndexSegmentRomStart, &_horseGrownSpritesheetIndexSegmentRomEnd, 1, 0, &horseGrownAnimationScripts);
    setEntityCollisionBuffers(0x42, 0xF, 0xF);
    func_8002FFF4(0x42, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CHICK, &_chickTextureSegmentRomStart, &_chickTextureSegmentRomEnd, &_chickAssetsIndexSegmentRomStart, &_chickAssetsIndexSegmentRomEnd, &_chickSpritesheetIndexSegmentRomStart, &_chickSpritesheetIndexSegmentRomEnd, 1, 2, &chickAnimationScripts);
    setEntityCollisionBuffers(0x43, 0xF, 0xF);
    func_8002FFF4(0x43, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CHICKEN, &_chickenTextureSegmentRomStart, &_chickenTextureSegmentRomEnd, &_chickenAssetsIndexSegmentRomStart, &_chickenAssetsIndexSegmentRomEnd, &_chickenSpritesheetIndexSegmentRomStart, &_chickenSpritesheetIndexSegmentRomEnd, 1, 0, &chickenAnimationScripts);
    setEntityCollisionBuffers(0x44, 0xF, 0xF);
    func_8002FFF4(0x44, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CALF, &_calfTextureSegmentRomStart, &_calfTextureSegmentRomEnd, &_calfAssetsIndexSegmentRomStart, &_calfAssetsIndexSegmentRomEnd, &_calfSpritesheetIndexSegmentRomStart, &_calfSpritesheetIndexSegmentRomEnd, 1, 0, &calfAnimationScripts);
    setEntityCollisionBuffers(0x45, 0xF, 0xF);
    func_8002FFF4(0x45, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_YOUNG_COW, &_youngCowTextureSegmentRomStart, &_youngCowTextureSegmentRomEnd, &_youngCowAssetsIndexSegmentRomStart, &_youngCowAssetsIndexSegmentRomEnd, &_youngCowSpritesheetIndexSegmentRomStart, &_youngCowSpritesheetIndexSegmentRomEnd, 1, 1, &youngCowAnimationScripts);
    setEntityCollisionBuffers(0x46, 0xF, 0xF);
    func_8002FFF4(0x46, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_COW, &_cowTextureSegmentRomStart, &_cowTextureSegmentRomEnd, &_cowAssetsIndexSegmentRomStart, &_cowAssetsIndexSegmentRomEnd, &_cowSpritesheetIndexSegmentRomStart, &_cowSpritesheetIndexSegmentRomEnd, 1, 1, &cowAnimationScripts);
    setEntityCollisionBuffers(0x47, 0xF, 0xF);
    func_8002FFF4(0x47, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_LAMB, &_lambTextureSegmentRomStart, &_lambTextureSegmentRomEnd, &_lambAssetsIndexSegmentRomStart, &_lambAssetsIndexSegmentRomEnd, &_lambSpritesheetIndexSegmentRomStart, &_lambSpritesheetIndexSegmentRomEnd, 1, 0, &lambAnimationScripts);
    setEntityCollisionBuffers(0x48, 0xF, 0xF);
    func_8002FFF4(0x48, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SHEEP, &_sheepTextureSegmentRomStart, &_sheepTextureSegmentRomEnd, &_sheepAssetsIndexSegmentRomStart, &_sheepAssetsIndexSegmentRomEnd, &_sheepSpritesheetIndexSegmentRomStart, &_sheepSpritesheetIndexSegmentRomEnd, 1, 0, &sheepAnimationScripts);
    setEntityCollisionBuffers(0x49, 0xF, 0xF);
    func_8002FFF4(0x49, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_FROG, &_frogTextureSegmentRomStart, &_frogTextureSegmentRomEnd, &_frogAssetsIndexSegmentRomStart, &_frogAssetsIndexSegmentRomEnd, &_frogSpritesheetIndexSegmentRomStart, &_frogSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x4A, 0xF, 0xF);
    func_8002FFF4(0x4A, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CRAB, &_crabTextureSegmentRomStart, &_crabTextureSegmentRomEnd, &_crabAssetsIndexSegmentRomStart, &_crabAssetsIndexSegmentRomEnd, &_crabSpritesheetIndexSegmentRomStart, &_crabSpritesheetIndexSegmentRomEnd, 1, 0, &crabAnimationScripts);
    setEntityCollisionBuffers(0x4B, 0xF, 0xF);
    func_8002FFF4(0x4B, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORNED_BEETLE, &_hornedBeetleTextureSegmentRomStart, &_hornedBeetleTextureSegmentRomEnd, &_hornedBeetleAssetsIndexSegmentRomStart, &_hornedBeetleAssetsIndexSegmentRomEnd, &_hornedBeetleSpritesheetIndexSegmentRomStart, &_hornedBeetleSpritesheetIndexSegmentRomEnd, 1, 0xFF, &hornedBeetleAnimationScripts);
    setEntityCollisionBuffers(0x4C, 0xF, 0xF);
    func_8002FFF4(0x4C, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_STAG_BEETLE, &_stagBeetleTextureSegmentRomStart, &_stagBeetleTextureSegmentRomEnd, &_stagBeetleAssetsIndexSegmentRomStart, &_stagBeetleAssetsIndexSegmentRomEnd, &_stagBeetleSpritesheetIndexSegmentRomStart, &_stagBeetleSpritesheetIndexSegmentRomEnd, 1, 0xFF, &stagBeetleAnimationScripts);
    setEntityCollisionBuffers(0x4D, 0xF, 0xF);
    func_8002FFF4(0x4D, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BELL_RING_CRICKET, &_bellRingCricketTextureSegmentRomStart, &_bellRingCricketTextureSegmentRomEnd, &_bellRingCricketAssetsIndexSegmentRomStart, &_bellRingCricketAssetsIndexSegmentRomEnd, &_bellRingCricketSpritesheetIndexSegmentRomStart, &_bellRingCricketSpritesheetIndexSegmentRomEnd, 1, 0xFF, &bellRingCricketAnimationScripts);
    setEntityCollisionBuffers(0x4E, 0xF, 0xF);
    func_8002FFF4(0x4E, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_CICADA, &_cicadaTextureSegmentRomStart, &_cicadaTextureSegmentRomEnd, &_cicadaAssetsIndexSegmentRomStart, &_cicadaAssetsIndexSegmentRomEnd, &_cicadaSpritesheetIndexSegmentRomStart, &_cicadaSpritesheetIndexSegmentRomEnd, 1, 0xFF, &cicadaAnimationScripts);
    setEntityCollisionBuffers(0x4F, 0xF, 0xF);
    func_8002FFF4(0x4F, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_BIRD, &_birdTextureSegmentRomStart, &_birdTextureSegmentRomEnd, &_birdAssetsIndexSegmentRomStart, &_birdAssetsIndexSegmentRomEnd, &_birdSpritesheetIndexSegmentRomStart, &_birdSpritesheetIndexSegmentRomEnd, 1, 2, &birdAnimationScripts);
    setEntityCollisionBuffers(0x50, 0xF, 0xF);
    func_8002FFF4(0x50, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_LADYBUG, &_ladybugTextureSegmentRomStart, &_ladybugTextureSegmentRomEnd, &_ladybugAssetsIndexSegmentRomStart, &_ladybugAssetsIndexSegmentRomEnd, &_ladybugSpritesheetIndexSegmentRomStart, &_ladybugSpritesheetIndexSegmentRomEnd, 1, 0xFF, &ladybugAnimationScripts);
    setEntityCollisionBuffers(0x51, 0xF, 0xF);
    func_8002FFF4(0x51, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DRAGONFLY, &_dragonflyTextureSegmentRomStart, &_dragonflyTextureSegmentRomEnd, &_dragonflyAssetsIndexSegmentRomStart, &_dragonflyAssetsIndexSegmentRomEnd, &_dragonflySpritesheetIndexSegmentRomStart, &_dragonflySpritesheetIndexSegmentRomEnd, 1, 2, &dragonflyAnimationScripts);
    setEntityCollisionBuffers(0x52, 0xF, 0xF);
    func_8002FFF4(0x52, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_GRASSHOPPER, &_grasshopperTextureSegmentRomStart, &_grasshopperTextureSegmentRomEnd, &_grasshopperAssetsIndexSegmentRomStart, &_grasshopperAssetsIndexSegmentRomEnd, &_grasshopperSpritesheetIndexSegmentRomStart, &_grasshopperSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x53, 0xF, 0xF);
    func_8002FFF4(0x53, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_SNAKE, &_snakeTextureSegmentRomStart, &_snakeTextureSegmentRomEnd, &_snakeAssetsIndexSegmentRomStart, &_snakeAssetsIndexSegmentRomEnd, &_snakeSpritesheetIndexSegmentRomStart, &_snakeSpritesheetIndexSegmentRomEnd, 1, 2, &snakeAnimationScripts);
    setEntityCollisionBuffers(0x54, 0xF, 0xF);
    func_8002FFF4(0x54, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_PURPLE_BUTTERFLY, &_purpleButterflyTextureSegmentRomStart, &_purpleButterflyTextureSegmentRomEnd, &_purpleButterflyAssetsIndexSegmentRomStart, &_purpleButterflyAssetsIndexSegmentRomEnd, &_purpleButterflySpritesheetIndexSegmentRomStart, &_purpleButterflySpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x55, 0xF, 0xF);
    func_8002FFF4(0x55, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_DAD, &_dadTextureSegmentRomStart, &_dadTextureSegmentRomEnd, &_dadAssetsIndexSegmentRomStart, &_dadAssetsIndexSegmentRomEnd, &_dadSpritesheetIndexSegmentRomStart, &_dadSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x56, 0xF, 0xF);
    func_8002FFF4(0x56, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_COW_INDOORS, &_cowIndoorsTextureSegmentRomStart, &_cowIndoorsTextureSegmentRomEnd, &_cowIndoorsAssetsIndexSegmentRomStart, &_cowIndoorsAssetsIndexSegmentRomEnd, &_cowIndoorsSpritesheetIndexSegmentRomStart, &_cowIndoorsSpritesheetIndexSegmentRomEnd, 1, 1, &cowIndoorsAnimationScripts);
    setEntityCollisionBuffers(0x57, 0xF, 0xF);
    func_8002FFF4(0x57, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_UNBRIDLED, &_horseUnbridledTextureSegmentRomStart, &_horseUnbridledTextureSegmentRomEnd, &_horseUnbridledAssetsIndexSegmentRomStart, &_horseUnbridledAssetsIndexSegmentRomEnd, &_horseUnbridledSpritesheetIndexSegmentRomStart, &_horseUnbridledSpritesheetIndexSegmentRomEnd, 1, 0, &horseUnbridledAnimationScripts);
    setEntityCollisionBuffers(0x58, 0xF, 0xF);
    func_8002FFF4(0x58, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_HORSE_RACER, &_horseRacerTextureSegmentRomStart, &_horseRacerTextureSegmentRomEnd, &_horseRacerAssetsIndexSegmentRomStart, &_horseRacerAssetsIndexSegmentRomEnd, &_horseRacerSpritesheetIndexSegmentRomStart, &_horseRacerSpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x59, 0xF, 0xF);
    func_8002FFF4(0x59, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_EMPTY_ENTITY, &_emptyEntityTextureSegmentRomStart, &_emptyEntityTextureSegmentRomEnd, &_emptyEntityAssetsIndexSegmentRomStart, &_emptyEntityAssetsIndexSegmentRomEnd, &_emptyEntitySpritesheetIndexSegmentRomStart, &_emptyEntitySpritesheetIndexSegmentRomEnd, 1, 0, &playerAnimationScripts);
    setEntityCollisionBuffers(0x5A, 0xF, 0xF);
    func_8002FFF4(0x5A, 0xC, 0xC);
    
    initializeEntityAsset(0x5B, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItemsAnimationScripts1);
    setEntityCollisionBuffers(0x5B, 0xF, 0xF);
    func_8002FFF4(0x5B, 0xC, 0xC);
    
    initializeEntityAsset(0x5C, &_sowingFestivalTextureSegmentRomStart, &_sowingFestivalTextureSegmentRomEnd, &_sowingFestivalAssetsIndexSegmentRomStart, &_sowingFestivalAssetsIndexSegmentRomEnd, &_sowingFestivalSpritesheetIndexSegmentRomStart, &_sowingFestivalSpritesheetIndexSegmentRomEnd, 1, 0, &homeItemsAnimationScripts);
    setEntityCollisionBuffers(0x5C, 0xF, 0xF);
    func_8002FFF4(0x5C, 0xC, 0xC);
    
    initializeEntityAsset(0x5D, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 2, &shopItemsAnimationScripts);
    setEntityCollisionBuffers(0x5D, 0xF, 0xF);
    func_8002FFF4(0x5D, 0xC, 0xC);
    
    initializeEntityAsset(0x5E, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItemsAnimationScripts2);
    setEntityCollisionBuffers(0x5E, 0xF, 0xF);
    func_8002FFF4(0x5E, 0xC, 0xC);
    
    initializeEntityAsset(0x5F, &_toolsTextureSegmentRomStart, &_toolsTextureSegmentRomEnd, &_toolsAssetsIndexSegmentRomStart, &_toolsAssetsIndexSegmentRomEnd, &_toolsSpritesheetIndexSegmentRomStart, &_toolsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &toolsAnimationScripts);
    setEntityCollisionBuffers(0x5F, 0xF, 0xF);
    func_8002FFF4(0x5F, 0xC, 0xC);
    
    initializeEntityAsset(0x60, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 1, 0xFF, &holdableItemsAnimationScripts3);
    setEntityCollisionBuffers(0x60, 0xF, 0xF);
    func_8002FFF4(0x60, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_MARIA_HARRIS_BABY, &_mariaHarrisBabyTextureSegmentRomStart, &_mariaHarrisBabyTextureSegmentRomEnd, &_mariaHarrisBabyAssetsIndexSegmentRomStart, &_mariaHarrisBabyAssetsIndexSegmentRomEnd, &_mariaHarrisBabySpritesheetIndexSegmentRomStart, &_mariaHarrisBabySpritesheetIndexSegmentRomEnd, 1, 0, &mariaHarrisBabyAnimationScripts);
    setEntityCollisionBuffers(0x61, 0xF, 0xF);
    func_8002FFF4(0x61, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_POPURI_GRAY_BABY, &_popuriGrayBabyTextureSegmentRomStart, &_popuriGrayBabyTextureSegmentRomEnd, &_popuriGrayBabyAssetsIndexSegmentRomStart, &_popuriGrayBabyAssetsIndexSegmentRomEnd, &_popuriGrayBabySpritesheetIndexSegmentRomStart, &_popuriGrayBabySpritesheetIndexSegmentRomEnd, 1, 0, &popuriGrayBabyAnimationScripts);
    setEntityCollisionBuffers(0x62, 0xF, 0xF);
    func_8002FFF4(0x62, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ELLI_JEFF_BABY, &_elliJeffBabyTextureSegmentRomStart, &_elliJeffBabyTextureSegmentRomEnd, &_elliJeffBabyAssetsIndexSegmentRomStart, &_elliJeffBabyAssetsIndexSegmentRomEnd, &_elliJeffBabySpritesheetIndexSegmentRomStart, &_elliJeffBabySpritesheetIndexSegmentRomEnd, 1, 0, &elliJeffBabyAnimationScripts);
    setEntityCollisionBuffers(0x63, 0xF, 0xF);
    func_8002FFF4(0x63, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_ANN_CLIFF_BABY, &_annCliffBabyTextureSegmentRomStart, &_annCliffBabyTextureSegmentRomEnd, &_annCliffBabyAssetsIndexSegmentRomStart, &_annCliffBabyAssetsIndexSegmentRomEnd, &_annCliffBabySpritesheetIndexSegmentRomStart, &_annCliffBabySpritesheetIndexSegmentRomEnd, 1, 0, &annCliffBabyAnimationScripts);
    setEntityCollisionBuffers(0x64, 0xF, 0xF);
    func_8002FFF4(0x64, 0xC, 0xC);
    
    initializeEntityAsset(ENTITY_KAREN_KAI_BABY, &_karenKaiBabyTextureSegmentRomStart, &_karenKaiBabyTextureSegmentRomEnd, &_karenKaiBabyAssetsIndexSegmentRomStart, &_karenKaiBabyAssetsIndexSegmentRomEnd, &_karenKaiBabySpritesheetIndexSegmentRomStart, &_karenKaiBabySpritesheetIndexSegmentRomEnd, 1, 0, &karenKaiBabyAnimationScripts);
    setEntityCollisionBuffers(0x65, 0xF, 0xF);
    func_8002FFF4(0x65, 0xC, 0xC);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/initialize", loadMapAddresses);

void loadMapAddresses(void) {
    
    setMapModelAddresses(RANCH_SPRING, &_ranchSpringMapSegmentRomStart, &_ranchSpringMapSegmentRomEnd);
    setMapModelAddresses(RANCH_SUMMER, &_ranchSummerMapSegmentRomStart, &_ranchSummerMapSegmentRomEnd);
    setMapModelAddresses(RANCH_AUTUMN, &_ranchFallMapSegmentRomStart, &_ranchFallMapSegmentRomEnd);
    setMapModelAddresses(RANCH_WINTER, &_ranchWinterMapSegmentRomStart, &_ranchWinterMapSegmentRomEnd);
 
    setMapModelAddresses(ANN_ROOM, &_annsRoomMapSegmentRomStart, &_annsRoomMapSegmentRomEnd);

    setMapModelAddresses(RANCH_BARN, &_ranchBarnMapSegmentRomStart, &_ranchBarnMapSegmentRomEnd);
    setMapModelAddresses(RANCH_STORE, &_ranchStoreMapSegmentRomStart, &_ranchStoreMapSegmentRomEnd);
    setMapModelAddresses(RANCH_HOUSE, &_ranchHouseMapSegmentRomStart, &_ranchHouseMapSegmentRomEnd);

    setMapModelAddresses(EMPTY_MAP_1, &_emptyMap1SegmentRomStart, &_emptyMap1SegmentRomEnd);
    
    setMapModelAddresses(BEACH_SPRING, &_beachSpringMapSegmentRomStart, &_beachSpringMapSegmentRomEnd);
    setMapModelAddresses(BEACH_SUMMER, &_beachSummerMapSegmentRomStart, &_beachSummerMapSegmentRomEnd);
    setMapModelAddresses(BEACH_AUTUMN, &_beachFallMapSegmentRomStart, &_beachFallMapSegmentRomEnd);
    setMapModelAddresses(BEACH_WINTER, &_beachWinterMapSegmentRomStart, &_beachWinterMapSegmentRomEnd);

    setMapModelAddresses(RACE_TRACK_SPRING, &_raceTrackSpringMapSegmentRomStart, &_raceTrackSpringMapSegmentRomEnd);
    // no race track summer: 0xE
    // empty 4 words in rom
    setMapModelAddresses(RACE_TRACK_AUTUMN, &_raceTrackFallMapSegmentRomStart, &_raceTrackFallMapSegmentRomEnd);
    setMapModelAddresses(RACE_TRACK_WINTER, &_raceTrackWinterMapSegmentRomStart, &_raceTrackWinterMapSegmentRomEnd);

    setMapModelAddresses(MOUNTAIN_1_SPRING, &_mountain1SpringMapSegmentRomStart, &_mountain1SpringMapSegmentRomEnd);
    setMapModelAddresses(MOUNTAIN_1_SUMMER, &_mountain1SummerMapSegmentRomStart, &_mountain1SummerMapSegmentRomEnd);
    setMapModelAddresses(MOUNTAIN_1_AUTUMN, &_mountain1FallMapSegmentRomStart, &_mountain1FallMapSegmentRomEnd);
    setMapModelAddresses(MOUNTAIN_1_WINTER, &_mountain1WinterMapSegmentRomStart, &_mountain1WinterMapSegmentRomEnd);

    setMapModelAddresses(MOUNTAIN_2_SPRING, &_mountain2SpringMapSegmentRomStart, &_mountain2SpringMapSegmentRomEnd);
    setMapModelAddresses(MOUNTAIN_2_SUMMER, &_mountain2SummerMapSegmentRomStart, &_mountain2SummerMapSegmentRomEnd);
    setMapModelAddresses(MOUNTAIN_2_AUTUMN, &_mountain2FallMapSegmentRomStart, &_mountain2FallMapSegmentRomEnd);
    setMapModelAddresses(MOUNTAIN_2_WINTER, &_mountain2WinterMapSegmentRomStart, &_mountain2WinterMapSegmentRomEnd);

    setMapModelAddresses(TOP_OF_MOUNTAIN_1_SPRING, &_topOfMountain1SpringMapSegmentRomStart, &_topOfMountain1SpringMapSegmentRomEnd);
    setMapModelAddresses(TOP_OF_MOUNTAIN_1_SUMMER, &_topOfMountain1SummerMapSegmentRomStart, &_topOfMountain1SummerMapSegmentRomEnd);
    setMapModelAddresses(TOP_OF_MOUNTAIN_1_AUTUMN, &_topOfMountain1FallMapSegmentRomStart, &_topOfMountain1FallMapSegmentRomEnd);
    setMapModelAddresses(TOP_OF_MOUNTAIN_1_WINTER, &_topOfMountain1WinterMapSegmentRomStart, &_topOfMountain1WinterMapSegmentRomEnd);

    setMapModelAddresses(MOON_MOUNTAIN_SPRING, &_moonMountainSpringMapSegmentRomStart, &_moonMountainSpringMapSegmentRomEnd);
    setMapModelAddresses(MOON_MOUNTAIN_SUMMER, &_moonMountainSummerMapSegmentRomStart, &_moonMountainSummerMapSegmentRomEnd);
    setMapModelAddresses(MOON_MOUNTAIN_AUTUMN, &_moonMountainFallMapSegmentRomStart, &_moonMountainFallMapSegmentRomEnd);
    setMapModelAddresses(MOON_MOUNTAIN_WINTER, &_moonMountainWinterMapSegmentRomStart, &_moonMountainWinterMapSegmentRomEnd);

    setMapModelAddresses(CARPENTER_HUT, &_carpentersHutMapSegmentRomStart, &_carpentersHutMapSegmentRomEnd);
    
    setMapModelAddresses(DUMPLING_HOUSE, &_dumplingHouseMapSegmentRomStart, &_dumplingHouseMapSegmentRomEnd);

    setMapModelAddresses(POND_SPRING, &_springSpringMapSegmentRomStart, &_springSpringMapSegmentRomEnd);
    setMapModelAddresses(POND_SUMMER, &_springSummerMapSegmentRomStart, &_springSummerMapSegmentRomEnd);
    setMapModelAddresses(POND_AUTUMN, &_springFallMapSegmentRomStart, &_springFallMapSegmentRomEnd);
    setMapModelAddresses(POND_WINTER, &_springWinterMapSegmentRomStart, &_springWinterMapSegmentRomEnd);

    setMapModelAddresses(ELLI_ROOM, &_ellisRoomMapSegmentRomStart, &_ellisRoomMapSegmentRomEnd);

    setMapModelAddresses(BAKERY, &_bakeryMapSegmentRomStart, &_bakeryMapSegmentRomEnd);

    setMapModelAddresses(VILLAGE_1_SPRING, &_village1SpringMapSegmentRomStart, &_village1SpringMapSegmentRomEnd);
    setMapModelAddresses(VILLAGE_1_SUMMER, &_village1SummerMapSegmentRomStart, &_village1SummerMapSegmentRomEnd);
    setMapModelAddresses(VILLAGE_1_AUTUMN, &_village1FallMapSegmentRomStart, &_village1FallMapSegmentRomEnd);
    setMapModelAddresses(VILLAGE_1_WINTER, &_village1WinterMapSegmentRomStart, &_village1WinterMapSegmentRomEnd);

    setMapModelAddresses(VILLAGE_2_SPRING, &_village2SpringMapSegmentRomStart, &_village2SpringMapSegmentRomEnd);
    setMapModelAddresses(VILLAGE_2_SUMMER, &_village2SummerMapSegmentRomStart, &_village2SummerMapSegmentRomEnd);
    setMapModelAddresses(VILLAGE_2_AUTUMN, &_village2FallMapSegmentRomStart, &_village2FallMapSegmentRomEnd);
    setMapModelAddresses(VILLAGE_2_WINTER, &_village2WinterMapSegmentRomStart, &_village2WinterMapSegmentRomEnd);

    setMapModelAddresses(POPURI_ROOM, &_popurisRoomMapSegmentRomStart, &_popurisRoomMapSegmentRomEnd);

    setMapModelAddresses(FLOWER_SHOP, &_flowerShopMapSegmentRomStart, &_flowerShopMapSegmentRomEnd);

    setMapModelAddresses(CHURCH, &_churchMapSegmentRomStart, &_churchMapSegmentRomEnd);

    setMapModelAddresses(SOUVENIR_SHOP, &_souvenirShopMapSegmentRomStart, &_souvenirShopMapSegmentRomEnd);

    setMapModelAddresses(SQUARE_SPRING, &_squareSpringMapSegmentRomStart, &_squareSpringMapSegmentRomEnd);
    setMapModelAddresses(SQUARE_SUMMER, &_squareSummerMapSegmentRomStart, &_squareSummerMapSegmentRomEnd);
    setMapModelAddresses(SQUARE_AUTUMN, &_squareFallMapSegmentRomStart, &_squareFallMapSegmentRomEnd);
    setMapModelAddresses(SQUARE_WINTER, &_squareWinterMapSegmentRomStart, &_squareWinterMapSegmentRomEnd);

    setMapModelAddresses(RICK_STORE, &_ricksShopMapSegmentRomStart, &_ricksShopMapSegmentRomEnd);

    setMapModelAddresses(MIDWIFE_HOUSE, &_midwifesHouseMapSegmentRomStart, &_midwifesHouseMapSegmentRomEnd);
    
    setMapModelAddresses(TAVERN, &_tavernMapSegmentRomStart, &_tavernMapSegmentRomEnd);
    
    setMapModelAddresses(LIBRARY, &_libraryMapSegmentRomStart, &_libraryMapSegmentRomEnd);

    setMapModelAddresses(MARIA_ROOM, &_mariasRoomMapSegmentRomStart, &_mariasRoomMapSegmentRomEnd);
    
    setMapModelAddresses(MAYOR_HOUSE, &_mayorsHouseMapSegmentRomStart, &_mayorsHouseMapSegmentRomEnd);

    setMapModelAddresses(POTION_SHOP_BEDROOM, &_potionShopBackroomMapSegmentRomStart, &_potionShopBackroomMapSegmentRomEnd);
    setMapModelAddresses(POTION_SHOP, &_potionShopMapSegmentRomStart, &_potionShopMapSegmentRomEnd);

    // empty, referenced in func_80074C50 
    setMapModelAddresses(EMPTY_MAP_2, &_emptyMap2SegmentRomStart, &_emptyMap2SegmentRomEnd);
    
    setMapModelAddresses(HARVEST_SPRITE_CAVE, &_spriteCaveMapSegmentRomStart, &_spriteCaveMapSegmentRomEnd);
    setMapModelAddresses(CAVE, &_caveMapSegmentRomStart, &_caveMapSegmentRomEnd);    
    setMapModelAddresses(MINE, &_emptyMineMapSegmentRomStart, &_emptyMineMapSegmentRomEnd);
    setMapModelAddresses(MINE_2, &_mineMapSegmentRomStart, &_mineMapSegmentRomEnd);

    setMapModelAddresses(KAREN_ROOM, &_karensRoomMapSegmentRomStart, &_karensRoomMapSegmentRomEnd);

    setMapModelAddresses(VINEYARD_SPRING, &_vineyardSpringMapSegmentRomStart, &_vineyardSpringMapSegmentRomEnd);
    setMapModelAddresses(VINEYARD_SUMMER, &_vineyardSummerMapSegmentRomStart, &_vineyardSummerMapSegmentRomEnd);
    setMapModelAddresses(VINEYARD_AUTUMN, &_vineyardFallMapSegmentRomStart, &_vineyardFallMapSegmentRomEnd);
    setMapModelAddresses(VINEYARD_WINTER, &_vineyardWinterMapSegmentRomStart, &_vineyardWinterMapSegmentRomEnd);

    setMapModelAddresses(VINEYARD_HOUSE, &_vineyardHouseMapSegmentRomStart, &_vineyardHouseMapSegmentRomEnd);

    setMapModelAddresses(VINEYARD_CELLAR, &_vineyardCellar1MapSegmentRomStart, &_vineyardCellar1MapSegmentRomEnd);
    setMapModelAddresses(VINEYARD_CELLAR_BASEMENT, &_vineyardCellar2MapSegmentRomStart, &_vineyardCellar2MapSegmentRomEnd);

    setMapModelAddresses(ROAD_SPRING, &_roadSpringMapSegmentRomStart, &_roadSpringMapSegmentRomEnd);
    setMapModelAddresses(ROAD_SUMMER, &_roadSummerMapSegmentRomStart, &_roadSummerMapSegmentRomEnd);
    setMapModelAddresses(ROAD_AUTUMN, &_roadFallMapSegmentRomStart, &_roadFallMapSegmentRomEnd);
    setMapModelAddresses(ROAD_WINTER, &_roadWinterMapSegmentRomStart, &_roadWinterMapSegmentRomEnd);

    setMapModelAddresses(FARM_SPRING, &_farmSpringMapSegmentRomStart, &_farmSpringMapSegmentRomEnd);
    setMapModelAddresses(FARM_SUMMER, &_farmSummerMapSegmentRomStart, &_farmSummerMapSegmentRomEnd);
    setMapModelAddresses(FARM_AUTUMN, &_farmFallMapSegmentRomStart, &_farmFallMapSegmentRomEnd);
    setMapModelAddresses(FARM_WINTER, &_farmWinterMapSegmentRomStart, &_farmWinterMapSegmentRomEnd);

    setMapModelAddresses(GREENHOUSE, &_greenhouseMapSegmentRomStart, &_greenhouseMapSegmentRomEnd);
    
    setMapModelAddresses(HOUSE, &_houseMapSegmentRomStart, &_houseMapSegmentRomEnd);

    setMapModelAddresses(BARN, &_barnMapSegmentRomStart, &_barnMapSegmentRomEnd);
    setMapModelAddresses(COOP, &_coopMapSegmentRomStart, &_coopMapSegmentRomEnd);
    
    setMapModelAddresses(KITCHEN, &_kitchenMapSegmentRomStart, &_kitchenMapSegmentRomEnd);
    setMapModelAddresses(BATHROOM, &_bathroomMapSegmentRomStart, &_bathroomMapSegmentRomEnd);
    
    setupMapModel(MAIN_MAP_INDEX, 0, MODEL_DATA_BANK);

}

INCLUDE_ASM("asm/nonmatchings/game/initialize", func_80053088);

// FIXME: matches by itself, but introduces rodata issue when included
/*
void func_80053088(void) {

    u32 ptr = &_fontTextureSegmentRomStart;
    u32 ptr2 = &_fontTextureSegmentRomEnd;
    u32 ptr3 = &_fontPalette1SegmentRomStart;
    u32 ptr4 = &_fontPalette1SegmentRomEnd;
    u32 ptr5 = &_fontPalette2SegmentRomStart;
    u32 ptr6 = &_fontPalette2SegmentRomEnd;

    func_800535DC();
    initializeNameStrings();
    func_8003FAE8(&D_8011BD18);

    nuPiReadRom(ptr, (void*)0x802FF000, ptr2 - ptr);
    nuPiReadRom(ptr3, (void*)0x8030A000, ptr4 - ptr3);
    nuPiReadRom(ptr5, (void*)0x8030A400, ptr6 - ptr5);

    func_8003F80C(0, 0x76, &_dialogueWindowTextureSegmentRomStart, &_dialogueWindowTextureSegmentRomEnd, &_dialogueWindowAssetsIndexSegmentRomStart, &_dialogueWindowAssetsIndexSegmentRomEnd, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 0, 1, -24.0f, 0, 0);
    func_8003F80C(1, 0x76, &_dialogueWindowTextureSegmentRomStart, &_dialogueWindowTextureSegmentRomEnd, &_dialogueWindowAssetsIndexSegmentRomStart, &_dialogueWindowAssetsIndexSegmentRomEnd, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 0, 0, 0, 0, 0);
    func_8003F80C(2, 0x77, &_dialogueWindowTextureSegmentRomStart, &_dialogueWindowTextureSegmentRomEnd, &_dialogueWindowAssetsIndexSegmentRomStart, &_dialogueWindowAssetsIndexSegmentRomEnd, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 1, 0, 0, 0, 0);
  
    func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_START, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_END, (void* )DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, 4, 0xFE, 106.0f, -15.0f, 0.0f);
    func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_START, (void* )DIALOGUE_ICONS_TEXTURE_VADDR_END, (void* )DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, 0xD, 0xFE, 106.0f, -15.0f, 0.0f);
    func_8003FA1C(0, 0x75, &_characterDialogueIconsTextureSegmentRomStart, &_characterDialogueIconsTextureSegmentRomEnd, &_characterDialogueIconsAssetsIndexSegmentRomStart, &_characterDialogueIconsAssetsIndexSegmentRomEnd, &_characterDialogueIconsSpritesheetIndexSegmentRomStart, &_characterDialogueIconsSpritesheetIndexSegmentRomEnd, (void*)0x8023D300, (void*)0x8023DB00, (void*)0x8023E300, (void*)0x8023EF00, (void*)0x8023FF00, (void*)0x80240000, -139.0f, 1.0f, 0);
 
    func_8003DBE8(0, 0x8030B000);
    func_8003F54C(0, 24.0f, -64.0f, 352.0f);
    func_8003F5D0(0, 0x10, 3);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, 0x802FF000, 0x8030A000);
    func_8003F360(0, ~(1 | 2), 0);
    setDialogueBoxSpriteIndices(0, 0, 0, 0);
    func_8003F4E0(0, 0x57, 8, 1);
    func_8003FAF8(0, 0xC000);
    func_8003FB4C(0, 1);
    func_8003DBE8(1, 0x8030B400);
    func_8003F54C(1, 64.0f, 32.0f, 352.0f);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, 0x802FF000, 0x8030A400);
    func_8003F5D0(1, 0xB, 4);
    func_8003F360(1, -4, 3);
    func_8003FAF8(1, 0xC000);
    func_8003FB4C(1, 2);
    setDialogueBoxSpriteIndices(1, 2, 0xFF, 0xFF);
    func_8003F4E0(1, 0xFF, 0xFF, 0xFF);
    
}
*/


INCLUDE_ASM("asm/nonmatchings/game/initialize", func_800535DC);

// dialogues
// TODO: need to add linker symbols
/*
void func_800535DC(void) {
    setDialogueInfo(0, &_D_E13920, &_D_E13990, &_D_E13800_2, 0x8030A800);
    setDialogueInfo(1, &_D_E13C30, &_D_E13C60, &_D_E13990_2, 0x8030A800);
    setDialogueInfo(2, &_D_E167C0, &_D_E16A90, &_D_E13C60_2, 0x8030A800);
    setDialogueInfo(3, &_D_E19F60, &_D_E19FF0, &_D_E16A90_2, 0x8030A800);
    setDialogueInfo(4, &_D_E1B3A0, &_D_E1B4E0, &_D_E19FF0_2, 0x8030A800);
    setDialogueInfo(5, &_D_E1C8C0, &_D_E1C9C0, &_D_E1B4E0_2, 0x8030A800);
    setDialogueInfo(6, &_D_E21150, &_D_E21700, &_D_E1C9C0_2, 0x8030A800);
    setDialogueInfo(7, &_D_E218A0, &_D_E218D0, &_D_E21700_2, 0x8030A800);
    setDialogueInfo(8, &_D_E24E20, &_D_E24F60, &_D_E218D0_2, 0x8030A800);
    setDialogueInfo(9, &_D_E24F70, &_D_E24F80, &_D_E24F60_2, 0x8030A800);
    setDialogueInfo(0xA, &_D_E25170, &_D_E251D0, &_D_E24F80_2, 0x8030A800);
    setDialogueInfo(0x15, &_D_E3C700, &_D_E3C9A0, &_D_E39610_2, 0x8030A800);
    setDialogueInfo(0x14, &_D_E39360, &_D_E39610, &_D_E36150_2, 0x8030A800);
    setDialogueInfo(0xB, &_D_E28460, &_D_E28720, &_D_E251D0_2, 0x8030A800);
    setDialogueInfo(0x17, &_D_E40B90, &_D_E40E50, &_D_E3D6D0_2, 0x8030A800);
    setDialogueInfo(0xD, &_D_E2D080, &_D_E2D330, &_D_E2A380_2, 0x8030A800);
    setDialogueInfo(0x44, &_D_E84910, &_D_E84990, &_D_E84760_2, 0x8030A800);
    setDialogueInfo(0x13, &_D_E36000, &_D_E36150, &_D_E34790_2, 0x8030A800);
    setDialogueInfo(0x19, &_D_E42D50, &_D_E42E60, &_D_E42100_2, 0x8030A800);
    setDialogueInfo(0x11, &_D_E33020, &_D_E33190, &_D_E31010_2, 0x8030A800);
    setDialogueInfo(0x10, &_D_E30ED0, &_D_E31010, &_D_E2F730_2, 0x8030A800);
    setDialogueInfo(0xC, &_D_E2A230, &_D_E2A380, &_D_E28720_2, 0x8030A800);
    setDialogueInfo(0x21, &_D_E4B6E0, &_D_E4B7D0, &_D_E4A2D0_2, 0x8030A800);
    setDialogueInfo(0x27, &_D_E51A00, &_D_E51AE0, &_D_E50600_2, 0x8030A800);
    setDialogueInfo(0x1B, &_D_E45130, &_D_E45200, &_D_E43F60_2, 0x8030A800);
    setDialogueInfo(0x1A, &_D_E43E80, &_D_E43F60, &_D_E42E60_2, 0x8030A800);
    setDialogueInfo(0x28, &_D_E52740, &_D_E527E0, &_D_E51AE0_2, 0x8030A800);
    setDialogueInfo(0x18, &_D_E42010, &_D_E42100, &_D_E40E50_2, 0x8030A800);
    setDialogueInfo(0xE, &_D_E2E500, &_D_E2E600, &_D_E2D330_2, 0x8030A800);
    setDialogueInfo(0xF, &_D_E2F650, &_D_E2F730, &_D_E2E600_2, 0x8030A800);
    setDialogueInfo(0x26, &_D_E50520, &_D_E50600, &_D_E4F3F0_2, 0x8030A800);
    setDialogueInfo(0x12, &_D_E34650, &_D_E34790, &_D_E33190_2, 0x8030A800);
    setDialogueInfo(0x29, &_D_E53450, &_D_E53540, &_D_E527E0_2, 0x8030A800);
    setDialogueInfo(0x2A, &_D_E54C60, &_D_E54D60, &_D_E53540_2, 0x8030A800);
    setDialogueInfo(0x16, &_D_E3D600, &_D_E3D6D0, &_D_E3C9A0_2, 0x8030A800);
    setDialogueInfo(0x23, &_D_E4E230, &_D_E4E320, &_D_E4C9B0_2, 0x8030A800);
    setDialogueInfo(0x2B, &_D_E55AE0, &_D_E55BA0, &_D_E54D60_2, 0x8030A800);
    setDialogueInfo(0x1D, &_D_E47140, &_D_E471E0, &_D_E46280_2, 0x8030A800);
    setDialogueInfo(0x2C, &_D_E569C0, &_D_E56A80, &_D_E55BA0_2, 0x8030A800);
    setDialogueInfo(0x1C, &_D_E461C0, &_D_E46280, &_D_E45200_2, 0x8030A800);
    setDialogueInfo(0x22, &_D_E4C8D0, &_D_E4C9B0, &_D_E4B7D0_2, 0x8030A800);
    setDialogueInfo(0x1F, &_D_E49480, &_D_E49540, &_D_E48720_2, 0x8030A800);
    setDialogueInfo(0x1F, &_D_E49480, &_D_E49540, &_D_E48720_2, 0x8030A800);
    setDialogueInfo(0x20, &_D_E4A230, &_D_E4A2D0, &_D_E49540_2, 0x8030A800);
    setDialogueInfo(0x1E, &_D_E485D0, &_D_E48720, &_D_E471E0_2, 0x8030A800);
    setDialogueInfo(0x1E, &_D_E485D0, &_D_E48720, &_D_E471E0_2, 0x8030A800);
    setDialogueInfo(0x1E, &_D_E485D0, &_D_E48720, &_D_E471E0_2, 0x8030A800);
    setDialogueInfo(0x24, &_D_E4EC70, &_D_E4ED10, &_D_E4E320_2, 0x8030A800);
    setDialogueInfo(0x25, &_D_E4F370, &_D_E4F3F0, &_D_E4ED10_2, 0x8030A800);
    setDialogueInfo(0x45, &_D_E84F50, &_D_E84FD0, &_D_E84990_2, 0x8030A800);
    setDialogueInfo(0x45, &_D_E84F50, &_D_E84FD0, &_D_E84990_2, 0x8030A800);
    setDialogueInfo(0x46, &_D_E85730, &_D_E857A0, &_D_E84FD0_2, 0x8030A800);
    setDialogueInfo(0x47, &_D_E84910, &_D_E84990, &_D_E84760_2, 0x8030A800);
    setDialogueInfo(0x47, &_D_E84910, &_D_E84990, &_D_E84760_2, 0x8030A800);
    setDialogueInfo(0x47, &_D_E84910, &_D_E84990, &_D_E84760_2, 0x8030A800);
    setDialogueInfo(0x47, &_D_E84910, &_D_E84990, &_D_E84760_2, 0x8030A800);
    setDialogueInfo(0x47, &_D_E84910, &_D_E84990, &_D_E84760_2, 0x8030A800);
    setDialogueInfo(0x46, &_D_E85730, &_D_E857A0, &_D_E84FD0_2, 0x8030A800);
    setDialogueInfo(0x3F, &_D_E7C7B0, &_D_E7D1E0, &_D_E73110_2, 0x8030A800);
    setDialogueInfo(0x40, &_D_E81660, &_D_E81F50, &_D_E7D1E0_2, 0x8030A800);
    setDialogueInfo(0x2D, &_D_E58010, &_D_E58160, &_D_E56A80_2, 0x8030A800);
    setDialogueInfo(0x35, &_D_E64680, &_D_E64AF0, &_D_E60170_2, 0x8030A800);
    setDialogueInfo(0x3B, &_D_E6F1E0, &_D_E6F5C0, &_D_E6AFE0_2, 0x8030A800);
    setDialogueInfo(0x3C, &_D_E714D0, &_D_E71690, &_D_E6F5C0_2, 0x8030A800);
    setDialogueInfo(0x3D, &_D_E72680, &_D_E72780, &_D_E71690_2, 0x8030A800);
    setDialogueInfo(0x3E, &_D_E73070, &_D_E73110, &_D_E72780_2, 0x8030A800);
    setDialogueInfo(0x42, &_D_E83880, &_D_E83960, &_D_E82C40_2, 0x8030A800);
    setDialogueInfo(0x36, &_D_E66260, &_D_E663F0, &_D_E64AF0_2, 0x8030A800);
    setDialogueInfo(0x3A, &_D_E6ADC0, &_D_E6AFE0, &_D_E68BF0_2, 0x8030A800);
    setDialogueInfo(0x30, &_D_E5A6D0, &_D_E5A8C0, &_D_E588D0_2, 0x8030A800);
    setDialogueInfo(0x43, &_D_E84690, &_D_E84760, &_D_E83960_2, 0x8030A800);
    setDialogueInfo(0x2F, &_D_E58880, &_D_E588D0, &_D_E583F0_2, 0x8030A800);
    setDialogueInfo(0x33, &_D_E5F220, &_D_E5F300, &_D_E5E340_2, 0x8030A800);
    setDialogueInfo(0x31, &_D_E5D240, &_D_E5D4C0, &_D_E5A8C0_2, 0x8030A800);
    setDialogueInfo(0x32, &_D_E5E270, &_D_E5E340, &_D_E5D4C0_2, 0x8030A800);
    setDialogueInfo(0x37, &_D_E677B0, &_D_E678E0, &_D_E663F0_2, 0x8030A800);
    setDialogueInfo(0x41, &_D_E82B30, &_D_E82C40, &_D_E81F50_2, 0x8030A800);
    setDialogueInfo(0x34, &_D_E60080, &_D_E60170, &_D_E5F300_2, 0x8030A800);
    setDialogueInfo(0x39, &_D_E68BA0, &_D_E68BF0, &_D_E68700_2, 0x8030A800);
    setDialogueInfo(0x38, &_D_E68620, &_D_E68700, &_D_E678E0_2, 0x8030A800);
    setDialogueInfo(0x2E, &_D_E583C0, &_D_E583F0, &_D_E58160_2, 0x8030A800);
    setDialogueInfo(0x48, &_D_E87080, &_D_E871A0, &_D_E857A0_2, 0x8030A800);
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeNameStrings);

void initializeNameStrings(void) {
    
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

// initialize dialogue structs
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
 
INCLUDE_ASM("asm/nonmatchings/game/initialize", func_80054734);

// TODO: have to add linker symbols
/* 
void func_80054734(void) {
    setDialogueMapAddressInfo(0, 0, 0, &_D_E871B0, &_D_E871C0, 0x8030C800, &_D_E871A0, 0x8030CC00);
    setDialogueMapAddressInfo(1, 2, 2, &_D_E872C0, &_D_E87320, 0x8030C800, &_D_E871C0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x3D, 6, 6, &_D_E92800, &_D_E928B0, 0x8030C800, &_D_E92640_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x3E, 4, 4, &_D_E92910, &_D_E92950, 0x8030C800, &_D_E928B0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x44, 1, 1, &_D_E92F90, &_D_E92FF0, 0x8030C800, &_D_E92EF0_2, 0x8030CC00);
    setDialogueMapAddressInfo(9, 0x15, 0x15, &_D_E8AE00, &_D_E8AF30, 0x8030C800, &_D_E8A590_2, 0x8030CC00);
    setDialogueMapAddressInfo(8, 0x14, 0x14, &_D_E8A460, &_D_E8A590, 0x8030C800, &_D_E89C00_2, 0x8030CC00);
    setDialogueMapAddressInfo(2, 0xB, 0xB, &_D_E87BB0, &_D_E87CE0, 0x8030C800, &_D_E87320_2, 0x8030CC00);
    setDialogueMapAddressInfo(0xA, 0x17, 0x17, &_D_E8B7A0, &_D_E8B8F0, 0x8030C800, &_D_E8AF30_2, 0x8030CC00);
    setDialogueMapAddressInfo(4, 0xD, 0xD, &_D_E88B20, &_D_E88C60, 0x8030C800, &_D_E88240_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x3F, 0x44, 0x44, &_D_E92B40, &_D_E92B90, 0x8030C800, &_D_E92950_2, 0x8030CC00);
    setDialogueMapAddressInfo(7, 0x13, 0x13, &_D_E89B60, &_D_E89C00, 0x8030C800, &_D_E896B0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0xB, 0x19, 0x19, &_D_E8BCE0, &_D_E8BD80, 0x8030C800, &_D_E8B8F0_2, 0x8030CC00);
    setDialogueMapAddressInfo(6, 0x11, 0x11, &_D_E89600, &_D_E896B0, 0x8030C800, &_D_E89120_2, 0x8030CC00);
    setDialogueMapAddressInfo(5, 0x10, 0x10, &_D_E89070, &_D_E89120, 0x8030C800, &_D_E88C60_2, 0x8030CC00);
    setDialogueMapAddressInfo(3, 0xC, 0xC, &_D_E88190, &_D_E88240, 0x8030C800, &_D_E87CE0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x1F, 0x21, 0x21, &_D_E90000, &_D_E90080, 0x8030C800, &_D_E8FD00_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x24, 0x27, 0x27, &_D_E91210, &_D_E91290, 0x8030C800, &_D_E90E40_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x14, 0x1B, 0x1B, &_D_E8DDF0, &_D_E8DE60, 0x8030C800, &_D_E8DB00_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x13, 0x1A, 0x1A, &_D_E8DA90, &_D_E8DB00, 0x8030C800, &_D_E8D830_2, 0x8030CC00);
    setDialogueMapAddressInfo(0xC, 0x28, 0x28, &_D_E8C000, &_D_E8C070, 0x8030C800, &_D_E8BD80_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x12, 0x18, 0x18, &_D_E8D7B0, &_D_E8D830, 0x8030C800, &_D_E8D470_2, 0x8030CC00);
    setDialogueMapAddressInfo(0xD, 0xE, 0xE, &_D_E8C400, &_D_E8C480, 0x8030C800, &_D_E8C070_2, 0x8030CC00);
    setDialogueMapAddressInfo(0xE, 0xF, 0xF, &_D_E8C750, &_D_E8C7D0, 0x8030C800, &_D_E8C480_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x26, 0x26, 0x26, &_D_E917E0, &_D_E91860, 0x8030C800, &_D_E91520_2, 0x8030CC00);
    setDialogueMapAddressInfo(0xF, 0x12, 0x12, &_D_E8CBA0, &_D_E8CC40, 0x8030C800, &_D_E8C7D0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x11, 0x29, 0x29, &_D_E8D3F0, &_D_E8D470, 0x8030C800, &_D_E8D010_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x16, 0x2A, 0x2A, &_D_E8E490, &_D_E8E520, 0x8030C800, &_D_E8E160_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x10, 0x16, 0x16, &_D_E8CF90, &_D_E8D010, 0x8030C800, &_D_E8CC40_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x21, 0x23, 0x23, &_D_E90810, &_D_E90890, 0x8030C800, &_D_E90460_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x25, 0x2B, 0x2B, &_D_E914C0, &_D_E91520, 0x8030C800, &_D_E91290_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x18, 0x1D, 0x1D, &_D_E8EAC0, &_D_E8EB30, 0x8030C800, &_D_E8E7F0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x15, 0x2C, 0x2C, &_D_E8E0E0, &_D_E8E160, 0x8030C800, &_D_E8DE60_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x17, 0x1C, 0x1C, &_D_E8E780, &_D_E8E7F0, 0x8030C800, &_D_E8E520_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x20, 0x22, 0x22, &_D_E903D0, &_D_E90460, 0x8030C800, &_D_E90080_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x1C, 0x1F, 0x1F, &_D_E8F7A0, &_D_E8F810, 0x8030C800, &_D_E8F5C0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x1D, 0x1F, 0x1F, &_D_E8FA00, &_D_E8FA70, 0x8030C800, &_D_E8F810_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x1E, 0x20, 0x20, &_D_E8FC90, &_D_E8FD00, 0x8030C800, &_D_E8FA70_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x19, 0x1E, 0x1E, &_D_E8EE50, &_D_E8EEC0, 0x8030C800, &_D_E8EB30_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x1A, 0x1E, 0x1E, &_D_E8F1E0, &_D_E8F250, 0x8030C800, &_D_E8EEC0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x1B, 0x1E, 0x1E, &_D_E8F550, &_D_E8F5C0, 0x8030C800, &_D_E8F250_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x22, 0x24, 0x24, &_D_E90B00, &_D_E90B70, 0x8030C800, &_D_E90890_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x23, 0x25, 0x25, &_D_E90DD0, &_D_E90E40, 0x8030C800, &_D_E90B70_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x40, 0x45, 0x45, &_D_E92C50, &_D_E92C70, 0x8030C800, &_D_E92B90_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x41, 0x45, 0x45, &_D_E92DB0, &_D_E92DE0, 0x8030C800, &_D_E92C70_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x42, 0x46, 0x46, &_D_E92E90, &_D_E92EC0, 0x8030C800, &_D_E92DE0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x43, 0x47, 0x47, &_D_E92EE0, &_D_E92EF0, 0x8030C800, &_D_E92EC0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x43, 0x47, 0x47, &_D_E92EE0, &_D_E92EF0, 0x8030C800, &_D_E92EC0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x43, 0x47, 0x47, &_D_E92EE0, &_D_E92EF0, 0x8030C800, &_D_E92EC0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x43, 0x47, 0x47, &_D_E92EE0, &_D_E92EF0, 0x8030C800, &_D_E92EC0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x43, 0x47, 0x47, &_D_E92EE0, &_D_E92EF0, 0x8030C800, &_D_E92EC0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x45, 0x46, 0x46, &_D_E93060, &_D_E93080, 0x8030C800, &_D_E92FF0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x35, 0x3F, 0x3F, &_D_E92180, &_D_E92220, 0x8030C800, &_D_E92040_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x36, 0x40, 0x40, &_D_E922E0, &_D_E92370, 0x8030C800, &_D_E92220_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x27, 0x2D, 0x2D, &_D_E918A0, &_D_E918D0, 0x8030C800, &_D_E91860_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x2F, 0x35, 0x35, &_D_E91CE0, &_D_E91D30, 0x8030C800, &_D_E91C20_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x38, 0x3B, 0x3B, &_D_E924A0, &_D_E92520, 0x8030C800, &_D_E92390_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x39, 0x3C, 0x3C, &_D_E92550, &_D_E92570, 0x8030C800, &_D_E92520_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x33, 0x3D, 0x3D, &_D_E91FF0, &_D_E92010, 0x8030C800, &_D_E91FB0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x34, 0x3E, 0x3E, &_D_E92030, &_D_E92040, 0x8030C800, &_D_E92010_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x3C, 0x42, 0x42, &_D_E92630, &_D_E92640, 0x8030C800, &_D_E92610_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x30, 0x36, 0x36, &_D_E91DC0, &_D_E91E00, 0x8030C800, &_D_E91D30_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x32, 0x3A, 0x3A, &_D_E91F90, &_D_E91FB0, 0x8030C800, &_D_E91F60_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x29, 0x30, 0x30, &_D_E91970, &_D_E919A0, 0x8030C800, &_D_E91920_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x2E, 0x43, 0x43, &_D_E91BF0, &_D_E91C20, 0x8030C800, &_D_E91BA0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x28, 0x2F, 0x2F, &_D_E91900, &_D_E91920, 0x8030C800, &_D_E918D0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x2C, 0x33, 0x33, &_D_E91B50, &_D_E91B80, 0x8030C800, &_D_E91AE0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x2A, 0x31, 0x31, &_D_E91A50, &_D_E91AB0, 0x8030C800, &_D_E919A0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x2B, 0x32, 0x32, &_D_E91AD0, &_D_E91AE0, 0x8030C800, &_D_E91AB0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x31, 0x37, 0x37, &_D_E91EF0, &_D_E91F60, 0x8030C800, &_D_E91E00_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x37, 0x41, 0x41, &_D_E92380, &_D_E92390, 0x8030C800, &_D_E92370_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x2D, 0x34, 0x34, &_D_E91B90, &_D_E91BA0, 0x8030C800, &_D_E91B80_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x3B, 0x38, 0x38, &_D_E92600, &_D_E92610, 0x8030C800, &_D_E925F0_2, 0x8030CC00);
    setDialogueMapAddressInfo(0x3A, 0xA, 0xA, &_D_E925B0, &_D_E925F0, 0x8030C800, &_D_E92570_2, 0x8030CC00);
}
*/


//INCLUDE_ASM("asm/nonmatchings/game/initialize", initializeDialogueVariables);

// TODO: rename to dialogue variables?
// variables referenced in dialogues
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