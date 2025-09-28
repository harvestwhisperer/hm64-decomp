#include "common.h"

#include "system/controller.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/initialize.h"
#include "game/transition.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "game/load.h"
#include "mainLoop.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/setCutscenes.h"

// possible bss
extern u32 D_8016FB00;

//INCLUDE_ASM("asm/nonmatchings/game/gameStart", func_8004DF10);

void func_8004DF10(void) {
    
    // set up game
    func_8004E160(); 
    
    if (contPattern & 1) {
        
        func_80055F08(INTRO, 0x61, 1);

    } else {

        setMainLoopCallbackFunctionIndex(NO_OP);
        setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
        setMessageBoxSpriteIndices(0, 1, 0, 0);
        func_8003F360(0, -4, 2);

        // load "no controller" dialogue
        initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, 0, 0x19, 0x80000);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStart", startGame);

void startGame(void) {

    setEntrance(MIDDLE_OF_HOUSE);

    // set spawnable/foragable items
    func_800598E0();

    setDailyEventBit(0x53);
    setDailyEventBit(0x54);

    gHour = 6;

    setMainLoopCallbackFunctionIndex(MAP_LOAD);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStart", func_8004DFF8);

// unused function: probably for sandboxing during development
// not in JP version
void func_8004DFF8(void) {
    
    setLifeEventBit(HAVE_HORSE);
    setLifeEventBit(HAVE_KITCHEN);
    setLifeEventBit(HAVE_BATHROOM);
    setLifeEventBit(HAVE_STAIRS);
    setLifeEventBit(HAVE_GREENHOUSE);
    setLifeEventBit(HAVE_LOG_TERRACE);
    setLifeEventBit(HAVE_BABY_BED);
    setLifeEventBit(HAVE_CABINET);
    setLifeEventBit(HAVE_TABLECLOTH);
    setLifeEventBit(MAYOR_TOUR);
    setLifeEventBit(PAINTING_FROM_MARIA);
    setLifeEventBit(BASIL_IN_TOWN);
    setLifeEventBit(CLIFF_ARRIVED);
    setLifeEventBit(BRIDGE_COMPLETED);
    setLifeEventBit(0x46);
    setLifeEventBit(0x47);
    setLifeEventBit(0x58);
    setLifeEventBit(0x59);
    setLifeEventBit(0x5A);
    setLifeEventBit(WON_CALENDAR_STICKERS_FROM_RAFFLE);
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

// empty function
void func_8004E154(void) {}
