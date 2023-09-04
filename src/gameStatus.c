#include "common.h"

#include "gameStatus.h"
#include "system/mathUtils.h"

#include "animals.h"
#include "game.h"
#include "level.h"
#include "mapObjects.h"
#include "npc.h"

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

// forward declaration
void toggleSpecialDialogueBit(u16);


// jtbl_8011F300
//INCLUDE_ASM(const s32, "gameStatus", func_80063AF0);

void func_80063AF0(void) {

    u8 temp;

    toggleSpecialDialogueBit(0x85);
    toggleSpecialDialogueBit(0x86);
    toggleSpecialDialogueBit(0x88);
    toggleSpecialDialogueBit(0x89);

    temp = getRandomNumberInRange(0, 1);

    switch (gWife) {
        case 0:
            if (!getRandomNumberInRange(0,  (5 - npcAffection[0] / 50))) {
                if (temp) {
                    func_80063D38();
                    break;
                }
                func_80063E18();
            } 
            break;
        case 1:
            if (!getRandomNumberInRange(0,  (5 - npcAffection[1] / 50))) {
                if (temp) {
                    func_80063F3C();
                    break;
                }
                func_80063E18();
            } 
            break;
        case 2:
            if (!getRandomNumberInRange(0,  (5 - npcAffection[2] / 50))) {
                if (temp) {
                    func_80064048();
                    break;
                }
                func_80063E18();
            } 
            break;        
        case 3:
            if (!getRandomNumberInRange(0,  (5 - npcAffection[3] / 50))) {
                if (temp) {
                    func_80064048();
                    break;
                }
                func_80063E18();
            } 
            break;
        case 4:
            if (!getRandomNumberInRange(0,  (5 - npcAffection[4] / 50))) {
                if (temp) {
                    func_80063E18();
                }
            } 
            break;
        default:
            break;
    }
}


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

//INCLUDE_ASM(const s32, "gameStatus", func_80063E18);

void func_80063E18(void) {

    u8 i = 0;
 
    for (i = 0; i < MAX_CHICKENS; i++) {
        
        if (gChickens[i].flags & 1 && gChickens[i].type == 0 && !(gChickens[i].flags & 0x20)) {
            
            setSpecialDialogueBit(0x85);
            gChickens[i].flags = 0;

            handleAddShipment(EGG_VALUE);
            handleShipEgg();
            
        }
    }
}

//INCLUDE_ASM(const s32, "gameStatus", func_80063F3C);

void func_80063F3C(void) {

    u8 i, j;
    u8 temp;
    
    if (gSeason != WINTER) {

        for (i = 0; i < FIELD_WIDTH; i++) {
            for (j = 0; j < FIELD_HEIGHT; j++) {
                temp = farmFieldTiles[i][j];
                if (farmFieldTiles[i][j] && func_800DA948(temp) & 0x10 && !(getRandomNumberInRange(0, 3))) {
                    setSpecialDialogueBit(0x89);
                    temp++;
                    func_800DAA90(0x52, temp, j, i);
                }
            }
        }   
    }
}

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

#ifdef PERMUTER
u8 func_8006536C(void) {

    u8 result = 0xFF;
    u8 i;
    u32 temp;

    for (i = 0; i < 0x50 && result == 0xFF; i++) {

        temp = i;

        if (D_8016FFEC[temp >> 5] & 1 << (temp & 0x1F)) {
            
            D_8016FFEC[temp] &= ~(1 << (temp & 0x1F));
            D_801C3F38[temp >> 5] |= 1 << (temp & 0x1F);
            
            result = i;
        }

        i++;
        
    } 
    
    return result;
    
}
#else
INCLUDE_ASM(const s32, "gameStatus", func_8006536C);
#endif

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

//INCLUDE_ASM(const s32, "gameStatus", func_80065518);

u8 func_80065518(void) {
    
    u8 result = 0;

    result = checkRecipe(0) != 0;
    
    if (checkRecipe(1)) {
        result++;
    }
    
    if (checkRecipe(2)) {
        result++;
    }

    if (checkRecipe(3)) {
        result++;
    }

    if (checkRecipe(4)) {
        result++;
    }

    if (checkRecipe(5)) {
        result++;
    }

    if (checkRecipe(6)) {
        result++;
    }
    
    if (checkRecipe(7)) {
        result++;
    }

    if (checkRecipe(8)) {
        result++;
    }

    if (checkRecipe(9)) {
        result++;
    }

    if (checkRecipe(0xA)) {
        result++;
    }

    if (checkRecipe(0xB)) {
        result++;
    }

    if (checkRecipe(0xC)) {
        result++;
    }

    if (checkRecipe(0xD)) {
        result++;
    }

    if (checkRecipe(0xE)) {
        result++;
    }

    if (checkRecipe(0xF)) {
        result++;
    }

    if (checkRecipe(0x10)) {
        result++;
    }

    if (checkRecipe(0x11)) {
        result++;
    }

    if (checkRecipe(0x12)) {
        result++;
    }

    if (checkRecipe(0x13)) {
        result++;
    }

    if (checkRecipe(0x14)) {
        result++;
    }

    if (checkRecipe(0x15)) {
        result++;
    }
    if (checkRecipe(0x16)) {
        result++;
    }

    if (checkRecipe(0x17)) {
        result++;
    }

    if (checkRecipe(0x18)) {
        result++;
    }

    if (checkRecipe(0x19)) {
        result++;
    }

    if (checkRecipe(0x1A)) {
        result++;
    }

    if (checkRecipe(0x1B)) {
        result++;
    }
    if (checkRecipe(0x1C)) {
        result++;
    }

    if (checkRecipe(0x1D)) {
        result++;
    }

    if (checkRecipe(0x1E)) {
        result++;
    }

    if (checkRecipe(0x1F)) {
        result++;
    }

    if (checkRecipe(0x20)) {
        result++;
    }

    if (checkRecipe(0x21)) {
        result++;
    }

    if (checkRecipe(0x22)) {
        result++;
    }

    return result;
}

//INCLUDE_ASM(const s32, "gameStatus", func_80065774);

u16 func_80065774(void) {

    u16 result = 0;
    u8 i;

    for (i = 0; i < TOTAL_NPCS; i++) {
        result += npcAffection[i];
    }

    return result;
    
}

void func_800657AC(void) {}

void func_800657B4(void) {}

void func_800657BC(void) {}

void func_800657C4(void) {}
