#include "common.h"

#include "game/transition.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/map.h"
#include "system/math.h"
#include "system/message.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameFile.h"
#include "game/gameStatus.h"
#include "game/initialize.h"
#include "game/items.h"
#include "game/level.h"
#include "game/load.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/time.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "buffers/buffers.h"

#include "assetIndices/cutscenes.h"
#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sprites.h"
      
// bss
// unused: likely to show player coordinates on screen for debugging
s16 D_80182D8C;
s16 D_8018A062;
s16 D_80237A20; 

//INCLUDE_ASM("asm/nonmatchings/game/transition", mainGameLoopCallback);

void mainGameLoopCallback(void) {
    
    // for debugging?
    D_8018A062 = (s16)entities[ENTITY_PLAYER].coordinates.x;
    D_80237A20 = (s16)entities[ENTITY_PLAYER].coordinates.y;
    D_80182D8C = (s16)entities[ENTITY_PLAYER].coordinates.z;
    
    if (func_8002FD24(ENTITY_PLAYER) && !(checkEntityPaused(ENTITY_PLAYER))) {

        // increment clock and handle audio changes
        func_800D7C20();
        // handle button input and player animation
        func_8006623C();
        setNpcAnimations();
        updateAnimals();
        handlePlayerAnimation();

    }

    // handle cutscene completion
    func_800A8F74();

    // dead code
    checkButtonPressed(CONTROLLER_1, BUTTON_C_RIGHT);
    checkButtonPressed(CONTROLLER_1, BUTTON_C_DOWN);
    checkButtonPressed(CONTROLLER_1, BUTTON_Z);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/transition", func_80055F08);

// setup title/demo/non game cutscenes
// used by game.c
inline void func_80055F08(u16 cutsceneIndex, u16 entranceIndex, u8 arg2) {
    
    deactivateSprites();
    deactivateGlobalSprites();
    initializeMessageBoxes();
    initializeCutsceneExecutors();
    func_80053088();
    
    resetGlobalLighting();

    setEntitiesColor(0, 0, 0, 0);
    func_8003BE98(MAIN_MAP_INDEX, 0, 0, 0, 0);

    gHour = 12;
    
    initializeEntityInstances(arg2);

    setEntrance(entranceIndex);

    gCutsceneIndex = cutsceneIndex;

    // trigger cutscene and load cutscene assets
    loadCutscene(0);
    
    // set up map/level
    func_8006E840(gEntranceIndex);
 
    setupPlayerEntity(gEntranceIndex, FALSE);
    handlePlayerAnimation();

    setMainLoopCallbackFunctionIndex(4);

}

//INCLUDE_ASM("asm/nonmatchings/game/transition", func_80056030);

// arg0 = entering new location
// called by map load callback, end of day callback, cutscene completion
void func_80056030(u8 arg0) {

    u8 mapIndex;
    u32 *ptr;

    gCutsceneCompletionFlags = 0;
    gCutsceneIndex = 0;
    gCutsceneFlags = 0;    
    
    toggleDailyEventBit(0x28);
    toggleDailyEventBit(0x4B);

    func_8005CDCC();
    func_80065F5C();
    updateAnimalCoordinates();
    updateNPCCoordinates();
    deactivateSprites();
    deactivateGlobalSprites();
    initializeCutsceneExecutors();
    deactivateNPCEntities();
    deactivateAnimalEntities();
    func_800D51B0();
    
    if (!checkDailyEventBit(0x2F)) {
        func_800A7DFC();
    }

    setDailyEventBit(0x2F);
    
    mapIndex = getMapFromExit(gEntranceIndex);
    func_8006E840(gEntranceIndex);

    if ((mapIndex == FARM || mapIndex == BARN) == 0 && mapIndex != COOP) {

        switch (gCutsceneIndex) {

            case SQUARE_FIREWORKS:
            case SOWING_FESTIVAL_POTION_SHOP_DEALER ... SOWING_FESTIVAL_HARRIS:
            case HORSE_RACE_SQUARE ... HORSE_RACE_STARTING_ANNOUNCEMENT:
            case FLOWER_FESTIVAL:
            case VEGETABLE_FESTIVAL_SQUARE:
            case FIREFLY_FESTIVAL:
            case HARVEST_FESTIVAL:
            case SEA_FESTIVAL:
            case EGG_FESTIVAL:
            case DOG_RACE_SQUARE ... DOG_RACE_AFTER_RACE:
            // spirit festival
            case 0x546 ... 0x548:
            case NEW_YEAR_FESTIVAL:
                initializeEntityInstances(2);
                break;            

            default:

                switch (gCutsceneIndex) {

                    case COW_FESTIVAL:
                        initializeEntityInstances(3);
                        break;
                    case HARVEST_FESTIVAL:
                    case SEA_FESTIVAL:
                    default:
                        initializeEntityInstances(1);
                        break;

                }

                break;

        }
        
    } else {
        initializeEntityInstances(0);
    }

   if (checkDailyEventBit(0x54)) {

        if (checkDailyEventBit(0x53)) {
            setPlayerAction(6, 8);
            gPlayer.heldItem = RICE_CAKE;
        } else {
            setPlayerAction(0, 0);
            gPlayer.heldItem = 0;
        }

        if (!(gCutsceneFlags & 1) && func_8009B5E0()) {
            playSfx(63);
        }

    }
    
    toggleDailyEventBit(0x53);
    toggleDailyEventBit(0x54);
    
    setupPlayerEntity(gEntranceIndex, arg0);
    
    func_80065AA0();
    func_800D5290();

    if (arg0 != 2) {

        ptr = &gCutsceneFlags;
        
        if (!(*ptr & (2 | 4))) {
            
            if (!checkDailyEventBit(0x4D)) {
                func_80088D54();
            }

            if (!(*ptr & (2 | 4))) {
                func_8005AAE4();
                // set NPC entity indices
                setupActiveNPCs();
            }
        }

        if (!(gCutsceneFlags & 4)) {
            func_800876D0();
        }

    }
    
    handlePlayerAnimation();
    // handle item state
    func_800D7010();
    // handle tool use
    func_800D0318();

    setEntitiesColor(0, 0, 0, 0);
    func_8003BE98(MAIN_MAP_INDEX, 0, 0, 0, 0);
    
    unknownRGBA.r = 0;
    unknownRGBA.g = 0;
    unknownRGBA.b = 0;
    unknownRGBA.a = 0;
    
}


//INCLUDE_ASM("asm/nonmatchings/game/transition", initializeEntityInstances);

void initializeEntityInstances(u8 arg0) {

    switch (arg0) {       
        
        // farm or barn, how to play cutscene
        case 0:
            
            initializeEntity(ENTITY_PLAYER, 0, 0x31, (u8*)PLAYER_TEXTURE_1_BUFFER, (u8*)PLAYER_TEXTURE_2_BUFFER, (u16*)PLAYER_PALETTE_BUFFER, (AnimationFrameMetadata*)PLAYER_ANIMATION_METADATA_BUFFER, (u32*)PLAYER_SPRITESHEET_INDEX_BUFFER, (u32*)PLAYER_TEXTURE_TO_PALETTE_LOOKUP_BUFFER);            
            initializeEntity(1, 1, 0x32, (void*)0x8026F000, (void*)0x8026F908, (void*)0x80270210, (void*)0x80270410, (void*)0x80272000, (void*)0x80272200);
            initializeEntity(2, 2, 0x33, (void*)0x80272800, (void*)0x80272C80, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(3, 3, 0x34, (void*)0x80273100, (void*)0x80273580, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(4, 4, 0x35, (void*)0x80273A00, (void*)0x80273E80, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(5, 5, 0x36, (void*)0x80274300, (void*)0x80274780, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(6, 6, 0x37, (void*)0x80274C00, (void*)0x80275080, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(7, 7, 0x38, (void*)0x80275500, (void*)0x80275980, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(8, 8, 0x39, (void*)0x80277300, (void*)0x80277390, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(9, 9, 0x3A, (void*)0x80277420, (void*)0x802774B0, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xA, 0xA, 0x3B, (void*)0x80277540, (void*)0x802775D0, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xB, 0xB, 0x3C, (void*)0x80277660, (void*)0x802776F0, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xC, 0xC, 0x3D, (void*)0x80277780, (void*)0x80277810, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xD, 0xD, 0x3E, (void*)0x802778A0, (void*)0x80277930, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xE, 0xE, 0x3F, (void*)0x8027ADC0, (void*)0x8027B848, (void*)0x80284130, (void*)0x80284230, (void*)0x802852B0, (void*)0x802854B0);
            initializeEntity(0xF, 0xF, 0x40, (void*)0x8027C2D0, (void*)0x8027CD58, (void*)0x80285930, (void*)0x80285A30, (void*)0x80286AB0, (void*)0x80286CB0);
            initializeEntity(0x10, 0x10, 0x41, (void*)0x8027D7E0, (void*)0x8027E268, (void*)0x80287130, (void*)0x80287230, (void*)0x802882B0, (void*)0x802884B0);
            initializeEntity(0x11, 0x11, 0x42, (void*)0x8027ECF0, (void*)0x8027F778, (void*)0x80288930, (void*)0x80288A30, (void*)0x80289AB0, (void*)0x80289CB0);
            initializeEntity(0x12, 0x12, 0x43, (void*)0x80280200, (void*)0x80280C88, (void*)0x8028A130, (void*)0x8028A230, (void*)0x8028B2B0, (void*)0x8028B4B0);
            initializeEntity(0x13, 0x13, 0x44, (void*)0x80281710, (void*)0x80282198, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x14, 0x14, 0x45, (void*)0x80282C20, (void*)0x802836A8, (void*)0x80270210, (void*)0x80270410, (void*)0x80272000, (void*)0x80272200);
            
            initializeEntity(0x27, 0x15, 0x46, (void*)0x8028B930, (void*)0x8028C730, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x28, 0x16, 0x47, (void*)0x80294250, (void*)0x80295050, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x29, 0x17, 0x48, (void*)0x80295E50, (void*)0x80296C50, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2A, 0x18, 0x49, (void*)0x80297A50, (void*)0x80298850, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2B, 0x19, 0x4A, (void*)0x80299650, (void*)0x8029A450, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2C, 0x1A, 0x4B, (void*)0x8029B250, (void*)0x8029C050, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2D, 0x1B, 0x4C, (void*)0x8029CE50, (void*)0x8029DC50, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2E, 0x1C, 0x4D, (void*)0x8029EA50, (void*)0x8029F850, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2F, 0x1D, 0x4E, (void*)0x802A0650, (void*)0x802A1450, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x30, 0x1E, 0x4F, (void*)0x802A2250, (void*)0x802A3050, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            
            initializeEntity(0x15, 0x1F, 0x50, (void*)0x802A60C0, (void*)0x802A6B48, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x16, 0x20, 0x51, (void*)0x802A75D0, (void*)0x802A8058, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x17, 0x21, 0x52, (void*)0x802A8AE0, (void*)0x802A9568, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x18, 0x22, 0x53, (void*)0x802A9FF0, (void*)0x802AAA78, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x19, 0x23, 0x54, (void*)0x802AB500, (void*)0x802ABF88, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x1A, 0x24, 0x55, (void*)0x802ACA10, (void*)0x802AD498, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x1B, 0x25, 0x56, (void*)0x802ADF20, (void*)0x802AE9A8, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x1C, 0x26, 0x57, (void*)0x802AF430, (void*)0x802AFEB8, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            
            initializeEntity(0x26, 0x27, 0x58, (void*)0x802B6640, (void*)0x802B7648, (void*)0x802B8650, (void*)0x802B8F40, (void*)0x802B9B40, (void*)0x802B9C40);
            
            initializeEntity(0x1D, 0x28, 0x59, (void*)0x802CA7C0, (void*)0x802CB440, (void*)0x802CC0C0, (void*)0x802CCB40, (void*)0x802CD940, (void*)0x802CDA40);
            initializeEntity(0x1E, 0x29, 0x5A, (void*)0x802CDCC0, (void*)0x802CE940, (void*)0x802CF5C0, (void*)0x802D0040, (void*)0x802D0E40, (void*)0x802D0F40);
            initializeEntity(0x1F, 0x2A, 0x5B, (void*)0x802D11C0, (void*)0x802D1E40, (void*)0x802D2AC0, (void*)0x802D3540, (void*)0x802D4340, (void*)0x802D4440);
            initializeEntity(0x20, 0x2B, 0x5C, (void*)0x802D46C0, (void*)0x802D5340, (void*)0x802D5FC0, (void*)0x802D6A40, (void*)0x802D7840, (void*)0x802D7940);
            initializeEntity(0x21, 0x2C, 0x5D, (void*)0x802D7BC0, (void*)0x802D8840, (void*)0x802D94C0, (void*)0x802D9F40, (void*)0x802DAD40, (void*)0x802DAE40);
            initializeEntity(0x22, 0x2D, 0x5E, (void*)0x802DB0C0, (void*)0x802DBD40, (void*)0x802DC9C0, (void*)0x802DD440, (void*)0x802DE240, (void*)0x802DE340);
            initializeEntity(0x23, 0x2E, 0x5F, (void*)0x802C08C0, (void*)0x802C1540, (void*)0x802C21C0, (void*)0x802C2C40, (void*)0x802C3A40, (void*)0x802C3B40);
            initializeEntity(0x24, 0x2F, 0x60, (void*)0x802C3DC0, (void*)0x802C4A40, (void*)0x802C56C0, (void*)0x802C6140, (void*)0x802C6F40, (void*)0x802C7040);
            initializeEntity(0x25, 0x30, 0x61, (void*)0x802C72C0, (void*)0x802C7F40, (void*)0x802C8BC0, (void*)0x802C9640, (void*)0x802CA440, (void*)0x802CA540);
            
            break;
        
        // default, opening, demo cutscenes
        case 1:
            
            initializeEntity(ENTITY_PLAYER, 0, 0x31, (void*)PLAYER_TEXTURE_1_BUFFER, (void*)0x80243B00, (void*)PLAYER_PALETTE_BUFFER, (void*)PLAYER_ANIMATION_METADATA_BUFFER, (void*)PLAYER_SPRITESHEET_INDEX_BUFFER, (void*)PLAYER_TEXTURE_TO_PALETTE_LOOKUP_BUFFER);
            initializeEntity(1, 1, 0x32, (void*)0x8026F000, (void*)0x8026F908, (void*)0x80270210, (void*)0x80270410, (void*)0x80272000, (void*)0x80272200);
            initializeEntity(2, 2, 0x33, (void*)0x80272800, (void*)0x80272C80, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(3, 3, 0x34, (void*)0x80273100, (void*)0x80273580, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(4, 4, 0x35, (void*)0x80273A00, (void*)0x80273E80, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(5, 5, 0x36, (void*)0x80274300, (void*)0x80274780, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(6, 6, 0x37, (void*)0x80274C00, (void*)0x80275080, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(7, 7, 0x38, (void*)0x80275500, (void*)0x80275980, (void*)0x80275E00, (void*)0x80275F00, (void*)0x80276700, (void*)0x80276800);
            initializeEntity(8, 8, 0x39, (void*)0x80277300, (void*)0x80277390, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(9, 9, 0x3A, (void*)0x80277420, (void*)0x802774B0, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xA, 0xA, 0x3B, (void*)0x80277540, (void*)0x802775D0, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xB, 0xB, 0x3C, (void*)0x80277660, (void*)0x802776F0, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xC, 0xC, 0x3D, (void*)0x80277780, (void*)0x80277810, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xD, 0xD, 0x3E, (void*)0x802778A0, (void*)0x80277930, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0xE, 0xE, 0x3F, (void*)0x8027ADC0, (void*)0x8027B848, (void*)0x80284130, (void*)0x80284230, (void*)0x802852B0, (void*)0x802854B0);
            initializeEntity(0xF, 0xF, 0x40, (void*)0x8027C2D0, (void*)0x8027CD58, (void*)0x80285930, (void*)0x80285A30, (void*)0x80286AB0, (void*)0x80286CB0);
            initializeEntity(0x10, 0x10, 0x41, (void*)0x8027D7E0, (void*)0x8027E268, (void*)0x80287130, (void*)0x80287230, (void*)0x802882B0, (void*)0x802884B0);
            initializeEntity(0x11, 0x11, 0x42, (void*)0x8027ECF0, (void*)0x8027F778, (void*)0x80288930, (void*)0x80288A30, (void*)0x80289AB0, (void*)0x80289CB0);
            initializeEntity(0x12, 0x12, 0x43, (void*)0x80280200, (void*)0x80280C88, (void*)0x8028A130, (void*)0x8028A230, (void*)0x8028B2B0, (void*)0x8028B4B0);
            initializeEntity(0x13, 0x13, 0x44, (void*)0x80281710, (void*)0x80282198, (void*)0x802779C0, (void*)0x80277AC0, (void*)0x8027A6C0, (void*)0x8027A8C0);
            initializeEntity(0x14, 0x14, 0x45, (void*)0x80282C20, (void*)0x802836A8, (void*)0x80270210, (void*)0x80270410, (void*)0x80272000, (void*)0x80272200);
            
            initializeEntity(0x27, 0x15, 0x46, (void*)0x8028B930, (void*)0x8028C730, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x28, 0x16, 0x47, (void*)0x80294250, (void*)0x80295050, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x29, 0x17, 0x48, (void*)0x80295E50, (void*)0x80296C50, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2A, 0x18, 0x49, (void*)0x80297A50, (void*)0x80298850, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2B, 0x19, 0x4A, (void*)0x80299650, (void*)0x8029A450, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2C, 0x1A, 0x4B, (void*)0x8029B250, (void*)0x8029C050, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2D, 0x1B, 0x4C, (void*)0x8029CE50, (void*)0x8029DC50, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2E, 0x1C, 0x4D, (void*)0x8029EA50, (void*)0x8029F850, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x2F, 0x1D, 0x4E, (void*)0x802A0650, (void*)0x802A1450, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            initializeEntity(0x30, 0x1E, 0x4F, (void*)0x802A2250, (void*)0x802A3050, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            
            initializeEntity(0x15, 0x1F, 0x50, (void*)0x802A60C0, (void*)0x802A6D40, (void*)0x802A79C0, (void*)0x802A8440, (void*)0x802A9240, (void*)0x802A9340);
            initializeEntity(0x16, 0x20, 0x51, (void*)0x802A95C0, (void*)0x802AA240, (void*)0x802AAEC0, (void*)0x802AB940, (void*)0x802AC740, (void*)0x802AC840);
            initializeEntity(0x17, 0x21, 0x52, (void*)0x802ACAC0, (void*)0x802AD740, (void*)0x802AE3C0, (void*)0x802AEE40, (void*)0x802AFC40, (void*)0x802AFD40);
            initializeEntity(0x18, 0x22, 0x53, (void*)0x802AFFC0, (void*)0x802B0C40, (void*)0x802B18C0, (void*)0x802B2340, (void*)0x802B3140, (void*)0x802B3240);
            initializeEntity(0x19, 0x23, 0x54, (void*)0x802B34C0, (void*)0x802B4140, (void*)0x802B4DC0, (void*)0x802B5840, (void*)0x802B6640, (void*)0x802B6740);
            initializeEntity(0x1A, 0x24, 0x55, (void*)0x802B69C0, (void*)0x802B7640, (void*)0x802B82C0, (void*)0x802B8D40, (void*)0x802B9B40, (void*)0x802B9C40);
            initializeEntity(0x1B, 0x25, 0x56, (void*)0x802B9EC0, (void*)0x802BAB40, (void*)0x802BB7C0, (void*)0x802BC240, (void*)0x802BD040, (void*)0x802BD140);
            initializeEntity(0x1C, 0x26, 0x57, (void*)0x802BD3C0, (void*)0x802BE040, (void*)0x802BECC0, (void*)0x802BF740, (void*)0x802C0540, (void*)0x802C0640);
            initializeEntity(0x1D, 0x28, 0x59, (void*)0x802CA7C0, (void*)0x802CB440, (void*)0x802CC0C0, (void*)0x802CCB40, (void*)0x802CD940, (void*)0x802CDA40);
            initializeEntity(0x1E, 0x29, 0x5A, (void*)0x802CDCC0, (void*)0x802CE940, (void*)0x802CF5C0, (void*)0x802D0040, (void*)0x802D0E40, (void*)0x802D0F40);
            initializeEntity(0x1F, 0x2A, 0x5B, (void*)0x802D11C0, (void*)0x802D1E40, (void*)0x802D2AC0, (void*)0x802D3540, (void*)0x802D4340, (void*)0x802D4440);
            initializeEntity(0x20, 0x2B, 0x5C, (void*)0x802D46C0, (void*)0x802D5340, (void*)0x802D5FC0, (void*)0x802D6A40, (void*)0x802D7840, (void*)0x802D7940);
            initializeEntity(0x21, 0x2C, 0x5D, (void*)0x802D7BC0, (void*)0x802D8840, (void*)0x802D94C0, (void*)0x802D9F40, (void*)0x802DAD40, (void*)0x802DAE40);
            initializeEntity(0x22, 0x2D, 0x5E, (void*)0x802DB0C0, (void*)0x802DBD40, (void*)0x802DC9C0, (void*)0x802DD440, (void*)0x802DE240, (void*)0x802DE340);
            initializeEntity(0x23, 0x2E, 0x5F, (void*)0x802C08C0, (void*)0x802C1540, (void*)0x802C21C0, (void*)0x802C2C40, (void*)0x802C3A40, (void*)0x802C3B40);
            initializeEntity(0x24, 0x2F, 0x60, (void*)0x802C3DC0, (void*)0x802C4A40, (void*)0x802C56C0, (void*)0x802C6140, (void*)0x802C6F40, (void*)0x802C7040);
            initializeEntity(0x25, 0x30, 0x61, (void*)0x802C72C0, (void*)0x802C7F40, (void*)0x802C8BC0, (void*)0x802C9640, (void*)0x802CA440, (void*)0x802CA540);
            
            break;
        
        // new year's festival
        case 2:
            
            initializeEntity(ENTITY_PLAYER, 0, 0x31, (void*)PLAYER_TEXTURE_1_BUFFER, (void*)0x80243B00, (void*)PLAYER_PALETTE_BUFFER, (void*)PLAYER_ANIMATION_METADATA_BUFFER, (void*)PLAYER_SPRITESHEET_INDEX_BUFFER, (void*)PLAYER_TEXTURE_TO_PALETTE_LOOKUP_BUFFER);
            
            initializeEntity(0x27, 0x15, 0x46, (void*)0x8028B930, (void*)0x8028C730, (void*)0x8028DD50, (void*)0x80290550, (void*)0x80293A50, (void*)0x80293C50);
            
            initializeEntity(1, 1, 0x2C, (void*)0x8026F000, (void*)0x8026FC80, (void*)0x80270900, (void*)0x80271380, (void*)0x80272180, (void*)0x80272280);
            initializeEntity(2, 2, 0x2D, (void*)0x80272500, (void*)0x80273180, (void*)0x80273E00, (void*)0x80274880, (void*)0x80275680, (void*)0x80275780);
            initializeEntity(3, 3, 0x2E, (void*)0x80275A00, (void*)0x80276680, (void*)0x80277300, (void*)0x80277D80, (void*)0x80278B80, (void*)0x80278C80);
            initializeEntity(4, 4, 0x2F, (void*)0x80278F00, (void*)0x80279B80, (void*)0x8027A800, (void*)0x8027B280, (void*)0x8027C080, (void*)0x8027C180);
            initializeEntity(5, 5, 0x30, (void*)0x8027C400, (void*)0x8027D080, (void*)0x8027DD00, (void*)0x8027E780, (void*)0x8027F580, (void*)0x8027F680);
            initializeEntity(6, 6, 0x32, (void*)0x8027F900, (void*)0x80280580, (void*)0x80281200, (void*)0x80281C80, (void*)0x80282A80, (void*)0x80282B80);
            initializeEntity(7, 7, 0x33, (void*)0x80282E00, (void*)0x80283A80, (void*)0x80284700, (void*)0x80285180, (void*)0x80285F80, (void*)0x80286080);
            initializeEntity(8, 8, 0x34, (void*)0x80286300, (void*)0x80286F80, (void*)0x80287C00, (void*)0x80288680, (void*)0x80289480, (void*)0x80289580);
            initializeEntity(9, 9, 0x35, (void*)0x80294250, (void*)0x80294ED0, (void*)0x80295B50, (void*)0x802965D0, (void*)0x802973D0, (void*)0x802974D0);
            initializeEntity(0xA, 0xA, 0x36, (void*)0x80297750, (void*)0x802983D0, (void*)0x80299050, (void*)0x80299AD0, (void*)0x8029A8D0, (void*)0x8029A9D0);
            initializeEntity(0xB, 0xB, 0x37, (void*)0x8029AC50, (void*)0x8029B8D0, (void*)0x8029C550, (void*)0x8029CFD0, (void*)0x8029DDD0, (void*)0x8029DED0);
            initializeEntity(0xC, 0xC, 0x38, (void*)0x8029E150, (void*)0x8029EDD0, (void*)0x8029FA50, (void*)0x802A04D0, (void*)0x802A12D0, (void*)0x802A13D0);
            initializeEntity(0xD, 0xD, 0x39, (void*)0x802A1650, (void*)0x802A22D0, (void*)0x802A2F50, (void*)0x802A39D0, (void*)0x802A47D0, (void*)0x802A48D0);
            initializeEntity(0xE, 0xE, 0x3A, (void*)0x802A4B50, (void*)0x802A57D0, (void*)0x802A6450, (void*)0x802A6ED0, (void*)0x802A7CD0, (void*)0x802A7DD0);
            initializeEntity(0xF, 0xF, 0x3B, (void*)0x802A8050, (void*)0x802A8CD0, (void*)0x802A9950, (void*)0x802AA3D0, (void*)0x802AB1D0, (void*)0x802AB2D0);
            initializeEntity(0x10, 0x10, 0x3C, (void*)0x802AB550, (void*)0x802AC1D0, (void*)0x802ACE50, (void*)0x802AD8D0, (void*)0x802AE6D0, (void*)0x802AE7D0);
            initializeEntity(0x11, 0x11, 0x3D, (void*)0x802AEA50, (void*)0x802AF6D0, (void*)0x802B0350, (void*)0x802B0DD0, (void*)0x802B1BD0, (void*)0x802B1CD0);
            initializeEntity(0x12, 0x12, 0x3E, (void*)0x802B1F50, (void*)0x802B2BD0, (void*)0x802B3850, (void*)0x802B42D0, (void*)0x802B50D0, (void*)0x802B51D0);
            initializeEntity(0x13, 0x13, 0x3F, (void*)0x802B5450, (void*)0x802B60D0, (void*)0x802B6D50, (void*)0x802B77D0, (void*)0x802B85D0, (void*)0x802B86D0);
            initializeEntity(0x14, 0x14, 0x40, (void*)0x802B8950, (void*)0x802B95D0, (void*)0x802BA250, (void*)0x802BACD0, (void*)0x802BBAD0, (void*)0x802BBBD0);
            
            initializeEntity(0x16, 0x16, 0x41, (void*)0x802BBE50, (void*)0x802BC250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x17, 0x17, 0x42, (void*)0x802BC650, (void*)0x802BCA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x18, 0x18, 0x43, (void*)0x802BCE50, (void*)0x802BD250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x19, 0x19, 0x44, (void*)0x802BD650, (void*)0x802BDA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x1A, 0x1A, 0x45, (void*)0x802BDE50, (void*)0x802BE250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x1B, 0x1B, 0x47, (void*)0x802BE650, (void*)0x802BEA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            
            if (gCutsceneIndex == SEA_FESTIVAL) {
                initializeEntity(0x1E, 0x1E, 0x4A, (void*)0x802BF650, (void*)0x802BFE50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            } else {
                
                initializeEntity(0x1C, 0x1C, 0x48, (void*)0x802BEE50, (void*)0x802BF250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x1D, 0x1D, 0x49, (void*)0x802BF650, (void*)0x802BFA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x1E, 0x1E, 0x4A, (void*)0x802BFE50, (void*)0x802C0250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x1F, 0x1F, 0x4B, (void*)0x802C0650, (void*)0x802C0A50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x20, 0x20, 0x4C, (void*)0x802C0E50, (void*)0x802C1250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x21, 0x21, 0x4D, (void*)0x802C1650, (void*)0x802C1A50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x22, 0x22, 0x4E, (void*)0x802C1E50, (void*)0x802C2250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x23, 0x23, 0x4F, (void*)0x802C2650, (void*)0x802C2A50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x24, 0x24, 0x50, (void*)0x802C2E50, (void*)0x802C3250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x25, 0x25, 0x51, (void*)0x802C3650, (void*)0x802C3A50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x26, 0x27, 0x52, (void*)0x802C3E50, (void*)0x802C4250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x28, 0x28, 0x53, (void*)0x802C4A50, (void*)0x802C4E50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x29, 0x29, 0x54, (void*)0x802C5A50, (void*)0x802C5E50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x2A, 0x2A, 0x55, (void*)0x802C6250, (void*)0x802C6650, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x2B, 0x2B, 0x56, (void*)0x802C6A50, (void*)0x802C7850, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
                initializeEntity(0x2C, 0x57, 0x58, (void*)0x802CEB50, (void*)0x802D1050, (void*)0x802D3550, (void*)0x802D3750, (void*)0x802D3850, (void*)0x802D3650);
            }

            break;
        
        // cow festival
        case 3:
            
            initializeEntity(ENTITY_PLAYER, 0, 0x31, (void*)PLAYER_TEXTURE_1_BUFFER, (void*)0x80243B00, (void*)PLAYER_PALETTE_BUFFER, (void*)PLAYER_ANIMATION_METADATA_BUFFER, (void*)PLAYER_SPRITESHEET_INDEX_BUFFER, (void*)PLAYER_TEXTURE_TO_PALETTE_LOOKUP_BUFFER);
            
            initializeEntity(6, 6, 0x32, (void*)0x80278D50, (void*)0x802799D0, (void*)0x8027A650, (void*)0x8027B0D0, (void*)0x8027BED0, (void*)0x8027BFD0);
            initializeEntity(7, 7, 0x33, (void*)0x8027C250, (void*)0x8027CED0, (void*)0x8027DB50, (void*)0x8027E5D0, (void*)0x8027F3D0, (void*)0x8027F4D0);
            initializeEntity(8, 8, 0x34, (void*)0x8027F750, (void*)0x802803D0, (void*)0x80281050, (void*)0x80281AD0, (void*)0x802828D0, (void*)0x802829D0);
            initializeEntity(9, 9, 0x35, (void*)0x80282C50, (void*)0x802838D0, (void*)0x80284550, (void*)0x80284FD0, (void*)0x80285DD0, (void*)0x80285ED0);
            initializeEntity(0xA, 0xA, 0x36, (void*)0x80286150, (void*)0x80286DD0, (void*)0x80287A50, (void*)0x802884D0, (void*)0x802892D0, (void*)0x802893D0);
            initializeEntity(0xB, 0xB, 0x37, (void*)0x80294250, (void*)0x80294ED0, (void*)0x80295B50, (void*)0x802965D0, (void*)0x802973D0, (void*)0x802974D0);
            initializeEntity(0xC, 0xC, 0x38, (void*)0x80297750, (void*)0x802983D0, (void*)0x80299050, (void*)0x80299AD0, (void*)0x8029A8D0, (void*)0x8029A9D0);
            initializeEntity(0xD, 0xD, 0x39, (void*)0x8029AC50, (void*)0x8029B8D0, (void*)0x8029C550, (void*)0x8029CFD0, (void*)0x8029DDD0, (void*)0x8029DED0);
            initializeEntity(0xE, 0xE, 0x3A, (void*)0x8029E150, (void*)0x8029EDD0, (void*)0x8029FA50, (void*)0x802A04D0, (void*)0x802A12D0, (void*)0x802A13D0);
            initializeEntity(0xF, 0xF, 0x3B, (void*)0x802A1650, (void*)0x802A22D0, (void*)0x802A2F50, (void*)0x802A39D0, (void*)0x802A47D0, (void*)0x802A48D0);
            initializeEntity(0x10, 0x10, 0x3C, (void*)0x802A4B50, (void*)0x802A57D0, (void*)0x802A6450, (void*)0x802A6ED0, (void*)0x802A7CD0, (void*)0x802A7DD0);
            initializeEntity(0x11, 0x11, 0x3D, (void*)0x802A8050, (void*)0x802A8CD0, (void*)0x802A9950, (void*)0x802AA3D0, (void*)0x802AB1D0, (void*)0x802AB2D0);
            initializeEntity(0x12, 0x12, 0x3E, (void*)0x802AB550, (void*)0x802AC1D0, (void*)0x802ACE50, (void*)0x802AD8D0, (void*)0x802AE6D0, (void*)0x802AE7D0);
            initializeEntity(0x13, 0x13, 0x3F, (void*)0x802AEA50, (void*)0x802AF6D0, (void*)0x802B0350, (void*)0x802B0DD0, (void*)0x802B1BD0, (void*)0x802B1CD0);
            initializeEntity(0x14, 0x14, 0x40, (void*)0x802B1F50, (void*)0x802B2BD0, (void*)0x802B3850, (void*)0x802B42D0, (void*)0x802B50D0, (void*)0x802B51D0);
            initializeEntity(0x15, 0x15, 0x59, (void*)0x802B5450, (void*)0x802B60D0, (void*)0x802B6D50, (void*)0x802B77D0, (void*)0x802B85D0, (void*)0x802B86D0);
            
            initializeEntity(1, 1, 0x2C, (void*)0x8026F000, (void*)0x8026FA88, (void*)0x80275950, (void*)0x80275A50, (void*)0x80278650, (void*)0x80278850);
            initializeEntity(2, 2, 0x2D, (void*)0x80270510, (void*)0x80270F98, (void*)0x80275950, (void*)0x80275A50, (void*)0x80278650, (void*)0x80278850);
            initializeEntity(3, 3, 0x2E, (void*)0x80271A20, (void*)0x802724A8, (void*)0x80275950, (void*)0x80275A50, (void*)0x80278650, (void*)0x80278850);
            initializeEntity(4, 4, 0x2F, (void*)0x80272F30, (void*)0x802739B8, (void*)0x80275950, (void*)0x80275A50, (void*)0x80278650, (void*)0x80278850);
            initializeEntity(5, 5, 0x30, (void*)0x80274440, (void*)0x80274EC8, (void*)0x80275950, (void*)0x80275A50, (void*)0x80278650, (void*)0x80278850);
            
            initializeEntity(0x16, 0x16, 0x41, (void*)0x802BBE50, (void*)0x802BC250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x17, 0x17, 0x42, (void*)0x802BC650, (void*)0x802BCA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x18, 0x18, 0x43, (void*)0x802BCE50, (void*)0x802BD250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x19, 0x19, 0x44, (void*)0x802BD650, (void*)0x802BDA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x1A, 0x1A, 0x45, (void*)0x802BDE50, (void*)0x802BE250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x1B, 0x1B, 0x47, (void*)0x802BE650, (void*)0x802BEA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x1C, 0x1C, 0x48, (void*)0x802BEE50, (void*)0x802BF250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            initializeEntity(0x1D, 0x1D, 0x49, (void*)0x802BF650, (void*)0x802BFA50, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);	
            initializeEntity(0x1E, 0x1E, 0x4A, (void*)0x802BFE50, (void*)0x802C0250, (void*)0x802C8650, (void*)0x802CAE50, (void*)0x802CE350, (void*)0x802CE550);
            
            break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/transition", func_80059300);

// resume everything
void func_80059300(void) {
    togglePauseEntities();
    func_80046CF4();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);
}

//INCLUDE_ASM("asm/nonmatchings/game/transition", func_80059334);

// pause everything
void inline func_80059334(void) {
    pauseEntities();
    pauseAllCutsceneExecutors();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, FALSE);
}
 
//INCLUDE_ASM("asm/nonmatchings/game/transition", func_80059368);

// overlay screen/naming screen transition
void func_80059368(void) {

    // load entities and entity global sprites
    func_8002FB7C();
    func_80046CF4();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);
    func_80065AA0();

    dmaMapAssets(0, gMapWithSeasonIndex);
    
    setupLevelMap(gBaseMapIndex);
    //setupLevelMap(currentMapContext.currentMapIndex);
    
    setPlayerAction(0xFE, 0);
    func_8006623C();
    
    setMainLoopCallbackFunctionIndex(MAIN_GAME);
    setPlayerAction(0, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/transition", func_800593EC);

// pause before going to overlay screen or naming screen
void inline func_800593EC(void) {
    func_8002FB3C();
    pauseAllCutsceneExecutors();
    setEntityMapSpaceIndependent(ENTITY_PLAYER, FALSE);
    func_8003C504(MAIN_MAP_INDEX);
}

//INCLUDE_ASM("asm/nonmatchings/game/transition", startNewDay);

void startNewDay(void) {
    
    u8 i, j;
    
    if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {
        houseExtensionConstructionCounter++;
    }
    
    if (checkDailyEventBit(CARPENTER_ESTIMATE)) {
        setLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION);
    }
     
    if (gSeason == AUTUMN && gDayOfMonth == 28) {
        setLifeEventBit(BRIDGE_COMPLETED);
    }
    
    if (gSeason == WINTER && gDayOfMonth == 17) {
        setLifeEventBit(HOT_SPRINGS_COMPLETED);
        setDailyEventBit(0x38);
    }
    
    if (gDayOfMonth == 1) {
        
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 7; j++) {
                D_80189108[i][j] = 0xFF;
            }
        }
        
    }
    
    if (gSeason == SPRING && gDayOfMonth == 23) {
        setFlowerFestivalGoddess();
    }

    if (gSeason == AUTUMN) {

        if (gDayOfMonth == 12) {
            gHarvestCoinFinder = getRandomNumberInRange(0, 5);
            if (checkLifeEventBit(CLIFF_GONE) && gHarvestCoinFinder == 4) {
                gHarvestCoinFinder = 1;
            }
            if (checkLifeEventBit(KAI_GONE) && gHarvestCoinFinder == 5) {
                gHarvestCoinFinder = 2;
            }
        }

        if (gSeason == AUTUMN && gDayOfMonth == 13 && !checkDailyEventBit(0x5D) && gHarvestCoinFinder == PLAYER) {
            gHarvestCoinFinder = 3;
        }
    }
    
    if (checkLifeEventBit(ELLEN_DIED)) {
        if (gElliGrievingCounter >= 31) {
            toggleSpecialDialogueBit(ELLI_FINISH_GRIEVING);
        } else {
            gElliGrievingCounter++;
        }
    }
    
    if (gWeather == TYPHOON) {
        
        // get average of sprite affection
        if (((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120) {
            
            // check animal score
            if ((func_8009B564() + func_8009B374()) >= 2) {
                setLifeEventBit(HARVEST_SPRITES_ANIMAL_HELP);
            }

        }
        
        // harvest sprites help with animals during typhoon
        if (gWeather == TYPHOON && checkLifeEventBit(HARVEST_SPRITES_ANIMAL_HELP)) {
            func_80087D5C();
        }

    }
    
    // dog/horse race
    if (checkLifeEventBit(0x51) && checkDailyEventBit(0x41)) {
        toggleLifeEventBit(0x51);
    }
    
    if (checkLifeEventBit(0x52) && checkDailyEventBit(0x42)) {
        toggleLifeEventBit(0x52);
    }
    
    // Mrs. Mana's cow
    if (checkLifeEventBit(0x93)) {
        toggleLifeEventBit(0x93);
        setLifeEventBit(0x94);
        setMrsManaCowsNames();
    }
    
    removeKeyItem(HORSE_RACE_TICKET);

    // equalize harvest sprite affection
    npcAffection[HARVEST_SPRITE_2] = npcAffection[HARVEST_SPRITE_1];
    npcAffection[HARVEST_SPRITE_3] = npcAffection[HARVEST_SPRITE_1];
    
    if (gWeather == RAIN) {
        // update animal status based on rain
        func_80064114();
    }
    
    if (gWeather == SNOW) {
        // update animal status based on snow
        func_800644B0();
    }
    
    if (gWeather == TYPHOON) {
        // update animal status based on typhoon
        func_80064814();
    }
    
    gWeather = gForecast;
    
    incrementVarietyShowCounter();
    setForecast();
    
    if (checkLifeEventBit(0x7E)) {
        // unused code?
        checkLifeEventBit(0x7F);
    }
    
    func_800DB424();

    resetAnimalStatuses();
    updateCropsIfRain();
    randomlyAddWeedsToFarmField();
    updateFamilyStates();
    setLetters();
    
    if (checkLifeEventBit(MARRIED)) {
        handleWifeMorningHelp();
    }
    
    if (checkDailyEventBit(7)) {
        // save game
        func_800E53E8(gCurrentGameIndex);
    }
    
    // update more game state
    func_800598E0();
    
}
