#include "common.h"

void func_8002B6B8(u32);                               
void func_8002BAD8(u8);  
void func_80046120(u8);                                 
void func_800C7DF8();                                  
void func_800C7E18();                                  
void func_800C7E38();                                  
void func_800C7E58();                                  
void func_800C81A8();                                  
void func_800C81C8();      

extern u32 recipesBitField[];

// rodata
/*
// const double D_80121C90 = 0.5;
const u8 D_80121E34[16] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E }
const u8 D_801220CC[][];
const u8 D_80122140[][];
const u16 D_801221CC[3] = { 0xBBB, 0x3E8, 0x1F4 }
*/

//INCLUDE_ASM(const s32, "overlayScreens", func_800B2CE0);

void func_800B2CE0(void) {

    u8 i, j, k;

    func_800C81A8();
    
    i = 0;
    
    func_800C81C8();
    func_800C7DF8();
    func_800C7E18();
    func_800C7E58();
    func_800C7E38();
    func_8002B6B8(0x82);
    func_8002B6B8(0x83);
    func_8002B6B8(0x84);
    func_8002B6B8(0x85);
    func_8002B6B8(0x86);
    func_8002B6B8(0x87);
    func_8002B6B8(0x88);
    func_8002B6B8(0x81);
    func_8002BAD8(0x79);
    func_8002B6B8(0x89);
    func_8002B6B8(0x8C);
    func_8002B6B8(0x8D);
    func_8002B6B8(0x8B);
    func_8002B6B8(0x8A);
    func_80046120(0);
    func_8002B6B8(0x8E);
    func_80046120(1);
    func_8002B6B8(0x86);
    func_8002B6B8(0x82);
    func_8002B6B8(0x83);
    func_8002B6B8(0x84);
    func_8002B6B8(0x85);
    func_8002B6B8(0xB9);
    func_8002B6B8(0xBA);
    func_8002B6B8(0xBB);
    func_8002B6B8(0xBC);
    func_8002B6B8(0xBD);
    func_8002B6B8(0xBE);
    func_8002B6B8(0x82);
    func_8002B6B8(0x83);
    func_8002B6B8(0x84);
    func_8002B6B8(0x85);
    func_8002B6B8(0x86);
    func_80046120(0);
    func_80046120(2);
    func_80046120(3);
    func_80046120(4);
    func_80046120(5);
    func_80046120(6);
    func_80046120(2);
    func_80046120(3);
    func_80046120(4);
    func_80046120(5);
    func_80046120(6);
    func_80046120(7);
    func_80046120(8);
    func_80046120(9);
    func_80046120(0xA);
    func_80046120(0xB);
    func_80046120(0xC);
    func_80046120(0xD);
    func_80046120(0xE);

    for (i = 0; i < 0x28; i++) {
        func_8002B6B8(i + 0xA3);
    }

    for (j = 0; j < 0x1E; j++) {
        func_8002B6B8(j + 0x8F);
    }

    for (k = 0; k < 0xA; k++) {
        func_8002B6B8(k + 0xAD);
    }
}

INCLUDE_ASM(const s32, "overlayScreens", func_800B2F34);

INCLUDE_ASM(const s32, "overlayScreens", func_800B3498);

INCLUDE_ASM(const s32, "overlayScreens", func_800B3694);

INCLUDE_ASM(const s32, "overlayScreens", func_800B3A60);

INCLUDE_ASM(const s32, "overlayScreens", func_800B3BD8);

INCLUDE_ASM(const s32, "overlayScreens", func_800B4160);

INCLUDE_ASM(const s32, "overlayScreens", func_800B4238);

INCLUDE_ASM(const s32, "overlayScreens", func_800B42E0);

INCLUDE_ASM(const s32, "overlayScreens", func_800B59AC);

INCLUDE_ASM(const s32, "overlayScreens", func_800B5BD4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B5DA8);

INCLUDE_ASM(const s32, "overlayScreens", func_800B5FC4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B60E4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B64E4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B6674);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7070);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7B34);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7CB4);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7E28);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7F6C);

INCLUDE_ASM(const s32, "overlayScreens", func_800B7FC8);

INCLUDE_ASM(const s32, "overlayScreens", func_800B8018);

INCLUDE_ASM(const s32, "overlayScreens", func_800B815C);

INCLUDE_ASM(const s32, "overlayScreens", func_800B82AC);

INCLUDE_ASM(const s32, "overlayScreens", func_800B83F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800B8554);

INCLUDE_ASM(const s32, "overlayScreens", func_800B86B8);

INCLUDE_ASM(const s32, "overlayScreens", func_800B881C);

INCLUDE_ASM(const s32, "overlayScreens", func_800B8844);

INCLUDE_ASM(const s32, "overlayScreens", func_800B8980);

INCLUDE_ASM(const s32, "overlayScreens", func_800B90E0);

INCLUDE_ASM(const s32, "overlayScreens", func_800B9914);

INCLUDE_ASM(const s32, "overlayScreens", func_800B99EC);

INCLUDE_ASM(const s32, "overlayScreens", func_800B9B8C);

// jtbl_80121C98
INCLUDE_ASM(const s32, "overlayScreens", func_800B9D3C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BA928);

INCLUDE_ASM(const s32, "overlayScreens", func_800BA9E8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BAAA0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BAC7C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BADD0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BAF1C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BB590);

INCLUDE_ASM(const s32, "overlayScreens", func_800BBAF0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BBBC8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BBD34);

// jtbl_80121CC0
INCLUDE_ASM(const s32, "overlayScreens", func_800BBEC0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC700);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC770);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC7D8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BC918);

INCLUDE_ASM(const s32, "overlayScreens", func_800BCA9C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BCBE8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD25C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD754);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD82C);

INCLUDE_ASM(const s32, "overlayScreens", func_800BD998);

// jtbl_80121CE0
INCLUDE_ASM(const s32, "overlayScreens", func_800BDB24);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE3B8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE428);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE490);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE5D0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE684);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE808);

INCLUDE_ASM(const s32, "overlayScreens", func_800BE954);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF0C8);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF5C0);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF698);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF804);

INCLUDE_ASM(const s32, "overlayScreens", func_800BF990);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0238);

INCLUDE_ASM(const s32, "overlayScreens", func_800C02A8);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0310);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0450);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0504);

// jtbl_80121D00
INCLUDE_ASM(const s32, "overlayScreens", func_800C0688);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0714);

INCLUDE_ASM(const s32, "overlayScreens", func_800C0F24);

INCLUDE_ASM(const s32, "overlayScreens", func_800C101C);

//INCLUDE_RODATA(const s32, "overlayScreens", D_80121D74);

static const u16 D_80121D74[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

// rodata: D_80121D74
// jtbl_80121D80
INCLUDE_ASM(const s32, "overlayScreens", func_800C1124);

INCLUDE_ASM(const s32, "overlayScreens", func_800C1710);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2060);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2128);

// jtbl_80121DA0
INCLUDE_ASM(const s32, "overlayScreens", func_800C224C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2364);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2504);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2660);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2780);

INCLUDE_ASM(const s32, "overlayScreens", func_800C292C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2A54);

// jtbl_80121DB8
INCLUDE_ASM(const s32, "overlayScreens", func_800C2B8C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C2F50);

INCLUDE_ASM(const s32, "overlayScreens", func_800C3108);

INCLUDE_ASM(const s32, "overlayScreens", func_800C34C0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C39A0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C3B1C);

// jtbl_80121DD0
INCLUDE_ASM(const s32, "overlayScreens", func_800C3D20);

INCLUDE_ASM(const s32, "overlayScreens", func_800C4A74);

INCLUDE_ASM(const s32, "overlayScreens", func_800C4E90);

INCLUDE_ASM(const s32, "overlayScreens", func_800C5080);

INCLUDE_ASM(const s32, "overlayScreens", func_800C5218);

// jtbl_80121DF0
INCLUDE_ASM(const s32, "overlayScreens", func_800C53C0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C5AA4);

// jtbl_80121E08
// jtbl_80121E20
INCLUDE_ASM(const s32, "overlayScreens", func_800C5DA4);

INCLUDE_ASM(const s32, "overlayScreens", func_800C6E98);

INCLUDE_ASM(const s32, "overlayScreens", func_800C6F70);

INCLUDE_RODATA(const s32, "overlayScreens", D_80121E34);

//static const u8 D_80121E34[20] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E, 0, 0, 0, 0 };

// static const u32 D_80121E34[4] = { 0x15161718, 0x191A1B1C, 0x1D1E1F20, 0x21222D2E, 0 };

// D_80121E34
// jtbl_80121E48
INCLUDE_ASM(const s32, "overlayScreens", func_800C7058);

INCLUDE_ASM(const s32, "overlayScreens", addRecipe);

INCLUDE_ASM(const s32, "overlayScreens", func_800C77EC);

//INCLUDE_ASM(const s32, "overlayScreens", checkRecipe);

u32 checkRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    return recipesBitField[temp >> 5] & (1 << (temp & 0x1F));
}

// possible split: 0xA2C50
INCLUDE_ASM(const s32, "overlayScreens", func_800C7850);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7C88);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7CE4);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7D40);

INCLUDE_ASM(const s32, "overlayScreens", func_800C7D9C);

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7DF8);

void func_800C7DF8(void) {
    func_8002BAD8(0x7C);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7E18);

void func_800C7E18(void) {
    func_8002BAD8(0x7D);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7E38);

void func_800C7E38(void) {
    func_8002BAD8(0x7F);
}

//INCLUDE_ASM(const s32, "overlayScreens", func_800C7E58);

void func_800C7E58(void) {
    func_8002BAD8(0x7E);
}

INCLUDE_ASM(const s32, "overlayScreens", func_800C7E78);

INCLUDE_ASM(const s32, "overlayScreens", func_800C80F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800C814C);

INCLUDE_ASM(const s32, "overlayScreens", func_800C81A8);

//INCLUDE_ASM(const s32, "overlayScreens", func_800C81C8);

void func_800C81C8(void) {
    func_8002BAD8(0x7A);
}

// jtbl_80121E60
INCLUDE_ASM(const s32, "overlayScreens", func_800C81E8);

// jtbl_80121F28
INCLUDE_ASM(const s32, "overlayScreens", func_800C8424);

// jtbl_80122098
INCLUDE_ASM(const s32, "overlayScreens", func_800C8784);

INCLUDE_ASM(const s32, "overlayScreens", func_800C88F4);

INCLUDE_ASM(const s32, "overlayScreens", func_800C8B04);

INCLUDE_ASM(const s32, "overlayScreens", func_800C9FF0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CA590);

INCLUDE_ASM(const s32, "overlayScreens", func_800CA678);

// jtbl_801220B0
INCLUDE_ASM(const s32, "overlayScreens", func_800CA808);

// horse race
// D_801220CC
INCLUDE_ASM(const s32, "overlayScreens", func_800CB0C4);

// D_80122140
INCLUDE_ASM(const s32, "overlayScreens", func_800CB8F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC11C);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC17C);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC518);

INCLUDE_ASM(const s32, "overlayScreens", func_800CC6F0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CCFC8);

INCLUDE_ASM(const s32, "overlayScreens", func_800CD628);

INCLUDE_ASM(const s32, "overlayScreens", func_800CD690);

INCLUDE_RODATA(const s32, "overlayScreens", D_801220CC);

INCLUDE_RODATA(const s32, "overlayScreens", D_80122140);

// jtbl_801221B8
INCLUDE_ASM(const s32, "overlayScreens", func_800CD750);

INCLUDE_ASM(const s32, "overlayScreens", func_800CD928);

INCLUDE_ASM(const s32, "overlayScreens", func_800CDA6C);

INCLUDE_ASM(const s32, "overlayScreens", func_800CDFB0);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE008);

INCLUDE_RODATA(const s32, "overlayScreens", D_801221CC);

// D_801221CC
// jtbl_801221D8
INCLUDE_ASM(const s32, "overlayScreens", func_800CE068);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE714);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE7A8);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE828);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE894);

INCLUDE_ASM(const s32, "overlayScreens", func_800CE930);

INCLUDE_ASM(const s32, "overlayScreens", func_800CEA74);

INCLUDE_ASM(const s32, "overlayScreens", func_800CED60);

INCLUDE_ASM(const s32, "overlayScreens", func_800CEDA8);

// jtbl_80122200
// jtbl_80122220
// jtbl_80122238
INCLUDE_ASM(const s32, "overlayScreens", func_800CEDF0);

// jtbl_80122250
INCLUDE_ASM(const s32, "overlayScreens", func_800CF34C);

// jtbl_80122268
INCLUDE_ASM(const s32, "overlayScreens", func_800CF3C4);

// jtbl_80122280
INCLUDE_ASM(const s32, "overlayScreens", func_800CF498);

// jtbl_80122298
INCLUDE_ASM(const s32, "overlayScreens", func_800CF544);
