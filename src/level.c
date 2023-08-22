#include "common.h"


void func_80036C08(s32);                                 
void func_80036FA0(s32);                                 
void func_8003BD60(s32);                                 
void func_8006EC58(s32);                               
void func_800735FC(s32);                               
void func_800D9600(u8);                                
void func_800DC360();    

extern u8 previousExitIndex;
extern u8 exitIndex;

extern u8 gBaseMapIndex;

// rodata
extern u8 levelFlags[];
// map indices for level interactions
extern u8 D_801147C0[];

//u8 getLevelFlags(u8);

//INCLUDE_ASM(const s32, "level", setExit);

void setExit(u16 index) {
    u16 temp = exitIndex;
    exitIndex = index;
    previousExitIndex = temp;
}

INCLUDE_ASM(const s32, "level", func_8006E840);

//INCLUDE_ASM(const s32, "level", setLevelGraphicsData);

void setLevelGraphicsData(u16 mapIndex) {

    // set tile flag
    func_8003BD60(0);
    
    func_8006EC58(mapIndex);
    func_800735FC(mapIndex);

    // load map object sprites
    func_8006F938(mapIndex);

    func_800746B4(mapIndex);

    func_800D9600(gBaseMapIndex);
    
    if (getLevelFlags(mapIndex) & OUTDOORS) {
        // load rain/snow sprites
        func_800DC360();
    }
    
    // tiling
    func_80036C08(0);

    func_80036FA0(0);
}

INCLUDE_ASM(const s32, "level", func_8006EA44);

INCLUDE_ASM(const s32, "level", func_8006EB94);

//INCLUDE_ASM(const s32, "level", getLevelFlags);

u8 getLevelFlags(u16 mapIndex) {
    return levelFlags[mapIndex];
}

INCLUDE_ASM(const s32, "level", func_8006EC58);

INCLUDE_ASM(const s32, "level", func_8006F938);

INCLUDE_ASM(const s32, "level", func_80073244);

INCLUDE_ASM(const s32, "level", func_8007341C);

INCLUDE_ASM(const s32, "level", func_800735FC);

INCLUDE_ASM(const s32, "level", func_800746B4);

//INCLUDE_ASM(const s32, "level", func_80074C38);

u8 func_80074C38(u8 mapIndex) {
    return D_801147C0[mapIndex];
}

// jtbl_8011FEA0
INCLUDE_ASM(const s32, "level", func_80074C50);

// D_80120008
// __osRunningThread?