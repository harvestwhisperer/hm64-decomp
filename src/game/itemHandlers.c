#include "common.h"

#include "game/itemHandlers.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/levelInteractions.h"
#include "game/loadGameScreen.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/setCutscenes.h"
#include "game/spriteInfo.h"


// bss
extern u8 D_8018907D;
extern ToolUse toolUse;
extern ItemContext itemInfo[10];

// data
extern u16 itemFlags[];
// entity animation indices
extern u16 D_801171A0[192][4];
extern u16 D_801174A0[4][5];
extern u16 D_80117D60[];
extern u8 D_80117F20[];
extern u8 D_80117180[];
extern u8 D_8011718C[];
extern u16 D_80118000[];
extern u16 D_801181C0[];
extern u8 D_80118540[];
extern u8 D_80118620[];

// rodata
static const u8 D_80122340[];
static const u8 D_80122344[];
static const u8 D_80122348[];
static const u8 D_8012234C[];
static const u8 D_80122350[];
static const u8 D_8012235C[];
static const u8 D_80122368[];
static const u8 D_80122374[];

// load game screen
// items shippd
// strawberries shipped
extern u32 D_801806C0;
extern u32 D_80188F60;
extern u32 D_801C3F80;
extern u32 D_801FB5D0;
extern u32 D_801FB6FC;
extern u32 D_80237414;


// forward declarations
bool func_800CFDC4(u8 arg0, s16 arg1, s16 arg2);
bool func_800CFF1C(u8 arg0, s16 arg1, s16 arg2);          
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
void func_800D373C(void);  
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
u8 func_800D5488(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4);
void func_800D5628(u8 index, f32 arg1, f32 arg2, f32 arg3);
void func_800D56E8(u8, u16);   
bool func_800D5CC0(u8 itemIndex);
void func_800D6868(u8 index);
void func_800D6B58(u8 arg0, u8 index);
u8 func_800D67E4(u8);
u16 getItemFlags(u16 index);


//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CF850);

void func_800CF850(void) {
    toolUse.unk_0 = 0;
    toolUse.entityIndex = 0;
    toolUse.unk_E = 2;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", getToolLevel);

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

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CF8F8);

void func_800CF8F8(u8 entityIndex, u8 animationIndex, f32 x, f32 y, f32 z) {

    loadEntity(entityIndex + ITEMS_OFFSET, 0x60, 1);
    func_8002FF38(entityIndex + ITEMS_OFFSET, FALSE);
    setEntityTrackingTarget(entityIndex + ITEMS_OFFSET, 0xFFFF, 0xFF);
    setEntityCollidable(entityIndex + ITEMS_OFFSET, FALSE);
    setEntityYMovement(entityIndex + ITEMS_OFFSET, FALSE);
    setEntityCoordinates(entityIndex + ITEMS_OFFSET, x, y, z);
    setEntityColor(entityIndex + ITEMS_OFFSET, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAnimation(entityIndex + ITEMS_OFFSET, animationIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CFB38);

// only used with fishing rod
void func_800CFB38(u8 entityIndex, u8 animationIndex, f32 x, f32 y, f32 z) {

    loadEntity(entityIndex + ITEMS_OFFSET, 0x5D, 1);
    func_8002FF38(entityIndex + ITEMS_OFFSET, FALSE);
    setEntityTrackingTarget(entityIndex + ITEMS_OFFSET, 0xFFFF, 0xFF);
    setEntityCollidable(entityIndex + ITEMS_OFFSET, FALSE);
    setEntityYMovement(entityIndex + ITEMS_OFFSET, FALSE);
    setEntityCoordinates(entityIndex + ITEMS_OFFSET, x, y, z);
    setEntityColor(entityIndex + ITEMS_OFFSET, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAnimation(entityIndex + ITEMS_OFFSET, animationIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CFD78);

void func_800CFD78(u8 entityIndex, u8 animationIndex) {
    setEntityAnimation(entityIndex + ITEMS_OFFSET, animationIndex);
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CFDA0);

void func_800CFDA0(u8 entityIndex) {
    deactivateEntity(entityIndex + ITEMS_OFFSET);
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CFDC4);

bool func_800CFDC4(u8 arg0, s16 arg1, s16 arg2) {

    bool result = FALSE;

    s16 temp;
    s16 temp2;
    
    switch (arg0) {

        case 0xC8:
            temp = arg1;
            temp2 = arg2;
            break;
        case 0xC9:
            temp = arg1 - 1;
            temp2 = arg2;
            break;
        case 0xCA:
            temp = arg1;
            temp2 = arg2 - 1;
            break;
        case 0xCB:
            temp = arg1 - 1;
            temp2 = arg2 - 1;
            break;
 
    }

    if (toolUse.unk_4 != temp || toolUse.unk_6 != temp2) {
        toolUse.unk_4 = temp;
        toolUse.unk_6 = temp2;
        toolUse.unk_3 = 0;
    }

    switch (gPlayer.staminaLevelForCurrentToolUse) {

        case 0:
            toolUse.unk_3 += 1;
            break;
        
        case 1:
            toolUse.unk_3 += 3;
            break;
        
        case 2:
            toolUse.unk_3 += 6;
            break;
        
    }
    
    if (toolUse.unk_3 >= 6) {

        toolUse.unk_3 = 0;
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800CFF1C);

bool func_800CFF1C(u8 arg0, s16 arg1, s16 arg2) {
    
    bool result = FALSE;

    s16 temp;
    s16 temp2;
    
    switch (arg0) {

        case 0xC4:
            temp = arg1;
            temp2 = arg2;
            break;
        case 0xC5:
            temp = arg1 - 1;
            temp2 = arg2;
            break;
        case 0xC6:
            temp = arg1;
            temp2 = arg2 - 1;
            break;
        case 0xC7:
            temp = arg1 - 1;
            temp2 = arg2 - 1;
            break;
 
    }

    if (toolUse.unk_A != temp || toolUse.unk_C != temp2) {
        toolUse.unk_A = temp;
        toolUse.unk_C = temp2;
        toolUse.unk_8 = 0;
    }

    switch (gPlayer.staminaLevelForCurrentToolUse) {

        case 0:
            toolUse.unk_8 += 1;
            break;
        
        case 1:
            toolUse.unk_8 += 3;
            break;
        
        case 2:
            toolUse.unk_8 += 6;
            break;
        
    }
    
    if (toolUse.unk_8 >= 6) {

        toolUse.unk_8 = 0;
        result = TRUE;

    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D0074);

inline u8 func_800D0074(s16 arg0, s16 arg1) {
    
    if (gBaseMapIndex == FARM) {
        
        setDailyEventBit(0x29); 
        
        if (arg0 == 12 && arg1 == 2) {
            toolUse.unk_F += 1;
        } else {
            toolUse.unk_F = 0;    
        }
        
    } else {
        toolUse.unk_F = 0;
    }
    
    return toolUse.unk_F;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleToolUse);

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
        case CHICKEN_FEED:
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

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D0318);

void func_800D0318(void) {

    if (toolUse.unk_E) {

        switch (toolUse.unk_E) {        
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

// alternate for reference
// void func_800D0318(void) {

//     int temp = toolUse.unk_E; 

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

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D0360);

// sickle
void func_800D0360(void) {

    Vec3f vec;
    
    f32 tempF;
    u8 temp1, temp2;
    u8 temp3, temp4;
    s32 tempX, tempZ;
    s32 direction;
    u8 currentGroundObjectIndex;
    u8 groundObjectIndex;

    u8 arr1[3];
    u8 arr2[3];
    u8 arr3[3];
    u8 arr4[3];
    u8 arr5[12];
    u8 arr6[8];
    
    u32 padding[2];

    memcpy(arr1, D_80122340, 3);
    memcpy(arr2, D_80122344, 3);
    memcpy(arr3, D_80122348, 3);
    memcpy(arr4, D_8012234C, 3);
    memcpy(arr5, D_80122350, 9);
    memcpy(arr6, D_8012235C, 9);

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);

    switch (gPlayer.staminaLevelForCurrentToolUse) {

        case 0:
            temp1 = 1;
            temp2 = 8;
            break;
        
        case 1:
            
            temp1 = 1;
            
            if (direction & 1) {
                
                if (entities[PLAYER].direction < 5) {
                    temp2 = arr3[toolUse.entityIndex];
                } else {
                    temp2 = arr4[toolUse.entityIndex]; 
                }
                
            } else if (entities[PLAYER].direction < 5) {
                temp2 = arr1[toolUse.entityIndex];
            } else {
                temp2 = arr2[toolUse.entityIndex];
            }
            
            break;
        
        case 2:
            
            temp1 = 0;

            if ((u8)direction < 5) {
                temp2 = arr5[toolUse.entityIndex];
            } else {
                temp2 = arr6[toolUse.entityIndex];
            }
            
            break;
        
    }

    tempF = (f32)temp1;
            
    vec = func_80065F94(tempF, temp2);
    currentGroundObjectIndex = getGroundObjectIndexFromPlayerPosition(tempF, temp2);
    
    if ((func_800DA948(currentGroundObjectIndex) & 1) && vec.y != 65535.0f && (gBaseMapIndex == GREENHOUSE || gSeason != WINTER)) {

        if (currentGroundObjectIndex == 0x8E) {
            
            groundObjectIndex = 0x8F;
            fodderQuantity += adjustValue(fodderQuantity, 1, MAX_FODDER);
            temp4 = 62;
            
        } else {

            switch (getGroundObjectMapAdditionIndex(currentGroundObjectIndex)) {

                case 4:
                    groundObjectIndex = 2;
                    break;
                case 5:
                    groundObjectIndex = 3;
                    break;
                default:
                    groundObjectIndex = 1;
                    break;
                
            }         

            temp4 = 1;
            
        }

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
           
        temp3 = func_800D5488(1, 12, temp4, 0, 8);

        vec = func_8003AF58(0, (u8)vec.x, (u8)vec.z);
        func_800D5628(temp3, vec.x, vec.y, vec.z);

    }

    if (toolUse.entityIndex == 0) {

        switch (gPlayer.staminaLevelForCurrentToolUse) {

            case 0:
                setAudio(0xB);
                break;
            case 1:
                setAudio(0xC);
                break;
            case 2:
                setAudio(0xD);
                break;
            
        }
        
    }

    toolUse.entityIndex++;
    
    switch (gPlayer.staminaLevelForCurrentToolUse) {
            
        case 0:
            toolUse.unk_E = 0;
            break;
        
        case 1:

            if (toolUse.entityIndex == 3) {
                toolUse.unk_E = 0;
            }
    
            break;
        
        case 2:

            if (toolUse.entityIndex == 9) {
                toolUse.unk_E = 0;
            }
            
            break;
            
    }

    // hit animals
    func_8009AC54();
    func_8009AE7C();
    func_8009AFB4();
    func_8009B068();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D093C);

// hoe
void func_800D093C(void) {

    Vec3f vec;
    f32 tempF;
    u8 temp;
    s32 tempX, tempZ;
    
    u8 direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);

    switch (gPlayer.staminaLevelForCurrentToolUse) {

        case 0:
            temp = 1;
            break;
        case 1:
            temp = toolUse.entityIndex + 1;
            break;
        case 2:
            temp = toolUse.entityIndex + 1;
            break;
        
    }

    tempF = temp;

    vec = func_80065F94(tempF, 8);

    if (vec.y != 65535.0f) {
        
        if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(tempF, 8)) & 2) && (gBaseMapIndex == GREENHOUSE || gBaseMapIndex == MINE || gBaseMapIndex == MINE_2 || gSeason != WINTER)) {

            addGroundObjectToMap(gBaseMapIndex, TILLED, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
            
            vec = func_8003AF58(0, (u8)vec.x, (u8)vec.z);
            
            func_800CF8F8(toolUse.entityIndex, 0, vec.x, vec.y, vec.z);

            if (toolUse.entityIndex == 0) {

                tempX = (s16)vec.x / 32;
                tempZ = (s16)vec.z / 32;

                func_800D0074(tempX, tempZ);
                setAudio(0xE);
                
            } else {
                setAudio(0xE);
            }
            
        } else {
            toolUse.unk_E = 0;
            setAudio(0x1B);
        }

        toolUse.entityIndex++;

        switch (gPlayer.staminaLevelForCurrentToolUse) {
            
            case 0:
                toolUse.unk_E = 0;
                break;
            
            case 1:

                if (!checkDailyEventBit(0x4A)) {

                    if (direction & 1) {

                        if (toolUse.entityIndex == 2) {
                            toolUse.unk_E = 0;
                        }
                        
                    } else {
                        if (toolUse.entityIndex == 3) {
                            toolUse.unk_E = 0;
                        }
                    }
                    
                } else {
                    toolUse.unk_E = 0;
                }

                break;
            
            case 2:
                
                if (!checkDailyEventBit(0x4A)) {

                    if (direction & 1) {

                        if (toolUse.entityIndex == 4) {
                            toolUse.unk_E = 0;
                        }
                        
                    } else {
                        if (toolUse.entityIndex == 6) {
                            toolUse.unk_E = 0;
                        }
                    }
                    
                } else {
                    toolUse.unk_E = 0;
                }

                break;
            
        }

    } else {
        toolUse.unk_E = 0;
        setAudio(0x1B);
    }

    // hit animals
    func_8009AC54();
    func_8009AE7C();
    func_8009AFB4();
    func_8009B068();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D0DD4);

// ax
void func_800D0DD4(void) {
    
    Vec3f vec;
    Vec3f vec2;
    u8 groundObjectIndex;

    vec = func_80065F94(1.0f, 8);

    if (vec.y != 65535.0f) {

        groundObjectIndex =getGroundObjectIndexFromPlayerPosition(1.0f, 8);

        if (func_800DA948(groundObjectIndex) & 4) {

            if (func_800CFDC4(groundObjectIndex, (s16)vec.x / 32, (s16)vec.z / 32)) {

                vec2.y = 0.0f;

                switch (groundObjectIndex) {

                    case 0xC8:
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ + 1);

                        vec2.x = 16.0f;
                        vec2.z = 16.0f;

                        break;

                    case 0xC9:
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ + 1);

                        vec2.x = -16.0f;
                        vec2.z = 16.0f;
                        
                        break;
                    
                    case 0xCA:
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ - 1);

                        vec2.x = 16.0f;
                        vec2.z = -16.0f;

                        
                        break;
                    
                    case 0xCB:
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                        addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ - 1);
    
                        vec2.x = -16.0f;
                        vec2.z = -16.0f;
    
                        break;
                    
                }

                groundObjectIndex = func_800D5488(1, 0xC, 0x3D, 0, 8);

                vec = func_8003AF58(MAIN_MAP_INDEX, (u8)vec.x, (u8)vec.z);
                vec.x += vec2.x;
                vec.z += vec2.z;
                
                func_800D5628(groundObjectIndex, vec.x, vec.y, vec.z);
                gLumber += adjustValue(gLumber, 6, MAX_LUMBER);
                
            }
            
        } else {
            toolUse.unk_E = 0;
        }

        if (toolUse.entityIndex == 0) {

            switch (gPlayer.staminaLevelForCurrentToolUse) {

                case 0:
                    setAudio(0x11);
                    break;
                case 1:
                    setAudio(0x12);
                    break;
                case 2:
                    setAudio(0x13);
                    break;
                
            }
            
        }

        toolUse.entityIndex++;

        switch (gPlayer.staminaLevelForCurrentToolUse) {
        
            case 0:
                toolUse.unk_E = 0;
                break;
            
            case 1:
                toolUse.unk_E = 0;
                break;
            
            case 2:
                toolUse.unk_E = 0;
                break;

        }
        
    } else {
        toolUse.unk_E = 0;
    }

    func_8009AC54();
    func_8009AE7C();
    func_8009AFB4();
    func_8009B068();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D1DB0);

// hammer
void func_800D1DB0(void) {

    Vec3f vec;
    Vec3f vec2;
    u8 temp;
    s16 tempX, tempZ;
    u8 groundObjectIndex;

    vec = func_80065F94(1.0f, 8);
    
    if (vec.y != 65535.0f) {
        
        groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8U);
    
        if (func_800DA948(groundObjectIndex) & 8) {

            vec2.y = 0.0f;

            switch (groundObjectIndex) {

                case SMALL_ROCK:

                    addGroundObjectToMap(gBaseMapIndex, UNTILLED, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                    
                    vec2.x = 0.0f;
                    vec2.z = 0.0f;
                    
                    toolUse.unk_8 = 0;
                    
                    groundObjectIndex = func_800D5488(1, 12, 2, 0, 8);
                    
                    vec = func_8003AF58(MAIN_MAP_INDEX, (u8)vec.x, (u8)vec.z);
                    vec.x += vec2.x;
                    vec.z += vec2.z;
                
                    func_800D5628(groundObjectIndex, vec.x, vec.y, vec.z);
                    break;

                case TILLED:
                case TILLED_WATERED:
                case BROKEN_LOG:
                    addGroundObjectToMap(gBaseMapIndex, UNTILLED, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                    break;
                
                case BOULDER ... 0xC7:
                    
                    if (func_800CFF1C(groundObjectIndex, (s16)vec.x / 32, (s16)vec.z / 32)) {

                        switch (groundObjectIndex) {

                            case BOULDER:
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ + 1);

                                vec2.x = 16.0f;
                                vec2.z = 16.0f;

                                temp = 60;
                                
                                break;

                            case 0xC5:
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ + 1);

                                vec2.x = -16.0f;
                                vec2.z = 16.0f;

                                temp = 60;
                                
                                break;

                            case 0xC6:
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ - 1);

                                vec2.x = 16.0f;
                                vec2.z = -16.0f;

                                temp = 60;
                                
                                break;
                        
                            case 0xC7:
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                                addGroundObjectToMap(gBaseMapIndex, 1, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ - 1);

                                vec2.x = -16.0f;
                                vec2.z = -16.0f;

                                temp = 60;
                                
                                break;
                        }

                        groundObjectIndex = func_800D5488(1, 12, temp, 0, 8);

                        vec = func_8003AF58(MAIN_MAP_INDEX, (u8)vec.x, (u8)vec.z);
                        vec.x += vec2.x;
                        vec.z += vec2.z;
                    
                        func_800D5628(groundObjectIndex, vec.x, vec.y, vec.z);
                        break;
                    
                    }
                    
                    break;
                
            }
            
        } else {
            toolUse.unk_E = 0;
        }

        if (toolUse.entityIndex == 0) {

            switch (gPlayer.staminaLevelForCurrentToolUse) {

                case 0:
                    setAudio(0x14);
                    break;
                case 1:
                    setAudio(0x15);
                    break;
                case 2:
                    setAudio(0x16);
                    break;
                
            }
            
        }

        toolUse.entityIndex++;

        switch (gPlayer.staminaLevelForCurrentToolUse) {
        
            case 0:
                toolUse.unk_E = 0;
                break;
            
            case 1:
                toolUse.unk_E = 0;
                break;
            
            case 2:
                toolUse.unk_E = 0;
                break;

        }
    
    } else {
        toolUse.unk_E = 0;
    }
 
    if (gBaseMapIndex == MOUNTAIN_1 && (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0, 32.0f) == 0x11) && (gPlayer.staminaLevelForCurrentToolUse == 2)) {
        deactivateMapObject(MAIN_MAP_INDEX, 3);
        setDailyEventBit(0x29);
    }
    
    func_8009AC54();
    func_8009AE7C();
    func_8009AFB4();
    func_8009B068();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D304C);

// watering can
void func_800D304C(void) {
    
    Vec3f vec;
    
    f32 tempF;
    u8 temp1, temp2;
    s32 tempX, tempZ;
    s32 direction;
    u8 currentGroundObjectIndex;

    u8 arr1[3];
    u8 arr2[3];
    u8 arr3[3];
    u8 arr4[3];
    u8 arr5[12];
    u8 arr6[8];
    
    u32 padding[2];

    memcpy(arr1, D_80122340, 3);
    memcpy(arr2, D_80122344, 3);
    memcpy(arr3, D_80122348, 3);
    memcpy(arr4, D_8012234C, 3);
    memcpy(arr5, D_80122368, 9);
    memcpy(arr6, D_80122374, 9);
    
    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);

    switch (gPlayer.staminaLevelForCurrentToolUse) {

        case 0:
            temp1 = 1;
            temp2 = 8;
            break;
        
        case 1:
            
            temp1 = 1;
            
            if (direction & 1) {
                
                if (entities[PLAYER].direction < 5) {
                    temp2 = arr3[toolUse.entityIndex];
                } else {
                    temp2 = arr4[toolUse.entityIndex]; 
                }

            } else if (entities[PLAYER].direction < 5) {
                temp2 = arr1[toolUse.entityIndex];
            } else {
                temp2 = arr2[toolUse.entityIndex];
            }
            
            break;
        
        case 2:
            
            temp1 = 2;

            if ((u8)direction < 5) {
                temp2 = arr5[toolUse.entityIndex];
            } else {
                temp2 = arr6[toolUse.entityIndex];
            }
            
            break;
        
    }

    tempF = (f32)temp1;
            
    vec = func_80065F94(tempF, temp2);
    currentGroundObjectIndex = getGroundObjectIndexFromPlayerPosition(tempF, temp2);
    
    if (wateringCanUses != 0) {

        if ((func_800DA948(currentGroundObjectIndex) & 0x10) && vec.y != 65535.0f) {

            if (currentGroundObjectIndex == TILLED) {
                temp2 = TILLED_WATERED;            
            } else {
                temp2 = currentGroundObjectIndex + 1;
            }
            
            addGroundObjectToMap(gBaseMapIndex, temp2, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

            vec = func_8003AF58(0, (u8)vec.x, (u8)vec.z);
            func_800CF8F8(toolUse.entityIndex, 1, vec.x, vec.y, vec.z);

            if (gBaseMapIndex == POND) {
                blueMistFlowerPlot = temp2;
            }
            
        } else {

            vec = func_8003AF58(0, (u8)vec.x, (u8)vec.z);
            vec.y = entities[ENTITY_PLAYER].coordinates.y;

            func_800CF8F8(toolUse.entityIndex, 1, vec.x, vec.y, vec.z);
            
        } 

        wateringCanUses--;
        
        if (toolUse.entityIndex == 0) {

            switch (gPlayer.staminaLevelForCurrentToolUse) {

                case 0:
                    setAudio(0x17);
                    break;
                case 1:
                    setAudio(0x18);
                    break;
                case 2:
                    setAudio(0x19);
                    break;
                
            }
            
        }
        
    }

    toolUse.entityIndex++;

    switch (gPlayer.staminaLevelForCurrentToolUse) {
            
        case 0:
            toolUse.unk_E = 0;
            break;
        
        case 1:

            if (toolUse.entityIndex == 3) {
                toolUse.unk_E = 0;
            }
    
            break;
        
        case 2:

            if (toolUse.entityIndex == 9) {
                toolUse.unk_E = 0;
            }
            
            break;
            
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3694);

// milker
void func_800D3694(void) {
    func_8009A53C();
    toolUse.unk_E = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D36BC);

// cow bell
void func_800D36BC(void) {
    func_8009B25C();
    toolUse.unk_E = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D36E4);

// brush
void func_800D36E4(void) {
    func_8009A17C();
    func_8009A2D0();
    toolUse.unk_E = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3714);

// clippers
void func_800D3714(void) {
    func_8009AAC8();
    toolUse.unk_E = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D373C);

// turnip seeds
void func_800D373C(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = 8;
        } else {
          groundObjectIndex = 0xD7;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
        
    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3958);

void func_800D3958(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }

    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = 0x13;
        } else {
          groundObjectIndex = 0xD7;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
        
    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3B74);

// cabbage seeds
void func_800D3B74(void) {
 
    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {
        
        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = 0x33;
        } else {
          groundObjectIndex = 0xD7;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3D90);

// tomato seeds
void func_800D3D90(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        if (gSeason == SUMMER || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = 0x53;
        } else {
          groundObjectIndex = 0xD7;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3FAC);

// corn seeds
void func_800D3FAC(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        if (gSeason == SUMMER || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = 0x68;
        } else {
            groundObjectIndex = 0xD7;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D41C8);

// eggplant seeds
void func_800D41C8(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    }
    else {
        temp = D_8011718C[toolUse.entityIndex];
    }

    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        if (gSeason == AUTUMN || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = 0x22;
        } else {
            groundObjectIndex = 0xD7;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D43E4);

// strawberry seeds
void func_800D43E4(void) {
    
    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    }
    else {
        temp = D_8011718C[toolUse.entityIndex];
    }

    vec = func_80065F94(0.0f, temp);

    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        // leaving for reference
        //groundObjectIndex = ((-(gBaseMapIndex != 0x56) & ~0x28) | 0x44);

        groundObjectIndex = (gBaseMapIndex == GREENHOUSE) ? 0x44 : 0xD7;

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }

    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }

    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D45F4);

// moon drop seeds
void func_800D45F4(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    }
    else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {
        
        // alternate for reference: (gSeason - 1 < 2U)
        if ((gSeason == SPRING || gSeason == SUMMER) || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = 0x90; 
        } else {
            groundObjectIndex = 0xD7;
        }

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4814);

void func_800D4814(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        // FIXME: should be range
        if ((gSeason - 1 < 2U) || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = 0x9F; 
        } else {
            groundObjectIndex = 0xD7;
        }

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4A34);

void func_800D4A34(void) {

    u8 direction;
    u8 temp;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    } else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {
        addGroundObjectToMap(gBaseMapIndex, 0xB2, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4C28);

// chicken feed
void func_800D4C28(void) {
    
    u8 levelInteractionIndex;
    
    levelInteractionIndex = func_800ACE50(gBaseMapIndex);
  
    if (levelInteractionIndex != 0xFF) {
        // set map object for fodder
        func_80073244(levelInteractionIndex - 0x13);
        gChickens[levelInteractionIndex - 0x13].flags |= CHICKEN_FED;
    }
    
    toolUse.unk_E = 0;  

}

// ball
// empty function
void func_800D4CA8(void) {}

// feeding bottle
// empty function
void func_800D4CB0(void) {}

// unknown tool
// empty function
void func_800D4CB8(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4CC0);

// fishing pole
void func_800D4CC0(void) {
    toolUse.unk_E = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4CD0);

// miracle potion
void func_800D4CD0(void) {

    if (func_8009A810()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    toolUse.unk_E = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4D1C);

// cow medicine
void func_800D4D1C(void) {

    if (func_8009A400()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    toolUse.unk_E = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4D68);

// grass seeds
void func_800D4D68(void) {

    u8 direction;
    u8 temp;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < SOUTHWEST) {
        temp = D_80117180[toolUse.entityIndex];
    }
    else {
        temp = D_8011718C[toolUse.entityIndex];
    }
    
    vec = func_80065F94(0.0f, temp);
 
    if ((func_800DA948(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & 0x20) && vec.y != 65535.0f) {

        if (gBaseMapIndex == FARM) {
            addGroundObjectToMap(FARM, 0x85, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);    
        }

    }
    
    if (toolUse.entityIndex == 0) {
        setAudio(0x1D);
    }
    
    toolUse.entityIndex++;
    
    if (toolUse.entityIndex == 9) {
        toolUse.unk_E = 0;
    }

}

static const u8 D_80122340[4] = { 2, 8, 6, 0 };

static const u8 D_80122344[4] = { 6, 8, 2, 0 };

static const u8 D_80122348[4] = { 3, 8, 5, 0 };

static const u8 D_8012234C[4] = { 5, 8, 3, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122340);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122344);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122348);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_8012234C);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122350);

static const u8 D_80122350[12] = { 8, 0, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_8012235C);

static const u8 D_8012235C[12] = { 8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122368);

static const u8 D_80122368[12] = { 3, 4, 5, 2, 8, 6, 1, 0, 7, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122374);

static const u8 D_80122374[12] = { 5, 4, 3, 6, 8, 2, 7, 0, 1, 0, 0, 0 };
 
//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleBlueFeatherUse);

void handleBlueFeatherUse(void) {
     
    toggleDailyEventBit(SUCCESSFUL_PROPOSAL);
    
    if (npcTalkingTo == 0xFF) goto func_end;

    func_80085D48(npcTalkingTo, 0);
    
    npcs[npcTalkingTo].movingFlag = npcs[npcTalkingTo].unk_1E;
    
    setSpecialDialogueBit(0x135);
    
    switch (npcTalkingTo) {
        case MARIA:
            if (npcAffection[MARIA] >= 220) {
                gWife = MARIA;   
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(MARIA);
            }
            break;
        case POPURI:
            if (npcAffection[POPURI] >= 220) {
                gWife = POPURI;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(POPURI);
            }
            break;
        case ELLI:
            if (npcAffection[ELLI] >= 220) {
                gWife = ELLI;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ELLI);
            }
            break;
        case ANN:
            if (npcAffection[ANN] >= 220) {
                gWife = ANN;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ANN);
            }
            break;
        case KAREN:
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
        toolUse.unk_E = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5130);

// empty bottle
void func_800D5130(void) {

    if (func_800ACFE8(gBaseMapIndex)) {
        gPlayer.bottleContents = 1;
    } else if (checkWineBarrelInteraction(gBaseMapIndex) ) {
        gPlayer.bottleContents = 2;
    } else {
        func_8009A97C();
    }

    toolUse.unk_E = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D51B0);

// inline, used by initializeGameVariables
void func_800D51B0(void) {

    u8 i;

    for (i = 0; i < 10; i++) {

        itemInfo[i].unk_0.x = 0;
        itemInfo[i].unk_0.y = 0;
        itemInfo[i].unk_0.z = 0;

        itemInfo[i].unk_C.x = 0;
        itemInfo[i].unk_C.y = 0;
        itemInfo[i].unk_C.z = 0;

        itemInfo[i].unk_18.x = 0;
        itemInfo[i].unk_18.y = 0;
        itemInfo[i].unk_18.z = 0;

        itemInfo[i].unk_24 = 0;
        itemInfo[i].unk_26 = 0;
        itemInfo[i].unk_28 = 0;
        itemInfo[i].unk_2A = 0;
        itemInfo[i].flags = 0;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5290);

void func_800D5290(void) {

    u8 i;
    
    for (i = 0; i < 10; i++) {
        if (itemInfo[i].flags & 1) {
            func_800D56E8(i, itemInfo[i].unk_28);
        }    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5308);

// tool acquisition handler
u8 func_800D5308(u8 index, u8 arg1, u32 arg2, s32 arg3, s32 arg4) {

    itemInfo[index].unk_2A = arg1;
    itemInfo[index].unk_28 = arg2;
    itemInfo[index].unk_26 = 0;
    itemInfo[index].flags = arg3 | (arg4 | 1);
    
    gItemBeingHeld = func_800D67E4(arg2);
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5390);

u8 func_800D5390(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4) {
    
    u8 found = FALSE;
    int tempBit = 1;
    
    while (index < 10 && !found) {
        
        if (!(itemInfo[index].flags & 1)) {
          found = TRUE;
        }
        else {
          index++;
        }

    }
    
    if (found) {
        itemInfo[index].unk_2A = arg1;
        itemInfo[index].unk_28 = arg2;
        itemInfo[index].flags = arg3 | (arg4 | tempBit);
        itemInfo[index].unk_26 = 0;
        gItemBeingHeld = func_800D67E4(arg2);
    }
      
    else {
        index = 0xff;
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5488);

u8 func_800D5488(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4) {
    
    u8 found = FALSE;
    
    while (index < 10 && !found) {

        if (!(itemInfo[index].flags & 1)) {
          found = TRUE;
        }
        else {
          index++;
        }

    }

    if (found) {
        
        itemInfo[index].unk_2A = arg1;
        itemInfo[index].unk_28 = arg2;
        itemInfo[index].flags = arg3 | (arg4 | 1);
        itemInfo[index].unk_26 = 0;

    } else {
        index = 0xff;
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5548);

void func_800D5548(u8 index) {

    if (itemInfo[index].flags & 1) {        
        deactivateEntity(index + ITEMS_OFFSET);
        itemInfo[index].unk_2A = 0;
        itemInfo[index].flags = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D55B8);

u16 func_800D55B8(u8 index) {
    return itemInfo[index].unk_28;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D55E4);

void func_800D55E4(u8 index, u8 arg1) {

    if (itemInfo[index].flags & 1) {
        itemInfo[index].unk_2A = arg1;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5628);

void func_800D5628(u8 index, f32 arg1, f32 arg2, f32 arg3) {

    itemInfo[index].unk_0.x = arg1;
    itemInfo[index].unk_0.y = arg2;
    itemInfo[index].unk_0.z = arg3;
    itemInfo[index].flags |= 2;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5688);

void func_800D5688(u8 index, f32 arg1, f32 arg2, f32 arg3) {

    itemInfo[index].unk_18.x = arg1;
    itemInfo[index].unk_18.y = arg2;
    itemInfo[index].unk_18.z = arg3;
    itemInfo[index].flags |= 8;
     
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D56E8);

void func_800D56E8(u8 itemIndex, u16 arg1) {

    itemInfo[itemIndex].unk_28 = arg1;

    loadEntity(itemIndex + 0x27, D_801174A0[arg1][0], TRUE);
    func_8002FF38(itemIndex + 0x27, FALSE);

    if (itemInfo[itemIndex].flags & 8){
        setEntityTrackingTarget(itemIndex + 0x27, 0, 0xFF);
    } else {
        setEntityTrackingTarget(itemIndex + 0x27, 0xFFFF, 0xFF);
    }

    setEntityCollidable(itemIndex + 0x27, FALSE);
    setEntityYMovement(itemIndex + 0x27, FALSE);
    setEntityTracksCollisions(itemIndex + 0x27, FALSE);
    enableEntityMovement(itemIndex + 0x27, FALSE);

    setEntityColor(itemIndex + 0x27, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);

    setEntityAttachmentOffset(itemIndex + 0x27, 0, 0, 0);

    if (itemInfo[itemIndex].flags & 4) {
        setEntityAnimationWithDirectionChange(itemIndex + 0x27, D_801174A0[arg1][gSeason]);
    } else {
        setEntityAnimation(itemIndex + 0x27, D_801174A0[arg1][gSeason]);
    }

    if (D_80117D60[arg1] != 0xFFFF) {
        setEntityPaletteIndex(itemIndex + 0x27, D_80117D60[arg1]);
    }

    setEntityShadow(itemIndex + 0x27, 2);

    itemInfo[itemIndex].unk_2A = 0;
    
}

u16 getItemFlags(u16 index) {
    return itemFlags[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5A88);

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

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5B00);

u8 func_800D5B00(u16 index) {
    return D_80118540[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5B18);

u8 func_800D5B18(u16 index) {
    return D_80118620[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5B30);

u8 func_800D5B30(void) {

    u32 padding[4];
    
    u8 groundObjectIndex;
    u8 heldItemIndex;
    bool result = FALSE;
    
    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

        if (gPlayer.heldItem == 0) {

            groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8);

            if (groundObjectIndex != 0xFF) {
                heldItemIndex = getHeldItemIndexFromGroundObject(groundObjectIndex);
            }

            if (groundObjectIndex && (groundObjectIndex != 0xFF && heldItemIndex)) {

                gPlayer.heldItem = heldItemIndex;
                
                switch (groundObjectIndex) {

                    case 0x65 ... 0x66:
                        addGroundObjectToMapFromPlayerPosition(0x5F, 1.0f, 8);
                        break;
                    
                    case 0x82 ... CORN_RIPE:
                        addGroundObjectToMapFromPlayerPosition(0x7A, 1.0f, 8);
                        break;

                    case 0x30 ... 0x31:
                        addGroundObjectToMapFromPlayerPosition(0x2A, 1.0f, 8);
                        break;
                    
                    default:
                        
                        switch (getGroundObjectMapAdditionIndex(groundObjectIndex)) {

                            case 4:
                                addGroundObjectToMapFromPlayerPosition(TILLED, 1.0f, 8);
                                break;
                            case 5:
                                addGroundObjectToMapFromPlayerPosition(TILLED_WATERED, 1.0f, 8);
                                break;
                            default:
                                addGroundObjectToMapFromPlayerPosition(UNTILLED, 1.0f, 8);
                                break;
                            
                        }         

                        break;

                }
                
                result = TRUE;

            }
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5CC0);

bool func_800D5CC0(u8 itemIndex) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    u8 temp;
    u16 temp2;
    u16 unk_28;
    u8 direction;
    u8 i;
    u8 levelInteractionIndex;
    
    bool result = FALSE;

    // FIXME: I am beyond clueless
    temp2 = *(u16*)0&itemInfo[itemIndex].unk_28;
    
    if (temp2) {
        unk_28 = itemInfo[itemIndex].unk_28;
    } else {
        unk_28 = itemInfo[itemIndex].unk_28;
    }
    
    switch (itemInfo[itemIndex].unk_28) {        
        
        case 2:              
            addGroundObjectToMapFromPlayerPosition(4, 1.0f, 8);
            setGroundObjects(mapControllers[MAIN_MAP_INDEX].mainMapIndex);
            break;
        
        case 3:
            if ((getLevelInteractionIndexFromEntityPosition(0, 0, 32.0f) == 0x11) && (gBaseMapIndex == FARM)) {
                gLumber += adjustValue(gLumber, 1, MAX_LUMBER);
            } else {
                addGroundObjectToMapFromPlayerPosition(6, 1.0f, 8);
                setGroundObjects(mapControllers[MAIN_MAP_INDEX].mainMapIndex);
            }
            break;

        case 0x60:                                     
        case 0x61:                                     
        case 0x62:                                     
        case 0x63:                                     
        case 0x64:                                     
        case 0x65:                                     
        case 0x66:                                     
        case 0x67:                                     
        case 0x68:                                     
        case 0x69:                                     
        case 0x6A:                                     
        case 0x6B:                                     
        case 0x6C:                                     
        case 0x6D:                                     
        case 0x6E:                                     
        case 0x6F:                                     

            vec2 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            gChickens[gPlayer.heldChickenIndex].coordinates = vec2;

            gChickens[gPlayer.heldChickenIndex].direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            gChickens[gPlayer.heldChickenIndex].location = gBaseMapIndex;

            gChickens[gPlayer.heldChickenIndex].flags &= ~8;

            func_8008B2E8(gPlayer.heldChickenIndex);
            
            break;

        case 0x58:                                     
        case 0x59:                                     
        case 0x5A:                                     
        case 0x5B:                                     
        case 0x5C:                                     
        case 0x5D:                                     
        case 0x5E:                                     
        case 0x5F:                              

            vec3 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            dogInfo.coordinates = vec3;

            dogInfo.direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            dogInfo.location = gBaseMapIndex;

            dogInfo.flags &= ~8;

            func_8008B1B8();
            
            break;

        case 0xAB:                                     
        case 0xAC:                                     
        case 0xAD:                                     
        case 0xAE:                                     
        case 0xAF:                                     
        case 0xB0:                                     
        case 0xB1:                                     
        case 0xB2:     
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(0, direction, vec.x, vec.y, vec.z), 0);

            break;
        
        case 0x7B:                                     
        case 0x7C:                                     
        case 0x7D:                                     
        case 0x7E:                                     
        case 0x7F:                                     
        case 0x80:                                     
        case 0x81:                                     
        case 0x82: 
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(1, direction, vec.x, vec.y, vec.z), 0);

            break;

        case 0x83:                                     
        case 0x84:                                     
        case 0x85:                                     
        case 0x86:                                     
        case 0x87:                                     
        case 0x88:                                     
        case 0x89:                                     
        case 0x8A:        
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(2, direction, vec.x, vec.y, vec.z), 0);

            break;
        case 0x8B:                                     
        case 0x8C:                                     
        case 0x8D:                                     
        case 0x8E:                                     
        case 0x8F:                                     
        case 0x90:                                     
        case 0x91:                                     
        case 0x92:   
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(7, direction, vec.x, vec.y, vec.z), 0);

            break;       
                
        case 0x93:                                     
        case 0x94:                                     
        case 0x95:                                     
        case 0x96:                                     
        case 0x97:                                     
        case 0x98:                                     
        case 0x99:                                     
        case 0x9A:  
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(9, direction, vec.x, vec.y, vec.z), 0);

            break;     

        case 0x9B:                                     
        case 0x9C:                                     
        case 0x9D:                                     
        case 0x9E:                                     
        case 0x9F:                                     
        case 0xA0:                                     
        case 0xA1:                                     
        case 0xA2:         
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(8, direction, vec.x, vec.y, vec.z), 0);
            break;     
        
        case 0xA3:                                     
        case 0xA4:                                     
        case 0xA5:                                     
        case 0xA6:                                     
        case 0xA7:                                     
        case 0xA8:                                     
        case 0xA9:                                     
        case 0xAA:    
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            func_8008BAF0(func_8008A4A8(10, direction, vec.x, vec.y, vec.z), 0);
            break;     

        case 0xB3:                                     
        case 0xB4:                                     
        case 0xB5:                                     
        case 0xB6:                                     
        case 0xB7:                                     
        case 0xB8:                                     
        case 0xB9:                  
            
            switch (unk_28) {                          
                case 0xB3:                                  
                    temp = 0xF;
                    break;
                case 0xB4:                                  
                    temp = 0x10;
                    break;
                case 0xB5:                                  
                    temp = 0x11;
                    break;
                case 0xB6:                                  
                    temp = 0x12;
                    break;
                case 0xB7:                                  
                    temp = 0x13;
                    break;
                case 0xB8:                                  
                    temp = 0x14;
                    break;
                case 0xB9:                                  
                    temp = 0x15;
                    break;
            }

            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            direction = convertWorldToSpriteDirection(entities[PLAYER].direction, MAIN_MAP_INDEX);
            vec.y = entities[ENTITY_PLAYER].coordinates.y;
            
            func_8008BAF0(func_8008A4A8(temp, direction, vec.x, vec.y, vec.z), 1);

            break;

       case 0x14:                              

            gPlayer.heldChickenIndex = func_80087F28(0, 0xFF);

            gChickens[gPlayer.heldChickenIndex].coordinates.x = 92.0f;
            gChickens[gPlayer.heldChickenIndex].coordinates.y = 0;
            gChickens[gPlayer.heldChickenIndex].coordinates.z = -148.0f;
            
            gChickens[gPlayer.heldChickenIndex].direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            gChickens[gPlayer.heldChickenIndex].location = gBaseMapIndex;
            
            gChickens[gPlayer.heldChickenIndex].flags &= ~8;
            gChickens[gPlayer.heldChickenIndex].flags |= 0x20;

            func_8008B2E8(gPlayer.heldChickenIndex);

            break;

        case 0x39:                    

            levelInteractionIndex = func_800ACDF4(gBaseMapIndex);

            if (levelInteractionIndex == 0x12) {
                
                func_8007341C(8);

                for (i = 0; i < MAX_FARM_ANIMALS; i++) {
                    gFarmAnimals[i].flags |= 0x10;
                }

            } else {
                
                func_8007341C(levelInteractionIndex - 0x13);
                gFarmAnimals[levelInteractionIndex - 0x13].flags |= 0x8;
                
            }

            
            break;
        
        case 0xBA:                                     
        case 0xBB:                                     
        case 0xBC:                                     
        case 0xBD:                                     
        case 0xBE:                                     
        case 0xBF:                                     
        case 0xC0:                                     
        case 0xC1:                  

            npcs[BABY].levelIndex = HOUSE;
            npcs[BABY].startingCoordinates.y = 0;
            npcs[BABY].direction = NORTH;
            npcs[BABY].unk_1E = 1;
            npcs[BABY].animationIndex1 = 0;
            npcs[BABY].movingFlag = 1;

            npcs[BABY].startingCoordinates.x = -192.0f;
            npcs[BABY].startingCoordinates.z = -64.0f;
            
            npcs[BABY].flags |= 1;
            
            setDailyEventBit(0x16);
            func_80075374(BABY, 0);

            break;
            
        case 0xC2:                                     
        case 0xC3:                                     
        case 0xC4:                                     
        case 0xC5:                                     
        case 0xC6:                                     
        case 0xC7:                                     
        case 0xC8:                                     
        case 0xC9:                               

            vec4 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            npcs[BABY].startingCoordinates = vec4;

            npcs[BABY].levelIndex = gBaseMapIndex;
            npcs[BABY].flags |= 1;

            npcs[BABY].direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            npcs[BABY].unk_1E = 3;
            npcs[BABY].animationIndex1 = 0x39;
            npcs[BABY].movingFlag = 3;
            
            setDailyEventBit(0x16);
            func_80075374(BABY, 0);
            
            break;
        
    }


    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D67E4);

u8 func_800D67E4(u8 index) {
    return D_80117F20[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D67FC);

void func_800D67FC(u8 index) {

    u16 temp;

    switch (index) {
        case 0x60 ... 0x6F:
            temp =  gPlayer.heldChickenIndex + 0xF5;
            break;
        default:
            // array of npc/dialogue-related u16s
            temp = D_80118000[index];
            break;
    }

    showTextBox(1, 6, temp, 0, 2);

}

// alternate for reference
// void func_800D67FC(u8 index) {
    
//     s32 temp;
//     u16 temp2;

//     temp = index;
    
//     if (temp < 0x70) {
//         if (temp > 0x5F) {
//             temp2 = gPlayer.heldChickenIndex + 0xF5;
//             goto func_end;
//         } 
//     } 

// 
//     temp2 = D_80118000[index];
    
// func_end:
//     showTextBox(1, 6, temp2, 0, 2);
// }

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D6868);

void func_800D6868(u8 index) {

    u32 result;
    
    dailyShippingBinValue += adjustValue(dailyShippingBinValue, D_801181C0[index], MAX_TOTAL_SHIPPING);
     
    switch (index) {
        case 13:
            D_801654F4 += adjustValue(D_801654F4, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 14:
            D_80237414 += adjustValue(D_80237414, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 18:
            D_801FB6FC += adjustValue(D_801FB6FC, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 15:
            D_801C3F80 += adjustValue(D_801C3F80, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 19:
            D_801806C0 += adjustValue(D_801806C0, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 16:
            D_80188F60 += adjustValue(D_80188F60, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case 17:
            D_801FB5D0 += adjustValue(D_801FB5D0, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
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

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D6B28);

// put food in dog bowl
void func_800D6B28(void) {

    func_80034DC8(MAIN_MAP_INDEX, 0, 0x13);
    setDailyEventBit(FED_DOG);

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D6B58);

static inline void setVec3f(u8 index, f32 x, f32 y, f32 z) {

    itemInfo[index].unk_0.x = x;
    itemInfo[index].unk_0.y = y;
    itemInfo[index].unk_0.z = z;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D6B58);

void func_800D6B58(u8 arg0, u8 index) {

    u8 temp;
    u8 temp2;
    u8 flags;
    
    itemInfo[index].unk_18.x = 0;
    itemInfo[index].unk_18.y = 0;
    itemInfo[index].unk_18.z = 0;
    itemInfo[index].flags |= 8;

    setEntityMovementVector(index + ITEMS_OFFSET, 0.0f, 0.0f, 0.0f, 0.0f);
    setEntityTrackingTarget(index + ITEMS_OFFSET, 0xFFFF, 0xFF);
    
    setEntityCollidable(index + ITEMS_OFFSET, FALSE);
    setEntityYMovement(index + ITEMS_OFFSET, FALSE);
    setEntityTracksCollisions(index + ITEMS_OFFSET, FALSE);
    enableEntityMovement(index + ITEMS_OFFSET, FALSE);

    if (gBaseMapIndex == FARM) {
        
        temp = getLevelInteractionIndexFromEntityPosition(0, 0.0f, 32.0f);
        // FIXME: possibly fake
        temp2 = temp - 0x1B;
        
        if (temp2 < 2 || temp == 0x1D) {
            setVec3f(index, 288.0f, 80.0f, -404.0f);
            itemInfo[index].flags |= 2;
            setEntityAnimation(index + ITEMS_OFFSET, 0xE9);
        } 

    }
    
    if (gBaseMapIndex == MOUNTAIN_1) {
        
        temp = getLevelInteractionIndexFromEntityPosition(0, 0.0f, 40.0f);
        
        if (temp == 0x10 || temp == 0x14) {
            setVec3f(index, 160.0f, 64.0f, -128.0f);
            itemInfo[index].flags |= 2;
            setEntityAnimation(index + ITEMS_OFFSET, 0xE9);
        }
        
        if (!(gCutsceneFlags & 1)) {

            if (!checkLifeEventBit(0x5E) && !checkDailyEventBit(0x21) && (itemInfo[index].unk_28 - 0x25) < 2U && 8 < gHour && gHour < 17) {
                setDailyEventBit(0x47);
            }
            if (!checkLifeEventBit(0x5F) && !checkDailyEventBit(0x21) && itemInfo[index].unk_28 ==  0x27 && 8 < gHour && gHour < 17) {
                setDailyEventBit(0x48);
            }

        }
        
    }

    if (gBaseMapIndex == POND) {
        
        temp = getLevelInteractionIndexFromEntityPosition(0, 0.0f, 32.0f);
        
        if (temp == 0x10) {
            setVec3f(index, -16.0f, 80.0f, -224.0f);
            itemInfo[index].flags |= 2;
            setEntityAnimation(index + ITEMS_OFFSET, 0xE9);
        }
        
        if (!(gCutsceneFlags & 1)) {
            if (!checkDailyEventBit(0x46) && itemFlags[itemInfo[index].unk_28] & 0x8000 && 8 < gHour && gHour < 17 ) {
                setDailyEventBit(0x45);    
            }
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D7010);

void func_800D7010(void) {

    Vec3f vec;
    u8 i = 0;
    bool set;

    f32 x, y, z;
    f32 x2, y2, z2;
    f32 z3;

    for (i = 0; i < 10; i++) {

        if (itemInfo[i].flags & 1) {
            
            do {

                // FIXME: fake match/likely dead code
                if (itemInfo[i].unk_2A) {};
                
                y = 14.0f;
                z = 20.0f;
                x = -16.0f;
                z3 = 8.0f;
                y2 = 32.0f;
                z2 = 16.0f;

                set = FALSE;
                
                switch (itemInfo[i].unk_2A) {

                    case 2:
                        func_800D56E8(i, itemInfo[i].unk_28);
                        itemInfo[i].unk_18.x = 0;
                        itemInfo[i].unk_18.y = y;
                        itemInfo[i].unk_18.z = z;
                        itemInfo[i].flags |= 8;
                        break;

                    case 16:
                        func_800D56E8(i, itemInfo[i].unk_28);
                        setEntityTrackingTarget(i + 0x27, 0, 0);
                        itemInfo[i].unk_18.x = x;
                        itemInfo[i].unk_18.y = 24.0f;
                        itemInfo[i].unk_18.z = z3;
                        itemInfo[i].flags |= 8;
                        break;
                    
                    case 21:
                        func_800D56E8(i, itemInfo[i].unk_28);
                        setEntityTrackingTarget(i + 0x27, 0, 0);
                        itemInfo[i].unk_18.x = x;
                        itemInfo[i].unk_18.y = 64.0f;
                        itemInfo[i].unk_18.z = z3;
                        itemInfo[i].flags |= 8;
                        break;

                    case 3:
                        func_800D56E8(i, itemInfo[i].unk_28);
                        itemInfo[i].unk_18.x = 0.0f;
                        itemInfo[i].unk_18.y = y2;
                        itemInfo[i].unk_18.z = z2;
                        itemInfo[i].unk_2A = 4;
                        itemInfo[i].flags |= 8;
                        break;                  

                    case 4:    
                        
                        setEntityTrackingTarget(i + 0x27, 0xFFFF, 0xFF);

                        vec = getMovementVectorFromDirection(4.0f, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX), 0.0f);

                        itemInfo[i].unk_C = vec;

                        itemInfo[i].unk_C.y = entities[i + 0x27].coordinates.y;
                        
                        itemInfo[i].unk_26 = 0;
                        itemInfo[i].unk_2A = 5;

                        break;

                    case 5:
                        
                        if (entities[i + 0x27].flags & 0x800) {
                            
                            itemInfo[i].unk_C.x = 0;
                            itemInfo[i].unk_C.y = 0;
                            itemInfo[i].unk_C.z = 0;
                            
                            func_8002FF38(i + 0x27, FALSE);
                            setEntityCollidable(i + 0x27, FALSE);
                            setEntityYMovement(i + 0x27, FALSE);
                            setEntityTracksCollisions(i + 0x27, FALSE);
                            enableEntityMovement(i + 0x27, FALSE);

                            itemInfo[i].unk_2A = 12;

                            set = TRUE;

                        } else {

                            entities[i + 0x27].coordinates.y = itemInfo[i].unk_C.y + (f32)(itemInfo[i].unk_26 * itemInfo[i].unk_26) * -0.5f;
                            itemInfo[i].unk_26++;
                            
                        }
                        
                        break;

                    default:
                        break;

                    case 12:

                        deactivateEntity(i + 0x27);
                        loadEntity(i + 0x27, 0x5E, 1);
                        setEntityColor(i + 0x27, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
                        setEntityMovementVector(i + 0x27, 0.0f, 0.0f, 0.0f, 0.0f);
                        func_8002FF38(i + 0x27, FALSE);
                        setEntityTrackingTarget(i + 0x27, 0xFFFF, 0xFF);
                        setEntityCollidable(i + 0x27, FALSE);
                        setEntityYMovement(i + 0x27, FALSE);
                        setEntityTracksCollisions(i + 0x27, FALSE);
                        enableEntityMovement(i + 0x27, FALSE);
                        setEntityAnimation(i + 0x27, D_801171A0[itemInfo[i].unk_28][gSeason-1]);

                        itemInfo[i].unk_2A = 1;

                        if (itemFlags[itemInfo[i].unk_28] & 1) {
                            gHappiness += adjustValue(gHappiness, -1, MAX_HAPPINESS);
                        } 

                        break;


                    case 6:

                        itemInfo[i].unk_18.x = 0;
                        itemInfo[i].unk_18.y = z3;
                        itemInfo[i].unk_18.z = z;
                        
                        itemInfo[i].flags |= 8;

                        func_800D56E8(i, itemInfo[i].unk_28);

                        break;

                    case 7:
                        itemInfo[i].unk_18.x = 0;
                        itemInfo[i].unk_18.y = y;
                        itemInfo[i].unk_18.z = z;
                        itemInfo[i].unk_2A = 0;
                        
                        itemInfo[i].flags |= 8;

                        break;
                    
                    case 8:
                        itemInfo[i].unk_18.x = 0;
                        itemInfo[i].unk_18.y = z3;
                        itemInfo[i].unk_18.z = z;
                    
                        itemInfo[i].flags |= 8;

                        break;
                    
                    case 9:
                        
                        func_800D5CC0(i);
       
                        if (itemInfo[i].flags & 1) {
                            deactivateEntity(i + 0x27);
                            itemInfo[i].unk_2A = 0;
                            itemInfo[i].flags = 0;
                        }
                        
                        break;

                    case 10:
                        
                        setEntityTrackingTarget(i + 0x27, 0, 0);
                        
                        itemInfo[i].unk_18.x = x;
                        itemInfo[i].unk_18.y = y2;
                        itemInfo[i].unk_18.z = z3;
                        itemInfo[i].flags |= 8;

                        break;

                    case 14:
                        
                        func_800D6868(itemInfo[i].unk_28);

                        switch (gBaseMapIndex) {        

                            case FARM:                     
                                func_80038990(0, 0x12, 0);
                                setAudio(4);
                                break;
                     
                            case BARN:
                                func_80038990(0, 0, 0);
                                setAudio(4);
                                break;

                            case COOP:
                                func_80038990(0, 0, 0);
                                setAudio(4);
                                break;
                            
                            case GREENHOUSE:                     
                                func_80038990(0, 0, 0);
                                setAudio(4);
                                break;
                            
                        }
                        
                        deactivateEntity(i + 0x27);
                        itemInfo[i].unk_2A = 1;

                        break;


                    case 17:
                        itemInfo[i].unk_18.x = 0.0f;
                        itemInfo[i].unk_18.y = 8.0f;
                        itemInfo[i].unk_18.z = z;
                        itemInfo[i].flags |= 8;
                        break;
                    
                    case 18:
                        func_80034DC8(MAIN_MAP_INDEX, 0, 0x13);
                        setDailyEventBit(0x43);
                        deactivateEntity(i + 0x27);
                        itemInfo[i].unk_2A = 1;
                        break;
                    
                    case 13:
                    case 19:
                        itemInfo[i].unk_18.x = 0.0f;
                        itemInfo[i].unk_18.y = y2;
                        itemInfo[i].unk_18.z = z2;
                        itemInfo[i].flags |= 8;
                        break;
                    
                    case 20:
                        func_800D6B58(itemInfo[i].unk_28, i);
                        setAudio(0x28);
                        itemInfo[i].unk_2A = 1;
                        break;
                    
                    case 1:
                        
                        if (checkEntityAnimationStateChanged(i + 0x27) || !(itemInfo[i].unk_26 < 30)) {

                            if (itemInfo[i].flags & 1) {
                                deactivateEntity(i + 0x27);

                                // FIXME: fake match in order to get float registers right
                                if (y2) {
                                    itemInfo[i].unk_2A = 0;                                    
                                } else {
                                    itemInfo[i].unk_2A = 0;
                                }
                                
                                if (z2) {
                                    itemInfo[i].flags = 0;
                                } else {
                                    itemInfo[i].flags = 0;
                                }
                                
                            }
                            
                            if (checkDailyEventBit(0x45)) {
                                
                                if (checkSpecialDialogueBit(0x25)) {
                                    
                                    if (!checkSpecialDialogueBit(0x14B)) {
                                         gCutsceneIndex = 0x158;
                                    } else {
                                         gCutsceneIndex = 0x155;
                                    }
                                    
                                } else {
                                    gCutsceneIndex = 0x155;
                                }
                                
                                loadCutscene(0);
                                
                            }
                            
                            if (checkDailyEventBit(0x47)) {
                                gCutsceneIndex = 0x150;
                                loadCutscene(0);
                            }
                            
                            if (checkDailyEventBit(0x48)) {
                                gCutsceneIndex = 0x151;
                                loadCutscene(0);
                            } 
                            
                        } else {
                            itemInfo[i].unk_26 +=  1;
                        }
                        
                        break;
                    
                }
                
            } while (set);

            if (itemInfo[i].flags & 2) {
                setEntityCoordinates(i + 0x27, itemInfo[i].unk_0.x, itemInfo[i].unk_0.y, itemInfo[i].unk_0.z);
            }
            
            if (itemInfo[i].flags & 8) {
                setEntityAttachmentOffset(i + 0x27, itemInfo[i].unk_18.x, itemInfo[i].unk_18.y, itemInfo[i].unk_18.z);
            }
            
            setEntityMovementVector(i + 0x27, itemInfo[i].unk_C.x, 0.0f, itemInfo[i].unk_C.z, 4.0f);
            setEntityDirection(i + 0x27, entities[ENTITY_PLAYER].direction);
            
        }
        
    }
    
}