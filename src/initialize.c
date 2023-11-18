#include "common.h"

#include "initialize.h"

#include "system/audio.h"
#include "system/cutscene.h"
#include "system/dialogue.h"
#include "system/memory.h"
#include "system/message.h"
#include "system/mapContext.h"

#include "animals.h" 
#include "evaluation.h"
#include "game.h"
#include "gameAudio.h"
#include "gameStart.h"
#include "gameStatus.h"
#include "initialize2.h"
#include "itemHandlers.h"
#include "level.h"
#include "loadGameScreen.h"
#include "mainLoop.h"
#include "mapObjects.h"
#include "npc.h"
#include "overlayScreens.h"
#include "player.h"
#include "setCutscenes.h"
#include "shop.h"
#include "title.h"
#include "tv.h"
#include "updateGame.h"
#include "weather.h"

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

// forward declaration
bool func_80054734(void);


//INCLUDE_ASM(const s32, "initialize", func_8004E160);

void func_8004E160(void) {
    
    volatile musConfig c;
    
    c.fxs = sfxList;
    c.priority = musPriorityList;
    
    initializeAudio(&c);

    // initialize all bss variables and structs
    initializeGameVariables();

    gCurrentGameIndex = 0; 
    
    registerMainLoopCallbacks();
    func_8004F768();
    func_800527D4();
    func_80053088();
    func_80054550();
    func_8002F7C8(0, 0, 0, 0);
    func_8003BE98(0, 0, 0, 0, 0);

    setMainLoopCallbackFunctionIndex(MAIN_GAME);
    
    initializeWaveTable(0);
}

//INCLUDE_ASM(const s32, "initialize", initializeGameVariables);

// initialize bss game variables
// probably uses static inlines for each group
void initializeGameVariables(void) {

    u8 i, j, k;
    
    MemoryRead_32 *ptr;
    MemoryRead_32 *ptr2;

    gBaseMapIndex = 0xFF;
    gCurrentSongIndex = 0xFF;
    
    gSongVolume = 128;
    
    previousMapIndex = 0xFF;
    gEntranceIndex = 0xFF;
    previousEntranceIndex = 0xFF;
    
    D_801C3F50 = 0;
    
    gWife = 0xFF;

    resetGlobalLighting();

    gYear = 1;
    gDayOfWeek = WEDNESDAY;
    gSeason = SPRING;
    gDayOfMonth = 3;
    gHour = 6;
    gMinutes = 0;
    gSeconds = 0;
    
    gGold = 300;
    
    dailyShippingBinValue = 0;
    
    gWeather = SUNNY;
    gForecast = SUNNY;
    gNextSeason = SPRING;

    D_80205230.unk_6 = 0;
    D_80205230.unk_2 = 0;
    D_80205230.unk_4 = 0;

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
    D_801FD621 = 0;
    D_801FC150 = 0;
    
    D_80237412 = 0;

    gHappiness = 0;

    blueMistFlowerPlot = 0;

    D_8016F8F4 = 0;

    D_8013DC2E = 0;

    D_802373F0 = 0;
    D_80181B10 = 0;

    D_802226E0 = 0;

    gPlayer.fatigue.counter = 0;
    gPlayer.fatigue.level = 0;
    gPlayer.fatigue.unk_2 = 0;
    gPlayer.unk_6B = 0;
    gPlayer.direction = 0;
    gPlayer.unk_70 = 0;
    gPlayer.action1 = 0;
    gPlayer.action2 = 0;
    gPlayer.action3 = 0;
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
    
    D_80189828.unk_0 = 0;
    D_80189828.unk_2 = 0;
    D_80189828.unk_3 = 0;
    D_80189828.unk_4 = 0;
    D_80189828.unk_6 = 0;
    D_80189828.unk_8 = 0;
    D_80189828.unk_A = 0;
    D_80189828.unk_C = 0;
    D_80189828.unk_E = 0;

    for (i = 0; i < TOTAL_NPCS; i++) {
        npcInfoArray[i].spriteIndex = 0;
        npcInfoArray[i].unk_1A = 0;
        npcInfoArray[i].unk_1B = 0;
        npcInfoArray[i].levelIndex = 0;
        npcInfoArray[i].movingFlag = 0;
        npcInfoArray[i].unk_1E = 0;
        npcInfoArray[i].direction = 0;
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
    dogInfo.unk_17 = 0;
    dogInfo.unk_18 = 0;
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
    horseInfo.unk_17 = 0;
    horseInfo.direction = 0;
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
        gChickens[i].unk_18 = 0;
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
        gFarmAnimals[i].unk_1C = 0;
        gFarmAnimals[i].unk_14 = 0;
        gFarmAnimals[i].unk_1D = 0;
        gFarmAnimals[i].unk_1E = 0;
         
        gFarmAnimals[i].type = 0;
        gFarmAnimals[i].condition = 0;
        gFarmAnimals[i].age = 0;
        gFarmAnimals[i].conditionCounter = 0;
         
        gFarmAnimals[i].unk_8 = 0;
        gFarmAnimals[i].unk_C = 0;
        gFarmAnimals[i].unk_10 = 0;
         
        gFarmAnimals[i].flags = 0;
         
        gFarmAnimals[i].name[0] = 0;
        gFarmAnimals[i].name[1] = 0;
        gFarmAnimals[i].name[2] = 0;
        gFarmAnimals[i].name[3] = 0;
        gFarmAnimals[i].name[4] = 0;
        gFarmAnimals[i].name[5] = 0;
          
        gFarmAnimals[i].existence = 0xFF;
         
        gFarmAnimals[i].unk_24 = 0xFF;
        gFarmAnimals[i].unk_25 = 0xFF;
        gFarmAnimals[i].unk_26 = 0xFF;
        gFarmAnimals[i].unk_27 = 0xFF;
        gFarmAnimals[i].unk_28 = 0xFF;
         
        gFarmAnimals[i].birthdaySeason = 0;
        gFarmAnimals[i].birthdayDayOfMonth = 0;
         
        gFarmAnimals[i].goldenMilk = 0xFF;
    }

    for (i = 0; i < 7; i++) {
        D_8016FB08[i].unk_15 = 0;
        D_8016FB08[i].unk_E = 0;
        D_8016FB08[i].unk_F = 0;
        D_8016FB08[i].unk_10 = 0;
        D_8016FB08[i].unk_11 = 0;
        D_8016FB08[i].unk_13 = 0;
        D_8016FB08[i].unk_14 = 0;
        D_8016FB08[i].unk_0 = 0;
        D_8016FB08[i].unk_4 = 0;
        D_8016FB08[i].unk_8 = 0;
        D_8016FB08[i].flags = 0;
    }

    for (i = 0; i < 10; i++) {
        D_80204DF8[i].unk_0.x = 0;
        D_80204DF8[i].unk_0.y = 0;
        D_80204DF8[i].unk_0.z = 0;
        
        D_80204DF8[i].unk_C = 0;
        D_80204DF8[i].unk_10 = 0;
        D_80204DF8[i].unk_14 = 0;
        
        D_80204DF8[i].unk_18.x = 0;
        D_80204DF8[i].unk_18.y = 0;
        D_80204DF8[i].unk_18.z = 0;
        
        D_80204DF8[i].unk_24 = 0;
        D_80204DF8[i].unk_26 = 0;
        D_80204DF8[i].unk_28 = 0;
        
        D_80204DF8[i].unk_2A = 0;
        D_80204DF8[i].flags = 0;
    }
    
    D_801FB6F0 = 0;
    D_801FB6F1 = 0;
    D_801FB6F3 = 0;
    D_801FB6F6 = 0;

    tvContext.channelIndex = 0;
    tvContext.mode = 0;
    tvContext.varietyShowEpisodeCounters[0] = 1;
    tvContext.varietyShowEpisodeCounters[1] = 1;
    tvContext.varietyShowEpisodeCounters[2] = 0;
    tvContext.varietyShowEpisodeCounters[3] = 0;
    tvContext.varietyShowEpisodeCounters[4] = 0;
    tvContext.varietyShowEpisodeCounters[5] = 0;
    tvContext.varietyShowEpisodeCounters[6] = 0;

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
    D_80180714 = 0;
    D_80237410 = 0;

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
    
    ptr = &farmFieldTiles;
    ptr2 = D_80113580;

    if (((u32)D_80113580 | (u32)farmFieldTiles) % 4) { 
        do {
            *(Unaligned32*)ptr++ = *(Unaligned32*)ptr2++;
        } while (ptr2 != (D_80113580 + 0x18));        
    } else {
        do {
            *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
        } while (ptr2 != (D_80113580 + 0x18));       
    }

    ptr = &D_80182BA8;
    ptr2 = &D_80113760;

    if (((u32)D_80113760 | (u32)D_80182BA8) % 4) { 
        do {
            *(Unaligned32*)ptr++ = *(Unaligned32*)ptr2++;
        } while (ptr2 != (D_80113760 + 0x14));        
    } else {
        do {
            *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
        } while (ptr2 != (D_80113760 + 0x14));       
    }
        
    albumBits |= 1;
}

//INCLUDE_ASM(const s32, "initialize", registerMainLoopCallbacks);

void registerMainLoopCallbacks(void) {

    registerMainLoopCallback(MAIN_GAME, &mainGameLoopCallback);
    registerMainLoopCallback(MAP_LOAD, &func_8005D0BC);
    registerMainLoopCallback(3, &func_80060490);
    registerMainLoopCallback(4, &func_8005C00C);
    registerMainLoopCallback(LEVEL_LOAD_1, &func_8005CA64);
    registerMainLoopCallback(LEVEL_LOAD_2, &func_8005CAA8);
    registerMainLoopCallback(OVERLAY_SCREEN_LOAD, &func_8005CB50);
    registerMainLoopCallback(ROTATING, &func_8005CBA4);
    registerMainLoopCallback(DIALOGUE, &func_8005CBF0);
    registerMainLoopCallback(TEXT, &func_8005CEFC);
    registerMainLoopCallback(0xB, &func_8005CF4C);
    registerMainLoopCallback(PINK_OVERLAY_TEXT, &func_8005D2B0);
    registerMainLoopCallback(END_OF_DAY_1, &func_80060624);
    registerMainLoopCallback(END_OF_FESTIVAL_DAY_1, &func_800604B0);
    registerMainLoopCallback(END_OF_FESTIVAL_DAY_2, &func_800605F0);
    registerMainLoopCallback(END_OF_DAY_2, &func_80060838);
    registerMainLoopCallback(0xF, &func_80060454);
    registerMainLoopCallback(TV, &tvMainLoopCallback);
    registerMainLoopCallback(SHOP_DIALOGUE, &func_800DCAB8);
    registerMainLoopCallback(PAUSE_SCREEN_LOAD, &func_800B881C);
    registerMainLoopCallback(TOOLBOX_LOAD, &func_800BADD0);
    registerMainLoopCallback(FREEZER_LOAD, &func_800BCA9C);
    registerMainLoopCallback(CABINET_LOAD, &func_800BE808);
    registerMainLoopCallback(HOME_EXTENSIONS_SELECT_LOAD, &func_800B82AC);
    registerMainLoopCallback(ESTIMATE_LOAD, &func_800B8018);
    registerMainLoopCallback(0x1A, &func_800B815C);
    registerMainLoopCallback(0x1B, &func_800B83F0);
    registerMainLoopCallback(0x1C, &func_800B8554);
    registerMainLoopCallback(0x1D, &func_800B86B8);
    registerMainLoopCallback(0x1E, &func_800C8424);
    registerMainLoopCallback(0x1F, &func_800C88F4);
    registerMainLoopCallback(HORSE_RACE_RESULTS_LOAD, &func_800CC518);
    registerMainLoopCallback(HORSE_RACE_GIFTS_LOAD, &func_800CD928);
    registerMainLoopCallback(LOTTERY_LOAD, &func_800CE930);
    registerMainLoopCallback(PAUSE_SCREEN, &func_800B9D3C);
    registerMainLoopCallback(TOOLBOX, &func_800BBEC0);
    registerMainLoopCallback(FREEZER, &func_800BDB24);
    registerMainLoopCallback(CABINET, &func_800BF990);
    registerMainLoopCallback(HOME_EXTENSIONS_SELECT, &func_800C1124);
    registerMainLoopCallback(ESTIMATE, &func_800C224C);
    registerMainLoopCallback(KITCHEN_PICTURE, &func_800C2B8C);
    registerMainLoopCallback(CALENDAR, &func_800C3D20);
    registerMainLoopCallback(RECIPE_BOOK, &func_800C53C0);
    registerMainLoopCallback(ALBUM, &func_800C7058);
    registerMainLoopCallback(0x2D, &func_800C8784);
    registerMainLoopCallback(HORSE_RACE_BETTING, &func_800CA808);
    registerMainLoopCallback(HORSE_RACE_RESULTS, &func_800CD750);
    registerMainLoopCallback(HORSE_RACE_GIFTS, &func_800CE068);
    registerMainLoopCallback(LOTTERY_2, &func_800CEDF0);
    registerMainLoopCallback(TITLE_SCREEN, &titleScreenMainLoopCallback);
    registerMainLoopCallback(NAMING_SCREEN, &func_8005CF94);
    registerMainLoopCallback(0x34, &func_800ED974);
    registerMainLoopCallback(SELECT_GAME, &func_800E1FAC);
    registerMainLoopCallback(ESTIMATE_SCREEN, &func_800E8F08);
    registerMainLoopCallback(0x37, &func_800657B4);
    registerMainLoopCallback(0x38, &func_800657C4);
    
}

// sprites
INCLUDE_ASM(const s32, "initialize", func_8004F768);

INCLUDE_ASM(const s32, "initialize", func_800527D4);

// have to add linker symbols
/*
void func_800527D4(void) {
    
    // ranch spring-winter
    setMapModelAddresses(0, &D_800530, &D_80D9D0);
    setMapModelAddresses(1, &D_80D9D0_2, &D_81AE30);
    setMapModelAddresses(2, &D_81AE30_2, &D_828FC0);
    setMapModelAddresses(3, &D_828FC0_2, &D_836420);

    // ann's room
    setMapModelAddresses(4, &D_836420_2, &D_83DC90);

    // ranch barn
    setMapModelAddresses(5, &D_83DC90_2, &D_849700);

    // ranch store
    setMapModelAddresses(6, &D_849700_2, &D_84F100);

    // ranch house
    setMapModelAddresses(7, &D_84F100_2, &D_856B80);

    // empty
    setMapModelAddresses(8, &D_856B80_2, &D_856B90);
    
    // beach spring-winter
    setMapModelAddresses(9, &D_856B90_2, &D_85EE70);
    setMapModelAddresses(0xA, &D_85EE70_2, &D_867150);
    setMapModelAddresses(0xB, &D_867150_2, &D_86F430);
    setMapModelAddresses(0xC, &D_86F430_2, &D_877710);

    // race track spring, autumn, and winter
    setMapModelAddresses(0xD, &D_877710_2, &D_87F510);
    
    // no race track summer: 0xE
    // empty 4 words in rom

    setMapModelAddresses(0xF, &D_87F520_2, &D_887320);
    setMapModelAddresses(0x10, &D_887320_2, &D_88F120);

    // mountain 1 spring-winter 
    setMapModelAddresses(0x11, &D_88F120_2, &D_8A6C20);
    setMapModelAddresses(0x12, &D_8A6C20_2, &D_8BE720);
    setMapModelAddresses(0x13, &D_8BE720_2, &D_8D6220);
    setMapModelAddresses(0x14, &D_8D6220_2, &D_8ED8E0);

    // mountain 2 spring-winter 
    setMapModelAddresses(0x15, &D_8ED8E0_2, &D_905980);
    setMapModelAddresses(0x16, &D_905980_2, &D_91D8A0);
    setMapModelAddresses(0x17, &D_91D8A0_2, &D_9357C0);
    setMapModelAddresses(0x18, &D_9357C0_2, &D_94D6E0);

    top of mountain 1 spring-winter
    setMapModelAddresses(0x19, &D_94D6E0_2, &D_961BD0);
    setMapModelAddresses(0x1A, &D_961BD0_2, &D_975E50);
    setMapModelAddresses(0x1B, &D_975E50_2, &D_98A0D0);
    setMapModelAddresses(0x1C, &D_98A0D0_2, &D_99F9C0);
    
    // moon mountain spring-winter
    setMapModelAddresses(0x1D, &D_99F9C0_2, &D_9B4530);
    setMapModelAddresses(0x1E, &D_9B4530_2, &D_9C90A0);
    setMapModelAddresses(0x1F, &D_9C90A0_2, &D_9DDC10);
    setMapModelAddresses(0x20, &D_9DDC10_2, &D_9F2780);

    // carpenter's
    setMapModelAddresses(0x21, &D_9F2780_2, &D_9FAA80);
    
    // dumpling house
    setMapModelAddresses(0x22, &D_9FAA80_2, &D_A04C80);

    // spring spring-winter
    setMapModelAddresses(0x23, &D_A04C80_2, &D_A0CEC0);
    setMapModelAddresses(0x24, &D_A0CEC0_2, &D_A14F10);
    setMapModelAddresses(0x25, &D_A14F10_2, &D_A1CF60);
    setMapModelAddresses(0x26, &D_A1CF60_2, &D_A24FB0);

    // elli's room
    setMapModelAddresses(0x27, &D_A24FB0_2, &D_A2C350);

    // bakery
    setMapModelAddresses(0x28, &D_A2C350_2, &D_A33460);

    // village 1 spring-winter
    setMapModelAddresses(0x29, &D_A33460_2, &D_A4C640);
    setMapModelAddresses(0x2A, &D_A4C640_2, &D_A65820);
    setMapModelAddresses(0x2B, &D_A65820_2, &D_A7F2B0);
    setMapModelAddresses(0x2C, &D_A7F2B0_2, &D_A98210);

    // village 2 spring-winter
    setMapModelAddresses(0x2D, &D_A98210_2, &D_AAC2C0);
    setMapModelAddresses(0x2E, &D_AAC2C0_2, &D_AC0370);
    setMapModelAddresses(0x2F, &D_AC0370_2, &D_AD4420);
    setMapModelAddresses(0x30, &D_AD4420_2, &D_AE84D0);

    // popuri's room
    setMapModelAddresses(0x31, &D_AE84D0_2, &D_AEF920);

    // flower shop
    setMapModelAddresses(0x32, &D_AEF920_2, &D_AF4F00);

    // church
    setMapModelAddresses(0x33, &D_AF4F00_2, &D_AFAAC0);

    // souvenir shop
    setMapModelAddresses(0x34, &D_AFAAC0_2, &D_AFF110);

    // square spring-winter
    setMapModelAddresses(0x35, &D_AFF110_2, &D_B0E3C0);
    setMapModelAddresses(0x36, &D_B0E3C0_2, &D_B1D460);
    setMapModelAddresses(0x37, &D_B1D460_2, &D_B2C500);
    setMapModelAddresses(0x38, &D_B2C500_2, &D_B38990);

    // rick's
    setMapModelAddresses(0x39, &D_B38990_2, &D_B3CE10);

    // midwife's
    setMapModelAddresses(0x3A, &D_B3CE10_2, &D_B41CC0);
    
    // tavern
    setMapModelAddresses(0x3B, &D_B41CC0_2, &D_B4A9C0);
    
    // library
    setMapModelAddresses(0x3C, &D_B4A9C0_2, &D_B4E440);
    
    // maria's room
    setMapModelAddresses(0x3D, &D_B4E440_2, &D_B543D0);
    
    // mayor's house
    setMapModelAddresses(0x3E, &D_B543D0_2, &D_B5B0B0);

    // potion shop back
    setMapModelAddresses(0x3F, &D_B5B0B0_2, &D_B60990);

    // potion shop
    setMapModelAddresses(0x40, &D_B60990_2, &D_B64E30);

    // empty, referenced in func_80074C50
    setMapModelAddresses(0x41, &D_B64E30_2, &D_B64E40);
    
    // sprite cave
    setMapModelAddresses(0x42, &D_B64E40_2, &D_B6AAC0);

    // cave
    setMapModelAddresses(0x43, &D_B6AAC0_2, &D_B6E700);
    
    // empty (mine)
    setMapModelAddresses(0x44, &D_B6E700_2, &D_B6E710);
    // mine 2
    setMapModelAddresses(0x45, &D_B6E710_2, &D_B6FB70);

    // karen's room
    setMapModelAddresses(0x46, &D_B6FB70_2, &D_B7AFB0);

    // vineyard spring-winter
    setMapModelAddresses(0x47, &D_B7AFB0_2, &D_B8B840);
    setMapModelAddresses(0x48, &D_B8B840_2, &D_B9C0D0);
    setMapModelAddresses(0x49, &D_B9C0D0_2, &D_BAC960);
    setMapModelAddresses(0x4A, &D_BAC960_2, &D_BBCD10);

    // vineyard house
    setMapModelAddresses(0x4B, &D_BBCD10_2, &D_BC5710);

    // vineyard cellar 1
    setMapModelAddresses(0x4C, &D_BC5710_2, &D_BC9C30);
    // vineyard cellar 2
    setMapModelAddresses(0x4D, &D_BC9C30_2, &D_BD1C80);

    // road spring-winter
    setMapModelAddresses(0x4E, &D_BD1C80_2, &D_BDBD50);
    setMapModelAddresses(0x4F, &D_BDBD50_2, &D_BE5E20);
    setMapModelAddresses(0x50, &D_BE5E20_2, &D_BEFEF0);
    setMapModelAddresses(0x51, &D_BEFEF0_2, &D_BF9FC0);

    // farm spring-winter
    setMapModelAddresses(0x52, &D_BF9FC0_2, &D_C13C40);
    setMapModelAddresses(0x53, &D_C13C40_2, &D_C2D8C0);
    setMapModelAddresses(0x54, &D_C2D8C0_2, &D_C47540);
    setMapModelAddresses(0x55, &D_C47540_2, &D_C609F0);

    // greenhouse
    setMapModelAddresses(0x56, &D_C609F0_2, &D_C64D60);

    // house
    setMapModelAddresses(0x57, &D_C64D60_2, &D_C72FB0);

    // barn
    setMapModelAddresses(0x58, &D_C72FB0_2, &D_C7DE40);

    // coop
    setMapModelAddresses(0x59, &D_C7DE40_2, &D_C840E0);

    // kitchen
    setMapModelAddresses(0x5A, &D_C840E0_2, &D_C8D5C0);

    // bathroom
    setMapModelAddresses(0x5B, &D_C8D5C0_2, &D_C951A0);
    
    func_8003BA44(0, 0, MODEL_DATA_BANK);
}
*/

INCLUDE_ASM(const s32, "initialize", func_80053088);

// rodata issue
/*
void func_80053088(void) {

    u32 ptr = &fontTexture_ROM_START;
    u32 ptr2 = &fontTexture_ROM_END;
    u32 ptr3 = &fontPalette1_ROM_START;
    u32 ptr4 = &fontPalette1_ROM_END;
    u32 ptr5 = &fontPalette2_ROM_START;
    u32 ptr6 = &fontPalette2_ROM_END;

    func_800535DC();
    func_80054180();
    func_8003FAE8(&D_8011BD18);

    nuPiReadRom(ptr, (void*)0x802FF000, ptr2 - ptr);
    nuPiReadRom(ptr3, (void*)0x8030A000, ptr4 - ptr3);
    nuPiReadRom(ptr5, (void*)0x8030A400, ptr6 - ptr5);

    func_8003F80C(0, 0x76, &dialogueWindowTexture_ROM_START, &dialogueWindowTexture_ROM_END, &dialogueWindowIndex_ROM_START, &dialogueWindowIndex_ROM_END, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 0, 1, -24.0f, 0, 0);
    func_8003F80C(1, 0x76, &dialogueWindowTexture_ROM_START, &dialogueWindowTexture_ROM_END, &dialogueWindowIndex_ROM_START, &dialogueWindowIndex_ROM_END, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 0, 0, 0, 0, 0);
    func_8003F80C(2, 0x77, &dialogueWindowTexture_ROM_START, &dialogueWindowTexture_ROM_END, &dialogueWindowIndex_ROM_START, &dialogueWindowIndex_ROM_END, 0x80238800, 0x8023B100, 0x8023B200, 0x8023B300, 0, 1, 0, 0, 0, 0);

    func_8003F910(0, 0x78, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsIndex_ROM_START, &dialogueIconsIndex_ROM_END, (void* )0x8023B400, (void* )0x8023CC00, (void* )0x8023CE00, 0x8023D200, 0, 4, 0xFE, 106.0f, -15.0f, 0.0f);
    func_8003F910(1, 0x78, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsIndex_ROM_START, &dialogueIconsIndex_ROM_END, (void* )0x8023B400, (void* )0x8023CC00, (void* )0x8023CE00, 0x8023D200, 0, 0xD, 0xFE, 106.0f, -15.0f, 0.0f);
    func_8003FA1C(0, 0x75, &characterDialogueIconsTexture_ROM_START, &characterDialogueIconsTexture_ROM_END, &characterdialogueIconsAssetsIndex_ROM_START, &characterdialogueIconsAssetsIndex_ROM_END, &characterDialogueIconsSpritesheetIndex_ROM_START, &characterDialogueIconsSpritesheetIndex_ROM_END, (void*)0x8023D300, (void*)0x8023DB00, (void*)0x8023E300, (void*)0x8023EF00, (void*)0x8023FF00, (void*)0x80240000, -139.0f, 1.0f, 0);
 
    func_8003DBE8(0, 0x8030B000);
    func_8003F54C(0, 24.0f, -64.0f, 352.0f);
    func_8003F5D0(0, 0x10, 3);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, 0x802FF000, 0x8030A000);
    func_8003F360(0, ~(1 | 2), 0);
    func_8003F690(0, 0, 0, 0);
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
    func_8003F690(1, 2, 0xFF, 0xFF);
    func_8003F4E0(1, 0xFF, 0xFF, 0xFF);
    
}
*/

INCLUDE_ASM(const s32, "initialize", func_800535DC);

// dialogues
// need to add linker symbols
/*
void func_800535DC(void) {
    func_8003F30C(0, &D_E13920, &D_E13990, &D_E13800_2, 0x8030A800);
    func_8003F30C(1, &D_E13C30, &D_E13C60, &D_E13990_2, 0x8030A800);
    func_8003F30C(2, &D_E167C0, &D_E16A90, &D_E13C60_2, 0x8030A800);
    func_8003F30C(3, &D_E19F60, &D_E19FF0, &D_E16A90_2, 0x8030A800);
    func_8003F30C(4, &D_E1B3A0, &D_E1B4E0, &D_E19FF0_2, 0x8030A800);
    func_8003F30C(5, &D_E1C8C0, &D_E1C9C0, &D_E1B4E0_2, 0x8030A800);
    func_8003F30C(6, &D_E21150, &D_E21700, &D_E1C9C0_2, 0x8030A800);
    func_8003F30C(7, &D_E218A0, &D_E218D0, &D_E21700_2, 0x8030A800);
    func_8003F30C(8, &D_E24E20, &D_E24F60, &D_E218D0_2, 0x8030A800);
    func_8003F30C(9, &D_E24F70, &D_E24F80, &D_E24F60_2, 0x8030A800);
    func_8003F30C(0xA, &D_E25170, &D_E251D0, &D_E24F80_2, 0x8030A800);
    func_8003F30C(0x15, &D_E3C700, &D_E3C9A0, &D_E39610_2, 0x8030A800);
    func_8003F30C(0x14, &D_E39360, &D_E39610, &D_E36150_2, 0x8030A800);
    func_8003F30C(0xB, &D_E28460, &D_E28720, &D_E251D0_2, 0x8030A800);
    func_8003F30C(0x17, &D_E40B90, &D_E40E50, &D_E3D6D0_2, 0x8030A800);
    func_8003F30C(0xD, &D_E2D080, &D_E2D330, &D_E2A380_2, 0x8030A800);
    func_8003F30C(0x44, &D_E84910, &D_E84990, &D_E84760_2, 0x8030A800);
    func_8003F30C(0x13, &D_E36000, &D_E36150, &D_E34790_2, 0x8030A800);
    func_8003F30C(0x19, &D_E42D50, &D_E42E60, &D_E42100_2, 0x8030A800);
    func_8003F30C(0x11, &D_E33020, &D_E33190, &D_E31010_2, 0x8030A800);
    func_8003F30C(0x10, &D_E30ED0, &D_E31010, &D_E2F730_2, 0x8030A800);
    func_8003F30C(0xC, &D_E2A230, &D_E2A380, &D_E28720_2, 0x8030A800);
    func_8003F30C(0x21, &D_E4B6E0, &D_E4B7D0, &D_E4A2D0_2, 0x8030A800);
    func_8003F30C(0x27, &D_E51A00, &D_E51AE0, &D_E50600_2, 0x8030A800);
    func_8003F30C(0x1B, &D_E45130, &D_E45200, &D_E43F60_2, 0x8030A800);
    func_8003F30C(0x1A, &D_E43E80, &D_E43F60, &D_E42E60_2, 0x8030A800);
    func_8003F30C(0x28, &D_E52740, &D_E527E0, &D_E51AE0_2, 0x8030A800);
    func_8003F30C(0x18, &D_E42010, &D_E42100, &D_E40E50_2, 0x8030A800);
    func_8003F30C(0xE, &D_E2E500, &D_E2E600, &D_E2D330_2, 0x8030A800);
    func_8003F30C(0xF, &D_E2F650, &D_E2F730, &D_E2E600_2, 0x8030A800);
    func_8003F30C(0x26, &D_E50520, &D_E50600, &D_E4F3F0_2, 0x8030A800);
    func_8003F30C(0x12, &D_E34650, &D_E34790, &D_E33190_2, 0x8030A800);
    func_8003F30C(0x29, &D_E53450, &D_E53540, &D_E527E0_2, 0x8030A800);
    func_8003F30C(0x2A, &D_E54C60, &D_E54D60, &D_E53540_2, 0x8030A800);
    func_8003F30C(0x16, &D_E3D600, &D_E3D6D0, &D_E3C9A0_2, 0x8030A800);
    func_8003F30C(0x23, &D_E4E230, &D_E4E320, &D_E4C9B0_2, 0x8030A800);
    func_8003F30C(0x2B, &D_E55AE0, &D_E55BA0, &D_E54D60_2, 0x8030A800);
    func_8003F30C(0x1D, &D_E47140, &D_E471E0, &D_E46280_2, 0x8030A800);
    func_8003F30C(0x2C, &D_E569C0, &D_E56A80, &D_E55BA0_2, 0x8030A800);
    func_8003F30C(0x1C, &D_E461C0, &D_E46280, &D_E45200_2, 0x8030A800);
    func_8003F30C(0x22, &D_E4C8D0, &D_E4C9B0, &D_E4B7D0_2, 0x8030A800);
    func_8003F30C(0x1F, &D_E49480, &D_E49540, &D_E48720_2, 0x8030A800);
    func_8003F30C(0x1F, &D_E49480, &D_E49540, &D_E48720_2, 0x8030A800);
    func_8003F30C(0x20, &D_E4A230, &D_E4A2D0, &D_E49540_2, 0x8030A800);
    func_8003F30C(0x1E, &D_E485D0, &D_E48720, &D_E471E0_2, 0x8030A800);
    func_8003F30C(0x1E, &D_E485D0, &D_E48720, &D_E471E0_2, 0x8030A800);
    func_8003F30C(0x1E, &D_E485D0, &D_E48720, &D_E471E0_2, 0x8030A800);
    func_8003F30C(0x24, &D_E4EC70, &D_E4ED10, &D_E4E320_2, 0x8030A800);
    func_8003F30C(0x25, &D_E4F370, &D_E4F3F0, &D_E4ED10_2, 0x8030A800);
    func_8003F30C(0x45, &D_E84F50, &D_E84FD0, &D_E84990_2, 0x8030A800);
    func_8003F30C(0x45, &D_E84F50, &D_E84FD0, &D_E84990_2, 0x8030A800);
    func_8003F30C(0x46, &D_E85730, &D_E857A0, &D_E84FD0_2, 0x8030A800);
    func_8003F30C(0x47, &D_E84910, &D_E84990, &D_E84760_2, 0x8030A800);
    func_8003F30C(0x47, &D_E84910, &D_E84990, &D_E84760_2, 0x8030A800);
    func_8003F30C(0x47, &D_E84910, &D_E84990, &D_E84760_2, 0x8030A800);
    func_8003F30C(0x47, &D_E84910, &D_E84990, &D_E84760_2, 0x8030A800);
    func_8003F30C(0x47, &D_E84910, &D_E84990, &D_E84760_2, 0x8030A800);
    func_8003F30C(0x46, &D_E85730, &D_E857A0, &D_E84FD0_2, 0x8030A800);
    func_8003F30C(0x3F, &D_E7C7B0, &D_E7D1E0, &D_E73110_2, 0x8030A800);
    func_8003F30C(0x40, &D_E81660, &D_E81F50, &D_E7D1E0_2, 0x8030A800);
    func_8003F30C(0x2D, &D_E58010, &D_E58160, &D_E56A80_2, 0x8030A800);
    func_8003F30C(0x35, &D_E64680, &D_E64AF0, &D_E60170_2, 0x8030A800);
    func_8003F30C(0x3B, &D_E6F1E0, &D_E6F5C0, &D_E6AFE0_2, 0x8030A800);
    func_8003F30C(0x3C, &D_E714D0, &D_E71690, &D_E6F5C0_2, 0x8030A800);
    func_8003F30C(0x3D, &D_E72680, &D_E72780, &D_E71690_2, 0x8030A800);
    func_8003F30C(0x3E, &D_E73070, &D_E73110, &D_E72780_2, 0x8030A800);
    func_8003F30C(0x42, &D_E83880, &D_E83960, &D_E82C40_2, 0x8030A800);
    func_8003F30C(0x36, &D_E66260, &D_E663F0, &D_E64AF0_2, 0x8030A800);
    func_8003F30C(0x3A, &D_E6ADC0, &D_E6AFE0, &D_E68BF0_2, 0x8030A800);
    func_8003F30C(0x30, &D_E5A6D0, &D_E5A8C0, &D_E588D0_2, 0x8030A800);
    func_8003F30C(0x43, &D_E84690, &D_E84760, &D_E83960_2, 0x8030A800);
    func_8003F30C(0x2F, &D_E58880, &D_E588D0, &D_E583F0_2, 0x8030A800);
    func_8003F30C(0x33, &D_E5F220, &D_E5F300, &D_E5E340_2, 0x8030A800);
    func_8003F30C(0x31, &D_E5D240, &D_E5D4C0, &D_E5A8C0_2, 0x8030A800);
    func_8003F30C(0x32, &D_E5E270, &D_E5E340, &D_E5D4C0_2, 0x8030A800);
    func_8003F30C(0x37, &D_E677B0, &D_E678E0, &D_E663F0_2, 0x8030A800);
    func_8003F30C(0x41, &D_E82B30, &D_E82C40, &D_E81F50_2, 0x8030A800);
    func_8003F30C(0x34, &D_E60080, &D_E60170, &D_E5F300_2, 0x8030A800);
    func_8003F30C(0x39, &D_E68BA0, &D_E68BF0, &D_E68700_2, 0x8030A800);
    func_8003F30C(0x38, &D_E68620, &D_E68700, &D_E678E0_2, 0x8030A800);
    func_8003F30C(0x2E, &D_E583C0, &D_E583F0, &D_E58160_2, 0x8030A800);
    func_8003F30C(0x48, &D_E87080, &D_E871A0, &D_E857A0_2, 0x8030A800);
}
*/

//INCLUDE_ASM(const s32, "initialize", func_80054180);

void func_80054180(void) {
    
    func_8003FBA0(0, gPlayer.name, 6);
    func_8003FBA0(1, gFarmName, 6);
    func_8003FBA0(2, dogInfo.name, 6);
    func_8003FBA0(3, horseInfo.name, 6);
    func_8003FBA0(4, gBabyName, 6); 
     
    func_8003FBA0(5, gFarmAnimals[0].name, 6);
    func_8003FBA0(6, gFarmAnimals[1].name, 6);
    func_8003FBA0(7, gFarmAnimals[2].name, 6);
    func_8003FBA0(8, gFarmAnimals[3].name, 6);
    func_8003FBA0(9, gFarmAnimals[4].name, 6);
    func_8003FBA0(0xA, gFarmAnimals[5].name, 6);
    func_8003FBA0(0xB, gFarmAnimals[6].name, 6);
    func_8003FBA0(0xC, gFarmAnimals[7].name, 6);
    
    func_8003FBA0(0xE, D_80237380, 6);
    
    func_8003FBA0(0xF, gWifeName, 6);
    
    func_8003FBA0(0x10, D_801FB5C4, 6);
    func_8003FBA0(0x11, D_801594E0, 3);
    func_8003FBA0(0x12, D_801A8B50, 8);
    func_8003FBA0(0x13, D_8018985C, 6);
    func_8003FBA0(0x14, gCurrentSeasonName, 6);
    func_8003FBA0(0x15, D_801FC152, 2);
    func_8003FBA0(0x16, gWifeName, 6);
    func_8003FBA0(0x17, D_80205210, 3);
    func_8003FBA0(0x18, D_80237418, 3);
    
    func_8003FBA0(0x19, gChickens[0].name, 6);
    func_8003FBA0(0x1A, gChickens[1].name, 6);
    func_8003FBA0(0x1B, gChickens[2].name, 6);
    func_8003FBA0(0x1C, gChickens[3].name, 6);
    func_8003FBA0(0x1D, gChickens[4].name, 6);
    func_8003FBA0(0x1E, gChickens[5].name, 6);
    
    func_8003FBA0(0x1F, D_8020563B, 4);
    func_8003FBA0(0x20, harvestKingName, 6);
    func_8003FBA0(0x21, D_801806C8, 2);
    func_8003FBA0(0x22, D_80170268, 6);
    func_8003FBA0(0x23, D_801FC156, 6);
    func_8003FBA0(0x24, D_80204B3C, 6);
    func_8003FBA0(0x25, D_8016FBCC, 1);
    func_8003FBA0(0x27, D_801886D4, 6);
    func_8003FBA0(0x30, D_801594E6, 3);
    func_8003FBA0(0x31, D_8016F6E0, 3);
    
    func_8003FBA0(0x32, D_80182D90[0], 3);
    func_8003FBA0(0x33, D_80182D90[1], 3);
    func_8003FBA0(0x34, D_80182D90[2], 3);
    func_8003FBA0(0x35, D_80182D90[3], 3);
    func_8003FBA0(0x36, D_80182D90[4], 3);
    func_8003FBA0(0x37, D_80182D90[5], 3);
    func_8003FBA0(0x38, D_80182D90[6], 3);
    func_8003FBA0(0x39, D_80182D90[7], 3);
    func_8003FBA0(0x3A, D_80182D90[8], 3);
    func_8003FBA0(0x3B, D_80182D90[9], 3);
    func_8003FBA0(0x3C, D_80182D90[10], 3);
    
}

//INCLUDE_ASM(const s32, "initialize", func_80054550);

void func_80054550(void) {

    func_80054734();

    setGameVariables();

    func_80043138(specialDialogueBits);

    func_80042FEC(0, 0, 1);

    func_8004318C(0, 0x79, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsIndex_ROM_START, &dialogueIconsIndex_ROM_END, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, 3, 0, 8.0f, -16.0f, 0);
    func_80043260(0, 0x7A, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsIndex_ROM_START, &dialogueIconsIndex_ROM_END, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, 0xA, 0xFE, 0, 40.0f, 0);
    func_80043334(0, 0x7B, &dialogueIconsTexture_ROM_START, &dialogueIconsTexture_ROM_END, &dialogueIconsIndex_ROM_START, &dialogueIconsIndex_ROM_END, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, 0xB, 0xFE, 0, -40.0f, 0);
   
    func_80043148(0, 2, 0, 8);

}
 
INCLUDE_ASM(const s32, "initialize", func_80054734);

// have to add assets to splat.yaml
/* 
void func_80054734(void) {
    func_80043050(0, 0, 0, &D_E871B0, &D_E871C0, 0x8030C800, &D_E871A0, 0x8030CC00);
    func_80043050(1, 2, 2, &D_E872C0, &D_E87320, 0x8030C800, &D_E871C0_2, 0x8030CC00);
    func_80043050(0x3D, 6, 6, &D_E92800, &D_E928B0, 0x8030C800, &D_E92640_2, 0x8030CC00);
    func_80043050(0x3E, 4, 4, &D_E92910, &D_E92950, 0x8030C800, &D_E928B0_2, 0x8030CC00);
    func_80043050(0x44, 1, 1, &D_E92F90, &D_E92FF0, 0x8030C800, &D_E92EF0_2, 0x8030CC00);
    func_80043050(9, 0x15, 0x15, &D_E8AE00, &D_E8AF30, 0x8030C800, &D_E8A590_2, 0x8030CC00);
    func_80043050(8, 0x14, 0x14, &D_E8A460, &D_E8A590, 0x8030C800, &D_E89C00_2, 0x8030CC00);
    func_80043050(2, 0xB, 0xB, &D_E87BB0, &D_E87CE0, 0x8030C800, &D_E87320_2, 0x8030CC00);
    func_80043050(0xA, 0x17, 0x17, &D_E8B7A0, &D_E8B8F0, 0x8030C800, &D_E8AF30_2, 0x8030CC00);
    func_80043050(4, 0xD, 0xD, &D_E88B20, &D_E88C60, 0x8030C800, &D_E88240_2, 0x8030CC00);
    func_80043050(0x3F, 0x44, 0x44, &D_E92B40, &D_E92B90, 0x8030C800, &D_E92950_2, 0x8030CC00);
    func_80043050(7, 0x13, 0x13, &D_E89B60, &D_E89C00, 0x8030C800, &D_E896B0_2, 0x8030CC00);
    func_80043050(0xB, 0x19, 0x19, &D_E8BCE0, &D_E8BD80, 0x8030C800, &D_E8B8F0_2, 0x8030CC00);
    func_80043050(6, 0x11, 0x11, &D_E89600, &D_E896B0, 0x8030C800, &D_E89120_2, 0x8030CC00);
    func_80043050(5, 0x10, 0x10, &D_E89070, &D_E89120, 0x8030C800, &D_E88C60_2, 0x8030CC00);
    func_80043050(3, 0xC, 0xC, &D_E88190, &D_E88240, 0x8030C800, &D_E87CE0_2, 0x8030CC00);
    func_80043050(0x1F, 0x21, 0x21, &D_E90000, &D_E90080, 0x8030C800, &D_E8FD00_2, 0x8030CC00);
    func_80043050(0x24, 0x27, 0x27, &D_E91210, &D_E91290, 0x8030C800, &D_E90E40_2, 0x8030CC00);
    func_80043050(0x14, 0x1B, 0x1B, &D_E8DDF0, &D_E8DE60, 0x8030C800, &D_E8DB00_2, 0x8030CC00);
    func_80043050(0x13, 0x1A, 0x1A, &D_E8DA90, &D_E8DB00, 0x8030C800, &D_E8D830_2, 0x8030CC00);
    func_80043050(0xC, 0x28, 0x28, &D_E8C000, &D_E8C070, 0x8030C800, &D_E8BD80_2, 0x8030CC00);
    func_80043050(0x12, 0x18, 0x18, &D_E8D7B0, &D_E8D830, 0x8030C800, &D_E8D470_2, 0x8030CC00);
    func_80043050(0xD, 0xE, 0xE, &D_E8C400, &D_E8C480, 0x8030C800, &D_E8C070_2, 0x8030CC00);
    func_80043050(0xE, 0xF, 0xF, &D_E8C750, &D_E8C7D0, 0x8030C800, &D_E8C480_2, 0x8030CC00);
    func_80043050(0x26, 0x26, 0x26, &D_E917E0, &D_E91860, 0x8030C800, &D_E91520_2, 0x8030CC00);
    func_80043050(0xF, 0x12, 0x12, &D_E8CBA0, &D_E8CC40, 0x8030C800, &D_E8C7D0_2, 0x8030CC00);
    func_80043050(0x11, 0x29, 0x29, &D_E8D3F0, &D_E8D470, 0x8030C800, &D_E8D010_2, 0x8030CC00);
    func_80043050(0x16, 0x2A, 0x2A, &D_E8E490, &D_E8E520, 0x8030C800, &D_E8E160_2, 0x8030CC00);
    func_80043050(0x10, 0x16, 0x16, &D_E8CF90, &D_E8D010, 0x8030C800, &D_E8CC40_2, 0x8030CC00);
    func_80043050(0x21, 0x23, 0x23, &D_E90810, &D_E90890, 0x8030C800, &D_E90460_2, 0x8030CC00);
    func_80043050(0x25, 0x2B, 0x2B, &D_E914C0, &D_E91520, 0x8030C800, &D_E91290_2, 0x8030CC00);
    func_80043050(0x18, 0x1D, 0x1D, &D_E8EAC0, &D_E8EB30, 0x8030C800, &D_E8E7F0_2, 0x8030CC00);
    func_80043050(0x15, 0x2C, 0x2C, &D_E8E0E0, &D_E8E160, 0x8030C800, &D_E8DE60_2, 0x8030CC00);
    func_80043050(0x17, 0x1C, 0x1C, &D_E8E780, &D_E8E7F0, 0x8030C800, &D_E8E520_2, 0x8030CC00);
    func_80043050(0x20, 0x22, 0x22, &D_E903D0, &D_E90460, 0x8030C800, &D_E90080_2, 0x8030CC00);
    func_80043050(0x1C, 0x1F, 0x1F, &D_E8F7A0, &D_E8F810, 0x8030C800, &D_E8F5C0_2, 0x8030CC00);
    func_80043050(0x1D, 0x1F, 0x1F, &D_E8FA00, &D_E8FA70, 0x8030C800, &D_E8F810_2, 0x8030CC00);
    func_80043050(0x1E, 0x20, 0x20, &D_E8FC90, &D_E8FD00, 0x8030C800, &D_E8FA70_2, 0x8030CC00);
    func_80043050(0x19, 0x1E, 0x1E, &D_E8EE50, &D_E8EEC0, 0x8030C800, &D_E8EB30_2, 0x8030CC00);
    func_80043050(0x1A, 0x1E, 0x1E, &D_E8F1E0, &D_E8F250, 0x8030C800, &D_E8EEC0_2, 0x8030CC00);
    func_80043050(0x1B, 0x1E, 0x1E, &D_E8F550, &D_E8F5C0, 0x8030C800, &D_E8F250_2, 0x8030CC00);
    func_80043050(0x22, 0x24, 0x24, &D_E90B00, &D_E90B70, 0x8030C800, &D_E90890_2, 0x8030CC00);
    func_80043050(0x23, 0x25, 0x25, &D_E90DD0, &D_E90E40, 0x8030C800, &D_E90B70_2, 0x8030CC00);
    func_80043050(0x40, 0x45, 0x45, &D_E92C50, &D_E92C70, 0x8030C800, &D_E92B90_2, 0x8030CC00);
    func_80043050(0x41, 0x45, 0x45, &D_E92DB0, &D_E92DE0, 0x8030C800, &D_E92C70_2, 0x8030CC00);
    func_80043050(0x42, 0x46, 0x46, &D_E92E90, &D_E92EC0, 0x8030C800, &D_E92DE0_2, 0x8030CC00);
    func_80043050(0x43, 0x47, 0x47, &D_E92EE0, &D_E92EF0, 0x8030C800, &D_E92EC0_2, 0x8030CC00);
    func_80043050(0x43, 0x47, 0x47, &D_E92EE0, &D_E92EF0, 0x8030C800, &D_E92EC0_2, 0x8030CC00);
    func_80043050(0x43, 0x47, 0x47, &D_E92EE0, &D_E92EF0, 0x8030C800, &D_E92EC0_2, 0x8030CC00);
    func_80043050(0x43, 0x47, 0x47, &D_E92EE0, &D_E92EF0, 0x8030C800, &D_E92EC0_2, 0x8030CC00);
    func_80043050(0x43, 0x47, 0x47, &D_E92EE0, &D_E92EF0, 0x8030C800, &D_E92EC0_2, 0x8030CC00);
    func_80043050(0x45, 0x46, 0x46, &D_E93060, &D_E93080, 0x8030C800, &D_E92FF0_2, 0x8030CC00);
    func_80043050(0x35, 0x3F, 0x3F, &D_E92180, &D_E92220, 0x8030C800, &D_E92040_2, 0x8030CC00);
    func_80043050(0x36, 0x40, 0x40, &D_E922E0, &D_E92370, 0x8030C800, &D_E92220_2, 0x8030CC00);
    func_80043050(0x27, 0x2D, 0x2D, &D_E918A0, &D_E918D0, 0x8030C800, &D_E91860_2, 0x8030CC00);
    func_80043050(0x2F, 0x35, 0x35, &D_E91CE0, &D_E91D30, 0x8030C800, &D_E91C20_2, 0x8030CC00);
    func_80043050(0x38, 0x3B, 0x3B, &D_E924A0, &D_E92520, 0x8030C800, &D_E92390_2, 0x8030CC00);
    func_80043050(0x39, 0x3C, 0x3C, &D_E92550, &D_E92570, 0x8030C800, &D_E92520_2, 0x8030CC00);
    func_80043050(0x33, 0x3D, 0x3D, &D_E91FF0, &D_E92010, 0x8030C800, &D_E91FB0_2, 0x8030CC00);
    func_80043050(0x34, 0x3E, 0x3E, &D_E92030, &D_E92040, 0x8030C800, &D_E92010_2, 0x8030CC00);
    func_80043050(0x3C, 0x42, 0x42, &D_E92630, &D_E92640, 0x8030C800, &D_E92610_2, 0x8030CC00);
    func_80043050(0x30, 0x36, 0x36, &D_E91DC0, &D_E91E00, 0x8030C800, &D_E91D30_2, 0x8030CC00);
    func_80043050(0x32, 0x3A, 0x3A, &D_E91F90, &D_E91FB0, 0x8030C800, &D_E91F60_2, 0x8030CC00);
    func_80043050(0x29, 0x30, 0x30, &D_E91970, &D_E919A0, 0x8030C800, &D_E91920_2, 0x8030CC00);
    func_80043050(0x2E, 0x43, 0x43, &D_E91BF0, &D_E91C20, 0x8030C800, &D_E91BA0_2, 0x8030CC00);
    func_80043050(0x28, 0x2F, 0x2F, &D_E91900, &D_E91920, 0x8030C800, &D_E918D0_2, 0x8030CC00);
    func_80043050(0x2C, 0x33, 0x33, &D_E91B50, &D_E91B80, 0x8030C800, &D_E91AE0_2, 0x8030CC00);
    func_80043050(0x2A, 0x31, 0x31, &D_E91A50, &D_E91AB0, 0x8030C800, &D_E919A0_2, 0x8030CC00);
    func_80043050(0x2B, 0x32, 0x32, &D_E91AD0, &D_E91AE0, 0x8030C800, &D_E91AB0_2, 0x8030CC00);
    func_80043050(0x31, 0x37, 0x37, &D_E91EF0, &D_E91F60, 0x8030C800, &D_E91E00_2, 0x8030CC00);
    func_80043050(0x37, 0x41, 0x41, &D_E92380, &D_E92390, 0x8030C800, &D_E92370_2, 0x8030CC00);
    func_80043050(0x2D, 0x34, 0x34, &D_E91B90, &D_E91BA0, 0x8030C800, &D_E91B80_2, 0x8030CC00);
    func_80043050(0x3B, 0x38, 0x38, &D_E92600, &D_E92610, 0x8030C800, &D_E925F0_2, 0x8030CC00);
    func_80043050(0x3A, 0xA, 0xA, &D_E925B0, &D_E925F0, 0x8030C800, &D_E92570_2, 0x8030CC00);
}
*/


//INCLUDE_ASM(const s32, "initialize", setGameVariables);

void setGameVariables(void) {

    // alcohol tolerance is first lol
    setGameVariable(0, &gAlcoholTolerance, 1, 0xFF);

    setGameVariable(1, &gSeason, 1, 4);
    setGameVariable(2, &gHour, 1, 23);
    setGameVariable(3, &gWeather, 1, 5);
    setGameVariable(4, &gDayOfWeek, 1, 6);
    setGameVariable(5, &gYear, 1, 99);
    
    setGameVariable(6, &gItemBeingHeld, 1, TOTAL_ITEMS);

    setGameVariable(7, &npcAffection[MARIA], 1, MAX_AFFECTION);
    setGameVariable(8, &npcAffection[POPURI], 1, MAX_AFFECTION);
    setGameVariable(9, &npcAffection[ELLI], 1, MAX_AFFECTION);
    setGameVariable(0xA, &npcAffection[ANN], 1, MAX_AFFECTION);
    setGameVariable(0xB, &npcAffection[KAREN], 1, MAX_AFFECTION);
    setGameVariable(0xC, &npcAffection[HARRIS], 1, MAX_AFFECTION);
    setGameVariable(0xD, &npcAffection[GRAY], 1, MAX_AFFECTION);
    setGameVariable(0xE, &npcAffection[JEFF], 1, MAX_AFFECTION);
    setGameVariable(0xF, &npcAffection[CLIFF], 1, MAX_AFFECTION);
    setGameVariable(0x10, &npcAffection[KAI], 1, MAX_AFFECTION);
    setGameVariable(0x11, &npcAffection[MAYOR], 1, MAX_AFFECTION);
    setGameVariable(0x12, &npcAffection[MAYOR_WIFE], 1, MAX_AFFECTION);
    setGameVariable(0x13, &npcAffection[LILLIA], 1, MAX_AFFECTION);
    setGameVariable(0x14, &npcAffection[BASIL], 1, MAX_AFFECTION);
    setGameVariable(0x15, &npcAffection[ELLEN], 1, MAX_AFFECTION);

    setGameVariable(0x16, &npcAffection[PASTOR], 1, MAX_AFFECTION);
    setGameVariable(0x17, &npcAffection[RICK], 1, MAX_AFFECTION);
    setGameVariable(0x18, &npcAffection[SAIBARA], 1, MAX_AFFECTION);
    setGameVariable(0x19, &npcAffection[POTION_SHOP_DEALER], 1, MAX_AFFECTION);
    setGameVariable(0x1A, &npcAffection[KENT], 1, MAX_AFFECTION);
    setGameVariable(0x1B, &npcAffection[STU], 1, MAX_AFFECTION);
    setGameVariable(0x1C, &npcAffection[MIDWIFE], 1, MAX_AFFECTION);
    setGameVariable(0x1D, &npcAffection[MAY], 1, MAX_AFFECTION);
    setGameVariable(0x1E, &npcAffection[CARPENTER_1], 1, MAX_AFFECTION);
    setGameVariable(0x1F, &npcAffection[CARPENTER_2], 1, MAX_AFFECTION);
    setGameVariable(0x20, &npcAffection[MASTER_CARPENTER], 1, MAX_AFFECTION);
    setGameVariable(0x21, &npcAffection[HARVEST_SPRITE_1], 1, MAX_AFFECTION);
    setGameVariable(0x22, &npcAffection[SYDNEY], 1, MAX_AFFECTION);
    setGameVariable(0x23, &npcAffection[BARLEY], 1, MAX_AFFECTION);
    setGameVariable(0x24, &npcAffection[GREG], 1, MAX_AFFECTION);
    setGameVariable(0x25, &npcAffection[BABY], 1, MAX_AFFECTION);

    // sum of girls' affection
    setGameVariable(0x26, &D_80215DF0, 2, 1275);

    setGameVariable(0x27, &gDayOfMonth, 1, 30);

    setGameVariable(0x28, &gGold, 4, MAX_GOLD);

    setGameVariable(0x29, &gTotalGrassTiles, 2, 1000);
    setGameVariable(0x2A, &gTotalPinkCatMintFlowersGrowing, 2, 1000);
    setGameVariable(0x2B, &D_801654F4, 4, 999);
    setGameVariable(0x2C, &D_80237414, 4, 999);
    setGameVariable(0x2D, &D_801C3F80, 4, 999);
    setGameVariable(0x2E, &D_80188F60, 4, 999);
    setGameVariable(0x2F, &D_801FB5D0, 4, 999);
    setGameVariable(0x30, &D_801FB6FC, 4, 999);

    setGameVariable(0x31, &npcAffection[DOUG], 1, MAX_AFFECTION);
    setGameVariable(0x32, &npcAffection[GOTZ], 1, MAX_AFFECTION);
    setGameVariable(0x33, &npcAffection[SASHA], 1, MAX_AFFECTION);
    setGameVariable(0x34, &npcAffection[SHIPPER], 1, MAX_AFFECTION);
    setGameVariable(0x35, &npcAffection[DUKE], 1, MAX_AFFECTION);

    setGameVariable(0x36, &horseInfo.affection, 1, MAX_AFFECTION);
    
    // cow status
    setGameVariable(0x37, &gFarmAnimals[0].affection, 1, MAX_AFFECTION);

    setGameVariable(0x38, &dogInfo.affection, 1, MAX_AFFECTION);

    setGameVariable(0x39, &gElliGrievingCounter, 1, 30);

    // set for each possible wife
    setGameVariable(0x3A, &gBabyAge, 1, 120);
    setGameVariable(0x3B, &gWifeConceptionCounter, 1, 120);
    setGameVariable(0x3C, &gWifeConceptionCounter, 1, 120);
    setGameVariable(0x3D, &gBabyAge, 1, 120);
    setGameVariable(0x3E, &D_801890D8, 1, 30);
    setGameVariable(0x3F, &gBabyAge, 1, 120);
    setGameVariable(0x40, &D_801C4214, 1, 30);
    setGameVariable(0x41, &gBabyAge, 1, 120);
    setGameVariable(0x42, &gBabyAge, 1, 120);
    setGameVariable(0x43, &D_80189A48, 1, 30);
    setGameVariable(0x44, &D_80180712, 1, 30);
    setGameVariable(0x45, &gWifePregnancyCounter, 1, 60);
    setGameVariable(0x46, &gWifePregnancyCounter, 1, 60);
    // unused
    setGameVariable(0x47, &D_8017044B, 1, 0xFF);
    setGameVariable(0x48, &D_801C3B64, 1, 0xFF);
    setGameVariable(0x49, &gWifePregnancyCounter, 1, 60);
    setGameVariable(0x4A, &gWifePregnancyCounter, 1, 60);
    setGameVariable(0x4B, &gWifePregnancyCounter, 1, 60);
}