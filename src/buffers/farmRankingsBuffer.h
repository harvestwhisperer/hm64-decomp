#ifndef _FARM_RANKINGS_BUFFER_H_
#define _FARM_RANKINGS_BUFFER_H_

#include "common.h"

#define FARM_RANKINGS_BUFFER 0x8030E000

typedef struct {
    u8 signature[16];               // 0x00-0x0F "PACKINSOFT FARM2"
    u8 flags;                       // 0x10
    u8 farmName[6];                 // 0x11-0x16
    u8 playerName[6];               // 0x17-0x1C
    u8 wifeName[6];                 // 0x1D-0x22
    u8 babyName[6];                 // 0x23-0x28
    u8 dogName[6];                  // 0x29-0x2E
    u8 horseName[6];                // 0x2F-0x34
    u8 year;                        // 0x35
    u8 season;                      // 0x36
    u8 mariaAffection;              // 0x37
    u8 popuriAffection;             // 0x38
    u8 elliAffection;               // 0x39
    u8 annAffection;                // 0x3A
    u8 karenAffection;              // 0x3B
    u8 harrisAffection;             // 0x3C
    u8 grayAffection;               // 0x3D
    u8 jeffAffection;               // 0x3E
    u8 cliffAffection;              // 0x3F
    u8 kaiAffection;                // 0x40
    u8 wifeAffection;               // 0x41
    u8 babyAffection;               // 0x42
    u8 dogAffection;                // 0x43
    u8 horseAffection;              // 0x44
    u8 farmAnimalAffection[8];      // 0x45-0x4C
    u8 chickenCount;                // 0x4D
    u8 wife;                        // 0x4E
    u8 pad_4F;                      // 0x4F
    u32 cropsShipped;               // 0x50-0x53
    u32 eggsShipped;                // 0x54-0x57
    u32 milkShipped;                // 0x58-0x5B
    u32 fishCaught;                 // 0x5C-0x5F
    u32 gold;                       // 0x60-0x63
    u16 grassTiles;                 // 0x64-0x65
    u8 maxStamina;                  // 0x66
    u8 photoCount;                  // 0x67
    u8 recipeCount;                 // 0x68
    u8 houseExtensions;             // 0x69
    u8 happiness;                   // 0x6A
    u8 pad_6B;                      // 0x6B
    f32 score;                      // 0x6C-0x6F
    u8 reserved[0x8E];              // 0x70-0xFD
    u16 checksum;                   // 0xFE-0xFF
} FarmRankingsBuffer;

extern FarmRankingsBuffer farmRankingsBuffer;

#endif