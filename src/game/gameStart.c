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

//INCLUDE_ASM("asm/nonmatchings/game/gameStart", setupGameStart);

void setupGameStart(void) {
    
    // set up game
    initializeAll(); 
    
    if (contPattern & 1) {
        launchIntroCutscene(OPENING_LOGOS, 0x61, 1);
        // startGame();
    } else {

        setMainLoopCallbackFunctionIndex(NO_OP);
        setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
        setMessageBoxSpriteIndices(0, 1, 0, 0);
        setMessageBoxInterpolationWithFlags(0, -4, 2);

        // load "no controller" dialogue
        initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, TEXT_1_TEXT_INDEX, 25, MESSAGE_BOX_MODE_NO_INPUT);

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/gameStart", startGame);

void startGame(void) {

    setSpawnPoint(HOUSE_SPAWN_POINT_3);

    // set spawnable/foragable items
    func_800598E0();
    func_8004DFF8();

    setDailyEventBit(0x53);
    setDailyEventBit(0x54);

    gHour = 6;

    setMainLoopCallbackFunctionIndex(MAP_LOAD);
    
}

u8 i;

//INCLUDE_ASM("asm/nonmatchings/game/gameStart", func_8004DFF8);

// unused function: probably for sandboxing during development or demos
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
//     setLifeEventBit(MAYOR_TOUR);
     setLifeEventBit(PAINTING_FROM_MARIA);
//     setLifeEventBit(BASIL_IN_TOWN);
//     setLifeEventBit(CLIFF_ARRIVED);
     setLifeEventBit(BRIDGE_COMPLETED);
//     setLifeEventBit(0x46);
//     setLifeEventBit(0x47);
//     setLifeEventBit(0x58);
//     setLifeEventBit(0x59);
//     setLifeEventBit(0x5A);
//     setLifeEventBit(WON_CALENDAR_STICKERS_FROM_RAFFLE);
//     setLifeEventBit(0x5C);
//     setLifeEventBit(0x5D);
    
//     setSpecialDialogueBit(0x25);

    gGold = 100000;
    
    gPlayer.belongingsSlots[0] = EGG_HELD_ITEM;
    gPlayer.belongingsSlots[1] = GOLDEN_EGG_HELD_ITEM;
    gPlayer.belongingsSlots[2] = GOLDEN_MILK;

    chickenFeedQuantity = 100;
    fodderQuantity = 100;
    
    for (i = 0; i < MAX_CHICKENS; i++) {
        if (!(gChickens[i].flags & CHICKEN_ACTIVE)) {
            gChickens[i].flags = CHICKEN_ACTIVE | CHICKEN_ENTITY_LOADED;
            gChickens[i].type = ADULT_CHICKEN;
            gChickens[i].affection = 222;
            gChickens[i].location = COOP;
            gChickens[i].coordinates.x = 0.0f;
            gChickens[i].coordinates.y = 0.0f;
            gChickens[i].coordinates.z = 0.0f;
            initializeChickenEntity(i);
            if(i == 1) break;
        }
    }
    
     initializeHorse();
    
     horseInfo.grown = TRUE;

     // setLifeEventBit(MARRIED);
     // setLifeEventBit(WIFE_PREGNANT);
    
     // gWife = KAREN;
    
//     npcAffection[POPURI] = MAX_AFFECTION;
    
//     gSeason = WINTER;
//     gDayOfMonth = 18;
    
//     dogInfo.affection = MAX_AFFECTION;
    
//     gHour = 8;
    
//     // max recipes
//     recipesBits[0] = -1;
    
//     // unused game variable
//     D_8016FB00 = 7;
    
}
