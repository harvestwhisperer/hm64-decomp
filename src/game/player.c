#include "common.h"

#include "game/player.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/mapController.h"
#include "system/math.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/levelInteractions.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/shop.h"
#include "game/time.h"
#include "game/transition.h"
#include "game/weather.h"

#include "mainLoop.h"
#include "mainproc.h"

#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

// bss
Player gPlayer;
// counter until napping animation
u16 playerIdleCounter;

// shared bss
u8 gAlcoholTolerance;
// u8 gHappiness;
u8 gMaximumStamina;
u8 gPlayerBirthdaySeason;
u8 gToolboxSlots[8 * 4];

bool heldItemChange = FALSE;

// data
// indexed by gSpawnPointIndex
Vec3f playerDefaultStartingCoordinates[] = {
    { -352.0f, 0.0f, -16.0f },
    { -544.0f, 0.0f, 144.0f },
    { -128.0f, 0.0f, -336.0f },
    { 64.0f, 0.0f, -368.0f },
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
    { -448.0f, 0.0f, -96.0f },
    { -272.0f, 0.0f, 326.0f },
    { -448.0f, 0.0f, -80.0f },
    { -352.0f, 0.0f, 144.0f },
    { -48.0f, 0.0f, 112.0f },
    { -176.0f, 0.0f, 16.0f },
    { -48.0f, 0.0f, -48.0f },
    { 48.0f, 0.0f, 64.0f },
    { 48.0f, 0.0f, 64.0f },
    { 96.0f, 0.0f, 48.0f },
    { 64.0f, 0.0f, -144.0f },
    { -16.0f, 0.0f, 208.0f },
    { -16.0f, 0.0f, 80.0f },
    { 240.0f, 0.0f, -16.0f },
    { 128.0f, 0.0f, -16.0f },
    { 64.0f, 0.0f, 240.0f },
    { -176.0f, 0.0f, -304.0f },
    { -224.0f, 0.0f, -144.0f },
    { -224.0f, 0.0f, 48.0f },
    { 96.0f, 0.0f, -304.0f },
    { -16.0f, 0.0f, 320.0f },
    { -160.0f, 0.0f, 128.0f },
    { 0.0f, 0.0f, -416.0f },
    { 96.0f, 0.0f, 224.0f },
    { -16.0f, 0.0f, -192.0f },
    { -224.0f, 0.0f, 240.0f },
    { -112.0f, 0.0f, 128.0f },
    { -176.0f, 0.0f, -480.0f },
    { 64.0f, 0.0f, -416.0f },
    { -96.0f, 0.0f, 112.0f },
    { -192.0f, 0.0f, 16.0f },
    { -384.0f, 0.0f, 288.0f },
    { 96.0f, 0.0f, -64.0f },
    { 0.0f, 0.0f, 0.0f },
    { -512.0f, 0.0f, -208.0f },
    { 176.0f, 0.0f, -480.0f },
    { 448.0f, 0.0f, -464.0f },
    { 320.0f, 0.0f, -208.0f },
    { 112.0f, 0.0f, -304.0f },
    { -368.0f, 0.0f, -80.0f },
    { 320.0f, 0.0f, 160.0f },
    { 80.0f, 0.0f, -144.0f },
    { 176.0f, 0.0f, 144.0f },
    { 16.0f, 0.0f, 112.0f },
    { 464.0f, 0.0f, -48.0f },
    { 192.0f, 0.0f, 384.0f },
    { 304.0f, 0.0f, -144.0f },
    { 224.0f, 0.0f, 240.0f },
    { -272.0f, 0.0f, 240.0f },
    { -176.0f, 0.0f, 240.0f },
    { -80.0f, 0.0f, -128.0f },
    { -416.0f, 0.0f, -160.0f },
    { -384.0f, 0.0f, -80.0f },
    { -384.0f, 0.0f, 176.0f },
    { -256.0f, 0.0f, 384.0f },
    { 288.0f, 0.0f, 160.0f },
    { 448.0f, 0.0f, 80.0f },
    { -240.0f, 0.0f, -192.0f },
    { -304.0f, 0.0f, -112.0f },
    { -400.0f, 0.0f, 176.0f },
    { 368.0f, 0.0f, -64.0f },
    { -64.0f, 0.0f, 0.0f },
    { 176.0f, 0.0f, -192.0f },
    { 80.0f, 0.0f, 160.0f },
    { -32.0f, 0.0f, -224.0f },
    { -304.0f, 0.0f, -128.0f },
    { 256.0f, 0.0f, -80.0f },
    { -32.0f, 0.0f, 80.0f },
    { -80.0f, 0.0f, -16.0f },
    { -16.0f, 0.0f, 96.0f },
    { 80.0f, 0.0f, -144.0f },
    { 32.0f, 0.0f, 64.0f },
    { 32.0f, 0.0f, -16.0f },
    { -48.0f, 0.0f, 240.0f },
    { -96.0f, 0.0f, -144.0f },
    { -16.0f, 0.0f, 128.0f },
    { -176.0f, 0.0f, -96.0f },
    { 64.0f, 0.0f, -16.0f },
    { -160.0f, 0.0f, 48.0f },
    { -32.0f, 0.0f, 48.0f },
    { -16.0f, 0.0f, 64.0f },
    { -112.0f, 0.0f, 64.0f },
    { 64.0f, 0.0f, 16.0f },
    { 48.0f, 0.0f, 64.0f },
    { -144.0f, 0.0f, 32.0f },
    { 96.0f, 0.0f, 48.0f },
    { 48.0f, 0.0f, 80.0f },
    { -32.0f, 0.0f, 96.0f },
    { -16.0f, 0.0f, 32.0f },
    { -16.0f, 0.0f, 96.0f },
    { -16.0f, 0.0f, 96.0f },
    { 192.0f, 0.0f, -144.0f },
    { -256.0f, 0.0f, 80.0f },
    { 192.0f, 0.0f, 112.0f },
    { 64.0f, 0.0f, -16.0f },
    { -32.0f, 0.0f, 96.0f },
    { -144.0f, 0.0f, 64.0f },
    { 64.0f, 0.0f, 80.0f },
    { 32.0f, 0.0f, -16.0f },
    { -112.0f, 0.0f, 32.0f },
    { 32.0f, 0.0f, 32.0f },
    { -16.0f, 0.0f, 64.0f },
    { 96.0f, 0.0f, -160.0f },
    { -32.0f, 0.0f, 80.0f },
    { 128.0f, 0.0f, 0.0f },
    { -352.0f, 0.0f, -64.0f },
    { 80.0f, 0.0f, -80.0f },
    { 0.0f, 0.0f, 0.0f },
};

// indexed by gSpawnPointIndex
u8 playerDefaultStartingDirections[] = {
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHEAST, 
    SOUTHWEST, 
    NORTHEAST, 
    NORTHEAST, 
    NORTHWEST, 
    SOUTHWEST, 
    NORTHEAST, 
    NORTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHEAST, 
    SOUTHWEST, 
    NORTHEAST, 
    SOUTHEAST, 
    SOUTHEAST, 
    NORTHWEST, 
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHWEST, 
    SOUTHWEST, 
    NORTHWEST, 
    NORTHWEST, 
    SOUTHEAST, 
    NORTHEAST, 
    SOUTHWEST, 
    NORTHEAST, 
    SOUTHEAST, 
    SOUTHWEST, 
    NORTHWEST, 
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHWEST, 
    NORTHEAST, 
    SOUTHWEST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHEAST, 
    NORTHEAST, 
    SOUTHEAST, 
    NORTHWEST, 
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHWEST, 
    SOUTHEAST, 
    SOUTHWEST, 
    SOUTHWEST, 
    NORTHEAST, 
    SOUTHWEST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHEAST, 
    NORTHEAST, 
    NORTHEAST, 
    SOUTHWEST, 
    NORTHEAST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHEAST, 
    NORTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    NORTHWEST, 
    SOUTHEAST, 
    NORTHEAST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    NORTHEAST, 
    NORTHEAST, 
    NORTHEAST, 
    NORTHEAST, 
    NORTHWEST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHWEST, 
    SOUTHEAST, 
    NORTHWEST, 
    NORTHEAST, 
    SOUTHWEST, 
    NORTHEAST, 
    NORTHWEST, 
    SOUTHWEST, 
    SOUTHWEST, 
    NORTHWEST, 
    SOUTHWEST, 
    SOUTHWEST
};

u8 toolToFatigueScore[MAX_TOOLS][3] = {
    { 0, 0, 0 },
    { 2, 4, 6 },
    { 2, 4, 6 },
    { 1, 2, 3 },
    { 1, 2, 3 },
    { 1, 2, 6 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 1, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 1, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
};

// rodata
// forward declaration
static const s8 directionToTileDeltaX[12];
static const s8 directionToTileDeltaZ[12]; 
static const u8 toolHeldItemIndices[5][3];

// forward declarations
void handleItemLevelInteraction(u8 arg0); 
u8 checkAndTriggerStaminaExhaustion(u8);                 
bool checkTooFatiguedToUseTool(void);
void handleToolAction(void);
void handleThrowItemAction(void);
void handlePutItemInShippingBinAction(void);
void handlePickUpItemAction(void);
void handlePlaceItemAction(void);
void handleEatAction(void);
void handleGetIntoBedAction(void);
void handleIdleNapAction(void);
void handleTurnOnTVAction(void);
void handleShopItemAction(void);
void handleJumpAction(void);
void handleMountHorseAction(void);
void handleDismountHorseAction(void);
void handleWalkDownStairsAction(void);
void handleToiletAction(void);
void handleBathingAction(void);
void handleHotSpringAction(void);
void handleWhistleForDogAction(void);
void handleWhistleForHorseAction(void);
void handleDrinkAction(void);
void handleUnusedAction21(void);
void handleFishingAction(void);
void handleUnusedAction23(void);
void handleUnusedAction24(void);
void handleStaminaExhaustionAction(void);
void handleUnusedAction25(void);
void handleTreeClimbingAction(void);
void handleToolAcquisitionAction(void);
void handleJumpDownAction(void);
void handleResetHeldItemAction(void);
void handlePutFoodInDogBowlAction(void);
void handleDropItemInWaterAction(void);
void handleUnusedAction33(void);
void handleAcquireMusicBoxAction(void);
void handleThrowItemAnimation(void);
void handleToolAnimation(void);
void handleShippingAnimation(void);
void handlePickUpAnimation(void);
void handlePlaceItemAnimation(void);
void handleEatAnimation(void);
void handleStaminaExhaustionAnimation(void);
void handleFatigueThresholdAnimation(void);
void handleIdleNapAnimation(void);
void handleUnusedAnimation29(void);
void handleDialogueSelectionAnimation(void);
void handlePutFoodInDogBowlAnimation(void);
void handleDropItemInWaterAnimation(void);
void handleDrinkAlcoholAnimation(void);
void handleAcquireMusicBoxAnimation(void);
void handleGetIntoBedAnimation(void);
void handleTurnOnTVAnimation(void);
void handleShopAnimation(void);
void handleJumpAnimation(void);
void handleMountHorseAnimation(void);
void handleDismountHorseAnimation(void);
void handleUnusedAnimation17(void);
void handleUnusedAnimation18(void);
void handleUnusedAnimation30(void);
void handleWhistleForDogAnimation(void);
void handleWhistleForHorseAnimation(void);
void handleDrinkingAnimation(void);
void handlePutItemInRucksackAnimation(void);
void handleFishingRodAnimation(void);
void handleUnusedAnimation24(void);
void handleUnusedAnimation25(void);
void handleClimbTreeAnimation(void);
void handleToolAcquisitionAnimation(void);
void handleToolUseAnimation(u16, u16);
void handleSickleAnimation();                                  
void handleHoeAnimation();                                  
void handleAxAnimation();                                  
void handleHammerAnimation();                                  
void handleWateringCanAnimation();                                  
void handleMilkerAnimation();                                  
void handleBellAnimation();                                  
void handleBrushAnimation();                                  
void handleClippersAnimation();                                  
void handleTurnipSeedsAnimation();                                  
void handlePotatoSeedsAnimation();                                  
void handleCabbageSeedsAnimation();                                  
void handleTomatoSeedsAnimation();                                  
void handleCornSeedsAnimation();                                  
void handleEggplantSeedsAnimation();                                  
void handleStrawberrySeedsAnimation();                                  
void handleMoonDropSeedsAnimation();                                  
void handlePinkCatMintSeedsAnimation();                                  
void handleBlueMistSeedsAnimation();                                  
void handleChickenFeedAnimation();                                  
void handleBallAnimation();                                  
void handleFeedingBottleAnimation();                                  
void handleUnusedTool23Animation();                                  
void handleFishingPoleAnimation();                                  
void handleMiraclePotionAnimation();                                  
void handleCowMedicineAnimation();                                  
void handleGrassSeedsAnimation();                                  
void handleBlueFeatherAnimation();                                  
void handleEmptyBottleAnimation();      
void dismountHorse(void);    
void handlePlayerInput(void);                              

static inline void resetAction() {
    gPlayer.actionPhaseFrameCounter = 0;
    gPlayer.actionPhase = 0;
    gPlayer.actionHandler = 0;
    gPlayer.animationHandler = 0;
}

static inline void startAction(u8 action, u8 animationHandler) {
    gPlayer.actionHandler = action;
    gPlayer.actionPhaseFrameCounter = 0;
    gPlayer.actionPhase = 0;
    gPlayer.animationHandler = animationHandler;
}

static inline void resetMovement() {
    gPlayer.movementVector.x = 0.0f;
    gPlayer.movementVector.y = 0.0f;
    gPlayer.movementVector.z = 0.0f;
    gPlayer.actionPhaseFrameCounter = 0;
}

// TODO: improve label
static inline void reset() {
    gPlayer.actionPhaseFrameCounter = 0;
    gPlayer.actionPhase = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", setupPlayerEntity);

void setupPlayerEntity(u16 spawnPoint, u8 resetPlayer) {
 
    loadEntity(ENTITY_PLAYER, 0, TRUE);

    setEntityCollidable(ENTITY_PLAYER, TRUE);
    setEntityYMovement(ENTITY_PLAYER, TRUE);
    setCameraTrackingEntity(ENTITY_PLAYER, TRUE);
    setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    setEntityColor(ENTITY_PLAYER, globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a);
    setEntityShadow(ENTITY_PLAYER, 0);

    if (resetPlayer) {
        gPlayer.coordinates.x = playerDefaultStartingCoordinates[spawnPoint].x;
        gPlayer.coordinates.y = playerDefaultStartingCoordinates[spawnPoint].y;
        gPlayer.coordinates.z = playerDefaultStartingCoordinates[spawnPoint].z;
        gPlayer.direction = playerDefaultStartingDirections[spawnPoint];
    }

    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
    setEntityCoordinates(ENTITY_PLAYER, gPlayer.coordinates.x, gPlayer.coordinates.y, gPlayer.coordinates.z);

    toolUse.stumpHitCounter = 0;
    toolUse.stumpHitPositionX = 0;
    toolUse.stumpHitPositionZ = 0;
    toolUse.boulderHitCounter = 0;
    toolUse.boulderHitPositionX = 0;
    toolUse.boulderHitPositionZ = 0;

}
  
//INCLUDE_ASM("asm/nonmatchings/game/player", initializePlayerHeldItem);

void initializePlayerHeldItem(void) {

    if (gPlayer.heldItem) {
        
        switch (gPlayer.heldItem) {

            case DOG_HELD_ITEM ... 0x6F:
            case BABY_HELD_ITEM ... 0xC9:
            case PUPPY_1_HELD_ITEM ... 0xB2:
                gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_HELD, gPlayer.heldItem, ITEM_CONTEXT_HAS_DIRECTION_FRAME, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            default:
                gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_HELD, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            
        }
        
        setItemAttachmentOffset(0, 0, 14.0f, 20.0f);
        
    } else {
        gItemBeingHeld = 0xFF;
        clearHeldItemSlot(gPlayer.itemInfoIndex);
    }
    
}

inline u8 addItemToRucksack(u8 item) {

    u8 i;
    u8 found = 0xFF;

    for (i = 0; i < MAX_RUCKSACK_SLOTS && found == 0xFF; i++) {
        if (gPlayer.belongingsSlots[i] == 0) {
            gPlayer.belongingsSlots[i] = item;
            found = 1;
        }
    }

    return found;    
    
}

inline u8 takeItemFromRucksack(void) {
    u8 i;
    u8 writeIdx = 0;
    u8 nextItem = 0xFF;
    u8 itemToStore = gPlayer.heldItem;
    u8 found = 0xFF;

    for (i = 0; i < MAX_RUCKSACK_SLOTS && found == 0xFF; i++) {
        if (gPlayer.belongingsSlots[i] != 0) {
            nextItem = gPlayer.belongingsSlots[i];
            gPlayer.belongingsSlots[i] = 0;
            found = 1;
        }
    }
    if (found == 1) {
        for (i = 0; i < MAX_RUCKSACK_SLOTS; i++) {
            if (gPlayer.belongingsSlots[i] != 0) {
                gPlayer.belongingsSlots[writeIdx] = gPlayer.belongingsSlots[i];
                if (writeIdx != i) {
                    gPlayer.belongingsSlots[i] = 0;
                }
                writeIdx++;
            }
        }
        gPlayer.belongingsSlots[writeIdx] = itemToStore;
        gPlayer.heldItem = nextItem;
    }
    return found;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", storeTool);

// store tool
u8 storeTool(u8 tool) {

    u8 i;
    u8 found = 0xFF;

    for (i = 0; i < MAX_TOOL_SLOTS_RUCKSACK && found == 0xFF; i++) {
        if (gPlayer.toolSlots[i] == tool) {
            found = 1;
        }
    }

    if (found == 0xFF) {
        for (i = 0; i < MAX_TOOLBOX_SLOTS && found == 0xFF; i++) {
            if (gToolboxSlots[i] == tool) {
                found = 2;
            }
        }
    }

    if (found == 0xFF) {
        for (i = 0; i <  MAX_TOOL_SLOTS_RUCKSACK && found == 0xFF; i++) {
            if (gPlayer.toolSlots[i] == 0) {
                gPlayer.toolSlots[i] = tool;
                found = 1;
            }
        }
    }

    if (found == 0xFF) {
        for (i = 0; i < MAX_TOOLBOX_SLOTS && found == 0xFF; i++) {
            if (gToolboxSlots[i] == 0) {
                gToolboxSlots[i] = tool;
                found = 2;
            }
        }
    }
    
    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", removeTool);

u8 removeTool(u8 tool) {

    u8 i;
    u8 found = 0xFF;

    for (i = 0; i < MAX_TOOL_SLOTS_RUCKSACK && found == 0xFF; i++) {
        if (gPlayer.toolSlots[i] == tool) {
            gPlayer.toolSlots[i] = 0;
            found = 1;
        }
    }

    if (found == 0xFF) {
        for (i = 0; i < MAX_TOOLBOX_SLOTS && found == 0xFF; i++) {
            if (gToolboxSlots[i] == tool) {
                gToolboxSlots[i] = 0;
                found = 2;
            }
        }
    }

    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", acquireKeyItem);

inline u8 acquireKeyItem(u8 item) {

    u8 i;
    u8 found = 0xFF;

    for (i = 0; i < MAX_KEY_ITEMS && found == 0xFF; i++) {
        if (gPlayer.keyItemSlots[i] == 0) {
            gPlayer.keyItemSlots[i] = item;
            found = 1;
        }
    }
    
    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", removeKeyItem);

u8 removeKeyItem(u8 item) {

    u8 i;
    u8 found = 0xFF;

    for (i = 0; i < MAX_KEY_ITEMS && found == 0xFF; i++) {
        if (gPlayer.keyItemSlots[i] == item) {
            gPlayer.keyItemSlots[i] = 0;
            found = 1;
        }
    }
    
    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkHaveTool);

bool checkHaveTool(u8 tool) {

    u8 i;
    bool found = 0;

    for (i = 0; i < MAX_TOOL_SLOTS_RUCKSACK && !found; i++) {
    
        if (gPlayer.toolSlots[i] == tool) {
            found = 1;
        }
    
    }

    if (!found) {

        for (i = 0; i < MAX_TOOLBOX_SLOTS && !found; i++) {
            if (gToolboxSlots[i] == tool) {
                found = 2;
            }
        }

    }

    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkHaveKeyItem);

inline bool checkHaveKeyItem(u8 item) {

    u8 i;
    bool found = FALSE;
    
    for (i = 0; i < MAX_KEY_ITEMS && !found; i++) {

        if (gPlayer.keyItemSlots[i] == item) {
            found = TRUE;
        }
        
    }

    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", syncPlayerCoordinatesFromEntity);

void syncPlayerCoordinatesFromEntity(void) {
    gPlayer.coordinates.x = entities[ENTITY_PLAYER].coordinates.x;
    gPlayer.coordinates.y = entities[ENTITY_PLAYER].coordinates.y;
    gPlayer.coordinates.z = entities[ENTITY_PLAYER].coordinates.z;
}

 
static const s8 directionToTileDeltaX[12] = { 0, -1, -1, -1, 0, 1, 1, 1, 0, 0, 0, 0 };
static const s8 directionToTileDeltaZ[12] = { 1, 1, 0, -1, -1, -1, 0, 1, 0, 0, 0, 0 };

Vec3f getOffsetTileCoordinates (f32 range, u8 directionalOffset) {
    
    Vec3f tileCoordinates;

    tileCoordinates = getEntityTileCoordinates(ENTITY_PLAYER);

    if (tileCoordinates.y != 65535.0f) {

        tileCoordinates.x += directionToTileDeltaX[convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)] * range;
        tileCoordinates.z += directionToTileDeltaZ[convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)] * range;

        if (directionalOffset != 8) {
            
            tileCoordinates.x += directionToTileDeltaX[((entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + directionalOffset) % 8)];
            tileCoordinates.z += directionToTileDeltaZ[((entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + directionalOffset) % 8)];
            
        }
    }

    return tileCoordinates;
    
}




/* Action handling */


void setPlayerAction(u16 action, u16 animationHandler) {

    if (action != 0xFF) {
        gPlayer.actionPhaseFrameCounter = 0;
        gPlayer.actionPhase = 0;
        gPlayer.actionHandler = action;
    }

    if (animationHandler != 0xFF) {
        gPlayer.animationHandler = animationHandler;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", updatePlayerAction);

void updatePlayerAction(void) {

    if (gPlayer.actionHandler == 0) {
        // handle button input
        handlePlayerInput();
    }
    
    switch (gPlayer.actionHandler - 1) {
        case (TOOL_USE - 1):
            handleToolAction();
            break;
        case (DISCARDING_ITEM - 1):
            handleThrowItemAction();
            break;
        case (SHIPPING_ITEM - 1):
            handlePutItemInShippingBinAction();
            break;
        case (PICKING_UP_ITEM - 1):
            handlePickUpItemAction();
            break;
        case (PLACING_ITEM - 1):
            handlePlaceItemAction();
            break;
        case (EATING - 1):
            handleEatAction();
            break;

        case (GETTING_INTO_BED - 1):
            handleGetIntoBedAction();
            break;
        case (IDLE_NAP - 1):
            handleIdleNapAction();
            break;
        case (WATCHING_TV - 1):
            handleTurnOnTVAction();
            break;
        case (SHOPPING - 1):
            handleShopItemAction();
            break;
        case (JUMPING - 1):
            handleJumpAction();
            break;
        case (MOUNTING_HORSE - 1):
            handleMountHorseAction();
            break;
        case (DISMOUNTING_HORSE - 1):
            handleDismountHorseAction();
            break;
        case (WALKING_DOWN_STAIRS - 1):
            handleWalkDownStairsAction();
            break;
        case (USING_TOILET - 1):
            handleToiletAction();
            break;
        case (BATHING - 1):
            handleBathingAction();
            break;
        case (USING_HOT_SPRINGS - 1):
            handleHotSpringAction();
            break;
        case (WHISTLING_FOR_DOG - 1):
            handleWhistleForDogAction();
            break;
        case (WHISTLING_FOR_HORSE - 1):
            handleWhistleForHorseAction();
            break;
        case (DRINKING - 1):
            handleDrinkAction();
            break;
        case (STORING_ITEM_IN_RUCKSACK - 1):
            handleUnusedAction21();
            break;
        case (FISHING - 1):
            handleFishingAction();
            break;
        case 22:
            handleUnusedAction23();
            break;
        case 23:
            handleUnusedAction24();
            break;
        case (STAMINA_EXHAUSTION - 1):
            handleStaminaExhaustionAction();
            break;
        case (FATIGUE_THRESHOLD - 1):
            handleUnusedAction25();
            break;
        case (CLIMBING_TREE - 1):
            handleTreeClimbingAction();
            break;
        case (ACQUIRING_TOOL - 1):
            handleToolAcquisitionAction();
            break;
        case (JUMPING_DOWN - 1):
            handleJumpDownAction();
            break;
        case (DIALOGUE_SELECTING - 1):
            handleResetHeldItemAction();
            break;
        case (PUTTING_FOOD_IN_DOG_BOWL - 1):
            handlePutFoodInDogBowlAction();
            break;
        case (DROPPING_ITEM_IN_WATER - 1):
            handleDropItemInWaterAction();
            break;
        case 32:
            handleUnusedAction33();
            break;
        case (ACQUIRING_MUSIC_BOX - 1):
            handleAcquireMusicBoxAction();
            break;
    }
    
    updateHeldItemState();
    processToolUseState();

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePlayerInput);

void handlePlayerInput(void) {

    Vec3f vec;
    Vec3f vec3;
    
    u8 temp;
    f32 tempAnalogStickMagnitude;
    
    u8 horseResult;
    u8 npcResult;
    
    u8 direction;
    u8 groundObjectIndex;

    bool set;

    set = FALSE;
    temp = 0;

    gPlayer.groundObjectIndex = getGroundObjectIndexFromPlayerPosition(0.6f, 8);

    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
        controllers[CONTROLLER_1].button &= BUTTON_A;
        controllers[CONTROLLER_1].buttonPressed &= BUTTON_A;
    }
    
    if (handleLevelInteraction(gBaseMapIndex)) {
        set = TRUE;
        temp = 0xFF;
    }

    checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, BUTTON_A);

    horseResult = handleHorseGrownPlayerInteraction();
    npcResult = checkNPCInteraction();

    if (!set && npcResult == 1) {
        temp = 0xFF;
        pauseGameplay();
        setMainLoopCallbackFunctionIndex(DIALOGUE);
        set = TRUE;
        resetAction();
    }

    // baby
    if (!set && npcResult == 2) {
        temp = 0xFF;
        set = TRUE;
        setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
    }

    if (!set) {
        if (handlePlayerAnimalInteraction()) {
            set = TRUE;
            temp = 0xFF;
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && handlePickUpGroundItem()) {
            set = TRUE;
            temp = 0xFF;
            setPlayerAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
        }
        
    }

    if (!set) {
        
        // jump over logs
        if (gBaseMapIndex == FARM && gPlayer.groundObjectIndex == LOG) {

            if ((getAnalogStickMagnitude(CONTROLLER_1) / 1.2f) > 4.6) {
                
                if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x34, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX))) {

                    vec3 = projectEntityPosition(ENTITY_PLAYER, 0x34, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

                    groundObjectIndex = getGroundObjectIndexFromCoordinates(vec3.x, vec3.z);

                    if (groundObjectIndex == 0xFF || getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8) {
                        
                        setDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
                        set = TRUE;
                        temp = 0xFF;
                        startAction(JUMPING, ANIM_JUMPING);
                        
                    }
                    
                } 
            }
        }
        
    }

    if (!set) {
        if (checkButtonPressed(CONTROLLER_1, BUTTON_C_DOWN)) {
            if (gPlayer.heldItem && getItemFlags(gPlayer.heldItem) & ITEM_EATABLE) {
                set = TRUE;
                startAction(EATING, ANIM_EATING);
            }
        }
    }

    if (!checkDailyEventBit(0xD)) {

        if (!set) {
        
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (entities[ENTITY_PLAYER].collision == 0xFFFF || horseResult > 0) {
                   
                    if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
                    
                        if (gPlayer.heldItem) {
                            set = TRUE; 
                            handleItemLevelInteraction(horseResult);
                            temp = 0xFF;
                        }
                        
                    } else {
                        dismountHorse();
                        set = TRUE;
                        temp = 0xFF;
                    }
                
                }
                
            }

        }

    }

    if (!set) {

        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && !checkDailyEventBit(18)) {
        
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
                
                if (gPlayer.heldItem == 0 && gPlayer.currentTool) {
                        
                    gPlayer.actionPhaseFrameCounter = 0;
                    gPlayer.actionPhase = 0;
                    gPlayer.toolHeldCounter = 0;
                    gPlayer.currentToolLevel = 0;

                    if (!checkAndTriggerStaminaExhaustion(1)) {

                        if (!checkTooFatiguedToUseTool()) {

                            startAction(TOOL_USE, ANIM_TOOL_USE);
                            set = TRUE;
                            temp = 0xFF;
                            
                        } else {
                            temp = 0xFF;
                        }
                        
                    }
                    
                    set = TRUE;
                    temp = 0xFF;
                    
                }
        
            }
        
        }

    }

    if (!set) {
        // show text for item being held
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && !checkDailyEventBit(18)) {
            if (checkButtonReleased(CONTROLLER_1, BUTTON_Z) && !heldItemChange) {
                if (gPlayer.heldItem != 0) {
                    set = TRUE;
                    showHeldItemText(gPlayer.heldItem);
                    temp = 0xFF;
                }
            }
            if(checkButtonReleased(CONTROLLER_1, BUTTON_Z) && heldItemChange) {
                heldItemChange = FALSE;
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_RIGHT)) {
                set = TRUE;
                temp = 0xFF;
                // whistle for dog
                startAction(WHISTLING_FOR_DOG, ANIM_WHISTLE_FOR_DOG);
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_LEFT)) {
                set = TRUE;
                temp = 0xFF;
                // whistle for horse
                startAction(WHISTLING_FOR_HORSE, ANIM_WHISTLE_FOR_HORSE);
            }
        }
    }

    if(!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            if (checkButtonHeld(CONTROLLER_1, BUTTON_Z) && checkButtonPressed(CONTROLLER_1, BUTTON_C_UP) && !checkDailyEventBit(18)) {
                if (takeItemFromRucksack() != 0xFF) {
                    set = TRUE;
                    temp = 0xFF;
                    heldItemChange = TRUE;
                    initializePlayerHeldItem();
                    // TODO: add animation
                }
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_UP)) {
                if (gPlayer.heldItem != 0 && getItemFlags(gPlayer.heldItem) & ITEM_RUCKSACK_STORABLE) {
                    if (addItemToRucksack(gPlayer.heldItem) != 0xFF) {
                        set = TRUE;
                        temp = 0xFF;
                        startAction(STORING_ITEM_IN_RUCKSACK, ANIM_PUT_ITEM_IN_RUCKSACK);
                    }
                }
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && !checkDailyEventBit(0x13)) {
            if (checkButtonPressed(CONTROLLER_1, BUTTON_START)) {
                set = TRUE;
                pauseGameplay();
                loadOverlayScreen(1, PAUSE_SCREEN_LOAD);
                playSfx(MENU_OPEN_SFX);
                temp = 0xFF;
            }
        }
    }

    if (gBaseMapIndex == FARM) {

        if (!set) {

            if (checkButtonPressed(CONTROLLER_1, BUTTON_R)) {

                resetAction();
                pauseGameplay();
                startMapRotation(MAIN_MAP_INDEX, 0, 0xFF);

                setMainLoopCallbackFunctionIndex(ROTATING);

                gCameraRotationOffset = (gCameraRotationOffset + 7) % 8;

                set = TRUE;
                temp = 0xFF;
                
            }

            if (!set) {
                
                if (checkButtonPressed(CONTROLLER_1, BUTTON_L)) {

                    resetAction();
                    pauseGameplay();
                    startMapRotation(MAIN_MAP_INDEX, 1, 0xFF);
    
                    setMainLoopCallbackFunctionIndex(ROTATING);
    
                    gCameraRotationOffset = (gCameraRotationOffset + 1) % 8;
    
                    set = TRUE;
                    temp = 0xFF;
                    
                }
                
            }
            
        }
        
    }

    if (!set) {
        if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && entities[ENTITY_PLAYER].collision != 0xFFFF) {
            gPlayer.animationHandler = ANIM_DEFAULT;
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            set = TRUE;
            temp = 0xFF;
        }
    }

    tempAnalogStickMagnitude = 0.0f;

    if (!set) {

        if (!temp) {

            temp = getAnalogStickDirection(CONTROLLER_1);
            tempAnalogStickMagnitude = getAnalogStickMagnitude(CONTROLLER_1);
            
        }

        if (temp != 0xFF) {

            temp = convertWorldToSpriteDirection(temp, MAIN_MAP_INDEX);
            
            // running
            if (tempAnalogStickMagnitude >= 4.0f) {
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.actionPhase = 0;
                gPlayer.actionHandler = CONTROLLER_INPUT;
                gPlayer.animationHandler = ANIM_RUNNING;
            // walking
            } else {
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.actionPhase = 0;
                gPlayer.actionHandler = CONTROLLER_INPUT;
                gPlayer.animationHandler = ANIM_WALKING;
            }

            direction = convertSpriteToWorldDirection(temp, MAIN_MAP_INDEX);
            setEntityDirection(ENTITY_PLAYER, direction);

        } else {
            
            tempAnalogStickMagnitude = 0.0f;
            resetAction();
            
        }
        
        tempAnalogStickMagnitude = (s8)tempAnalogStickMagnitude;

        vec = getMovementVectorFromDirection(tempAnalogStickMagnitude, temp, 0);
        
        setEntityMovementVector(ENTITY_PLAYER, vec.x, vec.y, vec.z, tempAnalogStickMagnitude);
        
    }

    gPlayer.direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    gPlayer.velocity = tempAnalogStickMagnitude;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", mountHorse);

inline void mountHorse(void) {

    deactivateEntity(horseInfo.entityIndex);

    horseInfo.flags &= ~4;
    horseInfo.flags |= 8;
    gPlayer.flags |= PLAYER_RIDING_HORSE;

    setDailyEventBit(0x5C);
 
    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(horseInfo.direction, MAIN_MAP_INDEX));
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", dismountHorse);

void dismountHorse(void) {

    Vec3f vec1;
    Vec3f vec2;
    u8 direction; // bug: not initialied
    bool set = FALSE;
    int groundObjectIndex;
    
    while (direction < 8 && !set) {
    
        if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 8, convertWorldToSpriteDirection(direction, gMainMapIndex))) {

            if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(direction, gMainMapIndex))) {

                vec1 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(direction, MAIN_MAP_INDEX));
                
                groundObjectIndex = getGroundObjectIndexFromCoordinates(vec1.x, vec1.z);
                
                if (groundObjectIndex == 0xFF || (getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8)) {
                    set = TRUE;
                }

            }

        }

        if (!set) {
            direction++;
        }

    }

    if (set) {

        vec2 = projectEntityPosition(ENTITY_PLAYER, 0, direction);
        
        horseInfo.coordinates = vec2;
        horseInfo.direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
        horseInfo.location = gBaseMapIndex;
        horseInfo.flags &= ~HORSE_BEING_RODE;     

        gPlayer.flags &= ~PLAYER_RIDING_HORSE;
        
        toggleDailyEventBit(0x5C);
        
        initializeHorseEntity();
        
        entities[ENTITY_PLAYER].direction = direction;

        startAction(DISMOUNTING_HORSE, ANIM_DISMOUNT_HORSE);

        gPlayer.actionPhase = 0;
        gPlayer.actionPhaseFrameCounter = 0;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleItemLevelInteraction);

// item <-> level interaction 
void handleItemLevelInteraction(u8 arg0) {

    bool set = FALSE;
    Vec3f vec;
    u8 groundObjectIndex;
    
    if (arg0 == 0) {

        if ((getItemFlags(gPlayer.heldItem) & ITEM_INCUBATOR_PLACEABLE) && (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 17) && gBaseMapIndex == COOP && getIncubatingEggCount() == 0) {
            set = TRUE;
            startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
        }

        if ((getItemFlags(gPlayer.heldItem) & ITEM_LUMBER_STORABLE) && !set && (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 17) && gBaseMapIndex == FARM) {
            set = TRUE;
            startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
        }

        if (checkFarmDogBowlInteraction(gBaseMapIndex) != 0xFF) {
            
            if ((getItemFlags(gPlayer.heldItem) & ITEM_EATABLE) && !set) {    
                startAction(PUTTING_FOOD_IN_DOG_BOWL, ANIM_PUT_FOOD_IN_DOG_BOWL);
            }
            
            set = TRUE;

        }

        // drop item in water
        if ((getItemFlags(gPlayer.heldItem) & ITEM_RUCKSACK_STORABLE) && !set && checkWaterSourceInteraction(gBaseMapIndex) != 0xFF) {
            set = TRUE;
            startAction(DROPPING_ITEM_IN_WATER, ANIM_DROP_ITEM_IN_WATER);
        }
        
    }

    if ((getItemFlags(gPlayer.heldItem) & ITEM_SHIPPABLE) && !set) {

        if (checkShippingBinInteraction(gBaseMapIndex) != 0xFF || arg0) {
            // shipping item
            startAction(SHIPPING_ITEM, ANIM_SHIPPING);
        } else {
            startAction(DISCARDING_ITEM, ANIM_THROWING);
        }

        set = TRUE;
        
    }    

    if (arg0 == 0) {

        if ((getItemFlags(gPlayer.heldItem) & ITEM_ANIMAL_FEED) && !set) {

            if (checkBarnAnimalFeedInteraction(gBaseMapIndex) != 0xFF) {
                startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
            } else {
                startAction(DISCARDING_ITEM, ANIM_THROWING);
            }

            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x80) && !set) {

            if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex))) {

                vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

                groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);
    
                if (groundObjectIndex == 0xFF || getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8) {
                    startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
                }
            
            }

            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x100) && !set) {
            startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
            set = TRUE;
        }
        
        if ((getItemFlags(gPlayer.heldItem) & ITEM_CRIB_PLACEABLE) && !set) {
            
            if (gBaseMapIndex == HOUSE) {

                // baby
                if (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x16) {
                    startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
                }
                
            } 
            
            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x800) && !set) {

            if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex))) {

                vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
    
                groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);
    
                if (groundObjectIndex == 0xFF || getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8) {
                    startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
                }
            
            }

            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x10) && !set) {

            groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8);

            if (groundObjectIndex && groundObjectIndex < 4 && groundObjectIndex != 0xFF) {
                startAction(PLACING_ITEM, ANIM_PLACING_ITEM);
            }  
            
            set = TRUE;
            
        }
    
        if (!set) {    
            startAction(DISCARDING_ITEM, ANIM_THROWING);
        }
        
    }
    
}


/* Stamina/fatigue helpers */

//INCLUDE_ASM("asm/nonmatchings/game/player", checkStaminaExhaustionLevel);

inline u8 checkStaminaExhaustionLevel(void) {

    s32 temp;
    u8 temp2;
    u8 result;

    result = 0;
    temp = gMaximumStamina;
    
    temp2 = temp / 4;

    if (toolToFatigueScore[gPlayer.currentTool][gPlayer.currentToolLevel] > gPlayer.currentStamina) {
        result = 4;
    } else {
        
        if (temp2 >= gPlayer.currentStamina) {
            result = 3;
        } else if ((temp2 * 2) >= gPlayer.currentStamina) {
            result = 2;
        } else {
            if ((temp2 * 3) >= gPlayer.currentStamina) {
                result = 1;
            }
        }
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkFatigueLevel);

inline u8 checkFatigueLevel(void) {

    u8 result;
     
    if (gPlayer.fatigueCounter == MAX_FATIGUE_POINTS) {
        result = 3;
    } else if (gPlayer.fatigueCounter >= 75) {
        result = 2;
    }  else {
        result = (gPlayer.fatigueCounter < 50) == 0;  
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkAndTriggerStaminaExhaustion);

bool checkAndTriggerStaminaExhaustion(u8 arg0) {

    bool result = FALSE;
    u8 staminaExhaustionLevel;

    staminaExhaustionLevel = checkStaminaExhaustionLevel();

    if (staminaExhaustionLevel && staminaExhaustionLevel != gPlayer.staminaExhaustionLevel && arg0 == 0 || staminaExhaustionLevel == 4)  {
        result = TRUE;
        startAction(STAMINA_EXHAUSTION, ANIM_STAMINA_EXHAUSTION);
        gPlayer.staminaExhaustionLevel = staminaExhaustionLevel;
    }

    return result; 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkTooFatiguedToUseTool);

bool checkTooFatiguedToUseTool(void) {

    bool tooFatigued = FALSE;
    
    u32 fatigueThreshold = checkFatigueLevel();

    if (fatigueThreshold != 0 && fatigueThreshold != gPlayer.fatigueThreshold) {
        tooFatigued = TRUE;
        startAction(FATIGUE_THRESHOLD, ANIM_FATIGUE_THRESHOLD);
        gPlayer.fatigueThreshold = fatigueThreshold;
    }

    return tooFatigued;

}



/* Action handlers */

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToolAction);

void handleToolAction(void) {

    s32 temp;

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER)) {

        if (gPlayer.actionPhase == 0) {

            if (gPlayer.actionPhaseFrameCounter == 0) {
                gPlayer.toolHeldCounter = 0;
                gPlayer.currentToolLevel = 0;
                toggleDailyEventBit(0x14);
                gPlayer.actionPhaseFrameCounter++;
            }

            if (!checkButtonHeld(CONTROLLER_1, BUTTON_B)) {

                gPlayer.actionPhase++;
                gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, -toolToFatigueScore[gPlayer.currentTool][gPlayer.currentToolLevel], gMaximumStamina);

                if (((SUNNY < gWeather && gWeather < 4) || !(5 < gHour && gHour < 18)) && gBaseMapIndex != GREENHOUSE) {

                    // kappa power nut
                    if (checkLifeEventBit(0x5F)) {
                        gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, toolToFatigueScore[gPlayer.currentTool][gPlayer.currentToolLevel] / 2, MAX_FATIGUE_POINTS);
                    } else {
                        gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, toolToFatigueScore[gPlayer.currentTool][gPlayer.currentToolLevel], MAX_FATIGUE_POINTS);
                    }

                }
                
            } else if (gPlayer.toolHeldCounter >= 16 && gPlayer.currentToolLevel < getToolLevel(gPlayer.currentTool)) {

                if (toolToFatigueScore[gPlayer.currentTool][gPlayer.currentToolLevel] && !(toolToFatigueScore[gPlayer.currentTool][gPlayer.currentToolLevel+1] > gPlayer.currentStamina)) {
                    gPlayer.toolHeldCounter = 0;
                    gPlayer.currentToolLevel++;
                }

                
            } else {
                gPlayer.toolHeldCounter++;
            }

        } else {
            
            gPlayer.actionHandler = 254;
            gPlayer.actionPhase++;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleThrowItemAction);

void handleThrowItemAction(void) {

    if (gPlayer.actionPhaseFrameCounter == 0 && gPlayer.actionPhase == 0) {
        playSfx(0x26);
        clearHeldItemSlot(gPlayer.itemInfoIndex);
        allocateThrownItemSlot(1, ITEM_STATE_THROW_START, gPlayer.heldItem, 0, 8);
        gPlayer.heldItem = 0;
        gPlayer.actionPhaseFrameCounter = 1;
    }

}

void handlePutItemInShippingBinAction(void) {

    if (gPlayer.actionPhaseFrameCounter == 3) {
        setItemState(gPlayer.itemInfoIndex, ITEM_STATE_SHIP);
        gPlayer.heldItem = 0;
        gPlayer.actionPhase++;
    }

    gPlayer.actionPhaseFrameCounter++;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePickUpItemAction);

void handlePickUpItemAction(void) {

    if (gPlayer.actionPhaseFrameCounter == 0 && gPlayer.actionPhase == 0) {
        
        playSfx(PICKING_UP_SFX);
        startAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);

        switch (gPlayer.heldItem) {

            case DOG_HELD_ITEM ... 0x6F:
            case BABY_HELD_ITEM ... 0xC9:
            case PUPPY_1_HELD_ITEM ... 0xB2:
                gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_PICKUP, gPlayer.heldItem, ITEM_CONTEXT_HAS_DIRECTION_FRAME, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            default:
                gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_PICKUP, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
        
            }
            
    }
    
    if (gPlayer.actionPhaseFrameCounter == 4) {
        setItemState(gPlayer.itemInfoIndex, ITEM_STATE_PICKUP_DONE);
        gPlayer.actionPhase++;
    }
    
    gPlayer.actionPhaseFrameCounter++;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePlaceItemAction);

void handlePlaceItemAction(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER)) {
     
        if (gPlayer.actionPhase == 0) {
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_PUT_DOWN);
            gPlayer.heldItem = 0;
            playSfx(PICKING_UP_SFX);
        }

        if (gPlayer.actionPhase != 1 || (gPlayer.actionPhaseFrameCounter++, gPlayer.actionPhaseFrameCounter == 2)) {
            gPlayer.actionPhase++;
        }

    }   

}


//INCLUDE_ASM("asm/nonmatchings/game/player", handleEatAction);

void handleEatAction(void) {

    if (gPlayer.actionPhase == 2) {

        if (gPlayer.actionPhaseFrameCounter == 10) {
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
            gPlayer.heldItem = 0;
            gPlayer.actionPhase++;
            playSfx(PUTTING_DOWN_SFX); 
        }

        gPlayer.actionPhaseFrameCounter++;

    }

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, getItemStaminaIncreaseValue(gPlayer.heldItem), gMaximumStamina);
            gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -getItemFatigueReductionValue(gPlayer.heldItem), 100);
        }

        if (gPlayer.actionPhaseFrameCounter == 30) {
            gPlayer.actionPhase++;
        }

        gPlayer.actionPhaseFrameCounter++;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleGetIntoBedAction);

void handleGetIntoBedAction(void) {

    if (gPlayer.actionPhase == 0) {
    
        setEntityCoordinates(ENTITY_PLAYER, -140.0f, 0.0f, -152.0f);
        setEntityCollidable(ENTITY_PLAYER, FALSE);
        setEntityYMovement(ENTITY_PLAYER, FALSE);
        setCameraTrackingEntity(ENTITY_PLAYER, FALSE);
    
        gPlayer.actionPhase++;

        setMapObjectAnimation(MAIN_MAP_INDEX, 0, 7);
    
    }

    if (gPlayer.actionPhase == 3) {
        setMapObjectAnimation(MAIN_MAP_INDEX, 0, 8);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleIdleNapAction);

void handleIdleNapAction(void) {
    if (checkButtonHeld(CONTROLLER_1, 0xFFFFFFFF) && gPlayer.actionPhase < 4) {
        gPlayer.actionPhase = 4;
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleTurnOnTVAction);

void handleTurnOnTVAction(void) {

    if (gPlayer.actionPhase == 0) {

        gPlayer.actionPhaseFrameCounter++;
        
        if (gPlayer.actionPhaseFrameCounter == 2) {
            gPlayer.actionPhase++;
        }
        
    }

    if (gPlayer.actionPhase == 3) {

        gPlayer.actionPhaseFrameCounter++;
        
        if (gPlayer.actionPhaseFrameCounter == 2) {
            gPlayer.actionPhase++;
        }
        
    }

    if (gPlayer.actionPhase == 2) {
        setMainLoopCallbackFunctionIndex(TV);
        gPlayer.actionPhaseFrameCounter = 0;
        gPlayer.actionPhase++;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleShopItemAction);

void handleShopItemAction(void) {

    if (gPlayer.actionPhase == 0) {
        if (gPlayer.actionPhaseFrameCounter == 4) {
            if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                setItemState(gPlayer.itemInfoIndex, ITEM_STATE_PICKUP_DONE);
            }
            gPlayer.actionPhase++;
        }
        gPlayer.actionPhaseFrameCounter++;
    }
    
    if (gPlayer.actionPhase == 2) {
        if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
        }
        loadShopItemSprite(gPlayer.shopItemIndex);
        gPlayer.actionPhase = 5;
    }
    
    if (gPlayer.actionPhase == 3) {
        if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
        }
        deactivateShopItemMapObject(gPlayer.shopItemIndex);
        gPlayer.actionPhase = 5;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleJumpAction);

void handleJumpAction(void) {

    Vec3f vec;
    Vec3f vec2;
    
    if (gPlayer.actionPhase) {
        if (checkEntityAnimationStateChanged(ENTITY_PLAYER)) {
            gPlayer.actionPhase++;
        }
    }

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {

            playSfx(ITEM_PLUCK_SFX);
            
            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityTracksCollisions(ENTITY_PLAYER, FALSE);
            enableEntityMovement(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            
            vec2 = getMovementVectorFromDirection(4.0f, gPlayer.direction, 0.0f);
            
            gPlayer.movementVector = vec2;
            
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            
        }

        if (gPlayer.actionPhaseFrameCounter < 15) {

            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 4.0f);
            
            // parabolic equation: y = initial_y - gravity * (time - peak_time)²
            entities[ENTITY_PLAYER].coordinates.y = 
                ((gPlayer.movementVector.y 
                     + (((gPlayer.actionPhaseFrameCounter - 7) * (gPlayer.actionPhaseFrameCounter - 7)) * -0.4f)) 
                     + 19.6);
            
        } else {
            
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhase++;
            gPlayer.actionPhaseFrameCounter = 0;
            toggleDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
            
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleJumpDownAction);

void handleJumpDownAction(void) {

    Vec3f vec;
    Vec3f vec2;
    
    if (gPlayer.actionPhase) {
        if (checkEntityAnimationStateChanged(ENTITY_PLAYER)) {
            gPlayer.actionPhase++;
        }
    }

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {

            playSfx(ITEM_PLUCK_SFX);
            
            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityTracksCollisions(ENTITY_PLAYER, FALSE);
            enableEntityMovement(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            
            vec2 = getMovementVectorFromDirection(4.0f, gPlayer.direction, 0.0f);
            
            gPlayer.movementVector = vec2;
            
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            
        }

        if (gPlayer.actionPhaseFrameCounter < 27) {

            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 4.0f);
            
            // parabolic equation: y = initial_y - gravity * (time - peak_time)²
            entities[ENTITY_PLAYER].coordinates.y = 
                ((gPlayer.movementVector.y 
                     + (((gPlayer.actionPhaseFrameCounter - 7) * (gPlayer.actionPhaseFrameCounter - 7)) * -0.4f)) 
                     + 19.6);
            
        } else {
            
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhase++;
            gPlayer.actionPhaseFrameCounter = 0;
            toggleDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
            
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }
    
}

// empty function
void handleResetHeldItemAction(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePutFoodInDogBowlAction);
  
// put food in dog bowl
void handlePutFoodInDogBowlAction(void) {
 
    if (gPlayer.actionPhaseFrameCounter == 3) {
        setItemState(gPlayer.itemInfoIndex, ITEM_STATE_FEED_DOG);
        gPlayer.heldItem = 0;
        gPlayer.actionPhase++;
    }

    gPlayer.actionPhaseFrameCounter++;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDropItemInWaterAction);

void handleDropItemInWaterAction(void) {

    if (gPlayer.actionPhaseFrameCounter == 3) {
        setItemState(gPlayer.itemInfoIndex, ITEM_STATE_DROP_IN_WATER);
        gPlayer.heldItem = 0;
        gPlayer.actionPhase++;
    }

    gPlayer.actionPhaseFrameCounter++;

}

// empty function
void handleUnusedAction33(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleAcquireMusicBoxAction);

void handleAcquireMusicBoxAction(void) {

    if (gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhaseFrameCounter == 0) {
            gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_HELD, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
        }
        
        if (gPlayer.actionPhaseFrameCounter >= 30) {
            gPlayer.actionPhase = 1;
        } else {
            gPlayer.actionPhaseFrameCounter++;
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleMountHorseAction);

void handleMountHorseAction(void) {

    Vec3f vec;

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {

            playSfx(ITEM_PLUCK_SFX);
            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            vec = getMovementVectorFromDirection(4.0f, gPlayer.direction, 0.0f);
            gPlayer.movementVector = vec;
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            
        }

        if (gPlayer.actionPhaseFrameCounter < 6) {
            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 4.0f);
        } else {
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhase++;
            gPlayer.actionPhaseFrameCounter = 0;
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }

    if (gPlayer.actionPhase) {
        
        setEntityCollidable(ENTITY_PLAYER, TRUE);
        setEntityYMovement(ENTITY_PLAYER, TRUE);
        
        resetAction();

        mountHorse();
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDismountHorseAction);

void handleDismountHorseAction(void) {

    Vec3f vec;

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {

            playSfx(ITEM_PLUCK_SFX);
            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            vec = getMovementVectorFromDirection(4.0f, gPlayer.direction, 0.0f);
            gPlayer.movementVector = vec;
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
             
        }

        if (gPlayer.actionPhaseFrameCounter < 8) {
            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 4.0f);
        } else {
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhase++;
            gPlayer.actionPhaseFrameCounter = 0;
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }

    if (gPlayer.actionPhase) {
        setEntityCollidable(ENTITY_PLAYER, TRUE);
        setEntityYMovement(ENTITY_PLAYER, TRUE);
        resetAction();
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleWalkDownStairsAction);

void handleWalkDownStairsAction(void) {

    Vec3f vec;

    switch (gPlayer.actionPhase) {
        
        case 0:

            gPlayer.animationHandler = ANIM_DEFAULT;
            gPlayer.actionPhase++;
            
            break;
        
        case 1:

            gPlayer.animationHandler = ANIM_WALKING;
            gPlayer.direction = gPlayer.savedDirection;

            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));

            vec = getMovementVectorFromDirection(1.0f, gPlayer.direction, 0.0f);

            gPlayer.movementVector = vec;

            gPlayer.actionPhase++;
            
            break;
        
        case 2:
                
            if (gPlayer.actionTimer == 0) {
                
                gPlayer.actionPhase++;
                
                gPlayer.movementVector.x = 0.0f;
                gPlayer.movementVector.y = 0.0f;
                gPlayer.movementVector.z = 0.0f;

            } else {
                gPlayer.actionTimer--;
            }
            
            break;
        
        case 3:
            resetAction();
            break;
    }

    setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 1.0f);
   
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToiletAction);

void handleToiletAction(void) {

    Vec3f vec;
    Vec3f vec2;

    switch (gPlayer.actionPhase) {

        case 0:

            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            
            activateMapAddition(MAIN_MAP_INDEX, 0, 0);
            
            gPlayer.direction = NORTHEAST;
            gPlayer.actionTimer = 16;
            gPlayer.actionPhase++;
            
            vec = getMovementVectorFromDirection(4.0f, 4, 0.0f);
            gPlayer.movementVector = vec;
            
            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
            gPlayer.animationHandler = ANIM_RUNNING;
            
            break;


        case 1:

            if (gPlayer.actionTimer == 0) {

                gPlayer.movementVector.x = 0.0f;
                gPlayer.movementVector.y = 0.0f;
                gPlayer.movementVector.z = 0.0f;

                activateMapAddition(MAIN_MAP_INDEX, 2, 0);
                             
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.animationHandler = ANIM_DEFAULT;
                gPlayer.actionPhase++;
                
            } else {
                gPlayer.actionTimer--;
            }
            
            break;

        
        case 2:

            if (gPlayer.actionPhaseFrameCounter == 90) {
                
                activateMapAddition(MAIN_MAP_INDEX, 0, 0);

                playSfx(DOOR_OPEN_SFX);
                
                gPlayer.direction = SOUTHWEST;
                gPlayer.actionTimer = 16;
                gPlayer.actionPhase++;
                
                vec2 = getMovementVectorFromDirection(4.0f, 0, 0.0f);
                gPlayer.movementVector = vec2;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                gPlayer.animationHandler = ANIM_RUNNING;
                
            } else {
                
                gPlayer.actionPhaseFrameCounter++;
                
                if (gPlayer.actionPhaseFrameCounter == 10) {
                    // flush
                    playSfx(86);
                }
                
            }

            break;
        
        case 3:

            if (gPlayer.actionTimer == 0) {

                gPlayer.movementVector.x = 0.0f;
                gPlayer.movementVector.y = 0.0f;
                gPlayer.movementVector.z = 0.0f;
                
                setEntityCollidable(ENTITY_PLAYER, TRUE);
                setEntityYMovement(ENTITY_PLAYER, TRUE);
    
                activateMapAddition(MAIN_MAP_INDEX, 2, 0);
    
                resetAction();
    
                toggleDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
    
                if (!checkDailyEventBit(0x31)) {
                    gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -10, MAX_FATIGUE_POINTS);
                } 
                
                setDailyEventBit(0x31);
                
            } else {
                gPlayer.actionTimer--;
            }

            break;
        
    }

    setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 1.0f);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleBathingAction);

// bathing
void handleBathingAction(void) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;

    switch (gPlayer.actionPhase) {

        case 0:

            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityTracksCollisions(ENTITY_PLAYER, FALSE);
            enableEntityMovement(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            activateMapAddition(MAIN_MAP_INDEX, 1, 0);
            
            gPlayer.direction = NORTHWEST;
            gPlayer.actionTimer = 16;
            gPlayer.actionPhase++;
            
            vec = getMovementVectorFromDirection(4.0f, 2, 0.0f);
            gPlayer.movementVector = vec;
            
            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
            gPlayer.animationHandler = ANIM_RUNNING;
            
            break;
        
        case 1:

            if (gPlayer.actionTimer == 0) {
                
                activateMapAddition(MAIN_MAP_INDEX, 3, 0);
                
                gPlayer.direction = NORTHEAST;
                gPlayer.actionTimer = 20;
                
                vec2 = getMovementVectorFromDirection(4.0f, 4, 0.0f);
                gPlayer.movementVector = vec2;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                gPlayer.actionPhase++;
                
            } else {
                gPlayer.actionTimer--;
            }
            
            break;
        
        case 2:

            if (gPlayer.actionTimer == 0) {
                
                gPlayer.direction = NORTHWEST;
                
                resetMovement();
                startAction(JUMPING, ANIM_JUMPING);
                
                gPlayer.flags |= PLAYER_BATH_POSE_1;
            
            } else {
                gPlayer.actionTimer--;
            }

            break;

        case 3:

            if (gPlayer.actionPhaseFrameCounter == 60) {
                
                gPlayer.direction = SOUTHEAST;
                startAction(JUMPING, ANIM_JUMPING);
                gPlayer.flags |= PLAYER_BATH_POSE_3;

            } else {
                gPlayer.actionPhaseFrameCounter++;
            }

            break;

        case 4:

            gPlayer.direction = SOUTHWEST;
            
            gPlayer.actionTimer = 20;
            gPlayer.actionPhase++;
            
            vec2 = getMovementVectorFromDirection(4.0f, 0, 0.0f);
            gPlayer.movementVector = vec2;

            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
            gPlayer.animationHandler = ANIM_RUNNING;
            
            break;

        case 5:

            if (gPlayer.actionTimer == 0) {

                activateMapAddition(MAIN_MAP_INDEX, 1, 0);
                playSfx(DOOR_OPEN_SFX);
                gPlayer.direction = SOUTHEAST;
                gPlayer.actionTimer = 16;
                vec3 = getMovementVectorFromDirection(4.0f, 6, 0.0f);

                gPlayer.movementVector = vec3;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                gPlayer.actionPhase++;
                
            } else {
                gPlayer.actionTimer--;
            }

            break;
        
        case 6:

            if (gPlayer.actionTimer == 0) {
    
                gPlayer.movementVector.x = 0.0f;
                gPlayer.movementVector.y = 0.0f;
                gPlayer.movementVector.z = 0.0f;
                
                setEntityCollidable(ENTITY_PLAYER, TRUE);
                setEntityTracksCollisions(ENTITY_PLAYER, TRUE);
                enableEntityMovement(ENTITY_PLAYER, TRUE);
                setEntityYMovement(ENTITY_PLAYER, TRUE);
    
                activateMapAddition(MAIN_MAP_INDEX, 3, 0);
    
                resetAction();
    
                toggleDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
    
                if (!checkDailyEventBit(0x32)) {
    
                    if (checkHaveKeyItem(FLOWER_BATH_CRYSTALS)) {
                        gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -20, MAX_FATIGUE_POINTS);
                    } else {
                        gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -10, MAX_FATIGUE_POINTS);
                    }
                    
                } 
                
                setDailyEventBit(0x32);
                
            } else {
                gPlayer.actionTimer--;
            }
        
        break;
        
    }

    setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 1.0f);

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleHotSpringAction);

void handleHotSpringAction(void) {

    // needing separate vecs implies these case statements are static inlines that have their own vecs
    Vec3f vec;
    Vec3f vec2;

    switch (gPlayer.actionPhase) {

        case 0:

            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityTracksCollisions(ENTITY_PLAYER, FALSE);
            enableEntityMovement(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            activateMapAddition(MAIN_MAP_INDEX, 1, 0);
            
            gPlayer.direction = NORTHEAST;
            gPlayer.actionTimer = 0x12;
            gPlayer.actionPhase++;

            vec = getMovementVectorFromDirection(4.0f, 4, 0.0f);
            gPlayer.movementVector = vec;
            
            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
            gPlayer.animationHandler = ANIM_RUNNING;
            
            break;

        case 1:

            if (gPlayer.actionTimer == 0) {
                activateMapAddition(MAIN_MAP_INDEX, 0, 0);
                gPlayer.direction = EAST;
                resetMovement();
                startAction(JUMPING, ANIM_JUMPING);
                gPlayer.flags |= PLAYER_HOT_SPRING_POSE_1;
            } else {
                gPlayer.actionTimer--;
            }

            break;

        case 2:

            if (gPlayer.actionPhaseFrameCounter == 60) {
                gPlayer.direction = WEST;
                startAction(JUMPING, ANIM_JUMPING);
                gPlayer.flags |= PLAYER_HOT_SPRING_POSE_3;
            } else {
                gPlayer.actionPhaseFrameCounter++;
            }

            break;

        case 3:

            if (gPlayer.actionTimer == 0) {
                
                activateMapAddition(MAIN_MAP_INDEX, 1, 0);
                
                playSfx(DOOR_OPEN_SFX);
                
                gPlayer.direction = SOUTHWEST;
                gPlayer.actionTimer = 18;
                
                vec2 = getMovementVectorFromDirection(4.0f, 0, 0.0f);
                
                gPlayer.movementVector = vec2;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                
                gPlayer.animationHandler = ANIM_RUNNING;
                gPlayer.actionPhase++;

            } else {
                gPlayer.actionTimer--;
            }

            break;

        case 4:

            if (gPlayer.actionTimer == 0) {

                gPlayer.movementVector.x = 0;
                gPlayer.movementVector.y = 0;
                gPlayer.movementVector.z = 0;

                setEntityCollidable(ENTITY_PLAYER, TRUE);
                setEntityTracksCollisions(ENTITY_PLAYER, TRUE);
                enableEntityMovement(ENTITY_PLAYER, 1);
                setEntityYMovement(ENTITY_PLAYER, TRUE);
                activateMapAddition(MAIN_MAP_INDEX, 0, 0);

                resetAction();

                toggleDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);

                if (!checkDailyEventBit(0x33)) {
                    gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -10, MAX_FATIGUE_POINTS);
                }

                setDailyEventBit(0x33);

            } else {
                gPlayer.actionTimer--;
            }

            break;

    }

    setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 1.0f);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleWhistleForDogAction);

void handleWhistleForDogAction(void) {
    
    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        playSfx(0xA);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleWhistleForHorseAction);

void handleWhistleForHorseAction(void) {

    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        playSfx(WHISTLE);
    }

}
 
//INCLUDE_ASM("asm/nonmatchings/game/player", handleDrinkAction);

void handleDrinkAction(void) {

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, getItemStaminaIncreaseValue(gPlayer.heldItem), gMaximumStamina);
            gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -getItemFatigueReductionValue(gPlayer.heldItem), MAX_FATIGUE_POINTS);
        }
        
        if (gPlayer.actionPhaseFrameCounter == 30) {
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
            gPlayer.actionPhase++;
        }
        
        gPlayer.actionPhaseFrameCounter++;
        
    }
}

// empty function
void handleUnusedAction21(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleFishingAction);

void handleFishingAction(void) {

    if (gPlayer.actionPhase == 2) {

        if (!getRandomNumberInRange(0, 400)) {
            gPlayer.actionPhase = 3;
        }
    
    }
    
    if (gPlayer.actionPhase && checkButtonReleased(CONTROLLER_1, BUTTON_B)) {

        switch (gPlayer.actionPhase) {
            
            case 3 ... 5:
                gPlayer.actionPhase = 6;
                break;
            default:
                gPlayer.actionPhase = 8;
                break;

        }

    }
    
}

// empty function
void handleUnusedAction23(void) {}

// empty function
void handleUnusedAction24(void) {}

// empty function
void handleStaminaExhaustionAction(void) {} 

// empty function
void handleUnusedAction25(void) {}

void handleTreeClimbingAction(void) {

    Vec3f vec;

    if (gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhaseFrameCounter == 0) {
            
            playSfx(ITEM_PLUCK_SFX);

            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            setEntityTracksCollisions(ENTITY_PLAYER, FALSE);
            enableEntityMovement(ENTITY_PLAYER, FALSE);
            
            vec = getMovementVectorFromDirection(2.0f, gPlayer.direction, 0.0f);
            
            gPlayer.movementVector = vec;
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            
        }

        if (gPlayer.actionPhaseFrameCounter < 6) {
            
            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 2.0f);
            
            // parabolic equation: y = initial_y - gravity * (time - peak_time)²
            entities->coordinates.y = 
                ((gPlayer.movementVector.y 
                     + (((gPlayer.actionPhaseFrameCounter - 7) * (gPlayer.actionPhaseFrameCounter - 7)) * -0.8f)) 
                     + 39.2);
            
        } else {
            
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhaseFrameCounter = 0;
            gPlayer.actionPhase++;
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            setEntityShadow(ENTITY_PLAYER, 0xFF);
            
        }

         gPlayer.actionPhaseFrameCounter++;
        
    } 
    
    if (gPlayer.actionPhase == 1) {
        
        if (gPlayer.actionPhaseFrameCounter < 0x80) {
            
            gPlayer.movementVector.y += 1.0f;
            entities[ENTITY_PLAYER].coordinates.y = gPlayer.movementVector.y;
            
        } else {
            
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            
            gPlayer.actionPhaseFrameCounter = 0;
            gPlayer.actionPhase++;
            
            vec = getMovementVectorFromDirection(10.0f, NORTHEAST, 0.0f);
            
            gPlayer.movementVector = vec;
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;

            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(NORTHEAST, MAIN_MAP_INDEX));
            
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }

    if (gPlayer.actionPhase == 2) {
        
        if (gPlayer.actionPhaseFrameCounter < 15) {
            
            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 10.0f);

            // parabolic equation: y = initial_y - gravity * (time - peak_time)²
            entities->coordinates.y = 
                ((gPlayer.movementVector.y 
                     + (((gPlayer.actionPhaseFrameCounter - 7) * (gPlayer.actionPhaseFrameCounter - 7)) * -1.2f)) 
                     + 58.5);
            
        } else {
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhaseFrameCounter = 0;
            gPlayer.actionPhase++;
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
        }
        
        gPlayer.actionPhaseFrameCounter++;
        
    }

    if (gPlayer.actionPhase == 3) {
        
        setEntityCollidable(ENTITY_PLAYER, TRUE);
        setEntityYMovement(ENTITY_PLAYER, TRUE);
        resetAction();
        setEntityTracksCollisions(ENTITY_PLAYER, TRUE);
        enableEntityMovement(ENTITY_PLAYER, TRUE);
        setEntityShadow(ENTITY_PLAYER, 0);
        toggleDailyEventBit(SUSPEND_TIME_DURING_ANIMATION);
        
    }
    
}

// empty function
void handleToolAcquisitionAction(void) {} 
 


/* Animation handlers */


//INCLUDE_ASM("asm/nonmatchings/game/player", handlePlayerAnimation);

void handlePlayerAnimation(void) {

    switch (gPlayer.animationHandler) {

        case ANIM_DEFAULT:
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                if ((gPlayer.flags & PLAYER_RIDING_HORSE)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 516);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_2) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 580);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_2) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 580);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
                }
            } else {
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 24);
            }
            if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
                if (playerIdleCounter == 1200) {
                    startAction(IDLE_NAP, ANIM_IDLE_NAP);
                } else {
                    playerIdleCounter++;
                }
            }
            break;

        default:
            break;

        case ANIM_WALKING:
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                   setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 524);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 8);
                }
            } else {
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 32);
            }
            playerIdleCounter = 0;
            break;

        case ANIM_RUNNING:
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                        playSfx(9);
                    }
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 532);
                } else {
                    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                        playSfx(WALKING_SFX);
                    }
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 16);
                }
            } else {
                if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                    playSfx(WALKING_SFX);
                }
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 40);
            }
            playerIdleCounter = 0;
            break;

        case ANIM_TOOL_USE:
            handleToolAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_THROWING:
            handleThrowItemAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_SHIPPING:
            handleShippingAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_PICKING_UP_ITEM:
            handlePickUpAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_PLACING_ITEM:
            handlePlaceItemAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_EATING:
            handleEatAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_STAMINA_EXHAUSTION:
            handleStaminaExhaustionAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_GET_INTO_BED:
            handleGetIntoBedAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_IDLE_NAP:
            handleIdleNapAnimation();
            break;
        case ANIM_WATCHING_TV:
            handleTurnOnTVAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_SHOPPING:
            handleShopAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_JUMPING:
            handleJumpAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_MOUNT_HORSE:
            handleMountHorseAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_DISMOUNT_HORSE:
            handleDismountHorseAnimation();
            playerIdleCounter = 0;
            break;
        case 17:
            handleUnusedAnimation17();
            playerIdleCounter = 0;
            break;
        case 18:
            handleUnusedAnimation18();
            playerIdleCounter = 0;
            break;
        case 30:
            handleUnusedAnimation30();
            playerIdleCounter = 0;
            break;
        case ANIM_WHISTLE_FOR_DOG:
            handleWhistleForDogAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_WHISTLE_FOR_HORSE:
            handleWhistleForHorseAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_DRINKING:
            handleDrinkingAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_PUT_ITEM_IN_RUCKSACK:
            handlePutItemInRucksackAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_FISHING:
            handleFishingRodAnimation();
            playerIdleCounter = 0;
            break;
        case 24:
            handleUnusedAnimation24();
            playerIdleCounter = 0;
            break;
        case 25:
            handleUnusedAnimation25();
            playerIdleCounter = 0;
            break;
        case ANIM_FATIGUE_THRESHOLD:
            handleFatigueThresholdAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_CLIMBING_TREE:
            handleClimbTreeAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_TOOL_ACQUISITION:
            handleToolAcquisitionAnimation();
            playerIdleCounter = 0;
            break;
        case 29:
            handleUnusedAnimation29();
            playerIdleCounter = 0;
            break;
        case ANIM_DIALOGUE_SELECTING:
            handleDialogueSelectionAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_PUT_FOOD_IN_DOG_BOWL:
            handlePutFoodInDogBowlAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_DROP_ITEM_IN_WATER:
            handleDropItemInWaterAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_DRINK_ALCOHOL:
            handleDrinkAlcoholAnimation();
            playerIdleCounter = 0;
            break;
        case ANIM_ACQUIRE_MUSIC_BOX:
            handleAcquireMusicBoxAnimation();
            playerIdleCounter = 0;
            break;
    }
    
}

// probably not necessary
static inline void handleStopHolding() {
    resetAction();
    gItemBeingHeld = 0xFF;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleThrowItemAnimation);

void handleThrowItemAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhase == 0) {
        
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 421);
            gPlayer.actionPhase++;
        
        } else {

            handleStopHolding();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleShippingAnimation);

void handleShippingAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 421);
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_RAISED);
        } else {
            handleStopHolding();
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePickUpAnimation);

void handlePickUpAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
    
        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 429);
        } else {
            resetAction();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePlaceItemAnimation);

void handlePlaceItemAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {     

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 439);
                setItemState(gPlayer.itemInfoIndex, ITEM_STATE_PLACE);
                break;
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 447);
                break;
            case 2:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 439);
                break;
            default:
                handleStopHolding();
                break;

            }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleEatAnimation);

void handleEatAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {       

            case 0:
                // holding
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 24);
                break;
            case 1:
                setItemState(gPlayer.itemInfoIndex, ITEM_STATE_EATING);
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.actionPhase++;
                break;
            case 2:
                // chewing and swallowing
                setEntityAnimation(ENTITY_PLAYER, 437);
                break;
            default:
                handleStopHolding();
                break;

            }

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleStaminaExhaustionAnimation);

void handleStaminaExhaustionAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhase == 0) {
            
            switch (gPlayer.staminaExhaustionLevel) {       
                case 1:
                    setEntityAnimation(ENTITY_PLAYER, 464);
                    break;
                case 2:
                    setEntityAnimation(ENTITY_PLAYER, 465);
                    break;
                case 3:
                    setEntityAnimation(ENTITY_PLAYER, 466);
                    break;
                case 4:
                    setEntityAnimation(ENTITY_PLAYER, 467);
                    break;
            }
    
            gPlayer.actionPhase++;

        } else {
            resetAction();
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleFatigueThresholdAnimation);

// fatigue levels 1 and 2
void handleFatigueThresholdAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhase == 0) {

            switch (gPlayer.fatigueThreshold) {                   
                case 1:
                    setEntityAnimation(ENTITY_PLAYER, 665);
                    break;
                case 2:
                    setEntityAnimation(ENTITY_PLAYER, 666);
                    break;
                case 3:
                    break;
                }
            
            gPlayer.actionPhase++;
        
        } else {
            resetAction();
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleGetIntoBedAnimation);

void handleGetIntoBedAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {         

            case 1:

                setEntityAnimation(ENTITY_PLAYER, 468);
                gPlayer.actionPhase++;

                if (checkLifeEventBit(MARRIED) && (19 < gHour && gHour < 22)) {

                    switch (gWife) {
                        case MARIA:
                            npcs[MARIA].animationMode = NPC_ANIMATION_SLEEPING;
                            break;
                        case POPURI:
                            npcs[POPURI].animationMode = NPC_ANIMATION_SLEEPING;
                            break;
                        case ELLI:
                            npcs[ELLI].animationMode = NPC_ANIMATION_SLEEPING;
                            break;
                        case ANN:
                            npcs[ANN].animationMode = NPC_ANIMATION_SLEEPING;
                            break;
                        case KAREN:
                            npcs[KAREN].animationMode = NPC_ANIMATION_SLEEPING;
                            break;
                    }

                }

                break;

            case 2:
                setEntityAnimation(ENTITY_PLAYER, 469);
                gPlayer.actionPhase++;
                break;

            case 3:
                setEntityAnimation(ENTITY_PLAYER, 470);
                gPlayer.actionPhase++;
                break;             

            default:
                if (gPlayer.actionPhase >= 10) {
                    handleExitLevel(8, END_OF_DAY_1);    
                } else {
                    gPlayer.actionPhase++;
                }
                
                break;

         }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleIdleNapAnimation);

// napping/idle animation
void handleIdleNapAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) { 

            case 0:
                setEntityAnimation(ENTITY_PLAYER, 480);
                gPlayer.actionPhase++;
                break;
            case 1:
                setEntityAnimation(ENTITY_PLAYER, 481);
                gPlayer.actionPhase++;
                break;
            case 2:
                playerIdleCounter++;
                if (playerIdleCounter == 1800) {
                    setEntityAnimation(ENTITY_PLAYER, 482);
                    gPlayer.actionPhase++;
                }
                break;
            case 4:
                setEntityAnimation(ENTITY_PLAYER, 483);
                gPlayer.actionPhase++;
                break;
            default:
                resetAction();
                playerIdleCounter = 0;
                break;
            case 3:
                break;

        } 
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleTurnOnTVAnimation);

// turn on TV
void handleTurnOnTVAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) { 
            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 439);
                break;
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 447);
                gPlayer.actionPhase++;
                break;
            case 3:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 447);
                break;            
            case 4:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 439);
                gPlayer.actionPhase++;
                break;           
            default:
                resetAction();
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleShopAnimation);

void handleShopAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {  
            case 0:
                if (!checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 429);
                }
                break;
            case 1:
                intializeShopDialogue(gPlayer.shopItemIndex);
                gPlayer.actionPhaseFrameCounter = 0;
                pauseGameplay();
                break;
            case 2:
                if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 429);
                }
                break;
            case 3:
                if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 429);
                }
                break;
            default:
                resetAction();
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleJumpAnimation);

void handleJumpAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.actionPhase == 0)) {
        
        switch (gPlayer.actionPhase) {                          
        
            case 0:
                
                if (gPlayer.heldItem != 0) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 64);
                } else if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 548);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 0x23C);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 0x241);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 0x23D);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 0x240);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 48);
                }
                
                break;
                
            case 1:
                
                if (gPlayer.heldItem != 0) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 72);
                } else if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 556);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 574);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 579);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 575);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 578);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 56);
                }

                break;
            
            default:
                
                if (gPlayer.flags & PLAYER_BATH_POSE_1) {
                    
                    startAction(BATHING, ANIM_DEFAULT);

                    gPlayer.direction = SOUTHWEST;
                    
                    gPlayer.flags &= ~PLAYER_BATH_POSE_1; 
                    gPlayer.flags |= PLAYER_BATH_POSE_2;
                    
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 3;

                    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                    playSfx(40);
                      
                } else if (gPlayer.flags & PLAYER_BATH_POSE_3) {
                    
                    startAction(BATHING, ANIM_DEFAULT);
                    
                    gPlayer.flags &= ~(PLAYER_BATH_POSE_2 | PLAYER_BATH_POSE_3);
                                        
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 4;
                    
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_1) {
                    
                    startAction(USING_HOT_SPRINGS, ANIM_DEFAULT);

                    gPlayer.direction = SOUTHWEST;
                    
                    gPlayer.flags &= ~PLAYER_HOT_SPRING_POSE_1;
                    gPlayer.flags |= PLAYER_HOT_SPRING_POSE_2;
                    
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 2;
                    
                    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                    playSfx(40);
                                        
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_3) {
                
                    startAction(USING_HOT_SPRINGS, ANIM_DEFAULT);
                    
                    gPlayer.flags &= ~(PLAYER_HOT_SPRING_POSE_2 | PLAYER_HOT_SPRING_POSE_3); 
    
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 3;
                
            } else {
                
                setEntityCollidable(ENTITY_PLAYER, TRUE);
                setEntityTracksCollisions(ENTITY_PLAYER, TRUE);
                enableEntityMovement(ENTITY_PLAYER, TRUE);
                setEntityYMovement(ENTITY_PLAYER, TRUE);
                
                resetAction();
                
            }

        }

    }
    
}

// empty function
void handleUnusedAnimation29(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDialogueSelectionAnimation);

// dialogue selecting
void handleDialogueSelectionAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimation(ENTITY_PLAYER, 475);
            
            gPlayer.actionPhase = 1;
            gPlayer.heldItem = 202;
            gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_DIALOGUE_SELECTION, 202, 0, ITEM_CONTEXT_USE_ATTACHMENT);
            
        } else {
            setMainLoopCallbackFunctionIndex(DIALOGUE_SELECTION);
        }

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePutFoodInDogBowlAnimation);

void handlePutFoodInDogBowlAnimation(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 439);
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_DOG_BOWL);
            
        } else {
            resetAction();
            gItemBeingHeld = 0xFF;
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDropItemInWaterAnimation);

void handleDropItemInWaterAnimation(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 421);
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_RAISED);
        } else {

            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            gItemBeingHeld = 0xFF;

            if (!checkDailyEventBit(HARVEST_GODDESS_OFFERING) && !checkDailyEventBit(0x47) && !checkDailyEventBit(0x48)) {
                resetAction();
            }

        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDrinkAlcoholAnimation);

void handleDrinkAlcoholAnimation(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        if (gPlayer.actionPhase == 0) {
            setEntityAnimation(ENTITY_PLAYER, 691);
            gPlayer.actionPhase++;
        } else {
            resetAction();
        }
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleAcquireMusicBoxAnimation);

void handleAcquireMusicBoxAnimation(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        switch (gPlayer.actionPhase) {

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 24);
                break;
            
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 618);
                setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
                gPlayer.actionPhase++;
                break;
            
            default:
                if (gPlayer.heldItem == BROKEN_MUSIC_BOX_HELD_ITEM) {
                    acquireKeyItem(BROKEN_MUSIC_BOX);
                    setSpecialDialogueBit(HAVE_BROKEN_MUSIC_BOX_DIALOGUE);
                }
    
                gPlayer.heldItem = 0;
                resetAction();
                gItemBeingHeld = 0xFF;
    
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleMountHorseAnimation);

void handleMountHorseAnimation(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
             setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 540);
        }

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDismountHorseAnimation);

void handleDismountHorseAnimation(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
             setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 604);
        }

    }
    
}

// empty function
void handleUnusedAnimation17(void) {}

// empty function
void handleUnusedAnimation18(void) {}

// empty function
void handleUnusedAnimation30(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleWhistleForDogAnimation);

void handleWhistleForDogAnimation(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 413);
            gPlayer.actionPhase++;
        } else {
            resetAction();
            handleWhistleForDog();
        }

    }

}


//INCLUDE_ASM("asm/nonmatchings/game/player", handleWhistleForHorseAnimation);

void handleWhistleForHorseAnimation(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 413);
            gPlayer.actionPhase++;
        } else {
            resetAction();
            handleWhistleForHorse();
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleDrinkingAnimation);

void handleDrinkingAnimation(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        switch (gPlayer.actionPhase) {

            case 0:
                // carrying
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 24);
                break;
            
            case 1:

                if (getItemFlags(gPlayer.heldItem) & ITEM_HAS_ALCOHOL) {
                    setEntityAnimation(ENTITY_PLAYER, 691);
                    gAlcoholTolerance += adjustValue(gAlcoholTolerance, 1, MAX_ALCOHOL_TOLERANCE);
                } else {
                    setEntityAnimation(ENTITY_PLAYER, 438);
                }
                
                gPlayer.heldItem = 0;
                gPlayer.actionPhase++;
                break;
            
            default:
                resetAction();
                gItemBeingHeld = 0xFF;
                break;
            
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePutItemInRucksackAnimation);

void handlePutItemInRucksackAnimation(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 618);
            setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
            gPlayer.heldItem = 0;
            gPlayer.actionPhase++;
            playSfx(PICKING_UP_SFX);
            
        } else {
            resetAction();
            gItemBeingHeld = 0xFF;
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleFishingRodAnimation);

void handleFishingRodAnimation(void) {

    Vec3f vec;
    u16 temp;
    s32 temp2;
    int max;

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        switch (gPlayer.actionPhase) {
            
            // cast
            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 386);
                gPlayer.actionPhase++;
                break;
            
            // fishing
            case 1:
                
                if (checkFishingSpotInteraction(gBaseMapIndex)) {
                
                    vec = getOffsetTileCoordinates (2.0f, 8);
                    vec = getGroundObjectWorldPosition(0, vec.x, vec.z);

                    switch (gPlayer.fishingSpotType) {
                        case 0:
                            vec.y = 36.0f;
                            break;
                        case 1:
                            vec.y = 4.0f;
                            break;
                        case 2:
                            vec.y = 4.0f;
                            break;
                        case 3:
                            vec.y = 128.0f;
                            break; 
                    } 

                    spawnFishingRodEntity(0, 157, vec.x, vec.y, vec.z);
                    playSfx(FISHING_ROD_CAST);
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 378);
                    gPlayer.actionPhase = 2;
                
                } else {
                    // end
                    gPlayer.actionPhase = 8;
                }
                
                break;
            
            // fish bite/reel in
            case 3:
                setItemEntityAnimation(0, 158);
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 394);
                gPlayer.actionPhase = 4;
                break;
            
            case 4:
                gPlayer.actionPhase = 5;
                break;
            
            // reset to fishing
            case 5:
                gPlayer.actionPhase = 1;
                break;
            
            // catch fish
            case 7:

                switch (gPlayer.fishingSpotType) {
                    case 0:
                        temp = 1;
                        break;
                    case 1 ... 4:
                        temp = 2;
                        break;
                }
                
                // randomize fish type
                switch (getRandomNumberInRange(0, temp)) {
                    case 0:
                        gPlayer.heldItem = SMALL_FISH;
                        break;
                    case 1:
                        gPlayer.heldItem = MEDIUM_FISH;
                        break;
                    case 2:
                        gPlayer.heldItem = LARGE_FISH;
                        break;                    
                }

                gPlayer.actionPhase = 9;

                gHappiness += adjustValue(gHappiness, 1, MAX_HAPPINESS);

                // get power nut 
                if (gPlayer.fishingSpotType == 3 && !(acquiredPowerNutBits & FISHING_POWER_NUT) && !getRandomNumberInRange(0, 50)) {
                    
                    gPlayer.heldItem = POWER_NUT;

                    // eating
                    startAction(EATING, ANIM_EATING);
                    
                    acquiredPowerNutBits |= FISHING_POWER_NUT;
                    
                    gMaximumStamina += adjustValue(gMaximumStamina, 15, MAX_STAMINA);
                    gHappiness += adjustValue(gHappiness, 4, MAX_HAPPINESS);


                }

                totalFishCaught += adjustValue(totalFishCaught, 1, MAX_FISH_CAUGHT);

                initializePlayerHeldItem();
                
                break;
            
            case 6:
            case 8:
                deactivateItemEntity(0);
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 402);
                // catch fish if phase 6, otherwise end
                gPlayer.actionPhase++;
                break;
              
            default:            
                resetAction();
            case 2:
                break;
        }
    }

    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        playSfx(11);
    }
    
}

// empty function
void handleUnusedAnimation24(void) {}

// empty function
void handleUnusedAnimation25(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleClimbTreeAnimation);

void handleClimbTreeAnimation(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0 || gPlayer.actionPhase == 2) {
        switch (gPlayer.actionPhase) {
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 667);
                break;
            case 0:
            case 2:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 48);
                break;
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToolAcquisitionAnimation);

void handleToolAcquisitionAnimation(void) {

    u8 temp;

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {

            case 0:
                setEntityAnimation(ENTITY_PLAYER, 0x171);
                gPlayer.actionPhase = 1;
                temp = toolHeldItemIndices[upgradedToolIndex][upgradedToolLevelIndex - 3];
                gPlayer.heldItem = temp;
                gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_TOOL_ACQUIRED, temp, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            
            case 1:
                gPlayer.actionPhase = 2;
                break;
            
            case 2:
                gPlayer.actionPhase = 3;
                playSfx(0x5A);
                setMainLoopCallbackFunctionIndex(END_CUTSCENE);
                pauseGameplay();
                break;

            default:
                setItemState(gPlayer.itemInfoIndex, ITEM_STATE_CLEANUP);
                gPlayer.heldItem = 0;
                resetAction();
                break;
            
        }
        
    }
    
}

//INCLUDE_RODATA("asm/nonmatchings/game/player", D_8011F5D4);

// for displaying upgraded tool
static const u8 toolHeldItemIndices[5][3] = {
    { SICKLE_HELD_ITEM, SILVER_SICKLE_HELD_ITEM, GOLDEN_SICKLE_HELD_ITEM },
    { HOE_HELD_ITEM, SILVER_HOE_HELD_ITEM, GOLDEN_HOE_HELD_ITEM },
    { AX_HELD_ITEM, SILVER_AX_HELD_ITEM, GOLDEN_AX_HELD_ITEM },
    { HAMMER_HELD_ITEM, SILVER_HAMMER_HELD_ITEM, GOLDEN_HAMMER_HELD_ITEM },
    { WATERING_CAN_HELD_ITEM, SILVER_WATERING_CAN_HELD_ITEM, GOLDEN_WATERING_CAN_HELD_ITEM }
};

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToolAnimation);

void handleToolAnimation(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.actionPhase == 0)) {
        
        switch (gPlayer.currentTool) {                          
            case SICKLE:                                     
                handleSickleAnimation();
                break;
            case HOE:                                     
                handleHoeAnimation();
                break;
            case AX:                                     
                handleAxAnimation();
                break;
            case HAMMER:                                     
                handleHammerAnimation();
                break;
            case WATERING_CAN:                                     
                handleWateringCanAnimation();
                break;
            case MILKER:                                     
                handleMilkerAnimation();
                break;
            case BELL:                                     
                handleBellAnimation();
                break;
            case BRUSH:                                     
                handleBrushAnimation();
                break;
            case CLIPPERS:                                     
                handleClippersAnimation();
                break;
            case TURNIP_SEEDS:                                    
                handleTurnipSeedsAnimation();
                break;
            case POTATO_SEEDS:                                    
                handlePotatoSeedsAnimation();
                break;
            case CABBAGE_SEEDS:                                    
                handleCabbageSeedsAnimation();
                break;
            case TOMATO_SEEDS:                                    
                handleTomatoSeedsAnimation();
                break;
            case CORN_SEEDS:                                    
                handleCornSeedsAnimation();
                break;
            case EGGPLANT_SEEDS:                                    
                handleEggplantSeedsAnimation();
                break;
            case STRAWBERRY_SEEDS:                                    
                handleStrawberrySeedsAnimation();
                break;
            case MOON_DROP_SEEDS:                                    
                handleMoonDropSeedsAnimation();
                break;
            case PINK_CAT_MINT_SEEDS:                                    
                handlePinkCatMintSeedsAnimation();
                break;
            case BLUE_MIST_SEEDS:                                    
                handleBlueMistSeedsAnimation();
                break;
            case CHICKEN_FEED:                                    
                handleChickenFeedAnimation();
                break;
            case BALL:                                    
                handleBallAnimation();
                break;
            case FEEDING_BOTTLE:                                    
                handleFeedingBottleAnimation();
                break;
            case 23:                                    
                handleUnusedTool23Animation();
                break;
            case FISHING_POLE:                                    
                handleFishingPoleAnimation();
                break;
            case MIRACLE_POTION:                                    
                handleMiraclePotionAnimation();
                break;
            case COW_MEDICINE:                                    
                handleCowMedicineAnimation();
                break;
            case GRASS_SEEDS:                                    
                handleGrassSeedsAnimation();
                break;
            case BLUE_FEATHER:                                    
                handleBlueFeatherAnimation();
                break;
            case EMPTY_BOTTLE:                                    
                handleEmptyBottleAnimation();
                break;
        }
    }
    
    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        
        switch (gPlayer.currentTool) {         

            case SICKLE:
            case HOE:
            case AX:
            case HAMMER:
            case WATERING_CAN:
                break;
            case MILKER:                                     
                playSfx(MILKER_SFX);
                break;
            case BELL:                                     
                playSfx(BELL_SFX);
                break;
            case BRUSH:                                     
                playSfx(BRUSH_SFX);
                break;
            case CLIPPERS:                                     
                playSfx(CLIPPERS_SFX);
                break;
            case CHICKEN_FEED:                                    
                playSfx(SEEDS_SFX);
                break;
            case MIRACLE_POTION:                                    
            case COW_MEDICINE:                                    
            case BLUE_FEATHER:                                    
                playSfx(SHIMMER_SFX);
                break;
            case EMPTY_BOTTLE:
            default:
                break;

        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToolUseAnimation);

void handleToolUseAnimation(u16 chargedAnimationIndex, u16 useToolAnimationIndex) {

    u8 found;
    u8 found2;

    switch (gPlayer.actionPhase) {

        // charge/prepare tool
        case 0:

            toggleDailyEventBit(0x29);
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, chargedAnimationIndex);

            switch (gPlayer.currentTool) {
                case SICKLE:
                case HOE:
                case AX:
                case HAMMER:
                    handleFarmAnimalPlayerCollision();
                    handleChickenPlayerCollision();
                    handleDogPlayerCollision();
                    handleHorsePlayerInteraction();
                    break;
            }

            break;
        
        // use tool
        case 1:                                         
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, useToolAnimationIndex);
            resetToolUseState();
            break;

        // post use
        case 2:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                switch (gPlayer.currentTool) {

                    case SICKLE:
                        
                        if (!(gCutsceneFlags & 1)) {
                            
                            if (gPlayer.toolUseCounters[0] != 400) {
                                
                                gPlayer.toolUseCounters[0]++;
                                
                                if (gPlayer.toolUseCounters[0] == 200) {
                                    
                                    gPlayer.toolLevels[0] = 1;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 1;
                                    upgradedToolLevelIndex = 1;
                                    
                                }
                                
                                if (gPlayer.toolUseCounters[0] == 400) {
                                    
                                    gPlayer.toolLevels[0] = 2;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 1;
                                    upgradedToolLevelIndex = 2;
                                    
                                }
                                
                            }
                            
                        }
                        
                        break;

                    case HOE:
                        
                        if (!(gCutsceneFlags & 1)) {

                            if (gPlayer.toolUseCounters[1] != 500) {
                                
                                gPlayer.toolUseCounters[1]++;

                                if (gPlayer.toolUseCounters[1] == 250) {
                                    
                                    gPlayer.toolLevels[1] = 1;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 2;
                                    upgradedToolLevelIndex = 1;
                                    
                                }
                                
                                if (gPlayer.toolUseCounters[1] == 500) {
                                    
                                    gPlayer.toolLevels[1] = 2;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 2;
                                    upgradedToolLevelIndex = 2;
                                    
                                }

                            } 
                            
                            if (toolUse.musicBoxTileDigCounter == 10) {

                                if (checkHaveKeyItem(TREASURE_MAP) && !checkHaveKeyItem(BROKEN_MUSIC_BOX)) {
                                    gPlayer.heldItem = BROKEN_MUSIC_BOX_HELD_ITEM;
                                    startAction(ACQUIRING_MUSIC_BOX, ANIM_ACQUIRE_MUSIC_BOX);
                                }
                                
                                toolUse.musicBoxTileDigCounter = 0;

                            } else if (checkDailyEventBit(0x29) && !(acquiredPowerNutBits & 1) && !(getRandomNumberInRange(0, 479))) {
                            
                                gPlayer.heldItem = POWER_NUT;
                                startAction(EATING, ANIM_EATING);
                                initializePlayerHeldItem();
                                acquiredPowerNutBits |= 1;
                                gMaximumStamina += adjustValue(gMaximumStamina, 15, MAX_STAMINA);
                                
                            }

                        }
    
                        toggleDailyEventBit(0x29);
                        break;

                    case AX:

                        if (!(gCutsceneFlags & 1)) {
                            
                            if (gPlayer.toolUseCounters[2] != 900) {

                                gPlayer.toolUseCounters[2]++;
                                
                                if (gPlayer.toolUseCounters[2] == 450) {
                                
                                    gPlayer.toolLevels[2] = 1;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 3;
                                    upgradedToolLevelIndex = 1;
                                
                                }
                                
                                if (gPlayer.toolUseCounters[2] == 900) {
                                    
                                    gPlayer.toolLevels[2] = 2;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 3;
                                    upgradedToolLevelIndex = 2;
                                    
                                }
                                
                            }
                       
                        }
                        
                        break;

                    case HAMMER:

                        if (!(gCutsceneFlags & 1)) {
                            
                            if (gPlayer.toolUseCounters[3] != 200) {

                                gPlayer.toolUseCounters[3]++;
                                
                                if (gPlayer.toolUseCounters[3] == 100) {
                                
                                    gPlayer.toolLevels[3] = 1;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 4;
                                    upgradedToolLevelIndex = 1;
                                
                                }
                                
                                if (gPlayer.toolUseCounters[3] == 200) {
                                    
                                    gPlayer.toolLevels[3] = 2;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 4;
                                    upgradedToolLevelIndex = 2;
                                    
                                }
                            
                            }
                            
                            if (checkDailyEventBit(0x29) && !(acquiredPowerNutBits & 0x10)) {
                                
                                gPlayer.heldItem = 0x57;
                                // eating
                                startAction(EATING, ANIM_EATING);
                                initializePlayerHeldItem();
                                acquiredPowerNutBits |= 0x10;
                                
                                gMaximumStamina += adjustValue(gMaximumStamina, 15, 250);
                                setLifeEventBit(0x45);
                                
                            }
                        
                        }
                        
                        toggleDailyEventBit(0x29);
                        break;
                    
                    case WATERING_CAN:

                        if (!(gCutsceneFlags & 1)) {

                            if (gPlayer.toolUseCounters[4] != 1200) {

                                gPlayer.toolUseCounters[4]++;
                                
                                if (gPlayer.toolUseCounters[4] == 600) {
                                
                                    gPlayer.toolLevels[4] = 1;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 5;
                                    upgradedToolLevelIndex = 1;
                                
                                }
                                
                                if (gPlayer.toolUseCounters[4] == 1200) {
                                    
                                    gPlayer.toolLevels[4] = 2;
                                    // tool acquisition
                                    startAction(ACQUIRING_TOOL, ANIM_TOOL_ACQUISITION);
                                    upgradedToolIndex = 5;
                                    upgradedToolLevelIndex = 2;
                                    
                                }
                                
                            }
                       
                        }

                        break;
                    
                }
                
            }
            
        break;
        
    }
    
}


// tool animations

//INCLUDE_ASM("asm/nonmatchings/game/player", handleSickleAnimation);

void handleSickleAnimation(void) {
    
    switch(gPlayer.currentToolLevel) {

        case 0:
            handleToolUseAnimation(80, 104);
            break;
        case 1:
            handleToolUseAnimation(88, 112);
            break;
        case 2:
            handleToolUseAnimation(96, 120);
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleHoeAnimation);

void handleHoeAnimation(void) {
    
    switch(gPlayer.currentToolLevel) {

        case 0:
            handleToolUseAnimation(128, 152);
            break;
        case 1:
            handleToolUseAnimation(136, 160);
            break;
        case 2:
            handleToolUseAnimation(144, 168);
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleAxAnimation);

void handleAxAnimation(void) {

    switch(gPlayer.currentToolLevel) {

        case 0:
            handleToolUseAnimation(176, 200);
            break;
        case 1:
            handleToolUseAnimation(184, 208);
            break;
        case 2:
            handleToolUseAnimation(192, 216);
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleHammerAnimation);

void handleHammerAnimation(void) {

    switch (gPlayer.currentToolLevel) {
        
        case 0:
            handleToolUseAnimation(224, 248);
            break;      
        case 1:
            handleToolUseAnimation(232, 256);
            break;
        case 2:
            handleToolUseAnimation(240, 264);
            break;        
        default:
            break;
    }
    
}

static inline void handleSuccessfulWateringCanAction() {

    u8 temp;
    
    switch (gPlayer.actionPhase) {
                    
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 320);
            break;
        
        case 1:

            if (toolUse.toolUseState == 0) {

                temp = gPlayer.toolLevels[4];

                if (temp != 1) {
                    
                    switch (temp) {

                        case 0:
                            wateringCanUses = 30;
                            break;
                        
                        case 1:
                            break;
                        
                        case 2:
                            wateringCanUses = 80;
                            break;

                    }
                    
                } else {
                    wateringCanUses = 50;
                }

                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }
                
            }
            
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleWateringCanAnimation);

void handleWateringCanAnimation(void) {

    u8 temp;
    
    switch (gPlayer.currentToolLevel) {                              
        
        case 0:

            if (checkWaterableSoilInteraction(gBaseMapIndex)) {
                handleSuccessfulWateringCanAction();
            } else {
                handleToolUseAnimation(272, 296);
                break;
            }
            
            break;
        
        case 1:
            handleToolUseAnimation(280, 304);
            break;

        case 2:
            handleToolUseAnimation(288, 312);
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleMilkerAnimation);

void handleMilkerAnimation(void) {

    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 329);
            break;
        
        case 2:
            // reset tool use struct
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if (toolUse.toolUseState == 0) {

                if (gPlayer.heldItem == 0) {

                    if (!(checkAndTriggerStaminaExhaustion(0))) {
                        resetAction();
                    }
                    
                } else {

                    if (!(checkAndTriggerStaminaExhaustion(0))) {
                        startAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                    } else {
                        gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_PICKUP, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                    }
                    
                }
                
            }
        
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleBellAnimation);

void handleBellAnimation(void) {
    
    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 337);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if ((toolUse.toolUseState == 0) && !(checkAndTriggerStaminaExhaustion(0))) {
                resetAction();
            }
        
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleBrushAnimation);

void handleBrushAnimation(void) {

    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            handleHorsePlayerInteraction();
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 345);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if ((toolUse.toolUseState == 0) && !(checkAndTriggerStaminaExhaustion(0))) {
                resetAction();
            }
        
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleClippersAnimation);

void handleClippersAnimation(void) {

    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            break;

        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 353);
            break;
    
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if (toolUse.toolUseState == 0) {
                
                if (gPlayer.heldItem == 0) {
                    
                    if (!(checkAndTriggerStaminaExhaustion(0))) {
                        resetAction();
                    }
                    
                } else {
                    
                    if (!(checkAndTriggerStaminaExhaustion(0))) {
                        startAction(PICKING_UP_ITEM, ANIM_PICKING_UP_ITEM);
                    } else {
                        gPlayer.itemInfoIndex = initializeHeldItem(0, ITEM_STATE_PICKUP, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                    }
                    
                    
                }
                
            } 
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleTurnipSeedsAnimation);

void handleTurnipSeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                turnipSeedsQuantity += adjustValue(turnipSeedsQuantity, -1, 20);

                if (!turnipSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePotatoSeedsAnimation);

void handlePotatoSeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                potatoSeedsQuantity += adjustValue(potatoSeedsQuantity, -1, 20);

                if (!potatoSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleCabbageSeedsAnimation);

void handleCabbageSeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                cabbageSeedsQuantity += adjustValue(cabbageSeedsQuantity, -1, 20);

                if (!cabbageSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }

            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleTomatoSeedsAnimation);

void handleTomatoSeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                tomatoSeedsQuantity += adjustValue(tomatoSeedsQuantity, -1, 20);

                if (!tomatoSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleCornSeedsAnimation);

void handleCornSeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                cornSeedsQuantity += adjustValue(cornSeedsQuantity, -1, 20);

                if (!cornSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleEggplantSeedsAnimation);

void handleEggplantSeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                eggplantSeedsQuantity += adjustValue(eggplantSeedsQuantity, -1, 20);

                if (!eggplantSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleStrawberrySeedsAnimation);

void handleStrawberrySeedsAnimation(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                strawberrySeedsQuantity += adjustValue(strawberrySeedsQuantity, -1, 20);

                if (!strawberrySeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleMoonDropSeedsAnimation);

void handleMoonDropSeedsAnimation(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                moondropSeedsQuantity += adjustValue(moondropSeedsQuantity, -1, 20);

                if (!moondropSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePinkCatMintSeedsAnimation);

void handlePinkCatMintSeedsAnimation(void) {

        switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                pinkCatMintSeedsQuantity += adjustValue(pinkCatMintSeedsQuantity, -1, 20);

                if (!pinkCatMintSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleBlueMistSeedsAnimation);

void handleBlueMistSeedsAnimation(void) {

    switch (gPlayer.actionPhase) {
        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        case 3:            
            if (toolUse.toolUseState == 0) {
                if (!checkAndTriggerStaminaExhaustion(0)) {
                    resetAction();
                }

                blueMistSeedsQuantity += adjustValue(blueMistSeedsQuantity, -1, 1);

                if (!blueMistSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
            }
            break;
        default:
            break;
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleChickenFeedAnimation);

void handleChickenFeedAnimation(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            break;

        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 361);
            break;

        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {

                if (!checkAndTriggerStaminaExhaustion(0)) {
                    resetAction();
                }

                chickenFeedQuantity += adjustValue(chickenFeedQuantity, -1, 999);
                
                if (!chickenFeedQuantity) {
                    gPlayer.currentTool = 0;
                }

            }

            break;

        default:
            break;
    }
}

// empty function
void handleBallAnimation(void) {}

// empty function
void handleFeedingBottleAnimation(void) {}

// empty function
void handleUnusedTool23Animation(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleFishingPoleAnimation);

void handleFishingPoleAnimation(void) {
    startAction(FISHING, ANIM_FISHING);
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleMiraclePotionAnimation);

void handleMiraclePotionAnimation(void) {

    switch (gPlayer.actionPhase) {                           
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 564);
            break;
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        case 3:
            if (toolUse.toolUseState == 0) {
                if (!checkAndTriggerStaminaExhaustion(0)) {
                    resetAction();
                }
                if (checkDailyEventBit(0x14)) {
                    gPlayer.currentTool = 0;
                }
            }
            break;
        default:
            break;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleCowMedicineAnimation);

void handleCowMedicineAnimation(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 564);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                if (checkDailyEventBit(0x14)) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleGrassSeedsAnimation);

void handleGrassSeedsAnimation(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 328);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(checkAndTriggerStaminaExhaustion(0))) {
                    resetAction();
                }

                grassSeedsQuantity += adjustValue(grassSeedsQuantity, -1, 20);

                if (!grassSeedsQuantity) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleBlueFeatherAnimation);

void handleBlueFeatherAnimation(void) {

    switch (gPlayer.actionPhase) {
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
            findNPCToTalkTo();
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 564);
            break;
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        case 3:
            if (toolUse.toolUseState == 0 && !checkAndTriggerStaminaExhaustion(0)) {
                resetAction();
            }
            break;
        default:
            break;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleEmptyBottleAnimation);

void handleEmptyBottleAnimation(void) {

    switch (gPlayer.actionPhase) {

        // handle start fill up or drink
        case 0:

            if (gPlayer.bottleContents == 0) {
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
                handleFarmAnimalPlayerCollision();
                
            } else {

                // drink
                startAction(DRINKING, ANIM_DRINKING);
                
                switch (gPlayer.bottleContents) {
                    
                    case 1:
                        gPlayer.heldItem = BOTTLE_WITH_WATER_HELD_ITEM;
                        break;
                    case 2:
                        gPlayer.heldItem = BOTTLE_WITH_WINE_HELD_ITEM;
                        break;
                    case 3:
                        gPlayer.heldItem = 0x73;
                        break;
                    case 4:
                        gPlayer.heldItem = 0x74;
                        break;
                    case 5:
                        gPlayer.heldItem = 0x75;
                        break;
                    case 6:
                        gPlayer.heldItem = 0x76;
                        break;
                    case 7:
                        gPlayer.heldItem = 0x77;
                        break;
                    case 0:
                    default:
                        break;
                    
                }
                
                initializePlayerHeldItem();

                gPlayer.bottleContents = 0;       

            }
            
            break;

        // fill up bottle
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 626);
            break;

        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:
            if (toolUse.toolUseState == 0 && !checkAndTriggerStaminaExhaustion(0)) {
                resetAction();
            }    
            break;

        default:
            break;
    
    }

}
