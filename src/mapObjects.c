#include "common.h"

extern u8 D_80118704[0xDA][5];
extern u8 D_80182BA8[0x18][0x14];

extern u8 farmFieldTiles[FARM_FIELD_WIDTH][FARM_FIELD_HEIGHT];

void func_80035004(u8, u8, u8, u8);                  
void func_80036C08(u32);                                 
void func_80036FA0(u32);                                 
void func_80038B58(u8, u8, u8, u8);                   
u8 func_800DAA90(u8, u16, u8, u8);      

extern u8 D_80118700[0xDA][4];
extern u8 D_801C3F35;
extern u8 D_801FD624;


INCLUDE_ASM(const s32, "mapObjects", func_800D9600);

INCLUDE_ASM(const s32, "mapObjects", func_800D9AC8);

INCLUDE_ASM(const s32, "mapObjects", func_800D9BFC);

INCLUDE_ASM(const s32, "mapObjects", func_800DA8B8);

INCLUDE_ASM(const s32, "mapObjects", func_800DA8E8);

INCLUDE_ASM(const s32, "mapObjects", func_800DA918);

INCLUDE_ASM(const s32, "mapObjects", func_800DA948);

//INCLUDE_ASM(const s32, "mapObjects", func_800DA978);

u8 func_800DA978(u8 index) {
    if (index < 0xDA) {
		return D_80118704[index][0];
    }
    return 0;
}

INCLUDE_ASM(const s32, "mapObjects", func_800DA9A8);

INCLUDE_ASM(const s32, "mapObjects", func_800DAA90);

#ifdef PERMUTER
void func_800DAC70(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    u32 temp;
    u32 temp2;
    u32 temp3;
    
    if (func_800DAA90(arg0, arg1, arg2, arg3)) {
        
        temp3 = D_80118700[arg1][0]; 
        temp = D_80118700[arg1][1];
        temp2 = temp;
        
        if (temp3 != 0xFF) {
            func_80038B58(0, temp3, D_801FD624 + arg2, D_801C3F35 + arg3);
        }

        if (temp3 == 0xFF) {
            temp2 = 0xffff;
        }
        if (temp3 == 0) {
            temp2 = 0;
        }
        
        func_80035004(0, temp2, arg2, arg3);
        func_80036FA0(0);
        func_80036C08(0);
    }
}
#else
INCLUDE_ASM(const s32, "mapObjects", func_800DAC70);
#endif

INCLUDE_ASM(const s32, "mapObjects", func_800DAD74);

INCLUDE_ASM(const s32, "mapObjects", func_800DAF58);

INCLUDE_ASM(const s32, "mapObjects", func_800DB1BC);

INCLUDE_ASM(const s32, "mapObjects", func_800DB424);

INCLUDE_ASM(const s32, "mapObjects", func_800DB858);

INCLUDE_ASM(const s32, "mapObjects", func_800DB99C);

INCLUDE_ASM(const s32, "mapObjects", func_800DBAC4);

INCLUDE_ASM(const s32, "mapObjects", func_800DBBB0);

INCLUDE_ASM(const s32, "mapObjects", func_800DBC9C);

INCLUDE_ASM(const s32, "mapObjects", func_800DBD88);

INCLUDE_ASM(const s32, "mapObjects", func_800DBE8C);

INCLUDE_ASM(const s32, "mapObjects", func_800DBF90);

INCLUDE_ASM(const s32, "mapObjects", func_800DC008);

INCLUDE_ASM(const s32, "mapObjects", func_800DC080);

//INCLUDE_ASM(const s32, "mapObjects", func_800DC0F8);

u16 func_800DC0F8(void) {
    u8 i, j;
    u16 count = 0;
    int temp;
    for (i = 0; i < 0x18; i++) {
        for (j = 0; j < 0x14; j++) {
            temp = farmFieldTiles[i][j];
            if (temp < 0xB1) { 
                if (0xAE < temp) {
                    count++;                    
                }
            }
        }
    }
    return count;
}


//INCLUDE_ASM(const s32, "mapObjects", func_800DC170);

u16 func_800DC170(void) {

    u8 i;
    u8 j;
    int tempVal;
    u16 count = 0;

    for (i = 0; i < 0x18; i++) {
        for (j = 0; j < 0x14; j++) {
            tempVal = D_80182BA8[i][j];
            if (tempVal < 0xB1) {
                if (tempVal > 0xAE) {
                    count++;                    
                }
            }
        }
    }
    
    return count;
}

INCLUDE_ASM(const s32, "mapObjects", func_800DC1E8);
