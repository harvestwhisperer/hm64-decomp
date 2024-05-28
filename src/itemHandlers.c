#include "common.h"

#include "itemHandlers.h"

#include "system/controller.h"
#include "system/cutscene.h"
#include "system/map.h"
#include "system/mapContext.h"
#include "system/sprite.h"

#include "animals.h"
#include "game.h"
#include "gameAudio.h"
#include "gameStatus.h"
#include "level.h"
#include "levelInteractions.h"
#include "loadGameScreen.h"
#include "mapObjects.h"
#include "npc.h"
#include "player.h"

// data
extern u16 itemFlags[];
extern u16 D_80118000[];
extern u16 D_801174A0[4][5];
extern u8 D_80117F20[];
extern u8 D_80117180[];
extern u8 D_8011718C[];
extern u16 D_801181C0[];
extern u8 D_80118540[];
extern u8 D_80118620[];

// bss
extern u8 D_8018907D;
extern u8 D_801C3F35;
extern u8 D_801FD624;

// load game screen
extern u32 D_801806C0;
extern u32 D_80188F60;
extern u32 D_801C3F80;
extern u32 D_801FB5D0;
extern u32 D_801FB6FC;
extern u32 D_80237414;

extern ToolUse D_80189828;

// bss
extern ToolStatus D_80204DF8[10];

// forward declarations
void handleBlueFeatherUse();         
void func_800D0360();                                  
void func_800D093C();                                  
void func_800D0DD4();                                  
void func_800D1DB0();                                  
void func_800D304C();                                  
void func_800D3694();                                  
void func_800D36BC();                                  
void func_800D36E4();                                  
void func_800D3714();                                                                   
void func_800D3958();                                  
void func_800D3B74();                                  
void func_800D3D90();                                  
void func_800D3FAC();                                  
void func_800D41C8();                                  
void func_800D43E4();                                  
void func_800D45F4();                                  
void func_800D4814();                                  
void func_800D4A34();                                  
void func_800D4C28();                                  
void func_800D4CA8();                                  
void func_800D4CB0();                                  
void func_800D4CB8();                                  
void func_800D4CC0();                                  
void func_800D4CD0();                                  
void func_800D4D1C();                                  
void func_800D4D68();                                  
void func_800D5130();
void func_800D56E8(u8, u16);   
u8 func_800D67E4(u8);


//INCLUDE_ASM(const s32, "itemHandlers", func_800CF850);

void func_800CF850(void) {
    D_80189828.unk_0 = 0;
    D_80189828.unk_2 = 0;
    D_80189828.unk_E = 2;
}

// jtbl_801222B0
//INCLUDE_ASM(const s32, "itemHandlers", getToolLevel);

u8 getToolLevel(u8 tool) {

    switch (tool) {
        case SICKLE:
            return gPlayer.toolLevels[0];
        case HOE:
            return gPlayer.toolLevels[1];
        case AX:
            return gPlayer.toolLevels[2];
        case HAMMER:
            return gPlayer.toolLevels[3];
        case WATERING_CAN:
            return gPlayer.toolLevels[4];
        default:
            return 0;
    }

}

INCLUDE_ASM(const s32, "itemHandlers", func_800CF8F8);

//INCLUDE_ASM(const s32, "itemHandlers", func_800CFB38);

void func_800CFB38(u8 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {

    func_8002E284(arg0 + 0x27, 0x5D, 1);
    func_8002FF38(arg0 + 0x27, 0);
    func_8002ECD4(arg0 + 0x27, 0xFFFF, 0xFF);
    func_8003019C(arg0 + 0x27, 0);
    func_80030054(arg0 + 0x27, 0);
    func_8002FD80(arg0 + 0x27, arg2, arg3, arg4);

    func_8002F014(arg0 + 0x27, mainMap[0].mapStruct8.groundRgba.r, mainMap[0].mapStruct8.groundRgba.g, mainMap[0].mapStruct8.groundRgba.b, mainMap[0].mapStruct8.groundRgba.a);
    func_8002F2FC(arg0 + 0x27, arg1);
    
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800CFD78);

void func_800CFD78(u8 arg0, u8 arg1) {
    func_8002F2FC(arg0 + 0x27, arg1);
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800CFDA0);

void func_800CFDA0(u8 arg0) {
    func_8002FA2C(arg0 + 0x27);
}

INCLUDE_ASM(const s32, "itemHandlers", func_800CFDC4);

INCLUDE_ASM(const s32, "itemHandlers", func_800CFF1C);

INCLUDE_ASM(const s32, "itemHandlers", func_800D0074);

// jtbl_801222C8
//INCLUDE_ASM(const s32, "itemHandlers", handleToolUse);

void handleToolUse(void) {
    
    switch (gPlayer.currentTool) {
        case 0:
            return;
        case SICKLE:
            func_800D0360();
            break;
        case HOE:
            func_800D093C();
            break;
        case AX:
            func_800D0DD4();
            break;
        case HAMMER:
            func_800D1DB0();
            break;
        case WATERING_CAN:
            func_800D304C();
            break;
        case MILKER:
            func_800D3694();
            break;
        case BELL:
            func_800D36BC();
            break;
        case BRUSH:
            func_800D36E4();
            break;
        case CLIPPERS:
            func_800D3714();
            break;
        case TURNIP_SEEDS:
            func_800D373C();
            break;
        case POTATO_SEEDS:
            func_800D3958();
            break;
        case CABBAGE_SEEDS:
            func_800D3B74();
            break;
        case TOMATO_SEEDS:
            func_800D3D90();
            break;
        case CORN_SEEDS:
            func_800D3FAC();
            break;
        case EGGPLANT_SEEDS:
            func_800D41C8();
            break;
        case STRAWBERRY_SEEDS:
            func_800D43E4();
            break;
        case MOON_DROP_SEEDS:
            func_800D45F4();
            break;
        case PINK_CAT_MINT_SEEDS:
            func_800D4814();
            break;
        case BLUE_MIST_SEEDS:
            func_800D4A34();
            break;
        case BIRD_FEED:
            func_800D4C28();
            break;
        case BALL:
            func_800D4CA8();
            break;
        case FEEDING_BOTTLE:
            func_800D4CB0();
            break;
        case 23:
            func_800D4CB8();
            break;
        case FISHING_POLE:
            func_800D4CC0();
            break;
        case MIRACLE_POTION:
            func_800D4CD0();
            break;
        case COW_MEDICINE:
            func_800D4D1C();
            break;
        case GRASS_SEEDS:
            func_800D4D68();
            break;
        case BLUE_FEATHER:
            handleBlueFeatherUse();
            break;
        case EMPTY_BOTTLE:
            func_800D5130();
            break;
        default:
            return;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D0318);

void func_800D0318(void) {

    if (D_80189828.unk_E) {
        switch (D_80189828.unk_E) {        
            case 2:
                handleToolUse();
            case 3:
                break;
            case 4:
            default:
                break;
        }
    }
}

// alternate for studying
// void func_800D0318(void) {

//     int temp = D_80189828.unk_E; 

//     if (temp) {
//         if (temp != 3) {
//             if (temp < 4) {
//                 if (temp == 2) {
//                     handleToolUse();
//                 }       
//             }
//         }
//     }
// }

INCLUDE_ASM(const s32, "itemHandlers", func_800D0360);

INCLUDE_ASM(const s32, "itemHandlers", func_800D093C);

INCLUDE_ASM(const s32, "itemHandlers", func_800D0DD4);

INCLUDE_ASM(const s32, "itemHandlers", func_800D1DB0);

INCLUDE_ASM(const s32, "itemHandlers", func_800D304C);

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3694);

// milker
void func_800D3694(void) {
    func_8009A53C();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D36BC);

// cow bell
void func_800D36BC(void) {
    func_8009B25C();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D36E4);

// brush
void func_800D36E4(void) {
    func_8009A17C();
    func_8009A2D0();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3714);

// clippers
void func_800D3714(void) {
    func_8009AAC8();
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D373C);

// turnip seeds
void func_800D373C(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          temp3 = 8;
        } else {
          temp3 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3958);

void func_800D3958(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }

    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          temp3 = 0x13;
        } else {
          temp3 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3B74);

// cabbage seeds
void func_800D3B74(void) {
 
    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          temp3 = 0x33;
        } else {
          temp3 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3D90);

// tomato seeds
void func_800D3D90(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gSeason == SUMMER || gBaseMapIndex == GREENHOUSE) {
          temp3 = 0x53;
        } else {
          temp3 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D3FAC);

// corn seeds
void func_800D3FAC(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gSeason == SUMMER || gBaseMapIndex == GREENHOUSE) {
          temp3 = 0x68;
        } else {
          temp3 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D41C8);

// eggplant seeds
void func_800D41C8(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }

    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gSeason == AUTUMN || gBaseMapIndex == GREENHOUSE) {
          temp3 = 0x22;
        } else {
          temp3 = 0xD7;
        }
        
        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D43E4);

// strawberry seeds
void func_800D43E4(void) {
    
    u8 temp1;
    u8 temp2;
    u8 tempFlags;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }

    func_80065F94(&vec, 0.0f, temp2);

    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {

        // leaving for reference
        //tempFlags = ((-(gBaseMapIndex != 0x56) & ~0x28) | 0x44);

        tempFlags = (gBaseMapIndex == 0x56) ? ( 4 | 0x40 ) : ~( 8 | 0x20 );

        func_800DAC70(gBaseMapIndex, tempFlags, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }

    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }

    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D45F4);

// moon drop seeds
void func_800D45F4(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        
        // FIXME: should be range
        if ((gSeason - 1 < 2U) || gBaseMapIndex == GREENHOUSE) {
            temp3 = 0x90; 
        } else {
            temp3 = 0xD7;
        }

        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4814);

void func_800D4814(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {

        // FIXME: should be range
        if ((gSeason - 1 < 2U) || gBaseMapIndex == GREENHOUSE) {
            temp3 = 0x9F; 
        } else {
            temp3 = 0xD7;
        }

        func_800DAC70(gBaseMapIndex, temp3, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2++;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4A34);

void func_800D4A34(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        func_800DAC70(gBaseMapIndex, 0xB2, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4C28);

// chicken feed
void func_800D4C28(void) {
    
    u8 chicken;
    
    chicken = func_800ACE50(gBaseMapIndex);
  
    if (chicken != 0xFF) {
        func_80073244(chicken - 0x13);
        gChickens[chicken - 0x13].flags |= 0x10;
    }
    
    D_80189828.unk_E = 0;  
}

// ball
void func_800D4CA8(void) {}

// feeding bottle
void func_800D4CB0(void) {}

// unknown tool
void func_800D4CB8(void) {}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4CC0);

// fishing pole
void func_800D4CC0(void) {
    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4CD0);

// miracle potion
void func_800D4CD0(void) {

    if (func_8009A810()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4D1C);

// cow medicine
void func_800D4D1C(void) {

    if (func_8009A400()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D4D68);

// grass seeds
void func_800D4D68(void) {

    u8 temp1;
    u8 temp2;
    u8 temp3;

    Vec3f vec;

    temp1 = (renderedSprites[PLAYER].direction + func_8003C1A4(MAIN_MAP_INDEX)) % 8;
    
    if (temp1 < 5) {
        temp2 = D_80117180[D_80189828.unk_2];
    }
    else {
        temp2 = D_8011718C[D_80189828.unk_2];
    }
    
    func_80065F94(&vec, 0.0f, temp2);
 
    if ((func_800DA948(func_800DAF58(0.0f, temp2)) & 0x20) && vec.y != MAX_UNSIGNED_SHORT) {
        if (gBaseMapIndex == FARM) {
            func_800DAC70(FARM, 0x85, (u8)vec.x - D_801FD624, (u8)vec.z - D_801C3F35);    
        }
    }
    
    if (!D_80189828.unk_2) {
        setAudio(0x1D);
    }
    
    D_80189828.unk_2 += 1;
    
    if (D_80189828.unk_2 == 9) {
        D_80189828.unk_E = 0;
    }
}

INCLUDE_RODATA(const s32, "itemHandlers", D_80122340);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122344);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122348);

INCLUDE_RODATA(const s32, "itemHandlers", D_8012234C);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122350);

INCLUDE_RODATA(const s32, "itemHandlers", D_8012235C);

INCLUDE_RODATA(const s32, "itemHandlers", D_80122368);

//INCLUDE_RODATA(const s32, "itemHandlers", D_80122374);

const u8 D_80122374[12] = { 5, 4, 3, 6, 8, 2, 7, 0, 1, 0, 0, 0 };
 
//INCLUDE_ASM(const s32, "itemHandlers", handleBlueFeatherUse);

// jumtable: 80122380, 0xFD780
void handleBlueFeatherUse(void) {
     
    toggleDailyEventBit(SUCCESSFUL_PROPOSAL);
    
    if (D_801C3E18 == 0xFF) goto func_end;

    func_80085D48(D_801C3E18, 0);
    
    npcInfoArray[D_801C3E18].movingFlag = npcInfoArray[D_801C3E18].unk_1E;
    
    setSpecialDialogueBit(0x135);
    
    switch (D_801C3E18) {
        case 0:
            if (npcAffection[MARIA] >= 220) {
                gWife = MARIA;   
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(MARIA);
            }
            break;
        case 1:
            if (npcAffection[POPURI] >= 220) {
                gWife = POPURI;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(POPURI);
            }
            break;
        case 2:
            if (npcAffection[ELLI] >= 220) {
                gWife = ELLI;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ELLI);
            }
            break;
        case 3:
            if (npcAffection[ANN] >= 220) {
                gWife = ANN;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ANN);
            }
            break;
        case 4:
            if (npcAffection[KAREN] >= 220) {
                gWife = KAREN;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(KAREN);
            }
            break;
        default:
            break;
        }

    if (checkDailyEventBit(SUCCESSFUL_PROPOSAL)) {
        // use up blue feather if successful proposal
        gPlayer.currentTool = 0;
    }
    
func_end:
        D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5130);

// empty bottle
void func_800D5130(void) {

    if (func_800ACFE8(gBaseMapIndex)) {
        gPlayer.bottleContents = 1;
    } else if (checkWineBarrelInteraction(gBaseMapIndex) ) {
        gPlayer.bottleContents = 2;
    } else {
        func_8009A97C();
    }

    D_80189828.unk_E = 0;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D51B0);

void func_800D51B0(void) {
    u8 i;

    for (i = 0; i < 0xA; i++) {
        D_80204DF8[i].unk_0.x = 0;
        D_80204DF8[i].unk_0.y = 0;
        D_80204DF8[i].unk_0.z = 0;
        D_80204DF8[i].unk_C = 0;
        D_80204DF8[i].unk_10 = 0;
        D_80204DF8[i].unk_14 = 0;
        D_80204DF8[i].unk_18.x = 0;
        D_80204DF8[i].unk_18.y = 0;
        D_80204DF8[i].unk_18.z = 0;
        D_80204DF8[i].unk_24 = 0;
        D_80204DF8[i].unk_26 = 0;
        D_80204DF8[i].unk_28 = 0;
        D_80204DF8[i].unk_2A = 0;
        D_80204DF8[i].flags = 0;
    }
}


//INCLUDE_ASM(const s32, "itemHandlers", func_800D5290);

void func_800D5290(void) {

    u8 i;
    
    for (i = 0; i < 10; i++) {
        if (D_80204DF8[i].flags & 1) {
            func_800D56E8(i, D_80204DF8[i].unk_28);
        }    
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5308);

// tool acquisition handler
u8 func_800D5308(u8 index, u8 arg1, u32 arg2, s32 arg3, s32 arg4) {

    D_80204DF8[index].unk_2A = arg1;
    D_80204DF8[index].unk_28 = arg2;
    D_80204DF8[index].unk_26 = 0;
    D_80204DF8[index].flags = arg3 | (arg4 | 1);
    
    gItemBeingHeld = func_800D67E4(arg2);
    
    return index;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5390);

u8 func_800D5390(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4) {
    
    u8 found = 0;
    int tempBit = 1;
    
    while (index < 10 && !found) {
        if (!(D_80204DF8[index].flags & 1)) {
          found = 1;
        }
        else {
          index++;
        }
    }
    
    if (found) {
        D_80204DF8[index].unk_2A = arg1;
        D_80204DF8[index].unk_28 = arg2;
        D_80204DF8[index].flags = arg3 | (arg4 | tempBit);
        D_80204DF8[index].unk_26 = 0;
        gItemBeingHeld = func_800D67E4(arg2);
    }
      
    else {
        index = 0xff;
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5488);

u8 func_800D5488(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4) {
    
    u8 found = 0;
    
    while (index < 10 && !found) {
        if (!(D_80204DF8[index].flags & 1)) {
          found = 1;
        }
        else {
          index++;
        }
    }

    if (found) {
        D_80204DF8[index].unk_2A = arg1;
        D_80204DF8[index].unk_28 = arg2;
        D_80204DF8[index].flags = arg3 | (arg4 | 1);
        D_80204DF8[index].unk_26 = 0;
    }
    else {
        index = 0xff;
    }
    
    return index;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5548);

void func_800D5548(u8 index) {

    if (D_80204DF8[index].flags & 1) {        
        func_8002FA2C(index + 0x27);
        D_80204DF8[index].unk_2A = 0;
        D_80204DF8[index].flags = 0;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D55B8);

u16 func_800D55B8(u8 index) {
    return D_80204DF8[index].unk_28;
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D55E4);

void func_800D55E4(u8 index, u8 arg1) {

    if (D_80204DF8[index].flags & 1) {
        D_80204DF8[index].unk_2A = arg1;
    }
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5628);

void func_800D5628(u8 index, f32 arg1, f32 arg2, f32 arg3) {

    D_80204DF8[index].unk_0.x = arg1;
    D_80204DF8[index].unk_0.y = arg2;
    D_80204DF8[index].unk_0.z = arg3;
    D_80204DF8[index].flags |= 2;

}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5688);

void func_800D5688(u8 index, f32 arg1, f32 arg2, f32 arg3) {

    D_80204DF8[index].unk_18.x = arg1;
    D_80204DF8[index].unk_18.y = arg2;
    D_80204DF8[index].unk_18.z = arg3;
    D_80204DF8[index].flags |= 8;
     
}

INCLUDE_ASM(const s32, "itemHandlers", func_800D56E8);

u16 func_800D5A6C(u16 index) {
    return itemFlags[index];
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5A88);

u16 func_800D5A88(u16 index) {

  u16 result;
  
  result = D_801174A0[index][gSeason];

  if (185 < index && index < 194) {
    result = 249;
  }

  if (193 < index && index < 202) {
    result = 250;
  }

  if (170 < index && index < 179) {
    result = 251;
  }

  return result;

}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5B00);

u8 func_800D5B00(u16 index) {
    return D_80118540[index];
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D5B18);

u8 func_800D5B18(u16 index) {
    return D_80118620[index];
}

INCLUDE_ASM(const s32, "itemHandlers", func_800D5B30);

INCLUDE_ASM(const s32, "itemHandlers", func_800D5CC0);

//INCLUDE_ASM(const s32, "itemHandlers", func_800D67E4);

u8 func_800D67E4(u8 index) {
    return D_80117F20[index];
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D67FC);

void func_800D67FC(u8 index) {

    u16 temp;

    switch (index) {
        case 0x60 ... 0x6F:
            temp =  gPlayer.unk_6C + 0xF5;
            break;
        default:
            // array of npc/dialogue-related u16s
            temp = D_80118000[index];
            break;
    }

    showTextBox(1, 6, temp, 0, 2);
}

// alternate for study
// void func_800D67FC(u8 index) {
    
//     s32 temp;
//     u16 temp2;

//     temp = index;
    
//     if (temp < 0x70) {
//         if (temp > 0x5F) {
//             temp2 = gPlayer.unk_6C + 0xF5;
//             goto func_end;
//         } 
//     } 

// 
//     temp2 = D_80118000[index];
    
// func_end:
//     showTextBox(1, 6, temp2, 0, 2);
// }

// jumptable: 801226D8, 0xFDAD8
//INCLUDE_ASM(const s32, "itemHandlers", func_800D6868);

void func_800D6868(u8 index) {

    u32 result;
    
    dailyShippingBinValue += adjustValue(dailyShippingBinValue, D_801181C0[index], MAX_TOTAL_SHIPPING);
     
    switch (index) {
        case 13:
            D_801654F4 += adjustValue(D_801654F4, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 14:
            D_80237414 += adjustValue(D_80237414, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 18:
            D_801FB6FC += adjustValue(D_801FB6FC, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 15:
            D_801C3F80 += adjustValue(D_801C3F80, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 19:
            D_801806C0 += adjustValue(D_801806C0, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 16:
            D_80188F60 += adjustValue(D_80188F60, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 17:
            D_801FB5D0 += adjustValue(D_801FB5D0, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped,1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 21:
        case 22:
        case 23:
        case 24:
            gTotalGoldenMilkShipped += adjustValue(gTotalGoldenMilkShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            break;
        case 20:
            gTotalEggsShipped += adjustValue(gTotalEggsShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            break;
            
    }

    func_8009A398();
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D6B28);

void func_800D6B28(void) {
    func_80034DC8(MAIN_MAP_INDEX, 0, 0x13);
    // put food in dog bowl
    setDailyEventBit(0x43);
}

//INCLUDE_ASM(const s32, "itemHandlers", func_800D6B58);

static inline void setVec3(u8 index, float x, float y, float z) {
    D_80204DF8[index].unk_0.x = x;
    D_80204DF8[index].unk_0.y = y;
    D_80204DF8[index].unk_0.z = z;
    D_80204DF8[index].flags |= 2;
}

void func_800D6B58(u8 arg0, u8 index) {

    u8 temp;
    u8 temp2;
    u8 flags;
    
    D_80204DF8[index].unk_18.x = 0;
    D_80204DF8[index].unk_18.y = 0;
    D_80204DF8[index].unk_18.z = 0;
    D_80204DF8[index].flags |= 8;

    func_8002FE10(index + 0x27, 0.0f, 0.0f, 0.0f, 0.0f);
    func_8002ECD4(index + 0x27, 0xFFFF, 0xFF);
    
    func_8003019C(index + 0x27, 0);
    func_80030054(index + 0x27, 0);
    func_80030240(index + 0x27, 0);
    func_800302E4(index + 0x27, 0);

    if (gBaseMapIndex == FARM) {
        
        temp = func_800309B4(0, 0.0f, 32.0f);
        temp2 = temp - 0x1B;
        
        if (temp2 < 2 || temp == 0x1D) {
            setVec3(index, 288.0f, 80.0f, -404.0f);
            func_8002F2FC(index + 0x27, 0xE9);
        } 
    }
    
    if (gBaseMapIndex == MOUNTAIN_1) {
        
        temp = func_800309B4(0, 0.0f, 40.0f);
        
        if (temp == 0x10 || temp == 0x14) {
            setVec3(index, 160.0f, 64.0f, -128.0f);
            func_8002F2FC(index + 0x27, 0xE9);
        }
        
        if (!(gCutsceneFlags & 1)) {
            if (!checkLifeEventBit(0x5E) && !checkDailyEventBit(0x21) && (D_80204DF8[index].unk_28 - 0x25) < 2U && 8 < gHour && gHour < 17) {
                setDailyEventBit(0x47);
            }
            if (!checkLifeEventBit(0x5F) && !checkDailyEventBit(0x21) && D_80204DF8[index].unk_28 ==  0x27 && 8 < gHour && gHour < 17) {
                setDailyEventBit(0x48);
            }
        }
    }

    if (gBaseMapIndex == POND) {
        
        temp = func_800309B4(0, 0.0f, 32.0f);
        
        if (temp == 0x10) {
            setVec3(index, -16.0f, 80.0f, -224.0f);
            func_8002F2FC(index + 0x27, 0xE9);
        }
        
        if (!(gCutsceneFlags & 1)) {
            if (!checkDailyEventBit(0x46) && itemFlags[D_80204DF8[index].unk_28] & 0x8000 && 8 < gHour && gHour < 17 ) {
                setDailyEventBit(0x45);    
            }
        }
    }
}

// jtbl_80122708
INCLUDE_ASM(const s32, "itemHandlers", func_800D7010);
