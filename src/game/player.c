#include "common.h"

#include "game/player.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/math.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "game/levelInteractions.h"
#include "mainLoop.h"
#include "mainproc.h"
#include "game/mapObjects.h"
#include "game/npc.h"
#include "game/shop.h"
#include "game/spriteInfo.h"
 
// forward declaration
u8 func_80067A24(u8);                 
void func_8006C628(u16, u16);

// consumable tool counters (seeds, feed)
extern u8 D_802373A8;
// chicken feed counter
extern u16 chickenFeedQuantity;

// FIXME: part of player struct, but necessary for matching right now
extern u16 D_801890C8;

extern u8 gMaximumStamina;
extern u32 powerNutBits;

// possible bss
extern u8 gToolchestSlots[];   
extern u8 D_8018A724;
extern u8 D_801A8B5C;

extern u32 totalFishCaught;

// data
extern Vec3f playerDefaultStartingCoordinates[];
// = { -352.0f, 0.0f, -16.0f }

// data
extern u8 playerDefaultStartingDirections[73];

// rodata
extern const s8 D_8011F3F0[12];
extern const s8 D_8011F3FC[12]; 
extern const u8 D_8011F5D4[];

// forward declarations
void func_80067BC4(void);
void func_80067E5C(void);
void func_80067EE0(void);
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
void func_80069C90(void);
void func_80069CC4(void);
void func_80069DA8(void);
void func_80069DB0(void);
void func_80069E54(void);
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
void func_8006AB90(void);
void func_8006C13C(void);
void func_8006C1DC(void);
void func_8006ADF4(void);
void func_8006B4D4(void);
void func_8006B4DC(void);
void func_8006B584(void);
void func_8006B61C(void);
void func_8006B6EC(void);
void func_8006B77C(void);
void func_8006AC4C(void);

static inline void resetAction() {
    gPlayer.actionProgress = 0;
    gPlayer.animationState = 0;
    gPlayer.currentAction = 0;
    gPlayer.nextAction = 0;
}

// FIXME: shouldn't be necessary
static inline void resetAction2() {
    gPlayer.actionProgress = 0;
    gPlayer.animationState = 0;
}

static inline void setAction(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    gPlayer.currentAction = arg0;
    gPlayer.actionProgress = arg1;
    gPlayer.animationState = arg2;
    gPlayer.nextAction = arg3;
}

static inline void setAction2(u8 arg0, u8 arg1) {
    gPlayer.currentAction = arg0;
    gPlayer.nextAction = arg1;
    gPlayer.actionProgress = 0;
    gPlayer.animationState = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", setupPlayerEntity);

void setupPlayerEntity(u16 arg0, u8 resetPlayer) {
 
    loadEntity(ENTITY_PLAYER, 0, TRUE);

    setEntityCollidable(ENTITY_PLAYER, TRUE);
    setEntityYMovement(ENTITY_PLAYER, TRUE);
    func_8002FF38(ENTITY_PLAYER, TRUE);
    func_8002FCB4(ENTITY_PLAYER, TRUE);

    setEntityColor(ENTITY_PLAYER, globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a);
    setEntityShadow(ENTITY_PLAYER, 0);

    if (resetPlayer) {
        gPlayer.startingCoordinates.x = playerDefaultStartingCoordinates[arg0].x;
        gPlayer.startingCoordinates.y = playerDefaultStartingCoordinates[arg0].y;
        gPlayer.startingCoordinates.z = playerDefaultStartingCoordinates[arg0].z;
        gPlayer.direction = playerDefaultStartingDirections[arg0];
    }

    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
    setEntityCoordinates(ENTITY_PLAYER, gPlayer.startingCoordinates.x, gPlayer.startingCoordinates.y, gPlayer.startingCoordinates.z);

    toolUse.unk_3 = 0;
    toolUse.unk_4 = 0;
    toolUse.unk_6 = 0;
    toolUse.unk_8 = 0;
    toolUse.unk_A = 0;
    toolUse.unk_C = 0;

}
  
//INCLUDE_ASM("asm/nonmatchings/game/player", handleEatingAndDrinking);

// handle eating/drinking
void handleEatingAndDrinking(void) {

    if (gPlayer.heldItem) {
        
        switch (gPlayer.heldItem) {

            case 0x58 ... 0x6F:
            case 0xBA ... 0xC9:
            case 0x7B ... 0xB2:
                gPlayer.itemInfoIndex = func_800D5308(0, 2, gPlayer.heldItem, 4, 8);
                break;
            default:
                gPlayer.itemInfoIndex = func_800D5308(0, 2, gPlayer.heldItem, 0, 8);
                break;
            
        }
        
        func_800D5688(0, 0, 14.0f, 20.0f);
        
    } else {
        gItemBeingHeld = 0xFF;
        func_800D5548(gPlayer.itemInfoIndex);
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
        D_8018908D = func_800D5308(0, 2, D_8018908C, var_a3, 8);
*/

//INCLUDE_ASM("asm/nonmatchings/game/player", addItemToRucksack);

u8 addItemToRucksack(u8 item) {

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

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80065BCC);

// store tool
u8 func_80065BCC(u8 tool) {

    u8 i;
    u8 found = 0xFF;

    for (i = 0; i < MAX_TOOL_SLOTS_RUCKSACK && found == 0xFF; i++) {
        if (gPlayer.toolSlots[i] == tool) {
            found = 1;
        }
    }

    if (found == 0xFF) {
        for (i = 0; i < MAX_TOOLCHEST_SLOTS && found == 0xFF; i++) {
            if (gToolchestSlots[i] == tool) {
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
        for (i = 0; i < MAX_TOOLCHEST_SLOTS && found == 0xFF; i++) {
            if (gToolchestSlots[i] == 0) {
                gToolchestSlots[i] = tool;
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
        for (i = 0; i < MAX_TOOLCHEST_SLOTS && found == 0xFF; i++) {
            if (gToolchestSlots[i] == tool) {
                gToolchestSlots[i] = 0;
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

        for (i = 0; i < MAX_TOOLCHEST_SLOTS && !found; i++) {
            if (gToolchestSlots[i] == tool) {
                found = 2;
            }
        }

    }

    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/player", checkHaveKeyItem);

bool checkHaveKeyItem(u8 item) {

    u8 i;
    bool found = FALSE;
    
    for (i = 0; i < MAX_KEY_ITEMS && !found; i++) {

        if (gPlayer.keyItemSlots[i] == item) {
            found = TRUE;
        }
        
    }

    return found;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80065F5C);

void func_80065F5C(void) {
    gPlayer.startingCoordinates.x = entities[PLAYER].coordinates.x;
    gPlayer.startingCoordinates.y = entities[PLAYER].coordinates.y;
    gPlayer.startingCoordinates.z = entities[PLAYER].coordinates.z;
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80065F94);

Vec3f func_80065F94(f32 arg1, u8 arg2) {
    
    Vec3f vec;

    s8 buffer[10];
    s8 buffer2[10];

    memcpy(buffer, D_8011F3F0, 9);
    memcpy(buffer2, D_8011F3FC, 9);
    
    vec = func_800315A0(ENTITY_PLAYER);

    if (vec.y != 65535.0f) {

        vec.x += buffer[convertWorldToSpriteDirection(entities[PLAYER].direction, gMainMapIndex)] * arg1;
        vec.z += buffer2[convertWorldToSpriteDirection(entities[PLAYER].direction, gMainMapIndex)] * arg1;

        if (arg2 != 8) {
            
            // TODO: is this a separate macro?
            vec.x += buffer[((entities[PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + arg2) % 8)];
            vec.z += buffer2[((entities[PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + arg2) % 8)];
            
        }
    }

    return vec;
    
}

// alternate without memcpy
/*
Vec3f* func_80065F94(Vec3f *arg0, f32 arg1, u8 arg2) {
     
    Vec3f vec;
    CoordinateOffsetData struct1;
    CoordinateOffsetData struct2;

    s8 *ptr;
    s8 *ptr2;

    struct1 = *(CoordinateOffsetData*)D_8011F3F0;
    struct2 = *(CoordinateOffsetData*)D_8011F3FC;
    
    ptr = (s8*)&struct1;
    ptr2 = (s8*)&struct2;

    func_800315A0(&vec, 0);

    if (vec.y != 65535.0f) {D_8011F3F0

        vec.x += ptr[(entities[PLAYER].direction + getCurrentMapRotation(gMainMapIndex)) % 8] * arg1;
        vec.z += ptr2[(entities[PLAYER].direction + getCurrentMapRotation(gMainMapIndex)) % 8] * arg1;

        if (arg2 != 8) {
            
            vec.x += ptr[((entities[PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + arg2) % 8)];
            vec.z += ptr2[((entities[PLAYER].direction + getCurrentMapRotation(gMainMapIndex) + arg2) % 8)];
            
        }
    }

    *arg0 = vec;
    
    return arg0;
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/player", setPlayerAction);

void setPlayerAction(u16 action, u16 nextAction) {

    if (action != 0xFF) {
        gPlayer.actionProgress = 0;
        gPlayer.animationState = 0;
        gPlayer.currentAction = action;
    }

    if (nextAction != 0xFF) {
        gPlayer.nextAction = nextAction;
    }
    
}

//INCLUDE_RODATA("asm/nonmatchings/game/player", D_8011F3F0);
 
const s8 D_8011F3F0[12] = { 0, 0xFF, 0xFF, 0xFF, 0, 1, 1, 1, 0, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/player", D_8011F3FC);
 
const s8 D_8011F3FC[12] = { 1, 1, 0, 0xFF, 0xFF, 0xFF, 0, 1, 0, 0, 0, 0 };

// jtbl_8011F408
//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006623C);

void func_8006623C(void) {

    if (gPlayer.currentAction == 0) {
        // handle button input
        func_800664C8();
    }
    
    switch (gPlayer.currentAction - 1) {
        case 0:
            func_80067BC4();
            break;
        case TOOL_USE:
            func_80067E5C();
            break;
        case THROWING:
            func_80067EE0();
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
        case 9:
            func_80068340();
            break;
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
        case 18:
            func_80069C90();
            break;
        case WHISTLING:
            func_80069CC4();
            break;
        case DRINKING:
            func_80069DA8();
            break;
        case 21:
            func_80069DB0();
            break;
        case CASTING_FISHING_ROD:
            func_80069E54();
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
    
    func_800D7010();
    func_800D0318();

}

// handle button input
// D_8011F490
INCLUDE_ASM("asm/nonmatchings/game/player", func_800664C8);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80066F98);

void func_80066F98(void) {

    deactivateEntity(horseInfo.entityIndex);

    horseInfo.flags &= ~4;
    horseInfo.flags |= 8;
    gPlayer.flags |= 1;

    setDailyEventBit(0x5C);
 
    setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(horseInfo.direction, MAIN_MAP_INDEX));
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067034);

void func_80067034(void) {

    Vec3f vec1;
    Vec3f vec2;
    u8 direction; // not initialied; likely a bug
    bool set = FALSE;
    int temp;
    
    while (direction < 8 && !set) {
    
        if (checkTerrainCollisionInDirection(ENTITY_PLAYER, 8, convertWorldToSpriteDirection(direction, gMainMapIndex)) == 0) {

            if (checkTerrainCollisionInDirection(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(direction, gMainMapIndex)) == 0) {

                vec1 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(direction, MAIN_MAP_INDEX));
                
                temp = func_800DB1BC(vec1.x, vec1.z);
                
                if (temp == 0xFF || (func_800DA978(temp) & 8)) {
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
            horseInfo.direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            horseInfo.location = gBaseMapIndex;
            horseInfo.flags &= ~0x8;     

            gPlayer.flags &= -2;
            
            toggleDailyEventBit(0x5C);
            
            // initialize animal locations
            func_8008B9AC();
           
            entities[PLAYER].direction = direction;

            // FIXME: repeated gPlayer.actionProgress and gPlayer.animationState assignments indicates 2 macros
            gPlayer.currentAction = 0xE;
            gPlayer.actionProgress = 0;
            gPlayer.animationState = 0;
            gPlayer.nextAction = 0x10;

            gPlayer.animationState = 0;
            gPlayer.actionProgress = 0;

        }

}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80067290);

INCLUDE_ASM("asm/nonmatchings/game/player", func_80067950);

//INCLUDE_ASM("asm/nonmatchings/game/player", checkFatigueLevel);

inline u8 checkFatigueLevel(void) {

    u8 result;
     
    if (gPlayer.fatigue.counter == 100) {
        result = 3;
    } else if (gPlayer.fatigue.counter >= 75) {
        result = 2;
    }  else {
        result = (gPlayer.fatigue.counter < 50) == 0;  
    }

    return result;

}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80067A24);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067B38);

bool func_80067B38(void) {

    bool set = FALSE;
    
    u32 fatigue = checkFatigueLevel();

    if (fatigue != 0 && fatigue != gPlayer.fatigue.unk_2) {
        
        set = TRUE;

        setAction(0x19, 0, 0, 0x1A);

        gPlayer.fatigue.unk_2 = fatigue;
        
    }

    return set;

}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80067BC4);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067E5C);

void func_80067E5C(void) {

    // FIXME: ??
    if (!(*(s32*)&gPlayer.actionProgress & ~0xFF)) {
        setAudio(0x26);
        func_800D5548(gPlayer.itemInfoIndex);
        func_800D5390(1, 3, gPlayer.heldItem, 0, 8);
        gPlayer.heldItem = 0;
        gPlayer.actionProgress = 1;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80067EE0);

void func_80067EE0(void) {

    if (gPlayer.actionProgress == 3) {
        func_800D55E4(gPlayer.itemInfoIndex, 0xE);
        gPlayer.heldItem = 0;
        gPlayer.animationState++;
    }

    gPlayer.actionProgress++;

}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80067F50);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006807C);

void func_8006807C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER)) {
     
        if (gPlayer.animationState == 0) {
            func_800D55E4(gPlayer.itemInfoIndex, 9);
            gPlayer.heldItem = 0;
            setAudio(0x24);
        }

        if (gPlayer.animationState != 1 || (gPlayer.actionProgress++, gPlayer.actionProgress == 2)) {
            gPlayer.animationState++;
        }

    }   

}


//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068120);

// putting down action
void func_80068120(void) {

    if (gPlayer.animationState == 2) {

        if (gPlayer.actionProgress == 0xA) {
            func_800D55E4(gPlayer.itemInfoIndex, 1);
            gPlayer.heldItem = 0;
            gPlayer.animationState++;
            setAudio(0x25); 
        }

        gPlayer.actionProgress++;
    }

    if (gPlayer.animationState == 0) {

        if (gPlayer.actionProgress == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, func_800D5B00(gPlayer.heldItem), gMaximumStamina);
            gPlayer.fatigue.counter += adjustValue(gPlayer.fatigue.counter, -func_800D5B18(gPlayer.heldItem), 100);
        }

        if (gPlayer.actionProgress == 0x1E) {
            gPlayer.animationState++;
        }

        gPlayer.actionProgress++;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068258);

void func_80068258(void) {

    if (gPlayer.animationState == 0) {
    
        setEntityCoordinates(ENTITY_PLAYER, -140.0f, 0.0f, -152.0f);
        setEntityCollidable(ENTITY_PLAYER, FALSE);
        setEntityYMovement(ENTITY_PLAYER, FALSE);
        func_8002FF38(ENTITY_PLAYER, FALSE);
    
        gPlayer.animationState++;

        func_80034DC8(MAIN_MAP_INDEX, 0, 7);
    
    }

    if (gPlayer.animationState == 3) {
        func_80034DC8(MAIN_MAP_INDEX, 0, 8);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_800682F8);

void func_800682F8(void) {
    if (checkButtonHeld(CONTROLLER_1, 0xFFFFFFFF) && gPlayer.animationState < 4) {
        gPlayer.animationState = 4;
    }
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80068340);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068410);

void func_80068410(void) {

    if (gPlayer.animationState == 0) {
        if (gPlayer.actionProgress == 4) {
            if (func_800DCAA0(gPlayer.shopItemIndex)) {
                func_800D55E4(gPlayer.itemInfoIndex, 7);
            }
            gPlayer.animationState++;
        }
        gPlayer.actionProgress++;
    }
    
    if (gPlayer.animationState == 2) {
        if (func_800DCAA0(gPlayer.shopItemIndex)) {
            func_800D55E4(gPlayer.itemInfoIndex, 1);
        }
        loadShopItemSprite(gPlayer.shopItemIndex);
        gPlayer.animationState = 5;
    }
    
    if (gPlayer.animationState == 3) {
        if (func_800DCAA0(gPlayer.shopItemIndex)) {
            func_800D55E4(gPlayer.itemInfoIndex, 1);
        }
        func_800DC9C0(gPlayer.shopItemIndex);
        gPlayer.animationState = 5;
    }

}

// D_8011F498
INCLUDE_ASM("asm/nonmatchings/game/player", func_80068558);

// D_8011F4A0
INCLUDE_ASM("asm/nonmatchings/game/player", func_80068738);

void func_80068918(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068920);
  
void func_80068920(void) {
 
    if (gPlayer.actionProgress == 3) {
        func_800D55E4(gPlayer.itemInfoIndex, 0x12);
        gPlayer.heldItem = 0;
        gPlayer.animationState++;
    }

    gPlayer.actionProgress++;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068990);

void func_80068990(void) {

    if (gPlayer.actionProgress == 3) {
        func_800D55E4(gPlayer.itemInfoIndex, 0x14);
        gPlayer.heldItem = 0;
        gPlayer.animationState++;
    }

    gPlayer.actionProgress++;

}

void func_80068A00(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068A08);

void func_80068A08(void) {

    if (gPlayer.animationState == 0) {
        
        if (gPlayer.actionProgress == 0) {
            gPlayer.itemInfoIndex = func_800D5308(0, 2, gPlayer.heldItem, 0, 8);
        }
        
        if (gPlayer.actionProgress >= 0x1E) {
            gPlayer.animationState = 1;
        } else {
            gPlayer.actionProgress++;
        }
    }

}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80068A98);

INCLUDE_ASM("asm/nonmatchings/game/player", func_80068C8C);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80068DFC);

void func_80068DFC(void) {

    Vec3f vec;

    switch (gPlayer.animationState) {
        
        case 0:

            gPlayer.nextAction = 0;
            gPlayer.animationState++;
            
            break;
        
        case 1:

            gPlayer.nextAction = 1;
            gPlayer.direction = gPlayer.unk_6E;

            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));

            vec = getMovementVectorFromDirection(1.0f, gPlayer.direction, 0.0f);

            gPlayer.movementVector = vec;

            gPlayer.animationState++;
            
            break;
        
        case 2:
                
            if (gPlayer.actionTimer == 0) {
                
                gPlayer.animationState++;
                
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

INCLUDE_ASM("asm/nonmatchings/game/player", func_80068FB0);

INCLUDE_ASM("asm/nonmatchings/game/player", func_800692E4);

INCLUDE_ASM("asm/nonmatchings/game/player", func_80069830);

// FIXME: have to fix D_801890C8 usage
/*
void func_80069830(void) {

    Vec3f vec;
    Vec3f vec2;

    switch (gPlayer.animationState) {

        case 0:

            setEntityCollidable(ENTITY_PLAYER, FALSE);
            setEntityTracksCollisions(ENTITY_PLAYER, FALSE);
            enableEntityMovement(ENTITY_PLAYER, FALSE);
            setEntityYMovement(ENTITY_PLAYER, FALSE);
            func_80038990(ENTITY_PLAYER, 1, 0);
            gPlayer.direction = SOUTH;
            gPlayer.actionTimer = 0x12;
            gPlayer.animationState++;
            vec = getMovementVectorFromDirection(4.0f, 4, 0.0f);
            gPlayer.movementVector = vec;
            setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
            gPlayer.nextAction = 2;
            break;

        case 1:

            if (gPlayer.actionTimer == 0) {
                func_80038990(ENTITY_PLAYER, 0, 0);
                gPlayer.direction = SOUTHWEST;
                // to-do: replace with static inline
                gPlayer.currentAction = 0xC;
                gPlayer.movementVector.x = 0;
                gPlayer.movementVector.y = 0;
                gPlayer.movementVector.z = 0;
                gPlayer.actionProgress = 0;
                D_801890C8 = 0;
                gPlayer.animationState = 0;
                gPlayer.nextAction = 0xE;
                gPlayer.flags |= 0x10;
            } else {
                gPlayer.actionTimer--;
            }
            break;

        case 2:

            if (gPlayer.actionProgress == 0x3C) {
                gPlayer.direction = NORTHEAST;
                // replace with single static inline
                gPlayer.currentAction = 0xC;
                resetAction2();
                gPlayer.nextAction = 0xE;
                gPlayer.flags |= 0x40;
            } else {
                gPlayer.actionProgress++;
            }

            break;

        case 3:

            if (gPlayer.actionTimer == 0) {
                
                func_80038990(ENTITY_PLAYER, 1, 0);
                
                setAudio(0x32);
                
                gPlayer.direction = NORTH;
                gPlayer.actionTimer = 0x12;
                
                vec = getMovementVectorFromDirection(4.0f, 0, 0.0f);
                
                gPlayer.movementVector = vec2;
                
                setEntityDirection(ENTITY_PLAYER, convertSpriteToWorldDirection(gPlayer.direction, MAIN_MAP_INDEX));
                
                gPlayer.nextAction = 2;
                gPlayer.animationState++;

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
                func_80038990(ENTITY_PLAYER, 0, 0);

                resetAction();

                toggleDailyEventBit(6);

                if (!checkDailyEventBit(0x33)) {
                    gPlayer.fatigue.counter += adjustValue(gPlayer.fatigue.counter, -10, 100);
                }

                setDailyEventBit(0x33);

            } else {
                gPlayer.actionTimer--;
            }

            break;

    }

    setEntityMovementVector(ENTITY_PLAYER, gPlayer.movementVector.x, 0.0f, gPlayer.movementVector.z, 1.0f);

}
*/

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069C5C);

void func_80069C5C(void) {
    
    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        setAudio(0xA);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069C90);

void func_80069C90(void) {

    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        setAudio(0xA);
    }

}
 
//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069CC4);

void func_80069CC4(void) {

    if (gPlayer.animationState == 0) {

        if (gPlayer.actionProgress == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, func_800D5B00(gPlayer.heldItem), gMaximumStamina);
            gPlayer.fatigue.counter += adjustValue(gPlayer.fatigue.counter, -func_800D5B18(gPlayer.heldItem), 0x64);
        }
        
        if (gPlayer.actionProgress == 0x1E) {
            func_800D55E4(gPlayer.itemInfoIndex, 1);
            gPlayer.animationState++;
        }
        
        gPlayer.actionProgress++;
        
    }
}

void func_80069DA8(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_80069DB0);

void func_80069DB0(void) {

    if (gPlayer.animationState == 2) {

        if (!getRandomNumberInRange(0, 400)) {
            gPlayer.animationState = 3;
        }
    }
    
    if (gPlayer.animationState && checkButtonReleased(CONTROLLER_1, BUTTON_B)) {

        switch (gPlayer.animationState) {
            
            case 3 ... 5:
                gPlayer.animationState = 6;
                break;
            default:
                gPlayer.animationState = 8;
                break;
        }
    }
}

void func_80069E54(void) {}

void func_80069E5C(void) {}

void func_80069E64(void) {} 

void func_80069E6C(void) {}

INCLUDE_ASM("asm/nonmatchings/game/player", func_80069E74);

void func_8006A2E0(void) {} 
 
//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A2E8);

void func_8006A2E8(void) {

    switch (gPlayer.nextAction) {
        
        case 0:
            
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                
                if ((gPlayer.flags & 1)) {
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
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18);
            }
            
            if (!(gPlayer.flags & 1)) {

                if (playerIdleCounter == 1200) {
                    gPlayer.currentAction = 9;
                    gPlayer.actionProgress = 0;
                    gPlayer.animationState = 0;
                    gPlayer.nextAction = 0xB;
                } else {
                    playerIdleCounter++;
                }

            }

            break;
        
        default:
            break;
        
        case 1:
            
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {
                if (gPlayer.flags & 1) {
                   setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x20C);
                } else {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 8);
                }
            } else {
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x20);
            }
            
            playerIdleCounter = 0;
            break;
        
        case 2:
            
            if (!gPlayer.heldItem && !checkDailyEventBit(0xD)) {

                if (gPlayer.flags & 1) { 
                    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                        setAudio(9);
                    }
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x214);

                } else {
                    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                        setAudio(5);
                    }
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x10);
                }

            } else {

                if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
                    setAudio(5);
                }

                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x28);
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
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
        
        if (gPlayer.animationState == 0) {
        
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1A5);
            gPlayer.animationState++;
        
        } else {

            handleStopHolding();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A7B0);

static inline updateAnimation(u16 animation) {
    
     if (gPlayer.animationState == 0) {

        setEntityAnimationWithDirectionChange(ENTITY_PLAYER, animation);
        func_800D55E4(gPlayer.itemInfoIndex, 0xD);
    
    } else {

        handleStopHolding();

    }
}

void func_8006A7B0(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
       updateAnimation(0x1A5);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A848);

void func_8006A848(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
    
        if (gPlayer.animationState == 0) {

            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
            
        } else {

            resetAction();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006A8C4);

void func_8006A8C4(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
        switch (gPlayer.animationState) {                       
            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                func_800D55E4(gPlayer.itemInfoIndex, 8);
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
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {

        switch (gPlayer.animationState) {       

            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18);
                break;
            case 1:
                func_800D55E4(gPlayer.itemInfoIndex, 0xA);
                gPlayer.actionProgress = 0;
                gPlayer.animationState++;
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

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
        
        if (gPlayer.animationState == 0) {
            
            switch (gPlayer.fatigue.level) {       
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
    
            gPlayer.animationState++;

        } else {

            resetAction();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AB90);

void func_8006AB90(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
        
        if (gPlayer.animationState == 0) {

            switch (gPlayer.fatigue.unk_2) {                   
                case 1:
                    setEntityAnimation(ENTITY_PLAYER, 0x299);
                    break;
                case 2:
                    setEntityAnimation(ENTITY_PLAYER, 0x29A);
                    break;
                case 3:
                    break;
                }
            
            gPlayer.animationState++;
        
        } else {

            resetAction();

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AC4C);

// handle wife going to sleep at the same time
void func_8006AC4C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {

        switch (gPlayer.animationState) {         

            case 1:

                setEntityAnimation(ENTITY_PLAYER, 0x1D4);
                gPlayer.animationState++;

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
                gPlayer.animationState++;
                break;

            case 3:
                setEntityAnimation(ENTITY_PLAYER, 0x1D6);
                gPlayer.animationState++;
                break;             

            default:
                if (gPlayer.animationState >= 10) {
                    func_8005C940(8, END_OF_DAY_1);    
                } else {
                    gPlayer.animationState++;
                }
                
                break;

         }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006ADF4);

// napping/idle animation
void func_8006ADF4(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {

        switch (gPlayer.animationState) { 
            case 0:
                setEntityAnimation(ENTITY_PLAYER, 0x1E0);
                gPlayer.animationState++;
                break;
            case 1:
                setEntityAnimation(ENTITY_PLAYER, 0x1E1);
                gPlayer.animationState++;
                break;
            case 2:
                playerIdleCounter++;
                if (playerIdleCounter == 1800) {
                    setEntityAnimation(ENTITY_PLAYER, 0x1E2);
                    gPlayer.animationState++;
                }
                break;
            case 4:
                setEntityAnimation(ENTITY_PLAYER, 0x1E3);
                gPlayer.animationState++;
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
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {

        switch (gPlayer.animationState) { 
            case 0:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                break;
            case 1:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1BF);
                gPlayer.animationState++;
                break;
            case 3:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1BF);
                break;            
            case 4:
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
                gPlayer.animationState++;
                break;           
            default:
                resetAction();
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006AFE4);

void func_8006AFE4(void) {
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {

        switch (gPlayer.animationState) {  
            case 0:
                if (!func_800DCAA0(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
                    break;
                }
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
                break;
            case 1:
                // shop.c
                func_800DC750(gPlayer.shopItemIndex);
                gPlayer.actionProgress = 0;
                func_80059334();
                break;
            case 2:
                if (func_800DCAA0(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
                }
                break;
            case 3:
                if (func_800DCAA0(gPlayer.shopItemIndex)) {
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1AD);
                }
                break;
            default:
                resetAction();
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B104);

void func_8006B4D4(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B4DC);

void func_8006B4DC(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || (gPlayer.animationState == 0)) {
        
        if (gPlayer.animationState == 0) {
            
            setEntityAnimation(ENTITY_PLAYER, 0x1DB);
            
            gPlayer.animationState = 1;
            gPlayer.heldItem = 0xCA;
            gPlayer.itemInfoIndex = func_800D5308(0, 0x10, 0xCA, 0, 8);
            
        } else {
            
            setMainLoopCallbackFunctionIndex(PINK_OVERLAY_TEXT);
        
        }
    }    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B584);

void func_8006B584(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {
        
        if (gPlayer.animationState == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1B7);
            func_800D55E4(gPlayer.itemInfoIndex, 0x11);
            
        } else {
            
            resetAction();
            gItemBeingHeld = 0xFF;
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B61C);

void func_8006B61C(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {
        
        if (gPlayer.animationState == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x1A5);
            func_800D55E4(gPlayer.itemInfoIndex, 0xD);
            
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

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {

        if (gPlayer.animationState == 0) {
            setEntityAnimation(ENTITY_PLAYER, 0x2B3);
            gPlayer.animationState++;
        } else {
            resetAction();
        }
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B77C);

void func_8006B77C(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {

        switch (gPlayer.animationState) {

            case 0:
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18);
                break;
            
            case 1:
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x26A);
                func_800D55E4(gPlayer.itemInfoIndex, 1);
                gPlayer.animationState++;
                break;
            
            default:
                
                if (gPlayer.heldItem == 0xDA) {
                    
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

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {
        
        if (gPlayer.animationState == 0) {
             setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x21C);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B910);

void func_8006B910(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {
        
        if (gPlayer.animationState == 0) {
             setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x25C);
        }
    }
    
}

void func_8006B964(void) {}

void func_8006B96C(void) {}

void func_8006B974(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006B97C);

void func_8006B97C(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {

        if (gPlayer.animationState == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x19D);
            gPlayer.animationState++;
        } else {
            resetAction();
            // animals.c
            func_8009B11C();
        }
    }

}


//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006BA14);

void func_8006BA14(void) {
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {

        if (gPlayer.animationState == 0) {
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x19D);
            gPlayer.animationState++;
        } else {
            resetAction();
            // animals.c
            func_8009B1BC();
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006BAAC);

void func_8006BAAC(void) {

    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {

        switch (gPlayer.animationState) {

            case 0:
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18);
                break;
            
            case 1:

                if (func_800D5A6C(gPlayer.heldItem) & 0x4000) {
                    setEntityAnimation(ENTITY_PLAYER, 0x2B3);
                    gAlcoholTolerance += adjustValue(gAlcoholTolerance, 1, 0xFF);
                } else {
                    setEntityAnimation(ENTITY_PLAYER, 0x1B6);
                }
                
                gPlayer.heldItem = 0;
                gPlayer.animationState++;
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
    
    if ((checkEntityAnimationStateChanged(ENTITY_PLAYER)) || (gPlayer.animationState == 0)) {
        
        if (gPlayer.animationState == 0) {
            
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x26A);
            func_800D55E4(gPlayer.itemInfoIndex, 1);
            gPlayer.heldItem = 0;
            gPlayer.animationState++;
            setAudio(0x24);
            
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

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
        
        switch (gPlayer.animationState) {
            
            case 0:
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x182);
                gPlayer.animationState++;
                break;
            
            case 1:
                
                if (func_800AD0C4(gBaseMapIndex)) {
                
                    vec = func_80065F94(2.0f, 8);
                    vec = func_8003AF58(0, vec.x, vec.z);

                    switch (gPlayer.fatigue.unk_3) {
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

                    func_800CFB38(0, 0x9D, vec.x, vec.y, vec.z);
                    setAudio(0x28);
                    setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x17A);
                    gPlayer.animationState = 2;
                
                } else {
                    gPlayer.animationState = 8;
                }
                
                break;
            
            case 3:
                func_800CFD78(0, 0x9E);
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x18A);
                gPlayer.animationState = 4;
                break;
            
            case 4:
                gPlayer.animationState = 5;
                break;
            
            case 5:
                gPlayer.animationState = 1;
                break;
            
            // catch fish
            case 7:

                // ?
                temp2 = gPlayer.fatigue.unk_3;
                max = 4;

                if (gPlayer.fatigue.unk_3 == 0) {
                    temp = 1;
                } else if (0 <= temp2 && gPlayer.fatigue.unk_3 < max) {
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

                gPlayer.animationState = 9;

                gHappiness += adjustValue(gHappiness, 1, MAX_HAPPINESS);

                // get power nut 
                if (gPlayer.fatigue.unk_3 == 3 && !(powerNutBits & FISHING_POWER_NUT) && !getRandomNumberInRange(0, 50)) {
                    
                    gPlayer.heldItem = POWER_NUT;

                    setAction(6, 0, 0, 8);
                    
                    powerNutBits |= FISHING_POWER_NUT;
                    
                    gMaximumStamina += adjustValue(gMaximumStamina, 15, MAX_STAMINA);
                    gHappiness += adjustValue(gHappiness, 4, MAX_HAPPINESS);


                }

                totalFishCaught += adjustValue(totalFishCaught, 1, MAX_FISH_CAUGHT);

                handleEatingAndDrinking();
                
                break;
            
            case 6:
            case 8:
                func_800CFDA0(0);
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x192);
                gPlayer.animationState++;
                break;
              
            default:            
                resetAction();
            case 2:
                break;
        }
    }

    if (checkEntityShouldPlaySoundEffect(ENTITY_PLAYER)) {
        setAudio(0xB);
    }
    
}

void func_8006C12C(void) {}

void func_8006C134(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C13C);

// climbing trees
void func_8006C13C(void) {

    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0 || gPlayer.animationState == 2) {
        switch (gPlayer.animationState) {
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

// array indexing is off
#ifdef PERMUTER
void func_8006C1DC(void) {

    UnknownPlayerData data;
    u8 *ptr;

    data = *(UnknownPlayerData*)D_8011F5D4;
    ptr = (u8*)&data;
    
    if (checkEntityAnimationStateChanged(ENTITY_PLAYER) || gPlayer.animationState == 0) {
        switch (gPlayer.animationState) {
            case 0:
                setEntityAnimation(ENTITY_PLAYER, 0x171);
                gPlayer.animationState = 1;
                gPlayer.heldItem = *(&ptr[D_801A8B5C+D_8018A724*3]-3);
                gPlayer.itemInfoIndex = func_800D5308(0, 0x15, gPlayer.heldItem, 0, 8);
                break;
            case 1:
                gPlayer.animationState = 2;
                break;
            case 2:
                gPlayer.animationState = 3;
                setAudio(0x5A);
                setMainLoopCallbackFunctionIndex(0xB);
                func_80059334();
                break;
            default:
                func_800D55E4(gPlayer.itemInfoIndex, 1);
                gPlayer.heldItem = 0;
                resetAction();
                break;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C1DC);
#endif

// u8 array, probably 2d
INCLUDE_RODATA("asm/nonmatchings/game/player", D_8011F5D4);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C384);

// jtbl_8011F6D8
INCLUDE_ASM("asm/nonmatchings/game/player", func_8006C628);


// tool animation

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CD84);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CDF8);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CE6C);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CEE0);

void func_8006CEE0(void) {

    switch (gPlayer.staminaLevelForCurrentToolUse) {
        
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

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006CF54);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D0AC);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D21C);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D304);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D3FC);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D56C);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D690);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D7B4);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D8D8);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006D9FC);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DB20);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DC44);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DD68);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DE8C);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006DFB0);

void func_8006DFB0(void) {

    switch (gPlayer.animationState) {
        case 0:
            setEntityAnimation(ENTITY_PLAYER, 0);
            break;
        case 1:
            setEntityAnimation(ENTITY_PLAYER, 0x148);
            break;
        case 2:
            func_800CF850();
            gPlayer.animationState++;
            break;
        case 3:            
            if (!toolUse.unk_E) {
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

    switch (gPlayer.animationState) {
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x169);
            break;
        case 2:
            func_800CF850();
            gPlayer.animationState++;
            break;
        case 3:
            if (!toolUse.unk_E) {
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

void func_8006E1F8(void) {}

void func_8006E200(void) {}

void func_8006E208(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E210);

void func_8006E210(void) {
    gPlayer.currentAction = 0x16;
    gPlayer.actionProgress = 0;
    gPlayer.animationState = 0;
    gPlayer.nextAction = 0x17;
}

// empty bottle
//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E240);

void func_8006E240(void) {

    switch (gPlayer.animationState) {                           
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            func_80099DE8();
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x234);
            break;
        case 2:
            func_800CF850();
            gPlayer.animationState++;
            break;
        case 3:
            if (!toolUse.unk_E) {
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

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E348);

INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E450);

//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E574);

void func_8006E574(void) {

    switch (gPlayer.animationState) {
        case 0:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
            func_800305CC(ENTITY_PLAYER, 0.0f, 8.0f, 0);
            func_80085C94();
            break;
        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x234);
            break;
        case 2:
            func_800CF850();
            gPlayer.animationState++;
            break;
        case 3:
            if (toolUse.unk_E == 0 && !func_80067A24(0)) {
                resetAction();
            }
            break;
        default:
            break;
    }
    
}

// jtbl_8011F6F0
//INCLUDE_ASM("asm/nonmatchings/game/player", func_8006E678);

void func_8006E678(void) {

    switch (gPlayer.animationState) {

        case 0:

            if (gPlayer.bottleContents == 0) {
                
                setEntityAnimationWithDirectionChange(ENTITY_PLAYER, STANDING);
                func_80099DE8();
                
            } else {

                setAction2(0x14, 0x15);
                
                switch (gPlayer.bottleContents) {
                    case 1:
                        gPlayer.heldItem = 0x71;
                        break;
                    case 2:
                        gPlayer.heldItem = BOTTLE_WITH_WINE;
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
                
                handleEatingAndDrinking();

                gPlayer.bottleContents = 0;       

            }
            
            break;

        case 1:
            setEntityAnimationWithDirectionChange(ENTITY_PLAYER, 0x272);
            break;

        case 2:
            func_800CF850();
            gPlayer.animationState++;
            break;

        case 3:
            if (toolUse.unk_E == 0 && !func_80067A24(0)) {
                resetAction();
            }    
            break;

        default:
            break;
    }
}