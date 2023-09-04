#include "common.h"

#include "level.h"
#include "mapObjects.h"

#include "system/map.h"

// bss
// object maps for levels
// top of mountain 1
extern u8 D_8013D258[FIELD_WIDTH][FIELD_HEIGHT];
// moon mountain
extern u8 D_8016FBF0[FIELD_WIDTH][FIELD_HEIGHT];
// pond
extern u8 D_8016FE08[FIELD_WIDTH][FIELD_HEIGHT];
// cave
extern u8 D_8016F8F8[FIELD_WIDTH][FIELD_HEIGHT];
// mine
extern u8 D_80170468[FIELD_WIDTH][FIELD_HEIGHT];
// greenhouse
extern u8 D_80182BA8[FIELD_WIDTH][FIELD_HEIGHT];
// mountain 1
extern u8 D_80182FD8[FIELD_WIDTH][FIELD_HEIGHT];
// mountain 2
extern u8 D_80189868[FIELD_WIDTH][FIELD_HEIGHT];
// ranch
extern u8 D_801C3FC0[FIELD_WIDTH][FIELD_HEIGHT];

extern u8 farmFieldTiles[FIELD_WIDTH][FIELD_HEIGHT];

extern u8 D_80118700[0xDA][4];

// shared
extern u8 D_801C3F35;
extern u8 D_801FD624;

// data
extern u8 D_80118704[0xDA][5];


// jtbl_80122858
//INCLUDE_ASM(const s32, "mapObjects", func_800D9600);

void func_800D9600(u8 mapIndex) {

    u8 i, j;
    
    switch (mapIndex) {

        case FARM:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (farmFieldTiles[i][j]) {
                        func_800DAC70(mapIndex, farmFieldTiles[i][j], j, i);
                    }
                }
            }
            return;

        case GREENHOUSE:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80182BA8[i][j]) {
                        func_800DAC70(mapIndex, D_80182BA8[i][j], j, i);
                    }
                }
            }
            return;

        case MOUNTAIN_1:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80182FD8[i][j]) {
                        func_800DAC70(mapIndex, D_80182FD8[i][j], j, i);
                    }
                }
            }
            return;

        case MOUNTAIN_2:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80189868[i][j]) {
                        func_800DAC70(mapIndex, D_80189868[i][j], j, i);
                    }
                }
            }
            return;    

        case TOP_OF_MOUNTAIN_1:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8013D258[i][j]) {
                        func_800DAC70(mapIndex, D_8013D258[i][j], j, i);
                    }
                }
            }
            return;    

        case MOON_MOUNTAIN:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8016FBF0[i][j]) {
                        func_800DAC70(mapIndex, D_8016FBF0[i][j], j, i);
                    }
                }
            }
            return;    

        case POND:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8016FE08[i][j]) {
                        func_800DAC70(mapIndex, D_8016FE08[i][j], j, i);
                    }
                }
            }
            return;

        case CAVE:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_8016F8F8[i][j]) {
                        func_800DAC70(mapIndex, D_8016F8F8[i][j], j, i);
                    }
                }
            }
            return;    

        case MINE:
        case 0x45:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
                    if (D_80170468[i][j]) {
                        func_800DAC70(mapIndex, D_80170468[i][j], j, i);
                    }
                }
            }
            return;    

        case RANCH:
            for (i = 0; i < FIELD_WIDTH; i++) {
                for (j = 0; j < FIELD_HEIGHT; j++) {
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

// jtbl_801229B8
//INCLUDE_ASM(const s32, "mapObjects", func_800D9AC8);

void func_800D9AC8(u8 mapIndex) {

    u8 i, j;
    
    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
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

//INCLUDE_ASM(const s32, "mapObjects", countPinkCatMintFlowersFarm);

u16 countPinkCatMintFlowersFarm(void) {

    u8 i, j;
    u16 count = 0;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            switch (farmFieldTiles[i][j]) {
                case 0xAF ... 0xB0:
                    count++;
                    break;
                default:
                    break;
            }
        }
    }
    
    return count;
}

//INCLUDE_ASM(const s32, "mapObjects", countPinkCatMintFlowersGreenhouse);

u16 countPinkCatMintFlowersGreenhouse(void) {

    u8 i, j;
    u16 count = 0;

    for (i = 0; i < FIELD_WIDTH; i++) {
        for (j = 0; j < FIELD_HEIGHT; j++) {
            switch (D_80182BA8[i][j]) {
                // pink cat mint flowers
                case 0xAF ... 0xB0:
                    count++;
                    break;
                default:
                    break;
            }
        }
    }
    
    return count;
}

INCLUDE_ASM(const s32, "mapObjects", func_800DC1E8);
