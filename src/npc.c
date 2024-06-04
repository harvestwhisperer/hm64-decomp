#include "common.h"

#include "npc.h"

#include "system/dialogue.h"
#include "system/map.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/mapContext.h"

#include "game.h"
#include "gameStatus.h"
#include "level.h"
#include "weather.h"

// bss
npcInfo npcInfoArray[0x30];

u16 D_80114900[];
u8 D_801C3E18;
u16 D_801FBE2E;  
u16 D_801FBFBE;
u16 D_801FBFE6;

// data
extern u16 D_80114960[30];

// forward declarations
void func_80085114(void);
void func_8008520C(void);
void func_80075A78(u8);
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


 
//INCLUDE_ASM(const s32, "npc", func_800752C0);

void func_800752C0(void) {
    
    u8 i;

    for (i = 0; i < TOTAL_NPCS; i++) {
        
        if (npcInfoArray[i].flags & 4) {
            func_8002FA2C(npcInfoArray[i].spriteIndex);
        }
        
        npcInfoArray[i].flags = 0;
        npcInfoArray[i].movingFlag = 0;
        npcInfoArray[i].unk_24 = 0;
        npcInfoArray[i].unk_25 = 0;
        npcInfoArray[i].unk_21 = 0;
        
    }

}

//INCLUDE_RODATA(const s32, "npc", jtbl_80120010);

// jtbl_80120010
//INCLUDE_ASM(const s32, "npc", func_80075374);

u8 func_80075374(u8 npcIndex, int arg1) {

    // FIXME: doesn't seem right
    int temp = arg1;

    if ((npcInfoArray[npcIndex].flags & 1) && npcInfoArray[npcIndex].levelIndex == gBaseMapIndex) {
        
        npcInfoArray[npcIndex].flags |= 4;

        switch (npcIndex) {
            case MARIA:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == MARIA) {
                        npcInfoArray[npcIndex].spriteIndex = 0x23; 
                        break;
                    } 
                } 
                npcInfoArray[npcIndex].spriteIndex = (u8)temp + 0x15;
                temp++;
                break;
            case POPURI:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == POPURI) {
                        npcInfoArray[npcIndex].spriteIndex = 0x23;
                        break;
                    } 
                }
                npcInfoArray[npcIndex].spriteIndex = (u8)temp + 0x15;
                temp++;
                break;
            case ELLI:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == ELLI) {
                        npcInfoArray[npcIndex].spriteIndex = 0x23;
                        break;
                    } 
                }
                npcInfoArray[npcIndex].spriteIndex = (u8)temp + 0x15;
                temp++;
                break;
            case ANN:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == ANN) {
                        npcInfoArray[npcIndex].spriteIndex = 0x23;
                        break;
                    } 
                }
                npcInfoArray[npcIndex].spriteIndex = (u8)temp + 0x15;
                temp++;
                break;
            case KAREN:
                if (checkLifeEventBit(MARRIED)) {
                    if (gWife == KAREN) {
                        npcInfoArray[npcIndex].spriteIndex = 0x23;
                        break;
                    } 
                }
                npcInfoArray[npcIndex].spriteIndex = (u8)temp + 0x15;
                temp++;
                break;
            case BABY:                               
                npcInfoArray[npcIndex].spriteIndex = 0x24;
                break;
            case PHOTOGRAPHER:
                npcInfoArray[npcIndex].spriteIndex = 0x1D;
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
                npcInfoArray[npcIndex].spriteIndex = (u8)temp + 0x15;
                temp++;
                break;
        }
    
        npcInfoArray[npcIndex].currentCoordinates.x = npcInfoArray[npcIndex].startingCoordinates.x;
        npcInfoArray[npcIndex].currentCoordinates.y = npcInfoArray[npcIndex].startingCoordinates.y;
        npcInfoArray[npcIndex].currentCoordinates.z = npcInfoArray[npcIndex].startingCoordinates.z;
    
        func_8002E284(npcInfoArray[npcIndex].spriteIndex, D_80114900[npcIndex], 1);
        func_8003019C(npcInfoArray[npcIndex].spriteIndex, 1);
        func_80030054(npcInfoArray[npcIndex].spriteIndex, 1);
        func_8002FF38(npcInfoArray[npcIndex].spriteIndex, 0);
    
        setSpriteDirection(npcInfoArray[npcIndex].spriteIndex, (npcInfoArray[npcIndex].direction + 8 - func_8003C1A4(MAIN_MAP_INDEX)) % 8);
        func_8002FD80(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[npcIndex].currentCoordinates.x, npcInfoArray[npcIndex].currentCoordinates.y, npcInfoArray[npcIndex].currentCoordinates.z);
    
        if (npcInfoArray[npcIndex].flags & 8) {
            switch (npcIndex) {                   
                case BABY:                             
                    func_8002ECD4(npcInfoArray[npcIndex].spriteIndex, 0x23, 0xFE);
                    break;
                case 41:                            
                    func_8002ECD4(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[MARIA].spriteIndex, 0xFE);
                    break;
                case 42:                            
                    func_8002ECD4(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[POPURI].spriteIndex, 0xFE);
                    break;
                case 43:                            
                    func_8002ECD4(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[ELLI].spriteIndex, 0xFE);
                    break;
                case 44:                            
                    func_8002ECD4(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[ANN].spriteIndex, 0xFE);
                    break;
                case 45:                            
                    func_8002ECD4(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[KAREN].spriteIndex, 0xFE);
                    break;
                }
            
                func_8002EDF0(npcInfoArray[npcIndex].spriteIndex, 0, 0xE, 0x14);
                func_8003019C(npcInfoArray[npcIndex].spriteIndex, 0);
                func_80030054(npcInfoArray[npcIndex].spriteIndex, 0);
                func_80030240(npcInfoArray[npcIndex].spriteIndex, 0);
                func_800302E4(npcInfoArray[npcIndex].spriteIndex, 0);
        }
    
        npcInfoArray[npcIndex].unk_21 = 0;
        npcInfoArray[npcIndex].unk_22 = 0;
        npcInfoArray[npcIndex].flags |= 4;
        
    }

    return temp;
}

//INCLUDE_ASM(const s32, "npc", func_800758B8);

void func_800758B8(void) {

    u8 spriteIndex;
    u8 i;

    setNPCSpawningLocations();
    
    spriteIndex = 0;
    
    for (i = 0; i < TOTAL_NPCS; i++) {
        spriteIndex = func_80075374(i, spriteIndex);
    }
    
    // handle all npc animations
    setNpcAnimations();
}

//INCLUDE_ASM(const s32, "npc", func_80075910);

void func_80075910(void) {

    u8 i;

    for (i = 0; i < TOTAL_NPCS; i++) {
        if (npcInfoArray[i].flags & 1) {
            if (npcInfoArray[i].levelIndex == gBaseMapIndex) {
                npcInfoArray[i].currentCoordinates.x = renderedSprites[npcInfoArray[i].spriteIndex].startingCoordinates.x;
                npcInfoArray[i].currentCoordinates.y = renderedSprites[npcInfoArray[i].spriteIndex].startingCoordinates.y;
                npcInfoArray[i].currentCoordinates.z = renderedSprites[npcInfoArray[i].spriteIndex].startingCoordinates.z; 
            }
            
        }
    }
}

//INCLUDE_ASM(const s32, "npc", func_80075A18);

void func_80075A18(void) {
    
    u8 i;

    for (i = 0; i < TOTAL_NPCS; i++) {
        npcInfoArray[i].location = getRandomNumberInRange(0, 3);
    }

}

INCLUDE_ASM(const s32, "npc", func_80075A78);


/* inlines */

//INCLUDE_ASM(const s32, "npc", func_80075E28);

inline void setStartMovingDefault(u8 npcIndex) {

    npcInfoArray[npcIndex].unk_20 = 0;
    npcInfoArray[npcIndex].unk_21 = 10;
    npcInfoArray[npcIndex].unk_22 = 0;

    setSpriteAnimation(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[npcIndex].unk_24);

    npcInfoArray[npcIndex].flags |= 2;
    
}

//INCLUDE_ASM(const s32, "npc", func_80075EBC);

// FIXME: should be inline?
void func_80075EBC(u8 index) {
    
    u16 temp;
    
    if (!npcInfoArray[index].unk_21) {

        npcInfoArray[index].unk_20 = 0;
        npcInfoArray[index].unk_22 = 0;

        setSpriteAnimation(npcInfoArray[index].spriteIndex, npcInfoArray[index].unk_24);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcInfoArray[index].direction = temp*2;
            npcInfoArray[index].unk_21 = 1;
        }

        npcInfoArray[index].flags |= 2;

        return;
    }
    
    if (npcInfoArray[index].direction & 1) {

        npcInfoArray[index].unk_20 = 0;
        npcInfoArray[index].unk_22 = 0;
        npcInfoArray[index].unk_21 = 0;

    } else {

        npcInfoArray[index].unk_20 = 1;
        npcInfoArray[index].unk_22 = 0;

        setSpriteAnimation(npcInfoArray[index].spriteIndex, npcInfoArray[index].unk_25);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npcInfoArray[index].unk_21 = 1;
            
        } else {
            npcInfoArray[index].unk_21 = 0;
        }
        
    }
    
    npcInfoArray[index].flags |= 2;
}

//INCLUDE_ASM(const s32, "npc", func_80076080);

inline void func_80076080(u8 index, u8 arg1) {

    npcInfoArray[index].unk_20 = 0;
    npcInfoArray[index].unk_21 = 10;
    npcInfoArray[index].unk_22 = 0;

    setSpriteAnimation(npcInfoArray[index].spriteIndex, arg1);

    npcInfoArray[index].flags |= 2;
}

//INCLUDE_ASM(const s32, "npc", func_80076108);

inline void func_80076108(u8 index, u8 arg1, u8 arg2) {

    u16 temp;
    
    if (!npcInfoArray[index].unk_21) {

        npcInfoArray[index].unk_20 = 0;
        npcInfoArray[index].unk_22 = 0;

        setSpriteAnimation(npcInfoArray[index].spriteIndex, arg1);

        temp = getRandomNumberInRange(0, 60);

        if (temp < 4) {
            npcInfoArray[index].direction = temp*2;
            npcInfoArray[index].unk_21 = 1;
        }

        npcInfoArray[index].flags |= 2;
        return;
    }
    
    if (npcInfoArray[index].direction & 1) {

        npcInfoArray[index].unk_20 = 0;
        npcInfoArray[index].unk_22 = 0;
        npcInfoArray[index].unk_21 = 0;

    } else {

        npcInfoArray[index].unk_20 = 1;
        npcInfoArray[index].unk_22 = 0;
        
        setSpriteAnimation(npcInfoArray[index].spriteIndex, arg2);
        
        if (getRandomNumberInRange(0, 19) < 8) {
            npcInfoArray[index].unk_21 = 1;
        } else {
            npcInfoArray[index].unk_21 = 0;
        }
        
    }
    
    npcInfoArray[index].flags |= 2;
    
}



// jtbl_80120158
INCLUDE_ASM(const s32, "npc", func_800762B4);

INCLUDE_ASM(const s32, "npc", func_80076944);

//INCLUDE_ASM(const s32, "npc", setNPCSpawningLocations);

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

//INCLUDE_ASM(const s32, "npc", func_80076CF4);

u8 func_80076CF4(void) {

    u8 result = 0;

    if (checkLifeEventBit(0) && checkLifeEventBit(1)) {

        if (gBabyAge < 30) {
            if (5 < gHour && gHour < 20) {
                result = 2;
                if (npcInfoArray[gWife].levelIndex != HOUSE) {
                    result = 1;
                }
            } else {
                result = 3;
            }  
        } else {
            if (5 < gHour && gHour < 20) {
                // -(120 < DAT_80189134) & 6U | 4;
                result = (gBabyAge < 120) ? 4 : 6;
            } else {
                result = 5;
            }
        }
    }
    
    return result;
    
}

// jtbl_80120188
INCLUDE_ASM(const s32, "npc", func_80076DCC);


/* set starting locations */

INCLUDE_ASM(const s32, "npc", func_80076F10);

// jtbl_801201A0
INCLUDE_ASM(const s32, "npc", func_80077D14);

// jtbl_801201C0
INCLUDE_ASM(const s32, "npc", func_80078BF0);

// jtbl_801201E0
INCLUDE_ASM(const s32, "npc", func_80079A74);

// jtbl_80120200
INCLUDE_ASM(const s32, "npc", func_8007A9B0);

INCLUDE_ASM(const s32, "npc", func_8007B828);

INCLUDE_ASM(const s32, "npc", func_8007BD50);

// jtbl_80120220
INCLUDE_ASM(const s32, "npc", func_8007C088);

INCLUDE_ASM(const s32, "npc", func_8007C3D4);

// jtbl_80120240
// jtbl_80120258
INCLUDE_ASM(const s32, "npc", func_8007C7C4);

INCLUDE_ASM(const s32, "npc", func_8007CEB4);

INCLUDE_ASM(const s32, "npc", func_8007D3C4);

// jtbl_80120278
INCLUDE_ASM(const s32, "npc", func_8007D6E0);

//INCLUDE_ASM(const s32, "npc", func_8007D9F4);

void func_8007D9F4(void) {

    npcInfoArray[LILLIA].unk_1A = 0x40;
    npcInfoArray[LILLIA].unk_1B = 0x40;
    npcInfoArray[LILLIA].unk_25 = 8;
    npcInfoArray[LILLIA].unk_24 = 0;

    if (gWeather != SUNNY) goto DEFAULT;

    switch (gDayOfWeek) {

        case SUNDAY:

            if (8 < gHour && gHour < 17) { 
                npcInfoArray[LILLIA].levelIndex = SQUARE;
                npcInfoArray[LILLIA].startingCoordinates.y = 0;
                npcInfoArray[LILLIA].direction = 4;
                npcInfoArray[LILLIA].unk_1E = 0;
                npcInfoArray[LILLIA].startingCoordinates.x = -144.0f;
                npcInfoArray[LILLIA].startingCoordinates.z = 256.0f;
                npcInfoArray[LILLIA].flags |= 1;
            }
            break;

        default:
            break;

        case MONDAY ... SATURDAY:
DEFAULT:
            if (8 < gHour && gHour < 17) {
                npcInfoArray[LILLIA].levelIndex = FLOWER_SHOP;
                npcInfoArray[LILLIA].startingCoordinates.y = 0;
                npcInfoArray[LILLIA].direction = 0;
                npcInfoArray[LILLIA].unk_1E = 0;
                npcInfoArray[LILLIA].startingCoordinates.x = 64.0f;
                npcInfoArray[LILLIA].startingCoordinates.z = -128.0f;
                npcInfoArray[LILLIA].flags |= 1;
            }
    
            break;
    }
    
    npcInfoArray[LILLIA].movingFlag = npcInfoArray[LILLIA].unk_1E;

}

// jtbl_80120298
//INCLUDE_ASM(const s32, "npc", func_8007DB38);

void func_8007DB38(void) {

    npcInfoArray[BASIL].unk_1A = 0x40;
    npcInfoArray[BASIL].unk_1B = 0x40;
    npcInfoArray[BASIL].unk_24 = 0;
    npcInfoArray[BASIL].unk_25 = 8;

    if (!checkLifeEventBit(BASIL_IN_TOWN)) goto FUNC_END;

    if (gWeather != SUNNY) goto NOT_SUNNY;
            
    switch (gDayOfWeek) {
        case SUNDAY:
            if (8 < gHour && gHour < 17) {
                npcInfoArray[BASIL].levelIndex = SQUARE;
                npcInfoArray[BASIL].startingCoordinates.y = 0;
                npcInfoArray[BASIL].direction = 4;
                npcInfoArray[BASIL].unk_1E = 0;
                npcInfoArray[BASIL].startingCoordinates.x = -176.0f;
                npcInfoArray[BASIL].startingCoordinates.z = 256.0f;
                npcInfoArray[BASIL].flags |= 1;
            }
            break;
        case THURSDAY:
            if (7  < gHour && gHour < 18) {
                npcInfoArray[BASIL].levelIndex = VILLAGE_1;
                npcInfoArray[BASIL].startingCoordinates.y = 0;
                npcInfoArray[BASIL].direction = 6;
                npcInfoArray[BASIL].unk_1E = 0;
                npcInfoArray[BASIL].startingCoordinates.x = 128.0f;
                npcInfoArray[BASIL].startingCoordinates.z = -416.0f;
                npcInfoArray[BASIL].flags |= 1;
            }
            break;
        case FRIDAY:
        case SATURDAY:
            if (7  < gHour && gHour < 18) {
                npcInfoArray[BASIL].levelIndex = MOUNTAIN_1;
                npcInfoArray[BASIL].startingCoordinates.y = 0;
                npcInfoArray[BASIL].direction = 4;
                npcInfoArray[BASIL].unk_1E = 1;
                npcInfoArray[BASIL].startingCoordinates.x = 192.0f;
                npcInfoArray[BASIL].startingCoordinates.z = -64.0f;
                npcInfoArray[BASIL].flags |= 1;
            }
            break;
        case MONDAY:
        case TUESDAY:
        case WEDNESDAY:
NOT_SUNNY:
            if (8 < gHour && gHour < 17) {
                npcInfoArray[BASIL].levelIndex = FLOWER_SHOP;
                npcInfoArray[BASIL].startingCoordinates.y = 0;
                npcInfoArray[BASIL].direction = 0;
                npcInfoArray[BASIL].unk_1E = 0;
                npcInfoArray[BASIL].startingCoordinates.x = -48.0f;
                npcInfoArray[BASIL].startingCoordinates.z = -64.0f;
                npcInfoArray[BASIL].flags |= 1;
            }
            break;
        default:
            break;
    }
        
    if ((17 < gHour && gHour < 24) && npcInfoArray[BASIL].location < 2) {
        npcInfoArray[BASIL].levelIndex = 0x3B;
        npcInfoArray[BASIL].startingCoordinates.y = 0;
        npcInfoArray[BASIL].direction = 6;
        npcInfoArray[BASIL].unk_1E = 0;
        npcInfoArray[BASIL].startingCoordinates.x = 32.0f;
        npcInfoArray[BASIL].startingCoordinates.z = 48.0f;
        npcInfoArray[BASIL].flags |= 1;
    }

FUNC_END:
    npcInfoArray[BASIL].movingFlag = npcInfoArray[BASIL].unk_1E;
    
}

//INCLUDE_ASM(const s32, "npc", setEllenLocation);

void setEllenLocation(void) {

    npcInfoArray[ELLEN].unk_1A = 0x40;
    npcInfoArray[ELLEN].unk_1B = 0x40;
    npcInfoArray[ELLEN].unk_24 = 0;
    npcInfoArray[ELLEN].unk_25 = 8;
    
    if (!checkLifeEventBit(ELLEN_DIED)) {
        if (gWeather == SUNNY && (7 < gHour && gHour < 17)) {
                npcInfoArray[ELLEN].levelIndex = VILLAGE_1;
                npcInfoArray[ELLEN].startingCoordinates.y = 0;
                npcInfoArray[ELLEN].direction = 0;
                npcInfoArray[ELLEN].unk_1E = 0;
                npcInfoArray[ELLEN].startingCoordinates.x = 320.0f;
                npcInfoArray[ELLEN].startingCoordinates.z = 128.0f;
                npcInfoArray[ELLEN].flags |= 1;
        } else {
            if (7 < gHour && gHour < 17) {
                npcInfoArray[ELLEN].levelIndex = BAKERY;
                npcInfoArray[ELLEN].startingCoordinates.y = 0;
                npcInfoArray[ELLEN].direction = 0;
                npcInfoArray[ELLEN].unk_1E = 0;
                npcInfoArray[ELLEN].startingCoordinates.x = -128.0f;
                npcInfoArray[ELLEN].startingCoordinates.z = -32.0f;
                npcInfoArray[ELLEN].flags |= 1;
            }
        }
    }
    
    npcInfoArray[ELLEN].movingFlag = npcInfoArray[ELLEN].unk_1E;
}

//INCLUDE_ASM(const s32, "npc", func_8007DF14);

void func_8007DF14(void) {

    int temp = gDayOfWeek;
    int temp2;

    npcInfoArray[DOUG].unk_1A = 0x40;
    npcInfoArray[DOUG].unk_1B = 0x40;
    npcInfoArray[DOUG].unk_24 = 0;
    npcInfoArray[DOUG].unk_25 = 8;

    // FIXME: something off here
    if (temp >= SUNDAY && (temp < THURSDAY || temp < 7 && (temp2 = temp) >= FRIDAY) && (7 < gHour && gHour < 17)) {
        npcInfoArray[DOUG].levelIndex = RANCH_STORE;
        npcInfoArray[DOUG].startingCoordinates.y = 0;
        npcInfoArray[DOUG].direction = 6;
        npcInfoArray[DOUG].unk_1E = 0;
        npcInfoArray[DOUG].startingCoordinates.x = -96.0f;
        npcInfoArray[DOUG].startingCoordinates.z = 64.0f;
        npcInfoArray[DOUG].flags |= 1;
    }

    if (NIGHTTIME && npcInfoArray[DOUG].location < 2) {
        npcInfoArray[DOUG].levelIndex = TAVERN;
        npcInfoArray[DOUG].startingCoordinates.y = 0;
        npcInfoArray[DOUG].direction = 2;
        npcInfoArray[DOUG].unk_1E = 0;
        npcInfoArray[DOUG].startingCoordinates.x = -96.0f;
        npcInfoArray[DOUG].startingCoordinates.z = 48.0f;
        npcInfoArray[DOUG].flags |= 1;
    }

    npcInfoArray[DOUG].movingFlag = npcInfoArray[DOUG].unk_1E;
    
}

INCLUDE_ASM(const s32, "npc", func_8007E07C);

INCLUDE_ASM(const s32, "npc", func_8007E2E4);

INCLUDE_ASM(const s32, "npc", func_8007E440);

INCLUDE_ASM(const s32, "npc", func_8007E600);

INCLUDE_ASM(const s32, "npc", func_8007ECE4);

INCLUDE_ASM(const s32, "npc", func_8007F378);

INCLUDE_ASM(const s32, "npc", func_8007F4E0);

// jtbl_801202B8
INCLUDE_ASM(const s32, "npc", func_8007F81C);

INCLUDE_ASM(const s32, "npc", func_8007FA44);

INCLUDE_ASM(const s32, "npc", func_8007FC0C);

INCLUDE_ASM(const s32, "npc", func_8007FCD8);

INCLUDE_ASM(const s32, "npc", func_8007FE40);

INCLUDE_ASM(const s32, "npc", func_8007FF04);

INCLUDE_ASM(const s32, "npc", func_800800D0);

INCLUDE_ASM(const s32, "npc", func_80080364);

INCLUDE_ASM(const s32, "npc", func_80080630);

INCLUDE_ASM(const s32, "npc", func_800807F8);

INCLUDE_ASM(const s32, "npc", func_800808B0);

INCLUDE_ASM(const s32, "npc", func_80080958);

INCLUDE_ASM(const s32, "npc", func_80080A10);

INCLUDE_ASM(const s32, "npc", func_80080B6C);

INCLUDE_ASM(const s32, "npc", func_80080C9C);

INCLUDE_ASM(const s32, "npc", func_80080DAC);

INCLUDE_ASM(const s32, "npc", func_80080EBC);

INCLUDE_ASM(const s32, "npc", func_80081030);

INCLUDE_ASM(const s32, "npc", func_80081228);

INCLUDE_ASM(const s32, "npc", func_80081420);

INCLUDE_ASM(const s32, "npc", func_80081618);

INCLUDE_ASM(const s32, "npc", func_80081810);

INCLUDE_ASM(const s32, "npc", func_80081A08);

static inline void updateAnimation(u8 index) {
    
    switch (npcInfoArray[index].movingFlag) {

        case 0:
            setStartMovingDefault(index);
            break;

        case 1:
            func_80075EBC(index);
            break;
            
    }
}

//INCLUDE_ASM(const s32, "npc", setNpcAnimations);

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

INCLUDE_ASM(const s32, "npc", func_80081C90);

// jtbl_801202D8
INCLUDE_ASM(const s32, "npc", func_80081E5C);

INCLUDE_ASM(const s32, "npc", func_80082164);

INCLUDE_ASM(const s32, "npc", func_80082330);

// jtbl_801202F0
INCLUDE_ASM(const s32, "npc", func_800824FC);

// jtbl_80120308
INCLUDE_ASM(const s32, "npc", func_8008277C);

INCLUDE_ASM(const s32, "npc", func_80082A0C);

INCLUDE_ASM(const s32, "npc", func_80082B04);

INCLUDE_ASM(const s32, "npc", func_80082C5C);

INCLUDE_ASM(const s32, "npc", func_80082DB4);

INCLUDE_ASM(const s32, "npc", func_80082EAC);

INCLUDE_ASM(const s32, "npc", func_80082FA4);

INCLUDE_ASM(const s32, "npc", func_8008309C);

INCLUDE_ASM(const s32, "npc", func_80083194);

INCLUDE_ASM(const s32, "npc", func_8008328C);

INCLUDE_ASM(const s32, "npc", func_80083384);

//INCLUDE_ASM(const s32, "npc", func_80083458);

void func_80083458(void) {

    if (npcInfoArray[DOUG].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[DOUG].spriteIndex) && !npcInfoArray[DOUG].unk_22) {
            updateAnimation(DOUG);
        }

        if (npcInfoArray[DOUG].unk_22) {
            npcInfoArray[DOUG].unk_22--; 
        } 
        
        func_80075A78(DOUG);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80083550);

void func_80083550(void) {

   u16 tempDirection;

    if (npcInfoArray[GOTZ].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[GOTZ].spriteIndex) && !npcInfoArray[GOTZ].unk_22) {

            switch (npcInfoArray[GOTZ].movingFlag) {

                case 0:
                    setStartMovingDefault(GOTZ);
                    break;

                case 1:
                    func_80075EBC(GOTZ);
                    break;

                case 3:

                    // FIXME: should be inline function
                    if (npcInfoArray[GOTZ].unk_21 == 0 ) {

                        npcInfoArray[GOTZ].unk_20 = 0;
                        npcInfoArray[GOTZ].unk_22 = 0;

                        setSpriteAnimation(npcInfoArray[GOTZ].spriteIndex, 0);

                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < 4) {
                            npcInfoArray[GOTZ].direction = tempDirection * 2;
                            npcInfoArray[GOTZ].unk_21 = 1;
                        }

                        npcInfoArray[GOTZ].flags |= 2;
                        
                    } else {       

                        npcInfoArray[GOTZ].unk_20 = 1;
                        npcInfoArray[GOTZ].unk_22 = 0;

                        setSpriteAnimation(npcInfoArray[GOTZ].spriteIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcInfoArray[GOTZ].unk_21 = 1;
                        } else {
                            npcInfoArray[GOTZ].unk_21 = 0;
                        }

                        npcInfoArray[GOTZ].flags |= 2;                
                    }

                    break;

                default:
                    break;
            }
        }
    
    if (npcInfoArray[GOTZ].unk_22) {
        npcInfoArray[GOTZ].unk_22--;
    }
    
    func_80075A78(GOTZ);

    }
}

//INCLUDE_ASM(const s32, "npc", func_8008374C);

void func_8008374C(void) {

   u16 tempDirection;

    if (npcInfoArray[SASHA].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[SASHA].spriteIndex) && !npcInfoArray[SASHA].unk_22) {

            switch (npcInfoArray[SASHA].movingFlag) {

                case 0:
                    setStartMovingDefault(SASHA);
                    break;

                case 1:
                    func_80075EBC(SASHA);
                    break;

                case 3:

                    // FIXME: should be inline function
                    if (npcInfoArray[SASHA].unk_21 == 0 ) {

                        npcInfoArray[SASHA].unk_20 = 0;
                        npcInfoArray[SASHA].unk_22 = 0;

                        setSpriteAnimation(npcInfoArray[SASHA].spriteIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < 4) {
                            npcInfoArray[SASHA].direction = tempDirection * 2;
                            npcInfoArray[SASHA].unk_21 = 1;
                        }

                        npcInfoArray[SASHA].flags |= 2;
                        
                    } else {       

                        npcInfoArray[SASHA].unk_20 = 1;
                        npcInfoArray[SASHA].unk_22 = 0;

                        setSpriteAnimation(npcInfoArray[SASHA].spriteIndex, 8);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcInfoArray[SASHA].unk_21 = 1;
                        } else {
                            npcInfoArray[SASHA].unk_21 = 0;
                        }

                        npcInfoArray[SASHA].flags |= 2;                
                    }

                    break;

                default:
                    break;
            }
        }
    
    if (npcInfoArray[SASHA].unk_22) {
        npcInfoArray[SASHA].unk_22--;
    }
    
    func_80075A78(SASHA);

    }
}

void func_80083948(void) {

    if (npcInfoArray[POTION_SHOP_DEALER].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[POTION_SHOP_DEALER].spriteIndex) && !npcInfoArray[POTION_SHOP_DEALER].unk_22) {
            updateAnimation(POTION_SHOP_DEALER);
        }

        if (npcInfoArray[POTION_SHOP_DEALER].unk_22) {
            npcInfoArray[POTION_SHOP_DEALER].unk_22--; 
        } 
        
        func_80075A78(POTION_SHOP_DEALER);
    }   
}


//INCLUDE_ASM(const s32, "npc", func_80083A40);

void func_80083A40(void) {

    u16 tempDirection;

    if (npcInfoArray[KENT].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[KENT].spriteIndex) && !npcInfoArray[KENT].unk_22) {

            switch (npcInfoArray[KENT].movingFlag) {

                case 0:
                    setStartMovingDefault(KENT);
                    break;
                
                case 1:
                    func_80075EBC(KENT);
                    break;

                case 2:

                    // FIXME: should be inline function
                    if (npcInfoArray[KENT].unk_21 == 0) {
                        
                        npcInfoArray[KENT].unk_20 = 0;
                        npcInfoArray[KENT].unk_22 = 0;
                        setSpriteAnimation(npcInfoArray[KENT].spriteIndex, 0);
                        tempDirection = getRandomNumberInRange(0, 60);

                        if (tempDirection < 4) {
                            npcInfoArray[KENT].direction = tempDirection * 2 ;
                            npcInfoArray[KENT].unk_21 = 1;
                        } 

                        npcInfoArray[KENT].flags |= 2;  

                    } else {

                        npcInfoArray[KENT].unk_20 = 2;
                        npcInfoArray[KENT].unk_22 = 0;

                        setSpriteAnimation(npcInfoArray[KENT].spriteIndex, 0x10);

                        if (getRandomNumberInRange(0, 19) < 8) {
                            npcInfoArray[KENT].unk_21 = 1;
                        } else {
                            npcInfoArray[KENT].unk_21 = 0;
                        }

                        npcInfoArray[KENT].flags |= 2;
                    }

                    break;

                default:
                    break;
            }
       }    

    if (npcInfoArray[KENT].unk_22) {
        npcInfoArray[KENT].unk_22--;
    }
    
    func_80075A78(KENT);
    
    }
}

//INCLUDE_ASM(const s32, "npc", func_80083C3C);

void func_80083C3C(void) {

    if (npcInfoArray[STU].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[STU].spriteIndex) && !npcInfoArray[STU].unk_22) {
            updateAnimation(STU);
        }

        if (npcInfoArray[STU].unk_22) {
            npcInfoArray[STU].unk_22--; 
        } 
        
        func_80075A78(STU);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80083D34);

void func_80083D34(void) {

    if (npcInfoArray[MIDWIFE].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[MIDWIFE].spriteIndex) && !npcInfoArray[MIDWIFE].unk_22) {
            updateAnimation(MIDWIFE);
        }

        if (npcInfoArray[MIDWIFE].unk_22) {
            npcInfoArray[MIDWIFE].unk_22--; 
        } 
        
        func_80075A78(MIDWIFE);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80083E2C);

void func_80083E2C(void) {

    if (npcInfoArray[MAY].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[MAY].spriteIndex) && !npcInfoArray[MAY].unk_22) {
            updateAnimation(MAY);
        }

        if (npcInfoArray[MAY].unk_22) {
            npcInfoArray[MAY].unk_22--; 
        } 
        
        func_80075A78(MAY);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80083F24);

void func_80083F24(void) {

    if (npcInfoArray[RICK].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[RICK].spriteIndex) && !npcInfoArray[RICK].unk_22) {
            updateAnimation(RICK);
        }

        if (npcInfoArray[RICK].unk_22) {
            npcInfoArray[RICK].unk_22--; 
        } 
        
        func_80075A78(RICK);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_8008401C);

void func_8008401C(void) {

    if (npcInfoArray[PASTOR].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[PASTOR].spriteIndex) && !npcInfoArray[PASTOR].unk_22) {
            updateAnimation(PASTOR);
        }

        if (npcInfoArray[PASTOR].unk_22) {
            npcInfoArray[PASTOR].unk_22--; 
        } 
        
        func_80075A78(PASTOR);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80084114);

void func_80084114(void) {

    if (npcInfoArray[SHIPPER].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[SHIPPER].spriteIndex) && !npcInfoArray[SHIPPER].unk_22) {
            updateAnimation(SHIPPER);
        }

        if (npcInfoArray[SHIPPER].unk_22) {
            npcInfoArray[SHIPPER].unk_22--; 
        } 
        
        func_80075A78(SHIPPER);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_8008420C);

void func_8008420C(void) {

    if (npcInfoArray[SAIBARA].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[SAIBARA].spriteIndex) && !npcInfoArray[SAIBARA].unk_22) {
            updateAnimation(SAIBARA);
        }

        if (npcInfoArray[SAIBARA].unk_22) {
            npcInfoArray[SAIBARA].unk_22--; 
        } 
        
        func_80075A78(SAIBARA);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80084304);
void func_80084304(void) {

    if (npcInfoArray[DUKE].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[DUKE].spriteIndex) && !npcInfoArray[DUKE].unk_22) {
            updateAnimation(DUKE);
        }

        if (npcInfoArray[DUKE].unk_22) {
            npcInfoArray[DUKE].unk_22--; 
        } 
        
        func_80075A78(DUKE);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_800843FC);

void func_800843FC(void) {

    if (npcInfoArray[GREG].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[GREG].spriteIndex) && !npcInfoArray[GREG].unk_22) {
            updateAnimation(GREG);
        }

        if (npcInfoArray[GREG].unk_22) {
            npcInfoArray[GREG].unk_22--; 
        } 
        
        func_80075A78(GREG);
    }   
}

INCLUDE_ASM(const s32, "npc", func_800844F4);

INCLUDE_ASM(const s32, "npc", func_80084770);

INCLUDE_ASM(const s32, "npc", func_800849EC);

INCLUDE_ASM(const s32, "npc", func_80084B44);

INCLUDE_ASM(const s32, "npc", func_80084C3C);

INCLUDE_ASM(const s32, "npc", func_80084D34);

//INCLUDE_ASM(const s32, "npc", func_80084E2C);

void func_80084E2C(void) {

    if (npcInfoArray[SYDNEY].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[SYDNEY].spriteIndex) && !npcInfoArray[SYDNEY].unk_22) {
            updateAnimation(SYDNEY);
        }

        if (npcInfoArray[SYDNEY].unk_22) {
            npcInfoArray[SYDNEY].unk_22--; 
        } 
        
        func_80075A78(SYDNEY);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80084F24);

void func_80084F24(void) {

    if (npcInfoArray[BARLEY].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[BARLEY].spriteIndex) && !npcInfoArray[BARLEY].unk_22) {
            updateAnimation(BARLEY);
        }

        if (npcInfoArray[BARLEY].unk_22) {
            npcInfoArray[BARLEY].unk_22--; 
        } 
        
        func_80075A78(BARLEY);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_8008501C);

void func_8008501C(void) {

    if (npcInfoArray[GOURMET_JUDGE].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[GOURMET_JUDGE].spriteIndex) && !npcInfoArray[GOURMET_JUDGE].unk_22) {
            updateAnimation(GOURMET_JUDGE);
        }

        if (npcInfoArray[GOURMET_JUDGE].unk_22) {
            npcInfoArray[GOURMET_JUDGE].unk_22--; 
        } 
        
        func_80075A78(GOURMET_JUDGE);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_80085114);

void func_80085114(void) {

    if (npcInfoArray[PHOTOGRAPHER].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[PHOTOGRAPHER].spriteIndex) && !npcInfoArray[PHOTOGRAPHER].unk_22) {
            updateAnimation(PHOTOGRAPHER);
        }

        if (npcInfoArray[PHOTOGRAPHER].unk_22) {
            npcInfoArray[PHOTOGRAPHER].unk_22--; 
        } 
        
        func_80075A78(PHOTOGRAPHER);
    }   
}

//INCLUDE_ASM(const s32, "npc", func_8008520C);

void func_8008520C(void) {

    if (npcInfoArray[0x28].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[0x28].spriteIndex) && !npcInfoArray[0x28].unk_22) {
            updateAnimation(0x28);
        }

        if (npcInfoArray[0x28].unk_22) {
            npcInfoArray[0x28].unk_22--; 
        } 
        
        func_80075A78(0x28);
    }   
}

INCLUDE_ASM(const s32, "npc", func_80085304);

INCLUDE_ASM(const s32, "npc", func_800853FC);

INCLUDE_ASM(const s32, "npc", func_800854F4);

INCLUDE_ASM(const s32, "npc", func_800855EC);

INCLUDE_ASM(const s32, "npc", func_800856E4);

INCLUDE_ASM(const s32, "npc", func_800857DC);

INCLUDE_ASM(const s32, "npc", func_800858D4);

//INCLUDE_ASM(const s32, "npc", func_80085C94);

bool func_80085C94(void) {

    bool found = 0;
    u8 i = 0;
    
    D_801C3E18 = 0xFF;

    while (i < TOTAL_NPCS && !found) {

        if (npcInfoArray[i].flags & 4) {
            if (!renderedSprites[npcInfoArray[i].spriteIndex].unk_58) {
                found = 1;
                npcInfoArray[i].movingFlag = 0x20;
                D_801C3E18 = i;
            }
        }  

        i++;

    } 
    
    return found;
}

//INCLUDE_ASM(const s32, "npc", func_80085D48);

bool func_80085D48(int index, u16 arg1) {

    bool result;
    
    int temp;
    // FIXME: likely a struct?
    u8 arr[8];
    
    arr[7] = index;
    index = temp;
    index = arr[7];
    
    result = 0;
    
    if (npcInfoArray[index].flags & 4) {

        // FIXME: 
        // check if girl and load heart icon
        if ((index < 5) && (index >= (result = 0))) {
            func_8003F910(0, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsIndexSegmentRomStart, &_dialogueIconsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURES_VADDR, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
            func_8003F910(1, 0x78, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsIndexSegmentRomStart, &_dialogueIconsIndexSegmentRomEnd, (void*)DIALOGUE_ICONS_TEXTURES_VADDR, (void*)0x8023CC00, (void*)0x8023CE00, (void*)0x8023D200, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
        }
        // get dialogue index

        showDialogueBox(0, D_80114960[arr[7]], arg1, 0, 0);
        result = 1;
    }
    
  return result;
}