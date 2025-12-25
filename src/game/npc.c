#include "common.h"

#include "game/npc.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/player.h"
#include "game/time.h"
#include "game/weather.h"

#include "assetIndices/entities.h"
#include "assetIndices/maps.h"

#include "buffers/buffers.h"

#include "ld_symbols.h"

// bss
npcInfo npcs[MAX_NPCS];

u8 npcTalkingTo;

// data
u16 npcToEntityAssetIndex[] = { 
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
    11, 12, 13, 14, 15, 16, 17, 18, 
    19, 20, 21, 22, 23, 24, 25, 26, 
    27, 28, 29, 30, 31, 31, 32, 33,
    33, 33, 34, 35, 41, 40, 37, 97,
    98, 99, 100, 101, 36 
};

u16 npcToDialogueBytecodeIndex[] = {
    9, 8, 2, 10, 4, 63, 7, 11, 6, 5, 
    3, 31, 36, 20, 19, 12, 18, 13, 14,
    38, 15, 17, 22, 16, 33, 37, 24, 21,
    23, 32, 28, 29, 30, 25, 26, 27, 34,
    35, 64, 65, 66, 67, 67, 67, 67, 67, 
    69
};

// forward declarations
void func_80085114(void);
void func_8008520C(void);
void func_80075A78(u8);
void func_80076108(u8 index, u8 arg1, u8 arg2);
void func_800762B4(u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8); 
void func_80076944(u8 npcIndex);
u8 func_80076DCC(u8);
void func_80076F10(void);
void func_80077D14(void);
void func_80078BF0(void);
void func_80079A74(void);
void func_8007A9B0(void);
void func_8007B828(void);
void func_8007BD50(void);
void func_8007C088(void);
void func_8007C3D4(void);
void func_8007C7C4(void);
void func_8007CEB4(void);
void func_8007D3C4(void);
void func_8007D6E0(void);
void func_8007D9F4(void);
void func_8007DB38(void);
void setEllenLocation(void);
void func_8007DF14(void);
void func_8007E07C(void);
void func_8007E2E4(void);
void func_8007E440(void);
void func_8007E600(void);
void func_8007ECE4(void);
void func_8007F378(void);
void func_8007F4E0(void);
void func_8007F81C(void);
void func_8007FA44(void);
void func_8007FC0C(void);
void func_8007FCD8(void);
void func_8007FE40(void);
void func_8007FF04(void);
void func_800800D0(void);
void func_80080364(void);
void func_80080630(void);
void func_800807F8(void);
void func_800808B0(void);
void func_80080958(void);
void func_80080A10(void);
void func_80080B6C(void);
void func_80080C9C(void);
void func_80080DAC(void);
void func_80080EBC(void);
void func_80081030(void);
void func_80081228(void);
void func_80081420(void);
void func_80081618(void);
void func_80081810(void);
void func_80081A08(void);
void func_80081C90(void);
void func_80081E5C(void);
void func_80082164(void);
void func_80082330(void);
void func_800824FC(void);
void func_8008277C(void);
void func_80082A0C(void);
void func_80082B04(void);
void func_80082C5C(void);
void func_80082DB4(void);
void func_80082EAC(void);
void func_80082FA4(void);
void func_8008309C(void);
void func_80083194(void);
void func_8008328C(void);
void func_80083384(void);
void func_80083458(void);
void func_80083550(void);
void func_8008374C(void);
void func_80083948(void);
void func_80083A40(void);
void func_80083C3C(void);
void func_80083D34(void);
void func_80083E2C(void);
void func_80083F24(void);
void func_8008401C(void);
void func_80084114(void);
void func_8008420C(void);
void func_80084304(void);
void func_800843FC(void);
void func_800844F4(void);
void func_80084770(void);
void func_800849EC(void);
void func_80084B44(void);
void func_80084C3C(void);
void func_80084D34(void);
void func_80084E2C(void);
void func_80084F24(void);
void func_8008501C(void);
void func_80085114(void);
void func_8008520C(void);
void func_80085304(void);
void func_800853FC(void);
void func_800854F4(void);
void func_800855EC(void);
void func_800856E4(void);
void func_800857DC(void);
 
//INCLUDE_ASM("asm/nonmatchings/game/npc", deactivateNPCEntities);

void deactivateNPCEntities(void) {
    
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        
        if (npcs[i].flags & 4) {
            deactivateEntity(npcs[i].entityIndex);
        }
        
        npcs[i].flags = 0;
        npcs[i].movingFlag = 0;
        npcs[i].animationIndex1 = 0;
        npcs[i].animationIndex2 = 0;
        npcs[i].unk_21 = 0;
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", ma);

u8 setupNPCEntity(u8 npcIndex, int currentEntityOffset) {

    int currentEntityIndex = currentEntityOffset;

    if ((npcs[npcIndex].flags & 1) && npcs[npcIndex].levelIndex == gBaseMapIndex) {
        
        npcs[npcIndex].flags |= 4;

        switch (npcIndex) {
            case MARIA:
                if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE; 
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case POPURI:
                if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case ELLI:
                if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case ANN:
                if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case KAREN:
                if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                    npcs[npcIndex].entityIndex = ENTITY_WIFE;
                } else {
                    npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                    currentEntityIndex++;
                }
                break;
            case BABY:                               
                npcs[npcIndex].entityIndex = ENTITY_BABY;
                break;
            case PHOTOGRAPHER:
                npcs[npcIndex].entityIndex = ENTITY_PHOTOGRAPHER;
                break;
            case HARRIS:                                 
            case GRAY:                                 
            case JEFF:                                 
            case CLIFF:                                 
            case KAI:                                
            case MAYOR:                                
            case MAYOR_WIFE:                                
            case LILLIA:                                
            case BASIL:                                
            case ELLEN:                                
            case DOUG:                                
            case GOTZ:                                
            case SASHA:                                
            case POTION_SHOP_DEALER:                                
            case KENT:                                
            case STU:                                
            case MIDWIFE:                                
            case MAY:                                
            case RICK:                                
            case PASTOR:                                
            case SHIPPER:                                
            case SAIBARA:                                
            case DUKE:                                
            case GREG:                                
            case CARPENTER_1:                                
            case CARPENTER_2:                                
            case MASTER_CARPENTER:                                
            case HARVEST_SPRITE_1:                                
            case HARVEST_SPRITE_2:                                
            case HARVEST_SPRITE_3:                                
            case SYDNEY:                                
            case BARLEY:                                
            case GOURMET_JUDGE:
            default:
                npcs[npcIndex].entityIndex = ENTITY_NPC_BASE + (u8)currentEntityIndex;
                currentEntityIndex++;
                break;
        }
    
        npcs[npcIndex].currentCoordinates.x = npcs[npcIndex].startingCoordinates.x;
        npcs[npcIndex].currentCoordinates.y = npcs[npcIndex].startingCoordinates.y;
        npcs[npcIndex].currentCoordinates.z = npcs[npcIndex].startingCoordinates.z;
    
        loadEntity(npcs[npcIndex].entityIndex, npcToEntityAssetIndex[npcIndex], TRUE);
        setEntityCollidable(npcs[npcIndex].entityIndex, TRUE);
        setEntityYMovement(npcs[npcIndex].entityIndex, TRUE);
        func_8002FF38(npcs[npcIndex].entityIndex, FALSE);
    
        // FIXME: macro not matching; should be convertSpriteToWorldDirection(npcs[npcIndex].direction, getCurrentMapRotation(MAIN_MAP_INDEX))
        setEntityDirection(npcs[npcIndex].entityIndex, (npcs[npcIndex].direction + 8 - getCurrentMapRotation(MAIN_MAP_INDEX)) % 8);
        setEntityCoordinates(npcs[npcIndex].entityIndex, npcs[npcIndex].currentCoordinates.x, npcs[npcIndex].currentCoordinates.y, npcs[npcIndex].currentCoordinates.z);
    
        if (npcs[npcIndex].flags & 8) {

            switch (npcIndex) {                   
                case BABY:                             
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, ENTITY_WIFE, 0xFE);
                    break;
                case MARIA_HARRIS_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[MARIA].entityIndex, 0xFE);
                    break;
                case POPURI_GRAY_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[POPURI].entityIndex, 0xFE);
                    break;
                case ELLI_JEFF_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[ELLI].entityIndex, 0xFE);
                    break;
                case ANN_CLIFF_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[ANN].entityIndex, 0xFE);
                    break;
                case KAREN_KAI_BABY:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[KAREN].entityIndex, 0xFE);
                    break;
                }
            
                setEntityAttachmentOffset(npcs[npcIndex].entityIndex, 0, 0xE, 0x14);
                setEntityCollidable(npcs[npcIndex].entityIndex, FALSE);
                setEntityYMovement(npcs[npcIndex].entityIndex, FALSE);
                setEntityTracksCollisions(npcs[npcIndex].entityIndex, FALSE);
                enableEntityMovement(npcs[npcIndex].entityIndex, FALSE);

        }
    
        npcs[npcIndex].unk_21 = 0;
        npcs[npcIndex].unk_22 = 0;
        npcs[npcIndex].flags |= 4;
        
    }

    return currentEntityIndex;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setupActiveNPCs);

// called on map load
void setupActiveNPCs(void) {

    u8 entityIndexOffset;
    u8 i;

    setNPCSpawningLocations();
    
    entityIndexOffset = 0;
    
    for (i = 0; i < MAX_NPCS; i++) {
        // loop through all NPCs but only increment entity index offset when active NPC is found
        entityIndexOffset = setupNPCEntity(i, entityIndexOffset);
    }
    
    // handle all npc animations
    setNpcAnimations();

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", updateNPCCoordinates);

void updateNPCCoordinates(void) {

    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        if (npcs[i].flags & 1) {
            if (npcs[i].levelIndex == gBaseMapIndex) {
                npcs[i].currentCoordinates.x = entities[npcs[i].entityIndex].coordinates.x;
                npcs[i].currentCoordinates.y = entities[npcs[i].entityIndex].coordinates.y;
                npcs[i].currentCoordinates.z = entities[npcs[i].entityIndex].coordinates.z; 
            }
            
        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075A18);

void func_80075A18(void) {
    
    u8 i;

    for (i = 0; i < MAX_NPCS; i++) {
        npcs[i].location = getRandomNumberInRange(0, 3);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075A78);
void func_80075A78(u8 npcIndex) {

    Vec3f vec;
    u8 direction = npcs[npcIndex].direction;

    f32 f1, f2, f3;
    f32 f4, f5, f6;
    
    if (npcs[npcIndex].movingFlag == 0x10) {

        direction = (gPlayer.direction + 4) % 8;        

        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].animationIndex1);
        npcs[npcIndex].movingFlag = npcs[npcIndex].unk_1E;
        
    }

    if (npcs[npcIndex].movingFlag == 0x20) {
        direction = (gPlayer.direction + 4) % 8;        
        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].animationIndex1);
    }

    npcs[npcIndex].currentCoordinates.x = entities[npcs[npcIndex].entityIndex].coordinates.x;
    npcs[npcIndex].currentCoordinates.y = entities[npcs[npcIndex].entityIndex].coordinates.y;
    npcs[npcIndex].currentCoordinates.z = entities[npcs[npcIndex].entityIndex].coordinates.z;

    setEntityDirection(npcs[npcIndex].entityIndex, convertSpriteToWorldDirection(direction, MAIN_MAP_INDEX));

    vec = getMovementVectorFromDirection(npcs[npcIndex].speed, direction, 0.0f);

    f1 = npcs[npcIndex].startingCoordinates.x;
    f2 = npcs[npcIndex].unk_1A;
    f3 = npcs[npcIndex].currentCoordinates.x + vec.x;
    
    if ((s16)(f1 + f2) >= (s16)f3 && (s16)f3 >= (s16)(f1 - f2)) {

        f4 = npcs[npcIndex].startingCoordinates.z;
        f5 = npcs[npcIndex].unk_1B;
        f6 = npcs[npcIndex].currentCoordinates.z + vec.z;

        if ((s16)(f4 + f5) >= (s16)f6 && (s16)f6 >= (s16)(f4 - f5)) {
            setEntityMovementVector(npcs[npcIndex].entityIndex, vec.x, vec.y, vec.z, npcs[npcIndex].speed);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075E28);

inline void func_80075E28(u8 npcIndex) {

    npcs[npcIndex].speed = 0;
    npcs[npcIndex].unk_21 = 10;
    npcs[npcIndex].unk_22 = 0;

    setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].animationIndex1);

    npcs[npcIndex].flags |= 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075EBC);

// FIXME: should be inline?
void func_80075EBC(u8 index) {
    
    u16 temp;
    
    if (!npcs[index].unk_21) {

        npcs[index].speed = 0;
        npcs[index].unk_22 = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, npcs[index].animationIndex1);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcs[index].direction = temp*2;
            npcs[index].unk_21 = 1;
        }

        npcs[index].flags |= 2;

        return;
    }
    
    if (npcs[index].direction & 1) {

        npcs[index].speed = 0;
        npcs[index].unk_22 = 0;
        npcs[index].unk_21 = 0;

    } else {

        npcs[index].speed = 1;
        npcs[index].unk_22 = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, npcs[index].animationIndex2);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npcs[index].unk_21 = 1;
            
        } else {
            npcs[index].unk_21 = 0;
        }
        
    }
    
    npcs[index].flags |= 2;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076080);

inline void func_80076080(u8 index, u8 arg1) {

    npcs[index].speed = 0;
    npcs[index].unk_21 = 10;
    npcs[index].unk_22 = 0;

    setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg1);

    npcs[index].flags |= 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076108);

void func_80076108(u8 index, u8 arg1, u8 arg2) {

    u16 temp;
    
    if (!npcs[index].unk_21) {

        npcs[index].speed = 0;
        npcs[index].unk_22 = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg1);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcs[index].direction = temp*2;
            npcs[index].unk_21 = 1;
        }

        npcs[index].flags |= 2;
        return;
    }
    
    if (npcs[index].direction & 1) {

        npcs[index].speed = 0;
        npcs[index].unk_22 = 0;
        npcs[index].unk_21 = 0;

    } else {

        npcs[index].speed = 1;
        npcs[index].unk_22 = 0;
        
        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg2);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npcs[index].unk_21 = 1;
        } else {
            npcs[index].unk_21 = 0;
        }
        
    }
    
    npcs[index].flags |= 2;
    
}



//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800762B4);

void func_800762B4(u8 npcIndex, u8 animationIndex1, u8 animationIndex2, u8 animationIndex3, u8 animationIndex4, u8 animationIndex5, u8 animationIndex6, u8 animationIndex7, u8 animationIndex8, u8 animationIndex9, u8 animationIndex10, u8 animationIndex11) {

    u16 temp;
    
    switch (npcs[npcIndex].unk_21) {

        case 0:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 10;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex1);

            temp = getRandomNumberInRange(0, 60);
            
            if (temp < 4) {
                npcs[npcIndex].direction = temp * 2;
                npcs[npcIndex].unk_21 = 1;
            } 
            
            if (temp == 4) {
                npcs[npcIndex].unk_21 = 7;
            } 
            
            if (temp == 5) {
                npcs[npcIndex].unk_21 = 2;
            } 

            if (temp == 6) {
                npcs[npcIndex].unk_21 = 3;
            } 
            
            if (temp == 7) {
                npcs[npcIndex].unk_21 = 4;
            }
        
            npcs[npcIndex].flags |= 2;
            break;

        case 1:

            if (npcs[npcIndex].direction & 1) {
                
                npcs[npcIndex].speed = 0;
                npcs[npcIndex].unk_22 = 0;
                npcs[npcIndex].unk_21 = 0;
                
            } else {
                
                npcs[npcIndex].speed = 1;
                npcs[npcIndex].unk_22 = 0;
                setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex2);
    
                if (getRandomNumberInRange(0, 19) < 8) {
                    npcs[npcIndex].unk_21 = 1;
                } else {
                    npcs[npcIndex].unk_21 = 0;
                }
                
            }

            npcs[npcIndex].flags |= 2;
            break;

        case 2:   
            
            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;

            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex3);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].unk_21 = 2;
            } else {
                npcs[npcIndex].unk_21 = 0;
            }
            
            break;

        case 3:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;

            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex4);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].unk_21 = 3;
            } else {
                npcs[npcIndex].unk_21 = 0;
            }
            
            break;

        case 4:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex5);

            npcs[npcIndex].unk_21 = 0;
            
            break;

        case 5:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex6);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].unk_21 = 5;
            } else {
                npcs[npcIndex].unk_21 = 10;
            }
            
            break;

        case 6:
            
            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex7);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].unk_21 = 6;
            } else {
                npcs[npcIndex].unk_21 = 10;
            }
            
            break;


        case 7:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex8);

            npcs[npcIndex].unk_21 = 10;
            
            break;

        case 8:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex9);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].unk_21 = 8;
            } else {
                npcs[npcIndex].unk_21 = 10;
            }
            
            break;

    case 9:

        npcs[npcIndex].speed = 0;
        npcs[npcIndex].unk_22 = 0;
        
        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex10);

        if (getRandomNumberInRange(0, 19) < 8) {
            npcs[npcIndex].unk_21 = 9;
        } else {
            npcs[npcIndex].unk_21 = 10;
        }
        
        break;


    case 10:

        npcs[npcIndex].speed = 0;
        npcs[npcIndex].unk_22 = 10;

        setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, animationIndex11);

        npcs[npcIndex].unk_21 = 10;

        temp = getRandomNumberInRange(0, 60);
        
        if (temp < 10) {
            npcs[npcIndex].unk_21 = 0;
        } 
        
        if (temp == 11) {
            npcs[npcIndex].unk_21 = 5;
        }
        
        if (temp == 12) {
            npcs[npcIndex].unk_21 = 6;
        } 
        
        if (temp == 13) {
            npcs[npcIndex].unk_21 = 8;
        } 
        
        if (temp == 14) {
            npcs[npcIndex].unk_21 = 9;
        }
    
        break;

    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076944);

void func_80076944(u8 npcIndex) {

    u16 temp;
    
    switch (npcs[npcIndex].unk_21) {

        case 0:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 0;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, 0);
            
            temp = getRandomNumberInRange(0, 60);

            if (temp < 8) {
                npcs[npcIndex].direction = temp;
                npcs[npcIndex].unk_21 = 1;
            }

            if (7 < temp && temp < 21) {
                npcs[npcIndex].unk_21 = 2;
            }

            npcs[npcIndex].flags |= 2;

            break;

        case 1:
            
            npcs[npcIndex].speed = 1;
            npcs[npcIndex].unk_22 = 0;
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, 8);

            if (getRandomNumberInRange(0, 19) < 8) {
                npcs[npcIndex].unk_21 = 1;
            } else {
                npcs[npcIndex].unk_21 = 0;
            }

            npcs[npcIndex].flags |= 2;

            break;

        case 2:

            npcs[npcIndex].speed = 0;
            npcs[npcIndex].unk_22 = 60;
            
            setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, 0x19);

            npcs[npcIndex].unk_21 = 1;
            npcs[npcIndex].flags |= 2;
            
            break;
            
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setNPCSpawningLocations);

void setNPCSpawningLocations(void) {
    
    // set girls' and baby's location
    func_80076F10();
    func_80077D14();
    func_80078BF0();
    func_80079A74();
    func_8007A9B0();
    func_8007B828();
    
    if (!checkDailyEventBit(FESTIVAL)) {
        
        // set rest of npcs' locations
        func_8007BD50();
        func_8007C088();
        func_8007C3D4();
        func_8007C7C4();
        func_8007CEB4();
        func_8007D3C4();
        func_8007D6E0();
        func_8007D9F4();
        func_8007DB38();
        setEllenLocation();
        func_8007DF14();
        func_8007E07C();
        func_8007E2E4();
        func_8007E440();
        func_8007E600();
        func_8007ECE4();
        func_8007F378();
        func_8007F4E0();
        func_8007F81C();
        func_8007FA44();
        func_8007FC0C();
        func_8007FCD8();
        func_8007FE40();
        func_8007FF04();
        func_800800D0();
        func_80080364();
        func_80080630();
        func_800807F8();
        func_800808B0();
        func_80080958();
        func_80080A10();
        func_80080B6C();
        func_80080C9C();
        func_80080DAC();
        func_80080EBC();
        func_80081030();
        func_80081228();
        func_80081420();
        func_80081618();
        func_80081810();
        func_80081A08();
        
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076CF4);

inline u8 func_80076CF4(void) {

    u8 result = 0;

    if (checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY)) {

        if (gBabyAge < 30) {
            
            if (5 < gHour && gHour < 20) {
            
                if (npcs[gWife].levelIndex != HOUSE) {
                    result = 1;
                } else {
                    result = 2;
                }
            
            } else {
                result = 3;
            }  

        } else {

            if (5 < gHour && gHour < 20) {
                // reference: -(120 < gBabyAge) & 6U | 4;
                result = (gBabyAge < 120) ? 4 : 6;
            } else {
                result = 5;
            }

        }
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076DCC);

u8 func_80076DCC(u8 npcIndex) {

    u8 result = 0; 
    
    switch (npcIndex) {

        case MARIA:

            if (checkLifeEventBit(MARIA_HARRIS_MARRIED) && checkLifeEventBit(MARIA_HARRIS_HAVE_BABY)) {
    
                if (mariaHarrisBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case POPURI:

            if (checkLifeEventBit(POPURI_GRAY_MARRIED) && checkLifeEventBit(POPURI_GRAY_HAVE_BABY)) {
    
                if (popuriGrayBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case ELLI:

            if (checkLifeEventBit(ELLI_JEFF_MARRIED) && checkLifeEventBit(ELLI_JEFF_HAVE_BABY)) {
    
                if (elliJeffBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case ANN:

            if (checkLifeEventBit(ANN_CLIFF_MARRIED) && checkLifeEventBit(ANN_CLIFF_HAVE_BABY)) {
    
                if (annCliffBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;

        case KAREN:

            if (checkLifeEventBit(KAREN_KAI_MARRIED) && checkLifeEventBit(KAREN_KAI_HAVE_BABY)) {
    
                if (karenKaiBabyAge < 30) {
                    result = 1;
                } else {
                    result = 2;
                }
                
            }

            break;
        
    }

    return result;
    
}



/* set starting locations */

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076F10);

void func_80076F10(void) {

     u8 result;

    npcs[MARIA].unk_1A = 0x40;
    npcs[MARIA].unk_1B = 0x40;
    npcs[MARIA].animationIndex1 = 0;
    npcs[MARIA].animationIndex2 = 8;

    if (!checkDailyEventBit(0x52) && !checkLifeEventBit(0xD9) && !checkDailyEventBit(0x56)) {

        if (checkLifeEventBit(MARRIED) && gWife == MARIA) {

            if (5 < gHour && gHour < 8) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = SOUTH;
                npcs[MARIA].unk_1E = 0;
                npcs[MARIA].startingCoordinates.x = -128.0f;
                npcs[MARIA].startingCoordinates.z = -64.0f;
                npcs[MARIA].flags |= 1;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (npcs[MARIA].location < 2) {

                        npcs[MARIA].levelIndex = CHURCH;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 0;
                        npcs[MARIA].startingCoordinates.x = -112.0f;
                        npcs[MARIA].startingCoordinates.z = -192.0f;
                        npcs[MARIA].flags |= 1;
                        
                    }  else {

                        npcs[MARIA].levelIndex = LIBRARY;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 0;
                        npcs[MARIA].startingCoordinates.x = 48.0f;
                        npcs[MARIA].startingCoordinates.z = -96.0f;
                        npcs[MARIA].flags |= 1;
                        
                    }
                    
                } else if (gWeather == SUNNY) {
                        
                    npcs[MARIA].levelIndex = FARM;
                    npcs[MARIA].startingCoordinates.y = 0.0f;
                    npcs[MARIA].direction = SOUTHWEST;
                    npcs[MARIA].unk_1E = 0;
                    npcs[MARIA].startingCoordinates.x = -256.0f;
                    npcs[MARIA].startingCoordinates.z = -240.0f;
                    npcs[MARIA].flags |= 1;
                    
                } else {

                    npcs[MARIA].levelIndex = HOUSE;
                    npcs[MARIA].startingCoordinates.x = 0.0f;
                    npcs[MARIA].startingCoordinates.y = 0.0f;
                    npcs[MARIA].startingCoordinates.z = 0.0f;
                    npcs[MARIA].direction = SOUTHWEST;
                    npcs[MARIA].unk_1E = 1;
                    npcs[MARIA].flags |= 1;
                    
                }
                
            }

            if (gHour == 12) {
                
                npcs[MARIA].levelIndex = KITCHEN;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = NORTHWEST;
                npcs[MARIA].unk_1E = 0;
                npcs[MARIA].startingCoordinates.x = -160.0f;
                npcs[MARIA].startingCoordinates.z = -64.0f;
                npcs[MARIA].flags |= 1;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.x = 0.0f;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].startingCoordinates.z = 0.0f;
                npcs[MARIA].direction = SOUTHWEST;
                npcs[MARIA].unk_1E = 1;
                npcs[MARIA].flags |= 1;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[MARIA].levelIndex = KITCHEN;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = NORTHWEST;
                npcs[MARIA].unk_1E = 0;
                npcs[MARIA].startingCoordinates.x = -160.0f;
                npcs[MARIA].startingCoordinates.z = -64.0f;
                npcs[MARIA].flags |= 1;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = SOUTHEAST;
                npcs[MARIA].unk_1E = 0;
                npcs[MARIA].startingCoordinates.x = -128.0f;
                npcs[MARIA].startingCoordinates.z = -112.0f;
                npcs[MARIA].flags |= 1;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[MARIA].levelIndex = HOUSE;
                npcs[MARIA].startingCoordinates.y = 0.0f;
                npcs[MARIA].direction = SOUTHWEST;
                npcs[MARIA].unk_1E = 4;
                npcs[MARIA].startingCoordinates.x = -192.0f;
                npcs[MARIA].startingCoordinates.z = -160.0f;
                npcs[MARIA].flags |= 1;
                
            }

            if (func_80076CF4() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[MARIA].animationIndex1 = 0x6C;
                npcs[MARIA].animationIndex2 = 0x76;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1A)) {

                switch (gDayOfWeek) {

                    case TUESDAY:
                    case WEDNESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:
                    case SUNDAY:

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[MARIA].levelIndex = LIBRARY;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].unk_1E = 0;
                            npcs[MARIA].startingCoordinates.x = 48.0f;
                            npcs[MARIA].startingCoordinates.z = -96.0f;
                            npcs[MARIA].flags |= 1;
        
                        }

                        break;

                    case MONDAY:

                        if (gWeather == SUNNY) {

                            if (npcs[MARIA].location < 2) {
                                
                                if (7 < gHour && gHour < 15) { 
                                
                                    npcs[MARIA].levelIndex = VILLAGE_1;
                                    npcs[MARIA].startingCoordinates.x = 0.0f;
                                    npcs[MARIA].startingCoordinates.y = 0.0f;
                                    npcs[MARIA].direction = SOUTHWEST;
                                    npcs[MARIA].unk_1E = 0;
                                    npcs[MARIA].startingCoordinates.z = -144.0f;
                                    npcs[MARIA].flags |= 1;
                                    
                                }
                                
                            } else {

                                if (7 < gHour && gHour < 17) { 
                                
                                    npcs[MARIA].levelIndex = VILLAGE_2;
                                    npcs[MARIA].startingCoordinates.x = 0.0f;
                                    npcs[MARIA].startingCoordinates.y = 0.0f;
                                    npcs[MARIA].startingCoordinates.z = 0.0f;
                                    npcs[MARIA].direction = SOUTHWEST;
                                    npcs[MARIA].unk_1E = 0;
                                    npcs[MARIA].flags |= 1;
                                    
                                }
                                
                            }
                            
                        } else {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[MARIA].levelIndex = MAYOR_HOUSE;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTHWEST;
                                npcs[MARIA].unk_1E = 0;
                                npcs[MARIA].startingCoordinates.x = -128.0f;
                                npcs[MARIA].startingCoordinates.z = -32.0f;
                                npcs[MARIA].flags |= 1;
        
                            }
                    
                        }
                        
                        break;
                    
                }

                if (gSeason == SUMMER &&  0 < gDayOfMonth && gDayOfMonth < 11) {
                    
                        npcs[MARIA].levelIndex = MAYOR_HOUSE;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 0;
                        npcs[MARIA].startingCoordinates.x = -128.0f;
                        npcs[MARIA].startingCoordinates.z = -32.0f;
                        npcs[MARIA].flags |= 1;
                    
                } 

                if (gSeason == WINTER && 0 < gDayOfMonth && gDayOfMonth < 11) {

                    if (npcs[MARIA].location < 2) {
                        
                        npcs[MARIA].levelIndex = MAYOR_HOUSE;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 0;
                        npcs[MARIA].startingCoordinates.x = -128.0f;
                        npcs[MARIA].startingCoordinates.z = -32.0f;
                        npcs[MARIA].flags |= 1;
                            
                    } else {

                        npcs[MARIA].levelIndex = CHURCH;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 0;
                        npcs[MARIA].startingCoordinates.x = -112.0f;
                        npcs[MARIA].startingCoordinates.z = -192.0f;
                        npcs[MARIA].flags |= 1;
                        
                    }
                    
                } 

                if (func_80076DCC(MARIA) == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                    npcs[MARIA].animationIndex1 = 0x6C;
                    npcs[MARIA].animationIndex2 = 0x76;
                }
                
            } else {

                if (gDayOfWeek == MONDAY) {
    
                    if (gWeather == SUNNY) {
                        
                        if (npcs[MARIA].location < 2) {
                            
                            if (7 < gHour && gHour < 15) {
                                
                                npcs[MARIA].levelIndex = VILLAGE_1;
                                npcs[MARIA].startingCoordinates.x = 0.0f;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTHWEST;
                                npcs[MARIA].unk_1E = 0;
                                npcs[MARIA].startingCoordinates.z = -144.0f;
                                npcs[MARIA].flags |= 1;
                                
                            } 
                            
                        } else if (9 < gHour && gHour < 17) {
                                    
                            npcs[MARIA].levelIndex = MOUNTAIN_1;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].unk_1E = 1;
                            npcs[MARIA].startingCoordinates.x = 128.0f;
                            npcs[MARIA].startingCoordinates.z = 128.0f;
                            npcs[MARIA].flags |= 1;
    
                            setSpecialDialogueBit(0x136);
                                
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[MARIA].levelIndex = MAYOR_HOUSE;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].unk_1E = 0;
                            npcs[MARIA].startingCoordinates.x = -128.0f;
                            npcs[MARIA].startingCoordinates.z = -32.0f;
                            npcs[MARIA].flags |= 1;
                            
                        }
                                                
                    }
                                      
                } else {
    
                    if (8 < gHour && gHour < 17) {
                        
                        npcs[MARIA].levelIndex = LIBRARY;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 0;
                        npcs[MARIA].startingCoordinates.x = 48.0f;
                        npcs[MARIA].startingCoordinates.z = -96.0f;
                        npcs[MARIA].flags |= 1;
                    
                    }
    
                }

                if (gSeason == SUMMER && 0 < gDayOfMonth && gDayOfMonth < 11) {
    
                    if (gWeather == SUNNY) {
                        
                        if (npcs[MARIA].location < 2) {
    
                            if (8 < gHour && gHour < 17) {
    
                                npcs[MARIA].levelIndex = MOUNTAIN_2;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTH;
                                npcs[MARIA].unk_1E = 0;
                                npcs[MARIA].startingCoordinates.x = -208.0f;
                                npcs[MARIA].startingCoordinates.z = -160.0f;
                                npcs[MARIA].flags |= 1;
        
                                setSpecialDialogueBit(0x136);
                                
                            }
                            
                        } else {
    
                            if (8 < gHour && gHour < 17) {
                               
                                npcs[MARIA].levelIndex = POND;
                                npcs[MARIA].startingCoordinates.y = 0.0f;
                                npcs[MARIA].direction = SOUTHEAST;
                                npcs[MARIA].unk_1E = 0;
                                npcs[MARIA].startingCoordinates.x = -96.0f;
                                npcs[MARIA].startingCoordinates.z = -32.0f;
                                npcs[MARIA].flags |= 1;
    
                                setSpecialDialogueBit(0x136);
                                
                            }
                            
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[MARIA].levelIndex = MAYOR_HOUSE;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].unk_1E = 0;
                            npcs[MARIA].startingCoordinates.x = -128.0f;
                            npcs[MARIA].startingCoordinates.z = -32.0f;
                            npcs[MARIA].flags |= 1;
                        
                        }
                        
                    }
                    
                }
    
                if (gSeason == WINTER && 0 < gDayOfMonth && gDayOfMonth < 11) {
    
                    if (npcs[MARIA].location < 2) {
    
                        if (8 < gHour && gHour < 17) { 
                        
                            npcs[MARIA].levelIndex = MAYOR_HOUSE;
                            npcs[MARIA].startingCoordinates.y = 0.0f;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].unk_1E = 0;
                            npcs[MARIA].startingCoordinates.x = -128.0f;
                            npcs[MARIA].startingCoordinates.z = -32.0f;
                            npcs[MARIA].flags |= 1;
                        
                        }
    
                    } else {
    
                        if (8 < gHour && gHour < 17) {
    
                            npcs[MARIA].levelIndex = CHURCH;
                            npcs[MARIA].startingCoordinates.y = 0;
                            npcs[MARIA].direction = SOUTHWEST;
                            npcs[MARIA].unk_1E = 0;
                            npcs[MARIA].startingCoordinates.x = -112.0f;
                            npcs[MARIA].startingCoordinates.z = -192.0f;
                            npcs[MARIA].flags |= 1;
                            
                        }
                        
                    }
                    
                }
    
                if (gSeason == SUMMER && gDayOfWeek == SUNDAY && gWeather == SUNNY && npcs[MARIA].location == 0) {
    
                    if (17 < gHour && gHour < 21) {
    
                        npcs[MARIA].levelIndex = BEACH;
                        npcs[MARIA].startingCoordinates.y = 0.0f;
                        npcs[MARIA].startingCoordinates.z = 0.0f;
                        npcs[MARIA].direction = SOUTHWEST;
                        npcs[MARIA].unk_1E = 2;
                        npcs[MARIA].startingCoordinates.x = -224.0f;
                        npcs[MARIA].flags |= 1;

                        setSpecialDialogueBit(0xB1);
                        
                    }
                    
                }
                
            }
            
        } 
        
    }

    npcs[MARIA].movingFlag = npcs[MARIA].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80077D14);

void func_80077D14(void) {

    u8 result;

    npcs[POPURI].unk_1A = 0x40;
    npcs[POPURI].unk_1B = 0x40;
    npcs[POPURI].animationIndex1 = 0;
    npcs[POPURI].animationIndex2 = 8;

    if (!checkDailyEventBit(0x52) && !checkLifeEventBit(0xD9) && !checkDailyEventBit(0x57)) {

        if (checkLifeEventBit(MARRIED) && gWife == POPURI) {

            if (5 < gHour && gHour < 8) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = SOUTH;
                npcs[POPURI].unk_1E = 0;
                npcs[POPURI].startingCoordinates.x = -128.0f;
                npcs[POPURI].startingCoordinates.z = -64.0f;
                npcs[POPURI].flags |= 1;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == MONDAY) {

                    if (npcs[ELLI].location < 2) {

                        if (8 < gHour && gHour < 17) {

                            if (gWeather == SUNNY) {
      
                                npcs[POPURI].levelIndex = MOUNTAIN_2;    
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHWEST;
                                npcs[POPURI].unk_1E = 1;
                                npcs[POPURI].startingCoordinates.x = -224.0f;
                                npcs[POPURI].startingCoordinates.z = -96.0f;
                                npcs[POPURI].flags |= 1;
        
                                setSpecialDialogueBit(0x91);
                                
                            } else {
                                
                                npcs[POPURI].levelIndex = KITCHEN;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHWEST;
                                npcs[POPURI].unk_1E = 1;
                                npcs[POPURI].startingCoordinates.x = -160.0f;
                                npcs[POPURI].startingCoordinates.z = -64.0f;
                                npcs[POPURI].flags |= 1;
                                                                
                            }
                            
                        }
                                              
                    } else {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;    
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].unk_1E = 0;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= 1;
                            
                        }

                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                        
                        npcs[POPURI].levelIndex = FARM;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHWEST;
                        npcs[POPURI].unk_1E = 0;
                        npcs[POPURI].startingCoordinates.x = 320.0f;
                        npcs[POPURI].startingCoordinates.z = -352.0f;
                        npcs[POPURI].flags |= 1;
                        
                    } else {
                            
                        npcs[POPURI].levelIndex = KITCHEN;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHWEST;
                        npcs[POPURI].unk_1E = 1;
                        npcs[POPURI].startingCoordinates.x = -160.0f;
                        npcs[POPURI].startingCoordinates.z = -64.0f;
                        npcs[POPURI].flags |= 1;
                                            
                    }
                    
                }
                
            }

            if (gHour == 12) {
                
                npcs[POPURI].levelIndex = KITCHEN;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = NORTHWEST;
                npcs[POPURI].unk_1E = 0;
                npcs[POPURI].startingCoordinates.x = -160.0f;
                npcs[POPURI].startingCoordinates.z = -64.0f;
                npcs[POPURI].flags |= 1;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.x = 0.0f;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].startingCoordinates.z = 0.0f;
                npcs[POPURI].direction = SOUTHWEST;
                npcs[POPURI].unk_1E = 1;
                npcs[POPURI].flags |= 1;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[POPURI].levelIndex = KITCHEN;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = NORTHWEST;
                npcs[POPURI].unk_1E = 0;
                npcs[POPURI].startingCoordinates.x = -160.0f;
                npcs[POPURI].startingCoordinates.z = -64.0f;
                npcs[POPURI].flags |= 1;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = SOUTHEAST;
                npcs[POPURI].unk_1E = 0;
                npcs[POPURI].startingCoordinates.x = -128.0f;
                npcs[POPURI].startingCoordinates.z = -112.0f;
                npcs[POPURI].flags |= 1;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[POPURI].levelIndex = HOUSE;
                npcs[POPURI].startingCoordinates.y = 0.0f;
                npcs[POPURI].direction = SOUTHWEST;
                npcs[POPURI].unk_1E = 4;
                npcs[POPURI].startingCoordinates.x = -192.0f;
                npcs[POPURI].startingCoordinates.z = -160.0f;
                npcs[POPURI].flags |= 1;
                
            }

            if (func_80076CF4() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[POPURI].animationIndex1 = 0x7A;
                npcs[POPURI].animationIndex2 = 0x84;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1B)) {

                switch (gDayOfWeek) {

                    case MONDAY:
                    case TUESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (8 < gHour && gHour < 17) {

                            if (gWeather == SUNNY) {
                                
                                npcs[POPURI].levelIndex = RANCH;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHEAST;
                                npcs[POPURI].startingCoordinates.x = -32.0f;
                                npcs[POPURI].startingCoordinates.z = -96.0f;
                                npcs[POPURI].flags |= 1;

                                if (func_80076DCC(POPURI) == 1) {
                                    npcs[POPURI].unk_1E = 1;
                                } else {
                                    npcs[POPURI].unk_1E = 3;
                                }

                                setSpecialDialogueBit(0x46);
                                                
                            } else {
                                
                                npcs[POPURI].levelIndex = RANCH_STORE;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = NORTHWEST;
                                npcs[POPURI].unk_1E = 0;
                                npcs[POPURI].startingCoordinates.x = 64.0f;
                                npcs[POPURI].startingCoordinates.z = 64.0f;
                                npcs[POPURI].flags |= 1;  
                            }
                        
                        }

                        break;

                    case SUNDAY:
                    case WEDNESDAY:
    
                        npcs[POPURI].levelIndex = RANCH_STORE;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = NORTHWEST;
                        npcs[POPURI].unk_1E = 0;
                        npcs[POPURI].startingCoordinates.x = 64.0f;
                        npcs[POPURI].startingCoordinates.z = 64.0f;
                        npcs[POPURI].flags |= 1;  
                        break;
                    
                }

                if (func_80076DCC(POPURI) == 1) {
                    npcs[POPURI].animationIndex1 = 0x7A;
                    npcs[POPURI].animationIndex2 = 0x84;
                }
                
            } else {

                if (SUNDAY < gDayOfWeek && gDayOfWeek < FRIDAY) {

                    if (gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = VILLAGE_1;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHWEST;
                            npcs[POPURI].unk_1E = 3;
                            npcs[POPURI].startingCoordinates.x = 96.0f;
                            npcs[POPURI].startingCoordinates.z = -224.0f;
                            npcs[POPURI].flags |= 1;  
                                
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].unk_1E = 0;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= 1;  
                            
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == FRIDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[POPURI].location < 2) {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = MOUNTAIN_2;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHWEST;
                                npcs[POPURI].unk_1E = 1;
                                npcs[POPURI].startingCoordinates.x = -224.0f;
                                npcs[POPURI].startingCoordinates.z = -128.0f;
                                npcs[POPURI].flags |= 1;  

                                setSpecialDialogueBit(0x91);
                                
                            }

                        } else {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = MOON_MOUNTAIN;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].startingCoordinates.z = 0.0f;
                                npcs[POPURI].direction = NORTHEAST;
                                npcs[POPURI].unk_1E = 0;
                                npcs[POPURI].startingCoordinates.x = 64.0f;
                                npcs[POPURI].flags |= 1;  

                                setSpecialDialogueBit(0x92);
                                
                            }
                            
                        }
                        
                    } else {

                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].unk_1E = 0;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= 1;  
                            
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == SATURDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[POPURI].location < 2) {

                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = MOUNTAIN_2;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHEAST;
                                npcs[POPURI].unk_1E = 1;
                                npcs[POPURI].startingCoordinates.x = -128.0f;
                                npcs[POPURI].startingCoordinates.z = -416.0f;
                                npcs[POPURI].flags |= 1;  

                                setSpecialDialogueBit(0x91);
                                
                            }

                        } else {
                          
                            if (8 < gHour && gHour < 17) {
                                
                                npcs[POPURI].levelIndex = POND;
                                npcs[POPURI].startingCoordinates.y = 0.0f;
                                npcs[POPURI].direction = SOUTHEAST;
                                npcs[POPURI].unk_1E = 0;
                                npcs[POPURI].startingCoordinates.x = -224.0f;
                                npcs[POPURI].startingCoordinates.z = -96.0f;
                                npcs[POPURI].flags |= 1;  

                            }      
                            
                        } 
                        
                    } else {
                        
                        if (8 < gHour && gHour < 17) {

                            npcs[POPURI].levelIndex = FLOWER_SHOP;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].unk_1E = 0;
                            npcs[POPURI].startingCoordinates.x = -112.0f;
                            npcs[POPURI].startingCoordinates.z = 64.0f;
                            npcs[POPURI].flags |= 1;  
                            
                        }
                    }
                    
                }

                if (gDayOfWeek == SUNDAY) {

                    if (!(npcs[POPURI].location < 2) && gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[POPURI].levelIndex = RANCH;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].direction = SOUTHEAST;
                            npcs[POPURI].unk_1E = 0;
                            npcs[POPURI].startingCoordinates.x = -32.0f;
                            npcs[POPURI].startingCoordinates.z = -96.0f;
                            npcs[POPURI].flags |= 1;
    
                            setSpecialDialogueBit(0x46);
                            
                        }
                                            
                    } else {

                        if (8 < gHour && gHour < 17) {
                            
                            npcs[POPURI].levelIndex = BAKERY;
                            npcs[POPURI].startingCoordinates.x = 0.0f;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].startingCoordinates.z = 0.0f;
                            npcs[POPURI].direction = SOUTHWEST;
                            npcs[POPURI].unk_1E = 0;
                            npcs[POPURI].flags |= 1;
    
                            setSpecialDialogueBit(0x93);
                            
                        }
                        
                    }
                    
                }

                if (gSeason == SUMMER && gDayOfWeek == THURSDAY) {

                    if (gWeather == SUNNY && npcs[POPURI].location == 0) {

                        if (18 < gHour && gHour < 21) {
    
                            npcs[POPURI].levelIndex = BEACH;
                            npcs[POPURI].startingCoordinates.y = 0.0f;
                            npcs[POPURI].startingCoordinates.z = 0.0f;
                            npcs[POPURI].direction = SOUTHWEST;
                            npcs[POPURI].unk_1E = 2;
                            npcs[POPURI].startingCoordinates.x = -224.0f;
                            npcs[POPURI].flags |= 1;
    
                            setSpecialDialogueBit(0xB4);
                            
                        }
                    
                        
                    }
                    
                }

                if (gWeather == 4 && npcs[POPURI].location == 0) {

                    if (8 < gHour && gHour < 17) {
                        
                        npcs[POPURI].levelIndex = MOUNTAIN_2;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHWEST;
                        npcs[POPURI].unk_1E = 0;
                        npcs[POPURI].startingCoordinates.x = -64.0f;
                        npcs[POPURI].startingCoordinates.z = -160.0f;
                        npcs[POPURI].flags |= 1;  

                        setSpecialDialogueBit(0x91);
                                   
                    }
                    
                }

                if (gSeason == WINTER && 24 < gDayOfMonth && gDayOfMonth < 31) {

                    if (8 < gHour && gHour < 17) {
       
                        npcs[POPURI].levelIndex = FLOWER_SHOP;
                        npcs[POPURI].startingCoordinates.y = 0.0f;
                        npcs[POPURI].direction = SOUTHEAST;
                        npcs[POPURI].unk_1E = 0;
                        npcs[POPURI].startingCoordinates.x = -112.0f;
                        npcs[POPURI].startingCoordinates.z = 64.0f;
                        npcs[POPURI].flags |= 1;  
                        
                    }
                                     
                }
                
            } 
        
        }
        
    }

    npcs[POPURI].movingFlag = npcs[POPURI].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80078BF0);

void func_80078BF0(void) {

    u8 result;

    npcs[ELLI].unk_1A = 0x40;
    npcs[ELLI].unk_1B = 0x40;
    npcs[ELLI].animationIndex1 = 0;
    npcs[ELLI].animationIndex2 = 8;

    if (!checkDailyEventBit(0x52) && !checkLifeEventBit(0xD9) && !checkDailyEventBit(0x58)) {

        if (checkLifeEventBit(MARRIED) && gWife == ELLI) {

            if (5 < gHour && gHour < 8) {

                npcs[ELLI].levelIndex = HOUSE;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTH;
                npcs[ELLI].unk_1E = 0;
                npcs[ELLI].startingCoordinates.x = -128.0f;
                npcs[ELLI].startingCoordinates.z = -64.0f;
                npcs[ELLI].flags |= 1;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (npcs[ELLI].location < 2) {
                                                    
                        npcs[ELLI].levelIndex = BAKERY;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].unk_1E = 0;
                        npcs[ELLI].startingCoordinates.x = -32.0f;
                        npcs[ELLI].startingCoordinates.z = 16.0f;
                        npcs[ELLI].flags |= 1;  

                    } else if (gWeather == SUNNY) {
                        
                        npcs[ELLI].levelIndex = MOUNTAIN_2;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = NORTHWEST;
                        npcs[ELLI].unk_1E = 0;
                        npcs[ELLI].startingCoordinates.x = 96.0f;
                        npcs[ELLI].startingCoordinates.z = -64.0f;
                        npcs[ELLI].flags |= 1;  
        
                        setSpecialDialogueBit(0x97);
                                
                    } else {
                        
                        npcs[ELLI].levelIndex = KITCHEN;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].unk_1E = 1;
                        npcs[ELLI].startingCoordinates.x = -160.0f;
                        npcs[ELLI].startingCoordinates.z = -64.0f;
                        npcs[ELLI].flags |= 1;
                        
                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                                                
                        npcs[ELLI].levelIndex = FARM;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].startingCoordinates.z = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].unk_1E = 0;
                        npcs[ELLI].startingCoordinates.x = -448.0f;
                        npcs[ELLI].flags |= 1;
                        
                    } else {
                                                
                        npcs[ELLI].levelIndex = KITCHEN;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].unk_1E = 1;
                        npcs[ELLI].startingCoordinates.x = -160.0f;
                        npcs[ELLI].startingCoordinates.z = -64.0f;
                        npcs[ELLI].flags |= 1;
                        
                    }
                                        
                }
                
            }

            if (gHour == 12) {
                
                npcs[ELLI].levelIndex = KITCHEN;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = NORTHWEST;
                npcs[ELLI].unk_1E = 1;
                npcs[ELLI].startingCoordinates.x = -128.0f;
                npcs[ELLI].startingCoordinates.z = -128.0f;
                npcs[ELLI].flags |= 1;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[ELLI].levelIndex = KITCHEN;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTHWEST;
                npcs[ELLI].unk_1E = 1;
                npcs[ELLI].startingCoordinates.x = -160.0f;
                npcs[ELLI].startingCoordinates.z = -64.0f;
                npcs[ELLI].flags |= 1;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[ELLI].levelIndex = KITCHEN;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = NORTHWEST;
                npcs[ELLI].unk_1E = 0;
                npcs[ELLI].startingCoordinates.x = -160.0f;
                npcs[ELLI].startingCoordinates.z = -64.0f;
                npcs[ELLI].flags |= 1;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[ELLI].levelIndex = HOUSE;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTHEAST;
                npcs[ELLI].unk_1E = 0;
                npcs[ELLI].startingCoordinates.x = -128.0f;
                npcs[ELLI].startingCoordinates.z = -112.0f;
                npcs[ELLI].flags |= 1;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[ELLI].levelIndex = HOUSE;
                npcs[ELLI].startingCoordinates.y = 0.0f;
                npcs[ELLI].direction = SOUTHWEST;
                npcs[ELLI].unk_1E = 4;
                npcs[ELLI].startingCoordinates.x = -192.0f;
                npcs[ELLI].startingCoordinates.z = -160.0f;
                npcs[ELLI].flags |= 1;
                
            }

            if (func_80076CF4() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[ELLI].animationIndex1 = 0x69;
                npcs[ELLI].animationIndex2 = 0x73;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1C)) {

                if (gWeather == SUNNY) {

                    switch (gDayOfWeek) {

                        case MONDAY:

                            switch (gSeason) {

                                case SPRING:
                                case SUMMER:
                                    
                                    if (5 < gHour && gHour < 18) {
                            
                                        npcs[ELLI].levelIndex = MOUNTAIN_2;
                                        npcs[ELLI].startingCoordinates.y = 0.0f;
                                        npcs[ELLI].direction = NORTHWEST;
                                        npcs[ELLI].unk_1E = 0;
                                        npcs[ELLI].startingCoordinates.x = 96.0f;
                                        npcs[ELLI].startingCoordinates.z = -64.0f;
                                        npcs[ELLI].flags |= 1;  
                
                                        setSpecialDialogueBit(0x97);
                                        
                                    }            
                                            
                                    break;
                                
                                case WINTER:
                                    
                                    if (5 < gHour && gHour < 18) {
                
                                        npcs[ELLI].levelIndex = BAKERY;
                                        npcs[ELLI].startingCoordinates.y = 0.0f;
                                        npcs[ELLI].direction = SOUTHWEST;
                                        npcs[ELLI].unk_1E = 1;
                                        npcs[ELLI].startingCoordinates.x = -32.0f;
                                        npcs[ELLI].startingCoordinates.z = 16.0f;
                                        npcs[ELLI].flags |= 1;  
                                    
                                    }
                                    
                                    break;
                                
                                case AUTUMN:

                                     if (5 < gHour && gHour < 18) {
                                
                                        npcs[ELLI].levelIndex = MOUNTAIN_1;
                                        npcs[ELLI].startingCoordinates.y = 0.0f;
                                        npcs[ELLI].direction = SOUTHWEST;
                                        npcs[ELLI].unk_1E = 0;
                                        npcs[ELLI].startingCoordinates.x = 112.0f;
                                        npcs[ELLI].startingCoordinates.z = -32.0f;
                                        npcs[ELLI].flags |= 1;  
                
                                        setSpecialDialogueBit(0x96);
                                        
                                    }       

                                    break;
                                
                            }

                            break;

                        case WEDNESDAY:
                            
                            if (8 < gHour && gHour < 12) {
                                
                                npcs[ELLI].levelIndex = FLOWER_SHOP;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].unk_1E = 0;
                                npcs[ELLI].startingCoordinates.x = 32.0f;
                                npcs[ELLI].startingCoordinates.z = -16.0f;
                                npcs[ELLI].flags |= 1;  
        
                                setSpecialDialogueBit(0x98);
        
                            }

                            if (11 < gHour && gHour < 17) {
    
                                npcs[ELLI].levelIndex = VILLAGE_1;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = NORTHWEST;
                                npcs[ELLI].unk_1E = 0;
                                npcs[ELLI].startingCoordinates.x = 352.0f;
                                npcs[ELLI].startingCoordinates.z = 96.0f;
                                npcs[ELLI].flags |= 1;  
                                
                            }

                            break;

                        case SUNDAY:
                        case TUESDAY:
                        case THURSDAY:
                        case FRIDAY:
                        case SATURDAY:

                            if (5 < gHour && gHour < 18) {

                                npcs[ELLI].levelIndex = BAKERY;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].unk_1E = 1;
                                npcs[ELLI].startingCoordinates.x = -32.0f;
                                npcs[ELLI].startingCoordinates.z = 16.0f;
                                npcs[ELLI].flags |= 1;  
                                
                            }
                            
                            break;
                        
                    }
                    
                } else {
                    
                    if (5 < gHour && gHour < 18) {

                        npcs[ELLI].levelIndex = BAKERY;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].unk_1E = 1;
                        npcs[ELLI].startingCoordinates.x = -32.0f;
                        npcs[ELLI].startingCoordinates.z = 16.0f;
                        npcs[ELLI].flags |= 1;  
                    
                    }
                    
                }

                if (func_80076DCC(ELLI) == 1) {
                    npcs[ELLI].animationIndex1 = 0x69;
                    npcs[ELLI].animationIndex2 = 0x73;
                }
                
            } else {

                switch (gDayOfWeek) {
                    
                    case TUESDAY:
                    case THURSDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (gWeather == SUNNY) {
                            
                            if (8 < gHour && gHour < 17) {
                            
                                npcs[ELLI].levelIndex = BAKERY;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].unk_1E = 1;
                                npcs[ELLI].startingCoordinates.x = -32.0f;
                                npcs[ELLI].startingCoordinates.z = 16.0f;
                                npcs[ELLI].flags |= 1;  
        
                            } 
                            
                        } else {
                                                    
                            if (8 < gHour && gHour < 17) {
                                
                                npcs[ELLI].levelIndex = BAKERY;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].unk_1E = 0;
                                npcs[ELLI].startingCoordinates.x = -32.0f;
                                npcs[ELLI].startingCoordinates.z = 16.0f;
                                npcs[ELLI].flags |= 1;  
        
                            } 
                            
                        }
                        
                        break;
                    
                }

                if (gDayOfWeek == WEDNESDAY) {

                    if (8 < gHour && gHour < 12) {
                        
                        npcs[ELLI].levelIndex = FLOWER_SHOP;
                        npcs[ELLI].startingCoordinates.y = 0.0f;
                        npcs[ELLI].direction = SOUTHWEST;
                        npcs[ELLI].unk_1E = 0;
                        npcs[ELLI].startingCoordinates.x = 32.0f;
                        npcs[ELLI].startingCoordinates.z = -16.0f;
                        npcs[ELLI].flags |= 1;  

                        setSpecialDialogueBit(0x98);

                    }

                    if (11 < gHour && gHour < 17) {

                        if (gWeather == SUNNY) {
                                                    
                            npcs[ELLI].levelIndex = VILLAGE_1;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = NORTHWEST;
                            npcs[ELLI].unk_1E = 0;
                            npcs[ELLI].startingCoordinates.x = 352.0f;
                            npcs[ELLI].startingCoordinates.z = 96.0f;
                            npcs[ELLI].flags |= 1;  

                        } else {
                            
                            npcs[ELLI].levelIndex = BAKERY;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = SOUTHWEST;
                            npcs[ELLI].unk_1E = 0;
                            npcs[ELLI].startingCoordinates.x = -32.0f;
                            npcs[ELLI].startingCoordinates.z = 16.0f;
                            npcs[ELLI].flags |= 1;  
    
                        }
                        
                    }
                    
                }

                if (gDayOfWeek == MONDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[ELLI].location < 2) {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ELLI].levelIndex = MOUNTAIN_2;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = NORTHWEST;
                                npcs[ELLI].unk_1E = 0;
                                npcs[ELLI].startingCoordinates.x = 96.0f;
                                npcs[ELLI].startingCoordinates.z = -64.0f;
                                npcs[ELLI].flags |= 1;  
        
                                setSpecialDialogueBit(0x97);
                                
                            }            
                            
                        } else {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ELLI].levelIndex = MOUNTAIN_1;
                                npcs[ELLI].startingCoordinates.y = 0.0f;
                                npcs[ELLI].direction = SOUTHWEST;
                                npcs[ELLI].unk_1E = 0;
                                npcs[ELLI].startingCoordinates.x = 112.0f;
                                npcs[ELLI].startingCoordinates.z = -32.0f;
                                npcs[ELLI].flags |= 1;  
        
                                setSpecialDialogueBit(0x96);
                                
                            }       
                            
                            
                        }
                        
                    } else {
                             
                        if (8 < gHour && gHour < 17) {
                            
                            npcs[ELLI].levelIndex = BAKERY;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = SOUTHWEST;
                            npcs[ELLI].unk_1E = 0;
                            npcs[ELLI].startingCoordinates.x = -32.0f;
                            npcs[ELLI].startingCoordinates.z = 16.0f;
                            npcs[ELLI].flags |= 1;  
    
                        }             

                    }
                    
                }

                if (gSeason == SUMMER) {

                    if (gDayOfWeek == WEDNESDAY && gWeather == SUNNY && npcs[ELLI].location == 0) {

                        if (18 < gHour && gHour < 21) {
                            
                            npcs[ELLI].levelIndex = BEACH;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].startingCoordinates.z = 0.0f;
                            npcs[ELLI].direction = SOUTHWEST;
                            npcs[ELLI].unk_1E = 2;
                            npcs[ELLI].startingCoordinates.x = -224.0f;
                            npcs[ELLI].flags |= 1;  
    
                            setSpecialDialogueBit(0x99);
                            
                        }
                        
                    }

                    if (gSeason == SUMMER && gDayOfWeek == MONDAY && gWeather == SUNNY) {
    
                        if (7 < gHour && gHour < 17) {
           
                            npcs[ELLI].levelIndex = BEACH;
                            npcs[ELLI].startingCoordinates.y = 0.0f;
                            npcs[ELLI].direction = NORTHEAST;
                            npcs[ELLI].unk_1E = 0;
                            npcs[ELLI].startingCoordinates.x = -128.0f;
                            npcs[ELLI].startingCoordinates.z = 160.0f;
                            npcs[ELLI].flags |= 1;  
    
                            setSpecialDialogueBit(0x99);
                            
                        }
                                         
                    }
                    
                }
                
            } 
        
        }
        
    }

    npcs[ELLI].movingFlag = npcs[ELLI].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80079A74);

void func_80079A74(void) {
u8 result;

    npcs[ANN].unk_1A = 0x40;
    npcs[ANN].unk_1B = 0x40;
    npcs[ANN].animationIndex1 = 0;
    npcs[ANN].animationIndex2 = 8;

    if (!checkDailyEventBit(0x52) && !checkLifeEventBit(0xD9) && !checkDailyEventBit(0x59)) {

        if (checkLifeEventBit(MARRIED) && gWife == ANN) {

            if (5 < gHour && gHour < 8) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = SOUTH;
                npcs[ANN].unk_1E = 0;
                npcs[ANN].startingCoordinates.x = -128.0f;
                npcs[ANN].startingCoordinates.z = -64.0f;
                npcs[ANN].flags |= 1;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    if (npcs[ANN].location < 2) {
                                                    
                        npcs[ANN].levelIndex = RICK_STORE;
                        npcs[ANN].startingCoordinates.x = 0.0f;
                        npcs[ANN].startingCoordinates.y = 0.0f;
                        npcs[ANN].startingCoordinates.z = 0.0f;
                        npcs[ANN].direction = SOUTHWEST;
                        npcs[ANN].unk_1E = 0;
                        npcs[ANN].flags |= 1;  

                        setSpecialDialogueBit(0xA1);

                    } else  {

                        if (gWeather == SUNNY) {
                            
                            npcs[ANN].levelIndex = RANCH;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].unk_1E = 1;
                            npcs[ANN].startingCoordinates.x = -64.0f;
                            npcs[ANN].startingCoordinates.z = 128.0f;
                            npcs[ANN].flags |= 1;  
                            
                        } else if (func_8009B374()) {

                            npcs[ANN].levelIndex = BARN;
                            npcs[ANN].startingCoordinates.x = 0.0f;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].startingCoordinates.z = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].unk_1E = 1;
                            npcs[ANN].flags |= 1;
                            
                        } else {

                            if (func_8009B564()) {

                                npcs[ANN].levelIndex = COOP;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].flags |= 1;
                                
                            } else {
                                
                                npcs[ANN].levelIndex = HOUSE;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].flags |= 1;
                                
                            }
                            
                        }
                        
                    }
                    
                } else {

                    if (gWeather == SUNNY) {
                                                
                        npcs[ANN].levelIndex = FARM;
                        npcs[ANN].startingCoordinates.y = 0.0f;
                        npcs[ANN].direction = SOUTHWEST;
                        npcs[ANN].unk_1E = 0;
                        npcs[ANN].startingCoordinates.x = -256.0f;
                        npcs[ANN].startingCoordinates.z = -320.0f;
                        npcs[ANN].flags |= 1;
                        
                    } else {
                                                
                        if (func_8009B374()) {

                            npcs[ANN].levelIndex = BARN;
                            npcs[ANN].startingCoordinates.x = 0.0f;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].startingCoordinates.z = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].unk_1E = 1;
                            npcs[ANN].flags |= 1;
                            
                        } else {

                            if (func_8009B564()) {

                                npcs[ANN].levelIndex = COOP;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].flags |= 1;
                                
                            } else {
                                
                                npcs[ANN].levelIndex = HOUSE;
                                npcs[ANN].startingCoordinates.x = 0.0f;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].startingCoordinates.z = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].flags |= 1;
                                
                            }
                            
                        }
                        
                    }
                                        
                }
                
            }

            if (gHour == 12) {
                
                npcs[ANN].levelIndex = KITCHEN;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = NORTHWEST;
                npcs[ANN].unk_1E = 0;
                npcs[ANN].startingCoordinates.x = -160.0f;
                npcs[ANN].startingCoordinates.z = -64.0f;
                npcs[ANN].flags |= 1;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.x = 0.0f;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].startingCoordinates.z = 0.0f;
                npcs[ANN].direction = SOUTHWEST;
                npcs[ANN].unk_1E = 1;
                npcs[ANN].flags |= 1;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[ANN].levelIndex = KITCHEN;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = NORTHWEST;
                npcs[ANN].unk_1E = 0;
                npcs[ANN].startingCoordinates.x = -160.0f;
                npcs[ANN].startingCoordinates.z = -64.0f;
                npcs[ANN].flags |= 1;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = SOUTHEAST;
                npcs[ANN].unk_1E = 0;
                npcs[ANN].startingCoordinates.x = -128.0f;
                npcs[ANN].startingCoordinates.z = -112.0f;
                npcs[ANN].flags |= 1;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[ANN].levelIndex = HOUSE;
                npcs[ANN].startingCoordinates.y = 0.0f;
                npcs[ANN].direction = SOUTHWEST;
                npcs[ANN].unk_1E = 4;
                npcs[ANN].startingCoordinates.x = -192.0f;
                npcs[ANN].startingCoordinates.z = -160.0f;
                npcs[ANN].flags |= 1;
                
            }

            if (func_80076CF4() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[ANN].animationIndex1 = 0x6F;
                npcs[ANN].animationIndex2 = 0x79;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1D)) {

                switch (gDayOfWeek) {

                    case SUNDAY:
                    case MONDAY:
                    case TUESDAY:
                    case WEDNESDAY:
                    case FRIDAY:
                    case SATURDAY:

                        if (gWeather == SUNNY) {
                            
                            if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].startingCoordinates.x = -64.0f;
                                npcs[ANN].startingCoordinates.z = 128.0f;
                                npcs[ANN].flags |= 1;  
                                
                            }
                            
                        } else {

                            if (7 < gHour && gHour < 17) {
                                
                                npcs[ANN].levelIndex = RANCH_HOUSE;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHEAST;
                                npcs[ANN].unk_1E = 0;
                                npcs[ANN].startingCoordinates.x = -192.0f;
                                npcs[ANN].startingCoordinates.z = -32.0f;
                                npcs[ANN].flags |= 1;  
                                
                            }
                            
                        }

                        break;

                    case THURSDAY:

                        if (gWeather == SUNNY) {

                            if (8 < gHour && gHour < 17) {

                                switch (gSeason) {

                                    case SPRING:
                                    case AUTUMN:
                                        npcs[ANN].levelIndex = MOUNTAIN_2;
                                        npcs[ANN].startingCoordinates.y = 0.0f;
                                        npcs[ANN].direction = SOUTHWEST;
                                        npcs[ANN].unk_1E = 0;
                                        npcs[ANN].startingCoordinates.x = -64.0f;
                                        npcs[ANN].startingCoordinates.z = -160.0f;
                                        npcs[ANN].flags |= 1;  
                
                                        setSpecialDialogueBit(0x9E);

                                        break;

                                    case SUMMER:
                                        
                                        npcs[ANN].levelIndex = CAVE;
                                        npcs[ANN].startingCoordinates.y = 0.0f;
                                        npcs[ANN].direction = SOUTHWEST;
                                        npcs[ANN].unk_1E = 0;
                                        npcs[ANN].startingCoordinates.x = 64.0f;
                                        npcs[ANN].startingCoordinates.z = -224.0f;
                                        npcs[ANN].flags |= 1;  
                
                                        setSpecialDialogueBit(0x9E);

                                        break;

                                    case WINTER:
                                                                                
                                        npcs[ANN].levelIndex = RANCH_HOUSE;
                                        npcs[ANN].startingCoordinates.y = 0.0f;
                                        npcs[ANN].direction = SOUTHEAST;
                                        npcs[ANN].unk_1E = 0;
                                        npcs[ANN].startingCoordinates.x = -192.0f;
                                        npcs[ANN].startingCoordinates.z = -32.0f;
                                        npcs[ANN].flags |= 1;  
                                        
                                        break;
                                    
                                    }
                                
                            }
                            
                        } else if (7 < gHour && gHour < 17) {
                            
                            npcs[ANN].levelIndex = RANCH_BARN;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].unk_1E = 0;
                            npcs[ANN].startingCoordinates.x = -128.0f;
                            npcs[ANN].startingCoordinates.z = -292.0f;
                            npcs[ANN].flags |= 1;  
    
                        }

                        break;
                    
                }
                
                if (func_80076DCC(ANN) == 1) {
                    npcs[ANN].animationIndex1 = 0x6F;
                    npcs[ANN].animationIndex2 = 0x79;
                }
                
            } else {

                switch (gDayOfWeek) {

                    case MONDAY:
                    case TUESDAY:
                    case WEDNESDAY:
                    case FRIDAY:
                    case SATURDAY:
                        
                        if (gWeather == SUNNY) {
                           
                            if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].startingCoordinates.x = -64.0f;
                                npcs[ANN].startingCoordinates.z = 128.0f;
                                npcs[ANN].flags |= 1;  
                                
                            }
                            
                        } else {
                            
                            if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH_BARN;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 0;
                                npcs[ANN].startingCoordinates.x = -128.0f;
                                npcs[ANN].startingCoordinates.z = -292.0f;
                                npcs[ANN].flags |= 1;  
        
                            }
                            
                        }
                        
                        break;
                    
                }

                if (gDayOfWeek == THURSDAY) {

                    if (gWeather == SUNNY) {

                        switch (npcs[ANN].location) {

                            case 0:

                                if (8 < gHour && gHour < 17) {
                        
                                    npcs[ANN].levelIndex = MOUNTAIN_1;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].unk_1E = 0;
                                    npcs[ANN].startingCoordinates.x = 128.0f;
                                    npcs[ANN].startingCoordinates.z = 32.0f;
                                    npcs[ANN].flags |= 1;  
            
                                    setSpecialDialogueBit(0x9E);
                                    
                                }        
                                
                                break;

                            case 1:
                                
                                if (8 < gHour && gHour < 17) {
                                
                                    npcs[ANN].levelIndex = MOUNTAIN_2;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].unk_1E = 0;
                                    npcs[ANN].startingCoordinates.x = -64.0f;
                                    npcs[ANN].startingCoordinates.z = -160.0f;
                                    npcs[ANN].flags |= 1;  
            
                                    setSpecialDialogueBit(0x9E);
                                    
                                }            
                                break;
                            
                            case 2:

                                if (8 < gHour && gHour < 17) {
                        
                                    npcs[ANN].levelIndex = VINEYARD;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].unk_1E = 0;
                                    npcs[ANN].startingCoordinates.x = 16.0f;
                                    npcs[ANN].startingCoordinates.z = -144.0f;
                                    npcs[ANN].flags |= 1;  
            
                                    setSpecialDialogueBit(0xA0);
                                    
                                }        
                                
                                break;

                            
                            case 3:

                                if (8 < gHour && gHour < 17) {
                                    
                                    npcs[ANN].levelIndex = RICK_STORE;
                                    npcs[ANN].startingCoordinates.x = 0.0f;
                                    npcs[ANN].startingCoordinates.y = 0.0f;
                                    npcs[ANN].startingCoordinates.z = 0.0f;
                                    npcs[ANN].direction = SOUTHWEST;
                                    npcs[ANN].unk_1E = 0;
                                    npcs[ANN].flags |= 1;  
            
                                    setSpecialDialogueBit(0xA1);
                                    
                                }
                            
                        }
                            
                        
                            
                        
                        
                    } else {
                        
                       if (8 < gHour && gHour < 17) {
                        
                            npcs[ANN].levelIndex = RANCH_STORE;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].unk_1E = 0;
                            npcs[ANN].startingCoordinates.x = 80.0f;
                            npcs[ANN].startingCoordinates.z = -144.0f;
                            npcs[ANN].flags |= 1;  
    
                                
                        }      
                        
                    }
                   
                }

                if (gDayOfWeek == SUNDAY) {

                    if (gWeather == SUNNY) {

                        if (npcs[ANN].location < 2) {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = POND;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 0;
                                npcs[ANN].startingCoordinates.x = 80.0f;
                                npcs[ANN].startingCoordinates.z = -176.0f;
                                npcs[ANN].flags |= 1;  
        
                            }            
                            
                        } else {
                            
                             if (7 < gHour && gHour < 17) {
                            
                                npcs[ANN].levelIndex = RANCH;
                                npcs[ANN].startingCoordinates.y = 0.0f;
                                npcs[ANN].direction = SOUTHWEST;
                                npcs[ANN].unk_1E = 1;
                                npcs[ANN].startingCoordinates.x = 256.0f;
                                npcs[ANN].startingCoordinates.z = 128.0f;
                                npcs[ANN].flags |= 1;  
        
                            }       
                            
                        }
                        
                    } else {
                             
                        if (8 < gHour && gHour < 17) {
                            
                            npcs[ANN].levelIndex = RANCH_BARN;
                            npcs[ANN].startingCoordinates.y = 0.0f;
                            npcs[ANN].direction = SOUTHWEST;
                            npcs[ANN].unk_1E = 0;
                            npcs[ANN].startingCoordinates.x = -128.0f;
                            npcs[ANN].startingCoordinates.z = -292.0f;
                            npcs[ANN].flags |= 1;  
    
                        }             
                    }
                    
                }


                if (gSeason == SUMMER && gDayOfWeek == TUESDAY && gWeather == SUNNY && npcs[ANN].location == 0) {

                    if (17 < gHour && gHour < 21) {
       
                        npcs[ANN].levelIndex = BEACH;
                        npcs[ANN].startingCoordinates.y = 0.0f;
                        npcs[ANN].startingCoordinates.z = 0.0f;
                        npcs[ANN].direction = SOUTHWEST;
                        npcs[ANN].unk_1E = 2;
                        npcs[ANN].startingCoordinates.x = -224.0f;
                        npcs[ANN].flags |= 1;  

                        setSpecialDialogueBit(0x9F);
                        
                    }
                                     
                    
                }
                
            } 
        
        }
        
    }

    npcs[ANN].movingFlag = npcs[ANN].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007A9B0);

void func_8007A9B0(void) {

    u8 result;

    npcs[KAREN].unk_1A = 0x40;
    npcs[KAREN].unk_1B = 0x40;
    npcs[KAREN].animationIndex1 = 0;
    npcs[KAREN].animationIndex2 = 8;

    if (!checkDailyEventBit(0x52) && !checkLifeEventBit(0xD9) && !checkLifeEventBit(0x2F) && !checkDailyEventBit(0x5A)) {

        if (checkLifeEventBit(MARRIED) && gWife == KAREN) {

            if (5 < gHour && gHour < 8) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = SOUTH;
                npcs[KAREN].unk_1E = 0;
                npcs[KAREN].startingCoordinates.x = -128.0f;
                npcs[KAREN].startingCoordinates.z = -64.0f;
                npcs[KAREN].flags |= 1;
                
            }

            if (7 < gHour && gHour < 12) {

                if (gDayOfWeek == SUNDAY) {

                    switch (npcs[KAREN].location) {

                        case 0:
                        case 1:
                            npcs[KAREN].levelIndex = BEACH;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].startingCoordinates.z = 0.0f;
                            npcs[KAREN].direction = SOUTHEAST;
                            npcs[KAREN].unk_1E = 0;
                            npcs[KAREN].startingCoordinates.x = -224.0f;
                            npcs[KAREN].flags |= 1;

                            setSpecialDialogueBit(0xA9);

                            break;
                        
                        case 2:
                                                        
                            npcs[KAREN].levelIndex = VINEYARD;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].startingCoordinates.z = 0.0f;
                            npcs[KAREN].direction = NORTHWEST;
                            npcs[KAREN].unk_1E = 1;
                            npcs[KAREN].startingCoordinates.x = -32.0f;
                            npcs[KAREN].flags |= 1;
                            
                            break;
                        
                        case 3:

                            npcs[KAREN].levelIndex = MOUNTAIN_1;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].direction = SOUTHEAST;
                            npcs[KAREN].unk_1E = 1;
                            npcs[KAREN].startingCoordinates.x = 96.0f;
                            npcs[KAREN].startingCoordinates.z = 96.0f;
                            npcs[KAREN].flags |= 1;

                            setSpecialDialogueBit(0xAa);

                            break;
                        
                    } 
                    
                } else if (gWeather == SUNNY) {
                        
                    npcs[KAREN].levelIndex = FARM;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].unk_1E = 0;
                    npcs[KAREN].startingCoordinates.x = -256.0f;
                    npcs[KAREN].startingCoordinates.z = -320.0f;
                    npcs[KAREN].flags |= 1;
                    
                } else {

                    npcs[KAREN].levelIndex = KITCHEN;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].direction = NORTHWEST;
                    npcs[KAREN].unk_1E = 1;
                    npcs[KAREN].startingCoordinates.x = -160.0f;
                    npcs[KAREN].startingCoordinates.z = -64.0f;
                    npcs[KAREN].flags |= 1;
                    
                }
                
            }

            if (gHour == 12) {
                
                npcs[KAREN].levelIndex = KITCHEN;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = NORTHWEST;
                npcs[KAREN].unk_1E = 0;
                npcs[KAREN].startingCoordinates.x = -160.0f;
                npcs[KAREN].startingCoordinates.z = -64.0f;
                npcs[KAREN].flags |= 1;
                
            }

            if (12 < gHour && gHour < 17) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.x = 0.0f;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].startingCoordinates.z = 0.0f;
                npcs[KAREN].direction = SOUTHWEST;
                npcs[KAREN].unk_1E = 1;
                npcs[KAREN].flags |= 1;
                
            }

            if (16 < gHour && gHour < 20) {
                
                npcs[KAREN].levelIndex = KITCHEN;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = NORTHWEST;
                npcs[KAREN].unk_1E = 0;
                npcs[KAREN].startingCoordinates.x = -160.0f;
                npcs[KAREN].startingCoordinates.z = -64.0f;
                npcs[KAREN].flags |= 1;
                
            }
            
            if (19 < gHour && gHour < 22) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = SOUTHEAST;
                npcs[KAREN].unk_1E = 0;
                npcs[KAREN].startingCoordinates.x = -128.0f;
                npcs[KAREN].startingCoordinates.z = -112.0f;
                npcs[KAREN].flags |= 1;
                
            }

            if ((u32)(gHour - 6) >= 16) {

                npcs[KAREN].levelIndex = HOUSE;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = SOUTHWEST;
                npcs[KAREN].unk_1E = 4;
                npcs[KAREN].startingCoordinates.x = -192.0f;
                npcs[KAREN].startingCoordinates.z = -160.0f;
                npcs[KAREN].flags |= 1;
                
            }

            if (func_80076CF4() == 1 && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {
                npcs[KAREN].animationIndex1 = 0x66;
                npcs[KAREN].animationIndex2 = 0x70;
            }
    
        } else if (!checkDailyEventBit(0x4D)) {

            if (checkLifeEventBit(0x1E)) {

                if (gWeather == SUNNY) {

                    switch (gDayOfWeek) {

                        case SUNDAY:

                            if (9 < gHour && gHour < 18) {
                                
                                npcs[KAREN].levelIndex = BEACH;
                                npcs[KAREN].startingCoordinates.y = 0.0f;
                                npcs[KAREN].startingCoordinates.z = 0.0f;
                                npcs[KAREN].direction = SOUTHEAST;
                                npcs[KAREN].unk_1E = 0;
                                npcs[KAREN].startingCoordinates.x = -224.0f;
                                npcs[KAREN].flags |= 1;
            
                                setSpecialDialogueBit(0xA9);
                                
                            }

                            break;

                        case MONDAY:
                        case THURSDAY:
                            
                            if (8 < gHour && gHour < 17) { 
                            
                                npcs[KAREN].levelIndex = VINEYARD;
                                npcs[KAREN].startingCoordinates.y = 0.0f;
                                npcs[KAREN].direction = NORTHEAST;
                                npcs[KAREN].unk_1E = 1;
                                npcs[KAREN].startingCoordinates.x = 192.0f;
                                npcs[KAREN].startingCoordinates.z = 192.0f;
                                npcs[KAREN].flags |= 1;
                                
                            }
                            
                            break;

                        case TUESDAY:
                        case WEDNESDAY:
                        case FRIDAY:
                        case SATURDAY:

                            if (8 < gHour && gHour < 17) {
                                
                                if (npcs[KAREN].location < 2) {
                                                         
                                    npcs[KAREN].levelIndex = VINEYARD_CELLAR_BASEMENT;
                                    npcs[KAREN].startingCoordinates.y = 0.0f;
                                    npcs[KAREN].direction = NORTHWEST;
                                    npcs[KAREN].unk_1E = 0;
                                    npcs[KAREN].startingCoordinates.x = -32.0f;
                                    npcs[KAREN].startingCoordinates.z = -128.0f;
                                    npcs[KAREN].flags |= 1;
                                            
                                }
                                
                            }
                        
                    }

                    if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

                        if (8 < gHour && gHour < 17) {
                                             
                            npcs[KAREN].levelIndex = VINEYARD;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].startingCoordinates.z = 0.0f;
                            npcs[KAREN].direction = SOUTHWEST;
                            npcs[KAREN].unk_1E = 3;
                            npcs[KAREN].startingCoordinates.x = -192.0f;
                            npcs[KAREN].flags |= 1;
                                              
                        }
                        
                    }
                    
                } else {

                    if (npcs[KAREN].location < 2) {

                        if (8 < gHour && gHour < 17) {

                            npcs[KAREN].levelIndex = VINEYARD_CELLAR;
                            npcs[KAREN].startingCoordinates.y = 0.0f;
                            npcs[KAREN].direction = NORTHWEST;
                            npcs[KAREN].unk_1E = 0;
                            npcs[KAREN].startingCoordinates.x = -80.0f;
                            npcs[KAREN].startingCoordinates.z = -112.0f;
                            npcs[KAREN].flags |= 1;
                            
                        }
                        
                    }
                    
                    
                }

                if (func_80076DCC(KAREN) == 1) {
                    npcs[KAREN].animationIndex1 = 0x66;
                    npcs[KAREN].animationIndex2 = 0x70;
                }
                
            } else {

            if ((gDayOfWeek == MONDAY || gDayOfWeek == THURSDAY) && gWeather == SUNNY && 8 < gHour && gHour < 17) {
                    
                npcs[KAREN].levelIndex = VINEYARD;
                npcs[KAREN].startingCoordinates.y = 0.0f;
                npcs[KAREN].direction = NORTHEAST;
                npcs[KAREN].unk_1E = 1;
                npcs[KAREN].startingCoordinates.x = 192.0f;
                npcs[KAREN].startingCoordinates.z = 192.0f;
                npcs[KAREN].flags |= 1;
                                  
            }

            switch (gDayOfWeek) {


                case TUESDAY:
                case WEDNESDAY:
                case FRIDAY:
                case SATURDAY:
                    
                    if (gWeather == SUNNY) {

                        switch (npcs[KAREN].location) {
                            
                            case 0:

                                if (9 < gHour && gHour < 17) {
                                    
                                    npcs[KAREN].levelIndex = MOUNTAIN_2;
                                    npcs[KAREN].startingCoordinates.y = 0.0f;
                                    npcs[KAREN].direction = SOUTHWEST;
                                    npcs[KAREN].unk_1E = 0;
                                    npcs[KAREN].startingCoordinates.x = -80.0f;
                                    npcs[KAREN].startingCoordinates.z = 160.0f;
                                    npcs[KAREN].flags |= 1;
        
                                    setSpecialDialogueBit(0xAA);
                                    
                                } 
                            
                                break;

                            case 1:

                                if (9 < gHour && gHour < 17) {
                                    
                                    npcs[KAREN].levelIndex = BEACH;
                                    npcs[KAREN].startingCoordinates.y = 0.0f;
                                    npcs[KAREN].direction = SOUTHEAST;
                                    npcs[KAREN].unk_1E = 0;
                                    npcs[KAREN].startingCoordinates.x = -96.0f;
                                    npcs[KAREN].startingCoordinates.z = 176.0f;
                                    npcs[KAREN].flags |= 1;
                
                                    setSpecialDialogueBit(0xA9);
                                    
                                }

                                break;
                            
                            case 2:
                            case 3:
                                break;
                            
                        }
                        
                    }


                
            }

            if (gDayOfWeek == SUNDAY && gWeather == SUNNY) {

                if (9 < gHour && gHour < 18) {
                    
                    npcs[KAREN].levelIndex = BEACH;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].startingCoordinates.z = 0.0f;
                    npcs[KAREN].direction = SOUTHEAST;
                    npcs[KAREN].unk_1E = 0;
                    npcs[KAREN].startingCoordinates.x = -224.0f;
                    npcs[KAREN].flags |= 1;

                    setSpecialDialogueBit(0xA9);
                
                    
                }
                
            } 

            if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11 && gWeather == SUNNY) {

                if (8 < gHour && gHour < 17) {
                    
                    npcs[KAREN].levelIndex = VINEYARD;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].startingCoordinates.z = 0.0f;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].unk_1E = 3;
                    npcs[KAREN].startingCoordinates.x = -192.0f;
                    npcs[KAREN].flags |= 1;
                    
                }
                
            }

            if (17 < gHour && gHour < 24) {

                if (3 < gDayOfWeek && gDayOfWeek < 7) {
                    
                    npcs[KAREN].levelIndex = TAVERN;
                    npcs[KAREN].startingCoordinates.y = 0.0f;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].unk_1E = 1;
                    npcs[KAREN].startingCoordinates.x = 64.0f;
                    npcs[KAREN].startingCoordinates.z = -32.0f;
                    npcs[KAREN].flags |= 1;

                    setSpecialDialogueBit(0xAB);
                    
                }

                if (gDayOfWeek == MONDAY) {

                    npcs[KAREN].levelIndex = TAVERN;
                    npcs[KAREN].startingCoordinates.y = 0;
                    npcs[KAREN].direction = SOUTHWEST;
                    npcs[KAREN].unk_1E = 0;
                    npcs[KAREN].startingCoordinates.x = -64.0f;
                    npcs[KAREN].startingCoordinates.z = -64.0f;
                    npcs[KAREN].flags |= 1;

                    setSpecialDialogueBit(0xAB);
                    
                }
                
            }

            if (gSeason == SUMMER && gDayOfWeek == SATURDAY && gWeather == SUNNY && npcs[KAREN].location == 0) {

                if (18 < gHour && gHour < 21) {

                        npcs[KAREN].levelIndex = BEACH;
                        npcs[KAREN].startingCoordinates.y = 0.0f;
                        npcs[KAREN].startingCoordinates.z = 0.0f;
                        npcs[KAREN].direction = SOUTHWEST;
                        npcs[KAREN].unk_1E = 2;
                        npcs[KAREN].startingCoordinates.x = -224.0f;
                        npcs[KAREN].flags |= 1;

                        setSpecialDialogueBit(0xA9);
                    
                }
                
            }
            
        }
            
        } 
        
    }

    npcs[KAREN].movingFlag = npcs[KAREN].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007B828);

void func_8007B828(void) {

    u8 set = FALSE;
    
    toggleDailyEventBit(0x16);

    if (!checkDailyEventBit(0x52) && !checkLifeEventBit(0xD9) && ((u8)(gPlayer.heldItem + 0x46)) >= 0x10) {

        npcs[BABY].unk_1A = 0x40;
        npcs[BABY].unk_1B = 0x40;
        npcs[BABY].animationIndex1 = 0;
        npcs[BABY].animationIndex2 = 8;

        set = func_80076CF4();

        if (set == 1) {

            if (npcs[gWife].flags & 1) {
                
                npcs[BABY].levelIndex = npcs[gWife].levelIndex;
                npcs[BABY].startingCoordinates.x = 0.0f;
                npcs[BABY].startingCoordinates.y = 0.0f;
                npcs[BABY].startingCoordinates.z = 0.0f;
                npcs[BABY].flags |= (1 | 8);
                npcs[BABY].direction = npcs[gWife].direction;
                npcs[BABY].unk_1E = 0;
                
            }
            
        }

        if (set == 2) {

            npcs[BABY].levelIndex = HOUSE;
            npcs[BABY].startingCoordinates.y = 0.0f;
            npcs[BABY].direction = SOUTHWEST;
            npcs[BABY].unk_1E = 1;
            npcs[BABY].startingCoordinates.x = -192.0f;
            npcs[BABY].startingCoordinates.z = -64.0f;
            npcs[BABY].flags |= 1;

            setDailyEventBit(0x16);
            
        }

        if (set == 3) {

            npcs[BABY].levelIndex = HOUSE;
            npcs[BABY].startingCoordinates.y = 0.0f;
            npcs[BABY].direction = SOUTHWEST;
            npcs[BABY].unk_1E = 2;
            npcs[BABY].startingCoordinates.x = -192.0f;
            npcs[BABY].startingCoordinates.z = -64.0f;
            npcs[BABY].flags |= 1;

            setDailyEventBit(0x16);
            
        } 

        if (set == 4) {

            if (npcs[gWife].flags & 1) {
                
                npcs[BABY].levelIndex = npcs[gWife].levelIndex;
                npcs[BABY].startingCoordinates.x = npcs[gWife].startingCoordinates.x;
                npcs[BABY].startingCoordinates.y = npcs[gWife].startingCoordinates.y;
                npcs[BABY].startingCoordinates.z = npcs[gWife].startingCoordinates.z;
                npcs[BABY].flags |= 1;
                npcs[BABY].direction = npcs[gWife].direction;
                npcs[BABY].unk_1E = 3;
                npcs[BABY].animationIndex1 = 0x39;
                npcs[BABY].animationIndex2 = 0x41;
                
            }
            
        }

        if (set == 5) {

            if (npcs[gWife].flags & 1) {
                
                npcs[BABY].levelIndex = HOUSE;
                npcs[BABY].startingCoordinates.y = 0.0f;
                npcs[BABY].direction = SOUTHWEST;
                npcs[BABY].unk_1E = 5;
                npcs[BABY].startingCoordinates.x = -194.0f;
                npcs[BABY].startingCoordinates.z = -64.0f;
                npcs[BABY].flags |= 1;
                
            }
            
        }

        if (set == 6) {
            
            if (npcs[gWife].flags & 1) {
                
                npcs[BABY].levelIndex = npcs[gWife].levelIndex;
                npcs[BABY].startingCoordinates.x = npcs[gWife].startingCoordinates.x;
                npcs[BABY].startingCoordinates.y = npcs[gWife].startingCoordinates.y;
                npcs[BABY].startingCoordinates.z = npcs[gWife].startingCoordinates.z;
                npcs[BABY].flags |= 1;
                npcs[BABY].direction = npcs[gWife].direction;
                npcs[BABY].unk_1E = 4;
                npcs[BABY].animationIndex1 = 0xE;
                npcs[BABY].animationIndex2 = 0x16;
                
            }
            
        }
        
        npcs[BABY].movingFlag = npcs[BABY].unk_1E;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007BD50);

void func_8007BD50(void) {

    npcs[HARRIS].unk_1A = 0x80;
    npcs[HARRIS].unk_1B = 0x80;
    npcs[HARRIS].animationIndex1 = 0;
    npcs[HARRIS].animationIndex2 = 8;

    if (gDayOfWeek != SUNDAY && gDayOfWeek < 7) {

        if (8 < gHour && gHour < 12) {

            if (npcs[HARRIS].location < 2) {

                npcs[HARRIS].levelIndex = VILLAGE_1;
                npcs[HARRIS].startingCoordinates.x = 0.0f;
                npcs[HARRIS].startingCoordinates.y = 0.0f;
                npcs[HARRIS].startingCoordinates.z = 0.0f;
                npcs[HARRIS].direction = SOUTHWEST;
                npcs[HARRIS].unk_1E = 1;
                npcs[HARRIS].flags |= 1;
                
                
            } else {

                npcs[HARRIS].levelIndex = VILLAGE_2;
                npcs[HARRIS].startingCoordinates.x = 0.0f;
                npcs[HARRIS].startingCoordinates.y = 0.0f;
                npcs[HARRIS].startingCoordinates.z = 0.0f;
                npcs[HARRIS].direction = SOUTHWEST;
                npcs[HARRIS].unk_1E = 1;
                npcs[HARRIS].flags |= 1;
                
            }
            
        }
        
        if (11 < gHour && gHour < 18) { 

            switch (npcs[HARRIS].location) {

                case 0:
                case 1:
                    npcs[HARRIS].levelIndex = MOUNTAIN_1;
                    npcs[HARRIS].startingCoordinates.y = 0.0f;
                    npcs[HARRIS].direction = SOUTHWEST;
                    npcs[HARRIS].unk_1E = 1;
                    npcs[HARRIS].startingCoordinates.x = -16.0f;
                    npcs[HARRIS].startingCoordinates.z = 128.0f;
                    npcs[HARRIS].flags |= 1;                    
                    break;
                
                case 2:
    
                    npcs[HARRIS].levelIndex = ROAD;
                    npcs[HARRIS].startingCoordinates.y = 0.0f;
                    npcs[HARRIS].startingCoordinates.z = 0.0f;
                    npcs[HARRIS].direction = SOUTHWEST;
                    npcs[HARRIS].unk_1E = 1;
                    npcs[HARRIS].startingCoordinates.x = -64.0f;
                    npcs[HARRIS].flags |= 1;
                    break;
                
                case 3:

                    npcs[HARRIS].levelIndex = RANCH;
                    npcs[HARRIS].startingCoordinates.y = 0.0f;
                    npcs[HARRIS].direction = SOUTHWEST;
                    npcs[HARRIS].unk_1E = 1;
                    npcs[HARRIS].startingCoordinates.x = 160.0f;
                    npcs[HARRIS].startingCoordinates.z = -320.0f;
                    npcs[HARRIS].flags |= 1;

                    break;
                
                
            }
            
        }
        
    }

    if (gDayOfWeek == SUNDAY) {

        if (11 < gHour && gHour < 17) {
            
            npcs[HARRIS].levelIndex = LIBRARY;
            npcs[HARRIS].startingCoordinates.y = 0.0f;
            npcs[HARRIS].direction = SOUTHEAST;
            npcs[HARRIS].unk_1E = 0;
            npcs[HARRIS].startingCoordinates.x = 32.0f;
            npcs[HARRIS].startingCoordinates.z = 96.0f;
            npcs[HARRIS].flags |= 1;
            
        }
        
    }

    if (!checkLifeEventBit(MARIA_HARRIS_MARRIED) && 17 < gHour && gHour < 24 && npcs[HARRIS].location < 2) {

        npcs[HARRIS].levelIndex = TAVERN;
        npcs[HARRIS].startingCoordinates.y = 0.0f;
        npcs[HARRIS].startingCoordinates.z = 0.0f;
        npcs[HARRIS].direction = SOUTHWEST;
        npcs[HARRIS].unk_1E = 0;
        npcs[HARRIS].startingCoordinates.x = -144.0f;
        npcs[HARRIS].flags |= 1;

        setSpecialDialogueBit(0x40);
        
    }

    npcs[HARRIS].movingFlag = npcs[HARRIS].unk_1E;
    
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007C088);

void func_8007C088(void) {

    npcs[GRAY].unk_1A = 0x40;
    npcs[GRAY].unk_1B = 0x40;
    npcs[GRAY].animationIndex1 = 0;
    npcs[GRAY].animationIndex2 = 8;

    switch (gDayOfWeek) {

        case SUNDAY:
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
        case FRIDAY:
        case SATURDAY:

            if (7 < gHour && gHour < 12) {

                npcs[GRAY].levelIndex = RANCH_BARN;
                npcs[GRAY].startingCoordinates.y = 0.0f;
                npcs[GRAY].direction = SOUTHWEST;
                npcs[GRAY].unk_1E = 1;
                npcs[GRAY].startingCoordinates.x = -128.0f;
                npcs[GRAY].startingCoordinates.z = -48.0f;
                npcs[GRAY].flags |= 1;
            
            }

            if (11 < gHour && gHour < 18) {

                if (gWeather == SUNNY) {
                    
                    npcs[GRAY].levelIndex = RANCH;
                    npcs[GRAY].startingCoordinates.y = 0.0f;
                    npcs[GRAY].direction = SOUTHWEST;
                    npcs[GRAY].unk_1E = 1;
                    npcs[GRAY].startingCoordinates.x = -160.0f;
                    npcs[GRAY].startingCoordinates.z = 128.0f;
                    npcs[GRAY].flags |= 1;
                                    
                } else {
                    
                    npcs[GRAY].levelIndex = RANCH_BARN;
                    npcs[GRAY].startingCoordinates.x = 0.0f;
                    npcs[GRAY].startingCoordinates.y = 0.0f;
                    npcs[GRAY].startingCoordinates.z = 0.0f;
                    npcs[GRAY].direction = SOUTHWEST;
                    npcs[GRAY].unk_1E = 2;
                    npcs[GRAY].flags |= 1;
                    
                }
                
            }

            break;

        case THURSDAY:

            if (gWeather == SUNNY) {

                 if (7 < gHour && gHour < 13) {

                    npcs[GRAY].levelIndex = MOUNTAIN_2;
                    npcs[GRAY].startingCoordinates.y = 0.0f;
                    npcs[GRAY].direction = SOUTHEAST;
                    npcs[GRAY].unk_1E = 0;
                    npcs[GRAY].startingCoordinates.x = -128.0f;
                    npcs[GRAY].startingCoordinates.z = 352.0f;
                    npcs[GRAY].flags |= 1;

                 }
                
            } else if (7 < gHour && gHour < 18) {

                npcs[GRAY].levelIndex = RANCH_HOUSE;
                npcs[GRAY].startingCoordinates.y = 0.0f;
                npcs[GRAY].direction = SOUTHWEST;
                npcs[GRAY].unk_1E = 1;
                npcs[GRAY].startingCoordinates.x = -64.0f;
                npcs[GRAY].startingCoordinates.z = -64.0f;
                npcs[GRAY].flags |= 1;
                
            }
        
    }

    if (!checkLifeEventBit(POPURI_GRAY_MARRIED) && 17 < gHour && gHour < 24 && npcs[GRAY].location < 2) {

        npcs[GRAY].levelIndex = TAVERN;
        npcs[GRAY].startingCoordinates.y = 0.0f;
        npcs[GRAY].startingCoordinates.z = 0.0f;
        npcs[GRAY].direction = SOUTHWEST;
        npcs[GRAY].unk_1E = 0;
        npcs[GRAY].startingCoordinates.x = 80.0f;
        npcs[GRAY].flags |= 1;

        setSpecialDialogueBit(0x131);
        
    }

    npcs[GRAY].movingFlag = npcs[GRAY].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007C3D4);

void func_8007C3D4(void) {

    npcs[JEFF].unk_1A = 0x40;
    npcs[JEFF].unk_1B = 0x40;
    npcs[JEFF].animationIndex1 = 0;
    npcs[JEFF].animationIndex2 = 8;

    if (checkLifeEventBit(ELLI_JEFF_MARRIED)) {

        switch (gDayOfWeek) {       

            case TUESDAY ... SATURDAY:
            case SUNDAY:
                
                if (8 < gHour && gHour < 17) {
                    
                    npcs[JEFF].levelIndex = BAKERY;
                    npcs[JEFF].startingCoordinates.y = 0.0f;
                    npcs[JEFF].direction = SOUTHWEST;
                    npcs[JEFF].unk_1E = 0;
                    npcs[JEFF].startingCoordinates.x = -96.0f;
                    npcs[JEFF].startingCoordinates.z = -96.0f;
                    npcs[JEFF].flags |= 1;
                    
                }
                
                break;

            case MONDAY:               

                switch (gSeason) {
                    
                    case SPRING:
                    case SUMMER:

                        if (7 < gHour && gHour < 17 && gWeather == SUNNY) {

                            npcs[JEFF].levelIndex = MOUNTAIN_1;
                            npcs[JEFF].startingCoordinates.y = 0.0f;
                            npcs[JEFF].direction = NORTHWEST;
                            npcs[JEFF].unk_1E = 2;
                            npcs[JEFF].startingCoordinates.x = -224.0f;
                            npcs[JEFF].startingCoordinates.z = -128.0f;
                            npcs[JEFF].flags |= 1;
                    
                        }

                        break;

                    case AUTUMN:
                        
                        if (7 < gHour && gHour < 15 && gWeather == SUNNY) {

                            npcs[JEFF].levelIndex = MOUNTAIN_1;
                            npcs[JEFF].startingCoordinates.y = 0.0f;
                            npcs[JEFF].direction = NORTHEAST;
                            npcs[JEFF].unk_1E = 0;
                            npcs[JEFF].startingCoordinates.x = 192.0f;
                            npcs[JEFF].startingCoordinates.z = 48.0f;
                            npcs[JEFF].flags |= 1;
                    
                        }
                        
                        break;
                }

                break;

        }
        
    } else {

        switch (gDayOfWeek) {       

            case TUESDAY ... SATURDAY:
            case SUNDAY:
                
                if (8 < gHour && gHour < 17) {
                    
                    npcs[JEFF].levelIndex = BAKERY;
                    npcs[JEFF].startingCoordinates.y = 0.0f;
                    npcs[JEFF].direction = SOUTHWEST;
                    npcs[JEFF].unk_1E = 0;
                    npcs[JEFF].startingCoordinates.x = -96.0f;
                    npcs[JEFF].startingCoordinates.z = -96.0f;
                    npcs[JEFF].flags |= 1;
                    
                }

                break;
            
            case MONDAY:         
        
                if (7 < gHour && gHour < 17 && gWeather == SUNNY) {

                    if (npcs[JEFF].location < 2) {
                        
                        npcs[JEFF].levelIndex = MOUNTAIN_1;
                        npcs[JEFF].startingCoordinates.y = 0.0f;
                        npcs[JEFF].direction = NORTHWEST;
                        npcs[JEFF].unk_1E = 2;
                        npcs[JEFF].startingCoordinates.x = -224.0f;
                        npcs[JEFF].startingCoordinates.z = -128.0f;
                        npcs[JEFF].flags |= 1;
                            
                    }
            
                }

                break;

        }

        if (17 < gHour && gHour < 24 && npcs[JEFF].location < 2) {
            
            npcs[JEFF].levelIndex = TAVERN;
            npcs[JEFF].startingCoordinates.y = 0.0f;
            npcs[JEFF].direction = NORTHEAST;
            npcs[JEFF].unk_1E = 0;
            npcs[JEFF].startingCoordinates.x = -160.0f;
            npcs[JEFF].startingCoordinates.z = -64.0f;
            npcs[JEFF].flags |= 1;

            setSpecialDialogueBit(0x132);
    
        }
        
    }

    npcs[JEFF].movingFlag = npcs[JEFF].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007C7C4);

void func_8007C7C4(void) {

    npcs[CLIFF].unk_1A = 0x40;
    npcs[CLIFF].unk_1B = 0x40;
    npcs[CLIFF].animationIndex1 = 0;
    npcs[CLIFF].animationIndex2 = 8;

    if (!checkLifeEventBit(CLIFF_GONE)) {

        if (checkLifeEventBit(ANN_CLIFF_MARRIED)) {

            switch (gDayOfWeek) {       

                case MONDAY:                                 
                case TUESDAY:                                 
                case WEDNESDAY:                                 
                case FRIDAY:                                 
                case SATURDAY:             
                    
                    if (gWeather == SUNNY) {
                        
                        if (7 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = RANCH;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].unk_1E = 1;
                            npcs[CLIFF].startingCoordinates.x = 64.0f;
                            npcs[CLIFF].startingCoordinates.z = 128.0f;
                            npcs[CLIFF].flags |= 1;

                        }
                        
                    } else if (7 < gHour && gHour < 17) {

                        npcs[CLIFF].levelIndex = RANCH_BARN;
                        npcs[CLIFF].startingCoordinates.x = 0.0f;
                        npcs[CLIFF].startingCoordinates.y = 0.0f;
                        npcs[CLIFF].startingCoordinates.z = -256.0f;
                        npcs[CLIFF].direction = SOUTHWEST;
                        npcs[CLIFF].unk_1E = 0;
                        npcs[CLIFF].flags |= 1;
                    
                    }
                    
                    break;

                case THURSDAY:          
                    
                    if (gWeather == SUNNY) {

                        if (8 < gHour && gHour < 17) {

                            if (npcs[CLIFF].location < 2) {

                                npcs[CLIFF].levelIndex = MOUNTAIN_2;
                                npcs[CLIFF].startingCoordinates.y = 0.0f;
                                npcs[CLIFF].direction = SOUTH;
                                npcs[CLIFF].unk_1E = 0;
                                npcs[CLIFF].startingCoordinates.x = -224.0f;
                                npcs[CLIFF].startingCoordinates.z = 160.0f;
                                npcs[CLIFF].flags |= 1;
                                
                            } else {
                                
                                npcs[CLIFF].levelIndex = CAVE;
                                npcs[CLIFF].startingCoordinates.y = 0.0f;
                                npcs[CLIFF].direction = SOUTHEAST;
                                npcs[CLIFF].unk_1E = 0;
                                npcs[CLIFF].startingCoordinates.x = -256.0f;
                                npcs[CLIFF].startingCoordinates.z = -128.0f;
                                npcs[CLIFF].flags |= 1;
                                
                            }
                        
                        }
                    
                    } else {

                        if (7 < gHour && gHour < 17) {
                            
                            npcs[CLIFF].levelIndex = CARPENTER_HUT;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = EAST;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = -144.0f;
                            npcs[CLIFF].startingCoordinates.z = 80.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }
                        
                    }

                    break;

            }
            
        } else if (checkLifeEventBit(CLIFF_ARRIVED) && !checkLifeEventBit(0x42)) {

            if (gWeather == SUNNY) {
                
                switch (gDayOfWeek) {    
                    
                    case SUNDAY:                             

                        if (npcs[CLIFF].location < 2 && 8 < gHour && gHour < 17) {
                            
                            npcs[CLIFF].levelIndex = MOUNTAIN_2;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTH;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = -224.0f;
                            npcs[CLIFF].startingCoordinates.z = 160.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }
                        
                        break;
                    
                    case MONDAY:                 

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = TOP_OF_MOUNTAIN_1;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].startingCoordinates.z = 0.0f;
                            npcs[CLIFF].direction = NORTHWEST;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = 192.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }

                        break;
                    
                    case TUESDAY:                             

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = BEACH;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].startingCoordinates.z = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].unk_1E = 1;
                            npcs[CLIFF].startingCoordinates.x = -224.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }
                        
                        break;
                    
                    case WEDNESDAY:                             

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = RANCH;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = -32.0f;
                            npcs[CLIFF].startingCoordinates.z = -96.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }
                        
                        break;
                    
                    case THURSDAY:                             

                        if (8 < gHour && gHour < 17) {

                            npcs[CLIFF].levelIndex = MOUNTAIN_2;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = NORTHWEST;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = 64.0f;
                            npcs[CLIFF].startingCoordinates.z = -160.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }
                        
                        break;
                    
                    case FRIDAY:                             
                    case SATURDAY:      

                        if (npcs[CLIFF].location < 2 && 8 < gHour && gHour < 17) {
                            
                            npcs[CLIFF].levelIndex = MOUNTAIN_1;
                            npcs[CLIFF].startingCoordinates.x = 0.0f;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].startingCoordinates.z = 0.0f;
                            npcs[CLIFF].direction = SOUTHWEST;
                            npcs[CLIFF].unk_1E = 1;
                            npcs[CLIFF].flags |= 1;
                            
                        }

                        break;
                    
                }
            
            } else if (8 < gHour && gHour < 17) {

                if (npcs[CLIFF].location < 2) {

                    npcs[CLIFF].levelIndex = CAVE;
                    npcs[CLIFF].startingCoordinates.y = 0.0f;
                    npcs[CLIFF].direction = SOUTHEAST;
                    npcs[CLIFF].unk_1E = 0;
                    npcs[CLIFF].startingCoordinates.x = -256.0f;
                    npcs[CLIFF].startingCoordinates.z = -128.0f;
                    npcs[CLIFF].flags |= 1;
                    
                } else {

                    npcs[CLIFF].levelIndex = CARPENTER_HUT;
                    npcs[CLIFF].startingCoordinates.y = 0.0f;
                    npcs[CLIFF].direction = EAST;
                    npcs[CLIFF].unk_1E = 0;
                    npcs[CLIFF].startingCoordinates.x = -144.0f;
                    npcs[CLIFF].startingCoordinates.z = 80.0f;
                    npcs[CLIFF].flags |= 1;
                
                }
                
            }

            switch (npcs[CLIFF].location) {

                case 0:
                    
                    if (gWeather == SUNNY) {
    
                        if (16 < gHour && gHour < 24) {
                            
                            npcs[CLIFF].levelIndex = MOUNTAIN_1;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = SOUTHEAST;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = 96.0f;
                            npcs[CLIFF].startingCoordinates.z = -16.0f;
                            npcs[CLIFF].flags |= 1;
                            
                        }
                    
                    }
                        
                    break;

                case 1:
                case 2:

                    if (17 < gHour && gHour < 24) {

                        if (npcs[CLIFF].location < 2) {
                            
                            npcs[CLIFF].levelIndex = TAVERN;
                            npcs[CLIFF].startingCoordinates.y = 0.0f;
                            npcs[CLIFF].direction = NORTHEAST;
                            npcs[CLIFF].unk_1E = 0;
                            npcs[CLIFF].startingCoordinates.x = 80.0f;
                            npcs[CLIFF].startingCoordinates.z = -64.0f;
                            npcs[CLIFF].flags |= 1;
            
                            setSpecialDialogueBit(0x133);
                        
                        }
                        
                    }

                    break;
                
            }
            
        }
        
         npcs[CLIFF].movingFlag = npcs[CLIFF].unk_1E;
        
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007CEB4);

void func_8007CEB4(void) {

    npcs[KAI].unk_1A = 0x40;
    npcs[KAI].unk_1B = 0x40;
    npcs[KAI].animationIndex1 = 0;
    npcs[KAI].animationIndex2 = 8;

    if (!checkLifeEventBit(KAI_GONE) && !checkDailyEventBit(0x4F)) {

        if (checkLifeEventBit(KAREN_KAI_MARRIED)) {

            if (gWeather == SUNNY) {

                if (gSeason != WINTER) {

                    if (8 < gHour && gHour < 17) {
                        
                        npcs[KAI].levelIndex = VINEYARD;
                        npcs[KAI].startingCoordinates.x = 0.0f;
                        npcs[KAI].startingCoordinates.y = 0.0f;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].direction = SOUTHWEST;
                        npcs[KAI].unk_1E = 1;
                        npcs[KAI].flags |= 1;

                    }
                    
                } else {

                    if (8 < gHour && gHour < 17) {
                        
                        npcs[KAI].levelIndex = VINEYARD_CELLAR_BASEMENT;
                        npcs[KAI].startingCoordinates.y = 0;
                        npcs[KAI].direction = SOUTHWEST;
                        npcs[KAI].unk_1E = 1;
                        npcs[KAI].startingCoordinates.x = -32.0f;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].flags |= 1;

                    }

                }

                 if (gDayOfWeek == SUNDAY && 9 < gHour && gHour < 18) {

                    npcs[KAI].levelIndex = BEACH;
                    npcs[KAI].startingCoordinates.y = 0;
                    npcs[KAI].direction = SOUTHEAST;
                    npcs[KAI].unk_1E = 0;
                    npcs[KAI].startingCoordinates.x = -224.0f;
                    npcs[KAI].startingCoordinates.z = -32.0f;
                    npcs[KAI].flags |= 1;
                     
                 }
                
                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11  && 8 < gHour && gHour < 17) {

                    npcs[KAI].levelIndex = VINEYARD;
                    npcs[KAI].startingCoordinates.x = -224.0f;
                    npcs[KAI].startingCoordinates.y = 0.0f;
                    npcs[KAI].startingCoordinates.z = 0.0f;
                    npcs[KAI].direction = SOUTHWEST;
                    npcs[KAI].unk_1E = 1;
                    npcs[KAI].flags |= 1;
                    
                }
                
            } else {

                if (8 < gHour && gHour < 17) {
                    npcs[KAI].levelIndex = VINEYARD_CELLAR;
                    npcs[KAI].startingCoordinates.y = 0.0f;
                    npcs[KAI].direction = SOUTHEAST;
                    npcs[KAI].unk_1E = 0;
                    npcs[KAI].startingCoordinates.x = -64.0f;
                    npcs[KAI].startingCoordinates.z = -64.0f;
                    npcs[KAI].flags |= 1;
                    
                }

            }
            
        } else {

            if (gWeather == SUNNY) {
                
                if (gSeason != WINTER) {

                    if (8 < gHour && gHour < 17) {
                    
                        npcs[KAI].levelIndex = VINEYARD;
                        npcs[KAI].startingCoordinates.x = 0.0f;
                        npcs[KAI].startingCoordinates.y = 0;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].direction = 0;
                        npcs[KAI].unk_1E = 1;
                        npcs[KAI].flags |= 1;
                        
                    }

                } else {

                    if (8 < gHour && gHour < 17) {
                    
                        npcs[KAI].levelIndex = VINEYARD_CELLAR_BASEMENT;
                        npcs[KAI].startingCoordinates.y = 0;
                        npcs[KAI].direction = SOUTHWEST;
                        npcs[KAI].unk_1E = 1;
                        npcs[KAI].startingCoordinates.x = -32.0f;
                        npcs[KAI].startingCoordinates.z = -64.0f;
                        npcs[KAI].flags |= 1;
                        
                    }
                    
                }
                    
            } else {

                if (8 < gHour && gHour < 17) {
                    
                    npcs[KAI].levelIndex = VINEYARD_CELLAR;
                    npcs[KAI].startingCoordinates.y = 0;
                    npcs[KAI].direction = SOUTHEAST;
                    npcs[KAI].unk_1E = 0;
                    npcs[KAI].startingCoordinates.x = -64.0f;
                    npcs[KAI].startingCoordinates.z = -64.0f;
                    npcs[KAI].flags |= 1;
                    
                }
                
            }

            if (17 < gHour && gHour < 24 && npcs[KAI].location < 2) {

                    npcs[KAI].levelIndex = TAVERN;
                    npcs[KAI].startingCoordinates.y = 0.0f;
                    npcs[KAI].direction = NORTHEAST;
                    npcs[KAI].unk_1E = 0;
                    npcs[KAI].startingCoordinates.x = -144.0f;
                    npcs[KAI].startingCoordinates.z = 96.0f;
                    npcs[KAI].flags |= 1;

                    setSpecialDialogueBit(0x134);
                
            }
            
        }
        
        npcs[KAI].movingFlag = npcs[KAI].unk_1E;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007D3C4);

void func_8007D3C4(void) {

    npcs[MAYOR].unk_1A = 0x40;
    npcs[MAYOR].unk_1B = 0x40;
    npcs[MAYOR].animationIndex1 = 0;
    npcs[MAYOR].animationIndex2 = 8;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case SUNDAY:

                if (8 < gHour && gHour < 15) {
                    npcs[MAYOR].levelIndex = CHURCH;
                    npcs[MAYOR].startingCoordinates.y = 0;
                    npcs[MAYOR].direction = NORTHEAST;
                    npcs[MAYOR].unk_1E = 0;
                    npcs[MAYOR].startingCoordinates.x = -80.0f;
                    npcs[MAYOR].startingCoordinates.z = -32.0f;
                    npcs[MAYOR].flags |= 1;
                }

                break;
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (npcs[MAYOR].location < 2) {
                    
                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = VILLAGE_2;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHWEST;
                        npcs[MAYOR].unk_1E = 1;
                        npcs[MAYOR].startingCoordinates.x = -224.0f;
                        npcs[MAYOR].startingCoordinates.z = 32.0f;
                        npcs[MAYOR].flags |= 1;
                    }
                    
                } else {

                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = VILLAGE_1;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHEAST;
                        npcs[MAYOR].unk_1E = 1;
                        npcs[MAYOR].startingCoordinates.x = -128.0f;
                        npcs[MAYOR].startingCoordinates.z = -64.0f;
                        npcs[MAYOR].flags |= 1;
                    }
                    
                }
                
                break;
            
            case SATURDAY:

                if (npcs[MAYOR].location < 2) {
                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = VILLAGE_1;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHWEST;
                        npcs[MAYOR].unk_1E = 1;
                        npcs[MAYOR].startingCoordinates.x = -224.0f;
                        npcs[MAYOR].startingCoordinates.z = 288.0f;
                        npcs[MAYOR].flags |= 1;
                    }
                } else {

                    if (7 < gHour && gHour < 17) {
                        npcs[MAYOR].levelIndex = LIBRARY;
                        npcs[MAYOR].startingCoordinates.y = 0;
                        npcs[MAYOR].direction = SOUTHWEST;
                        npcs[MAYOR].unk_1E = 0;
                        npcs[MAYOR].startingCoordinates.x = 64.0f;
                        npcs[MAYOR].startingCoordinates.z = 64.0f;
                        npcs[MAYOR].flags |= 1;
                    }
                    
                }

                break;
            
        }
        
    } else if (8 < gHour && gHour < 15) {
        npcs[MAYOR].levelIndex = MAYOR_HOUSE;
        npcs[MAYOR].startingCoordinates.y = 0;
        npcs[MAYOR].direction = SOUTHWEST;
        npcs[MAYOR].unk_1E = 0;
        npcs[MAYOR].startingCoordinates.x = 16.0f;
        npcs[MAYOR].startingCoordinates.z = -32.0f;
        npcs[MAYOR].flags |= 1;
    }

    npcs[MAYOR].movingFlag = npcs[MAYOR].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007D6E0);

void func_8007D6E0(void) {

    npcs[MAYOR_WIFE].unk_1A = 0x40;
    npcs[MAYOR_WIFE].unk_1B = 0x40;
    npcs[MAYOR_WIFE].animationIndex1 = 0;
    npcs[MAYOR_WIFE].animationIndex2 = 8;

    if (checkLifeEventBit(MARRIED) && gWife == MARIA) {

        switch (gDayOfWeek) {
            case SUNDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
                
                if (8 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = LIBRARY;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = 0;
                    npcs[MAYOR_WIFE].unk_1E = 0;
                    npcs[MAYOR_WIFE].startingCoordinates.x = 48.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -96.0f;
                    npcs[MAYOR_WIFE].flags |= 1;
                }
                break;
            
            default:
                break;
            
        }
        
    } else if (gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case SUNDAY:
                if (8 < gHour && gHour < 15) {
                    npcs[MAYOR_WIFE].levelIndex = CHURCH;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = NORTHEAST;
                    npcs[MAYOR_WIFE].unk_1E = 0;
                    npcs[MAYOR_WIFE].startingCoordinates.x = -112.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -32.0f;
                    npcs[MAYOR_WIFE].flags |= 1;
                }

                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
                
                if (7 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = MAYOR_HOUSE;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = SOUTHWEST;
                    npcs[MAYOR_WIFE].unk_1E = 1;
                    npcs[MAYOR_WIFE].startingCoordinates.x = -64.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -64.0f;
                    npcs[MAYOR_WIFE].flags |= 1;
                }
                
                break;

            case FRIDAY:

                if (11 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = BAKERY;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = SOUTHWEST;
                    npcs[MAYOR_WIFE].unk_1E = 0;
                    npcs[MAYOR_WIFE].startingCoordinates.x = 64.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -32.0f;
                    npcs[MAYOR_WIFE].flags |= 1;
                }
                
                break;

            case SATURDAY:
            
                if (7 < gHour && gHour < 17) {
                    npcs[MAYOR_WIFE].levelIndex = MAYOR_HOUSE;
                    npcs[MAYOR_WIFE].startingCoordinates.y = 0;
                    npcs[MAYOR_WIFE].direction = SOUTHWEST;
                    npcs[MAYOR_WIFE].unk_1E = 1;
                    npcs[MAYOR_WIFE].startingCoordinates.x = -64.0f;
                    npcs[MAYOR_WIFE].startingCoordinates.z = -64.0f;
                    npcs[MAYOR_WIFE].flags |= 1;
                }
                
                break;
            
        }
        
    } else if (7 < gHour && gHour < 17) {
        npcs[MAYOR_WIFE].levelIndex = MAYOR_HOUSE;
        npcs[MAYOR_WIFE].startingCoordinates.y = 0;
        npcs[MAYOR_WIFE].direction = NORTHWEST;
        npcs[MAYOR_WIFE].unk_1E = 0;
        npcs[MAYOR_WIFE].startingCoordinates.x = 64.0f;
        npcs[MAYOR_WIFE].startingCoordinates.z = 16.0f;
        npcs[MAYOR_WIFE].flags |= 1;
    }
    
    npcs[MAYOR_WIFE].movingFlag = npcs[MAYOR_WIFE].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007D9F4);

void func_8007D9F4(void) {

    npcs[LILLIA].unk_1A = 0x40;
    npcs[LILLIA].unk_1B = 0x40;
    npcs[LILLIA].animationIndex2 = 8;
    npcs[LILLIA].animationIndex1 = 0;

    if (gWeather != SUNNY) goto DEFAULT;

    switch (gDayOfWeek) {

        case SUNDAY:

            if (8 < gHour && gHour < 17) { 
                npcs[LILLIA].levelIndex = SQUARE;
                npcs[LILLIA].startingCoordinates.y = 0;
                npcs[LILLIA].direction = NORTHEAST;
                npcs[LILLIA].unk_1E = 0;
                npcs[LILLIA].startingCoordinates.x = -144.0f;
                npcs[LILLIA].startingCoordinates.z = 256.0f;
                npcs[LILLIA].flags |= 1;
            }
            break;

        default:
            break;

        case MONDAY ... SATURDAY:
DEFAULT:
            if (8 < gHour && gHour < 17) {
                npcs[LILLIA].levelIndex = FLOWER_SHOP;
                npcs[LILLIA].startingCoordinates.y = 0;
                npcs[LILLIA].direction = SOUTHWEST;
                npcs[LILLIA].unk_1E = 0;
                npcs[LILLIA].startingCoordinates.x = 64.0f;
                npcs[LILLIA].startingCoordinates.z = -128.0f;
                npcs[LILLIA].flags |= 1;
            }
    
            break;

    }
    
    npcs[LILLIA].movingFlag = npcs[LILLIA].unk_1E;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007DB38);

void func_8007DB38(void) {

    npcs[BASIL].unk_1A = 0x40;
    npcs[BASIL].unk_1B = 0x40;
    npcs[BASIL].animationIndex1 = 0;
    npcs[BASIL].animationIndex2 = 8;

    if (!checkLifeEventBit(BASIL_IN_TOWN)) goto FUNC_END;

    if (gWeather != SUNNY) goto NOT_SUNNY;
            
    switch (gDayOfWeek) {
        case SUNDAY:
            if (8 < gHour && gHour < 17) {
                npcs[BASIL].levelIndex = SQUARE;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = NORTHEAST;
                npcs[BASIL].unk_1E = 0;
                npcs[BASIL].startingCoordinates.x = -176.0f;
                npcs[BASIL].startingCoordinates.z = 256.0f;
                npcs[BASIL].flags |= 1;
            }
            break;
        case THURSDAY:
            if (7  < gHour && gHour < 18) {
                npcs[BASIL].levelIndex = VILLAGE_1;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = SOUTHEAST;
                npcs[BASIL].unk_1E = 0;
                npcs[BASIL].startingCoordinates.x = 128.0f;
                npcs[BASIL].startingCoordinates.z = -416.0f;
                npcs[BASIL].flags |= 1;
            }
            break;
        case FRIDAY:
        case SATURDAY:
            if (7  < gHour && gHour < 18) {
                npcs[BASIL].levelIndex = MOUNTAIN_1;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = NORTHEAST;
                npcs[BASIL].unk_1E = 1;
                npcs[BASIL].startingCoordinates.x = 192.0f;
                npcs[BASIL].startingCoordinates.z = -64.0f;
                npcs[BASIL].flags |= 1;
            }
            break;
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
NOT_SUNNY:
            if (8 < gHour && gHour < 17) {
                npcs[BASIL].levelIndex = FLOWER_SHOP;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = SOUTHWEST;
                npcs[BASIL].unk_1E = 0;
                npcs[BASIL].startingCoordinates.x = -48.0f;
                npcs[BASIL].startingCoordinates.z = -64.0f;
                npcs[BASIL].flags |= 1;
            }
            break;
        default:
            break;
    }
        
    if ((17 < gHour && gHour < 24) && npcs[BASIL].location < 2) {
        npcs[BASIL].levelIndex = 0x3B;
        npcs[BASIL].startingCoordinates.y = 0;
        npcs[BASIL].direction = SOUTHEAST;
        npcs[BASIL].unk_1E = 0;
        npcs[BASIL].startingCoordinates.x = 32.0f;
        npcs[BASIL].startingCoordinates.z = 48.0f;
        npcs[BASIL].flags |= 1;
    }

FUNC_END:
    npcs[BASIL].movingFlag = npcs[BASIL].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setEllenLocation);

void setEllenLocation(void) {

    npcs[ELLEN].unk_1A = 0x40;
    npcs[ELLEN].unk_1B = 0x40;
    npcs[ELLEN].animationIndex1 = 0;
    npcs[ELLEN].animationIndex2 = 8;
    
    if (!checkLifeEventBit(ELLEN_DIED)) {

        if (gWeather == SUNNY && (7 < gHour && gHour < 17)) {

                npcs[ELLEN].levelIndex = VILLAGE_1;
                npcs[ELLEN].startingCoordinates.y = 0;
                npcs[ELLEN].direction = SOUTHWEST;
                npcs[ELLEN].unk_1E = 0;
                npcs[ELLEN].startingCoordinates.x = 320.0f;
                npcs[ELLEN].startingCoordinates.z = 128.0f;
                npcs[ELLEN].flags |= 1;

        } else {

            if (7 < gHour && gHour < 17) {

                npcs[ELLEN].levelIndex = BAKERY;
                npcs[ELLEN].startingCoordinates.y = 0;
                npcs[ELLEN].direction = SOUTHWEST;
                npcs[ELLEN].unk_1E = 0;
                npcs[ELLEN].startingCoordinates.x = -128.0f;
                npcs[ELLEN].startingCoordinates.z = -32.0f;
                npcs[ELLEN].flags |= 1;

            }

        }

    }
    
    npcs[ELLEN].movingFlag = npcs[ELLEN].unk_1E;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007DF14);

void func_8007DF14(void) {

    int temp = gDayOfWeek;
    int temp2;

    npcs[DOUG].unk_1A = 0x40;
    npcs[DOUG].unk_1B = 0x40;
    npcs[DOUG].animationIndex1 = 0;
    npcs[DOUG].animationIndex2 = 8;

    // FIXME: something off here
    if (temp >= SUNDAY && (temp < THURSDAY || temp < 7 && (temp2 = temp) >= FRIDAY) && (7 < gHour && gHour < 17)) {
        npcs[DOUG].levelIndex = RANCH_STORE;
        npcs[DOUG].startingCoordinates.y = 0;
        npcs[DOUG].direction = SOUTHEAST;
        npcs[DOUG].unk_1E = 0;
        npcs[DOUG].startingCoordinates.x = -96.0f;
        npcs[DOUG].startingCoordinates.z = 64.0f;
        npcs[DOUG].flags |= 1;
    }

    if (NIGHTTIME && npcs[DOUG].location < 2) {
        npcs[DOUG].levelIndex = TAVERN;
        npcs[DOUG].startingCoordinates.y = 0;
        npcs[DOUG].direction = NORTHWEST;
        npcs[DOUG].unk_1E = 0;
        npcs[DOUG].startingCoordinates.x = -96.0f;
        npcs[DOUG].startingCoordinates.z = 48.0f;
        npcs[DOUG].flags |= 1;
    }

    npcs[DOUG].movingFlag = npcs[DOUG].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E07C);

void func_8007E07C(void) {

    npcs[GOTZ].unk_1A = 0x40;
    npcs[GOTZ].unk_1B = 0x40;
    npcs[GOTZ].animationIndex1 = 0;
    npcs[GOTZ].animationIndex2 = 8;

    if (gWeather == SUNNY) {

        switch (gSeason) {

            case SPRING:
            case SUMMER:
            case AUTUMN:

                if (8 < gHour && gHour < 17) {

                    npcs[GOTZ].levelIndex = VINEYARD;
                    npcs[GOTZ].startingCoordinates.y = 0.0f;
                    npcs[GOTZ].direction = SOUTHWEST;
                    npcs[GOTZ].unk_1E = 0;
                    npcs[GOTZ].startingCoordinates.x = 128.0f;
                    npcs[GOTZ].startingCoordinates.z = -128.0f;
                    npcs[GOTZ].flags |= 1;
                    
                }

                if (gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

                    if (8 < gHour && gHour < 17) {
    
                        npcs[GOTZ].levelIndex = VINEYARD;
                        npcs[GOTZ].startingCoordinates.y = 0.0f;
                        npcs[GOTZ].direction = SOUTHWEST;
                        npcs[GOTZ].unk_1E = 3;
                        npcs[GOTZ].startingCoordinates.x = -192.0f;
                        npcs[GOTZ].startingCoordinates.z = -64.0f;
                        npcs[GOTZ].flags |= 1;
                        
                    }
                    
                }
                
                break;

            case WINTER:
                
                if (8 < gHour && gHour < 17) {

                    npcs[GOTZ].levelIndex = VINEYARD_HOUSE;
                    npcs[GOTZ].startingCoordinates.y = 0.0f;
                    npcs[GOTZ].direction = SOUTH;
                    npcs[GOTZ].unk_1E = 0;
                    npcs[GOTZ].startingCoordinates.x = -160.0f;
                    npcs[GOTZ].startingCoordinates.z = -32.0f;
                    npcs[GOTZ].flags |= 1;
                    
                }
                
                break;
            
        }
        
    } else {

        if (8 < gHour && gHour < 17) {

            npcs[GOTZ].levelIndex = VINEYARD_HOUSE;
            npcs[GOTZ].startingCoordinates.y = 0.0f;
            npcs[GOTZ].direction = SOUTH;
            npcs[GOTZ].unk_1E = 0;
            npcs[GOTZ].startingCoordinates.x = -160.0f;
            npcs[GOTZ].startingCoordinates.z = -32.0f;
            npcs[GOTZ].flags |= 1;
            
        }
        
    } 

    if (17 < gHour && gHour < 24) {

        if (npcs[GOTZ].location < 2) {
            
            npcs[GOTZ].levelIndex = TAVERN;
            npcs[GOTZ].startingCoordinates.y = 0.0f;
            npcs[GOTZ].direction = SOUTHWEST;
            npcs[GOTZ].unk_1E = 0;
            npcs[GOTZ].startingCoordinates.x = 80.0f;
            npcs[GOTZ].startingCoordinates.z = -160.0f;
            npcs[GOTZ].flags |= 1;

        
        }
            
    }
        
    npcs[GOTZ].movingFlag = npcs[GOTZ].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E2E4);

void func_8007E2E4(void) {

    npcs[SASHA].unk_1A = 0x40;
    npcs[SASHA].unk_1B = 0x40;
    npcs[SASHA].animationIndex1 = 0;
    npcs[SASHA].animationIndex2 = 8;

    if (7 < gHour && gHour < 17) {
        
        npcs[SASHA].levelIndex = VINEYARD_HOUSE;
        npcs[SASHA].startingCoordinates.y = 0.0f;
        npcs[SASHA].direction = SOUTHWEST;
        npcs[SASHA].unk_1E = 0;
        npcs[SASHA].startingCoordinates.x = -128.0f;
        npcs[SASHA].startingCoordinates.z = -96.0f;
        npcs[SASHA].flags |= 1;
        
    }

    if (gWeather == SUNNY && gSeason == AUTUMN && 7 < gDayOfMonth && gDayOfMonth < 11) {

        if (8 < gHour && gHour < 17) {
        
            npcs[SASHA].levelIndex = VINEYARD;
            npcs[SASHA].startingCoordinates.y = 0.0f;
            npcs[SASHA].direction = SOUTHWEST;
            npcs[SASHA].unk_1E = 3;
            npcs[SASHA].startingCoordinates.x = -192.0f;
            npcs[SASHA].startingCoordinates.z = 64.0f;
            npcs[SASHA].flags |= 1;
            
        }
        
    }
    
    npcs[SASHA].movingFlag = npcs[SASHA].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E440);

void func_8007E440(void) {

    npcs[POTION_SHOP_DEALER].unk_1A = 0x40;
    npcs[POTION_SHOP_DEALER].animationIndex2 = 8;
    npcs[POTION_SHOP_DEALER].unk_1B = 0x40;
    npcs[POTION_SHOP_DEALER].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
                
                if (8 < gHour && gHour < 17) {

                    npcs[POTION_SHOP_DEALER].levelIndex = POTION_SHOP;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
                    npcs[POTION_SHOP_DEALER].direction = SOUTHEAST;
                    npcs[POTION_SHOP_DEALER].unk_1E = 0;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.x = -96.0f;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.z = -16.0f;
                    npcs[POTION_SHOP_DEALER].flags |= 1;
                        
                    }
                
                break;
            
            case SATURDAY:
            case SUNDAY:

                if (npcs[POTION_SHOP_DEALER].location < 2) {

                    if (5 < gHour && gHour < 16) {
            
                        npcs[POTION_SHOP_DEALER].levelIndex = MOUNTAIN_2;
                        npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
                        npcs[POTION_SHOP_DEALER].direction = SOUTH;
                        npcs[POTION_SHOP_DEALER].unk_1E = 1;
                        npcs[POTION_SHOP_DEALER].startingCoordinates.x = -112.0f;
                        npcs[POTION_SHOP_DEALER].startingCoordinates.z = 192.0f;
                        npcs[POTION_SHOP_DEALER].flags |= 1;
                        
                    }
                    
                } else if (8 < gHour && gHour < 17) {
            
                    npcs[POTION_SHOP_DEALER].levelIndex = CAVE;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
                    npcs[POTION_SHOP_DEALER].direction = SOUTH;
                    npcs[POTION_SHOP_DEALER].unk_1E = 0;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.x = -288.0f;
                    npcs[POTION_SHOP_DEALER].startingCoordinates.z = -224.0f;
                    npcs[POTION_SHOP_DEALER].flags |= 1;
                    
                }
    
                break;
            
        }
        
    } else if (8 < gHour && gHour < 17) {

        npcs[POTION_SHOP_DEALER].levelIndex = POTION_SHOP;
        npcs[POTION_SHOP_DEALER].startingCoordinates.y = 0.0f;
        npcs[POTION_SHOP_DEALER].direction = SOUTHEAST;
        npcs[POTION_SHOP_DEALER].unk_1E = 0;
        npcs[POTION_SHOP_DEALER].startingCoordinates.x = -96.0f;
        npcs[POTION_SHOP_DEALER].startingCoordinates.z = -16.0f;
        npcs[POTION_SHOP_DEALER].flags |= 1;
            
    }
        
    npcs[POTION_SHOP_DEALER].movingFlag = npcs[POTION_SHOP_DEALER].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E600);

void func_8007E600(void) {

    // FIXME: seems fake
    int temp = 0x40;

    npcs[KENT].unk_1A = temp;
    npcs[KENT].unk_1B = temp;
    npcs[KENT].animationIndex2 = 8;
    npcs[KENT].animationIndex1 = 0;

    if (gWeather == SUNNY) {
        
        switch (gDayOfWeek) {

            case SUNDAY:

                switch (gSeason) {

                    case SPRING:
                    case AUTUMN:

                        switch (npcs[KENT].location) {

                            case 0:
                            case 1:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[KENT].levelIndex = MOUNTAIN_2;
                                    npcs[KENT].startingCoordinates.y = 0.0f;
                                    npcs[KENT].direction = SOUTHWEST;
                                    npcs[KENT].unk_1E = 1;
                                    npcs[KENT].startingCoordinates.x = -160.0f;
                                    npcs[KENT].startingCoordinates.z = -160.0f;
                                    npcs[KENT].flags |= 1;
                                    
                                    setSpecialDialogueBit(0x137);
                                    
                                }
                                
                                break;

                            case 2:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[KENT].levelIndex = LIBRARY;
                                    npcs[KENT].startingCoordinates.y = 0.0f;
                                    npcs[KENT].startingCoordinates.z = 0.0f;
                                    npcs[KENT].direction = SOUTHEAST;
                                    npcs[KENT].unk_1E = 1;
                                    npcs[KENT].startingCoordinates.x = -32.0f;
                                    npcs[KENT].flags |= 1;
                                    
                                }
           
                                break;

                            case 3:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[KENT].levelIndex = VILLAGE_2;
                                    npcs[KENT].startingCoordinates.y = 0.0f;
                                    npcs[KENT].startingCoordinates.z = 0.0f;
                                    npcs[KENT].direction = SOUTH;
                                    npcs[KENT].unk_1E = 0;
                                    npcs[KENT].startingCoordinates.x = 288.0f;
                                    npcs[KENT].flags |= 1;
                                    
                                }

                                break;
                            
                        }
                        
                        break;

                    case SUMMER:

                        if (npcs[KENT].location < 2) {

                            if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = BEACH;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHEAST;
                                npcs[KENT].unk_1E = 0;
                                npcs[KENT].startingCoordinates.x = -224.0f;
                                npcs[KENT].startingCoordinates.z = -96.0f;
                                npcs[KENT].flags |= 1;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = MOUNTAIN_2;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHWEST;
                                npcs[KENT].unk_1E = 1;
                                npcs[KENT].startingCoordinates.x = -128.0f;
                                npcs[KENT].startingCoordinates.z = -160.0f;
                                npcs[KENT].flags |= 1;

                                setSpecialDialogueBit(0x137);
                                 
                             }
                            
                        }

                        break;

                    case WINTER:

                        if (npcs[KENT].location < 2) {

                            if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = POTION_SHOP_BEDROOM;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHWEST;
                                npcs[KENT].unk_1E = 0;
                                npcs[KENT].startingCoordinates.x = 32.0f;
                                npcs[KENT].startingCoordinates.z = -32.0f;
                                npcs[KENT].flags |= 1;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                npcs[KENT].levelIndex = MOUNTAIN_2;
                                npcs[KENT].startingCoordinates.y = 0.0f;
                                npcs[KENT].direction = SOUTHWEST;
                                npcs[KENT].unk_1E = 1;
                                npcs[KENT].startingCoordinates.x = -128.0f;
                                npcs[KENT].startingCoordinates.z = -160.0f;
                                npcs[KENT].flags |= 1;

                                setSpecialDialogueBit(0x137);
                                 
                             }
                            
                        }

                        break;

                }
                
                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (7 < gHour && gHour < 15) {

                    npcs[KENT].levelIndex = CHURCH;
                    npcs[KENT].startingCoordinates.y = 0.0f;
                    npcs[KENT].direction = 4;
                    npcs[KENT].unk_1E = 0;
                    npcs[KENT].startingCoordinates.x = 48.0f;
                    npcs[KENT].startingCoordinates.z = -32.0f;
                    npcs[KENT].flags |= 1;
                     
                 } 
                
                if (14 < gHour && gHour < 17) {
                     
                    npcs[KENT].levelIndex = VILLAGE_1;
                    npcs[KENT].startingCoordinates.x = 0.0f;
                    npcs[KENT].startingCoordinates.y = 0.0f;
                    npcs[KENT].direction = NORTHWEST;
                    npcs[KENT].unk_1E = 1;
                    npcs[KENT].startingCoordinates.z = 352.0f;
                    npcs[KENT].flags |= 1;
                             
                 } 
                
                break;

            case SATURDAY:

                if (npcs[KENT].location < 2) {

                    if (8 < gHour && gHour < 15) {

                        npcs[KENT].levelIndex = SQUARE;
                        npcs[KENT].startingCoordinates.x = 0.0f;
                        npcs[KENT].startingCoordinates.y = 0.0f;
                        npcs[KENT].startingCoordinates.z = 0.0f;
                        npcs[KENT].direction = NORTHEAST;
                        npcs[KENT].unk_1E = 2;
                        npcs[KENT].flags |= 1;
                        
                    }
                    
                } else {

                    if (8 < gHour && gHour < 15) {
                                    
                        npcs[KENT].levelIndex = MOUNTAIN_1;
                        npcs[KENT].startingCoordinates.y = 0.0f;
                        npcs[KENT].direction = SOUTHEAST;
                        npcs[KENT].unk_1E = 1;
                        npcs[KENT].startingCoordinates.x = 64.0f;
                        npcs[KENT].startingCoordinates.z = -96.0f;
                        npcs[KENT].flags |= 1;

                        setSpecialDialogueBit(0x137);
                        
                    }
                    
                }
                
                break;
            
        }
        
    } else if (gWeather == SNOW && npcs[KENT].location >= 2) {
        
        if (7 < gHour && gHour < 17) {

            npcs[KENT].levelIndex = VILLAGE_2;
            npcs[KENT].startingCoordinates.y = 0.0f;
            npcs[KENT].direction = SOUTHWEST;
            npcs[KENT].unk_1E = 0;
            npcs[KENT].startingCoordinates.x = 412.0f;
            npcs[KENT].startingCoordinates.z = -64.0f;
            npcs[KENT].flags |= 1;
            
        }
        
    } else if (7 < gHour && gHour < 17) {

        npcs[KENT].levelIndex = temp;
        npcs[KENT].startingCoordinates.x = 0.0f;
        npcs[KENT].startingCoordinates.y = 0.0f;
        npcs[KENT].direction = NORTHWEST;
        npcs[KENT].unk_1E = 0;
        npcs[KENT].startingCoordinates.z = -16.0f;
        npcs[KENT].flags |= 1;
            
    }

    npcs[KENT].movingFlag = npcs[KENT].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007ECE4);

void func_8007ECE4(void) {
    
    // FIXME: seems fake
    int temp = 0x40;

    npcs[STU].unk_1A = temp;
    npcs[STU].unk_1B = temp;
    npcs[STU].animationIndex2 = 8;
    npcs[STU].animationIndex1 = 0;

    if (gWeather == SUNNY) {
        
        switch (gDayOfWeek) {

            case SUNDAY:

                switch (gSeason) {

                    case SPRING:
                    case AUTUMN:

                        switch (npcs[STU].location) {

                            case 0:
                            case 1:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[STU].levelIndex = MOUNTAIN_2;
                                    npcs[STU].startingCoordinates.y = 0.0f;
                                    npcs[STU].direction = SOUTHWEST;
                                    npcs[STU].unk_1E = 1;
                                    npcs[STU].startingCoordinates.x = -160.0f;
                                    npcs[STU].startingCoordinates.z = -160.0f;
                                    npcs[STU].flags |= 1;
                                    
                                }
                                
                                break;

                            case 2:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[STU].levelIndex = LIBRARY;
                                    npcs[STU].startingCoordinates.y = 0.0f;
                                    npcs[STU].direction = SOUTHEAST;
                                    npcs[STU].unk_1E = 1;
                                    npcs[STU].startingCoordinates.x = -64.0f;
                                    npcs[STU].startingCoordinates.z = -64.0f;
                                    npcs[STU].flags |= 1;
                                    
                                }
           
                                break;

                            case 3:

                                if (7 < gHour && gHour < 16) {
                
                                    npcs[STU].levelIndex = VILLAGE_2;
                                    npcs[STU].startingCoordinates.y = 0.0f;
                                    npcs[STU].startingCoordinates.z = 0.0f;
                                    npcs[STU].direction = SOUTH;
                                    npcs[STU].unk_1E = 0;
                                    npcs[STU].startingCoordinates.x = 256.0f;
                                    npcs[STU].flags |= 1;
                                    
                                }

                                break;
                            
                        }
                        
                        break;

                    case SUMMER:

                        if (npcs[STU].location < 2) {

                            if (7 < gHour && gHour < 16) {

                                npcs[STU].levelIndex = BEACH;
                                npcs[STU].startingCoordinates.y = 0.0f;
                                npcs[STU].direction = SOUTHEAST;
                                npcs[STU].unk_1E = 0;
                                npcs[STU].startingCoordinates.x = -224.0f;
                                npcs[STU].startingCoordinates.z = -128.0f;
                                npcs[STU].flags |= 1;
                            
                            } 
                            
                        } else {

                             if (7 < gHour && gHour < 16) {

                                npcs[STU].levelIndex = MOUNTAIN_2;
                                npcs[STU].startingCoordinates.y = 0.0f;
                                npcs[STU].direction = SOUTHWEST;
                                npcs[STU].unk_1E = 1;
                                npcs[STU].startingCoordinates.x = -160.0f;
                                npcs[STU].startingCoordinates.z = -160.0f;
                                npcs[STU].flags |= 1;

                             }
                            
                        }

                        break;

                    case WINTER:

                        if (7 < gHour && gHour < 16) {

                            npcs[STU].levelIndex = POTION_SHOP_BEDROOM;
                            npcs[STU].startingCoordinates.x = 0.0f;
                            npcs[STU].startingCoordinates.y = 0.0f;
                            npcs[STU].direction = SOUTHWEST;
                            npcs[STU].unk_1E = 0;
                            npcs[STU].startingCoordinates.z = -32.0f;
                            npcs[STU].flags |= 1;
                        
                        } 

                        break;

                }
                
                break;

            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:

                if (7 < gHour && gHour < 15) {

                    npcs[STU].levelIndex = CHURCH;
                    npcs[STU].startingCoordinates.y = 0.0f;
                    npcs[STU].direction = 4;
                    npcs[STU].unk_1E = 0;
                    npcs[STU].startingCoordinates.x = 80.0f;
                    npcs[STU].startingCoordinates.z = -32.0f;
                    npcs[STU].flags |= 1;
                     
                 } 
                
                if (14 < gHour && gHour < 17) {
                     
                    npcs[STU].levelIndex = VILLAGE_1;
                    npcs[STU].startingCoordinates.y = 0.0f;
                    npcs[STU].direction = NORTHWEST;
                    npcs[STU].unk_1E = 1;
                    npcs[STU].startingCoordinates.x = 32.0f;
                    npcs[STU].startingCoordinates.z = 352.0f;
                    npcs[STU].flags |= 1;
                             
                 } 
                
                break;

            case SATURDAY:

                if (npcs[STU].location < 2) {

                    if (8 < gHour && gHour < 15) {

                        npcs[STU].levelIndex = SQUARE;
                        npcs[STU].startingCoordinates.y = 0.0f;
                        npcs[STU].startingCoordinates.z = 0.0f;
                        npcs[STU].direction = NORTHEAST;
                        npcs[STU].unk_1E = 1;
                        npcs[STU].startingCoordinates.x = -32.0f;
                        npcs[STU].flags |= 1;
                        
                    }
                    
                } else {

                    if (8 < gHour && gHour < 15) {
                                    
                        npcs[STU].levelIndex = MOUNTAIN_1;
                        npcs[STU].startingCoordinates.y = 0.0f;
                        npcs[STU].direction = SOUTHEAST;
                        npcs[STU].unk_1E = 1;
                        npcs[STU].startingCoordinates.x = 64.0f;
                        npcs[STU].startingCoordinates.z = -64.0f;
                        npcs[STU].flags |= 1;
                        
                    }
                    
                }
                
                break;
            
        }
        
    } else if (gWeather == SNOW) {

        if (npcs[STU].location < 2) {
            
            if (7 < gHour && gHour < 17) {
    
                npcs[STU].levelIndex = POTION_SHOP_BEDROOM;
                npcs[STU].startingCoordinates.x = 0.0f;
                npcs[STU].startingCoordinates.y = 0.0f;
                npcs[STU].direction = SOUTHWEST;
                npcs[STU].unk_1E = 0;
                npcs[STU].startingCoordinates.z = -32.0f;
                npcs[STU].flags |= 1;
                
            }
            
        } else {

            if (7 < gHour && gHour < 17) {
    
                npcs[STU].levelIndex = VILLAGE_2;
                npcs[STU].startingCoordinates.y = 0.0f;
                npcs[STU].direction = SOUTHWEST;
                npcs[STU].unk_1E = 0;
                npcs[STU].startingCoordinates.x = 444.0f;
                npcs[STU].startingCoordinates.z = -64.0f;
                npcs[STU].flags |= 1;
                
            }
            
        }
        
        
    } else if (7 < gHour && gHour < 17) {

        npcs[STU].levelIndex = temp;
        npcs[STU].startingCoordinates.x = 0.0f;
        npcs[STU].startingCoordinates.y = 0.0f;
        npcs[STU].direction = NORTHWEST;
        npcs[STU].unk_1E = 0;
        npcs[STU].startingCoordinates.z = -48.0f;
        npcs[STU].flags |= 1;
            
    }

    npcs[STU].movingFlag = npcs[STU].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007F378);

void func_8007F378(void) {

    npcs[MIDWIFE].unk_1A = 0x40;
    npcs[MIDWIFE].unk_1B = 0x40;
    npcs[MIDWIFE].animationIndex1 = 0;
    npcs[MIDWIFE].animationIndex2 = 8;

    if (!checkDailyEventBit(0x52)) {

        if (gWeather == SUNNY) {

            switch (gDayOfWeek) {
            
                case SUNDAY:
    
                    if (8 < gHour && gHour < 17) {
            
                        npcs[MIDWIFE].levelIndex = VILLAGE_2;
                        npcs[MIDWIFE].startingCoordinates.y = 0.0f;
                        npcs[MIDWIFE].direction = SOUTHWEST;
                        npcs[MIDWIFE].unk_1E = 1;
                        npcs[MIDWIFE].startingCoordinates.x = 48.0f;
                        npcs[MIDWIFE].startingCoordinates.z = -64.0f;
                        npcs[MIDWIFE].flags |= 1;
                        
                    }
                    
                    break;
                
                case MONDAY:
                case TUESDAY:
                case WEDNESDAY:
                case THURSDAY:
                case FRIDAY:
                case SATURDAY:
                    
                    if (8 < gHour && gHour < 17) {
    
                        npcs[MIDWIFE].levelIndex = MIDWIFE_HOUSE;
                        npcs[MIDWIFE].startingCoordinates.y = 0.0f;
                        npcs[MIDWIFE].direction = SOUTHWEST;
                        npcs[MIDWIFE].unk_1E = 0;
                        npcs[MIDWIFE].startingCoordinates.x = -16.0f;
                        npcs[MIDWIFE].startingCoordinates.z = -32.0f;
                        npcs[MIDWIFE].flags |= 1;
                        
                    }
                    
                    break;
                    
            
            }
            
        } else if (8 < gHour && gHour < 17) {

            npcs[MIDWIFE].levelIndex = MIDWIFE_HOUSE;
            npcs[MIDWIFE].startingCoordinates.y = 0.0f;
            npcs[MIDWIFE].direction = SOUTHWEST;
            npcs[MIDWIFE].unk_1E = 0;
            npcs[MIDWIFE].startingCoordinates.x = -16.0f;
            npcs[MIDWIFE].startingCoordinates.z = -32.0f;
            npcs[MIDWIFE].flags |= 1;
            
        }
            
    }
    
    npcs[MIDWIFE].movingFlag = npcs[MIDWIFE].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007F4E0);

void func_8007F4E0(void) {
    
    npcs[MAY].unk_1A = 0x40;
    npcs[MAY].unk_1B = 0x40;
    npcs[MAY].animationIndex2 = 8;
    npcs[MAY].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case SUNDAY:

                if (npcs[MAY].location < 2) {
                    
                    if (8 < gHour && gHour < 15) {
            
                        npcs[MAY].levelIndex = SQUARE;
                        npcs[MAY].startingCoordinates.y = 0.0f;
                        npcs[MAY].direction = NORTHEAST;
                        npcs[MAY].unk_1E = 0;
                        npcs[MAY].startingCoordinates.x = -32.0f;
                        npcs[MAY].startingCoordinates.z = -320.0f;
                        npcs[MAY].flags |= 1;
                        
                    }
                    
                } else {
                    
                    if (8 < gHour && gHour < 15) {
            
                        npcs[MAY].levelIndex = MOUNTAIN_2;
                        npcs[MAY].startingCoordinates.y = 0.0f;
                        npcs[MAY].direction = SOUTHEAST;
                        npcs[MAY].unk_1E = 0;
                        npcs[MAY].startingCoordinates.x = -160.0f;
                        npcs[MAY].startingCoordinates.z = 288.0f;
                        npcs[MAY].flags |= 1;
                        
                    }
                }
    
                break;
            
            case MONDAY ... FRIDAY:

                if (7 < gHour && gHour < 15) {
        
                    npcs[MAY].levelIndex = CHURCH;
                    npcs[MAY].startingCoordinates.y = 0.0f;
                    npcs[MAY].direction = NORTHEAST;
                    npcs[MAY].unk_1E = 0;
                    npcs[MAY].startingCoordinates.x = -96.0f;
                    npcs[MAY].startingCoordinates.z = -32.0f;
                    npcs[MAY].flags |= 1;
                    
                }

                if (14 < gHour && gHour < 17) {
                    
                    npcs[MAY].levelIndex = VILLAGE_2;
                    npcs[MAY].startingCoordinates.y = 0.0f;
                    npcs[MAY].direction = NORTHEAST;
                    npcs[MAY].unk_1E = 0;
                    npcs[MAY].startingCoordinates.x = -64.0f;
                    npcs[MAY].startingCoordinates.z = 64.0f;
                    npcs[MAY].flags |= 1;
                    
                }
                
                break;
                
            case SATURDAY:

                if (8 < gHour && gHour < 15) {
        
                    npcs[MAY].levelIndex = VILLAGE_2;
                    npcs[MAY].startingCoordinates.y = 0.0f;
                    npcs[MAY].direction = SOUTHWEST;
                    npcs[MAY].unk_1E = 1;
                    npcs[MAY].startingCoordinates.x = 48.f;
                    npcs[MAY].startingCoordinates.z = -64.0f;
                    npcs[MAY].flags |= 1;
                    
                }
            
                break;
            
        }
        
    } else if (npcs[MAY].location < 2) {

        if (8 < gHour && gHour < 15) {
    
            npcs[MAY].levelIndex = LIBRARY;
            npcs[MAY].startingCoordinates.x = 0.0f;
            npcs[MAY].startingCoordinates.y = 0.0f;
            npcs[MAY].direction = SOUTHEAST;
            npcs[MAY].unk_1E = 0;
            npcs[MAY].startingCoordinates.z = -64.0f;
            npcs[MAY].flags |= 1;
        
        }
        
    } else {
        
        if (8 < gHour && gHour < 15) {
    
            npcs[MAY].levelIndex = MIDWIFE_HOUSE;
            npcs[MAY].startingCoordinates.y = 0.0f;
            npcs[MAY].direction = SOUTHEAST;
            npcs[MAY].unk_1E = 0;
            npcs[MAY].startingCoordinates.x = -96.0f;
            npcs[MAY].startingCoordinates.z = -32.0f;
            npcs[MAY].flags |= 1;
        
        }
        
    }
        
    npcs[MAY].movingFlag = npcs[MAY].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007F81C);

void func_8007F81C(void) {

    npcs[RICK].unk_1A = 0x40;
    npcs[RICK].unk_1B = 0x40;
    npcs[RICK].animationIndex2 = 8;
    npcs[RICK].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case SUNDAY:

                if (npcs[RICK].location < 2) {
                    
                    if (9 < gHour && gHour < 18) {
            
                        npcs[RICK].levelIndex = SQUARE;
                        npcs[RICK].startingCoordinates.y = 0.0f;
                        npcs[RICK].direction = SOUTHEAST;
                        npcs[RICK].unk_1E = 0;
                        npcs[RICK].startingCoordinates.x = -192.0f;
                        npcs[RICK].startingCoordinates.z = -32.0f;
                        npcs[RICK].flags |= 1;
                        
                    }
                    
                }
    
                break;
            
            case MONDAY:
            case TUESDAY:

                if (9 < gHour && gHour < 18) {
        
                    npcs[RICK].levelIndex = RICK_STORE;
                    npcs[RICK].startingCoordinates.x = 0.0f;
                    npcs[RICK].startingCoordinates.y = 0.0f;
                    npcs[RICK].startingCoordinates.z = 0.0f;
                    npcs[RICK].direction = SOUTHWEST;
                    npcs[RICK].unk_1E = 1;
                    npcs[RICK].flags |= 1;
                    
                }
                
                break;
            
            case THURSDAY:
            case FRIDAY:
                
                if (9 < gHour && gHour < 18) {
        
                    npcs[RICK].levelIndex = RICK_STORE;
                    npcs[RICK].startingCoordinates.y = 0.0f;
                    npcs[RICK].direction = SOUTHWEST;
                    npcs[RICK].unk_1E = 0;
                    npcs[RICK].startingCoordinates.x = -96.0f;
                    npcs[RICK].startingCoordinates.z = -96.0f;
                    npcs[RICK].flags |= 1;
                    
                }
            
                break;
            
            case SATURDAY:

                if (npcs[RICK].location < 2) {
                
                    if (9 < gHour && gHour < 18) {
    
                        npcs[RICK].levelIndex = VILLAGE_1;
                        npcs[RICK].startingCoordinates.y = 0.0f;
                        npcs[RICK].direction = SOUTHEAST;
                        npcs[RICK].unk_1E = 0;
                        npcs[RICK].startingCoordinates.x = -96.0f;
                        npcs[RICK].startingCoordinates.z = 160.0f;
                        npcs[RICK].flags |= 1;
                            
                    }
                    
                }
                
                break;
            
            
        }
        
    } 
        
    npcs[RICK].movingFlag = npcs[RICK].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FA44);

void func_8007FA44(void) {

    npcs[PASTOR].unk_1A = 0x40;
    npcs[PASTOR].unk_1B = 0x40;
    npcs[PASTOR].animationIndex2 = 8;
    npcs[PASTOR].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SUNDAY:

                if (7 < gHour && gHour < 17) {
        
                    npcs[PASTOR].levelIndex = CHURCH;
                    npcs[PASTOR].startingCoordinates.y = 0.0f;
                    npcs[PASTOR].direction = SOUTHWEST;
                    npcs[PASTOR].unk_1E = 0;
                    npcs[PASTOR].startingCoordinates.x = -16.0f;
                    npcs[PASTOR].startingCoordinates.z = -192.0f;
                    npcs[PASTOR].flags |= 1;
                    
                }
                
    
                break;
            
            case SATURDAY:
                
                if (7 < gHour && gHour < 17) {

                    npcs[PASTOR].levelIndex = VILLAGE_1;
                    npcs[PASTOR].startingCoordinates.y = 0.0f;
                    npcs[PASTOR].direction = SOUTHWEST;
                    npcs[PASTOR].unk_1E = 0;
                    npcs[PASTOR].startingCoordinates.x = -128.0f;
                    npcs[PASTOR].startingCoordinates.z = -128.0f;
                    npcs[PASTOR].flags |= 1;
                        
                    }
                
                break;
            
            
        }
        
    } else if (7 < gHour && gHour < 17) {

        npcs[PASTOR].levelIndex = CHURCH;
        npcs[PASTOR].startingCoordinates.x = 0.0f;
        npcs[PASTOR].startingCoordinates.y = 0.0f;
        npcs[PASTOR].startingCoordinates.z = 0.0f;
        npcs[PASTOR].direction = SOUTHWEST;
        npcs[PASTOR].unk_1E = 1;
        npcs[PASTOR].flags |= 1;
            
    }
        
    npcs[PASTOR].movingFlag = npcs[PASTOR].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FC0C);

void func_8007FC0C(void) {

    npcs[SHIPPER].unk_1A = 0x40;
    npcs[SHIPPER].unk_1B = 0x40;
    npcs[SHIPPER].animationIndex1 = 0;
    npcs[SHIPPER].animationIndex2 = 8;

    if ((17 < gHour && gHour < 24) && npcs[SHIPPER].location < 2) {
        
        npcs[SHIPPER].levelIndex = TAVERN;
        npcs[SHIPPER].startingCoordinates.y = 0.0f;
        npcs[SHIPPER].direction = NORTHWEST;
        npcs[SHIPPER].unk_1E = 0;
    
        npcs[SHIPPER].startingCoordinates.x = 128.0f;
        npcs[SHIPPER].startingCoordinates.z = 64.0f;
    
        npcs[SHIPPER].flags |= 1;

    }

    npcs[SHIPPER].movingFlag =  npcs[SHIPPER].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FCD8);

void func_8007FCD8(void) {

    npcs[SAIBARA].unk_1A = 0x40;
    npcs[SAIBARA].unk_1B = 0x40;
    npcs[SAIBARA].animationIndex2 = 8;
    npcs[SAIBARA].animationIndex1 = 0;

    if (gWeather == SUNNY) {

        switch (gDayOfWeek) {
            
            case MONDAY:
                
                if (7 < gHour && gHour < 17) {
        
                    npcs[SAIBARA].levelIndex = TOP_OF_MOUNTAIN_1;
                    npcs[SAIBARA].startingCoordinates.y = 0.0f;
                    npcs[SAIBARA].startingCoordinates.z = 0.0f;
                    npcs[SAIBARA].direction = SOUTHEAST;
                    npcs[SAIBARA].unk_1E = 0;
                    npcs[SAIBARA].startingCoordinates.x = -128.0f;
                    npcs[SAIBARA].flags |= 1;
                    
                }
                break;

            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
                
                if (7 < gHour && gHour < 17) {
        
                    npcs[SAIBARA].levelIndex = SOUVENIR_SHOP;
                    npcs[SAIBARA].startingCoordinates.y = 0.0f;
                    npcs[SAIBARA].direction = SOUTHWEST;
                    npcs[SAIBARA].unk_1E = 0;
                    npcs[SAIBARA].startingCoordinates.x = 16.0f;
                    npcs[SAIBARA].startingCoordinates.z = -64.0f;
                    npcs[SAIBARA].flags |= 1;
                    
                }

                break;
            
        }
        
    } else {

        if (7 < gHour && gHour < 17) {

            npcs[SAIBARA].levelIndex = SOUVENIR_SHOP;
            npcs[SAIBARA].startingCoordinates.y = 0.0f;
            npcs[SAIBARA].direction = SOUTHWEST;
            npcs[SAIBARA].unk_1E = 0;
            npcs[SAIBARA].startingCoordinates.x = 16.0f;
            npcs[SAIBARA].startingCoordinates.z = -64.0f;
            npcs[SAIBARA].flags |= 1;
            
        }
        
    }

    npcs[SAIBARA].movingFlag = npcs[SAIBARA].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FE40);

void func_8007FE40(void) { 

    int temp = gDayOfWeek;

    npcs[DUKE].unk_1A = 0x40;
    npcs[DUKE].unk_1B = 0x40;
    npcs[DUKE].animationIndex1 = 0;
    npcs[DUKE].animationIndex2 = 8;

    switch (temp) {
        
        case MONDAY ... SATURDAY:
            if (17 < gHour && gHour < 24) {
                npcs[DUKE].levelIndex = TAVERN;
                npcs[DUKE].startingCoordinates.y = 0.0f;
                npcs[DUKE].direction = 0;
                npcs[DUKE].unk_1E = 0;
            
                npcs[DUKE].startingCoordinates.x = -128.0f;
                npcs[DUKE].startingCoordinates.z = -128.0f;
            
                npcs[DUKE].flags |= 1;
            }
            break;

        default:
            break;
        
    }

    npcs[DUKE].movingFlag =  npcs[DUKE].unk_1E;

}

// alternate without switch
/*
void func_8007FE40(void) {
    
        int temp = gDayOfWeek;

        npcs[DUKE].unk_1A = 0x40;
        npcs[DUKE].unk_1B = 0x40;
        npcs[DUKE].animationIndex1 = 0;
        npcs[DUKE].animationIndex2 = 8;

        if (temp < 7 && gDayOfWeek && (17 < gHour && gHour < 24)) {
            
        npcs[DUKE].levelIndex = TAVERN;
        npcs[DUKE].startingCoordinates.y = 0.0f;
        npcs[DUKE].direction = 0;
        npcs[DUKE].unk_1E = 0;

        npcs[DUKE].startingCoordinates.x = -128.0f;
        npcs[DUKE].startingCoordinates.z = -128.0f;
        
        npcs[DUKE].flags |= 1;
        
    }

    npcs[DUKE].movingFlag =  npcs[DUKE].unk_1E;
    
}
*/

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FF04);

void func_8007FF04(void) {

    npcs[GREG].unk_1A = 0x40;
    npcs[GREG].unk_1B = 0x40;
    npcs[GREG].animationIndex1 = 0;
    npcs[GREG].animationIndex2 = 8;

    if (gSeason != WINTER && checkHaveTool(FISHING_POLE) && gWeather == SUNNY) {

        switch (gDayOfWeek) {

            case TUESDAY:
            case WEDNESDAY:
            case THURSDAY:
            case FRIDAY:
            case SATURDAY:
            case SUNDAY:
                
                if (npcs[GREG].location < 2) {

                    if (5 < gHour && gHour < 17) {
                        
                        npcs[GREG].levelIndex = MOUNTAIN_1;
                
                        npcs[GREG].startingCoordinates.y = 0;
                        npcs[GREG].direction = NORTHWEST;
                        npcs[GREG].unk_1E = 0;
                        
                        npcs[GREG].startingCoordinates.x = -160.0f;
                        npcs[GREG].startingCoordinates.z = -128.0f;
                
                        npcs[GREG].flags |= 1;
                        
                    }
                    
                } else {

                    if (5 < gHour && gHour < 17) {

                        npcs[GREG].levelIndex = MOUNTAIN_2;
                
                        npcs[GREG].startingCoordinates.y = 0;
                        npcs[GREG].direction = SOUTHEAST;
                        npcs[GREG].unk_1E = 0;
                        
                        npcs[GREG].startingCoordinates.x = 96.0f;
                        npcs[GREG].startingCoordinates.z = 32.0f;
                
                        npcs[GREG].flags |= 1;
                        
                    }
                    
                }
                
                break;
                    
            case MONDAY:
    
                if (6 < gHour && gHour < 17) {    
                
                    npcs[GREG].levelIndex = BEACH;
            
                    npcs[GREG].startingCoordinates.y = 0;
                    npcs[GREG].direction = SOUTHEAST;
                    npcs[GREG].unk_1E = 0;
                    
                    npcs[GREG].startingCoordinates.x = -96.0f;
                    npcs[GREG].startingCoordinates.z = 176.0f;
            
                    npcs[GREG].flags |= 1;
                    
                }
                    
                break;
            
        }
        
    }

    npcs[GREG].movingFlag = npcs[GREG].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800800D0);

void func_800800D0(void) {

    npcs[CARPENTER_1].unk_1A = 0x40;
    npcs[CARPENTER_1].unk_1B = 0x40;
    npcs[CARPENTER_1].animationIndex1 = 0;
    npcs[CARPENTER_1].animationIndex2 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gSeason != WINTER) {

            if (gWeather == SUNNY) {
                
                if (7 < gHour && gHour < 17) {    
                
                    npcs[CARPENTER_1].levelIndex = MOUNTAIN_2;
                    npcs[CARPENTER_1].startingCoordinates.x = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.y = 0;
                    npcs[CARPENTER_1].direction = SOUTHWEST;
                    npcs[CARPENTER_1].unk_1E = 2;
                    npcs[CARPENTER_1].startingCoordinates.z = 32.0f;
                    npcs[CARPENTER_1].flags |= 1;
                    
                }
    
                 if (16 < gHour && gHour < 22) {
    
                    npcs[CARPENTER_1].levelIndex = TOP_OF_MOUNTAIN_1;
                    npcs[CARPENTER_1].startingCoordinates.x = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.y = 0.0f;
                    npcs[CARPENTER_1].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_1].direction = SOUTHWEST;
                    npcs[CARPENTER_1].unk_1E = 0;
                    npcs[CARPENTER_1].flags |= 1;

                }
                
            } else {
                
                if (7 < gHour && gHour < 18) {

                    npcs[CARPENTER_1].levelIndex = CARPENTER_HUT;
            
                    npcs[CARPENTER_1].startingCoordinates.y = 0;
                    npcs[CARPENTER_1].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_1].direction = SOUTHEAST;
                    npcs[CARPENTER_1].unk_1E = 0;
                    npcs[CARPENTER_1].startingCoordinates.x = -80.0f;
                    npcs[CARPENTER_1].flags |= 1;
                
                }
                
            }

        } else if (8 < gHour && gHour < 17) {

            npcs[CARPENTER_1].levelIndex = CAVE;
    
            npcs[CARPENTER_1].startingCoordinates.y = 0;
            npcs[CARPENTER_1].direction = SOUTHEAST;
            npcs[CARPENTER_1].unk_1E = 0;
            npcs[CARPENTER_1].startingCoordinates.x = -304.0f;
            npcs[CARPENTER_1].startingCoordinates.z = -80.0f;
            npcs[CARPENTER_1].flags |= 1;
        
        }
        
    } 

    npcs[CARPENTER_1].movingFlag = npcs[CARPENTER_1].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080364);

void func_80080364(void) {

    npcs[CARPENTER_2].unk_1A = 0x40;
    npcs[CARPENTER_2].unk_1B = 0x40;
    npcs[CARPENTER_2].animationIndex1 = 0;
    npcs[CARPENTER_2].animationIndex2 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gSeason != WINTER) {

            if (gWeather == SUNNY) {

                if (npcs[CARPENTER_2].location < 2) {
                    
                    if (7 < gHour && gHour < 17) {    
                    
                        npcs[CARPENTER_2].levelIndex = MOUNTAIN_2;
                        npcs[CARPENTER_2].startingCoordinates.y = 0;
                        npcs[CARPENTER_2].direction = SOUTHWEST;
                        npcs[CARPENTER_2].unk_1E = 2;
                        npcs[CARPENTER_2].startingCoordinates.x = -224.0f;
                        npcs[CARPENTER_2].startingCoordinates.z = 64.0f;
                        npcs[CARPENTER_2].flags |= 1;
                        
                    }
        
                } else if (7 < gHour && gHour < 18) {
    
                    npcs[CARPENTER_2].levelIndex = 0x21;
                    npcs[CARPENTER_2].startingCoordinates.y = 0.0f;
                    npcs[CARPENTER_2].direction = NORTHEAST;
                    npcs[CARPENTER_2].unk_1E = 0;
                    npcs[CARPENTER_2].startingCoordinates.x = 64.0f;
                    npcs[CARPENTER_2].startingCoordinates.z = -112.0f;
                    npcs[CARPENTER_2].flags |= 1;

                }
                
            } else {
                
                if (7 < gHour && gHour < 18) {

                    npcs[CARPENTER_2].levelIndex = CARPENTER_HUT;
                    npcs[CARPENTER_2].startingCoordinates.y = 0;
                    npcs[CARPENTER_2].startingCoordinates.z = 0.0f;
                    npcs[CARPENTER_2].direction = NORTHWEST;
                    npcs[CARPENTER_2].unk_1E = 0;
                    npcs[CARPENTER_2].startingCoordinates.x = 16.0f;
                    npcs[CARPENTER_2].flags |= 1;
                
                }
                
            }

        } else if (npcs[CARPENTER_2].location == 0 && 8 < gHour && gHour < 17) {

            npcs[CARPENTER_2].levelIndex = MINE;
            npcs[CARPENTER_2].startingCoordinates.x = 0.0f;
            npcs[CARPENTER_2].startingCoordinates.y = 0;
            npcs[CARPENTER_2].startingCoordinates.z = 0.0f;
            npcs[CARPENTER_2].direction = SOUTHWEST;
            npcs[CARPENTER_2].unk_1E = 1;
            npcs[CARPENTER_2].flags |= 1;
        
        }
        
    } 

    npcs[CARPENTER_2].movingFlag = npcs[CARPENTER_2].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080630);

void func_80080630(void) {

    npcs[MASTER_CARPENTER].unk_1A = 0x40;
    npcs[MASTER_CARPENTER].unk_1B = 0x40;
    npcs[MASTER_CARPENTER].animationIndex1 = 0;
    npcs[MASTER_CARPENTER].animationIndex2 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gWeather == SUNNY && npcs[MASTER_CARPENTER].location >= 3) {

            if (7 < gHour && gHour < 18) {    
            
                npcs[MASTER_CARPENTER].levelIndex = MOUNTAIN_2;
        
                npcs[MASTER_CARPENTER].startingCoordinates.y = 0;
                npcs[MASTER_CARPENTER].direction = SOUTHWEST;
                npcs[MASTER_CARPENTER].unk_1E = 0;
                
                npcs[MASTER_CARPENTER].startingCoordinates.x = -112.0f;
                npcs[MASTER_CARPENTER].startingCoordinates.z = 112.0f;
        
                npcs[MASTER_CARPENTER].flags |= 1;
                
            }

        } else if (7 < gHour && gHour < 18) {

            npcs[MASTER_CARPENTER].levelIndex = CARPENTER_HUT;
    
            npcs[MASTER_CARPENTER].startingCoordinates.y = 0;
            npcs[MASTER_CARPENTER].direction = SOUTHWEST;
            npcs[MASTER_CARPENTER].unk_1E = 2;
            
            npcs[MASTER_CARPENTER].startingCoordinates.x = -32.0f;
            npcs[MASTER_CARPENTER].startingCoordinates.z = -48.0f;
    
            npcs[MASTER_CARPENTER].flags |= 1;
        
        }
        
    } 

    npcs[MASTER_CARPENTER].movingFlag = npcs[MASTER_CARPENTER].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800807F8);

void func_800807F8(void) {

    npcs[HARVEST_SPRITE_1].unk_1A = 0x40;
    npcs[HARVEST_SPRITE_1].unk_1B = 0x40;
    npcs[HARVEST_SPRITE_1].animationIndex1 = 0;
    npcs[HARVEST_SPRITE_1].animationIndex2 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_1].levelIndex = CAVE;
        npcs[HARVEST_SPRITE_1].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_1].direction = 0;
        npcs[HARVEST_SPRITE_1].unk_1E = 1;
    
        npcs[HARVEST_SPRITE_1].startingCoordinates.x = -32.0f;
        npcs[HARVEST_SPRITE_1].startingCoordinates.z = -192.0f;
    
        npcs[HARVEST_SPRITE_1].flags |= 1;

    }

    npcs[HARVEST_SPRITE_1].movingFlag =  npcs[HARVEST_SPRITE_1].unk_1E;
    
}


//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800808B0);

void func_800808B0(void) {

    npcs[HARVEST_SPRITE_2].unk_1A = 0x40;
    npcs[HARVEST_SPRITE_2].unk_1B = 0x40;
    npcs[HARVEST_SPRITE_2].animationIndex1 = 0;
    npcs[HARVEST_SPRITE_2].animationIndex2 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_2].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_2].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_2].direction = SOUTHWEST;
        npcs[HARVEST_SPRITE_2].unk_1E = 0;
    
        npcs[HARVEST_SPRITE_2].startingCoordinates.x = -128.0f;
        npcs[HARVEST_SPRITE_2].startingCoordinates.z = -128.0f;
    
        npcs[HARVEST_SPRITE_2].flags |= 1;

    }

    npcs[HARVEST_SPRITE_2].movingFlag =  npcs[HARVEST_SPRITE_2].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080958);

void func_80080958(void) {

    npcs[HARVEST_SPRITE_3].unk_1A = 0x40;
    npcs[HARVEST_SPRITE_3].unk_1B = 0x40;
    npcs[HARVEST_SPRITE_3].animationIndex1 = 0;
    npcs[HARVEST_SPRITE_3].animationIndex2 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_3].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_3].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_3].direction = NORTHWEST;
        npcs[HARVEST_SPRITE_3].unk_1E = 0;
    
        npcs[HARVEST_SPRITE_3].startingCoordinates.x = 64.0f;
        npcs[HARVEST_SPRITE_3].startingCoordinates.z = -64.0f;
    
        npcs[HARVEST_SPRITE_3].flags |= 1;

    }

    npcs[HARVEST_SPRITE_3].movingFlag =  npcs[HARVEST_SPRITE_3].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080A10);

void func_80080A10(void) {

    npcs[SYDNEY].unk_1A = 0x40;
    npcs[SYDNEY].unk_1B = 0x40;
    npcs[SYDNEY].animationIndex1 = 0;
    npcs[SYDNEY].animationIndex2 = 8;

    if (gSeason != WINTER && gWeather == SUNNY && npcs[SYDNEY].location >= 2 && (7 < gHour && gHour < 17)) {

        npcs[SYDNEY].levelIndex = MOON_MOUNTAIN;
        npcs[SYDNEY].startingCoordinates.y = 0.0f;
        npcs[SYDNEY].direction = NORTHWEST;
        npcs[SYDNEY].unk_1E = 1;

        npcs[SYDNEY].startingCoordinates.x = 64.0F;
        npcs[SYDNEY].startingCoordinates.z = -32.0f;

        npcs[SYDNEY].flags |= 1;
        
    } else if (7 < gHour && gHour < 17) {
        
        npcs[SYDNEY].levelIndex = DUMPLING_HOUSE;
        npcs[SYDNEY].startingCoordinates.y = 0.0f;
        npcs[SYDNEY].direction = SOUTHEAST;
        npcs[SYDNEY].unk_1E = 0;

        npcs[SYDNEY].startingCoordinates.x = -128.0f;
        npcs[SYDNEY].startingCoordinates.z = -128.0f;

        npcs[SYDNEY].flags |= 1;

    }

    npcs[SYDNEY].movingFlag =  npcs[SYDNEY].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080B6C);

void func_80080B6C(void) {

    npcs[BARLEY].unk_1A = 0x40;
    npcs[BARLEY].unk_1B = 0x40;
    npcs[BARLEY].animationIndex1 = 0;
    npcs[BARLEY].animationIndex2 = 8;

    if (npcs[BARLEY].location < 2 && (7 < gHour && gHour < 17)) {

        npcs[BARLEY].levelIndex = DUMPLING_HOUSE;
        npcs[BARLEY].startingCoordinates.y = 0.0f;
        npcs[BARLEY].direction = 0;
        npcs[BARLEY].unk_1E = 1;

        npcs[BARLEY].startingCoordinates.x = -32.0f;
        npcs[BARLEY].startingCoordinates.z = -128.0f;

        npcs[BARLEY].flags |= 1;
        
    } else if (7 < gHour && gHour < 17) {
        
        npcs[BARLEY].levelIndex = DUMPLING_HOUSE;
        npcs[BARLEY].startingCoordinates.x = 0.0f;
        npcs[BARLEY].startingCoordinates.y = 0.0f;
        npcs[BARLEY].direction = SOUTHWEST;
        npcs[BARLEY].unk_1E = 0;

        npcs[BARLEY].startingCoordinates.z = -192.0f;

        npcs[BARLEY].flags |= 1;

    }

    npcs[BARLEY].movingFlag =  npcs[BARLEY].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080C9C);

void func_80080C9C(void) {
    
    npcs[GOURMET_JUDGE].unk_1A = 0x40;
    npcs[GOURMET_JUDGE].unk_1B = 0x40;
    npcs[GOURMET_JUDGE].animationIndex1 = 0;
    npcs[GOURMET_JUDGE].animationIndex2 = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(0x94) && (17 < gHour && gHour < 24)) {

        npcs[GOURMET_JUDGE].levelIndex = TAVERN;
        npcs[GOURMET_JUDGE].startingCoordinates.y = 0;
        npcs[GOURMET_JUDGE].direction = NORTHWEST;
        npcs[GOURMET_JUDGE].unk_1E = 0;

        npcs[GOURMET_JUDGE].startingCoordinates.x = 128.0f;
        npcs[GOURMET_JUDGE].startingCoordinates.z = -112.0f;

        npcs[GOURMET_JUDGE].flags |= 1;
        
    }

    npcs[GOURMET_JUDGE].movingFlag =  npcs[GOURMET_JUDGE].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080DAC);

void func_80080DAC(void) {
    
    npcs[PHOTOGRAPHER].unk_1A = 0x40;
    npcs[PHOTOGRAPHER].unk_1B = 0x40;
    npcs[PHOTOGRAPHER].animationIndex1 = 0;
    npcs[PHOTOGRAPHER].animationIndex2 = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(0x94) && (9 < gHour && gHour < 16)) {

        npcs[PHOTOGRAPHER].levelIndex = BARN;
        npcs[PHOTOGRAPHER].startingCoordinates.y = 0;
        npcs[PHOTOGRAPHER].direction = SOUTH;
        npcs[PHOTOGRAPHER].unk_1E = 0;

        npcs[PHOTOGRAPHER].startingCoordinates.x = -192.0f;
        npcs[PHOTOGRAPHER].startingCoordinates.z = -256.0f;

        npcs[PHOTOGRAPHER].flags |= 1;
        
    }

    npcs[PHOTOGRAPHER].movingFlag =  npcs[PHOTOGRAPHER].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080EBC);

void func_80080EBC(void) {

    npcs[40].unk_1A = 0x40;
    npcs[40].unk_1B = 0x40;
    npcs[40].animationIndex1 = 0;
    npcs[40].animationIndex2 = 8;

    if (checkLifeEventBit(0x20) && npcs[40].location == 0 && (8 < gHour && gHour < 17)) {

        npcs[40].levelIndex = ROAD;
        npcs[40].startingCoordinates.y = 0;
        npcs[40].direction = NORTHEAST;
        npcs[40].unk_1E = 1;

        npcs[40].startingCoordinates.x = -32.0f;
        npcs[40].startingCoordinates.z = 64.0f;

        npcs[40].flags |= 1;

    }

    if (checkLifeEventBit(0x19) && (8 < gHour && gHour < 17)) {
        
        npcs[40].levelIndex = VILLAGE_1;
        npcs[40].startingCoordinates.y = 0;
        npcs[40].direction = SOUTHWEST;
        npcs[40].unk_1E = 1;

        npcs[40].startingCoordinates.x = 320.0f;
        npcs[40].startingCoordinates.z = 288.0f;

        npcs[40].flags |= 1;
    
    }

     npcs[40].movingFlag =  npcs[40].unk_1E; 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081030);

void func_80081030(void) {

    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[MARIA_HARRIS_BABY].unk_1A = 0x40;
    npcs[MARIA_HARRIS_BABY].unk_1B = 0x40;
    npcs[MARIA_HARRIS_BABY].animationIndex1 = 0;
    npcs[MARIA_HARRIS_BABY].animationIndex2 = 0;

    temp = func_80076DCC(MARIA);

    if (temp == 1) {

        if (npcs[MARIA].flags & 1) {
            
            npcs[MARIA_HARRIS_BABY].startingCoordinates.x = 0.0f;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.y = 0.0f;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.z = 0.0f;

            npcs[MARIA_HARRIS_BABY].unk_1E = 0;

            npcs[MARIA_HARRIS_BABY].animationIndex1 = 8;
            npcs[MARIA_HARRIS_BABY].animationIndex2 = 8;

            npcs[MARIA_HARRIS_BABY].levelIndex = npcs[MARIA].levelIndex;
            npcs[MARIA_HARRIS_BABY].flags |= (1 | 8);

            npcs[MARIA_HARRIS_BABY].direction = npcs[MARIA].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[MARIA].flags & 1) {

            npcs[MARIA_HARRIS_BABY].levelIndex = npcs[MARIA].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[MARIA].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[MARIA_HARRIS_BABY].startingCoordinates, rotationAngles);

            npcs[MARIA_HARRIS_BABY].unk_1E = 1;

            npcs[MARIA_HARRIS_BABY].animationIndex1 = 0;
            npcs[MARIA_HARRIS_BABY].animationIndex2 = 0;

            npcs[MARIA_HARRIS_BABY].flags |= 1;

            npcs[MARIA_HARRIS_BABY].startingCoordinates.x = npcs[MARIA].startingCoordinates.x + vec1.x;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.y = npcs[MARIA].startingCoordinates.y;
            npcs[MARIA_HARRIS_BABY].startingCoordinates.z = npcs[MARIA].startingCoordinates.z + vec1.z;
            
            npcs[MARIA_HARRIS_BABY].direction = npcs[MARIA].direction;
                            
        }
        
    }

    npcs[MARIA_HARRIS_BABY].movingFlag = npcs[MARIA_HARRIS_BABY].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081228);

void func_80081228(void) {
    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[POPURI_GRAY_BABY].unk_1A = 0x40;
    npcs[POPURI_GRAY_BABY].unk_1B = 0x40;
    npcs[POPURI_GRAY_BABY].animationIndex1 = 0;
    npcs[POPURI_GRAY_BABY].animationIndex2 = 0;

    temp = func_80076DCC(POPURI);

    if (temp == 1) {

        if (npcs[POPURI].flags & 1) {
            
            npcs[POPURI_GRAY_BABY].startingCoordinates.x = 0.0f;
            npcs[POPURI_GRAY_BABY].startingCoordinates.y = 0.0f;
            npcs[POPURI_GRAY_BABY].startingCoordinates.z = 0.0f;

            npcs[POPURI_GRAY_BABY].unk_1E = 0;

            npcs[POPURI_GRAY_BABY].animationIndex1 = 8;
            npcs[POPURI_GRAY_BABY].animationIndex2 = 8;

            npcs[POPURI_GRAY_BABY].levelIndex = npcs[POPURI].levelIndex;
            npcs[POPURI_GRAY_BABY].flags |= (1 | 8);

            npcs[POPURI_GRAY_BABY].direction = npcs[POPURI].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[POPURI].flags & 1) {

            npcs[POPURI_GRAY_BABY].levelIndex = npcs[POPURI].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[POPURI].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[POPURI_GRAY_BABY].startingCoordinates, rotationAngles);

            npcs[POPURI_GRAY_BABY].unk_1E = 1;

            npcs[POPURI_GRAY_BABY].animationIndex1 = 0;
            npcs[POPURI_GRAY_BABY].animationIndex2 = 0;

            npcs[POPURI_GRAY_BABY].flags |= 1;

            npcs[POPURI_GRAY_BABY].startingCoordinates.x = npcs[POPURI].startingCoordinates.x + vec1.x;
            npcs[POPURI_GRAY_BABY].startingCoordinates.y = npcs[POPURI].startingCoordinates.y;
            npcs[POPURI_GRAY_BABY].startingCoordinates.z = npcs[POPURI].startingCoordinates.z + vec1.z;
            
            npcs[POPURI_GRAY_BABY].direction = npcs[POPURI].direction;
                            
        }
        
    }

    npcs[POPURI_GRAY_BABY].movingFlag = npcs[POPURI_GRAY_BABY].unk_1E;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081420);

void func_80081420(void) {
    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[ELLI_JEFF_BABY].unk_1A = 0x40;
    npcs[ELLI_JEFF_BABY].unk_1B = 0x40;
    npcs[ELLI_JEFF_BABY].animationIndex1 = 0;
    npcs[ELLI_JEFF_BABY].animationIndex2 = 0;

    temp = func_80076DCC(ELLI);

    if (temp == 1) {

        if (npcs[ELLI].flags & 1) {
            
            npcs[ELLI_JEFF_BABY].startingCoordinates.x = 0.0f;
            npcs[ELLI_JEFF_BABY].startingCoordinates.y = 0.0f;
            npcs[ELLI_JEFF_BABY].startingCoordinates.z = 0.0f;

            npcs[ELLI_JEFF_BABY].unk_1E = 0;

            npcs[ELLI_JEFF_BABY].animationIndex1 = 8;
            npcs[ELLI_JEFF_BABY].animationIndex2 = 8;

            npcs[ELLI_JEFF_BABY].levelIndex = npcs[ELLI].levelIndex;
            npcs[ELLI_JEFF_BABY].flags |= (1 | 8);

            npcs[ELLI_JEFF_BABY].direction = npcs[ELLI].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[ELLI].flags & 1) {

            npcs[ELLI_JEFF_BABY].levelIndex = npcs[ELLI].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[ELLI].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[ELLI_JEFF_BABY].startingCoordinates, rotationAngles);

            npcs[ELLI_JEFF_BABY].unk_1E = 1;

            npcs[ELLI_JEFF_BABY].animationIndex1 = 0;
            npcs[ELLI_JEFF_BABY].animationIndex2 = 0;

            npcs[ELLI_JEFF_BABY].flags |= 1;

            npcs[ELLI_JEFF_BABY].startingCoordinates.x = npcs[ELLI].startingCoordinates.x + vec1.x;
            npcs[ELLI_JEFF_BABY].startingCoordinates.y = npcs[ELLI].startingCoordinates.y;
            npcs[ELLI_JEFF_BABY].startingCoordinates.z = npcs[ELLI].startingCoordinates.z + vec1.z;
            
            npcs[ELLI_JEFF_BABY].direction = npcs[ELLI].direction;
                            
        }
        
    }

    npcs[ELLI_JEFF_BABY].movingFlag = npcs[ELLI_JEFF_BABY].unk_1E;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081618);

void func_80081618(void) {
    
    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[ANN_CLIFF_BABY].unk_1A = 0x40;
    npcs[ANN_CLIFF_BABY].unk_1B = 0x40;
    npcs[ANN_CLIFF_BABY].animationIndex1 = 0;
    npcs[ANN_CLIFF_BABY].animationIndex2 = 0;

    temp = func_80076DCC(ANN);

    if (temp == 1) {

        if (npcs[ANN].flags & 1) {
            
            npcs[ANN_CLIFF_BABY].startingCoordinates.x = 0.0f;
            npcs[ANN_CLIFF_BABY].startingCoordinates.y = 0.0f;
            npcs[ANN_CLIFF_BABY].startingCoordinates.z = 0.0f;

            npcs[ANN_CLIFF_BABY].unk_1E = 0;

            npcs[ANN_CLIFF_BABY].animationIndex1 = 8;
            npcs[ANN_CLIFF_BABY].animationIndex2 = 8;

            npcs[ANN_CLIFF_BABY].levelIndex = npcs[ANN].levelIndex;
            npcs[ANN_CLIFF_BABY].flags |= (1 | 8);

            npcs[ANN_CLIFF_BABY].direction = npcs[ANN].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[ANN].flags & 1) {

            npcs[ANN_CLIFF_BABY].levelIndex = npcs[ANN].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[ANN].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[ANN_CLIFF_BABY].startingCoordinates, rotationAngles);

            npcs[ANN_CLIFF_BABY].unk_1E = 1;

            npcs[ANN_CLIFF_BABY].animationIndex1 = 0;
            npcs[ANN_CLIFF_BABY].animationIndex2 = 0;

            npcs[ANN_CLIFF_BABY].flags |= 1;

            npcs[ANN_CLIFF_BABY].startingCoordinates.x = npcs[ANN].startingCoordinates.x + vec1.x;
            npcs[ANN_CLIFF_BABY].startingCoordinates.y = npcs[ANN].startingCoordinates.y;
            npcs[ANN_CLIFF_BABY].startingCoordinates.z = npcs[ANN].startingCoordinates.z + vec1.z;
            
            npcs[ANN_CLIFF_BABY].direction = npcs[ANN].direction;
                            
        }
        
    }

    npcs[ANN_CLIFF_BABY].movingFlag = npcs[ANN_CLIFF_BABY].unk_1E;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081810);

void func_80081810(void) {

    Vec3f vec1;
    Vec3f rotationAngles;
    
    u8 temp;
    
    npcs[KAREN_KAI_BABY].unk_1A = 0x40;
    npcs[KAREN_KAI_BABY].unk_1B = 0x40;
    npcs[KAREN_KAI_BABY].animationIndex1 = 0;
    npcs[KAREN_KAI_BABY].animationIndex2 = 0;

    temp = func_80076DCC(KAREN);

    if (temp == 1) {

        if (npcs[KAREN].flags & 1) {
            
            npcs[KAREN_KAI_BABY].startingCoordinates.x = 0.0f;
            npcs[KAREN_KAI_BABY].startingCoordinates.y = 0.0f;
            npcs[KAREN_KAI_BABY].startingCoordinates.z = 0.0f;

            npcs[KAREN_KAI_BABY].unk_1E = 0;

            npcs[KAREN_KAI_BABY].animationIndex1 = 8;
            npcs[KAREN_KAI_BABY].animationIndex2 = 8;

            npcs[KAREN_KAI_BABY].levelIndex = npcs[KAREN].levelIndex;
            npcs[KAREN_KAI_BABY].flags |= (1 | 8);

            npcs[KAREN_KAI_BABY].direction = npcs[KAREN].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[KAREN].flags & 1) {

            npcs[KAREN_KAI_BABY].levelIndex = npcs[KAREN].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[KAREN].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[KAREN_KAI_BABY].startingCoordinates, rotationAngles);

            npcs[KAREN_KAI_BABY].unk_1E = 1;

            npcs[KAREN_KAI_BABY].animationIndex1 = 0;
            npcs[KAREN_KAI_BABY].animationIndex2 = 0;

            npcs[KAREN_KAI_BABY].flags |= 1;

            npcs[KAREN_KAI_BABY].startingCoordinates.x = npcs[KAREN].startingCoordinates.x + vec1.x;
            npcs[KAREN_KAI_BABY].startingCoordinates.y = npcs[KAREN].startingCoordinates.y;
            npcs[KAREN_KAI_BABY].startingCoordinates.z = npcs[KAREN].startingCoordinates.z + vec1.z;
            
            npcs[KAREN_KAI_BABY].direction = npcs[KAREN].direction;
                            
        }
        
    }

    npcs[KAREN_KAI_BABY].movingFlag = npcs[KAREN_KAI_BABY].unk_1E;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081A08);

void func_80081A08(void) {
    
    npcs[0x2E].unk_1A = 0x40;
    npcs[0x2E].unk_1B = 0x40;
    npcs[0x2E].animationIndex1 = 0;
    npcs[0x2E].animationIndex2 = 8;

    if (gYear == 2 && gSeason == SPRING && 1 < gDayOfMonth && gDayOfMonth < 21 && (5 < gHour && gHour < 18)) {

        npcs[0x2E].levelIndex = MOUNTAIN_1;
        npcs[0x2E].startingCoordinates.y = 0;
        npcs[0x2E].direction = SOUTH;
        npcs[0x2E].unk_1E = 1;

        npcs[0x2E].startingCoordinates.x = -32.0f;
        npcs[0x2E].startingCoordinates.z = -64.0f;

        npcs[0x2E].flags |= 1;
        
    }

    npcs[0x2E].movingFlag =  npcs[0x2E].unk_1E;
    
}



/* animations */

static inline void updateAnimation(u8 index) {
    
    switch (npcs[index].movingFlag) {

        case 0:
            func_80075E28(index);
            break;

        case 1:
            func_80075EBC(index);
            break;
            
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", setNpcAnimations);

void setNpcAnimations(void) {
    func_80081C90();
    func_80081E5C();
    func_80082164();
    func_80082330();
    func_800824FC();
    func_8008277C();
    func_80082A0C();
    func_80082B04();
    func_80082C5C();
    func_80082DB4();
    func_80082EAC();
    func_80082FA4();
    func_8008309C();
    func_80083194();
    func_8008328C();
    func_80083384();
    func_80083458();
    func_80083550();
    func_8008374C();
    func_80083948();
    func_80083A40();
    func_80083C3C();
    func_80083D34();
    func_80083E2C();
    func_80083F24();
    func_8008401C();
    func_80084114();
    func_8008420C();
    func_80084304();
    func_800843FC();
    func_800844F4();
    func_80084770();
    func_800849EC();
    func_80084B44();
    func_80084C3C();
    func_80084D34();
    func_80084E2C();
    func_80084F24();
    func_8008501C();
    func_80085114();
    func_8008520C();
    func_80085304();
    func_800853FC();
    func_800854F4();
    func_800855EC();
    func_800856E4();
    func_800857DC();
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081C90);

void func_80081C90(void) {

    u16 temp;

    if (npcs[MARIA].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[MARIA].entityIndex) && npcs[MARIA].unk_22 == 0) {

            switch (npcs[MARIA].movingFlag) {

                case 0:
                    func_80075E28(MARIA);
                    break;

                case 1:
                    func_80075EBC(MARIA);
                    break;

                case 2:
                    func_80076944(MARIA);
                    break;

                case 4:

                    setEntityAnimation(npcs[MARIA].entityIndex, 0x18);

                    entities[npcs[MARIA].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[MARIA].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[MARIA].entityIndex].coordinates.z = -160.0f;
                
            }
            
        }
        
        if (npcs[MARIA].unk_22) {
            npcs[MARIA].unk_22--;
        }
    
        func_80075A78(MARIA);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081E5C);

void func_80081E5C(void) {

    u16 temp;

    if (npcs[POPURI].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[POPURI].entityIndex) && npcs[POPURI].unk_22 == 0) {

            switch (npcs[POPURI].movingFlag) {

                case 0:
                    func_80075E28(POPURI);
                    break;

                case 1:
                    func_80075EBC(POPURI);
                    break;

                case 2:
                    func_80076944(POPURI);
                    break;

                case 3:

                    switch (npcs[POPURI].unk_21) {

                        case 0:
                            npcs[POPURI].speed = 0;
                            npcs[POPURI].unk_22 = 0;
    
                            setEntityAnimationWithDirectionChange(npcs[POPURI].entityIndex, 0);
    
                            temp = getRandomNumberInRange(0, 60);
    
                            if (temp < 8) {
                                npcs[POPURI].direction = temp;
                                npcs[POPURI].unk_21 = 1;
                            }
    
                            if (7 < temp && temp < 21) {
                                npcs[POPURI].unk_21 = 2;
                            }
    
                            npcs[POPURI].flags |= 2;
                            break;

                        case 1:
                            npcs[POPURI].speed = 1;
                            npcs[POPURI].unk_22 = 0;
    
                            setEntityAnimationWithDirectionChange(npcs[POPURI].entityIndex, 8);
    
                            if (getRandomNumberInRange(0, 19) <= 7) {
                                npcs[POPURI].unk_21 = 1;
                            } else {
                                npcs[POPURI].unk_21 = 0;
                            }
    
                            npcs[POPURI].flags |= 2;
                            break;

                        case 2:

                            npcs[POPURI].speed = 0;
                            npcs[POPURI].unk_22 = 60;
    
                            setEntityAnimationWithDirectionChange(npcs[POPURI].entityIndex, 0x72);
                            npcs[POPURI].unk_21 = 1;
                            
                            npcs[POPURI].flags |= 2;
                            break;
                        
                    }
                        
                    break;
                
                case 4:

                    setEntityAnimation(npcs[POPURI].entityIndex, 0x18);

                    entities[npcs[POPURI].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[POPURI].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[POPURI].entityIndex].coordinates.z = -160.0f;
                
            }
            
        }
        
        if (npcs[POPURI].unk_22) {
            npcs[POPURI].unk_22--;
        }
    
        func_80075A78(POPURI);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082164);

void func_80082164(void) {
    
    u16 temp;

    if (npcs[ELLI].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[ELLI].entityIndex) && npcs[ELLI].unk_22 == 0) {

            switch (npcs[ELLI].movingFlag) {

                case 0:
                    func_80075E28(ELLI);
                    break;

                case 1:
                    func_80075EBC(ELLI);
                    break;

                case 2:
                    func_80076944(ELLI);
                    break;

                case 4:

                    setEntityAnimation(npcs[ELLI].entityIndex, 0x18);

                    entities[npcs[ELLI].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[ELLI].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[ELLI].entityIndex].coordinates.z = -160.0f;
                
            }
            
        }
        
        if (npcs[ELLI].unk_22) {
            npcs[ELLI].unk_22--;
        }
    
        func_80075A78(ELLI);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082330);

void func_80082330(void) {

    u16 temp;

    if (npcs[ANN].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[ANN].entityIndex) && npcs[ANN].unk_22 == 0) {

            switch (npcs[ANN].movingFlag) {

                case 0:
                    func_80075E28(ANN);
                    break;

                case 1:
                    func_80075EBC(ANN);
                    break;

                case 2:
                    func_80076944(ANN);
                    break;

                case 4:

                    setEntityAnimation(npcs[ANN].entityIndex, 0x18);

                    entities[npcs[ANN].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[ANN].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[ANN].entityIndex].coordinates.z = -160.0f;
                
            }
            
        }
        
        if (npcs[ANN].unk_22) {
            npcs[ANN].unk_22--;
        }
    
        func_80075A78(ANN);
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800824FC);

void func_800824FC(void) {

    u16 temp;

    if (npcs[KAREN].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[KAREN].entityIndex) && npcs[KAREN].unk_22 == 0) {

            switch (npcs[KAREN].movingFlag) {

                case 0:
                    func_80075E28(KAREN);
                    break;

                case 1:
                    func_80075EBC(KAREN);
                    break;

                case 2:
                    func_80076944(KAREN);
                    break;

                case 3:

                    if (npcs[KAREN].unk_21 == 0) {

                        npcs[KAREN].speed = 0;
                        npcs[KAREN].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[KAREN].entityIndex, 0);

                        temp = getRandomNumberInRange(0, 60);

                        if (temp < 4) {
                            npcs[KAREN].direction = temp * 2;
                            npcs[KAREN].unk_21 = 1;
                        }

                        npcs[KAREN].flags |= 2;
                        
                    } else {

                        npcs[KAREN].speed = 1;
                        npcs[KAREN].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[KAREN].entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) <=7) {
                            npcs[KAREN].unk_21 = 1;
                        } else {
                            npcs[KAREN].unk_21 = 0;
                        }

                        npcs[KAREN].flags |= 2;
                        
                    }
                    
                    break;
                
                case 4:

                    setEntityAnimation(npcs[KAREN].entityIndex, 0x18);

                    entities[npcs[KAREN].entityIndex].coordinates.x = -192.0f;
                    entities[npcs[KAREN].entityIndex].coordinates.y = 0.0f;
                    entities[npcs[KAREN].entityIndex].coordinates.z = -160.0f;
                
            }
            
        }
        
        if (npcs[KAREN].unk_22) {
            npcs[KAREN].unk_22--;
        }
    
        func_80075A78(KAREN);
    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008277C);

void func_8008277C(void) {

    if (npcs[BABY].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[BABY].entityIndex)) {
            
            if (npcs[BABY].unk_22 == 0) {
    
                switch (npcs[BABY].movingFlag) {
    
                    case 0:
                        func_80076080(BABY, 0);
                        break;
                    
                    case 1:
    
                        npcs[BABY].speed = 0;
                        npcs[BABY].unk_21 = 10;
                        npcs[BABY].unk_22 = 0;
    
                        if (npcAffection[BABY] < 50) {
    
                            if (getRandomNumberInRange(0, 100)) {
                                setEntityAnimation(npcs[BABY].entityIndex, 3);                            
                            } else {
                                setEntityAnimation(npcs[BABY].entityIndex, 11);
                            }
                            
                        } else if (npcAffection[BABY] >= 200 && !getRandomNumberInRange(0, 100)) {
                            setEntityAnimation(npcs[BABY].entityIndex, 9);
                        } else {
                            setEntityAnimation(npcs[BABY].entityIndex, 3);
                        }
                        
                        npcs[BABY].flags |= 2;
                        break;
    
                    case 2:
                        npcs[BABY].speed = 0;
                        npcs[BABY].unk_21 = 10;
                        npcs[BABY].unk_22 = 0;
                        setEntityAnimation(npcs[BABY].entityIndex, 0xD);
                        npcs[BABY].flags |= 2;
                        break;
    
                    case 3:
                        switch (getRandomNumberInRange(0, 1)) {            
                            case 0:                         
                                func_80076108(5, 0x39, 0x41);
                                break;
                            case 1:                         
                                func_80076108(5, 0x39, 0x49);
                                break;
                            }
                        break;
                    
                    case 4:
                        func_800762B4(5, 0xE, 0x16, 0x51, 0x59, 0x79, 0x69, 0x71, 0x1E, 0x2E, 0x61, 0x26);
                        break;

                    case 5:
                        npcs[BABY].speed = 0;
                        npcs[BABY].unk_21 = 10;
                        npcs[BABY].unk_22 = 0;
                        setEntityAnimation(npcs[BABY].entityIndex, 0x36);
                        npcs[BABY].flags |= 2;
                        break;
                        
                }
            }
            
        }

        if (npcs[BABY].unk_22) {
            npcs[BABY].unk_22--;
        }

        func_80075A78(BABY);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082A0C);

void func_80082A0C(void) {

    if (npcs[HARRIS].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[HARRIS].entityIndex)) {

            if (npcs[HARRIS].unk_22 == 0) {

                switch (npcs[HARRIS].movingFlag) {

                    case 0:
                        func_80075E28(HARRIS);
                        break;

                    case 1:
                        func_80075EBC(HARRIS);
                        break;
                }
            } 
            
        }

        if (npcs[HARRIS].unk_22) {
            npcs[HARRIS].unk_22--;
        }
    
        func_80075A78(HARRIS);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082B04);

void func_80082B04(void) {

    if (npcs[GRAY].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[GRAY].entityIndex)) {

            if (npcs[GRAY].unk_22 == 0) {

                switch (npcs[GRAY].movingFlag) {

                    case 0:
                        func_80075E28(GRAY);
                        break;

                    case 1:
                        func_80075EBC(GRAY);
                        break;

                    case 2:

                        npcs[GRAY].speed = 0;
                        npcs[GRAY].unk_21 = 10;
                        npcs[GRAY].unk_22 = 0;
                    
                        setEntityAnimation(npcs[GRAY].entityIndex, 0x2A);
                    
                        npcs[GRAY].flags |= 2;
                        break;
                    
                }
            } 
            
        }

        if (npcs[GRAY].unk_22) {
            npcs[GRAY].unk_22--;
        }
    
        func_80075A78(GRAY);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082C5C);

void func_80082C5C(void) {

    if (npcs[JEFF].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[JEFF].entityIndex)) {

            if (npcs[JEFF].unk_22 == 0) {

                switch (npcs[JEFF].movingFlag) {

                    case 0:
                        func_80075E28(JEFF);
                        break;

                    case 1:
                        func_80075EBC(JEFF);
                        break;

                    case 2:

                        npcs[JEFF].speed = 0;
                        npcs[JEFF].unk_21 = 10;
                        npcs[JEFF].unk_22 = 0;
                    
                        setEntityAnimation(npcs[JEFF].entityIndex, 0x33);
                    
                        npcs[JEFF].flags |= 2;
                        break;
                    
                }
            } 
            
        }

        if (npcs[JEFF].unk_22) {
            npcs[JEFF].unk_22--;
        }
    
        func_80075A78(JEFF);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082DB4);

void func_80082DB4(void) {
    
    if (npcs[CLIFF].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[CLIFF].entityIndex)) {

            if (npcs[CLIFF].unk_22 == 0) {

                switch (npcs[CLIFF].movingFlag) {

                    case 0:
                        func_80075E28(CLIFF);
                        break;

                    case 1:
                        func_80075EBC(CLIFF);
                        break;
                }
            } 
            
        }

        if (npcs[CLIFF].unk_22) {
            npcs[CLIFF].unk_22--;
        }
    
        func_80075A78(CLIFF);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082EAC);

void func_80082EAC(void) {
    
    if (npcs[KAI].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[KAI].entityIndex)) {

            if (npcs[KAI].unk_22 == 0) {

                switch (npcs[KAI].movingFlag) {

                    case 0:
                        func_80075E28(KAI);
                        break;

                    case 1:
                        func_80075EBC(KAI);
                        break;
                }
            } 
            
        }

        if (npcs[KAI].unk_22) {
            npcs[KAI].unk_22--;
        }
    
        func_80075A78(KAI);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082FA4);

void func_80082FA4(void) {
    
    if (npcs[MAYOR].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[MAYOR].entityIndex)) {

            if (npcs[MAYOR].unk_22 == 0) {

                switch (npcs[MAYOR].movingFlag) {

                    case 0:
                        func_80075E28(MAYOR);
                        break;

                    case 1:
                        func_80075EBC(MAYOR);
                        break;
                }
            } 
            
        }

        if (npcs[MAYOR].unk_22) {
            npcs[MAYOR].unk_22--;
        }
    
        func_80075A78(MAYOR);
        
    }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008309C);

void func_8008309C(void) {
    
    if (npcs[MAYOR_WIFE].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[MAYOR_WIFE].entityIndex)) {

            if (npcs[MAYOR_WIFE].unk_22 == 0) {

                switch (npcs[MAYOR_WIFE].movingFlag) {

                    case 0:
                        func_80075E28(MAYOR_WIFE);
                        break;

                    case 1:
                        func_80075EBC(MAYOR_WIFE);
                        break;
                }
            } 
            
        }

        if (npcs[MAYOR_WIFE].unk_22) {
            npcs[MAYOR_WIFE].unk_22--;
        }
    
        func_80075A78(MAYOR_WIFE);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083194);

void func_80083194(void) {
    
    if (npcs[LILLIA].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[LILLIA].entityIndex)) {

            if (npcs[LILLIA].unk_22 == 0) {

                switch (npcs[LILLIA].movingFlag) {

                    case 0:
                        func_80075E28(LILLIA);
                        break;

                    case 1:
                        func_80075EBC(LILLIA);
                        break;
                }
            } 
            
        }

        if (npcs[LILLIA].unk_22) {
            npcs[LILLIA].unk_22--;
        }
    
        func_80075A78(LILLIA);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008328C);

void func_8008328C(void) {

    if (npcs[BASIL].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[BASIL].entityIndex)) {

            if (npcs[BASIL].unk_22 == 0) {

                switch (npcs[BASIL].movingFlag) {

                    case 0:
                        func_80075E28(BASIL);
                        break;

                    case 1:
                        func_80075EBC(BASIL);
                        break;
                }
            } 
            
        }

        if (npcs[BASIL].unk_22) {
            npcs[BASIL].unk_22--;
        }
    
        func_80075A78(BASIL);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083384);

void func_80083384(void) {

    if (npcs[ELLEN].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[ELLEN].entityIndex)) {

            if (npcs[ELLEN].unk_22 == 0 && !npcs[ELLEN].movingFlag) {
                npcs[ELLEN].speed = 0;
                npcs[ELLEN].unk_21 = 10;
                npcs[ELLEN].unk_22 = 0;
                setEntityAnimation(npcs[ELLEN].entityIndex, 8);
                npcs[ELLEN].flags |= 2;
            } 
            
        }

        if (npcs[ELLEN].unk_22) {
            npcs[ELLEN].unk_22--;
        }
    
        func_80075A78(ELLEN);
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083458);

void func_80083458(void) {

    if (npcs[DOUG].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[DOUG].entityIndex) && !npcs[DOUG].unk_22) {
            updateAnimation(DOUG);
        }

        if (npcs[DOUG].unk_22) {
            npcs[DOUG].unk_22--; 
        } 
        
        func_80075A78(DOUG);
    }   
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083550);

void func_80083550(void) {

   u16 tempDirection;

    if (npcs[GOTZ].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[GOTZ].entityIndex) && !npcs[GOTZ].unk_22) {

            switch (npcs[GOTZ].movingFlag) {

                case 0:
                    func_80075E28(GOTZ);
                    break;

                case 1:
                    func_80075EBC(GOTZ);
                    break;

                case 3:

                    // FIXME: should be inline function
                    if (npcs[GOTZ].unk_21 == 0 ) {

                        npcs[GOTZ].speed = 0;
                        npcs[GOTZ].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[GOTZ].entityIndex, 0);

                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < NORTHEAST) {
                            npcs[GOTZ].direction = tempDirection * 2;
                            npcs[GOTZ].unk_21 = 1;
                        }

                        npcs[GOTZ].flags |= 2;
                        
                    } else {       

                        npcs[GOTZ].speed = 1;
                        npcs[GOTZ].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[GOTZ].entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcs[GOTZ].unk_21 = 1;
                        } else {
                            npcs[GOTZ].unk_21 = 0;
                        }

                        npcs[GOTZ].flags |= 2;                
                    }

                    break;

                default:
                    break;
            }
        }
    
    if (npcs[GOTZ].unk_22) {
        npcs[GOTZ].unk_22--;
    }
    
    func_80075A78(GOTZ);

    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008374C);

void func_8008374C(void) {

   u16 tempDirection;

    if (npcs[SASHA].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[SASHA].entityIndex) && !npcs[SASHA].unk_22) {

            switch (npcs[SASHA].movingFlag) {

                case 0:
                    func_80075E28(SASHA);
                    break;

                case 1:
                    func_80075EBC(SASHA);
                    break;

                case 3:

                    // FIXME: should be inline function
                    if (npcs[SASHA].unk_21 == 0 ) {

                        npcs[SASHA].speed = 0;
                        npcs[SASHA].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[SASHA].entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < NORTHEAST) {
                            npcs[SASHA].direction = tempDirection * 2;
                            npcs[SASHA].unk_21 = 1;
                        }

                        npcs[SASHA].flags |= 2;
                        
                    } else {       

                        npcs[SASHA].speed = 1;
                        npcs[SASHA].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[SASHA].entityIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcs[SASHA].unk_21 = 1;
                        } else {
                            npcs[SASHA].unk_21 = 0;
                        }

                        npcs[SASHA].flags |= 2;
                        
                    }

                    break;

                default:
                    break;
            }
        }
    
    if (npcs[SASHA].unk_22) {
        npcs[SASHA].unk_22--;
    }
    
    func_80075A78(SASHA);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083948);

void func_80083948(void) {

    if (npcs[POTION_SHOP_DEALER].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[POTION_SHOP_DEALER].entityIndex) && !npcs[POTION_SHOP_DEALER].unk_22) {
            updateAnimation(POTION_SHOP_DEALER);
        }

        if (npcs[POTION_SHOP_DEALER].unk_22) {
            npcs[POTION_SHOP_DEALER].unk_22--; 
        } 
        
        func_80075A78(POTION_SHOP_DEALER);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083A40);

void func_80083A40(void) {

    u16 tempDirection;

    if (npcs[KENT].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[KENT].entityIndex) && !npcs[KENT].unk_22) {

            switch (npcs[KENT].movingFlag) {

                case 0:
                    func_80075E28(KENT);
                    break;
                
                case 1:
                    func_80075EBC(KENT);
                    break;

                case 2:

                    // FIXME: should be inline function
                    if (npcs[KENT].unk_21 == 0) {
                        
                        npcs[KENT].speed = 0;
                        npcs[KENT].unk_22 = 0;
                        setEntityAnimationWithDirectionChange(npcs[KENT].entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < NORTHEAST) {
                            npcs[KENT].direction = tempDirection * 2 ;
                            npcs[KENT].unk_21 = 1;
                        } 

                        npcs[KENT].flags |= 2;  

                    } else {

                        npcs[KENT].speed = 2;
                        npcs[KENT].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[KENT].entityIndex, 0x10);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcs[KENT].unk_21 = 1;
                        } else {
                            npcs[KENT].unk_21 = 0;
                        }

                        npcs[KENT].flags |= 2;
                    }

                    break;

                default:
                    break;
            }
       }    

    if (npcs[KENT].unk_22) {
        npcs[KENT].unk_22--;
    }
    
    func_80075A78(KENT);
    
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083C3C);

void func_80083C3C(void) {

    if (npcs[STU].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[STU].entityIndex) && !npcs[STU].unk_22) {
            updateAnimation(STU);
        }

        if (npcs[STU].unk_22) {
            npcs[STU].unk_22--; 
        } 
        
        func_80075A78(STU);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083D34);

void func_80083D34(void) {

    if (npcs[MIDWIFE].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[MIDWIFE].entityIndex) && !npcs[MIDWIFE].unk_22) {
            updateAnimation(MIDWIFE);
        }

        if (npcs[MIDWIFE].unk_22) {
            npcs[MIDWIFE].unk_22--; 
        } 
        
        func_80075A78(MIDWIFE);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083E2C);

void func_80083E2C(void) {

    if (npcs[MAY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[MAY].entityIndex) && !npcs[MAY].unk_22) {
            updateAnimation(MAY);
        }

        if (npcs[MAY].unk_22) {
            npcs[MAY].unk_22--; 
        } 
        
        func_80075A78(MAY);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083F24);

void func_80083F24(void) {

    if (npcs[RICK].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[RICK].entityIndex) && !npcs[RICK].unk_22) {
            updateAnimation(RICK);
        }

        if (npcs[RICK].unk_22) {
            npcs[RICK].unk_22--; 
        } 
        
        func_80075A78(RICK);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008401C);

void func_8008401C(void) {

    if (npcs[PASTOR].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[PASTOR].entityIndex) && !npcs[PASTOR].unk_22) {
            updateAnimation(PASTOR);
        }

        if (npcs[PASTOR].unk_22) {
            npcs[PASTOR].unk_22--; 
        } 
        
        func_80075A78(PASTOR);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084114);

void func_80084114(void) {

    if (npcs[SHIPPER].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[SHIPPER].entityIndex) && !npcs[SHIPPER].unk_22) {
            updateAnimation(SHIPPER);
        }

        if (npcs[SHIPPER].unk_22) {
            npcs[SHIPPER].unk_22--; 
        } 
        
        func_80075A78(SHIPPER);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008420C);

void func_8008420C(void) {

    if (npcs[SAIBARA].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[SAIBARA].entityIndex) && !npcs[SAIBARA].unk_22) {
            updateAnimation(SAIBARA);
        }

        if (npcs[SAIBARA].unk_22) {
            npcs[SAIBARA].unk_22--; 
        } 
        
        func_80075A78(SAIBARA);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084304);
void func_80084304(void) {

    if (npcs[DUKE].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[DUKE].entityIndex) && !npcs[DUKE].unk_22) {
            updateAnimation(DUKE);
        }

        if (npcs[DUKE].unk_22) {
            npcs[DUKE].unk_22--; 
        } 
        
        func_80075A78(DUKE);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800843FC);

void func_800843FC(void) {

    if (npcs[GREG].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[GREG].entityIndex) && !npcs[GREG].unk_22) {
            updateAnimation(GREG);
        }

        if (npcs[GREG].unk_22) {
            npcs[GREG].unk_22--; 
        } 
        
        func_80075A78(GREG);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800844F4);

void func_800844F4(void) {

    u16 temp;

    if (npcs[CARPENTER_1].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[CARPENTER_1].entityIndex) && npcs[CARPENTER_1].unk_22 == 0) {

            switch (npcs[CARPENTER_1].movingFlag) {

                case 0:
                    func_80075E28(CARPENTER_1);
                    break;
                
                case 1:
                    func_80075EBC(CARPENTER_1);
                    break;

                case 2:

                    switch (npcs[CARPENTER_1].unk_21) {
                        
                        case 0:
                                
                            npcs[CARPENTER_1].speed = 0;
                            npcs[CARPENTER_1].unk_22 = 0;
                            
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_1].entityIndex, 0);
                            
                            temp = getRandomNumberInRange(0, 60);
                            
                            if (temp < 8) {
                                npcs[CARPENTER_1].direction = temp;
                                npcs[CARPENTER_1].unk_21 = 1;
                            } 
                            
                            if (7 < temp && temp < 21) {
                                npcs[CARPENTER_1].unk_21 = 2;
                            }
                        
                            npcs[CARPENTER_1].flags |= 2;
                            
                            break;
                        
                        case 1:

                            npcs[CARPENTER_1].speed = 1;
                            npcs[CARPENTER_1].unk_22 = 0;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_1].entityIndex, 8);

                            temp = getRandomNumberInRange(0, 19);
                            
                            if (temp < 8) {
                                npcs[CARPENTER_1].unk_21 = 1;
                            } else {
                                npcs[CARPENTER_1].unk_21 = 0;
                            }

                            npcs[CARPENTER_1].flags |= 2;
                            
                            break;
                        
                        case 2:
                            
                            npcs[CARPENTER_1].speed = 0;
                            npcs[CARPENTER_1].unk_22 = 60;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_1].entityIndex, 20);
    
                            npcs[CARPENTER_1].unk_21 = 1;
                            npcs[CARPENTER_1].flags |= 2;
                            
                            break;
                        
                    }

            }
        }

        if (npcs[CARPENTER_1].unk_22) {
            npcs[CARPENTER_1].unk_22--; 
        } 
        
        func_80075A78(CARPENTER_1);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084770);

void func_80084770(void) {

    u16 temp;

    if (npcs[CARPENTER_2].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[CARPENTER_2].entityIndex) && npcs[CARPENTER_2].unk_22 == 0) {

            switch (npcs[CARPENTER_2].movingFlag) {

                case 0:
                    func_80075E28(CARPENTER_2);
                    break;
                
                case 1:
                    func_80075EBC(CARPENTER_2);
                    break;

                case 2:

                    switch (npcs[CARPENTER_2].unk_21) {
                        
                        case 0:
                                
                            npcs[CARPENTER_2].speed = 0;
                            npcs[CARPENTER_2].unk_22 = 0;
                            
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_2].entityIndex, 0);
                            
                            temp = getRandomNumberInRange(0, 60);
                            
                            if (temp < 8) {
                                npcs[CARPENTER_2].direction = temp;
                                npcs[CARPENTER_2].unk_21 = 1;
                            } 
                            
                            if (7 < temp && temp < 21) {
                                npcs[CARPENTER_2].unk_21 = 2;
                            }
                        
                            npcs[CARPENTER_2].flags |= 2;
                            
                            break;
                        
                        case 1:

                            npcs[CARPENTER_2].speed = 1;
                            npcs[CARPENTER_2].unk_22 = 0;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_2].entityIndex, 8);

                            temp = getRandomNumberInRange(0, 19);
                            
                            if (temp < 8) {
                                npcs[CARPENTER_2].unk_21 = 1;
                            } else {
                                npcs[CARPENTER_2].unk_21 = 0;
                            }

                            npcs[CARPENTER_2].flags |= 2;
                            
                            break;
                        
                        case 2:
                            
                            npcs[CARPENTER_2].speed = 0;
                            npcs[CARPENTER_2].unk_22 = 60;
                            setEntityAnimationWithDirectionChange(npcs[CARPENTER_2].entityIndex, 28);
    
                            npcs[CARPENTER_2].unk_21 = 1;
                            npcs[CARPENTER_2].flags |= 2;
                            
                            break;
                        
                    }

            }
        }

        if (npcs[CARPENTER_2].unk_22) {
            npcs[CARPENTER_2].unk_22--; 
        } 
        
        func_80075A78(CARPENTER_2);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800849EC);

void func_800849EC(void) {
    
    if (npcs[MASTER_CARPENTER].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[MASTER_CARPENTER].entityIndex)) {

            if (npcs[MASTER_CARPENTER].unk_22 == 0) {

                switch (npcs[MASTER_CARPENTER].movingFlag) {

                    case 0:
                        func_80075E28(MASTER_CARPENTER);
                        break;

                    case 1:
                        func_80075EBC(MASTER_CARPENTER);
                        break;

                    case 2:
                        npcs[MASTER_CARPENTER].speed = 0;
                        npcs[MASTER_CARPENTER].unk_21 = 10;
                        npcs[MASTER_CARPENTER].unk_22 = 0;
                    
                        setEntityAnimation(npcs[MASTER_CARPENTER].entityIndex, 0x10);
                    
                        npcs[MASTER_CARPENTER].flags |= 2;
                        break;
                    
                }
                
            }
            
        }
 
        if (npcs[MASTER_CARPENTER].unk_22) {
            npcs[MASTER_CARPENTER].unk_22--;
        }
        
        func_80075A78(MASTER_CARPENTER);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084B44);

void func_80084B44(void) {

    if (npcs[HARVEST_SPRITE_1].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_1].entityIndex)) {

            if (npcs[HARVEST_SPRITE_1].unk_22 == 0) {

                switch (npcs[HARVEST_SPRITE_1].movingFlag) {

                    case 0:
                        func_80075E28(HARVEST_SPRITE_1);
                        break;

                    case 1:
                        func_80075EBC(HARVEST_SPRITE_1);
                        break;
                    
                }
                
            }
            
        }
 
        if (npcs[HARVEST_SPRITE_1].unk_22) {
            npcs[HARVEST_SPRITE_1].unk_22--;
        }
        
        func_80075A78(HARVEST_SPRITE_1);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084C3C);

void func_80084C3C(void) {

    if (npcs[HARVEST_SPRITE_2].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_2].entityIndex)) {

            if (npcs[HARVEST_SPRITE_2].unk_22 == 0) {

                switch (npcs[HARVEST_SPRITE_2].movingFlag) {

                    case 0:
                        func_80075E28(HARVEST_SPRITE_2);
                        break;

                    case 1:
                        func_80075EBC(HARVEST_SPRITE_2);
                        break;
                    
                }
                
            }
            
        }
 
        if (npcs[HARVEST_SPRITE_2].unk_22) {
            npcs[HARVEST_SPRITE_2].unk_22--;
        }
        
        func_80075A78(HARVEST_SPRITE_2);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084D34);

void func_80084D34(void) {

    if (npcs[HARVEST_SPRITE_3].flags & 4) {

        if (checkEntityAnimationStateChanged(npcs[HARVEST_SPRITE_3].entityIndex)) {

            if (npcs[HARVEST_SPRITE_3].unk_22 == 0) {

                switch (npcs[HARVEST_SPRITE_3].movingFlag) {

                    case 0:
                        func_80075E28(HARVEST_SPRITE_3);
                        break;

                    case 1:
                        func_80075EBC(HARVEST_SPRITE_3);
                        break;
                    
                }
                
            }
            
        }
 
        if (npcs[HARVEST_SPRITE_3].unk_22) {
            npcs[HARVEST_SPRITE_3].unk_22--;
        }
        
        func_80075A78(HARVEST_SPRITE_3);
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084E2C);

void func_80084E2C(void) {

    if (npcs[SYDNEY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[SYDNEY].entityIndex) && !npcs[SYDNEY].unk_22) {
            updateAnimation(SYDNEY);
        }

        if (npcs[SYDNEY].unk_22) {
            npcs[SYDNEY].unk_22--; 
        } 
        
        func_80075A78(SYDNEY);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084F24);

void func_80084F24(void) {

    if (npcs[BARLEY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[BARLEY].entityIndex) && !npcs[BARLEY].unk_22) {
            updateAnimation(BARLEY);
        }

        if (npcs[BARLEY].unk_22) {
            npcs[BARLEY].unk_22--; 
        } 
        
        func_80075A78(BARLEY);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008501C);

void func_8008501C(void) {

    if (npcs[GOURMET_JUDGE].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[GOURMET_JUDGE].entityIndex) && !npcs[GOURMET_JUDGE].unk_22) {
            updateAnimation(GOURMET_JUDGE);
        }

        if (npcs[GOURMET_JUDGE].unk_22) {
            npcs[GOURMET_JUDGE].unk_22--; 
        } 
        
        func_80075A78(GOURMET_JUDGE);

    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80085114);

void func_80085114(void) {

    if (npcs[PHOTOGRAPHER].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[PHOTOGRAPHER].entityIndex) && !npcs[PHOTOGRAPHER].unk_22) {
            updateAnimation(PHOTOGRAPHER);
        }

        if (npcs[PHOTOGRAPHER].unk_22) {
            npcs[PHOTOGRAPHER].unk_22--; 
        } 
        
        func_80075A78(PHOTOGRAPHER);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008520C);

void func_8008520C(void) {

    if (npcs[0x28].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[0x28].entityIndex) && !npcs[0x28].unk_22) {
            updateAnimation(0x28);
        }

        if (npcs[0x28].unk_22) {
            npcs[0x28].unk_22--; 
        } 
        
        func_80075A78(0x28);
    
    }   

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80085304);

void func_80085304(void) {

    if (npcs[MARIA_HARRIS_BABY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[MARIA_HARRIS_BABY].entityIndex) && npcs[MARIA_HARRIS_BABY].unk_22 == 0) {

            switch (npcs[MARIA_HARRIS_BABY].movingFlag) {

                case 0:
                    npcs[MARIA_HARRIS_BABY].speed = 0;
                    npcs[MARIA_HARRIS_BABY].unk_21 = 10;
                    npcs[MARIA_HARRIS_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[MARIA_HARRIS_BABY].entityIndex, 8);
                
                    npcs[MARIA_HARRIS_BABY].flags |= 2;
                    break;

                case 1:
                    npcs[MARIA_HARRIS_BABY].speed = 0;
                    npcs[MARIA_HARRIS_BABY].unk_21 = 10;
                    npcs[MARIA_HARRIS_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[MARIA_HARRIS_BABY].entityIndex, 0);
                
                    npcs[MARIA_HARRIS_BABY].flags |= 2;
                    break;
                
            }
        }

        if (npcs[MARIA_HARRIS_BABY].unk_22) {
            npcs[MARIA_HARRIS_BABY].unk_22--; 
        } 
        
        func_80075A78(MARIA_HARRIS_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800853FC);

void func_800853FC(void) {

    if (npcs[POPURI_GRAY_BABY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[POPURI_GRAY_BABY].entityIndex) && npcs[POPURI_GRAY_BABY].unk_22 == 0) {

            switch (npcs[POPURI_GRAY_BABY].movingFlag) {

                case 0:
                    npcs[POPURI_GRAY_BABY].speed = 0;
                    npcs[POPURI_GRAY_BABY].unk_21 = 10;
                    npcs[POPURI_GRAY_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[POPURI_GRAY_BABY].entityIndex, 8);
                
                    npcs[POPURI_GRAY_BABY].flags |= 2;
                    break;

                case 1:
                    npcs[POPURI_GRAY_BABY].speed = 0;
                    npcs[POPURI_GRAY_BABY].unk_21 = 10;
                    npcs[POPURI_GRAY_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[POPURI_GRAY_BABY].entityIndex, 0);
                
                    npcs[POPURI_GRAY_BABY].flags |= 2;
                    break;
                
            }
        }

        if (npcs[POPURI_GRAY_BABY].unk_22) {
            npcs[POPURI_GRAY_BABY].unk_22--; 
        } 
        
        func_80075A78(POPURI_GRAY_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800854F4);

void func_800854F4(void) {

    if (npcs[ELLI_JEFF_BABY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[ELLI_JEFF_BABY].entityIndex) && npcs[ELLI_JEFF_BABY].unk_22 == 0) {

            switch (npcs[ELLI_JEFF_BABY].movingFlag) {

                case 0:
                    npcs[ELLI_JEFF_BABY].speed = 0;
                    npcs[ELLI_JEFF_BABY].unk_21 = 10;
                    npcs[ELLI_JEFF_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[ELLI_JEFF_BABY].entityIndex, 8);
                
                    npcs[ELLI_JEFF_BABY].flags |= 2;
                    break;

                case 1:
                    npcs[ELLI_JEFF_BABY].speed = 0;
                    npcs[ELLI_JEFF_BABY].unk_21 = 10;
                    npcs[ELLI_JEFF_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[ELLI_JEFF_BABY].entityIndex, 0);
                
                    npcs[ELLI_JEFF_BABY].flags |= 2;
                    break;
                
            }
        }

        if (npcs[ELLI_JEFF_BABY].unk_22) {
            npcs[ELLI_JEFF_BABY].unk_22--; 
        } 
        
        func_80075A78(ELLI_JEFF_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800855EC);

void func_800855EC(void) {

    if (npcs[ANN_CLIFF_BABY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[ANN_CLIFF_BABY].entityIndex) && npcs[ANN_CLIFF_BABY].unk_22 == 0) {

            switch (npcs[ANN_CLIFF_BABY].movingFlag) {

                case 0:
                    npcs[ANN_CLIFF_BABY].speed = 0;
                    npcs[ANN_CLIFF_BABY].unk_21 = 10;
                    npcs[ANN_CLIFF_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[ANN_CLIFF_BABY].entityIndex, 8);
                
                    npcs[ANN_CLIFF_BABY].flags |= 2;
                    break;

                case 1:
                    npcs[ANN_CLIFF_BABY].speed = 0;
                    npcs[ANN_CLIFF_BABY].unk_21 = 10;
                    npcs[ANN_CLIFF_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[ANN_CLIFF_BABY].entityIndex, 0);
                
                    npcs[ANN_CLIFF_BABY].flags |= 2;
                    break;
                
            }
        }   

        if (npcs[ANN_CLIFF_BABY].unk_22) {
            npcs[ANN_CLIFF_BABY].unk_22--; 
        } 
        
        func_80075A78(ANN_CLIFF_BABY);
    
    }    
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800856E4);

void func_800856E4(void) {

    if (npcs[KAREN_KAI_BABY].flags & 4) {
        
        if (checkEntityAnimationStateChanged(npcs[KAREN_KAI_BABY].entityIndex) && npcs[KAREN_KAI_BABY].unk_22 == 0) {

            switch (npcs[KAREN_KAI_BABY].movingFlag) {

                case 0:
                    npcs[KAREN_KAI_BABY].speed = 0;
                    npcs[KAREN_KAI_BABY].unk_21 = 10;
                    npcs[KAREN_KAI_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[KAREN_KAI_BABY].entityIndex, 8);
                
                    npcs[KAREN_KAI_BABY].flags |= 2;
                    break;

                case 1:
                    npcs[KAREN_KAI_BABY].speed = 0;
                    npcs[KAREN_KAI_BABY].unk_21 = 10;
                    npcs[KAREN_KAI_BABY].unk_22 = 0;
                
                    setEntityAnimationWithDirectionChange(npcs[KAREN_KAI_BABY].entityIndex, 0);
                
                    npcs[KAREN_KAI_BABY].flags |= 2;
                    break;
                
            }
        }

        if (npcs[KAREN_KAI_BABY].unk_22) {
            npcs[KAREN_KAI_BABY].unk_22--; 
        } 
        
        func_80075A78(KAREN_KAI_BABY);
    
    }   
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800857DC);

void func_800857DC(void) {

    if (npcs[0x2E].flags & 4) {
    
        if (checkEntityAnimationStateChanged(npcs[0x2E].entityIndex) && npcs[0x2E].unk_22 == 0) {

            switch (npcs[0x2E].movingFlag) {

                case 0:
                    func_80075E28(0x2E);
                    break;
                    
                case 1:
                    func_80075EBC(0x2E);
                    break;
                
            }
        }

        if (npcs[0x2E].unk_22) {
            npcs[0x2E].unk_22--; 
        } 
        
        func_80075A78(0x2E);
    
    }   
    
}



//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800858D4);

static inline bool checkNotHoldingItem() {
    return gPlayer.heldItem != 0;
}

// FIXME: totally cursed
// initiates dialogue with NPC on button press or pick up baby
u8 func_800858D4(void) {
    
    u8 result = 0;
    u8 i = 0;
    
    s32 i2;
    u16 temp;

    // ??
    u8 *ptr2 = &gPlayer.heldItem;
    u8 *ptr = npcAffection + 5;

    do {

        // ???
        ptr = npcAffection+5;
        
        if (npcs[i].flags & 4) {
            
            if (entities[npcs[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER && entities[npcs[i].entityIndex].buttonPressed == BUTTON_A) {

                // FIXME: might be static inline baby handler function
                if (i == BABY && gBabyAge < 120) {
                                 
                    // should be gPlayer.heldItem == 0
                    if (*ptr2 == 0) {
         
                        // should be !(gPlayer.flags & 1)
                        if (!(*(u16*)(ptr2+0x4A) & 1)) {

                            if (gBabyAge >= 30) {
                                *ptr2 = 0xC2;
                            } else {
                                *ptr2 = 0xBA;  
                            }

                            if (!checkDailyEventBit(HELD_BABY)) {
                                *ptr += adjustValue(*ptr, 2, MAX_AFFECTION);
                                setDailyEventBit(HELD_BABY);
                            }
                            
                            deactivateEntity(npcs[i].entityIndex);
                         
                            result = 2;
                            npcs[i].flags &= ~4;
                           
                        } 
                        
                        temp = 0xFFFF;
                        
                    } else {
                        temp = 1;
                    } 
                                 
                } else {
                    temp = checkNotHoldingItem();
                }
   
                if (temp != 0xFFFF) {

                    // ?? switch statement?
                    i2 = i;

                    if (i2 < 5) {

                        if (!(i2 < 0)) {

                            func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[i] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
                            func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[i] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
                       
                        }
                        
                    }
                    
                    showMessageBox(0, npcToDialogueBytecodeIndex[i], temp, 0, 0);
                    result = 1;
                    npcs[i].movingFlag = 0x10;
                    
                } 
            }    
        }
        
        i++;
            
    } while (i < MAX_NPCS && result == 0);

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80085C94);

bool func_80085C94(void) {

    bool found = FALSE;
    u8 i = 0;
    
    npcTalkingTo = 0xFF;

    while (i < MAX_NPCS && !found) {

        if (npcs[i].flags & 4) {

            if (entities[npcs[i].entityIndex].entityCollidedWithIndex == ENTITY_PLAYER) {
                npcs[i].movingFlag = 0x20;
                npcTalkingTo = i;
                found = TRUE;
            }

        }  

        i++;

    } 
    
    return found;

}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80085D48);

// blue feather
bool func_80085D48(int index, u16 arg1) {

    bool result;
    
    int temp;
    // FIXME: likely a struct?
    u8 arr[8];
    
    arr[7] = index;
    index = temp;
    index = arr[7];
    
    result = FALSE;
    
    if (npcs[index].flags & 4) {

        // FIXME: fake match
        // check if girl and load heart icon
        if ((index < 5) && (index >= (result = 0))) {
            func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
            func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
        }

        showMessageBox(0, npcToDialogueBytecodeIndex[arr[7]], arg1, 0, 0);
        result = TRUE;
        
    }
    
    return result;

}