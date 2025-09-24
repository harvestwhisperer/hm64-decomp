#include "common.h"

#include "game/npc.h"

#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"
#include "system/mapController.h"

#include "game/game.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/player.h"
#include "game/spriteInfo.h"
#include "game/weather.h"

#include "ld_symbols.h"

// bss
npcInfo npcs[MAX_NPCS];

u8 npcTalkingTo;
u16 D_801FBE2E;  
u16 D_801FBFBE;
u16 D_801FBFE6;

// data
// entity asset indices
extern u16 D_80114900[];
// dialogue bytecode indices; indexed by npc
extern u16 D_80114960[30];

// forward declarations
void func_80085114(void);
void func_8008520C(void);
void func_80075A78(u8);
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
        npcs[i].unk_24 = 0;
        npcs[i].unk_25 = 0;
        npcs[i].unk_21 = 0;
        
    }

}

//INCLUDE_RODATA("asm/nonmatchings/gamenpc", jtbl_80120010);

// jtbl_80120010
//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075374);

u8 func_80075374(u8 npcIndex, int arg1) {

    // FIXME: doesn't seem right
    int temp = arg1;

    if ((npcs[npcIndex].flags & 1) && npcs[npcIndex].levelIndex == gBaseMapIndex) {
        
        npcs[npcIndex].flags |= 4;

        switch (npcIndex) {
            case MARIA:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == MARIA) {
                        npcs[npcIndex].entityIndex = 0x23; 
                        break;
                    } 
                } 
                npcs[npcIndex].entityIndex = (u8)temp + 0x15;
                temp++;
                break;
            case POPURI:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == POPURI) {
                        npcs[npcIndex].entityIndex = 0x23;
                        break;
                    } 
                }
                npcs[npcIndex].entityIndex = (u8)temp + 0x15;
                temp++;
                break;
            case ELLI:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == ELLI) {
                        npcs[npcIndex].entityIndex = 0x23;
                        break;
                    } 
                }
                npcs[npcIndex].entityIndex = (u8)temp + 0x15;
                temp++;
                break;
            case ANN:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == ANN) {
                        npcs[npcIndex].entityIndex = 0x23;
                        break;
                    } 
                }
                npcs[npcIndex].entityIndex = (u8)temp + 0x15;
                temp++;
                break;
            case KAREN:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == KAREN) {
                        npcs[npcIndex].entityIndex = 0x23;
                        break;
                    } 
                }
                npcs[npcIndex].entityIndex = (u8)temp + 0x15;
                temp++;
                break;
            case BABY:                               
                npcs[npcIndex].entityIndex = 0x24;
                break;
            case PHOTOGRAPHER:
                npcs[npcIndex].entityIndex = 0x1D;
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
                npcs[npcIndex].entityIndex = (u8)temp + 0x15;
                temp++;
                break;
        }
    
        npcs[npcIndex].currentCoordinates.x = npcs[npcIndex].startingCoordinates.x;
        npcs[npcIndex].currentCoordinates.y = npcs[npcIndex].startingCoordinates.y;
        npcs[npcIndex].currentCoordinates.z = npcs[npcIndex].startingCoordinates.z;
    
        loadEntity(npcs[npcIndex].entityIndex, D_80114900[npcIndex], TRUE);
        setEntityCollidable(npcs[npcIndex].entityIndex, TRUE);
        setEntityYMovement(npcs[npcIndex].entityIndex, TRUE);
        func_8002FF38(npcs[npcIndex].entityIndex, FALSE);
    
        // FIXME: macro not matching; should be convertSpriteToWorldDirection(npcs[npcIndex].direction, getCurrentMapRotation(MAIN_MAP_INDEX))
        setEntityDirection(npcs[npcIndex].entityIndex, (npcs[npcIndex].direction + 8 - getCurrentMapRotation(MAIN_MAP_INDEX)) % 8);
        setEntityCoordinates(npcs[npcIndex].entityIndex, npcs[npcIndex].currentCoordinates.x, npcs[npcIndex].currentCoordinates.y, npcs[npcIndex].currentCoordinates.z);
    
        if (npcs[npcIndex].flags & 8) {

            switch (npcIndex) {                   
                case BABY:                             
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, 0x23, 0xFE);
                    break;
                case 41:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[MARIA].entityIndex, 0xFE);
                    break;
                case 42:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[POPURI].entityIndex, 0xFE);
                    break;
                case 43:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[ELLI].entityIndex, 0xFE);
                    break;
                case 44:                            
                    setEntityTrackingTarget(npcs[npcIndex].entityIndex, npcs[ANN].entityIndex, 0xFE);
                    break;
                case 45:                            
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

    return temp;
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800758B8);

void func_800758B8(void) {

    u8 npcIndex;
    u8 i;

    setNPCSpawningLocations();
    
    npcIndex = 0;
    
    for (i = 0; i < MAX_NPCS; i++) {
        npcIndex = func_80075374(i, npcIndex);
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

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075A78);


/* inlines */

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075E28);

inline void setStartMovingDefault(u8 npcIndex) {

    npcs[npcIndex].unk_20 = 0;
    npcs[npcIndex].unk_21 = 10;
    npcs[npcIndex].unk_22 = 0;

    setEntityAnimationWithDirectionChange(npcs[npcIndex].entityIndex, npcs[npcIndex].unk_24);

    npcs[npcIndex].flags |= 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80075EBC);

// FIXME: should be inline?
void func_80075EBC(u8 index) {
    
    u16 temp;
    
    if (!npcs[index].unk_21) {

        npcs[index].unk_20 = 0;
        npcs[index].unk_22 = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, npcs[index].unk_24);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcs[index].direction = temp*2;
            npcs[index].unk_21 = 1;
        }

        npcs[index].flags |= 2;

        return;
    }
    
    if (npcs[index].direction & 1) {

        npcs[index].unk_20 = 0;
        npcs[index].unk_22 = 0;
        npcs[index].unk_21 = 0;

    } else {

        npcs[index].unk_20 = 1;
        npcs[index].unk_22 = 0;

        setEntityAnimationWithDirectionChange(npcs[index].entityIndex, npcs[index].unk_25);
        
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

    npcs[index].unk_20 = 0;
    npcs[index].unk_21 = 10;
    npcs[index].unk_22 = 0;

    setEntityAnimationWithDirectionChange(npcs[index].entityIndex, arg1);

    npcs[index].flags |= 2;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076108);

inline void func_80076108(u8 index, u8 arg1, u8 arg2) {

    u16 temp;
    
    if (!npcs[index].unk_21) {

        npcs[index].unk_20 = 0;
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

        npcs[index].unk_20 = 0;
        npcs[index].unk_22 = 0;
        npcs[index].unk_21 = 0;

    } else {

        npcs[index].unk_20 = 1;
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



// jtbl_80120158
INCLUDE_ASM("asm/nonmatchings/game/npc", func_800762B4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076944);

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

u8 func_80076CF4(void) {

    u8 result = 0;

    if (checkLifeEventBit(MARRIED) && checkLifeEventBit(HAVE_BABY)) {

        if (gBabyAge < 30) {
            if (5 < gHour && gHour < 20) {
                result = 2;
                if (npcs[gWife].levelIndex != HOUSE) {
                    result = 1;
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

// jtbl_80120188
INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076DCC);



/* set starting locations */

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80076F10);

// jtbl_801201A0
INCLUDE_ASM("asm/nonmatchings/game/npc", func_80077D14);

// jtbl_801201C0
INCLUDE_ASM("asm/nonmatchings/game/npc", func_80078BF0);

// jtbl_801201E0
INCLUDE_ASM("asm/nonmatchings/game/npc", func_80079A74);

// jtbl_80120200
INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007A9B0);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007B828);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007BD50);

// jtbl_80120220
INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007C088);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007C3D4);

// jtbl_80120240
// jtbl_80120258
INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007C7C4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007CEB4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007D3C4);

// jtbl_80120278
INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007D6E0);

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007D9F4);

void func_8007D9F4(void) {

    npcs[LILLIA].unk_1A = 0x40;
    npcs[LILLIA].unk_1B = 0x40;
    npcs[LILLIA].unk_25 = 8;
    npcs[LILLIA].unk_24 = 0;

    if (gWeather != SUNNY) goto DEFAULT;

    switch (gDayOfWeek) {

        case SUNDAY:

            if (8 < gHour && gHour < 17) { 
                npcs[LILLIA].levelIndex = SQUARE;
                npcs[LILLIA].startingCoordinates.y = 0;
                npcs[LILLIA].direction = SOUTH;
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
                npcs[LILLIA].direction = NORTH;
                npcs[LILLIA].unk_1E = 0;
                npcs[LILLIA].startingCoordinates.x = 64.0f;
                npcs[LILLIA].startingCoordinates.z = -128.0f;
                npcs[LILLIA].flags |= 1;
            }
    
            break;

    }
    
    npcs[LILLIA].movingFlag = npcs[LILLIA].unk_1E;

}

// jtbl_80120298
//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007DB38);

void func_8007DB38(void) {

    npcs[BASIL].unk_1A = 0x40;
    npcs[BASIL].unk_1B = 0x40;
    npcs[BASIL].unk_24 = 0;
    npcs[BASIL].unk_25 = 8;

    if (!checkLifeEventBit(BASIL_IN_TOWN)) goto FUNC_END;

    if (gWeather != SUNNY) goto NOT_SUNNY;
            
    switch (gDayOfWeek) {
        case SUNDAY:
            if (8 < gHour && gHour < 17) {
                npcs[BASIL].levelIndex = SQUARE;
                npcs[BASIL].startingCoordinates.y = 0;
                npcs[BASIL].direction = SOUTH;
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
                npcs[BASIL].direction = WEST;
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
                npcs[BASIL].direction = SOUTH;
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
                npcs[BASIL].direction = NORTH;
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
        npcs[BASIL].direction = WEST;
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
    npcs[ELLEN].unk_24 = 0;
    npcs[ELLEN].unk_25 = 8;
    
    if (!checkLifeEventBit(ELLEN_DIED)) {
        if (gWeather == SUNNY && (7 < gHour && gHour < 17)) {
                npcs[ELLEN].levelIndex = VILLAGE_1;
                npcs[ELLEN].startingCoordinates.y = 0;
                npcs[ELLEN].direction = NORTH;
                npcs[ELLEN].unk_1E = 0;
                npcs[ELLEN].startingCoordinates.x = 320.0f;
                npcs[ELLEN].startingCoordinates.z = 128.0f;
                npcs[ELLEN].flags |= 1;
        } else {
            if (7 < gHour && gHour < 17) {
                npcs[ELLEN].levelIndex = BAKERY;
                npcs[ELLEN].startingCoordinates.y = 0;
                npcs[ELLEN].direction = NORTH;
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
    npcs[DOUG].unk_24 = 0;
    npcs[DOUG].unk_25 = 8;

    // FIXME: something off here
    if (temp >= SUNDAY && (temp < THURSDAY || temp < 7 && (temp2 = temp) >= FRIDAY) && (7 < gHour && gHour < 17)) {
        npcs[DOUG].levelIndex = RANCH_STORE;
        npcs[DOUG].startingCoordinates.y = 0;
        npcs[DOUG].direction = WEST;
        npcs[DOUG].unk_1E = 0;
        npcs[DOUG].startingCoordinates.x = -96.0f;
        npcs[DOUG].startingCoordinates.z = 64.0f;
        npcs[DOUG].flags |= 1;
    }

    if (NIGHTTIME && npcs[DOUG].location < 2) {
        npcs[DOUG].levelIndex = TAVERN;
        npcs[DOUG].startingCoordinates.y = 0;
        npcs[DOUG].direction = EAST;
        npcs[DOUG].unk_1E = 0;
        npcs[DOUG].startingCoordinates.x = -96.0f;
        npcs[DOUG].startingCoordinates.z = 48.0f;
        npcs[DOUG].flags |= 1;
    }

    npcs[DOUG].movingFlag = npcs[DOUG].unk_1E;
    
}

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E07C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E2E4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E440);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007E600);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007ECE4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007F378);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007F4E0);

// jtbl_801202B8
INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007F81C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FA44);

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FC0C);

void func_8007FC0C(void) {

    npcs[SHIPPER].unk_1A = 0x40;
    npcs[SHIPPER].unk_1B = 0x40;
    npcs[SHIPPER].unk_24 = 0;
    npcs[SHIPPER].unk_25 = 8;

    if ((17 < gHour && gHour < 24) && npcs[SHIPPER].location < 2) {
        
        npcs[SHIPPER].levelIndex = TAVERN;
        npcs[SHIPPER].startingCoordinates.y = 0.0f;
        npcs[SHIPPER].direction = EAST;
        npcs[SHIPPER].unk_1E = 0;
    
        npcs[SHIPPER].startingCoordinates.x = 128.0f;
        npcs[SHIPPER].startingCoordinates.z = 64.0f;
    
        npcs[SHIPPER].flags |= 1;

    }

    npcs[SHIPPER].movingFlag =  npcs[SHIPPER].unk_1E;
    
}

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FCD8);

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FE40);

void func_8007FE40(void) { 

    int temp = gDayOfWeek;

    npcs[DUKE].unk_1A = 0x40;
    npcs[DUKE].unk_1B = 0x40;
    npcs[DUKE].unk_24 = 0;
    npcs[DUKE].unk_25 = 8;

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
        npcs[DUKE].unk_24 = 0;
        npcs[DUKE].unk_25 = 8;

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

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8007FF04);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800800D0);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080364);

//INCLUDE_ASM("asm/nonmatchings/game/npc", func_80080630);

void func_80080630(void) {

    npcs[MASTER_CARPENTER].unk_1A = 0x40;
    npcs[MASTER_CARPENTER].unk_1B = 0x40;
    npcs[MASTER_CARPENTER].unk_24 = 0;
    npcs[MASTER_CARPENTER].unk_25 = 8;

    if (!checkLifeEventBit(0xD) && !checkDailyEventBit(9) 
        && (gYear != 1 
            || (gSeason != AUTUMN || !(19 < gDayOfMonth && gDayOfMonth < 28))
            && ((gSeason != WINTER || !(8 < gDayOfMonth && gDayOfMonth < 17)))
           )
       ) {

        if (gWeather == SUNNY && npcs[MASTER_CARPENTER].location >= 3) {

            if (7 < gHour && gHour < 18) {    
            
                npcs[MASTER_CARPENTER].levelIndex = 0x15;
        
                npcs[MASTER_CARPENTER].startingCoordinates.y = 0;
                npcs[MASTER_CARPENTER].direction = NORTH;
                npcs[MASTER_CARPENTER].unk_1E = 0;
                
                npcs[MASTER_CARPENTER].startingCoordinates.x = -112.0f;
                npcs[MASTER_CARPENTER].startingCoordinates.z = 112.0f;
        
                npcs[MASTER_CARPENTER].flags |= 1;
                
            }

        } else if (7 < gHour && gHour < 18) {

            npcs[MASTER_CARPENTER].levelIndex = 0x21;
    
            npcs[MASTER_CARPENTER].startingCoordinates.y = 0;
            npcs[MASTER_CARPENTER].direction = NORTH;
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
    npcs[HARVEST_SPRITE_1].unk_24 = 0;
    npcs[HARVEST_SPRITE_1].unk_25 = 8;

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
    npcs[HARVEST_SPRITE_2].unk_24 = 0;
    npcs[HARVEST_SPRITE_2].unk_25 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_2].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_2].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_2].direction = NORTH;
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
    npcs[HARVEST_SPRITE_3].unk_24 = 0;
    npcs[HARVEST_SPRITE_3].unk_25 = 8;

    if (7 < gHour && gHour < 19) {
        
        npcs[HARVEST_SPRITE_3].levelIndex = HARVEST_SPRITE_CAVE;
        npcs[HARVEST_SPRITE_3].startingCoordinates.y = 0.0f;
        npcs[HARVEST_SPRITE_3].direction = EAST;
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
    npcs[SYDNEY].unk_24 = 0;
    npcs[SYDNEY].unk_25 = 8;

    if (gSeason != WINTER && gWeather == SUNNY && npcs[SYDNEY].location >= 2 && (7 < gHour && gHour < 17)) {

        npcs[SYDNEY].levelIndex = MOON_MOUNTAIN;
        npcs[SYDNEY].startingCoordinates.y = 0.0f;
        npcs[SYDNEY].direction = EAST;
        npcs[SYDNEY].unk_1E = 1;

        npcs[SYDNEY].startingCoordinates.x = 64.0F;
        npcs[SYDNEY].startingCoordinates.z = -32.0f;

        npcs[SYDNEY].flags |= 1;
        
    } else if (7 < gHour && gHour < 17) {
        
        npcs[SYDNEY].levelIndex = DUMPLING_HOUSE;
        npcs[SYDNEY].startingCoordinates.y = 0.0f;
        npcs[SYDNEY].direction = WEST;
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
    npcs[BARLEY].unk_24 = 0;
    npcs[BARLEY].unk_25 = 8;

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
        npcs[BARLEY].direction = NORTH;
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
    npcs[GOURMET_JUDGE].unk_24 = 0;
    npcs[GOURMET_JUDGE].unk_25 = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(0x94) && (17 < gHour && gHour < 24)) {

        npcs[GOURMET_JUDGE].levelIndex = TAVERN;
        npcs[GOURMET_JUDGE].startingCoordinates.y = 0;
        npcs[GOURMET_JUDGE].direction = EAST;
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
    npcs[PHOTOGRAPHER].unk_24 = 0;
    npcs[PHOTOGRAPHER].unk_25 = 8;

    if (gYear == 1 && gSeason == WINTER && (gDayOfMonth < 5) && checkLifeEventBit(0x94) && (9 < gHour && gHour < 16)) {

        npcs[PHOTOGRAPHER].levelIndex = BARN;
        npcs[PHOTOGRAPHER].startingCoordinates.y = 0;
        npcs[PHOTOGRAPHER].direction = NORTHWEST;
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
    npcs[40].unk_24 = 0;
    npcs[40].unk_25 = 8;

    if (checkLifeEventBit(0x20) && npcs[40].location == 0 && (8 < gHour && gHour < 17)) {

        npcs[40].levelIndex = ROAD;
        npcs[40].startingCoordinates.y = 0;
        npcs[40].direction = SOUTH;
        npcs[40].unk_1E = 1;

        npcs[40].startingCoordinates.x = -32.0f;
        npcs[40].startingCoordinates.z = 64.0f;

        npcs[40].flags |= 1;

    }

    if (checkLifeEventBit(0x19) && (8 < gHour && gHour < 17)) {
        
        npcs[40].levelIndex = VILLAGE_1;
        npcs[40].startingCoordinates.y = 0;
        npcs[40].direction = NORTH;
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
    
    npcs[41].unk_1A = 0x40;
    npcs[41].unk_1B = 0x40;
    npcs[41].unk_24 = 0;
    npcs[41].unk_25 = 0;

    temp = func_80076DCC(0);

    if (temp == 1) {

        if (npcs[MARIA].flags & 1) {
            
            npcs[41].startingCoordinates.x = 0.0f;
            npcs[41].startingCoordinates.y = 0.0f;
            npcs[41].startingCoordinates.z = 0.0f;

            npcs[41].unk_1E = 0;

            npcs[41].unk_24 = 8;
            npcs[41].unk_25 = 8;

            npcs[41].levelIndex = npcs[MARIA].levelIndex;
            npcs[41].flags |= (1 | 8);

            npcs[41].direction = npcs[MARIA].direction;
            
        }
        
    } 
    
    if (temp == 2) {

        if (npcs[MARIA].flags & 1) {

            npcs[41].levelIndex = npcs[0].levelIndex;
            
            vec1.x = 0.0f;
            vec1.y = 0.0f;
            vec1.z = 32.0f;
            
            rotationAngles.x = 0.0f;
            rotationAngles.y = getSpriteYValueFromDirection(npcs[0].direction);
            rotationAngles.z = 0.0f;
            
            rotateVector3D(vec1, &npcs[41].startingCoordinates, rotationAngles);

            npcs[41].unk_1E = 1;

            npcs[41].unk_24 = 0;
            npcs[41].unk_25 = 0;

            npcs[41].flags |= 1;

            npcs[41].startingCoordinates.x = npcs[MARIA].startingCoordinates.x + vec1.x;
            npcs[41].startingCoordinates.y = npcs[MARIA].startingCoordinates.y;
            npcs[41].startingCoordinates.z = npcs[MARIA].startingCoordinates.z + vec1.z;
            
            npcs[41].direction = npcs[MARIA].direction;
                            
        }
        
    }

    npcs[41].movingFlag = npcs[41].unk_1E;
    
}

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081228);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081420);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081618);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081810);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081A08);



/* animations */

static inline void updateAnimation(u8 index) {
    
    switch (npcs[index].movingFlag) {

        case 0:
            setStartMovingDefault(index);
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

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081C90);

// jtbl_801202D8
INCLUDE_ASM("asm/nonmatchings/game/npc", func_80081E5C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082164);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082330);

// jtbl_801202F0
INCLUDE_ASM("asm/nonmatchings/game/npc", func_800824FC);

// jtbl_80120308
INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008277C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082A0C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082B04);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082C5C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082DB4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082EAC);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80082FA4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008309C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083194);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_8008328C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80083384);

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
                    setStartMovingDefault(GOTZ);
                    break;

                case 1:
                    func_80075EBC(GOTZ);
                    break;

                case 3:

                    // FIXME: should be inline function
                    if (npcs[GOTZ].unk_21 == 0 ) {

                        npcs[GOTZ].unk_20 = 0;
                        npcs[GOTZ].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[GOTZ].entityIndex, 0);

                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < SOUTH) {
                            npcs[GOTZ].direction = tempDirection * 2;
                            npcs[GOTZ].unk_21 = 1;
                        }

                        npcs[GOTZ].flags |= 2;
                        
                    } else {       

                        npcs[GOTZ].unk_20 = 1;
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
                    setStartMovingDefault(SASHA);
                    break;

                case 1:
                    func_80075EBC(SASHA);
                    break;

                case 3:

                    // FIXME: should be inline function
                    if (npcs[SASHA].unk_21 == 0 ) {

                        npcs[SASHA].unk_20 = 0;
                        npcs[SASHA].unk_22 = 0;

                        setEntityAnimationWithDirectionChange(npcs[SASHA].entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < SOUTH) {
                            npcs[SASHA].direction = tempDirection * 2;
                            npcs[SASHA].unk_21 = 1;
                        }

                        npcs[SASHA].flags |= 2;
                        
                    } else {       

                        npcs[SASHA].unk_20 = 1;
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
                    setStartMovingDefault(KENT);
                    break;
                
                case 1:
                    func_80075EBC(KENT);
                    break;

                case 2:

                    // FIXME: should be inline function
                    if (npcs[KENT].unk_21 == 0) {
                        
                        npcs[KENT].unk_20 = 0;
                        npcs[KENT].unk_22 = 0;
                        setEntityAnimationWithDirectionChange(npcs[KENT].entityIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < SOUTH) {
                            npcs[KENT].direction = tempDirection * 2 ;
                            npcs[KENT].unk_21 = 1;
                        } 

                        npcs[KENT].flags |= 2;  

                    } else {

                        npcs[KENT].unk_20 = 2;
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

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800844F4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084770);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800849EC);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084B44);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084C3C);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80084D34);

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

INCLUDE_ASM("asm/nonmatchings/game/npc", func_80085304);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800853FC);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800854F4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800855EC);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800856E4);

INCLUDE_ASM("asm/nonmatchings/game/npc", func_800857DC);




//INCLUDE_ASM("asm/nonmatchings/game/npc", func_800858D4);

static inline bool checkNotHoldingItem() {
    return gPlayer.heldItem != 0;
}

// FIXME: totally cursed
// initiates dialogue with NPC on button press
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
            
            if (!entities[npcs[i].entityIndex].entityCollidedWithIndex && entities[npcs[i].entityIndex].buttonPressed == BUTTON_A) {

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

                            func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_START, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_END, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, (npcAffection[i] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
                            func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_START, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_END, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, (npcAffection[i] / 52) + 5, 0xFE, 106.0f, -15.0f, 0.0f);
                       
                        }
                        
                    }
                    
                    showMessageBox(0, D_80114960[i], temp, 0, 0);
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
            if (!entities[npcs[i].entityIndex].entityCollidedWithIndex) {
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
            func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_START, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_END, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
            func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_START, (void*)DIALOGUE_ICONS_TEXTURE_VADDR_END, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_START, (void*)DIALOGUE_ICONS_ASSETS_INDEX_VADDR_END, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
        }

        // D_80114960 =  dialogue bytecode addresses indices
        showMessageBox(0, D_80114960[arr[7]], arg1, 0, 0);
        result = TRUE;
        
    }
    
  return result;

}