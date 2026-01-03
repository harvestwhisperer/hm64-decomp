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


// data
// indexed by gEntranceIndex
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

// indexed by gEntranceIndex
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

// FIXME: probably shouldn't be volatile
volatile u8 D_8011421C[MAX_TOOLS][3] = {
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
u8 func_80067A24(u8);                 
bool func_80067B38(void);
void handleToolCharging(void);
void handleToolUseAction(void);
void handleThrowingAction(void);
void func_80067F50(void);
void func_8006807C(void);
void func_80068120(void);
void func_80068258(void);
void func_800682F8(void);
void func_80068340(void);
void func_80068410(void);
void func_80068558(void);
void func_80068A98(void);
void func_80068C8C(void);
void func_80068DFC(void);
void func_80068FB0(void);
void func_800692E4(void);
void func_80069830(void);
void func_80069C5C(void);
void handleWhistleForDogAction(void);
void handlehandleWhistleForHorseAction(void);
void func_80069DA8(void);
void handlePuttingItemInRucksackAction(void);
void handleCastingFishingRodActionStub(void);
void func_80069E5C(void);
void func_80069E64(void);
void func_80069E6C(void);
void func_80069E74(void);
void func_8006A2E0(void);
void func_80068738(void);
void func_80068918(void);
void func_80068920(void);
void func_80068990(void);
void func_80068A00(void);
void func_80068A08(void);
void func_8006A714(void);
void func_8006C384(void);
void func_8006A7B0(void);
void func_8006A848(void);
void func_8006A8C4(void);
void func_8006A9A8(void);
void func_8006AA9C(void);
void func_8006AB90(void);
void func_8006ADF4(void);
void func_8006B4D4(void);
void func_8006B4DC(void);
void func_8006B584(void);
void func_8006B61C(void);
void func_8006B6EC(void);
void func_8006B77C(void);
void func_8006AC4C(void);
void func_8006AEEC(void);
void func_8006AFE4(void);
void func_8006B104(void);
void func_8006B8BC(void);
void func_8006B910(void);
void func_8006B964(void);
void func_8006B96C(void);
void func_8006B974(void);
void func_8006B97C(void);
void func_8006BA14(void);
void func_8006BAAC(void);
void func_8006BBC4(void);
void handleFishingRodUse(void);
void func_8006C12C(void);
void func_8006C134(void);
void func_8006C13C(void);
void func_8006C1DC(void);
void func_8006C628(u16, u16);
void func_8006CD84();                                  
void func_8006CDF8();                                  
void func_8006CE6C();                                  
void func_8006CEE0();                                  
void func_8006CF54();                                  
void func_8006D0AC();                                  
void func_8006D21C();                                  
void func_8006D304();                                  
void func_8006D3FC();                                  
void func_8006D56C();                                  
void func_8006D690();                                  
void func_8006D7B4();                                  
void func_8006D8D8();                                  
void func_8006D9FC();                                  
void func_8006DB20();                                  
void func_8006DC44();                                  
void func_8006DD68();                                  
void func_8006DE8C();                                  
void func_8006DFB0();                                  
void func_8006E0D4();                                  
void func_8006E1F8();                                  
void func_8006E200();                                  
void func_8006E208();                                  
void func_8006E210();                                  
void func_8006E240();                                  
void func_8006E348();                                  
void func_8006E450();                                  
void func_8006E574();                                  
void func_8006E678();                                  

static inline void resetAction() {
    gPlayer.actionPhaseFrameCounter = 0;
    gPlayer.actionPhase = 0;
    gPlayer.actionHandler = 0;
    gPlayer.animationHandler = 0;
}

static inline void startAction(u8 arg0, u8 arg1) {
    gPlayer.actionHandler = arg0;
    gPlayer.actionPhaseFrameCounter = 0;
    gPlayer.actionPhase = 0;
    gPlayer.animationHandler = arg1;
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

void setupPlayerEntity(u16 entranceIndex, u8 resetPlayer) {
 
    loadEntity(ENTITY_PLAYER, 0, TRUE);

    setEntityCollidable(ENTITY_PLAYER, TRUE);
    setEntityYMovement(ENTITY_PLAYER, TRUE);
    setCameraTrackingEntity(ENTITY_PLAYER, TRUE);
    setEntityMapSpaceIndependent(ENTITY_PLAYER, TRUE);

    setEntityColor(ENTITY_PLAYER, globalLightingRGBA.r, globalLightingRGBA.g, globalLightingRGBA.b, globalLightingRGBA.a);
    setEntityShadow(ENTITY_PLAYER, 0);

    if (resetPlayer) {
        gPlayer.coordinates.x = playerDefaultStartingCoordinates[entranceIndex].x;
        gPlayer.coordinates.y = playerDefaultStartingCoordinates[entranceIndex].y;
        gPlayer.coordinates.z = playerDefaultStartingCoordinates[entranceIndex].z;
        gPlayer.direction = playerDefaultStartingDirections[entranceIndex];
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
                gPlayer.itemInfoIndex = initializeHeldItem(0, 2, gPlayer.heldItem, ITEM_CONTEXT_HAS_DIRECTION_FRAME, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            default:
                gPlayer.itemInfoIndex = initializeHeldItem(0, 2, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            
        }
        
        setItemAttachmentOffset(0, 0, 14.0f, 20.0f);
        
    } else {
        gItemBeingHeld = 0xFF;
        clearHeldItemSlot(gPlayer.itemInfoIndex);
    }
    
}

// m2c decomp of ranged switch for reference:
/*
        if ((s32) D_8018908C < 0xB3) {
            if ((s32) D_8018908C < 0x7B) {
                var_v0 = (s32) D_8018908C < 0x58;
                if ((s32) D_8018908C < 0x70) {
                    goto block_6;
                }
                goto block_8;
            }
            goto block_7;
        }   
        var_v0 = (s32) D_8018908C < 0xBA;
        if ((s32) D_8018908C < 0xCA) {
block_6:
            if (var_v0 == 0) {
block_7:
                var_a3 = 4;
            } else {
                goto block_9;
            }
        } else {
block_8:
block_9:
            var_a3 = 0;
        }
        D_8018908D = initializeHeldItem(0, 2, D_8018908C, var_a3, 8);
*/

//INCLUDE_ASM("asm/nonmatchings/game/player", addItemToRucksack);

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

//INCLUDE_ASM("asm/nonmatchings/game/player", getOffsetTileCoordinates );

Vec3f getOffsetTileCoordinates (f32 range, u8 directionalOffset) {
    
    Vec3f tileCoordinates;

    s8 buffer[10];
    s8 buffer2[10];

    memcpy(buffer, directionToTileDeltaX, 9);
    memcpy(buffer2, directionToTileDeltaZ, 9);
    
    tileCoordinates = getEntityTileCoordinates(ENTITY_PLAYER);

    if (tileCoordinates.y != 65535.0f) {

        tileCoordinates.x += buffer[convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)] * range;
        tileCoordinates.z += buffer2[convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex)] * range;

        if (directionalOffset != 8) {
            
            tileCoordinates.x += buffer[((entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + directionalOffset) % 8)];
            tileCoordinates.z += buffer2[((entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + directionalOffset) % 8)];
            
        }
    }

    return tileCoordinates;
    
}

// alternate without memcpy
/*
Vec3f* getOffsetTileCoordinates (Vec3f *arg0, f32 arg1, u8 arg2) {
     
    Vec3f vec;
    CoordinateOffsetData struct1;
    CoordinateOffsetData struct2;

    s8 *ptr;
    s8 *ptr2;

    struct1 = *(CoordinateOffsetData*)directionToTileDeltaX;
    struct2 = *(CoordinateOffsetData*)directionToTileDeltaZ;
    
    ptr = (s8*)&struct1;
    ptr2 = (s8*)&struct2;

    getEntityTileCoordinates(&vec, 0);

    if (vec.y != 65535.0f) {directionToTileDeltaX

        vec.x += ptr[(entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex)) % 8] * arg1;
        vec.z += ptr2[(entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex)) % 8] * arg1;

        if (arg2 != 8) {
            
            vec.x += ptr[((entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + arg2) % 8)];
            vec.z += ptr2[((entities[ENTITY_PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + arg2) % 8)];
            
        }
    }

    *arg0 = vec;
    
    return arg0;
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/player", setPlayerAction);

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

//INCLUDE_RODATA("asm/nonmatchings/game/player", directionToTileDeltaX);
 
static const s8 directionToTileDeltaX[12] = { 0, -1, -1, -1, 0, 1, 1, 1, 0, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/player", directionToTileDeltaZ);
 
static const s8 directionToTileDeltaZ[12] = { 1, 1, 0, -1, -1, -1, 0, 1, 0, 0, 0, 0 };

//INCLUDE_ASM("asm/nonmatchings/game/player", updatePlayerAction);

void updatePlayerAction(void) {

    if (gPlayer.actionHandler == 0) {
        // handle button input
        handlePlayerInput();
    }
    
    // TODO: label actions
    switch (gPlayer.actionHandler - 1) {
        case 0:
            handleToolCharging();
            break;
        case TOOL_USE:
            handleToolUseAction();
            break;
        case THROWING:
            handleThrowingAction();
            break;
        case 3:
            func_80067F50();
            break;
        case PICKING_UP:
            func_8006807C();
            break;
        case PUTTING_DOWN:
            func_80068120();
            break;
        case 7:
            func_80068258();
            break;
        case 8:
            func_800682F8();
            break;
        // turn on TV
        case 9:
            func_80068340();
            break;
        // shop item interaction
        case 10:
            func_80068410();
            break;
        case 11:
            func_80068558();
            break;
        case ABOUT_TO_DRINK:
            func_80068A98();
            break;
        case 13:
            func_80068C8C();
            break;
        case 14:
            func_80068DFC();
            break;
        case 15:
            func_80068FB0();
            break;
        case 16:
            func_800692E4();
            break;
        case 28:
            func_80069830();
            break;
        case 17:
            func_80069C5C();
            break;
        case WHISTLE_FOR_DOG:
            handleWhistleForDogAction();
            break;
        case WHISTLE_FOR_HORSE:
            handlehandleWhistleForHorseAction();
            break;
        case DRINKING:
            func_80069DA8();
            break;
        case 21:
            handlePuttingItemInRucksackAction();
            break;
        case CASTING_FISHING_ROD:
            handleCastingFishingRodActionStub();
            break;
        case 23:
            func_80069E5C();
            break;
        case 6:
            func_80069E64();
            break;
        case 24:
            func_80069E6C();
            break;
        case 25:
            func_80069E74();
            break;
        case 26:
            func_8006A2E0();
            break;
        case 27:
            func_80068738();
            break;
        case 29:
            func_80068918();
            break;
        case 30:
            func_80068920();
            break;
        case 31:
            func_80068990();
            break;
        case 32:
            func_80068A00();
            break;
        case 33:
            func_80068A08();
            break;
    }
    
    updateHeldItemState();
    processToolUseState();

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePlayerInput);

void handlePlayerInput(void) {

    Vec3f vec;
    u32 padding[3];
    Vec3f vec3;
    
    u8 temp;
    f32 tempAnalogStickMagnitude;
    f32 temp2;
    
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
    
    // FIXME: fake
    do {} while (0);

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

    if (!set && npcResult == 2) {
        temp = 0xFF;
        set = TRUE;
        reset();
        gPlayer.actionHandler = PICKING_UP;
        gPlayer.animationHandler = PICKING_UP + 2;
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
            reset();
            gPlayer.actionHandler = 4;
            gPlayer.animationHandler = 6;
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
                        
                        setDailyEventBit(6);
                        set = TRUE;
                        temp = 0xFF;
                        startAction(0xC, 0xE);
                        
                    }
                    
                } 
            }
        }
        
    }

    if (!set) {
        if (checkButtonPressed(CONTROLLER_1, BUTTON_C_DOWN)) {
            if (gPlayer.heldItem && getItemFlags(gPlayer.heldItem) & ITEM_EATABLE) {
                set = TRUE;
                startAction(EATING, EATING + 2);
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
                            func_80067290(horseResult);
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

        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && !checkDailyEventBit(0x12)) {
        
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
                
                if (gPlayer.heldItem == 0 && gPlayer.currentTool) {
                        
                    gPlayer.actionPhaseFrameCounter = 0;
                    gPlayer.actionPhase = 0;
                    gPlayer.toolHeldCounter = 0;
                    gPlayer.currentToolLevel = 0;

                    if (!func_80067A24(1)) {

                        if (!func_80067B38()) {

                            startAction(1, 3);
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
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && !checkDailyEventBit(0x12)) {
            if (checkButtonPressed(CONTROLLER_1, BUTTON_Z)) {
                if (gPlayer.heldItem != 0) {
                    set = TRUE;
                    showHeldItemText(gPlayer.heldItem);
                    temp = 0xFF;
                }
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            // whistle for dog
            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_RIGHT)) {
                set = TRUE;
                temp = 0xFF;
                startAction(WHISTLE_FOR_DOG, WHISTLE_FOR_DOG + 1);
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {
            // whistle for horse
            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_LEFT)) {
                set = TRUE;
                temp = 0xFF;
                startAction(WHISTLE_FOR_HORSE, WHISTLE_FOR_HORSE + 1);
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
                        startAction(PUTTING_ITEM_IN_RUCKSACK, PUTTING_ITEM_IN_RUCKSACK + 1);
                    }
                }
            }
        }
    }

    if (!set) {
        if (!(gPlayer.flags & PLAYER_RIDING_HORSE) && !checkDailyEventBit(0x13)) {
            if (checkButtonPressed(CONTROLLER_1, BUTTON_START)) {
                set = TRUE;
                // pause everything
                pauseGameplay();
                loadOverlayScreen(1, 20);
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
            gPlayer.animationHandler = 0;
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
                gPlayer.actionHandler = 0;
                gPlayer.animationHandler = 2;
            // walking
            } else {
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.actionPhase = 0;
                gPlayer.actionHandler = 0;
                gPlayer.animationHandler = 1;
            }

            direction = convertSpriteToWorldDirection(temp, MAIN_MAP_INDEX);
            setEntityDirection(ENTITY_PLAYER, direction);

            // FIXME: likely fake
            temp2 = 0;
            
        } else {
            
            tempAnalogStickMagnitude = 0.0f;
            resetAction();
            
        }
        
        tempAnalogStickMagnitude = (s8)tempAnalogStickMagnitude;

        vec = getMovementVectorFromDirection(tempAnalogStickMagnitude, temp, temp2);
        
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
        horseInfo.flags &= ~0x8;     

        gPlayer.flags &= -2;
        
        toggleDailyEventBit(0x5C);
        
        initializeHorseEntity();
        
        entities[ENTITY_PLAYER].direction = direction;

        startAction(0xE, 0x10);

        gPlayer.actionPhase = 0;
        gPlayer.actionPhaseFrameCounter = 0;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067290);

// item <-> level interaction 
void func_80067290(u8 arg0) {

    bool set = FALSE;
    Vec3f vec;
    u8 groundObjectIndex;
    
    if (arg0 == 0) {

        if ((getItemFlags(gPlayer.heldItem) & ITEM_INCUBATOR_PLACEABLE) && (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 17) && gBaseMapIndex == COOP && getIncubatingEggCount() == 0) {
            set = TRUE;
            startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
        }

        if ((getItemFlags(gPlayer.heldItem) & ITEM_LUMBER_STORABLE) && !set && (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 17) && gBaseMapIndex == FARM) {
            set = TRUE;
            startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
        }

        if (checkFarmDogBowlInteraction(gBaseMapIndex) != 0xFF) {
            
            if ((getItemFlags(gPlayer.heldItem) & ITEM_EATABLE) && !set) {    
                startAction(0x1F, 0x20);
            }
            
            set = TRUE;

        }

        if ((getItemFlags(gPlayer.heldItem) & ITEM_RUCKSACK_STORABLE) && !set && checkWaterSourceInteraction(gBaseMapIndex) != 0xFF) {
            set = TRUE;
            startAction(0x20, 0x21);
        }
        
    }

    if ((getItemFlags(gPlayer.heldItem) & ITEM_SHIPPABLE) && !set) {

        if (checkShippingBinInteraction(gBaseMapIndex) != 0xFF || arg0) {
            startAction(3, 5);
        } else {
            startAction(THROWING, THROWING + 2);
        }

        set = TRUE;
        
    }    

    if (arg0 == 0) {

        if ((getItemFlags(gPlayer.heldItem) & ITEM_IS_ANIMAL_FEED) && !set) {

            if (checkBarnAnimalFeedInteraction(gBaseMapIndex) != 0xFF) {
                startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
            } else {
                startAction(THROWING, THROWING + 2);
            }

            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x80) && !set) {

            if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex))) {

                vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

                groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);
    
                if (groundObjectIndex == 0xFF || getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8) {
                    startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
                }
            
            }

            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x100) && !set) {
            startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
            set = TRUE;
        }
        
        if ((getItemFlags(gPlayer.heldItem) & ITEM_CRIB_PLACEABLE) && !set) {
            
            if (gBaseMapIndex == HOUSE) {

                // baby
                if (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f) == 0x16) {
                    startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
                }
                
            } 
            
            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x800) && !set) {

            if (!checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, gMainMapIndex))) {

                vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
    
                groundObjectIndex = getGroundObjectIndexFromCoordinates(vec.x, vec.z);
    
                if (groundObjectIndex == 0xFF || getGroundObjectPlayerInteractionsFlags(groundObjectIndex) & 8) {
                    startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
                }
            
            }

            set = TRUE;
            
        }

        if ((getItemFlags(gPlayer.heldItem) & 0x10) && !set) {

            groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8);

            if (groundObjectIndex && groundObjectIndex < 4 && groundObjectIndex != 0xFF) {
                startAction(PUTTING_DOWN, PUTTING_DOWN + 2);
            }  
            
            set = TRUE;
            
        }
    
        if (!set) {    
            startAction(2, 4);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkStaminaExhaustionLevel);

inline u8 checkStaminaExhaustionLevel(void) {

    s32 temp;
    u8 temp2;
    u8 result;

    result = 0;
    temp = gMaximumStamina;
    
    temp2 = temp / 4;

    if (D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel] > gPlayer.currentStamina) {
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067A24);

bool func_80067A24(u8 arg0) {

    bool result = FALSE;
    u8 staminaExhaustionLevel;

    staminaExhaustionLevel = checkStaminaExhaustionLevel();

    if (staminaExhaustionLevel && staminaExhaustionLevel != gPlayer.staminaExhaustionLevel && arg0 == 0 || staminaExhaustionLevel == 4)  {
        result = TRUE;
        startAction(7, 9);
        gPlayer.staminaExhaustionLevel = staminaExhaustionLevel;
    }

    return result; 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067B38);

bool func_80067B38(void) {

    bool set = FALSE;
    
    u32 fatigueThreshold = checkFatigueLevel();

    if (fatigueThreshold != 0 && fatigueThreshold != gPlayer.fatigueThreshold) {
        set = TRUE;
        startAction(0x19, 0x1A);
        gPlayer.fatigueThreshold = fatigueThreshold;
    }

    return set;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToolCharging);

void handleToolCharging(void) {

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
                gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, -D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel], gMaximumStamina);

                if (((SUNNY < gWeather && gWeather < 4) || !(5 < gHour && gHour < 18)) && gBaseMapIndex != GREENHOUSE) {

                    // kappa power nut
                    if (checkLifeEventBit(0x5F)) {
                        gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel] / 2, MAX_FATIGUE_POINTS);
                    } else {
                        
                        // fake match if D_8011421C isn't volatile
                        // if (D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel] + 1) {
                        //     temp = D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel];
                        // }
                        
                        // temp = D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel];
                        
                        gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel], MAX_FATIGUE_POINTS);
                        
                    }

                }
                
            } else if (gPlayer.toolHeldCounter >= 16 && gPlayer.currentToolLevel < getToolLevel(gPlayer.currentTool)) {

                if (D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel] && !(D_8011421C[gPlayer.currentTool][gPlayer.currentToolLevel+1] > gPlayer.currentStamina)) {
                    gPlayer.toolHeldCounter = 0;
                    gPlayer.currentToolLevel++;
                }

                
            } else {
                gPlayer.toolHeldCounter++;
            }

        } else {
            
            gPlayer.actionHandler = 0xFE;
            gPlayer.actionPhase++;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleToolUseAction);

void handleToolUseAction(void) {

    // FIXME: possibly a union. This matches:
    // if (!(gPlayer.actionUnion.actionPhaseFrameCounter & ~0xFF)) {
    if (!(*(s32*)&gPlayer.actionPhaseFrameCounter & ~0xFF)) {
        playSfx(0x26);
        clearHeldItemSlot(gPlayer.itemInfoIndex);
        func_800D5390(1, 3, gPlayer.heldItem, 0, 8);
        gPlayer.heldItem = 0;
        gPlayer.actionPhaseFrameCounter = 1;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleThrowingAction);

void handleThrowingAction(void) {

    if (gPlayer.actionPhaseFrameCounter == 3) {
        setItemState(gPlayer.itemInfoIndex, 0xE);
        gPlayer.heldItem = 0;
        gPlayer.actionPhase++;
    }

    gPlayer.actionPhaseFrameCounter++;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067F50);

void func_80067F50(void) {

    // FIXME
    if (!(*(s32*)&gPlayer.actionPhaseFrameCounter & ~0xFF)) {
        
        playSfx(PICKING_UP_SFX);
        startAction(PICKING_UP, 6);

        switch (gPlayer.heldItem) {

            case DOG_HELD_ITEM ... 0x6F:
            case BABY_HELD_ITEM ... 0xC9:
            case PUPPY_1_HELD_ITEM ... 0xB2:
                gPlayer.itemInfoIndex = initializeHeldItem(0, 6, gPlayer.heldItem, ITEM_CONTEXT_HAS_DIRECTION_FRAME, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
            default:
                gPlayer.itemInfoIndex = initializeHeldItem(0, 6, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                break;
        
            }
            
    }
    
    if (gPlayer.actionPhaseFrameCounter == 4) {
    
        setItemState(gPlayer.itemInfoIndex, 7);
        gPlayer.actionPhase++;
    
    }
    
    gPlayer.actionPhaseFrameCounter++;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006807C);

void func_8006807C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER)) {
     
        if (gPlayer.actionPhase == 0) {
            setItemState(gPlayer.itemInfoIndex, 9);
            gPlayer.heldItem = 0;
            playSfx(PICKING_UP_SFX);
        }

        if (gPlayer.actionPhase != 1 || (gPlayer.actionPhaseFrameCounter++, gPlayer.actionPhaseFrameCounter == 2)) {
            gPlayer.actionPhase++;
        }

    }   

}


//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068120);

// putting down action
void func_80068120(void) {

    if (gPlayer.actionPhase == 2) {

        if (gPlayer.actionPhaseFrameCounter == 10) {
            setItemState(gPlayer.itemInfoIndex, 1);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068258);

void func_80068258(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_800682F8);

void func_800682F8(void) {
    if (checkButtonHeld(CONTROLLER_1, 0xFFFFFFFF) && gPlayer.actionPhase < 4) {
        gPlayer.actionPhase = 4;
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068340);

// turn on tv
void func_80068340(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068410);

void func_80068410(void) {

    if (gPlayer.actionPhase == 0) {
        if (gPlayer.actionPhaseFrameCounter == 4) {
            if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                setItemState(gPlayer.itemInfoIndex, 7);
            }
            gPlayer.actionPhase++;
        }
        gPlayer.actionPhaseFrameCounter++;
    }
    
    if (gPlayer.actionPhase == 2) {
        if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
            setItemState(gPlayer.itemInfoIndex, 1);
        }
        loadShopItemSprite(gPlayer.shopItemIndex);
        gPlayer.actionPhase = 5;
    }
    
    if (gPlayer.actionPhase == 3) {
        if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
            setItemState(gPlayer.itemInfoIndex, 1);
        }
        deactivateShopItemMapObject(gPlayer.shopItemIndex);
        gPlayer.actionPhase = 5;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068558);

void func_80068558(void) {

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
            
            setEntityCollidable(ENTITY_PLAYER, 0);
            setEntityTracksCollisions(ENTITY_PLAYER, 0);
            enableEntityMovement(ENTITY_PLAYER, 0);
            setEntityYMovement(ENTITY_PLAYER, 0);
            
            vec2 = getMovementVectorFromDirection(4.0f, gPlayer.direction, 0.0f);
            
            gPlayer.movementVector = vec2;
            
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            
        }

        if (gPlayer.actionPhaseFrameCounter < 15) {

            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 4.0f);
            
            // parabolic equation: y = initial_y - gravity * (time - peak_time)²
            entities->coordinates.y = 
                ((gPlayer.movementVector.y 
                     + (((gPlayer.actionPhaseFrameCounter - 7) * (gPlayer.actionPhaseFrameCounter - 7)) * -0.4f)) 
                     + 19.6);
            
        } else {
            
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhase++;
            gPlayer.actionPhaseFrameCounter = 0;
            toggleDailyEventBit(6);
            
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068738);

void func_80068738(void) {

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
            
            setEntityCollidable(ENTITY_PLAYER, 0);
            setEntityTracksCollisions(ENTITY_PLAYER, 0);
            enableEntityMovement(ENTITY_PLAYER, 0);
            setEntityYMovement(ENTITY_PLAYER, 0);
            
            vec2 = getMovementVectorFromDirection(4.0f, gPlayer.direction, 0.0f);
            
            gPlayer.movementVector = vec2;
            
            gPlayer.movementVector.y = entities[ENTITY_PLAYER].coordinates.y;
            
        }

        if (gPlayer.actionPhaseFrameCounter < 27) {

            setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 4.0f);
            
            // parabolic equation: y = initial_y - gravity * (time - peak_time)²
            entities->coordinates.y = 
                ((gPlayer.movementVector.y 
                     + (((gPlayer.actionPhaseFrameCounter - 7) * (gPlayer.actionPhaseFrameCounter - 7)) * -0.4f)) 
                     + 19.6);
            
        } else {
            
            setEntityMovementVector(ENTITY_PLAYER, 0.0f, 0.0f, 0.0f, 0.0f);
            gPlayer.actionPhase++;
            gPlayer.actionPhaseFrameCounter = 0;
            toggleDailyEventBit(6);
            
        }

        gPlayer.actionPhaseFrameCounter++;
        
    }
    
}

// empty function
void func_80068918(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068920);
  
void func_80068920(void) {
 
    if (gPlayer.actionPhaseFrameCounter == 3) {
        setItemState(gPlayer.itemInfoIndex, 0x12);
        gPlayer.heldItem = 0;
        gPlayer.actionPhase++;
    }

    gPlayer.actionPhaseFrameCounter++;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068990);

void func_80068990(void) {

    if (gPlayer.actionPhaseFrameCounter == 3) {
        setItemState(gPlayer.itemInfoIndex, 0x14);
        gPlayer.heldItem = 0;
        gPlayer.actionPhase++;
    }

    gPlayer.actionPhaseFrameCounter++;

}

// empty function
void func_80068A00(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068A08);

void func_80068A08(void) {

    if (gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhaseFrameCounter == 0) {
            gPlayer.itemInfoIndex = initializeHeldItem(0, 2, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
        }
        
        if (gPlayer.actionPhaseFrameCounter >= 30) {
            gPlayer.actionPhase = 1;
        } else {
            gPlayer.actionPhaseFrameCounter++;
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068A98);

void func_80068A98(void) {

    Vec3f vec;

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {

            playSfx(4);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068C8C);

void func_80068C8C(void) {

    Vec3f vec;

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {

            playSfx(4);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068DFC);

void func_80068DFC(void) {

    Vec3f vec;

    switch (gPlayer.actionPhase) {
        
        case 0:

            gPlayer.animationHandler = 0;
            gPlayer.actionPhase++;
            
            break;
        
        case 1:

            gPlayer.animationHandler = 1;
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

    setEntityMovementVector(0, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 1.0f);
   
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068FB0);

void func_80068FB0(void) {

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
            gPlayer.animationHandler = 2;
            
            break;


        case 1:

            if (gPlayer.actionTimer == 0) {

                gPlayer.movementVector.x = 0.0f;
                gPlayer.movementVector.y = 0.0f;
                gPlayer.movementVector.z = 0.0f;

                activateMapAddition(MAIN_MAP_INDEX, 2, 0);
                             
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.animationHandler = 0;
                gPlayer.actionPhase++;
                
            } else {
                gPlayer.actionTimer--;
            }
            
            break;

        
        case 2:

            if (gPlayer.actionPhaseFrameCounter == 90) {
                
                activateMapAddition(MAIN_MAP_INDEX, 0, 0);

                playSfx(0x32);
                
                gPlayer.direction = SOUTHWEST;
                gPlayer.actionTimer = 16;
                gPlayer.actionPhase++;
                
                vec2 = getMovementVectorFromDirection(4.0f, 0, 0.0f);
                gPlayer.movementVector = vec2;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                gPlayer.animationHandler = 2;
                
            } else {
                
                gPlayer.actionPhaseFrameCounter++;
                
                if (gPlayer.actionPhaseFrameCounter == 10) {
                    playSfx(0x56);
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
    
                toggleDailyEventBit(6);
    
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_800692E4);

void func_800692E4(void) {

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
            gPlayer.animationHandler = 2;
            
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
                startAction(0xC, 0xE);
                
                gPlayer.flags |= PLAYER_BATH_POSE_1;
            
            } else {
                gPlayer.actionTimer--;
            }

            break;

        case 3:

            if (gPlayer.actionPhaseFrameCounter == 60) {
                
                gPlayer.direction = SOUTHEAST;
                startAction(0xC, 0xE);
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
            gPlayer.animationHandler = 2;
            
            break;

        case 5:

            if (gPlayer.actionTimer == 0) {

                activateMapAddition(MAIN_MAP_INDEX, 1, 0);
                playSfx(0x32);
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
    
                toggleDailyEventBit(6);
    
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069830);

void func_80069830(void) {

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
            gPlayer.animationHandler = 2;
            
            break;

        case 1:

            if (gPlayer.actionTimer == 0) {
                activateMapAddition(MAIN_MAP_INDEX, 0, 0);
                gPlayer.direction = EAST;
                resetMovement();
                startAction(0xC, 0xE);
                gPlayer.flags |= PLAYER_HOT_SPRING_POSE_1;
            } else {
                gPlayer.actionTimer--;
            }

            break;

        case 2:

            if (gPlayer.actionPhaseFrameCounter == 60) {
                gPlayer.direction = WEST;
                startAction(0xC, 0xE);
                gPlayer.flags |= PLAYER_HOT_SPRING_POSE_3;
            } else {
                gPlayer.actionPhaseFrameCounter++;
            }

            break;

        case 3:

            if (gPlayer.actionTimer == 0) {
                
                activateMapAddition(MAIN_MAP_INDEX, 1, 0);
                
                playSfx(0x32);
                
                gPlayer.direction = SOUTHWEST;
                gPlayer.actionTimer = 0x12;
                
                vec2 = getMovementVectorFromDirection(4.0f, 0, 0.0f);
                
                gPlayer.movementVector = vec2;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                
                gPlayer.animationHandler = 2;
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

                toggleDailyEventBit(6);

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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069C5C);

void func_80069C5C(void) {
    
    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        playSfx(0xA);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleWhistleForDogAction);

void handleWhistleForDogAction(void) {

    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        playSfx(WHISTLE);
    }

}
 
//INCLUDE_ASM("asm/nonmatchings/game/player", handlehandleWhistleForHorseAction);

void handlehandleWhistleForHorseAction(void) {

    if (gPlayer.actionPhase == 0) {

        if (gPlayer.actionPhaseFrameCounter == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, getItemStaminaIncreaseValue(gPlayer.heldItem), gMaximumStamina);
            gPlayer.fatigueCounter += adjustValue(gPlayer.fatigueCounter, -getItemFatigueReductionValue(gPlayer.heldItem), MAX_FATIGUE_POINTS);
        }
        
        if (gPlayer.actionPhaseFrameCounter == 30) {
            setItemState(gPlayer.itemInfoIndex, 1);
            gPlayer.actionPhase++;
        }
        
        gPlayer.actionPhaseFrameCounter++;
        
    }
}

// empty function
void func_80069DA8(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", handlePuttingItemInRucksackAction);

void handlePuttingItemInRucksackAction(void) {

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
void handleCastingFishingRodActionStub(void) {}

// empty function
void func_80069E5C(void) {}

// empty function
void func_80069E64(void) {} 

// empty function
void func_80069E6C(void) {}

static inline void fakeInline() {
    gPlayer.movementVector.y += 1.0f;
    entities[ENTITY_PLAYER].coordinates.y = gPlayer.movementVector.y;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069E74);

// FIXME: have to trick the CSE pass to access actionPhaseFrameCounter via -2 of animationState instead of reloading it from memory every time
// The CSE pass doesn't do this for smaller functions with the same code (func_80068C8C), so there's something around the basic block size/complexity 
void func_80069E74(void) {

    Vec3f vec;

    u16 *temp;

    if (gPlayer.actionPhase == 0) {

        // FIXME: fake
        temp = &gPlayer.actionPhaseFrameCounter;
        
        if (gPlayer.actionPhaseFrameCounter == 0) {
            
            playSfx(4);

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
        
        // FIXME: fake
        // tricks CSE2 pass
        do {} while (0);
        temp = &gPlayer.actionPhaseFrameCounter;

        if (gPlayer.actionPhaseFrameCounter < 0x80) {
            
            // need this to force memory lookup and NOT have pointer access for the other gPlayer members
            fakeInline();
            
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
        
        // FIXME: fake
        temp = &gPlayer.actionPhaseFrameCounter;
        
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
        toggleDailyEventBit(6);
        
    }
    
}

// empty function
void func_8006A2E0(void) {} 
 
//INCLUDE_ASM("asm/nonmatchings/game/player", handlePlayerAnimation);

void handlePlayerAnimation(void) {

    switch (gPlayer.animationHandler) {
        
        case 0:
            
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                
                if ((gPlayer.flags & PLAYER_RIDING_HORSE)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x204);    
                } else {
                    if (gPlayer.flags & 4) {
                         setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x244);
                    } else {
                        if (gPlayer.flags & 0x20) {
                            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x244);
                        } else {
                            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
                        }
                    }   
                }
                
            } else {
                // running
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 24);
            }
            
            if (!(gPlayer.flags & PLAYER_RIDING_HORSE)) {

                if (playerIdleCounter == 1200) {
                    startAction(9, 0xB);
                } else {
                    playerIdleCounter++;
                }

            }

            break;
        
        default:
            break;
        
        case 1:
            
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                   setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 524);
                } else {
                    // walking
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 8);
                }
            } else {
                // standing + carrying
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 32);
            }
            
            playerIdleCounter = 0;
            break;
        
        case 2:
            
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
                    // running
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 16);
                }

            } else {
                if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                    playSfx(WALKING_SFX);
                }
                // running + carrying
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 40);

            }

            playerIdleCounter = 0;
            break;
        
        case 3:
            func_8006C384();
            playerIdleCounter = 0;
            break;
        case 4:
            func_8006A714();
            playerIdleCounter = 0;
            break;
        case 5:
            func_8006A7B0();
            playerIdleCounter = 0;
            break;
        case 6:
            func_8006A848();
            playerIdleCounter = 0;
            break;
        case 7:
            func_8006A8C4();
            playerIdleCounter = 0;
            break;
        case 8:
            func_8006A9A8();
            playerIdleCounter = 0;
            break;
        case 9:
            func_8006AA9C();
            playerIdleCounter = 0;
            break;
        case 10:
            func_8006AC4C();
            playerIdleCounter = 0;
            break;
        // idle/napping animation
        case 11:
            func_8006ADF4();
            break;
        case 12:
            func_8006AEEC();
            playerIdleCounter = 0;
            break;
        case 13:
            func_8006AFE4();
            playerIdleCounter = 0;
            break;
        case 14:
            func_8006B104();
            playerIdleCounter = 0;
            break;
        case 15:
            func_8006B8BC();
            playerIdleCounter = 0;
            break;
        case 16:
            func_8006B910();
            playerIdleCounter = 0;
            break;
        case 17:
            func_8006B964();
            playerIdleCounter = 0;
            break;
        case 18:
            func_8006B96C();
            playerIdleCounter = 0;
            break;
        case 30:
            func_8006B974();
            playerIdleCounter = 0;
            break;
        case 19:
            func_8006B97C();
            playerIdleCounter = 0;
            break;
        case 20:
            func_8006BA14();
            playerIdleCounter = 0;
            break;
        case 21:
            func_8006BAAC();
            playerIdleCounter = 0;
            break;
        case 22:
            func_8006BBC4();
            playerIdleCounter = 0;
            break;
        case 23:
            handleFishingRodUse();
            playerIdleCounter = 0;
            break;
        case 24:
            func_8006C12C();
            playerIdleCounter = 0;
            break;
        case 25:
            func_8006C134();
            playerIdleCounter = 0;
            break;
        case 26:
            func_8006AB90();
            playerIdleCounter = 0;
            break;
        case 27:
            func_8006C13C();
            playerIdleCounter = 0;
            break;
        case 28:
            func_8006C1DC();
            playerIdleCounter = 0;
            break;
        case 29:
            func_8006B4D4();
            playerIdleCounter = 0;
            break;
        case 31:
            func_8006B4DC();
            playerIdleCounter = 0;
            break;
        case 32:
            func_8006B584();
            playerIdleCounter = 0;
            break;
        case 33:
            func_8006B61C();
            playerIdleCounter = 0;
            break;
        case 34:
            func_8006B6EC();
            playerIdleCounter = 0;
            break;
        case 35:
            func_8006B77C();
            playerIdleCounter = 0;
            break;

    }
    
}

// probably not necessary
static inline handleStopHolding() {
    resetAction();
    gItemBeingHeld = 0xFF;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A714);

void func_8006A714(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhase == 0) {
        
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1A5);
            gPlayer.actionPhase++;
        
        } else {

            handleStopHolding();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A7B0);

static inline updateAnimation(u16 animation) {
    
     if (gPlayer.actionPhase == 0) {
        setEntityAnimationWithDirectionChange(ENTITY_PLAYER, animation);
        setItemState(gPlayer.itemInfoIndex, 0xD);
    } else {
        handleStopHolding();
    }
}

void func_8006A7B0(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
       updateAnimation(0x1A5);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A848);

void func_8006A848(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
    
        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
        } else {
            resetAction();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A8C4);

void func_8006A8C4(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {     

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                setItemState(gPlayer.itemInfoIndex, 8);
                break;
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1BF);
                break;
            case 2:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                break;
            default:
                handleStopHolding();
                break;

            }

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A9A8);

void func_8006A9A8(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {       

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 24);
                break;
            case 1:
                setItemState(gPlayer.itemInfoIndex, 10);
                gPlayer.actionPhaseFrameCounter = 0;
                gPlayer.actionPhase++;
                break;
            case 2:
                setEntityAnimation(ENTITY_PLAYER, 0x1B5);
                break;
            default:
                handleStopHolding();
                break;

            }

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AA9C);

void func_8006AA9C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhase == 0) {
            
            switch (gPlayer.staminaExhaustionLevel) {       
                case 1:
                    setEntityAnimation(ENTITY_PLAYER, 0x1D0);
                    break;
                case 2:
                    setEntityAnimation(ENTITY_PLAYER, 0x1D1);
                    break;
                case 3:
                    setEntityAnimation(ENTITY_PLAYER, 0x1D2);
                    break;
                case 4:
                    setEntityAnimation(ENTITY_PLAYER, 0x1D3);
                    break;
            }
    
            gPlayer.actionPhase++;

        } else {

            resetAction();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AB90);

void func_8006AB90(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        if (gPlayer.actionPhase == 0) {

            switch (gPlayer.fatigueThreshold) {                   
                case 1:
                    setEntityAnimation(ENTITY_PLAYER, 0x299);
                    break;
                case 2:
                    setEntityAnimation(ENTITY_PLAYER, 0x29A);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AC4C);

void func_8006AC4C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {         

            case 1:

                setEntityAnimation(ENTITY_PLAYER, 0x1D4);
                gPlayer.actionPhase++;

                if (checkLifeEventBit(MARRIED) && (19 < gHour && gHour < 22)) {

                    switch (gWife) {
                        case MARIA:
                            npcs[MARIA].movingFlag = 4;
                            break;
                        case POPURI:
                            npcs[POPURI].movingFlag = 4;
                            break;
                        case ELLI:
                            npcs[ELLI].movingFlag = 4;
                            break;
                        case ANN:
                            npcs[ANN].movingFlag = 4;
                            break;
                        case KAREN:
                            npcs[KAREN].movingFlag = 4;
                            break;
                    }
                }

                break;

            case 2:
                setEntityAnimation(ENTITY_PLAYER, 0x1D5);
                gPlayer.actionPhase++;
                break;

            case 3:
                setEntityAnimation(ENTITY_PLAYER, 0x1D6);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006ADF4);

// napping/idle animation
void func_8006ADF4(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) { 

            case 0:
                setEntityAnimation(ENTITY_PLAYER, 0x1E0);
                gPlayer.actionPhase++;
                break;
            case 1:
                setEntityAnimation(ENTITY_PLAYER, 0x1E1);
                gPlayer.actionPhase++;
                break;
            case 2:
                playerIdleCounter++;
                if (playerIdleCounter == 1800) {
                    setEntityAnimation(ENTITY_PLAYER, 0x1E2);
                    gPlayer.actionPhase++;
                }
                break;
            case 4:
                setEntityAnimation(ENTITY_PLAYER, 0x1E3);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AEEC);

void func_8006AEEC(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) { 
            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                break;
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1BF);
                gPlayer.actionPhase++;
                break;
            case 3:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1BF);
                break;            
            case 4:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                gPlayer.actionPhase++;
                break;           
            default:
                resetAction();
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AFE4);

void func_8006AFE4(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {  
            case 0:
                if (!checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
                }
                break;
            case 1:
                // shop.c
                intializeShopDialogue(gPlayer.shopItemIndex);
                gPlayer.actionPhaseFrameCounter = 0;
                pauseGameplay();
                break;
            case 2:
                if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
                }
                break;
            case 3:
                if (checkCanPickUpShopItem(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
                }
                break;
            default:
                resetAction();
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B104);

void func_8006B104(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.actionPhase == 0)) {
        
        switch (gPlayer.actionPhase) {                          
        
            case 0:
                
                if (gPlayer.heldItem != 0) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x40);
                } else if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x224);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 0x23C);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 0x241);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 0x23D);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 0x240);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x30);
                }
                
                break;
                
            case 1:
                
                if (gPlayer.heldItem != 0) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x48);
                } else if (gPlayer.flags & PLAYER_RIDING_HORSE) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x22C);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 0x23E);
                } else if (gPlayer.flags & PLAYER_BATH_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 0x243);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_1) {
                    setEntityAnimation(ENTITY_PLAYER, 0x23F);
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_3) {
                    setEntityAnimation(ENTITY_PLAYER, 0x242);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x38);
                }

                break;
            
            default:
                
                if (gPlayer.flags & PLAYER_BATH_POSE_1) {
                    
                    startAction(0x11, 0);

                    gPlayer.direction = SOUTHWEST;
                    
                    gPlayer.flags &= ~PLAYER_BATH_POSE_1; 
                    gPlayer.flags |= PLAYER_BATH_POSE_2;
                    
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 3;

                    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                    playSfx(0x28);
                      
                } else if (gPlayer.flags & PLAYER_BATH_POSE_3) {
                    
                    startAction(0x11, 0);
                    
                    gPlayer.flags &= ~(PLAYER_BATH_POSE_2 | PLAYER_BATH_POSE_3);
                                        
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 4;
                    
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_1) {
                    
                    startAction(0x1D, 0);

                    gPlayer.direction = SOUTHWEST;
                    
                    gPlayer.flags &= ~PLAYER_HOT_SPRING_POSE_1;
                    gPlayer.flags |= PLAYER_HOT_SPRING_POSE_2;
                    
                    gPlayer.movementVector.x = 0.0f;
                    gPlayer.movementVector.y = 0.0f;
                    gPlayer.movementVector.z = 0.0f;
                    gPlayer.actionPhase = 2;
                    
                    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                    playSfx(0x28);
                                        
                } else if (gPlayer.flags & PLAYER_HOT_SPRING_POSE_3) {
                
                    startAction(0x1D, 0);
                    
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
void func_8006B4D4(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B4DC);

void func_8006B4DC(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimation(ENTITY_PLAYER, 0x1DB);
            
            gPlayer.actionPhase = 1;
            gPlayer.heldItem = 0xCA;
            gPlayer.itemInfoIndex = initializeHeldItem(0, 0x10, 0xCA, 0, ITEM_CONTEXT_USE_ATTACHMENT);
            
        } else {
            
            setMainLoopCallbackFunctionIndex(PINK_OVERLAY_TEXT);
        
        }
    }    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B584);

void func_8006B584(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
            setItemState(gPlayer.itemInfoIndex, 0x11);
            
        } else {
            
            resetAction();
            gItemBeingHeld = 0xFF;
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B61C);

void func_8006B61C(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1A5);
            setItemState(gPlayer.itemInfoIndex, 0xD);
            
        } else {

            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            gItemBeingHeld = 0xFF;

            if (!checkDailyEventBit(0x45) && !checkDailyEventBit(0x47) && !checkDailyEventBit(0x48)) {
                resetAction();
            }

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B6EC);

void func_8006B6EC(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        if (gPlayer.actionPhase == 0) {
            setEntityAnimation(ENTITY_PLAYER, 0x2B3);
            gPlayer.actionPhase++;
        } else {
            resetAction();
        }
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B77C);

void func_8006B77C(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        switch (gPlayer.actionPhase) {

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18);
                break;
            
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x26A);
                setItemState(gPlayer.itemInfoIndex, 1);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B8BC);

void func_8006B8BC(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
             setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x21C);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B910);

void func_8006B910(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
             setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x25C);
        }
    }
    
}

// empty function
void func_8006B964(void) {}

// empty function
void func_8006B96C(void) {}

// empty function
void func_8006B974(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B97C);

void func_8006B97C(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x19D);
            gPlayer.actionPhase++;
        } else {
            resetAction();
            handleWhistleForDog();
        }
    }

}


//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006BA14);

void func_8006BA14(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        if (gPlayer.actionPhase == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x19D);
            gPlayer.actionPhase++;
        } else {
            resetAction();
            handleWhistleForHorse();
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006BAAC);

// handle drinking
void func_8006BAAC(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {

        switch (gPlayer.actionPhase) {

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18);
                break;
            
            case 1:

                if (getItemFlags(gPlayer.heldItem) & ITEM_HAS_ALCOHOL) {
                    setEntityAnimation(ENTITY_PLAYER, 0x2B3);
                    gAlcoholTolerance += adjustValue(gAlcoholTolerance, 1, 0xFF);
                } else {
                    setEntityAnimation(ENTITY_PLAYER, 0x1B6);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006BBC4);

void func_8006BBC4(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.actionPhase == 0)) {
        
        if (gPlayer.actionPhase == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x26A);
            setItemState(gPlayer.itemInfoIndex, 1);
            gPlayer.heldItem = 0;
            gPlayer.actionPhase++;
            playSfx(PICKING_UP_SFX);
            
        } else {
            resetAction();
            gItemBeingHeld = 0xFF;
        }
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", handleFishingRodUse);

// fishing handler
void handleFishingRodUse(void) {

    Vec3f vec;
    u16 temp;
    s32 temp2;
    int max;

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {
        
        switch (gPlayer.actionPhase) {
            
            case 0:
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x182);
                gPlayer.actionPhase++;
                break;
            
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

                    spawnFishingRodEntity(0, 0x9D, vec.x, vec.y, vec.z);
                    playSfx(FISHING_ROD_CAST);
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x17A);
                    gPlayer.actionPhase = 2;
                
                } else {
                    gPlayer.actionPhase = 8;
                }
                
                break;
            
            case 3:
                setItemEntityAnimation(0, 0x9E);
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18A);
                gPlayer.actionPhase = 4;
                break;
            
            case 4:
                gPlayer.actionPhase = 5;
                break;
            
            case 5:
                gPlayer.actionPhase = 1;
                break;
            
            // catch fish
            case 7:

                // FIXME: likely a switch
                // ?
                temp2 = gPlayer.fishingSpotType;
                max = 4;

                if (gPlayer.fishingSpotType == 0) {
                    temp = 1;
                } else if (0 <= temp2 && gPlayer.fishingSpotType < max) {
                    temp = 2;
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
                if (gPlayer.fishingSpotType == 3 && !(powerNutBits & FISHING_POWER_NUT) && !getRandomNumberInRange(0, 50)) {
                    
                    gPlayer.heldItem = POWER_NUT;

                    startAction(6, 8);
                    
                    powerNutBits |= FISHING_POWER_NUT;
                    
                    gMaximumStamina += adjustValue(gMaximumStamina, 15, MAX_STAMINA);
                    gHappiness += adjustValue(gHappiness, 4, MAX_HAPPINESS);


                }

                totalFishCaught += adjustValue(totalFishCaught, 1, MAX_FISH_CAUGHT);

                initializePlayerHeldItem();
                
                break;
            
            case 6:
            case 8:
                deactivateItemEntity(0);
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x192);
                gPlayer.actionPhase++;
                break;
              
            default:            
                resetAction();
            case 2:
                break;
        }
    }

    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        playSfx(0xB);
    }
    
}

// empty function
void func_8006C12C(void) {}

// empty function
void func_8006C134(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C13C);

// climbing trees
void func_8006C13C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0 || gPlayer.actionPhase == 2) {
        switch (gPlayer.actionPhase) {
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x29B);
                break;
            case 0:
            case 2:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x30);
                break;
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C1DC);

// acquire tool upgrade
void func_8006C1DC(void) {

    u8 buffer[5][3];
    u8 temp;

    memcpy(buffer, toolHeldItemIndices, 15);

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.actionPhase == 0) {

        switch (gPlayer.actionPhase) {

            case 0:
                setEntityAnimation(ENTITY_PLAYER, 0x171);
                gPlayer.actionPhase = 1;
                temp = buffer[upgradedToolIndex][upgradedToolLevelIndex - 3];
                gPlayer.heldItem = temp;
                gPlayer.itemInfoIndex = initializeHeldItem(0, 0x15, temp, 0, ITEM_CONTEXT_USE_ATTACHMENT);
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
                setItemState(gPlayer.itemInfoIndex, 1);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C384);

// handle initial tool use animation and sound effects
void func_8006C384(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.actionPhase == 0)) {
        
        switch (gPlayer.currentTool) {                          
            case SICKLE:                                     
                func_8006CD84();
                break;
            case HOE:                                     
                func_8006CDF8();
                break;
            case AX:                                     
                func_8006CE6C();
                break;
            case HAMMER:                                     
                func_8006CEE0();
                break;
            case WATERING_CAN:                                     
                func_8006CF54();
                break;
            case MILKER:                                     
                func_8006D0AC();
                break;
            case BELL:                                     
                func_8006D21C();
                break;
            case BRUSH:                                     
                func_8006D304();
                break;
            case CLIPPERS:                                     
                func_8006D3FC();
                break;
            case TURNIP_SEEDS:                                    
                func_8006D56C();
                break;
            case POTATO_SEEDS:                                    
                func_8006D690();
                break;
            case CABBAGE_SEEDS:                                    
                func_8006D7B4();
                break;
            case TOMATO_SEEDS:                                    
                func_8006D8D8();
                break;
            case CORN_SEEDS:                                    
                func_8006D9FC();
                break;
            case EGGPLANT_SEEDS:                                    
                func_8006DB20();
                break;
            case STRAWBERRY_SEEDS:                                    
                func_8006DC44();
                break;
            case MOON_DROP_SEEDS:                                    
                func_8006DD68();
                break;
            case PINK_CAT_MINT_SEEDS:                                    
                func_8006DE8C();
                break;
            case BLUE_MIST_SEEDS:                                    
                func_8006DFB0();
                break;
            case CHICKEN_FEED:                                    
                func_8006E0D4();
                break;
            case BALL:                                    
                func_8006E1F8();
                break;
            case FEEDING_BOTTLE:                                    
                func_8006E200();
                break;
            case 23:                                    
                func_8006E208();
                break;
            case FISHING_POLE:                                    
                func_8006E210();
                break;
            case MIRACLE_POTION:                                    
                func_8006E240();
                break;
            case COW_MEDICINE:                                    
                func_8006E348();
                break;
            case GRASS_SEEDS:                                    
                func_8006E450();
                break;
            case BLUE_FEATHER:                                    
                func_8006E574();
                break;
            case EMPTY_BOTTLE:                                    
                func_8006E678();
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
                playSfx(34);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C628);

void func_8006C628(u16 arg0, u16 arg1) {

    u8 found;
    u8 found2;

    switch (gPlayer.actionPhase) {

        case 0:

            toggleDailyEventBit(0x29);
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, arg0);

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
        
        case 1:                                         
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, arg1);
            resetToolUseState();
            break;


        case 2:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                switch (gPlayer.currentTool) {

                    case SICKLE:
                        
                        if (!(gCutsceneFlags & 1)) {
                            
                            if (gPlayer.toolUseCounters[0] != 400) {
                                
                                gPlayer.toolUseCounters[0]++;
                                
                                if (gPlayer.toolUseCounters[0] == 200) {
                                    
                                    gPlayer.toolLevels[0] = 1;
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 1;
                                    upgradedToolLevelIndex = 1;
                                    
                                }
                                
                                if (gPlayer.toolUseCounters[0] == 400) {
                                    
                                    gPlayer.toolLevels[0] = 2;
                                    startAction(0x1B, 0x1C);
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
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 2;
                                    upgradedToolLevelIndex = 1;
                                    
                                }
                                
                                if (gPlayer.toolUseCounters[1] == 500) {
                                    
                                    gPlayer.toolLevels[1] = 2;
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 2;
                                    upgradedToolLevelIndex = 2;
                                    
                                }

                            } 
                            
                            if (toolUse.musicBoxTileDigCounter == 10) {

                                if (checkHaveKeyItem(TREASURE_MAP) && !checkHaveKeyItem(BROKEN_MUSIC_BOX)) {
                                    gPlayer.heldItem = BROKEN_MUSIC_BOX_HELD_ITEM;
                                    startAction(0x22, 0x23);
                                }
                                
                                toolUse.musicBoxTileDigCounter = 0;

                            } else if (checkDailyEventBit(0x29) && !(powerNutBits & 1) && !(getRandomNumberInRange(0, 479))) {
                            
                                gPlayer.heldItem = POWER_NUT;
                                startAction(EATING, EATING + 2);
                                initializePlayerHeldItem();
                                powerNutBits |= 1;
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
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 3;
                                    upgradedToolLevelIndex = 1;
                                
                                }
                                
                                if (gPlayer.toolUseCounters[2] == 900) {
                                    
                                    gPlayer.toolLevels[2] = 2;
                                    startAction(0x1B, 0x1C);
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
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 4;
                                    upgradedToolLevelIndex = 1;
                                
                                }
                                
                                if (gPlayer.toolUseCounters[3] == 200) {
                                    
                                    gPlayer.toolLevels[3] = 2;
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 4;
                                    upgradedToolLevelIndex = 2;
                                    
                                }
                            
                            }
                            
                            if (checkDailyEventBit(0x29) && !(powerNutBits & 0x10)) {
                                
                                gPlayer.heldItem = 0x57;
                                startAction(6, 8);
                                initializePlayerHeldItem();
                                powerNutBits |= 0x10;
                                
                                gMaximumStamina += adjustValue(gMaximumStamina, 15, 250);
                                setLifeEventBit(0x45);
                                
                            }
                        
                        }
                        
                        toggleDailyEventBit(0x29U);
                        break;
                    
                    case WATERING_CAN:

                        if (!(gCutsceneFlags & 1)) {

                            if (gPlayer.toolUseCounters[4] != 1200) {

                                gPlayer.toolUseCounters[4]++;
                                
                                if (gPlayer.toolUseCounters[4] == 600) {
                                
                                    gPlayer.toolLevels[4] = 1;
                                    startAction(0x1B, 0x1C);
                                    upgradedToolIndex = 5;
                                    upgradedToolLevelIndex = 1;
                                
                                }
                                
                                if (gPlayer.toolUseCounters[4] == 1200) {
                                    
                                    gPlayer.toolLevels[4] = 2;
                                    startAction(0x1B, 0x1C);
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CD84);

// sickle
void func_8006CD84(void) {
    
    switch(gPlayer.currentToolLevel) {

        case 0:
            func_8006C628(0x50, 104);
            break;
        case 1:
            func_8006C628(0x58, 112);
            break;
        case 2:
            func_8006C628(0x60, 120);
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CDF8);

void func_8006CDF8(void) {
    
    switch(gPlayer.currentToolLevel) {

        case 0:
            func_8006C628(0x80, 0x98);
            break;
        case 1:
            func_8006C628(0x88, 0xA0);
            break;
        case 2:
            func_8006C628(0x90, 0xA8);
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CE6C);

void func_8006CE6C(void) {

    switch(gPlayer.currentToolLevel) {

        case 0:
            func_8006C628(0xB0, 0xC8);
            break;
        case 1:
            func_8006C628(0xB8, 0xD0);
            break;
        case 2:
            func_8006C628(0xC0, 0xD8);
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CEE0);

void func_8006CEE0(void) {

    switch (gPlayer.currentToolLevel) {
        
        case 0:
            func_8006C628(0xE0, 0xF8);
            break;      
        case 1:
            func_8006C628(0xE8, 0x100);
            break;
        case 2:
            func_8006C628(0xF0, 0x108);
            break;        
        default:
            break;
    }
    
}

// watering can
static inline void unknownInlineFunc() {

    u8 temp;
    
    switch (gPlayer.actionPhase) {
                    
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x140);
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

                if (!(func_80067A24(0))) {
                    resetAction();
                }
                
            }
            
            break;
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CF54);

void func_8006CF54(void) {

    u8 temp;
    
    switch (gPlayer.currentToolLevel) {                              
        
        case 0:

            if (checkWaterableSoilInteraction(gBaseMapIndex)) {

                unknownInlineFunc();
                
            } else {
                func_8006C628(0x110, 0x128);
                break;
            }
            
            break;
        
        case 1:
            func_8006C628(0x118, 0x130);
            break;

        case 2:
            func_8006C628(0x120, 0x138);
            break;
        
        default:
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D0AC);

void func_8006D0AC(void) {

    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x149);
            break;
        
        case 2:
            // reset tool use struct
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if (toolUse.toolUseState == 0) {

                if (gPlayer.heldItem == 0) {

                    if (!(func_80067A24(0))) {
                        resetAction();
                    }
                    
                } else {

                    if (!(func_80067A24(0))) {
                        startAction(4, 6);
                    } else {
                        gPlayer.itemInfoIndex = initializeHeldItem(0, 6, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                    }
                    
                }
                
            }
        
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D21C);

void func_8006D21C(void) {
    
    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x151);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if ((toolUse.toolUseState == 0) && !(func_80067A24(0))) {
                resetAction();
            }
        
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D304);

void func_8006D304(void) {

    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            handleHorsePlayerInteraction();
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x159);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if ((toolUse.toolUseState == 0) && !(func_80067A24(0))) {
                resetAction();
            }
        
            break;
        
        }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D3FC);

void func_8006D3FC(void) {

    switch (gPlayer.actionPhase) {                              
        
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            handleFarmAnimalPlayerCollision();
            break;

        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x161);
            break;
    
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:
            
            if (toolUse.toolUseState == 0) {
                
                if (gPlayer.heldItem == 0) {
                    
                    if (!(func_80067A24(0))) {
                        resetAction();
                    }
                    
                } else {
                    
                    if (!(func_80067A24(0))) {
                        startAction(PICKING_UP, PICKING_UP + 2);
                    } else {
                        gPlayer.itemInfoIndex = initializeHeldItem(0, 6, gPlayer.heldItem, 0, ITEM_CONTEXT_USE_ATTACHMENT);
                    }
                    
                    
                }
                
            } 
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D56C);

void func_8006D56C(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_801FC154 += adjustValue(D_801FC154, -1, 20);

                if (!D_801FC154) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D690);

void func_8006D690(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_80204DF4 += adjustValue(D_80204DF4, -1, 20);

                if (!D_80204DF4) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D7B4);

void func_8006D7B4(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_8018A725 += adjustValue(D_8018A725, -1, 20);

                if (!D_8018A725) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D8D8);

void func_8006D8D8(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_8013DC2C += adjustValue(D_8013DC2C, -1, 20);

                if (!D_8013DC2C) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D9FC);

void func_8006D9FC(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_801FAD91 += adjustValue(D_801FAD91, -1, 20);

                if (!D_801FAD91) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DB20);

void func_8006DB20(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_80237458 += adjustValue(D_80237458, -1, 20);

                if (!D_80237458) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DC44);

void func_8006DC44(void) {
    
    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_802373E8 += adjustValue(D_802373E8, -1, 20);

                if (!D_802373E8) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DD68);

void func_8006DD68(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        
        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_801C3F70 += adjustValue(D_801C3F70, -1, 20);

                if (!D_801C3F70) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DE8C);

void func_8006DE8C(void) {

        switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_80205636 += adjustValue(D_80205636, -1, 20);

                if (!D_80205636) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DFB0);

void func_8006DFB0(void) {

    switch (gPlayer.actionPhase) {
        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        case 3:            
            if (toolUse.toolUseState == 0) {
                if (!func_80067A24(0)) {
                    resetAction();
                }

                D_802373A8 += adjustValue(D_802373A8, -1, 1);

                if (!D_802373A8) {
                    gPlayer.currentTool = 0;
                }
            }
            break;
        default:
            break;
    }
}

// chicken feed
//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E0D4);

void func_8006E0D4(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            break;

        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x169);
            break;

        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {

                if (!func_80067A24(0)) {
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
void func_8006E1F8(void) {}

// empty function
void func_8006E200(void) {}

// empty function
void func_8006E208(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E210);

void func_8006E210(void) {
    startAction(0x16, 0x17);
}

// empty bottle
//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E240);

void func_8006E240(void) {

    switch (gPlayer.actionPhase) {                           
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            handleFarmAnimalPlayerCollision();
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x234);
            break;
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        case 3:
            if (toolUse.toolUseState == 0) {
                if (!func_80067A24(0)) {
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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E348);

void func_8006E348(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0);
            handleFarmAnimalPlayerCollision();
            break;
        
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x234);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                if (checkDailyEventBit(0x14)) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E450);

void func_8006E450(void) {

    switch (gPlayer.actionPhase) {

        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:

            if (toolUse.toolUseState == 0) {
                
                if (!(func_80067A24(0))) {
                    resetAction();
                }

                D_801C3E28 += adjustValue(D_801C3E28, -1, 20);

                if (!D_801C3E28) {
                    gPlayer.currentTool = 0;
                }
                
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E574);

void func_8006E574(void) {

    switch (gPlayer.actionPhase) {
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            checkEntityProximity(ENTITY_PLAYER, 0.0f, 8.0f, 0);
            findNPCToTalkTo();
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x234);
            break;
        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;
        case 3:
            if (toolUse.toolUseState == 0 && !func_80067A24(0)) {
                resetAction();
            }
            break;
        default:
            break;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E678);

void func_8006E678(void) {

    switch (gPlayer.actionPhase) {

        case 0:

            if (gPlayer.bottleContents == 0) {
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
                handleFarmAnimalPlayerCollision();
                
            } else {

                startAction(0x14, 0x15);
                
                switch (gPlayer.bottleContents) {
                    
                    case 1:
                        gPlayer.heldItem = 0x71;
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

        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x272);
            break;

        case 2:
            resetToolUseState();
            gPlayer.actionPhase++;
            break;

        case 3:
            if (toolUse.toolUseState == 0 && !func_80067A24(0)) {
                resetAction();
            }    
            break;

        default:
            break;
    
    }

}