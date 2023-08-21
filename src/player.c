#include "common.h"

extern Player gPlayer;

extern u8 keyItemSlots[];

// player starting coordinates
extern Vec3f D_80113C50[];
// player starting directions
extern u8 D_801141A8[];
extern Vec3f D_8016F8A4;
extern Vec4f globalLightingRgba;

// tool use info struct?
extern UnknownStruct3 D_80189828;

extern u8 D_80189836;
extern u16 D_80237410;

void setAudio(u16);   
void setSpriteAnimation(u8, u32);    

u8 func_8002E284(u16, u16, u32);                           
u8 func_8002F014(u16, u8, u8, u8, u8);             
u8 func_8002F114(u16, u8);                              
u8 func_8002F684(u16, u8);                    
u32 func_8002FD80(u8, f32, f32, f32);   
u32 func_8002FF38(u8, u16);                           
u32 func_80030054(u8, u8);
u32 func_8003019C(u16, u8);                              
u8 func_80031380(s32);                               
u32 func_80034DC8(u8, u8, u8);   
u8 func_8003C1A4(u16);            
u32 func_8004D35C(u16, s16);      
u8 func_80067A24(u8);                               
void func_80099DE8();                                  
void func_800DC7BC(u8);                            
void func_800CF850();                                  
void func_800D55E4(u8, u8);         
void func_800DC9C0(u8);
u8 func_800DCAA0(u8); 


//INCLUDE_ASM(const s32, "player", func_800657D0);

void func_800657D0(u16 arg0, u8 arg1) {

    s32 temp;
    s32 temp2;
    
    func_8002E284(0, 0, 1);
    func_8003019C(0, 1);
    func_80030054(0, 1);
    func_8002FF38(0, 1);
    func_8002FCB4(0, 1);
    func_8002F014(0, globalLightingRgba.r, globalLightingRgba.g, globalLightingRgba.b, globalLightingRgba.a);
    func_8002F114(0, 0);

    if (arg1) {
        gPlayer.startingCoordinates.x = D_80113C50[arg0].x;
        gPlayer.startingCoordinates.y = D_80113C50[arg0].y;
        gPlayer.startingCoordinates.z = D_80113C50[arg0].z;
        gPlayer.direction = D_801141A8[arg0];
    }

    temp = gPlayer.direction + 8 - (func_8003C1A4(0));
    temp2 = temp;
    
    if (temp < 0) {
        temp2 = temp + 7;
    }

    func_8002F684(0, (temp - (temp2 >> 3) * 8));
    func_8002FD80(0, gPlayer.startingCoordinates.x, gPlayer.startingCoordinates.y, gPlayer.startingCoordinates.z);

    D_80189828.unk_3 = 0;
    D_80189828.unk_4 = 0;
    D_80189828.unk_6 = 0;
    D_80189828.unk_8 = 0;
    D_80189828.unk_A = 0;
    D_80189828.unk_C = 0;

}

INCLUDE_ASM(const s32, "player", func_80065AA0);

INCLUDE_ASM(const s32, "player", func_80065B74);

INCLUDE_ASM(const s32, "player", func_80065BCC);

INCLUDE_ASM(const s32, "player", func_80065D18);

INCLUDE_ASM(const s32, "player", acquireKeyItem);

INCLUDE_ASM(const s32, "player", removeKeyItem);

INCLUDE_ASM(const s32, "player", func_80065E80);

//INCLUDE_ASM(const s32, "player", checkHaveKeyItem);

u8 checkHaveKeyItem(u8 item) {

    u8 i;
    u8 found = 0;
    
    for (i = 0; i < MAX_KEY_ITEMS && !found; i++) {

        if (keyItemSlots[i] == item) {
            found = 1;
        }
    }

    return found;
}

INCLUDE_ASM(const s32, "player", func_80065F5C);

// D_8011F3F0
// D_8011F3FC
INCLUDE_ASM(const s32, "player", func_80065F94);

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

const u8 D_8011F3F0[12] = { 0, 0xFF, 0xFF, 0xFF, 0, 1, 1, 1, 0, 0, 0, 0 };

//INCLUDE_RODATA(const s32, "player", D_8011F3FC);

const u8 D_8011F3FC[12] = { 1, 1, 0, 0xFF, 0xFF, 0xFF, 0, 1, 0, 0, 0, 0 };

// jtbl_8011F408
//INCLUDE_ASM(const s32, "player", func_8006623C);

void func_8006623C(void) {
    
    u32 temp;

    temp = gPlayer.action1 - 1;
    
    if (gPlayer.action1 == 0) {
        func_800664C8();
        temp = gPlayer.action1 - 1;
    }
    
    switch (temp) {
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
// D_801FC1C7
INCLUDE_ASM(const s32, "player", func_800664C8);

INCLUDE_ASM(const s32, "player", func_80066F98);

INCLUDE_ASM(const s32, "player", func_80067034);

INCLUDE_ASM(const s32, "player", func_80067290);

INCLUDE_ASM(const s32, "player", func_80067950);

//INCLUDE_ASM(const s32, "player", checkFatigueLevel);

u8 checkFatigueLevel(void) {

    u8 temp;
    
    if (gPlayer.fatigue[0] == 100) {
        temp = 3;
    } else if (gPlayer.fatigue[0] >= 75) {
        return 2;
    }  else {
        return (gPlayer.fatigue[0] < 50) ^ 1;  
    }

    return temp;
}

INCLUDE_ASM(const s32, "player", func_80067A24);

INCLUDE_ASM(const s32, "player", func_80067B38);

INCLUDE_ASM(const s32, "player", func_80067BC4);

INCLUDE_ASM(const s32, "player", func_80067E5C);

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

INCLUDE_ASM(const s32, "player", func_8006807C);

INCLUDE_ASM(const s32, "player", func_80068120);

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
    if ((func_8004D35C(0, -1)) && (gPlayer.action4 < 4)) {
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
        func_800D55E4(gPlayer.unk_2D, 0x12U);
        gPlayer.unk_2C = 0;
        gPlayer.action4 += 1;
    }

    gPlayer.action3 += 1;
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

INCLUDE_ASM(const s32, "player", func_80069830);

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

INCLUDE_ASM(const s32, "player", func_80069CC4);

void func_80069DA8(void) {
}

INCLUDE_ASM(const s32, "player", func_80069DB0);

void func_80069E54(void) {}

void func_80069E5C(void) {}

void func_80069E64(void) {}

void func_80069E6C(void) {}

INCLUDE_ASM(const s32, "player", func_80069E74);

void func_8006A2E0(void) {}

INCLUDE_ASM(const s32, "player", func_8006A2E8);

INCLUDE_ASM(const s32, "player", func_8006A714);

INCLUDE_ASM(const s32, "player", func_8006A7B0);

INCLUDE_ASM(const s32, "player", func_8006A848);

INCLUDE_ASM(const s32, "player", func_8006A8C4);

INCLUDE_ASM(const s32, "player", func_8006A9A8);

INCLUDE_ASM(const s32, "player", func_8006AA9C);

INCLUDE_ASM(const s32, "player", func_8006AB90);

INCLUDE_ASM(const s32, "player", func_8006AC4C);

INCLUDE_ASM(const s32, "player", func_8006ADF4);

INCLUDE_ASM(const s32, "player", func_8006AEEC);

INCLUDE_ASM(const s32, "player", func_8006AFE4);

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

INCLUDE_ASM(const s32, "player", func_8006DFB0);

// chicken feed
//INCLUDE_ASM(const s32, "player", func_8006E0D4);

void func_8006E0D4(void) {

    switch (gPlayer.action4) {
        case 0:
            setSpriteAnimation(0, 0);
            break;
        case 1:
            setSpriteAnimation(0, 0x169);
            break;
        case 2:
            func_800CF850();
            gPlayer.action4++;
            break;
            do {} while (0);
        case 3:
            if (!D_80189836) {
                if (!func_80067A24(0)) {
                    gPlayer.action3 = 0;
                    gPlayer.action4 = 0;
                    gPlayer.action1 = 0;
                    gPlayer.action2 = 0;
                }
                D_80237410 += adjustValue(D_80237410, -1, 0x3E7);;
                if (!D_80237410) {
                    gPlayer.currentTool = 0;
                }
            }
            break;
        default:
            break;
    }

}

void func_8006E1F8(void) {
}

void func_8006E200(void) {
}

void func_8006E208(void) {
}

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
            setSpriteAnimation(0, 0);
            func_80099DE8();
            break;
        case 1:
            setSpriteAnimation(0, 0x234);
            break;
        case 2:
            func_800CF850();
            gPlayer.action4++;
            break;
            // necessary to match
            do {} while (0);
        case 3:
            if (!D_80189836) {
                if (!func_80067A24(0)) {
                    gPlayer.action3 = 0;
                    gPlayer.action4 = 0;
                    gPlayer.action1 = 0;
                    gPlayer.action2 = 0;
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
