#include "common.h"

extern u8 D_80118704[0xDA][5];

// object maps for levels
// top of mountain 1
extern u8 D_8013D258[0x18][0x14];
// moon mountain
extern u8 D_8016FBF0[0x18][0x14];
// pond
extern u8 D_8016FE08[0x18][0x14];
// cave
extern u8 D_8016F8F8[0x18][0x14];
// mine
extern u8 D_80170468[0x18][0x14];
// greenhouse
extern u8 D_80182BA8[0x18][0x14];
// mountain 1
extern u8 D_80182FD8[0x18][0x14];
// mountain 2
extern u8 D_80189868[0x18][0x14];
// ranch
extern u8 D_801C3FC0[0x18][0x14];

extern u8 farmFieldTiles[FARM_FIELD_WIDTH][FARM_FIELD_HEIGHT];

void func_80035004(u8, u8, u8, u8);                  
void func_80036C08(u32);                                 
void func_80036FA0(u32);                                 
void func_80038B58(u8, u8, u8, u8);                   
u8 func_800DAA90(u8, u16, u8, u8);      
void func_800DAC70(u8, u8, u8, u8);

extern u8 D_80118700[0xDA][4];
extern u8 D_801C3F35;
extern u8 D_801FD624;


// jtbl_80122858
INCLUDE_ASM(const s32, "mapObjects", func_800D9600);

/*
void func_800D9600(u8 mapIndex) {

    u8 i, j;
    
    switch (mapIndex) {
        case 0x52:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (farmFieldTiles[i][j]) {
                        func_800DAC70(mapIndex, farmFieldTiles[i][j], j, i);
                    }
                }
            }
            return;
        case 0x56:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_80182BA8[i][j]) {
                        func_800DAC70(mapIndex, D_80182BA8[i][j], j, i);
                    }
                }
            }
            return;
        case 0x11:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_80182FD8[i][j]) {
                        func_800DAC70(mapIndex, D_80182FD8[i][j], j, i);
                    }
                }
            }
            return;
        case 0x15:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_80189868[i][j]) {
                        func_800DAC70(mapIndex, D_80189868[i][j], j, i);
                    }
                }
            }
            return;        
        case 0x19:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_8013D258[i][j]) {
                        func_800DAC70(mapIndex, D_8013D258[i][j], j, i);
                    }
                }
            }
            return;    
        case 0x1D:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_8016FBF0[i][j]) {
                        func_800DAC70(mapIndex, D_8016FBF0[i][j], j, i);
                    }
                }
            }
            return;    
        case 0x23:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_8016FE08[i][j]) {
                        func_800DAC70(mapIndex, D_8016FE08[i][j], j, i);
                    }
                }
            }
            return;
        case 0x43:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_8016F8F8[i][j]) {
                        func_800DAC70(mapIndex, D_8016F8F8[i][j], j, i);
                    }
                }
            }
            return;    
        case 0x44:
        case 0x45:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_80170468[i][j]) {
                        func_800DAC70(mapIndex, D_80170468[i][j], j, i);
                    }
                }
            }
            return;    
        case 0:
            for (i = 0; i < 0x18; i++) {
                for (j = 0; j < 0x14; j++) {
                    if (D_801C3FC0[i][j]) {
                        func_800DAC70(mapIndex, D_801C3FC0[i][j], j, i);
                    }
                }
            }
            return;   
        default:
            return;
    }
}
*/

// jtbl_801229B8
INCLUDE_ASM(const s32, "mapObjects", func_800D9AC8);

/*
void func_800D9AC8(u8 mapIndex) {

    u8 i, j;
    
    for (i = 0; i < 0x18; i++) {
        for (j = 0; j < 0x14; j++) {
            switch (mapIndex) {
                case MOUNTAIN_1:
                    D_80182FD8[i][j] = 0;
                    break;
                case MOUNTAIN_2:
                    D_80189868[i][j] = 0;
                    break;
                case TOP_OF_MOUNTAIN_1:
                    D_8013D258[i][j] = 0;
                    break;
                case MOON_MOUNTAIN:
                    D_8016FBF0[i][j] = 0;
                    break;
                case POND:
                    D_8016FE08[i][j] = 0;
                    break;
                case CAVE:
                    D_8016F8F8[i][j] = 0;
                    break;
                case MINE:
                case 0x45:
                    D_80170468[i][j] = 0;
                    break;
                case RANCH:
                    D_801C3FC0[i][j] = 0;
                    break;

            }
        }
    }   
}
*/

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

// jtbl_80122AD0
INCLUDE_ASM(const s32, "mapObjects", func_800DA9A8);

// jtbl_80122C30
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

// greenhouse
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
