#include "common.h"

#include "npc.h"

#include "system/mathUtils.h"
#include "system/message.h"
#include "system/sprite.h"
#include "system/tiles.h"

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

// shared global: animals, npc, cutscene, renderedSprite 
extern u16 D_801FC1B8[30][54];

// shared: overlayScreens, npc, title
extern void *D_D47F00;
// naming screen
extern void *D_D49B80;
extern void *D_D49B80_2;
extern void *tvSprites_romTextureStart;

// data
extern u16 D_80114960[30];

// forward declarations
void func_80075A78(u8);


INCLUDE_ASM(const s32, "npc", func_800752C0);

//INCLUDE_RODATA(const s32, "npc", jtbl_80120010);

// jtbl_80120010
//INCLUDE_ASM(const s32, "npc", func_80075374);

u8 func_80075374(u8 npcIndex, int arg1) {

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
    
        func_8002F684(npcInfoArray[npcIndex].spriteIndex, (npcInfoArray[npcIndex].direction + 8 - func_8003C1A4(0)) % 8);
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

//INCLUDE_ASM(const s32, "npc", func_80075E28);

void func_80075E28(u8 npcIndex) {

    npcInfoArray[npcIndex].unk_20 = 0;
    npcInfoArray[npcIndex].unk_21 = 0xA;
    npcInfoArray[npcIndex].unk_22 = 0;

    setSpriteAnimation(npcInfoArray[npcIndex].spriteIndex, npcInfoArray[npcIndex].unk_24);

    npcInfoArray[npcIndex].flags |= 2;
    
}

//INCLUDE_ASM(const s32, "npc", func_80075EBC);

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
            goto func_end;
        }
        
        npcInfoArray[index].unk_21 = 0;
    }
    
func_end:
    npcInfoArray[index].flags |= 2;
}

//INCLUDE_ASM(const s32, "npc", func_80076080);

void func_80076080(u8 index, u8 arg1) {

    npcInfoArray[index].unk_20 = 0;
    npcInfoArray[index].unk_21 = 10;
    npcInfoArray[index].unk_22 = 0;

    setSpriteAnimation(npcInfoArray[index].spriteIndex, arg1);

    npcInfoArray[index].flags |= 2;
}

//INCLUDE_ASM(const s32, "npc", func_80076108);

void func_80076108(u8 index, u8 arg1, u8 arg2) {

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
            goto func_end;
        }
        
        npcInfoArray[index].unk_21 = 0;
    }
    
func_end:
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

INCLUDE_ASM(const s32, "npc", func_80076CF4);

// jtbl_80120188
INCLUDE_ASM(const s32, "npc", func_80076DCC);

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

INCLUDE_ASM(const s32, "npc", func_8007D9F4);

// jtbl_80120298
INCLUDE_ASM(const s32, "npc", func_8007DB38);

//INCLUDE_ASM(const s32, "npc", setEllenLocation);

void setEllenLocation(void) {

    npcInfoArray[ELLEN].unk_1A = 0x40;
    npcInfoArray[ELLEN].unk_1B = 0x40;
    npcInfoArray[ELLEN].unk_24 = 0;
    npcInfoArray[ELLEN].unk_25 = 8;
    
    if (!checkLifeEventBit(ELLEN_DIED)) {
        if (gWeather == SUNNY && (gHour - 8) < 9U) {
                npcInfoArray[ELLEN].levelIndex = VILLAGE_1;
                npcInfoArray[ELLEN].startingCoordinates.y = 0;
                npcInfoArray[ELLEN].direction = 0;
                npcInfoArray[ELLEN].unk_1E = 0;
                npcInfoArray[ELLEN].startingCoordinates.x = 320.0f;
                npcInfoArray[ELLEN].startingCoordinates.z = 128.0f;
                npcInfoArray[ELLEN].flags |= 1;
        } else {
            if ((gHour - 8) < 9U) {
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

INCLUDE_ASM(const s32, "npc", func_8007DF14);

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

INCLUDE_ASM(const s32, "npc", func_80083458);

INCLUDE_ASM(const s32, "npc", func_80083550);

INCLUDE_ASM(const s32, "npc", func_8008374C);

INCLUDE_ASM(const s32, "npc", func_80083948);

INCLUDE_ASM(const s32, "npc", func_80083A40);

INCLUDE_ASM(const s32, "npc", func_80083C3C);

INCLUDE_ASM(const s32, "npc", func_80083D34);

INCLUDE_ASM(const s32, "npc", func_80083E2C);

INCLUDE_ASM(const s32, "npc", func_80083F24);

INCLUDE_ASM(const s32, "npc", func_8008401C);

INCLUDE_ASM(const s32, "npc", func_80084114);

//INCLUDE_ASM(const s32, "npc", func_8008420C);

void func_8008420C(void) {

    if (!(npcInfoArray[SAIBARA].flags & 4)) return;

    if (!func_8002FECC(npcInfoArray[SAIBARA].spriteIndex)) goto check; 
    
    if (npcInfoArray[SAIBARA].unk_22) goto decrement;

    switch (npcInfoArray[SAIBARA].movingFlag) {
        case FALSE:
            npcInfoArray[SAIBARA].unk_20 = 0;
            npcInfoArray[SAIBARA].unk_21 = 10;
            npcInfoArray[SAIBARA].unk_22 = 0;
            setSpriteAnimation(npcInfoArray[SAIBARA].spriteIndex, npcInfoArray[SAIBARA].unk_24);
            // can't use struct member for some reason
            D_801FBE2E |= 2;
            break;
        case TRUE:
            func_80075EBC(SAIBARA);
            break;
        default:
            break;
    }
    
check:
     if (!npcInfoArray[SAIBARA].unk_22) {
         goto func_end;
    }
    
decrement:
    npcInfoArray[SAIBARA].unk_22--;
    
func_end:    
    func_80075A78(SAIBARA);
}

INCLUDE_ASM(const s32, "npc", func_80084304);

INCLUDE_ASM(const s32, "npc", func_800843FC);

INCLUDE_ASM(const s32, "npc", func_800844F4);

INCLUDE_ASM(const s32, "npc", func_80084770);

INCLUDE_ASM(const s32, "npc", func_800849EC);

INCLUDE_ASM(const s32, "npc", func_80084B44);

INCLUDE_ASM(const s32, "npc", func_80084C3C);

INCLUDE_ASM(const s32, "npc", func_80084D34);

INCLUDE_ASM(const s32, "npc", func_80084E2C);

//INCLUDE_ASM(const s32, "npc", func_80084F24);

void func_80084F24(void) {

    if (!(npcInfoArray[37].flags & 4)) return;

    if (!func_8002FECC(npcInfoArray[37].spriteIndex)) goto check; 
    
    if (npcInfoArray[37].unk_22) goto decrement;

    switch (npcInfoArray[37].movingFlag) {
        case FALSE:
            npcInfoArray[37].unk_20 = 0;
            npcInfoArray[37].unk_21 = 10;
            npcInfoArray[37].unk_22 = 0;
            setSpriteAnimation(npcInfoArray[37].spriteIndex, npcInfoArray[37].unk_24);
            // can't use struct member for some reason
            D_801FBFBE |= 2;
            break;
        case TRUE:
            func_80075EBC(0x25);
            break;
        default:
            break;
    }
    
check:
     if (!npcInfoArray[37].unk_22) {
         goto func_end;
    }
    
decrement:
    npcInfoArray[37].unk_22--;
    
func_end:    
    func_80075A78(0x25);
}

//INCLUDE_ASM(const s32, "npc", func_8008501C);

void func_8008501C(void) {

    if (npcInfoArray[38].flags & 4) {
        
        if (func_8002FECC(npcInfoArray[38].spriteIndex)) {

            if (!npcInfoArray[38].unk_22) {
                
                switch (npcInfoArray[38].movingFlag) {
                    case 0:
                        npcInfoArray[38].unk_20 = 0;
                        npcInfoArray[38].unk_21 = 0xA;
                        npcInfoArray[38].unk_22 = 0;
                        setSpriteAnimation(npcInfoArray[38].spriteIndex, npcInfoArray[38].unk_24);
                        D_801FBFE6 |= 2;
                        break;
                    case 1:
                        func_80075EBC(0x26);
                        break;
                }

            }

        }

        if (npcInfoArray[38].unk_22) {
            npcInfoArray[38].unk_22--;
        }

        func_80075A78(0x26);
    }
}

INCLUDE_ASM(const s32, "npc", func_80085114);

INCLUDE_ASM(const s32, "npc", func_8008520C);

INCLUDE_ASM(const s32, "npc", func_80085304);

INCLUDE_ASM(const s32, "npc", func_800853FC);

INCLUDE_ASM(const s32, "npc", func_800854F4);

INCLUDE_ASM(const s32, "npc", func_800855EC);

INCLUDE_ASM(const s32, "npc", func_800856E4);

INCLUDE_ASM(const s32, "npc", func_800857DC);

INCLUDE_ASM(const s32, "npc", func_800858D4);

//INCLUDE_ASM(const s32, "npc", func_80085C94);

u8 func_80085C94(void) {

    u8 found = 0;
    u8 i = 0;
    
    D_801C3E18 = 0xFF;

    while (i < TOTAL_NPCS && !found) {
        if (npcInfoArray[i].flags & 4) {
            if (!renderedSprites[npcInfoArray[i].spriteIndex].unk_58) {
                found = 1;
                // bit 6
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
    // something very wrong here... maybe a struct?
    u8 arr[8];
    
    arr[7] = index;
    index = temp;
    index = arr[7];
    
    result = 0;
    
    if (npcInfoArray[index].flags & 4) {
        // check if girl and load heart icon
        if ((index < 5) && (index >= (result = 0))) {
            func_8003F910(0, 0x78, &D_D47F00, &D_D49B80, &D_D49B80_2, &tvSprites_romTextureStart, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
            func_8003F910(1, 0x78, &D_D47F00, &D_D49B80, &D_D49B80_2, &tvSprites_romTextureStart, 0x8023B400, 0x8023CC00, 0x8023CE00, 0x8023D200, 0, (npcAffection[index] / 52) + 5, 0xFE, 106.0f, -15.0f, 0);
        }
        // get dialogue index
        func_8005AF94(0, D_80114960[arr[7]], arg1, 0, 0);
        result = 1;
    }
    
  return result;
}