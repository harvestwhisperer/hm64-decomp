#include "common.h"
#include "animals.h"
#include "npc.h"

void registerMainLoopCallback(u8, void (*func)(void)); 

extern void mainGameLoopCallback;

extern void func_8005C00C;
extern void func_8005CA64;
extern void func_8005CAA8;
extern void func_8005CB50;
extern void func_8005CBA4;
extern void func_8005CBF0;
extern void func_8005CEFC;
extern void func_8005CF4C;
extern void func_8005CF94;
extern void func_8005D0BC;
extern void func_8005D2B0;
extern void func_80060454;
extern void func_80060490;
extern void func_800604B0;
extern void func_800605F0;
extern void func_80060624;
extern void func_80060838;
extern void func_800657B4;
extern void func_800657C4;
extern void func_800B8018;
extern void func_800B815C;
extern void func_800B82AC;
extern void func_800B83F0;
extern void func_800B8554;
extern void func_800B86B8;
extern void func_800B881C;
extern void func_800B9D3C;
extern void func_800BADD0;
extern void func_800BBEC0;
extern void func_800BCA9C;
extern void func_800BDB24;
extern void func_800BE808;
extern void func_800BF990;
extern void func_800C1124;
extern void func_800C224C;
extern void func_800C2B8C;
extern void func_800C3D20;
extern void func_800C53C0;
extern void func_800C7058;
extern void func_800C8424;
extern void func_800C8784;
extern void func_800C88F4;
extern void func_800CA808;
extern void func_800CC518;
extern void func_800CD750;
extern void func_800CD928;
extern void func_800CE068;
extern void func_800CE930;
extern void func_800CEDF0;
extern void func_800D93CC;
extern void func_800DCAB8;
extern void func_800E00A4;
extern void func_800E1FAC;
extern void func_800E8F08;
extern void func_800ED974;

void func_8002F7C8(u32, u32, u32, u32);
void func_8003BE98(u32, u32, u32, u32, s32);            
void initializeAudio(musConfig*);                            
void func_8004E210();                               
void registerMainLoopCallbacks();                            
void func_8004F768();                          
void func_800527D4();                            
void func_80053088();                                
void func_80054550();                            
void initializeWaveTable(u32);                                
void setMainLoopCallbackFunctionIndex(u16);             

void setGameVariable(u8 index, void* variable, u8, u32 max);

extern u16 D_801654F4;
extern u8 D_8017044B;
extern u8 D_80180712;
extern u16 D_80188F60;
extern u8 D_801890D8;
extern u8 D_80189A48;
extern u8 D_801C3B64;
extern u16 D_801C3F80;
extern u8 D_801C4214;
extern u16 D_801FB5D0;
extern u16 D_801FB6FC;
extern u16 D_80215DF0;
extern u16 D_80237414;

extern u8 gFarmAnimals;

extern u16 gTotalPinkCatMintFlowersGrowing;
extern u16 gTotalGrassTiles;

extern u8 gWifeConceptionCounter;
extern u8 gWifePregnancyCounter;
extern u8 gBabyAge;

extern u8 gAlcoholTolerance;

extern u32 gGold;
extern u8 gItemBeingHeld;

extern u8 gElliGrievingCounter;

extern u8 gHour;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gYear;

extern u8 gWeather;

extern Dog dogInfo;
extern Horse horseInfo;

extern u8 npcAffection[TOTAL_NPCS];

// current game index
extern u8 gCurrentGameIndex; 
extern u8 *sfxList[];
extern int musPriorityList[];


void func_8004E160(void) {
    
    // why volatile?
    volatile musConfig c;
    
    c.fxs = sfxList;
    c.priority = musPriorityList;
    
    initializeAudio(&c);

    // initialize all bss variables and structs
    func_8004E210();

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

//INCLUDE_ASM(const s32, "initialize", func_8004E160);


// initialize bss game variables
INCLUDE_ASM(const s32, "initialize", func_8004E210);

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
    registerMainLoopCallback(TV, &func_800D93CC);
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
    registerMainLoopCallback(TITLE_SCREEN, &func_800E00A4);
    registerMainLoopCallback(NAMING_SCREEN, &func_8005CF94);
    registerMainLoopCallback(0x34, &func_800ED974);
    registerMainLoopCallback(SELECT_GAME, &func_800E1FAC);
    registerMainLoopCallback(ESTIMATE_SCREEN, &func_800E8F08);
    registerMainLoopCallback(0x37, &func_800657B4);
    registerMainLoopCallback(0x38, &func_800657C4);
    
}

INCLUDE_ASM(const s32, "initialize", func_8004F768);

INCLUDE_ASM(const s32, "initialize", func_800527D4);

INCLUDE_ASM(const s32, "initialize", func_80053088);

INCLUDE_ASM(const s32, "initialize", func_800535DC);

INCLUDE_ASM(const s32, "initialize", func_80054180);

INCLUDE_ASM(const s32, "initialize", func_80054550);

INCLUDE_ASM(const s32, "initialize", func_80054734);

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

    setGameVariable(0x26, &D_80215DF0, 2, 1275);

    setGameVariable(0x27, &gDayOfMonth, 1, 30);

    setGameVariable(0x28, &gGold, 4, 999999);

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
    setGameVariable(0x37, &gFarmAnimals, 1, MAX_AFFECTION);

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
    setGameVariable(0x47, &D_8017044B, 1, 0xFF);
    setGameVariable(0x48, &D_801C3B64, 1, 0xFF);
    setGameVariable(0x49, &gWifePregnancyCounter, 1, 60);
    setGameVariable(0x4A, &gWifePregnancyCounter, 1, 60);
    setGameVariable(0x4B, &gWifePregnancyCounter, 1, 60);
}