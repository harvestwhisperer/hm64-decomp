#include "common.h"

#include "player.h"

#include "system/controller.h"
#include "system/graphic.h"
#include "system/mathUtils.h"
#include "system/sprite.h"
#include "system/mapContext.h"

#include "animals.h"
#include "game.h"
#include "gameAudio.h"
#include "gameStatus.h"
#include "itemHandlers.h"
#include "level.h"
#include "mainproc.h"
#include "mapObjects.h"
#include "npc.h"
#include "shop.h"
 
// forward declaration
u8 func_80067A24(u8);                               

// consumable tool counters (seeds, feed)
extern u8 D_802373A8;
extern u16 D_80237410;

// part of player struct, but necessary for matching right now
extern u16 D_801890C8;

// possible bss
extern u8 gMaximumStamina;
extern u8 gToolchestSlots[];   

// data
extern Vec3f playerDefaultStartingCoordinates[];
// data
extern u8 playerDefaultStartingDirections[];

// rodata
extern s8 D_8011F3F0[12];
extern s8 D_8011F3FC[12]; 
 

static inline void resetAction() {
    gPlayer.action3 = 0;
    gPlayer.action4 = 0;
    gPlayer.action1 = 0;
    gPlayer.action2 = 0;
}

static inline void resetAction2() {
    gPlayer.action3 = 0;
    gPlayer.action4 = 0;
}

//INCLUDE_ASM(const s32, "player", setupPlayerSprite);

void setupPlayerSprite(u16 arg0, u8 resetPlayer) {
 
    func_8002E284(PLAYER, 0, 1);
    func_8003019C(PLAYER, 1);
    func_80030054(PLAYER, 1);
    func_8002FF38(PLAYER, 1);
    func_8002FCB4(PLAYER, 1);
    func_8002F014(PLAYER, globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a);
    func_8002F114(PLAYER, 0);

    if (resetPlayer) {
        gPlayer.startingCoordinates.x = playerDefaultStartingCoordinates[arg0].x;
        gPlayer.startingCoordinates.y = playerDefaultStartingCoordinates[arg0].y;
        gPlayer.startingCoordinates.z = playerDefaultStartingCoordinates[arg0].z;
        gPlayer.direction = playerDefaultStartingDirections[arg0];
    }

    setSpriteDirection(PLAYER, (gPlayer.direction + 8 - func_8003C1A4(0)) % 8);
    func_8002FD80(PLAYER, gPlayer.startingCoordinates.x, gPlayer.startingCoordinates.y, gPlayer.startingCoordinates.z);

    D_80189828.unk_3 = 0;
    D_80189828.unk_4 = 0;
    D_80189828.unk_6 = 0;
    D_80189828.unk_8 = 0;
    D_80189828.unk_A = 0;
    D_80189828.unk_C = 0;

}

INCLUDE_ASM(const s32, "player", func_80065AA0);

//INCLUDE_ASM(const s32, "player", addItemToRucksack);

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

//INCLUDE_ASM(const s32, "player", func_80065BCC);

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

//INCLUDE_ASM(const s32, "player", removeTool);

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

//INCLUDE_ASM(const s32, "player", acquireKeyItem);

u8 acquireKeyItem(u8 item) {

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

//INCLUDE_ASM(const s32, "player", removeKeyItem);

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

//INCLUDE_ASM(const s32, "player", checkHaveTool);

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

//INCLUDE_ASM(const s32, "player", checkHaveKeyItem);

bool checkHaveKeyItem(u8 item) {

    u8 i;
    bool found = 0;
    
    for (i = 0; i < MAX_KEY_ITEMS && !found; i++) {

        if (gPlayer.keyItemSlots[i] == item) {
            found = 1;
        }
    }

    return found;
}

//INCLUDE_ASM(const s32, "player", func_80065F5C);

void func_80065F5C(void) {
    gPlayer.startingCoordinates.x = renderedSprites[PLAYER].startingCoordinates.x;
    gPlayer.startingCoordinates.y = renderedSprites[PLAYER].startingCoordinates.y;
    gPlayer.startingCoordinates.z = renderedSprites[PLAYER].startingCoordinates.z;
}

//INCLUDE_ASM(const s32, "player", func_80065F94);

Vec3f* func_80065F94(Vec3f *arg0, f32 arg1, u8 arg2) {
    
    Vec3f vec;
    UnknownStruct4 struct1;
    UnknownStruct4 struct2;

    s8 *ptr;
    s8 *ptr2;

    struct1 = *(UnknownStruct4*)D_8011F3F0;
    struct2 = *(UnknownStruct4*)D_8011F3FC;
    
    ptr = (s8*)&struct1;
    ptr2 = (s8*)&struct2;

    func_800315A0(&vec, 0);

    if (vec.y != MAX_UNSIGNED_SHORT) {

        vec.x += ptr[(renderedSprites[PLAYER].direction + func_8003C1A4(D_801FD610)) % 8] * arg1;
        vec.z += ptr2[(renderedSprites[PLAYER].direction + func_8003C1A4(D_801FD610)) % 8] * arg1;

        if (arg2 != 8) {
            
            vec.x += ptr[((renderedSprites[PLAYER].direction + func_8003C1A4(D_801FD610)+ arg2) % 8)];
            vec.z += ptr2[((renderedSprites[PLAYER].direction + func_8003C1A4(D_801FD610) + arg2) % 8)];
            
        }
    }

    *arg0 = vec;
    
    return arg0;
}

//INCLUDE_ASM(const s32, "player", setPlayerAction);

void setPlayerAction(u16 arg0, u16 arg1) {

    if (arg0 != 0xFF) {
        gPlayer.action3 = 0;
        gPlayer.action4 = 0;
        gPlayer.action1 = arg0;
    }

    if (arg1 != 0xFF) {
        gPlayer.action2 = arg1;
    }
    
}

//INCLUDE_RODATA(const s32, "player", D_8011F3F0);
 
const s8 D_8011F3F0[12] = { 0, 0xFF, 0xFF, 0xFF, 0, 1, 1, 1, 0, 0, 0, 0 };

//INCLUDE_RODATA(const s32, "player", D_8011F3FC);
 
const s8 D_8011F3FC[12] = { 1, 1, 0, 0xFF, 0xFF, 0xFF, 0, 1, 0, 0, 0, 0 };

// jtbl_8011F408
//INCLUDE_ASM(const s32, "player", func_8006623C);

void func_8006623C(void) {

    if (gPlayer.action1 == 0) {
        // handle button input
        func_800664C8();
    }
    
    switch (gPlayer.action1 - 1) {
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

// D_8011F490
INCLUDE_ASM(const s32, "player", func_800664C8);

//INCLUDE_ASM(const s32, "player", func_80066F98);

void func_80066F98(void) {

    func_8002FA2C(horseInfo.unk_14);

    horseInfo.flags &= ~4;
    horseInfo.flags |= 8;
    gPlayer.flags |= 1;

    setDailyEventBit(0x5C);
 
    setSpriteDirection(PLAYER, (horseInfo.direction + 8 - func_8003C1A4(0)) % 8);
    
}

//INCLUDE_ASM(const s32, "player", func_80067034);

void func_80067034(void) {

    Vec3f vec1;
    Vec3f vec2;
    u8 i; // not initialied: likely a bug
    u8 set = 0;
    int temp;
    
    while (i < 8 && !set) {
    
        if (func_80031830(0, 8, (i + func_8003C1A4(D_801FD610)) % 8) == 0) {

            if (func_80031830(0, 0x20, (i + func_8003C1A4(D_801FD610)) % 8) == 0) {

                func_80031904(&vec1, 0, 0x20, (i + func_8003C1A4(0)) % 8);
                
                temp = func_800DB1BC(vec1.x, vec1.z);
                
                if (temp == 0xFF || (func_800DA978(temp) & 8)) {
                    set = 1;
                }
            }
        }

        if (!set) {
            i++;
        }
    }

    if (set) {

            func_80031904(&vec2, 0, 0, i);
            
            horseInfo.coordinates = vec2;
            
            horseInfo.direction = (renderedSprites[PLAYER].direction + func_8003C1A4(0)) % 8;
            horseInfo.location = gBaseMapIndex;
            horseInfo.flags &= ~0x8;            
            gPlayer.flags &= -2;
            
            toggleDailyEventBit(0x5C);
            func_8008B9AC();
           
            renderedSprites[PLAYER].direction = i;

            gPlayer.action1 = 0xE;
            gPlayer.action3 = 0;
            gPlayer.action4 = 0;
            gPlayer.action2 = 0x10;
            gPlayer.action4 = 0;
            gPlayer.action3 = 0;

        }
}

INCLUDE_ASM(const s32, "player", func_80067290);

INCLUDE_ASM(const s32, "player", func_80067950);

//INCLUDE_ASM(const s32, "player", checkFatigueLevel);

u8 checkFatigueLevel(void) {

    u8 temp;
     
    if (gPlayer.fatigue.counter == 100) {
        temp = 3;
    } else if (gPlayer.fatigue.counter >= 75) {
        return 2;
    }  else {
        return (gPlayer.fatigue.counter < 50) == 0;  
    }

    return temp;
}

INCLUDE_ASM(const s32, "player", func_80067A24);

INCLUDE_ASM(const s32, "player", func_80067B38);

INCLUDE_ASM(const s32, "player", func_80067BC4);

//INCLUDE_ASM(const s32, "player", func_80067E5C);

void func_80067E5C(void) {

    // ?
    if (!(*(s32*)&gPlayer.action3 & ~0xFF)) {
        setAudio(0x26);
        func_800D5548(gPlayer.unk_2D);
        func_800D5390(1, 3, gPlayer.unk_2C, 0, 8);
        gPlayer.unk_2C = 0;
        gPlayer.action3 = 1;
    }
}

//INCLUDE_ASM(const s32, "player", func_80067EE0);

void func_80067EE0(void) {

    if (gPlayer.action3 == 3) {
        func_800D55E4(gPlayer.unk_2D, 0xE);
        gPlayer.unk_2C = 0;
        gPlayer.action4 += 1;
    }

    gPlayer.action3 += 1;
}

INCLUDE_ASM(const s32, "player", func_80067F50);

//INCLUDE_ASM(const s32, "player", func_8006807C);

void func_8006807C(void) {

    if (func_8002FECC(0)) {
     
        if (!gPlayer.action4) {
            func_800D55E4(gPlayer.unk_2D, 9);
            gPlayer.unk_2C = 0;
            setAudio(0x24);
        }

        if (gPlayer.action4 != 1 || (gPlayer.action3++, gPlayer.action3 == 2)) {
            gPlayer.action4++;
        }
    }   
}


//INCLUDE_ASM(const s32, "player", func_80068120);

// putting down action
void func_80068120(void) {

    if (gPlayer.action4 == 2) {

        if (gPlayer.action3 == 0xA) {
            func_800D55E4(gPlayer.unk_2D, 1);
            gPlayer.unk_2C = 0;
            gPlayer.action4++;
            setAudio(0x25); 
        }

        gPlayer.action3++;
    }

    if (gPlayer.action4 == 0) {

        if (gPlayer.action3 == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, func_800D5B00(gPlayer.unk_2C), gMaximumStamina);
            gPlayer.fatigue.counter += adjustValue(gPlayer.fatigue.counter, -func_800D5B18(gPlayer.unk_2C), 100);
        }

        if (gPlayer.action3 == 0x1E) {
            gPlayer.action4++;
        }

        gPlayer.action3++;
    }
}

//INCLUDE_ASM(const s32, "player", func_80068258);

void func_80068258(void) {

    if (gPlayer.action4 == 0) {
        func_8002FD80(0, -140.0f, 0.0f, -152.0f);
        func_8003019C(0, 0);
        func_80030054(0, 0);
        func_8002FF38(0, 0);
        gPlayer.action4 += 1;
        func_80034DC8(0, 0, 7);
    }

    if (gPlayer.action4 == 3) {
        func_80034DC8(0, 0, 8);
    }
}

//INCLUDE_ASM(const s32, "player", func_800682F8);

void func_800682F8(void) {
    if (func_8004D35C(0, -1) && gPlayer.action4 < 4) {
        gPlayer.action4 = 4;
    }
}

INCLUDE_ASM(const s32, "player", func_80068340);

//INCLUDE_ASM(const s32, "player", func_80068410);

void func_80068410(void) {

    if (gPlayer.action4 == 0) {
        if (gPlayer.action3 == 4) {
            if (func_800DCAA0(gPlayer.unk_6B)) {
                func_800D55E4(gPlayer.unk_2D, 7);
            }
            gPlayer.action4++;
        }
        gPlayer.action3++;
    }
    
    if (gPlayer.action4 == 2) {
        if (func_800DCAA0(gPlayer.unk_6B)) {
            func_800D55E4(gPlayer.unk_2D, 1);
        }
        func_800DC7BC(gPlayer.unk_6B);
        gPlayer.action4 = 5;
    }
    
    if (gPlayer.action4 == 3) {
        if (func_800DCAA0(gPlayer.unk_6B)) {
            func_800D55E4(gPlayer.unk_2D, 1);
        }
        func_800DC9C0(gPlayer.unk_6B);
        gPlayer.action4 = 5;
    }
}

// D_8011F498
INCLUDE_ASM(const s32, "player", func_80068558);

// D_8011F4A0
INCLUDE_ASM(const s32, "player", func_80068738);

void func_80068918(void) {}

//INCLUDE_ASM(const s32, "player", func_80068920);
  
void func_80068920(void) {
 
    if (gPlayer.action3 == 3) {
        func_800D55E4(gPlayer.unk_2D, 0x12);
        gPlayer.unk_2C = 0;
        gPlayer.action4++;
    }

    gPlayer.action3++;
}

//INCLUDE_ASM(const s32, "player", func_80068990);

void func_80068990(void) {

    if (gPlayer.action3 == 3) {
        func_800D55E4(gPlayer.unk_2D, 0x14);
        gPlayer.unk_2C = 0;
        gPlayer.action4++;
    }

    gPlayer.action3++;
}

void func_80068A00(void) {}

INCLUDE_ASM(const s32, "player", func_80068A08);

INCLUDE_ASM(const s32, "player", func_80068A98);

INCLUDE_ASM(const s32, "player", func_80068C8C);

INCLUDE_ASM(const s32, "player", func_80068DFC);

INCLUDE_ASM(const s32, "player", func_80068FB0);

INCLUDE_ASM(const s32, "player", func_800692E4);

//INCLUDE_ASM(const s32, "player", func_80069830);

void func_80069830(void) {

    Vec3f vec;
    Vec3f vec2;

    switch (gPlayer.action4) {

        case 0:

            func_8003019C(PLAYER, 0);
            func_80030240(PLAYER, 0);
            func_800302E4(PLAYER, 0);
            func_80030054(PLAYER, 0);
            func_80038990(PLAYER, 1, 0);
            gPlayer.direction = 4;
            gPlayer.unk_6F = 0x12;
            gPlayer.action4++;
            func_80028520(&vec, 4.0f, 4, 0);
            gPlayer.currentCoordinates = vec;
            setSpriteDirection(PLAYER, (gPlayer.direction + 8 - func_8003C1A4(0)) % 8);
            gPlayer.action2 = 2;
            break;

        case 1:

            if (gPlayer.unk_6F == 0) {
                func_80038990(PLAYER, 0, 0);
                gPlayer.direction = 5;
                gPlayer.action1 = 0xC;
                gPlayer.currentCoordinates.x = 0;
                gPlayer.currentCoordinates.y = 0;
                gPlayer.currentCoordinates.z = 0;
                gPlayer.action3 = 0;
                D_801890C8 = 0;
                gPlayer.action4 = 0;
                gPlayer.action2 = 0xE;
                gPlayer.flags |= 0x10;
            } else {
                gPlayer.unk_6F--;
            }
            break;

        case 2:

            if (gPlayer.action3 == 0x3C) {
                gPlayer.direction = 1;
                gPlayer.action1 = 0xC;
                resetAction2();
                gPlayer.action2 = 0xE;
                gPlayer.flags |= 0x40;
            } else {
                gPlayer.action3++;
            }

            break;

        case 3:

            if (gPlayer.unk_6F == 0) {
                func_80038990(PLAYER, 1, 0);
                setAudio(0x32);
                gPlayer.direction = 0;
                gPlayer.unk_6F = 0x12;
                func_80028520(&vec2, 4.0f, 0, 0);
                gPlayer.currentCoordinates = vec2;
                setSpriteDirection(PLAYER, (gPlayer.direction + 8 - func_8003C1A4(0)) % 8);
                gPlayer.action2 = 2;
                gPlayer.action4++;
            } else {
                gPlayer.unk_6F--;
            }

            break;

        case 4:

            if (gPlayer.unk_6F == 0) {

                gPlayer.currentCoordinates.x = 0;
                gPlayer.currentCoordinates.y = 0;
                gPlayer.currentCoordinates.z = 0;

                func_8003019C(PLAYER, 1);
                func_80030240(PLAYER, 1);
                func_800302E4(PLAYER, 1);
                func_80030054(PLAYER, 1);
                func_80038990(PLAYER, 0, 0);

                resetAction();

                toggleDailyEventBit(6);

                if (!checkDailyEventBit(0x33)) {
                    gPlayer.fatigue.counter += adjustValue(gPlayer.fatigue.counter, -10, 100);
                }

                setDailyEventBit(0x33);

            } else {
                gPlayer.unk_6F--;
            }

            break;
    }

    func_8002FE10(PLAYER, gPlayer.currentCoordinates.x, 0.0f, gPlayer.currentCoordinates.z, 1.0f);
}

//INCLUDE_ASM(const s32, "player", func_80069C5C);

void func_80069C5C(void) {
    if (func_80031380(0)) {
        setAudio(0xA);
    }
}

//INCLUDE_ASM(const s32, "player", func_80069C90);

void func_80069C90(void) {
    if (func_80031380(0)) {
        setAudio(0xA);
    }
}
 
//INCLUDE_ASM(const s32, "player", func_80069CC4);

void func_80069CC4(void) {

    if (gPlayer.action4 == 0) {
        if (gPlayer.action3 == 0) {
            gPlayer.currentStamina += adjustValue(gPlayer.currentStamina, func_800D5B00(gPlayer.unk_2C), gMaximumStamina);
            gPlayer.fatigue.counter += adjustValue(gPlayer.fatigue.counter, -func_800D5B18(gPlayer.unk_2C), 0x64);
        }
        
        if (gPlayer.action3 == 0x1E) {
            func_800D55E4(gPlayer.unk_2D, 1);
            gPlayer.action4++;
        }
        
        gPlayer.action3++;
    }
}

void func_80069DA8(void) {}

//INCLUDE_ASM(const s32, "player", func_80069DB0);

void func_80069DB0(void) {

    if (gPlayer.action4 == 2) {

        if (!getRandomNumberInRange(0, 400)) {
            gPlayer.action4 = 3;
        }
    }
    
    if (gPlayer.action4 && func_8004D3A4(CONTROLLER_1, BUTTON_B)) {

        switch (gPlayer.action4) {
            
            case 3 ... 5:
                gPlayer.action4 = 6;
                break;
            default:
                gPlayer.action4 = 8;
                break;
        }
    }
}

void func_80069E54(void) {}

void func_80069E5C(void) {}

void func_80069E64(void) {}

void func_80069E6C(void) {}

INCLUDE_ASM(const s32, "player", func_80069E74);

void func_8006A2E0(void) {}

INCLUDE_ASM(const s32, "player", func_8006A2E8);

static inline handleStopHolding() {
    gPlayer.action3 = 0;
    gPlayer.action4 = 0;
    gPlayer.action1 = 0;
    gPlayer.action2 = 0;
    gItemBeingHeld = 0xFF;
}

//INCLUDE_ASM(const s32, "player", func_8006A714);

void func_8006A714(void) {
    
    if (func_8002FECC(0) || gPlayer.action4 == 0) {
        
        if (gPlayer.action4 == 0) {
            setSpriteAnimation(PLAYER, 0x1A5);
            gPlayer.action4++;
            return;
        }
        
       handleStopHolding();
    }
}

//INCLUDE_ASM(const s32, "player", func_8006A7B0);

static inline updateAnimation(u16 animation) {
    
     if (gPlayer.action4 == 0) {
        setSpriteAnimation(PLAYER, animation);
        func_800D55E4(gPlayer.unk_2D, 0xD);
        return;
    }
    
    handleStopHolding();
}

void func_8006A7B0(void) {
    
    if (func_8002FECC(0) || gPlayer.action4 == 0) {
       updateAnimation(0x1A5);
    }
}

//INCLUDE_ASM(const s32, "player", func_8006A848);

void func_8006A848(void) {
    if (func_8002FECC(0) || gPlayer.action4 == 0) {
        if (gPlayer.action4 == 0) {
            setSpriteAnimation(PLAYER, 0x1AD);
            return;
        }
        resetAction();
    }
}

//INCLUDE_ASM(const s32, "player", func_8006A8C4);

void func_8006A8C4(void) {
    
    if (func_8002FECC(0) || gPlayer.action4 == 0) {
        switch (gPlayer.action4) {                       
            case 0:
                setSpriteAnimation(PLAYER, 0x1B7);
                func_800D55E4(gPlayer.unk_2D, 8);
                return;
            case 1:
                setSpriteAnimation(PLAYER, 0x1BF);
                return;
            case 2:
                setSpriteAnimation(PLAYER, 0x1B7);
                return;
            default:
                handleStopHolding();
                break;
            }
    }
}

//INCLUDE_ASM(const s32, "player", func_8006A9A8);

void func_8006A9A8(void) {
    
    if (func_8002FECC(0) || gPlayer.action4 == 0) {
        switch (gPlayer.action4) {       
            case 0:
                setSpriteAnimation(PLAYER, 0x18);
                return;
            case 1:
                func_800D55E4(gPlayer.unk_2D, 0xA);
                gPlayer.action3 = 0;
                gPlayer.action4++;
                return;
            case 2:
                func_8002F2FC(0, 0x1B5);
                return;
            default:
                handleStopHolding();
                break;
            }
    }
}

//INCLUDE_ASM(const s32, "player", func_8006AA9C);

void func_8006AA9C(void) {

    if (func_8002FECC(0) || gPlayer.action4 == 0) {
        
        if (gPlayer.action4 == 0) {
            
            switch (gPlayer.fatigue.level) {       
                case 1:
                    func_8002F2FC(0, 0x1D0);
                    break;
                case 2:
                    func_8002F2FC(0, 0x1D1);
                    break;
                case 3:
                    func_8002F2FC(0, 0x1D2);
                    break;
                case 4:
                    func_8002F2FC(0, 0x1D3);
                    break;
            }
    
            gPlayer.action4++;
            return;
        }

        resetAction();
           
    }
}

//INCLUDE_ASM(const s32, "player", func_8006AB90);

void func_8006AB90(void) {

    if (func_8002FECC(0) || gPlayer.action4 == 0) {
        
        if (gPlayer.action4 == 0) {
            switch (gPlayer.fatigue.unk_2) {                   
                case 1:
                    func_8002F2FC(0, 0x299);
                    break;
                case 2:
                    func_8002F2FC(0, 0x29A);
                    break;
                case 3:
                    break;
                }
            
            gPlayer.action4++;
            return;
        }
        
        resetAction();
    }
}

//INCLUDE_ASM(const s32, "player", func_8006AC4C);

void func_8006AC4C(void) {

    if (func_8002FECC(0) || gPlayer.action4 == 0) {

        switch (gPlayer.action4) {         

            case 1:

                func_8002F2FC(0, 0x1D4);
                gPlayer.action4++;

                if (checkLifeEventBit(MARRIED) && (19 < gHour && gHour < 22)) {
                    switch (gWife) {
                        case MARIA:
                            npcInfoArray[MARIA].movingFlag = 4;
                            break;
                        case POPURI:
                            npcInfoArray[POPURI].movingFlag = 4;
                            break;
                        case ELLI:
                            npcInfoArray[ELLI].movingFlag = 4;
                            break;
                        case ANN:
                            npcInfoArray[ANN].movingFlag = 4;
                            break;
                        case KAREN:
                            npcInfoArray[KAREN].movingFlag = 4;
                            break;
                    }
                }

                break;

            case 2:
                func_8002F2FC(0, 0x1D5);
                gPlayer.action4++;
                break;

            case 3:
                func_8002F2FC(0, 0x1D6);
                gPlayer.action4++;
                break;             

            default:
                if (gPlayer.action4 >= 10) {
                    func_8005C940(8, 0xD);
                    return;
                }
                gPlayer.action4++;
                break;
         }
    }
}

//INCLUDE_ASM(const s32, "player", func_8006ADF4);

// napping/idle animation
void func_8006ADF4(void) {

    if (func_8002FECC(0) || gPlayer.action4 == 0) {

        switch (gPlayer.action4) { 
            case 0:
                func_8002F2FC(0, 0x1E0);
                gPlayer.action4++;
                break;
            case 1:
                func_8002F2FC(0, 0x1E1);
                gPlayer.action4++;
                break;
            case 2:
                // counter until napping animation
                D_802226E0++;
                if (D_802226E0 == 1800) {
                    func_8002F2FC(0, 0x1E2);
                    gPlayer.action4++;
                }
                break;
            case 4:
                func_8002F2FC(0, 0x1E3);
                gPlayer.action4++;
                break;
            default:
                resetAction();
                D_802226E0 = 0;
                break;
            case 3:
                break;
        } 
    }
}

//INCLUDE_ASM(const s32, "player", func_8006AEEC);

void func_8006AEEC(void) {
    
    if (func_8002FECC(0) || gPlayer.action4 == 0) {

        switch (gPlayer.action4) { 
            case 0:
                setSpriteAnimation(PLAYER, 0x1B7);
                break;
            case 1:
                setSpriteAnimation(PLAYER, 0x1BF);
                gPlayer.action4++;
                break;
            case 3:
                setSpriteAnimation(PLAYER, 0x1BF);
                break;            
            case 4:
                setSpriteAnimation(PLAYER, 0x1B7);
                gPlayer.action4++;
                break;           
            default:
                resetAction();
        }
    }
}

//INCLUDE_ASM(const s32, "player", func_8006AFE4);

void func_8006AFE4(void) {
    
    if (func_8002FECC(0) || gPlayer.action4 == 0) {

        switch (gPlayer.action4) {  
            case 0:
                if (!func_800DCAA0(gPlayer.unk_6B)) {
                    setSpriteAnimation(PLAYER, 0);
                    break;
                }
                setSpriteAnimation(PLAYER, 0x1AD);
                break;
            case 1:
                func_800DC750(gPlayer.unk_6B);
                gPlayer.action3 = 0;
                func_80059334();
                break;
            case 2:
                if (func_800DCAA0(gPlayer.unk_6B)) {
                    setSpriteAnimation(PLAYER, 0x1AD);
                }
                break;
            case 3:
                if (func_800DCAA0(gPlayer.unk_6B)) {
                    setSpriteAnimation(PLAYER, 0x1AD);
                }
                break;
            default:
                resetAction();
        }
    }
}

INCLUDE_ASM(const s32, "player", func_8006B104);

void func_8006B4D4(void) {}

INCLUDE_ASM(const s32, "player", func_8006B4DC);

INCLUDE_ASM(const s32, "player", func_8006B584);

INCLUDE_ASM(const s32, "player", func_8006B61C);

INCLUDE_ASM(const s32, "player", func_8006B6EC);

INCLUDE_ASM(const s32, "player", func_8006B77C);

INCLUDE_ASM(const s32, "player", func_8006B8BC);

INCLUDE_ASM(const s32, "player", func_8006B910);

void func_8006B964(void) {}

void func_8006B96C(void) {}

void func_8006B974(void) {}

INCLUDE_ASM(const s32, "player", func_8006B97C);

INCLUDE_ASM(const s32, "player", func_8006BA14);

INCLUDE_ASM(const s32, "player", func_8006BAAC);

INCLUDE_ASM(const s32, "player", func_8006BBC4);

// fishing handler
INCLUDE_ASM(const s32, "player", func_8006BC84);

void func_8006C12C(void) {}

void func_8006C134(void) {}

INCLUDE_ASM(const s32, "player", func_8006C13C);

INCLUDE_ASM(const s32, "player", func_8006C1DC);

INCLUDE_RODATA(const s32, "player", D_8011F5D4);

INCLUDE_ASM(const s32, "player", func_8006C384);

// jtbl_8011F6D8
INCLUDE_ASM(const s32, "player", func_8006C628);


// tool animation

INCLUDE_ASM(const s32, "player", func_8006CD84);

INCLUDE_ASM(const s32, "player", func_8006CDF8);

INCLUDE_ASM(const s32, "player", func_8006CE6C);

INCLUDE_ASM(const s32, "player", func_8006CEE0);

INCLUDE_ASM(const s32, "player", func_8006CF54);

INCLUDE_ASM(const s32, "player", func_8006D0AC);

INCLUDE_ASM(const s32, "player", func_8006D21C);

INCLUDE_ASM(const s32, "player", func_8006D304);

INCLUDE_ASM(const s32, "player", func_8006D3FC);

INCLUDE_ASM(const s32, "player", func_8006D56C);

INCLUDE_ASM(const s32, "player", func_8006D690);

INCLUDE_ASM(const s32, "player", func_8006D7B4);

INCLUDE_ASM(const s32, "player", func_8006D8D8);

INCLUDE_ASM(const s32, "player", func_8006D9FC);

INCLUDE_ASM(const s32, "player", func_8006DB20);

INCLUDE_ASM(const s32, "player", func_8006DC44);

INCLUDE_ASM(const s32, "player", func_8006DD68);

INCLUDE_ASM(const s32, "player", func_8006DE8C);

//INCLUDE_ASM(const s32, "player", func_8006DFB0);

void func_8006DFB0(void) {

    switch (gPlayer.action4) {
        case 0:
            func_8002F2FC(0, 0);
            return;
        case 1:
            func_8002F2FC(0, 0x148);
            break;
        case 2:
            func_800CF850();
            gPlayer.action4++;
            break;
        case 3:            
            if (!D_80189828.unk_E) {
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
//INCLUDE_ASM(const s32, "player", func_8006E0D4);

void func_8006E0D4(void) {

    switch (gPlayer.action4) {
        case 0:
            setSpriteAnimation(PLAYER, 0);
            break;
        case 1:
            setSpriteAnimation(PLAYER, 0x169);
            break;
        case 2:
            func_800CF850();
            gPlayer.action4++;
            break;
        case 3:
            if (!D_80189828.unk_E) {
                if (!func_80067A24(0)) {
                    resetAction();
                }
                D_80237410 += adjustValue(D_80237410, -1, 0x3E7);
                if (!D_80237410) {
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

//INCLUDE_ASM(const s32, "player", func_8006E210);

void func_8006E210(void) {
    gPlayer.action1 = 0x16;
    gPlayer.action3 = 0;
    gPlayer.action4 = 0;
    gPlayer.action2 = 0x17;
}

// empty bottle
//INCLUDE_ASM(const s32, "player", func_8006E240);

void func_8006E240(void) {

    switch (gPlayer.action4) {                           
        case 0:
            setSpriteAnimation(PLAYER, 0);
            func_80099DE8();
            break;
        case 1:
            setSpriteAnimation(PLAYER, 0x234);
            break;
        case 2:
            func_800CF850();
            gPlayer.action4++;
            break;
        case 3:
            if (!D_80189828.unk_E) {
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

INCLUDE_ASM(const s32, "player", func_8006E348);

INCLUDE_ASM(const s32, "player", func_8006E450);

INCLUDE_ASM(const s32, "player", func_8006E574);

// jtbl_8011F6F0
INCLUDE_ASM(const s32, "player", func_8006E678);
