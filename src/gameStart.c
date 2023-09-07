#include "common.h"

#include "system/controller.h"
#include "system/message.h"

#include "animals.h"
#include "game.h"
#include "gameStatus.h"
#include "initialize.h"
#include "initialize2.h"
#include "level.h"
#include "load.h"
#include "mainLoop.h"
#include "npc.h"
#include "player.h"
#include "setCutscenes.h"

// possible bss
extern u32 D_8016FB00;

//INCLUDE_ASM(const s32, "gameStart", func_8004DF10);

void func_8004DF10(void) {
    
    // set up game
    func_8004E160(); 
    
    if (contPattern & 1) {
        func_80055F08(INTRO, 0x61, 1);
        return;
    }
    
    // set to no op
    setMainLoopCallbackFunctionIndex(0);

    // set position floats
    func_8003F54C(0, 0, -64.0f, 352.0f);

    // dialogue box flags
    func_8003F690(0, 1, 0, 0);

    // dialogue box flags
    func_8003F360(0, 0xFFFC, 2);

    // load "no controller" dialogue
    func_8003DDF8(0, 0, 0x19, 0x80000);

}

//INCLUDE_ASM(const s32, "gameStart", startGame);

void startGame(void) {

    setEntrance(MIDDLE_OF_HOUSE);

    // set spawnable/foragable items
    func_800598E0();

    setDailyEventBit(0x53);
    setDailyEventBit(0x54);

    gHour = 6;

    setMainLoopCallbackFunctionIndex(2);
}

//INCLUDE_ASM(const s32, "gameStart", func_8004DFF8);

// unused function: probably for sandboxing during development
void func_8004DFF8(void) {
    
    setLifeEventBit(HAVE_HORSE);
    setLifeEventBit(0x10);
    setLifeEventBit(0x11);
    setLifeEventBit(0x12);
    setLifeEventBit(0x13);
    setLifeEventBit(0x14);
    setLifeEventBit(0x15);
    setLifeEventBit(0x16);
    setLifeEventBit(0x17);
    setLifeEventBit(0x3E);
    setLifeEventBit(0x3F);
    setLifeEventBit(0x40);
    setLifeEventBit(0x41);
    setLifeEventBit(0x48);
    setLifeEventBit(0x46);
    setLifeEventBit(0x47);
    setLifeEventBit(0x58);
    setLifeEventBit(0x59);
    setLifeEventBit(0x5A);
    setLifeEventBit(0x5B);
    setLifeEventBit(0x5C);
    setLifeEventBit(0x5D);
    
    setSpecialDialogueBit(0x25);
    
    gPlayer.belongingsSlots[0] = TOMATO;
    
    initializeHorse();
    
    horseInfo.grown = TRUE;

    setLifeEventBit(MARRIED);
    setLifeEventBit(WIFE_PREGNANT);
    
    gWife = POPURI;
    
    npcAffection[POPURI] = MAX_AFFECTION;
    
    gSeason = WINTER;
    gDayOfMonth = 18;
    
    dogInfo.affection = MAX_AFFECTION;
    
    gHour = 8;
    
    // max recipes
    recipesBits[0] = -1;
    
    // unused game variable
    D_8016FB00 = 7;
    
}

void func_8004E154(void) {}
