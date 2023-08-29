#include "common.h"

#include "gameStatus.h"

#include "animals.h"
#include "game.h"
#include "mapObjects.h"

// bss
extern u32 dailyEventBits[];
extern u32 lifeEventBits[];
extern u32 specialDialogueBits[];
// mail bits
extern u32 D_801C3F38[];
// mailbox bits
extern u32 D_8016FFEC[];

// shared
extern u32 D_80189058;


// jtbl_8011F300
INCLUDE_ASM(const s32, "gameStatus", func_80063AF0);

//INCLUDE_ASM(const s32, "gameStatus", func_80063D38);

// wife helps with weeding
void func_80063D38(void) {
    
    u8 i;
    u8 j;

    if (gSeason != WINTER) {
        for (i = 0; i < FIELD_WIDTH; i++) {
            for (j = 0; j < FIELD_HEIGHT; j++) {
                if (farmFieldTiles[i][j] == WEED) {
                    setSpecialDialogueBit(0x88);
                    func_800DAA90(FARM, 1, j, i);
                } 
            }
        } 
    }
}

INCLUDE_ASM(const s32, "gameStatus", func_80063E18);

INCLUDE_ASM(const s32, "gameStatus", func_80063F3C);

//INCLUDE_ASM(const s32, "gameStatus", func_80064048);

void func_80064048(void) {
    u8 i;

    for (i = 0; i < MAX_CHICKENS; i++) {
        if ((gChickens[i].flags & 1) && (gChickens[i].type == 2) && (gChickens[i].location == COOP)) {
            setSpecialDialogueBit(0x86);
            gChickens[i].flags |= 0x10;
        }
    }
}

INCLUDE_ASM(const s32, "gameStatus", func_80064114);

INCLUDE_ASM(const s32, "gameStatus", func_800644B0);

INCLUDE_ASM(const s32, "gameStatus", func_80064814);

INCLUDE_ASM(const s32, "gameStatus", func_80064CF0);

//INCLUDE_ASM(const s32, "gameStatus", func_80065074);

void setDailyEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    dailyEventBits[temp >> 5] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM(const s32, "gameStatus", toggleDailyEventBit);

void toggleDailyEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    dailyEventBits[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", checkDailyEventBit);

u32 checkDailyEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return dailyEventBits[temp >> 5] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", setLifeEventBit);

void setLifeEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    lifeEventBits[temp >> 5] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM(const s32, "gameStatus", toggleLifeEventBit);

void toggleLifeEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    lifeEventBits[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", checkLifeEventBit);

u32 checkLifeEventBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return lifeEventBits[temp >> 5] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", setSpecialDialogueBit);

void setSpecialDialogueBit(u16 bitIndex) {
    u32 temp = bitIndex;
    specialDialogueBits[temp >> 5] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM(const s32, "gameStatus", toggleSpecialDialogueBit);

void toggleSpecialDialogueBit(u16 bitIndex) {
    u32 temp = bitIndex;
    specialDialogueBits[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", checkSpecialDialogueBit);

u32 checkSpecialDialogueBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return specialDialogueBits[temp >> 5] & (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", func_8006523C);

void func_8006523C(u16 bitIndex) {
    u32 temp = bitIndex;
    return D_801C3F38[temp >> 5] |= (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", toggleReadLetterBit);

void toggleReadLetterBit(u16 bitIndex) {
    u32 temp = bitIndex;
    return D_801C3F38[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", checkMailRead);

u32 checkMailRead(u16 bitIndex) {
    u32 temp = bitIndex;
    return D_801C3F38[temp >> 5] & (1 << (temp & 0x1F));
}

//NCLUDE_ASM(const s32, "gameStatus", setMail);

void setMail(u16 bitIndex) {
    u32 temp = bitIndex;
    return D_8016FFEC[temp >> 5] |= (1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", func_80065308);

void func_80065308(u16 bitIndex) {
    u32 temp = bitIndex;
    return D_8016FFEC[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM(const s32, "gameStatus", func_80065340);

u32 func_80065340(u16 bitIndex) {
    u32 temp = bitIndex;
    return D_8016FFEC[temp >> 5] & (1 << (temp & 0x1F));
}

INCLUDE_ASM(const s32, "gameStatus", func_8006536C);


// jtbl_8011F398
//INCLUDE_ASM(const s32, "gameStatus", setAlbumPicture);

void setAlbumPicture(u8 pictureBitIndex) {
    
    switch (pictureBitIndex) {
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
            D_80189058 |= 0x80;
            break;
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
            D_80189058 |= 0x100;
            break;
        case 0x2E:
            D_80189058 |= 2;
            break;
        case 0x2F:
            D_80189058 |= 4;
            break;
        case 0x30:
            D_80189058 |= 0x10;
            break;
        case 0x31:
            D_80189058 |= 0x40;
            break;
        case 0x32:
            D_80189058 |= 0x20;
            break;
        case 0x33:
            D_80189058 |= 0x4000;
            break;
        case 0x34:
            D_80189058 |= 8;
            break;
        case 0x35:
            D_80189058 |= 0x200;
            break;
        case 0x36:
            D_80189058 |= 0x400;
            break;
        case 0x37:
            D_80189058 |= 0x800;
            break;
        case 0x38:
            D_80189058 |= 0x1000;
            break;
        case 0x39:
            D_80189058 |= 0x2000;
            break;
        }
}

INCLUDE_ASM(const s32, "gameStatus", func_80065518);

INCLUDE_ASM(const s32, "gameStatus", func_80065774);

void func_800657AC(void) {}

void func_800657B4(void) {}

void func_800657BC(void) {}

void func_800657C4(void) {}
