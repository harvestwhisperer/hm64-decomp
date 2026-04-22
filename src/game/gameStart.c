#include "common.h"

#include "system/controller.h"
#include "system/message.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/initialize.h"
#include "game/items.h"
#include "game/level.h"
#include "game/load.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/time.h"
#include "game/transition.h"

#include "mainLoop.h"

#include "assetIndices/cutscenes.h"
#include "assetIndices/maps.h"

// shared bss
u32 D_8016FB00;

void setupGameStart(void) {
    
    // set up game
    initializeAll(); 
    
    if (contPattern & 1) {
        launchIntroCutscene(OPENING_LOGOS, SQUARE_SPAWN_POINT_1, 1);
    } else {

        setMainLoopCallbackFunctionIndex(NO_OP);
        setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
        setMessageBoxSpriteIndices(0, 1, 0, 0);
        setMessageBoxInterpolationWithFlags(0, -4, 2);

        // load "no controller" dialogue
        initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, TEXT_1_TEXT_INDEX, 25, MESSAGE_BOX_MODE_NO_INPUT);

    }
    
}

void startGame(void) {

    setSpawnPoint(HOUSE_SPAWN_POINT_3);

    // set spawnable/foragable items
    func_800598E0();

    setDailyEventBit(EAT_BREAKFAST);
    setDailyEventBit(DEFAULT_MORNING);

    gHour = 6;

    setMainLoopCallbackFunctionIndex(MAP_LOAD);
    
}

// unused function: probably for sandboxing during development or demos
// not in JP version
/*
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
    setLifeEventBit(HAVE_TURTLE_FIGURINE);
    setLifeEventBit(HAVE_INVINCILE_KATORI);
    setLifeEventBit(HAVE_STUFFED_HORSE);
    setLifeEventBit(HAVE_CUTE_CLOCK);
    setLifeEventBit(HAVE_CUSHION);
    setLifeEventBit(WON_CALENDAR_STICKERS_FROM_RAFFLE);
    setLifeEventBit(HAVE_MUG);
    setLifeEventBit(HAVE_LUNCHEON_MAT);
    
    setSpecialDialogueBit(HARVEST_GODDESS_OFFERING_ADVICE_DIALOGUE);
    
    gPlayer.belongingsSlots[0] = TOMATO_HELD_ITEM;
    
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
*/
