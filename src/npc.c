#include "common.h"
#include "npc.h"
#include "system/sprite.h"

u16 getRandomNumberInRange(u16 min, u16 max);                           
u32 checkDailyEventBit(u16);                          

// bss
extern npcInfo npcInfoArray[0x30];
extern RenderedSprite renderedSprites[0x31];

extern u8 D_801C3E18;
extern u16 D_801FC1B8[30][54];

extern u8 gBaseMapIndex;

extern u8 gHour;
extern u8 gWeather;

u8 func_8003F910(u8, u16, void*, void*, void*, void*, void*, void*, void*, u32, u32, u16, u16, f32, f32, f32); 
u8 func_8005AF94(u8, u16, u16, u16, u8);               
u8 setSpriteAnimation(u16, u16);                           
u8 func_8002FECC(u16);                             
void func_80075A78(u8);                                 
void func_80075EBC(u8);

extern u16 D_80114960[30];
extern void *D_D47F00;
extern void *D_D49B80;
extern void *D_D49B80_2;
extern u8 npcAffection[0x30];
extern void *tvSprites_romTextureStart;

extern u16 D_801FBFBE;
extern u16 D_801FBE2E;

void func_80076F10();                                  
void func_80077D14();                                  
void func_80078BF0();                                  
void func_80079A74();                                  
void func_8007A9B0();                                  
void func_8007B828();                                  
void func_8007BD50();                                  
void func_8007C088();                                  
void func_8007C3D4();                                  
void func_8007C7C4();                                  
void func_8007CEB4();                                  
void func_8007D3C4();                                  
void func_8007D6E0();                                  
void func_8007D9F4();                                  
void func_8007DB38();                                  
void setEllenLocation();                                  
void func_8007DF14();                                  
void func_8007E07C();                                  
void func_8007E2E4();                                  
void func_8007E440();                                  
void func_8007E600();                                  
void func_8007ECE4();                                  
void func_8007F378();                                  
void func_8007F4E0();                                  
void func_8007F81C();                                  
void func_8007FA44();                                  
void func_8007FC0C();                                  
void func_8007FCD8();                                  
void func_8007FE40();                                  
void func_8007FF04();                                  
void func_800800D0();                                  
void func_80080364();                                  
void func_80080630();                                  
void func_800807F8();                                  
void func_800808B0();                                  
void func_80080958();                                  
void func_80080A10();                                  
void func_80080B6C();                                  
void func_80080C9C();                                  
void func_80080DAC();                                  
void func_80080EBC();                                  
void func_80081030();                                  
void func_80081228();                                  
void func_80081420();                                  
void func_80081618();                                  
void func_80081810();                                  
void func_80081A08();                  

void func_80081C90();                                  
void func_80081E5C();                                  
void func_80082164();                                  
void func_80082330();                                  
void func_800824FC();                                  
void func_8008277C();                                  
void func_80082A0C();                                  
void func_80082B04();                                  
void func_80082C5C();                                  
void func_80082DB4();                                  
void func_80082EAC();                                  
void func_80082FA4();                                  
void func_8008309C();                                  
void func_80083194();                                  
void func_8008328C();                                  
void func_80083384();                                  
void func_80083458();                                  
void func_80083550();                                  
void func_8008374C();                                  
void func_80083948();                                  
void func_80083A40();                                  
void func_80083C3C();                                  
void func_80083D34();                                  
void func_80083E2C();                                  
void func_80083F24();                                  
void func_8008401C();                                  
void func_80084114();                                  
void func_8008420C();                                  
void func_80084304();                                  
void func_800843FC();                                  
void func_800844F4();                                  
void func_80084770();                                  
void func_800849EC();                                  
void func_80084B44();                                  
void func_80084C3C();                                  
void func_80084D34();                                  
void func_80084E2C();                                  
void func_80084F24();                                  
void func_8008501C();                                  
void func_80085114();                                  
void func_8008520C();                                  
void func_80085304();                                  
void func_800853FC();                                  
void func_800854F4();                                  
void func_800855EC();                                  
void func_800856E4();                                  
void func_800857DC();      

u8 func_80075374(u8, u8);                        

void setNpcAnimations();                         
void setNPCSpawningLocations();       


INCLUDE_ASM(const s32, "npc", func_800752C0);

#ifdef PERMUTER
u8 func_80075374(u8 arg0, u8 arg2, u8 arg1) {

    s32 tempDirection;
    s32 adjustedDirection;
    
    if ((D_801FB9D0[arg0].flags & 1) && D_801FB9D0[arg0].levelIndex == gBaseMapIndex) {
        
        D_801FB9D0[arg0].flags |= 4;

        switch (arg0) {
            case 0:
                if (checkLifeEventBit(0)) {
                    if (gWife != 0) {
                        D_801FB9D0[arg0].spriteIndex = 0x23; 
                        break;
                    } 
                } 
                goto default_set;
            case 1:
                if (checkLifeEventBit(0)) {
                    if (gWife != 1) {
                        D_801FB9D0[arg0].spriteIndex = 0x23;
                        break;
                    } 
                }
                goto default_set;
            case 2:
                if (checkLifeEventBit(0)) {
                    if (gWife != 2) {
                        D_801FB9D0[arg0].spriteIndex = 0x23;
                        break;
                    } 
                }
                goto default_set;
            case 3:
                if (checkLifeEventBit(0)) {
                    if (gWife != 3) {
                        D_801FB9D0[arg0].spriteIndex = 0x23;
                        break;
                    } 
                }
                goto default_set;
            case 4:
                if (checkLifeEventBit(0)) {
                    if (gWife == 4) {
                        D_801FB9D0[arg0].spriteIndex = 0x23;
                        break;
                    } 
                }
                goto default_set;
            case 5:                               
                D_801FB9D0[arg0].spriteIndex = 0x24;
                break;
            case 39:
                D_801FB9D0[arg0].spriteIndex = 0x1D;
                break;
            case 6:                                 
            case 7:                                 
            case 8:                                 
            case 9:                                 
            case 10:                                
            case 11:                                
            case 12:                                
            case 13:                                
            case 14:                                
            case 15:                                
            case 16:                                
            case 17:                                
            case 18:                                
            case 19:                                
            case 20:                                
            case 21:                                
            case 22:                                
            case 23:                                
            case 24:                                
            case 25:                                
            case 26:                                
            case 27:                                
            case 28:                                
            case 29:                                
            case 30:                                
            case 31:                                
            case 32:                                
            case 33:                                
            case 34:                                
            case 35:                                
            case 36:                                
            case 37:                                
            case 38:
            default:
default_set:
                D_801FB9D0[arg0].spriteIndex = arg1 + 0x15;
                arg1++;
                break;
        }
    
        D_801FB9D0[arg0].currentCoordinates.x = D_801FB9D0[arg0].startingCoordinates.x;
        D_801FB9D0[arg0].currentCoordinates.y = D_801FB9D0[arg0].startingCoordinates.y;
        D_801FB9D0[arg0].currentCoordinates.z = D_801FB9D0[arg0].startingCoordinates.z;
    
        func_8002E284(D_801FB9D0[arg0].spriteIndex, D_80114900[arg0], 1);
        func_8003019C(D_801FB9D0[arg0].spriteIndex, 1);
        func_80030054(D_801FB9D0[arg0].spriteIndex, 1);
        func_8002FF38(D_801FB9D0[arg0].spriteIndex, 0);
    
        tempDirection = D_801FB9D0[arg0].direction + 8 - func_8003C1A4(0);
    
        adjustedDirection = tempDirection;
    
        if (tempDirection < 0) {
            adjustedDirection = tempDirection + 7;
        }
        
        func_8002F684(D_801FB9D0[arg0].spriteIndex, (tempDirection - (adjustedDirection >> 3)*8));
        func_8002FD80(D_801FB9D0[arg0].spriteIndex, D_801FB9D0[arg0].currentCoordinates.x, D_801FB9D0[arg0].currentCoordinates.y, D_801FB9D0[arg0].currentCoordinates.z);
    
        if (D_801FB9D0[arg0].flags & 8) {
            switch (arg0) {                   
                case 5:                             
                    func_8002ECD4(D_801FB9D0[arg0].spriteIndex, 0x23, 0xFE);
                    break;
                case 41:                            
                    func_8002ECD4(D_801FB9D0[arg0].spriteIndex, D_801FB9D0[0].spriteIndex, 0xFE);
                    break;
                case 42:                            
                    func_8002ECD4(D_801FB9D0[arg0].spriteIndex, D_801FB9D0[1].spriteIndex, 0xFE);
                    break;
                case 43:                            
                    func_8002ECD4(D_801FB9D0[arg0].spriteIndex, D_801FB9D0[2].spriteIndex, 0xFE);
                    break;
                case 44:                            
                    func_8002ECD4(D_801FB9D0[arg0].spriteIndex, D_801FB9D0[3].spriteIndex, 0xFE);
                    break;
                case 45:                            
                    func_8002ECD4(D_801FB9D0[arg0].spriteIndex, D_801FB9D0[4].spriteIndex, 0xFE);
                    break;
                }
            
                func_8002EDF0(D_801FB9D0[arg0].spriteIndex, 0, 0xE, 0x14);
                func_8003019C(D_801FB9D0[arg0].spriteIndex, 0);
                func_80030054(D_801FB9D0[arg0].spriteIndex, 0);
                func_80030240(D_801FB9D0[arg0].spriteIndex, 0);
                func_800302E4(D_801FB9D0[arg0].spriteIndex, 0);
        }
    
        D_801FB9D0[arg0].unk_21 = 0;
        D_801FB9D0[arg0].unk_22 = 0;
        D_801FB9D0[arg0].flags |= 4;
    }

    return arg1;
}
#else
// jtbl_80120010
INCLUDE_ASM(const s32, "npc", func_80075374);
#endif

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

//INCLUDE_ASM(const s32, "npc", func_80075E28);//

void func_80075E28(u8 index) {

    npcInfoArray[index].unk_20 = 0;
    npcInfoArray[index].unk_21 = 0xA;
    npcInfoArray[index].unk_22 = 0;

    setSpriteAnimation(npcInfoArray[index].spriteIndex, npcInfoArray[index].unk_24);

    npcInfoArray[index].flags |= 2;
    
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
        if (gWeather == 1 && (gHour - 8) < 9U) {
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

INCLUDE_ASM(const s32, "npc", func_8008501C);

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

u8 func_80085D48(int index, u16 arg1) {

    u8 result;
    
    int temp;
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
