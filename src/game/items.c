#include "common.h"

#include "game/items.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/fieldObjects.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameFile.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/levelInteractions.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/time.h"

#include "assetIndices/entities.h"
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"
#include "assetIndices/texts.h"

// bss
ToolUse toolUse;
ItemContext itemInfo[10];

// shared bss (load game screen)
u32 totalStrawberriesShipped;
u32 totalTomatoesShipped;
u32 totalCabbageShipped;
u32 totalCornShipped;
u32 totalEggplantsShipped;
u32 totalPotatoesShipped;

u8 tomatoSeedsQuantity;
u8 cabbageSeedsQuantity;
u8 grassSeedsQuantity;
u8 moondropSeedsQuantity;
u8 cornSeedsQuantity;
u8 turnipSeedsQuantity;
u8 potatoSeedsQuantity;
u8 pinkCatMintSeedsQuantity;
u8 strawberrySeedsQuantity;
u8 eggplantSeedsQuantity;

u32 totalTurnipsShipped;

// consumable tool counters (seeds, feed)
u8 blueMistSeedsQuantity;
u16 chickenFeedQuantity;
u16 fodderQuantity;
u8 wateringCanUses;

u8 upgradedToolIndex;
u8 upgradedToolLevelIndex;

// data
// offsets for tool sweeping effects (sickle and watering can)
u8 toolSweepOffsetsLeft[] = { 8, 0, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0 };
u8 toolSweepOffsetsRight[] = { 8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0 }; 

u8 unused[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

u16 itemSpriteAnimations[96][4] = {
    { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
    { 0x0000, 0x003D, 0x003E, 0x003E },
    { 0x0001, 0x0001, 0x0001, 0x0042 },
    { 0x0002, 0x0002, 0x0002, 0x0002 },
    { 0x0003, 0x0003, 0x0003, 0x0003 },
    { 0x0004, 0x0004, 0x0004, 0x0004 },
    { 0x0005, 0x0005, 0x0005, 0x0005 },
    { 0x0006, 0x0006, 0x0006, 0x0006 },
    { 0x0007, 0x0007, 0x0007, 0x0007 },
    { 0x0008, 0x0008, 0x0008, 0x0008 },
    { 0x0009, 0x0009, 0x0009, 0x0009 },
    { 0x000A, 0x000A, 0x000A, 0x000A },
    { 0x000B, 0x000B, 0x000B, 0x000B },
    { 0x000C, 0x000C, 0x000C, 0x000C },
    { 0x000D, 0x000D, 0x000D, 0x000D },
    { 0x000E, 0x000E, 0x000E, 0x000E },
    { 0x000F, 0x000F, 0x000F, 0x000F },
    { 0x0010, 0x0010, 0x0010, 0x0010 },
    { 0x0011, 0x0011, 0x0011, 0x0011 },
    { 0x0012, 0x0012, 0x0012, 0x0012 },
    { 0x0013, 0x0013, 0x0013, 0x0013 },
    { 0x0014, 0x0014, 0x0014, 0x0014 },
    { 0x0015, 0x0015, 0x0015, 0x0015 },
    { 0x0016, 0x0016, 0x0016, 0x0016 },
    { 0x0017, 0x0017, 0x0017, 0x0017 },
    { 0x0018, 0x0018, 0x0018, 0x0018 },
    { 0x0019, 0x0019, 0x0019, 0x0019 },
    { 0x001A, 0x001A, 0x001A, 0x001A },
    { 0x001B, 0x001B, 0x001B, 0x001B },
    { 0x001C, 0x001C, 0x001C, 0x001C },
    { 0x001D, 0x001D, 0x001D, 0x001D },
    { 0x001E, 0x001E, 0x001E, 0x001E },
    { 0x001F, 0x001F, 0x001F, 0x001F },
    { 0x0020, 0x0020, 0x0020, 0x0020 },
    { 0x0021, 0x0021, 0x0021, 0x0021 },
    { 0x0022, 0x0022, 0x0022, 0x0022 },
    { 0x0023, 0x0023, 0x0023, 0x0023 },
    { 0x0024, 0x0024, 0x0024, 0x0024 },
    { 0x0025, 0x0025, 0x0025, 0x0025 },
    { 0x0026, 0x0026, 0x0026, 0x0026 },
    { 0x0027, 0x0027, 0x0027, 0x0027 },
    { 0x0028, 0x0028, 0x0028, 0x0028 },
    { 0x0029, 0x0029, 0x0029, 0x0029 },
    { 0x002A, 0x002A, 0x002A, 0x002A },
    { 0x002B, 0x002B, 0x002B, 0x002B },
    { 0x002C, 0x002C, 0x002C, 0x002C },
    { 0x002D, 0x002D, 0x002D, 0x002D },
    { 0x002E, 0x002E, 0x002E, 0x002E },
    { 0x002F, 0x002F, 0x002F, 0x002F },
    { 0x0030, 0x0030, 0x0030, 0x0030 },
    { 0x0031, 0x0031, 0x0031, 0x0031 },
    { 0x0032, 0x0032, 0x0032, 0x0032 },
    { 0x0033, 0x0033, 0x0033, 0x0033 },
    { 0x0034, 0x0034, 0x0034, 0x0034 },
    { 0x0035, 0x0035, 0x0035, 0x0035 },
    { 0x0036, 0x0036, 0x0036, 0x0036 },
    { 0x0037, 0x0037, 0x0037, 0x0037 },
    { 0x0038, 0x0038, 0x0038, 0x0038 },
    { 0x0039, 0x0039, 0x0039, 0x0039 },
    { 0x003A, 0x003A, 0x003A, 0x003A },
    { 0x003B, 0x003B, 0x003B, 0x0043 },
    { 0x003C, 0x003C, 0x003C, 0x0044 },
    { 0x003F, 0x0040, 0x0041, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000 },
};

// entity metadata struct
//   entity asset index
//   animation index per season
u16 itemEntityMetadata[224][5] = {
    { 0x005D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
    { 0x005D, 0x0000, 0x008B, 0x008C, 0x008C },
    { 0x005D, 0x0001, 0x0001, 0x0001, 0x0001 },
    { 0x005D, 0x0002, 0x0002, 0x0002, 0x0002 },
    { 0x005D, 0x0003, 0x0003, 0x0003, 0x0003 },
    { 0x005D, 0x0004, 0x0004, 0x0004, 0x0004 },
    { 0x005D, 0x0005, 0x0005, 0x0005, 0x0005 },
    { 0x005D, 0x0006, 0x0006, 0x0006, 0x0006 },
    { 0x005D, 0x0007, 0x0007, 0x0007, 0x0007 },
    { 0x005D, 0x0008, 0x0008, 0x0008, 0x0008 },
    { 0x005D, 0x0009, 0x0009, 0x0009, 0x0009 },
    { 0x005D, 0x000A, 0x000A, 0x000A, 0x000A },
    { 0x005D, 0x000B, 0x000B, 0x000B, 0x000B },
    { 0x005D, 0x000C, 0x000C, 0x000C, 0x000C },
    { 0x005D, 0x000D, 0x000D, 0x000D, 0x000D },
    { 0x005D, 0x000E, 0x000E, 0x000E, 0x000E },
    { 0x005D, 0x000F, 0x000F, 0x000F, 0x000F },
    { 0x005D, 0x0010, 0x0010, 0x0010, 0x0010 },
    { 0x005D, 0x0011, 0x0011, 0x0011, 0x0011 },
    { 0x005D, 0x0012, 0x0012, 0x0012, 0x0012 },
    { 0x005D, 0x0013, 0x0013, 0x0013, 0x0013 },
    { 0x005D, 0x0014, 0x0014, 0x0014, 0x0014 },
    { 0x005D, 0x0015, 0x0015, 0x0015, 0x0015 },
    { 0x005D, 0x0016, 0x0016, 0x0016, 0x0016 },
    { 0x005D, 0x0017, 0x0017, 0x0017, 0x0017 },
    { 0x005D, 0x0018, 0x0018, 0x0018, 0x0018 },
    { 0x005D, 0x0019, 0x0019, 0x0019, 0x0019 },
    { 0x005D, 0x001A, 0x001A, 0x001A, 0x001A },
    { 0x005D, 0x001B, 0x001B, 0x001B, 0x001B },
    { 0x005D, 0x001C, 0x001C, 0x001C, 0x001C },
    { 0x005D, 0x001D, 0x001D, 0x001D, 0x001D },
    { 0x005D, 0x001E, 0x001E, 0x001E, 0x001E },
    { 0x005D, 0x001F, 0x001F, 0x001F, 0x001F },
    { 0x005D, 0x0020, 0x0020, 0x0020, 0x0020 },
    { 0x005D, 0x0021, 0x0021, 0x0021, 0x0021 },
    { 0x005D, 0x0022, 0x0022, 0x0022, 0x0022 },
    { 0x005D, 0x0023, 0x0023, 0x0023, 0x0023 },
    { 0x005D, 0x0024, 0x0024, 0x0024, 0x0024 },
    { 0x005D, 0x0025, 0x0025, 0x0025, 0x0025 },
    { 0x005D, 0x0026, 0x0026, 0x0026, 0x0026 },
    { 0x005D, 0x0027, 0x0027, 0x0027, 0x0027 },
    { 0x005D, 0x0028, 0x0028, 0x0028, 0x0028 },
    { 0x005D, 0x0029, 0x0029, 0x0029, 0x0029 },
    { 0x005D, 0x002A, 0x002A, 0x002A, 0x002A },
    { 0x005D, 0x002B, 0x002B, 0x002B, 0x002B },
    { 0x005D, 0x002C, 0x002C, 0x002C, 0x002C },
    { 0x005D, 0x002D, 0x002D, 0x002D, 0x002D },
    { 0x005D, 0x002E, 0x002E, 0x002E, 0x002E },
    { 0x005D, 0x002F, 0x002F, 0x002F, 0x002F },
    { 0x005D, 0x0030, 0x0030, 0x0030, 0x0030 },
    { 0x005D, 0x0031, 0x0031, 0x0031, 0x0031 },
    { 0x005D, 0x0032, 0x0032, 0x0032, 0x0032 },
    { 0x005D, 0x0033, 0x0033, 0x0033, 0x0033 },
    { 0x005D, 0x0034, 0x0034, 0x0034, 0x0034 },
    { 0x005D, 0x0035, 0x0035, 0x0035, 0x0035 },
    { 0x005D, 0x0036, 0x0036, 0x0036, 0x0036 },
    { 0x005D, 0x0037, 0x0037, 0x0037, 0x0037 },
    { 0x005D, 0x0038, 0x0038, 0x0038, 0x0038 },
    { 0x005D, 0x0060, 0x0060, 0x0060, 0x0060 },
    { 0x005D, 0x0061, 0x0061, 0x0061, 0x0061 },
    { 0x005D, 0x0062, 0x0062, 0x0062, 0x0062 },
    { 0x005D, 0x0063, 0x0063, 0x0063, 0x0063 },
    { 0x005D, 0x0064, 0x0064, 0x0064, 0x0064 },
    { 0x005D, 0x0065, 0x0065, 0x0065, 0x0065 },
    { 0x005D, 0x0066, 0x0066, 0x0066, 0x0066 },
    { 0x005D, 0x0067, 0x0067, 0x0067, 0x0067 },
    { 0x005D, 0x0068, 0x0068, 0x0068, 0x0068 },
    { 0x005D, 0x0069, 0x0069, 0x0069, 0x0069 },
    { 0x005D, 0x006A, 0x006A, 0x006A, 0x006A },
    { 0x005D, 0x0082, 0x0082, 0x0082, 0x0082 },
    { 0x005D, 0x0083, 0x0083, 0x0083, 0x0083 },
    { 0x005D, 0x0084, 0x0084, 0x0084, 0x0084 },
    { 0x005D, 0x0085, 0x0085, 0x0085, 0x0085 },
    { 0x005D, 0x0086, 0x0086, 0x0086, 0x0086 },
    { 0x005D, 0x0087, 0x0087, 0x0087, 0x0087 },
    { 0x005D, 0x0088, 0x0088, 0x0088, 0x0088 },
    { 0x005D, 0x0089, 0x0089, 0x0089, 0x0089 },
    { 0x005D, 0x008A, 0x008A, 0x008A, 0x008A },
    { 0x005D, 0x009B, 0x009B, 0x009B, 0x009B },
    { 0x005D, 0x009C, 0x009C, 0x009C, 0x009C },
    { 0x005D, 0x0039, 0x0039, 0x0039, 0x0039 },
    { 0x005D, 0x003A, 0x003A, 0x003A, 0x003A },
    { 0x005D, 0x003B, 0x003B, 0x003B, 0x003B },
    { 0x005D, 0x003C, 0x003C, 0x003C, 0x003C },
    { 0x005D, 0x003D, 0x003D, 0x003D, 0x003D },
    { 0x005D, 0x003E, 0x003E, 0x003E, 0x003E },
    { 0x005D, 0x003F, 0x003F, 0x003F, 0x003F },
    { 0x005D, 0x00D2, 0x00D2, 0x00D2, 0x00D2 },
    { 0x005D, 0x0040, 0x0040, 0x0040, 0x0040 },
    { 0x005D, 0x0041, 0x0041, 0x0041, 0x0041 },
    { 0x005D, 0x0042, 0x0042, 0x0042, 0x0042 },
    { 0x005D, 0x0043, 0x0043, 0x0043, 0x0043 },
    { 0x005D, 0x0044, 0x0044, 0x0044, 0x0044 },
    { 0x005D, 0x0045, 0x0045, 0x0045, 0x0045 },
    { 0x005D, 0x0046, 0x0046, 0x0046, 0x0046 },
    { 0x005D, 0x0047, 0x0047, 0x0047, 0x0047 },
    { 0x005D, 0x0050, 0x0050, 0x0050, 0x0050 },
    { 0x005D, 0x0051, 0x0051, 0x0051, 0x0051 },
    { 0x005D, 0x0052, 0x0052, 0x0052, 0x0052 },
    { 0x005D, 0x0053, 0x0053, 0x0053, 0x0053 },
    { 0x005D, 0x0054, 0x0054, 0x0054, 0x0054 },
    { 0x005D, 0x0055, 0x0055, 0x0055, 0x0055 },
    { 0x005D, 0x0056, 0x0056, 0x0056, 0x0056 },
    { 0x005D, 0x0057, 0x0057, 0x0057, 0x0057 },
    { 0x005D, 0x0058, 0x0058, 0x0058, 0x0058 },
    { 0x005D, 0x0059, 0x0059, 0x0059, 0x0059 },
    { 0x005D, 0x005A, 0x005A, 0x005A, 0x005A },
    { 0x005D, 0x005B, 0x005B, 0x005B, 0x005B },
    { 0x005D, 0x005C, 0x005C, 0x005C, 0x005C },
    { 0x005D, 0x005D, 0x005D, 0x005D, 0x005D },
    { 0x005D, 0x005E, 0x005E, 0x005E, 0x005E },
    { 0x005D, 0x005F, 0x005F, 0x005F, 0x005F },
    { 0x005D, 0x0070, 0x0070, 0x0070, 0x0070 },
    { 0x005D, 0x008D, 0x008D, 0x008D, 0x008D },
    { 0x005D, 0x006D, 0x006D, 0x006D, 0x006D },
    { 0x005D, 0x006F, 0x006F, 0x006F, 0x006F },
    { 0x005D, 0x006E, 0x006E, 0x006E, 0x006E },
    { 0x005D, 0x006E, 0x006E, 0x006E, 0x006E },
    { 0x005D, 0x0071, 0x0071, 0x0071, 0x0071 },
    { 0x005D, 0x006E, 0x006E, 0x006E, 0x006E },
    { 0x005D, 0x006D, 0x006D, 0x006D, 0x006D },
    { 0x005D, 0x006D, 0x006D, 0x006D, 0x006D },
    { 0x005D, 0x006E, 0x006E, 0x006E, 0x006E },
    { 0x005D, 0x0048, 0x0048, 0x0048, 0x0048 },
    { 0x005D, 0x0049, 0x0049, 0x0049, 0x0049 },
    { 0x005D, 0x004A, 0x004A, 0x004A, 0x004A },
    { 0x005D, 0x004B, 0x004B, 0x004B, 0x004B },
    { 0x005D, 0x004C, 0x004C, 0x004C, 0x004C },
    { 0x005D, 0x004D, 0x004D, 0x004D, 0x004D },
    { 0x005D, 0x004E, 0x004E, 0x004E, 0x004E },
    { 0x005D, 0x004F, 0x004F, 0x004F, 0x004F },
    { 0x005D, 0x00C3, 0x00C3, 0x00C3, 0x00C3 },
    { 0x005D, 0x00C4, 0x00C4, 0x00C4, 0x00C4 },
    { 0x005D, 0x00C5, 0x00C5, 0x00C5, 0x00C5 },
    { 0x005D, 0x00C6, 0x00C6, 0x00C6, 0x00C6 },
    { 0x005D, 0x00C7, 0x00C7, 0x00C7, 0x00C7 },
    { 0x005D, 0x00C8, 0x00C8, 0x00C8, 0x00C8 },
    { 0x005D, 0x00C9, 0x00C9, 0x00C9, 0x00C9 },
    { 0x005D, 0x00CA, 0x00CA, 0x00CA, 0x00CA },
    { 0x005D, 0x00A3, 0x00A3, 0x00A3, 0x00A3 },
    { 0x005D, 0x00A4, 0x00A4, 0x00A4, 0x00A4 },
    { 0x005D, 0x00A5, 0x00A5, 0x00A5, 0x00A5 },
    { 0x005D, 0x00A6, 0x00A6, 0x00A6, 0x00A6 },
    { 0x005D, 0x00A7, 0x00A7, 0x00A7, 0x00A7 },
    { 0x005D, 0x00A8, 0x00A8, 0x00A8, 0x00A8 },
    { 0x005D, 0x00A9, 0x00A9, 0x00A9, 0x00A9 },
    { 0x005D, 0x00AA, 0x00AA, 0x00AA, 0x00AA },
    { 0x005D, 0x00AB, 0x00AB, 0x00AB, 0x00AB },
    { 0x005D, 0x00AC, 0x00AC, 0x00AC, 0x00AC },
    { 0x005D, 0x00AD, 0x00AD, 0x00AD, 0x00AD },
    { 0x005D, 0x00AE, 0x00AE, 0x00AE, 0x00AE },
    { 0x005D, 0x00AF, 0x00AF, 0x00AF, 0x00AF },
    { 0x005D, 0x00B0, 0x00B0, 0x00B0, 0x00B0 },
    { 0x005D, 0x00B1, 0x00B1, 0x00B1, 0x00B1 },
    { 0x005D, 0x00B2, 0x00B2, 0x00B2, 0x00B2 },
    { 0x005D, 0x00B3, 0x00B3, 0x00B3, 0x00B3 },
    { 0x005D, 0x00B4, 0x00B4, 0x00B4, 0x00B4 },
    { 0x005D, 0x00B5, 0x00B5, 0x00B5, 0x00B5 },
    { 0x005D, 0x00B6, 0x00B6, 0x00B6, 0x00B6 },
    { 0x005D, 0x00B7, 0x00B7, 0x00B7, 0x00B7 },
    { 0x005D, 0x00B8, 0x00B8, 0x00B8, 0x00B8 },
    { 0x005D, 0x00B9, 0x00B9, 0x00B9, 0x00B9 },
    { 0x005D, 0x00BA, 0x00BA, 0x00BA, 0x00BA },
    { 0x005D, 0x00BB, 0x00BB, 0x00BB, 0x00BB },
    { 0x005D, 0x00BC, 0x00BC, 0x00BC, 0x00BC },
    { 0x005D, 0x00BD, 0x00BD, 0x00BD, 0x00BD },
    { 0x005D, 0x00BE, 0x00BE, 0x00BE, 0x00BE },
    { 0x005D, 0x00BF, 0x00BF, 0x00BF, 0x00BF },
    { 0x005D, 0x00C0, 0x00C0, 0x00C0, 0x00C0 },
    { 0x005D, 0x00C1, 0x00C1, 0x00C1, 0x00C1 },
    { 0x005D, 0x00C2, 0x00C2, 0x00C2, 0x00C2 },
    { 0x005D, 0x0040, 0x0040, 0x0040, 0x0040 },
    { 0x005D, 0x0041, 0x0041, 0x0041, 0x0041 },
    { 0x005D, 0x0042, 0x0042, 0x0042, 0x0042 },
    { 0x005D, 0x0043, 0x0043, 0x0043, 0x0043 },
    { 0x005D, 0x0044, 0x0044, 0x0044, 0x0044 },
    { 0x005D, 0x0045, 0x0045, 0x0045, 0x0045 },
    { 0x005D, 0x0046, 0x0046, 0x0046, 0x0046 },
    { 0x005D, 0x0047, 0x0047, 0x0047, 0x0047 },
    { 0x005D, 0x00CB, 0x00CB, 0x00CB, 0x00CB },
    { 0x005D, 0x00CC, 0x00CC, 0x00CC, 0x00CC },
    { 0x005D, 0x00CD, 0x00CD, 0x00CD, 0x00CD },
    { 0x005D, 0x00CE, 0x00CE, 0x00CE, 0x00CE },
    { 0x005D, 0x00CF, 0x00CF, 0x00CF, 0x00CF },
    { 0x005D, 0x00D0, 0x00D0, 0x00D0, 0x00D0 },
    { 0x005D, 0x00D1, 0x00D1, 0x00D1, 0x00D1 },
    { 0x005D, 0x00D5, 0x00D5, 0x00D5, 0x00D5 },
    { 0x005D, 0x00D6, 0x00D6, 0x00D6, 0x00D6 },
    { 0x005D, 0x00D7, 0x00D7, 0x00D7, 0x00D7 },
    { 0x005D, 0x00D8, 0x00D8, 0x00D8, 0x00D8 },
    { 0x005D, 0x00D9, 0x00D9, 0x00D9, 0x00D9 },
    { 0x005D, 0x00DA, 0x00DA, 0x00DA, 0x00DA },
    { 0x005D, 0x00DB, 0x00DB, 0x00DB, 0x00DB },
    { 0x005D, 0x00DC, 0x00DC, 0x00DC, 0x00DC },
    { 0x005D, 0x00DD, 0x00DD, 0x00DD, 0x00DD },
    { 0x005D, 0x00DE, 0x00DE, 0x00DE, 0x00DE },
    { 0x005D, 0x00DF, 0x00DF, 0x00DF, 0x00DF },
    { 0x005D, 0x00E0, 0x00E0, 0x00E0, 0x00E0 },
    { 0x005D, 0x00E1, 0x00E1, 0x00E1, 0x00E1 },
    { 0x005D, 0x00E2, 0x00E2, 0x00E2, 0x00E2 },
    { 0x005D, 0x00E3, 0x00E3, 0x00E3, 0x00E3 },
    { 0x005D, 0x00E4, 0x00E4, 0x00E4, 0x00E4 },
    { 0x005D, 0x00E7, 0x00E7, 0x00E7, 0x00E7 },
    { 0x005D, 0x00EA, 0x00EA, 0x00EA, 0x00EA },
    { 0x005D, 0x00EB, 0x00EB, 0x00EB, 0x00EB },
    { 0x005D, 0x00EC, 0x00EC, 0x00EC, 0x00EC },
    { 0x005D, 0x00ED, 0x00ED, 0x00ED, 0x00ED },
    { 0x005D, 0x00EE, 0x00EE, 0x00EE, 0x00EE },
    { 0x005D, 0x00EF, 0x00EF, 0x00EF, 0x00EF },
    { 0x005D, 0x00F0, 0x00F0, 0x00F0, 0x00F0 },
    { 0x005D, 0x00F1, 0x00F1, 0x00F1, 0x00F1 },
    { 0x005D, 0x00F2, 0x00F2, 0x00F2, 0x00F2 },
    { 0x005D, 0x00F3, 0x00F3, 0x00F3, 0x00F3 },
    { 0x005D, 0x00F4, 0x00F4, 0x00F4, 0x00F4 },
    { 0x005D, 0x00F5, 0x00F5, 0x00F5, 0x00F5 },
    { 0x005D, 0x00F6, 0x00F6, 0x00F6, 0x00F6 },
    { 0x005D, 0x00F7, 0x00F7, 0x00F7, 0x00F7 },
    { 0x005D, 0x00F8, 0x00F8, 0x00F8, 0x00F8 },
    { 0x005D, 0x0090, 0x0090, 0x0090, 0x0090 },
    { 0x005D, 0x0095, 0x0095, 0x0095, 0x0095 },
    { 0x005D, 0x0093, 0x0093, 0x0093, 0x0093 },
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
    { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
};

// entity palette index overrides (0xFFFF = use default)
u16 itemPaletteOverrides[] = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0x0096, 0x0096, 0x0096, 0x0096, 0x0096, 
    0x0096, 0x0096, 0x0096, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0000
};

u8 heldItemIndices[] = {
    0x00, 0x30, 0x2F, 0x2A, 0x0D, 0x0E, 0x00, 0x0F, 0x10, 0x11, 0x32, 0x00, 0x00, 0x01, 0x02, 0x03, 
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 
    0x31, 0x00, 0x19, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x1A, 0x20, 0x21, 0x00, 0x23, 0x22, 0x2B, 0x2D, 
    0x2C, 0x2E, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 
    0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x41, 0x41, 0x41, 0x41, 
    0x41, 0x41, 0x41, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x42, 0x42, 0x42, 0x42, 
    0x42, 0x42, 0x42, 0x39, 0x3A, 0x38, 0x35, 0x36, 0x3B, 0x3C, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 
    0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u16 itemTextIndices[] = {
    0x0000, 0x013B, 0x013A, 0x0135, 0x011D, 0x011E, 0x0000, 0x011F, 
    0x0120, 0x0121, 0x013D, 0x0000, 0x0000, 0x0111, 0x0112, 0x0113, 
    0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0119, 0x011A, 0x011B, 
    0x011C, 0x0122, 0x0123, 0x0124, 0x0125, 0x0126, 0x0127, 0x0128, 
    0x013C, 0x0000, 0x0129, 0x012B, 0x012C, 0x012D, 0x012E, 0x012F, 
    0x012A, 0x0130, 0x0131, 0x014E, 0x0133, 0x0132, 0x0136, 0x0138, 
    0x0137, 0x0139, 0x014D, 0x0000, 0x0000, 0x014F, 0x0000, 0x0000, 
    0x0000, 0x0134, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x013E, 0x013E, 0x013E, 0x013E, 0x013E, 0x013E, 0x013E, 0x013E, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0150, 0x0150, 0x0150, 0x0150, 0x0150, 
    0x0150, 0x0150, 0x0150, 0x014A, 0x014A, 0x014A, 0x014A, 0x014A, 
    0x014A, 0x014A, 0x014A, 0x0149, 0x0149, 0x0149, 0x0149, 0x0149, 
    0x0149, 0x0149, 0x0149, 0x014B, 0x014B, 0x014B, 0x014B, 0x014B, 
    0x014B, 0x014B, 0x014B, 0x0148, 0x0148, 0x0148, 0x0148, 0x0148, 
    0x0148, 0x0148, 0x0148, 0x014C, 0x014C, 0x014C, 0x014C, 0x014C, 
    0x014C, 0x014C, 0x014C, 0x0151, 0x0151, 0x0151, 0x0151, 0x0151, 
    0x0151, 0x0151, 0x0151, 0x0144, 0x0145, 0x0143, 0x0140, 0x0141, 
    0x0146, 0x0147, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 
    0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 0x013F, 
    0x013F, 0x013F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
};

u16 itemShippingValues[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x003C, 0x0050, 0x005A, 
    0x005A, 0x0078, 0x003C, 0x0096, 0x0032, 0x0064, 0x0096, 0x012C, 
    0x01F4, 0x0384, 0x0708, 0x0032, 0x0028, 0x0046, 0x0028, 0x003C, 
    0x0064, 0x0000, 0x012C, 0x0046, 0x001E, 0x001E, 0x0064, 0x00B4, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0064, 0x02BC, 
    0x03E8, 0x01F4, 0x0320, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
};

u16 itemFlags[] = {
    0x0000, 0x100A, 0x101A, 0x301A, 0x100A, 0x100A, 0x0000, 0x020B,
    0x020B, 0x020B, 0x0000, 0x0000, 0x0000, 0x820F, 0x820F, 0x820F,
    0x820F, 0x820F, 0x820F, 0x820F, 0x822F, 0x900E, 0x900E, 0x900E,
    0x900E, 0x100E, 0x100E, 0x020F, 0x020F, 0x020F, 0x020F, 0x020F,
    0x020F, 0x0000, 0x020F, 0x020F, 0x020F, 0x020F, 0x020F, 0x020F,
    0x020B, 0x020B, 0x020B, 0x0201, 0x020B, 0x020B, 0x100E, 0x100E,
    0x100E, 0x100E, 0x100E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x104A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0000, 0x0000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x4000, 0x4000, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0088,
    0x0088, 0x0088, 0x0088, 0x0088, 0x0088, 0x0108, 0x0108, 0x0108,
    0x0088, 0x0088, 0x0408, 0x0408, 0x0408, 0x0408, 0x0408, 0x0408,
    0x0408, 0x0408, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808, 0x0808,
    0x0808, 0x0808, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

u8 itemStaminaRestorationValue[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x00, 0x00, 
    0x00, 0x08, 0x0A, 0x08, 0x08, 0x0A, 0x08, 0x0C, 0x08, 0x08, 0x0C, 0x10, 
    0x14, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x14, 0x00, 
    0x06, 0x06, 0x08, 0x10, 0x02, 0x08, 0x08, 0x08, 0x02, 0x02, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08, 0x00, 0x00, 0x00, 0x0C, 0xC8, 
    0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 itemFatigueReductionValue[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x14, 0x0A, 0x1E, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


// rodata
static const u8 D_80122340[];
static const u8 D_80122344[];
static const u8 D_80122348[];
static const u8 D_8012234C[];
static const u8 D_80122350[];
static const u8 D_8012235C[];
static const u8 D_80122368[];
static const u8 D_80122374[];


// forward declarations
bool processStumpHit(u8 arg0, s16 arg1, s16 arg2);
bool processBoulderHit(u8 arg0, s16 arg1, s16 arg2);          
void handleBlueFeatherUse();         
void useSickle();
void useHoe();
void useAx();
void useHammer();
void useWateringCan();
void useMilker();
void useCowBell();
void useBrush();
void useClippers();
void useTurnipSeeds(void);
void usePotatoSeeds();
void useCabbageSeeds();
void useTomatoSeeds();
void useCornSeeds();
void useEggplantSeeds();
void useStrawberrySeeds();
void useMoonDropSeeds();
void usePinkCatMintSeeds();
void useBlueMistSeeds();
void useChickenFeed();
void useBall();
void useFeedingBottle();
void useUnknownTool();
void useFishingPole();
void useMiraclePotion();
void useCowMedicine();
void useGrassSeeds();
void useEmptyBottle();
u8 allocateGroundItemSlot(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4);
void setItemPosition(u8 index, f32 arg1, f32 arg2, f32 arg3);
void loadHeldItemEntity(u8, u16);   
bool handlePutDownHeldItem(u8 itemIndex);
void processItemShipping(u8 index);
void handleItemDroppedInWater(u8 arg0, u8 index);
u8 getHeldItemIndex(u8);
u16 getItemFlags(u16 index);


//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", resetToolUseState);

void resetToolUseState(void) {
    toolUse.unk_0 = 0;
    toolUse.stepIndex = 0;
    toolUse.toolUseState = 2;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", getToolLevel);

u8 getToolLevel(u8 tool) {

    switch (tool) {
        case SICKLE:
            return gPlayer.toolLevels[0];
        case HOE:
            return gPlayer.toolLevels[1];
        case AX:
            return gPlayer.toolLevels[2];
        case HAMMER:
            return gPlayer.toolLevels[3];
        case WATERING_CAN:
            return gPlayer.toolLevels[4];
        default:
            return 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", spawnToolEffectEntity);

void spawnToolEffectEntity(u8 itemOffset, u8 animationIndex, f32 x, f32 y, f32 z) {

    loadEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, ENTITY_ASSET_HOLDABLE_ITEMS_4, 1);
    setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemOffset, 0xFFFF, 0xFF);
    setEntityCollidable(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityYMovement(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityCoordinates(ENTITY_ITEM_BASE_INDEX + itemOffset, x, y, z);
    setEntityColor(ENTITY_ITEM_BASE_INDEX + itemOffset, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemOffset, animationIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", spawnFishingRodEntity);

void spawnFishingRodEntity(u8 itemOffset, u8 animationIndex, f32 x, f32 y, f32 z) {

    loadEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, ENTITY_ASSET_HOLDABLE_ITEMS_2, 1);
    setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemOffset, 0xFFFF, 0xFF);
    setEntityCollidable(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityYMovement(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityCoordinates(ENTITY_ITEM_BASE_INDEX + itemOffset, x, y, z);
    setEntityColor(ENTITY_ITEM_BASE_INDEX + itemOffset, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemOffset, animationIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", setItemEntityAnimation);

void setItemEntityAnimation(u8 itemOffset, u8 animationIndex) {
    setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemOffset, animationIndex);
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", deactivateItemEntity);

void deactivateItemEntity(u8 offset) {
    deactivateEntity(ENTITY_ITEM_BASE_INDEX + offset);
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", processStumpHit);

bool processStumpHit(u8 groundObjectIndex, s16 x, s16 z) {

    bool result = FALSE;

    s16 temp;
    s16 temp2;
    
    switch (groundObjectIndex) {

        case STUMP:
            temp = x;
            temp2 = z;
            break;
        case 0xC9:
            temp = x - 1;
            temp2 = z;
            break;
        case 0xCA:
            temp = x;
            temp2 = z - 1;
            break;
        case 0xCB:
            temp = x - 1;
            temp2 = z - 1;
            break;
 
    }

    if (toolUse.stumpHitPositionX != temp || toolUse.stumpHitPositionZ != temp2) {
        toolUse.stumpHitPositionX = temp;
        toolUse.stumpHitPositionZ = temp2;
        toolUse.stumpHitCounter = 0;
    }

    switch (gPlayer.currentToolLevel) {

        case 0:
            toolUse.stumpHitCounter += 1;
            break;
        
        case 1:
            toolUse.stumpHitCounter += 3;
            break;
        
        case 2:
            toolUse.stumpHitCounter += 6;
            break;
        
    }
    
    // break stump after multiple hits
    if (toolUse.stumpHitCounter >= 6) {
        toolUse.stumpHitCounter = 0;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", processBoulderHit);

bool processBoulderHit(u8 groundObjectIndex, s16 x, s16 z) {
    
    bool result = FALSE;

    s16 temp;
    s16 temp2;
    
    switch (groundObjectIndex) {

        case BOULDER:
            temp = x;
            temp2 = z;
            break;
        case 0xC5:
            temp = x - 1;
            temp2 = z;
            break;
        case 0xC6:
            temp = x;
            temp2 = z - 1;
            break;
        case 0xC7:
            temp = x - 1;
            temp2 = z - 1;
            break;
 
    }

    if (toolUse.boulderHitPositionX != temp || toolUse.boulderHitPositionZ != temp2) {
        toolUse.boulderHitPositionX = temp;
        toolUse.boulderHitPositionZ = temp2;
        toolUse.boulderHitCounter = 0;
    }

    switch (gPlayer.currentToolLevel) {

        case 0:
            toolUse.boulderHitCounter += 1;
            break;
        
        case 1:
            toolUse.boulderHitCounter += 3;
            break;
        
        case 2:
            toolUse.boulderHitCounter += 6;
            break;
        
    }
    
    // break boulder after multiple hits
    if (toolUse.boulderHitCounter >= 6) {
        toolUse.boulderHitCounter = 0;
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleMusicBoxDigging);

inline u8 handleMusicBoxDigging(s16 arg0, s16 arg1) {
    
    if (gBaseMapIndex == FARM) {
         
        setDailyEventBit(0x29); 
        
        if (arg0 == 12 && arg1 == 2) {
            toolUse.musicBoxTileDigCounter++;
        } else {
            toolUse.musicBoxTileDigCounter = 0;    
        }
        
    } else {
        toolUse.musicBoxTileDigCounter = 0;
    }
    
    return toolUse.musicBoxTileDigCounter;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleToolUse);

void handleToolUse(void) {

    switch (gPlayer.currentTool) {
        case 0:
            return;
        case SICKLE:
            useSickle();
            break;
        case HOE:
            useHoe();
            break;
        case AX:
            useAx();
            break;
        case HAMMER:
            useHammer();
            break;
        case WATERING_CAN:
            useWateringCan();
            break;
        case MILKER:
            useMilker();
            break;
        case BELL:
            useCowBell();
            break;
        case BRUSH:
            useBrush();
            break;
        case CLIPPERS:
            useClippers();
            break;
        case TURNIP_SEEDS:
            useTurnipSeeds();
            break;
        case POTATO_SEEDS:
            usePotatoSeeds();
            break;
        case CABBAGE_SEEDS:
            useCabbageSeeds();
            break;
        case TOMATO_SEEDS:
            useTomatoSeeds();
            break;
        case CORN_SEEDS:
            useCornSeeds();
            break;
        case EGGPLANT_SEEDS:
            useEggplantSeeds();
            break;
        case STRAWBERRY_SEEDS:
            useStrawberrySeeds();
            break;
        case MOON_DROP_SEEDS:
            useMoonDropSeeds();
            break;
        case PINK_CAT_MINT_SEEDS:
            usePinkCatMintSeeds();
            break;
        case BLUE_MIST_SEEDS:
            useBlueMistSeeds();
            break;
        case CHICKEN_FEED:
            useChickenFeed();
            break;
        case BALL:
            useBall();
            break;
        case FEEDING_BOTTLE:
            useFeedingBottle();
            break;
        case 23:
            useUnknownTool();
            break;
        case FISHING_POLE:
            useFishingPole();
            break;
        case MIRACLE_POTION:
            useMiraclePotion();
            break;
        case COW_MEDICINE:
            useCowMedicine();
            break;
        case GRASS_SEEDS:
            useGrassSeeds();
            break;
        case BLUE_FEATHER:
            handleBlueFeatherUse();
            break;
        case EMPTY_BOTTLE:
            useEmptyBottle();
            break;
        default:
            return;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", processToolUseState);

void processToolUseState(void) {

    if (toolUse.toolUseState) {

        switch (toolUse.toolUseState) {        
            case 2:
                handleToolUse();
            case 3:
                break;
            case 4:
            default:
                break;

        }

    }

}

// alternate for reference
// void processToolUseState(void) {

//     int temp = toolUse.toolUseState; 

//     if (temp) {
//         if (temp != 3) {
//             if (temp < 4) {
//                 if (temp == 2) {
//                     handleToolUse();
//                 }       
//             }
//         }
//     }
// }

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D0360);

// sickle
void useSickle(void) {

    Vec3f vec;
    
    f32 tempF;
    u8 temp1, temp2;
    u8 temp3, temp4;
    s32 tempX, tempZ;
    s32 direction;
    u8 currentGroundObjectIndex;
    u8 groundObjectIndex;

    u8 arr1[3];
    u8 arr2[3];
    u8 arr3[3];
    u8 arr4[3];
    u8 arr5[12];
    u8 arr6[8];
    
    u32 padding[2];

    memcpy(arr1, D_80122340, 3);
    memcpy(arr2, D_80122344, 3);
    memcpy(arr3, D_80122348, 3);
    memcpy(arr4, D_8012234C, 3);
    memcpy(arr5, D_80122350, 9);
    memcpy(arr6, D_8012235C, 9);

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);

    switch (gPlayer.currentToolLevel) {

        case 0:
            temp1 = 1;
            temp2 = 8;
            break;
        
        case 1:
            
            temp1 = 1;
            
            if (direction & 1) {
                
                if (entities[ENTITY_PLAYER].direction < 5) {
                    temp2 = arr3[toolUse.stepIndex];
                } else {
                    temp2 = arr4[toolUse.stepIndex]; 
                }
                
            } else if (entities[ENTITY_PLAYER].direction < 5) {
                temp2 = arr1[toolUse.stepIndex];
            } else {
                temp2 = arr2[toolUse.stepIndex];
            }
            
            break;
        
        case 2:
            
            temp1 = 0;

            if ((u8)direction < 5) {
                temp2 = arr5[toolUse.stepIndex];
            } else {
                temp2 = arr6[toolUse.stepIndex];
            }
            
            break;
        
    }

    tempF = (f32)temp1;
            
    vec = getOffsetTileCoordinates(tempF, temp2);
    currentGroundObjectIndex = getGroundObjectIndexFromPlayerPosition(tempF, temp2);
    
    if ((getGroundObjectToolInteractionFlags(currentGroundObjectIndex) & GROUND_OBJECT_CUTTABLE) && vec.y != 65535.0f && (gBaseMapIndex == GREENHOUSE || gSeason != WINTER)) {

        if (currentGroundObjectIndex == GRASS_RIPE) {
            
            groundObjectIndex = GRASS_CUT;
            fodderQuantity += adjustValue(fodderQuantity, 1, MAX_FODDER);
            temp4 = 62;
            
        } else {

            switch (getGroundObjectMapAdditionIndex(currentGroundObjectIndex)) {

                case 4:
                    groundObjectIndex = TILLED;
                    break;
                case 5:
                    groundObjectIndex = TILLED_WATERED;
                    break;
                default:
                    groundObjectIndex = BASE_TILE;
                    break;
                
            }         

            temp4 = 1;
            
        }

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
           
        temp3 = allocateGroundItemSlot(1, ITEM_STATE_THROW_LANDED, temp4, 0, 8);

        vec = getGroundObjectWorldPosition(0, (u8)vec.x, (u8)vec.z);
        setItemPosition(temp3, vec.x, vec.y, vec.z);

    }

    if (toolUse.stepIndex == 0) {

        switch (gPlayer.currentToolLevel) {

            case 0:
                playSfx(SICKLE_SFX);
                break;
            case 1:
                playSfx(SILVER_SICKLE_SFX);
                break;
            case 2:
                playSfx(GOLDEN_SICKLE_SFX );
                break;
            
        }
        
    }

    toolUse.stepIndex++;
    
    switch (gPlayer.currentToolLevel) {
            
        case 0:
            toolUse.toolUseState = 0;
            break;
        
        case 1:

            if (toolUse.stepIndex == 3) {
                toolUse.toolUseState = 0;
            }
    
            break;
        
        case 2:

            if (toolUse.stepIndex == 9) {
                toolUse.toolUseState = 0;
            }
            
            break;
            
    }

    handleHitFarmAnimalWithTool();
    handleHitChickenWithTool();
    handleHitDogWithTool();
    handleHitHorseWithTool();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D093C);

void useHoe(void) {

    Vec3f vec;
    f32 tempF;
    u8 temp;
    s32 tempX, tempZ;
    
    u8 direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);

    switch (gPlayer.currentToolLevel) {

        case 0:
            temp = 1;
            break;
        case 1:
            temp = toolUse.stepIndex + 1;
            break;
        case 2:
            temp = toolUse.stepIndex + 1;
            break;
        
    }

    tempF = temp;

    vec = getOffsetTileCoordinates(tempF, 8);

    if (vec.y != 65535.0f) {
        
        if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(tempF, 8)) & GROUND_OBJECT_TILLABLE) && (gBaseMapIndex == GREENHOUSE || gBaseMapIndex == MINE || gBaseMapIndex == MINE_2 || gSeason != WINTER)) {

            addGroundObjectToMap(gBaseMapIndex, TILLED, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
            
            vec = getGroundObjectWorldPosition(0, (u8)vec.x, (u8)vec.z);
            
            spawnToolEffectEntity(toolUse.stepIndex, 0, vec.x, vec.y, vec.z);

            if (toolUse.stepIndex == 0) {

                tempX = (s16)vec.x / 32;
                tempZ = (s16)vec.z / 32;

                handleMusicBoxDigging(tempX, tempZ);
                playSfx(HOE_SFX);
                
            } else {
                playSfx(HOE_SFX);
            }
            
        } else {
            toolUse.toolUseState = 0;
            playSfx(0x1B);
        }

        toolUse.stepIndex++;

        switch (gPlayer.currentToolLevel) {
            
            case 0:
                toolUse.toolUseState = 0;
                break;
            
            case 1:

                if (!checkDailyEventBit(0x4A)) {

                    if (direction & 1) {

                        if (toolUse.stepIndex == 2) {
                            toolUse.toolUseState = 0;
                        }
                        
                    } else {
                        if (toolUse.stepIndex == 3) {
                            toolUse.toolUseState = 0;
                        }
                    }
                    
                } else {
                    toolUse.toolUseState = 0;
                }

                break;
            
            case 2:
                
                if (!checkDailyEventBit(0x4A)) {

                    if (direction & 1) {

                        if (toolUse.stepIndex == 4) {
                            toolUse.toolUseState = 0;
                        }
                        
                    } else {
                        if (toolUse.stepIndex == 6) {
                            toolUse.toolUseState = 0;
                        }
                    }
                    
                } else {
                    toolUse.toolUseState = 0;
                }

                break;
            
        }

    } else {
        toolUse.toolUseState = 0;
        playSfx(27);
    }

    handleHitFarmAnimalWithTool();
    handleHitChickenWithTool();
    handleHitDogWithTool();
    handleHitHorseWithTool();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D0DD4);

void useAx(void) {
    
    Vec3f vec;
    Vec3f vec2;
    u8 groundObjectIndex;

    vec = getOffsetTileCoordinates(1.0f, 8);

    if (vec.y != 65535.0f) {

        groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8);

        if (getGroundObjectToolInteractionFlags(groundObjectIndex) & GROUND_OBJECT_CHOPPABLE) {

            if (processStumpHit(groundObjectIndex, (s16)vec.x / 32, (s16)vec.z / 32)) {

                vec2.y = 0.0f;

                switch (groundObjectIndex) {

                    case STUMP:
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ + 1);

                        vec2.x = 16.0f;
                        vec2.z = 16.0f;

                        break;

                    case 0xC9:
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ + 1);

                        vec2.x = -16.0f;
                        vec2.z = 16.0f;
                        
                        break;
                    
                    case 0xCA:
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ - 1);

                        vec2.x = 16.0f;
                        vec2.z = -16.0f;

                        
                        break;
                    
                    case 0xCB:
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                        addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ - 1);
    
                        vec2.x = -16.0f;
                        vec2.z = -16.0f;
    
                        break;
                    
                }

                groundObjectIndex = allocateGroundItemSlot(1, ITEM_STATE_THROW_LANDED, 0x3D, 0, 8);

                vec = getGroundObjectWorldPosition(MAIN_MAP_INDEX, (u8)vec.x, (u8)vec.z);
                vec.x += vec2.x;
                vec.z += vec2.z;
                
                setItemPosition(groundObjectIndex, vec.x, vec.y, vec.z);
                gLumber += adjustValue(gLumber, 6, MAX_LUMBER);
                
            }
            
        } else {
            toolUse.toolUseState = 0;
        }

        if (toolUse.stepIndex == 0) {

            switch (gPlayer.currentToolLevel) {

                case 0:
                    playSfx(AX_SFX);
                    break;
                case 1:
                    playSfx(SILVER_AX_SFX);
                    break;
                case 2:
                    playSfx(GOLDEN_AX_SFX);
                    break;
                
            }
            
        }

        toolUse.stepIndex++;

        switch (gPlayer.currentToolLevel) {
        
            case 0:
                toolUse.toolUseState = 0;
                break;
            
            case 1:
                toolUse.toolUseState = 0;
                break;
            
            case 2:
                toolUse.toolUseState = 0;
                break;

        }
        
    } else {
        toolUse.toolUseState = 0;
    }

    handleHitFarmAnimalWithTool();
    handleHitChickenWithTool();
    handleHitDogWithTool();
    handleHitHorseWithTool();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D1DB0);

void useHammer(void) {

    Vec3f vec;
    Vec3f vec2;
    u8 temp;
    s16 tempX, tempZ;
    u8 groundObjectIndex;

    vec = getOffsetTileCoordinates(1.0f, 8);
    
    if (vec.y != 65535.0f) {
        
        groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8);
    
        if (getGroundObjectToolInteractionFlags(groundObjectIndex) & GROUND_OBJECT_HAMMERABLE) {

            vec2.y = 0.0f;

            switch (groundObjectIndex) {

                case SMALL_ROCK:

                    addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                    
                    vec2.x = 0.0f;
                    vec2.z = 0.0f;
                    
                    toolUse.boulderHitCounter = 0;
                    
                    groundObjectIndex = allocateGroundItemSlot(1, 12, 2, 0, 8);
                    
                    vec = getGroundObjectWorldPosition(MAIN_MAP_INDEX, (u8)vec.x, (u8)vec.z);
                    vec.x += vec2.x;
                    vec.z += vec2.z;
                
                    setItemPosition(groundObjectIndex, vec.x, vec.y, vec.z);
                    break;

                case TILLED:
                case TILLED_WATERED:
                case BROKEN_LOG:
                    addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                    break;
                
                case BOULDER ... 0xC7:
                    
                    if (processBoulderHit(groundObjectIndex, (s16)vec.x / 32, (s16)vec.z / 32)) {

                        switch (groundObjectIndex) {

                            case BOULDER:
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ + 1);

                                vec2.x = 16.0f;
                                vec2.z = 16.0f;

                                temp = 60;
                                
                                break;

                            case 0xC5:
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ + 1);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ + 1);

                                vec2.x = -16.0f;
                                vec2.z = 16.0f;

                                temp = 60;
                                
                                break;

                            case 0xC6:
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX + 1, (u8)vec.z - groundObjectsGridZ - 1);

                                vec2.x = 16.0f;
                                vec2.z = -16.0f;

                                temp = 60;
                                
                                break;
                        
                            case 0xC7:
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ - 1);
                                addGroundObjectToMap(gBaseMapIndex, BASE_TILE, (u8)vec.x - groundObjectsGridX - 1, (u8)vec.z - groundObjectsGridZ - 1);

                                vec2.x = -16.0f;
                                vec2.z = -16.0f;

                                temp = 60;
                                
                                break;

                        }

                        groundObjectIndex = allocateGroundItemSlot(1, 12, temp, 0, 8);

                        vec = getGroundObjectWorldPosition(MAIN_MAP_INDEX, (u8)vec.x, (u8)vec.z);
                        vec.x += vec2.x;
                        vec.z += vec2.z;
                    
                        setItemPosition(groundObjectIndex, vec.x, vec.y, vec.z);
                        break;
                    
                    }
                    
                    break;
                
            }
            
        } else {
            toolUse.toolUseState = 0;
        }

        if (toolUse.stepIndex == 0) {

            switch (gPlayer.currentToolLevel) {

                case 0:
                    playSfx(HAMMER_SFX);
                    break;
                case 1:
                    playSfx(SILVER_HAMMER_SFX);
                    break;
                case 2:
                    playSfx(GOLDEN_HAMMER_SFX);
                    break;
                
            }
            
        }

        toolUse.stepIndex++;

        switch (gPlayer.currentToolLevel) {
        
            case 0:
                toolUse.toolUseState = 0;
                break;
            
            case 1:
                toolUse.toolUseState = 0;
                break;
            
            case 2:
                toolUse.toolUseState = 0;
                break;

        }
    
    } else {
        toolUse.toolUseState = 0;
    }
 
    if (gBaseMapIndex == MOUNTAIN_1 && (getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0, 32.0f) == 0x11) && (gPlayer.currentToolLevel == 2)) {
        deactivateMapObject(MAIN_MAP_INDEX, 3);
        setDailyEventBit(0x29);
    }
    
    handleHitFarmAnimalWithTool();
    handleHitChickenWithTool();
    handleHitDogWithTool();
    handleHitHorseWithTool();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D304C);

void useWateringCan(void) {
    
    Vec3f vec;
    
    f32 tempF;
    u8 temp1, temp2;
    s32 tempX, tempZ;
    s32 direction;
    u8 currentGroundObjectIndex;

    u8 arr1[3];
    u8 arr2[3];
    u8 arr3[3];
    u8 arr4[3];
    u8 arr5[12];
    u8 arr6[8];
    
    u32 padding[2];

    memcpy(arr1, D_80122340, 3);
    memcpy(arr2, D_80122344, 3);
    memcpy(arr3, D_80122348, 3);
    memcpy(arr4, D_8012234C, 3);
    memcpy(arr5, D_80122368, 9);
    memcpy(arr6, D_80122374, 9);
    
    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);

    switch (gPlayer.currentToolLevel) {

        case 0:
            temp1 = 1;
            temp2 = 8;
            break;
        
        case 1:
            
            temp1 = 1;
            
            if (direction & 1) {
                
                if (entities[ENTITY_PLAYER].direction < 5) {
                    temp2 = arr3[toolUse.stepIndex];
                } else {
                    temp2 = arr4[toolUse.stepIndex]; 
                }

            } else if (entities[ENTITY_PLAYER].direction < 5) {
                temp2 = arr1[toolUse.stepIndex];
            } else {
                temp2 = arr2[toolUse.stepIndex];
            }
            
            break;
        
        case 2:
            
            temp1 = 2;

            if ((u8)direction < 5) {
                temp2 = arr5[toolUse.stepIndex];
            } else {
                temp2 = arr6[toolUse.stepIndex];
            }
            
            break;
        
    }

    tempF = (f32)temp1;
            
    vec = getOffsetTileCoordinates(tempF, temp2);
    currentGroundObjectIndex = getGroundObjectIndexFromPlayerPosition(tempF, temp2);
    
    if (wateringCanUses != 0) {

        if ((getGroundObjectToolInteractionFlags(currentGroundObjectIndex) & GROUND_OBJECT_WATERABLE) && vec.y != 65535.0f) {

            if (currentGroundObjectIndex == TILLED) {
                temp2 = TILLED_WATERED;            
            } else {
                temp2 = currentGroundObjectIndex + 1;
            }
            
            addGroundObjectToMap(gBaseMapIndex, temp2, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

            vec = getGroundObjectWorldPosition(0, (u8)vec.x, (u8)vec.z);
            spawnToolEffectEntity(toolUse.stepIndex, 1, vec.x, vec.y, vec.z);

            if (gBaseMapIndex == POND) {
                blueMistFlowerPlot = temp2;
            }
            
        } else {

            vec = getGroundObjectWorldPosition(0, (u8)vec.x, (u8)vec.z);
            vec.y = entities[ENTITY_PLAYER].coordinates.y;

            spawnToolEffectEntity(toolUse.stepIndex, 1, vec.x, vec.y, vec.z);
            
        } 

        wateringCanUses--;
        
        if (toolUse.stepIndex == 0) {

            switch (gPlayer.currentToolLevel) {

                case 0:
                    playSfx(WATERING_CAN_SFX);
                    break;
                case 1:
                    playSfx(SILVER_WATERING_CAN_SFX);
                    break;
                case 2:
                    playSfx(GOLDEN_WATERING_CAN_SFX);
                    break;
                
            }
            
        }
        
    }

    toolUse.stepIndex++;

    switch (gPlayer.currentToolLevel) {
            
        case 0:
            toolUse.toolUseState = 0;
            break;
        
        case 1:

            if (toolUse.stepIndex == 3) {
                toolUse.toolUseState = 0;
            }
    
            break;
        
        case 2:

            if (toolUse.stepIndex == 9) {
                toolUse.toolUseState = 0;
            }
            
            break;
            
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3694);

void useMilker(void) {
    handleMilkCow();
    toolUse.toolUseState = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D36BC);

void useCowBell(void) {
    handleCallFarmAnimalsWithCowBell();
    toolUse.toolUseState = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D36E4);

void useBrush(void) {
    handleBrushFarmAnimal();
    handleBrushHorse();
    toolUse.toolUseState = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3714);

void useClippers(void) {
    handleShearSheep();
    toolUse.toolUseState = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D373C);

void useTurnipSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = TURNIP_PLANTED_STAGE_1;
        } else {
            groundObjectIndex = INVALID_GROUND_OBJECT;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
        
    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3958);

void usePotatoSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }

    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = POTATO_PLANTED_STAGE_1;
        } else {
          groundObjectIndex = INVALID_GROUND_OBJECT;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
        
    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3B74);

void useCabbageSeeds(void) {
 
    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {
        
        if (gSeason == SPRING || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = CABBAGE_PLANTED_STAGE_1;
        } else {
          groundObjectIndex = INVALID_GROUND_OBJECT;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3D90);

void useTomatoSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if (gSeason == SUMMER || gBaseMapIndex == GREENHOUSE) {
          groundObjectIndex = TOMATO_PLANTED_STAGE_1;
        } else {
          groundObjectIndex = INVALID_GROUND_OBJECT;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D3FAC);

void useCornSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if (gSeason == SUMMER || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = CORN_PLANTED_STAGE_1;
        } else {
            groundObjectIndex = INVALID_GROUND_OBJECT;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D41C8);

void useEggplantSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    }
    else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }

    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if (gSeason == AUTUMN || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = EGGPLANT_PLANTED_STAGE_1;
        } else {
            groundObjectIndex = INVALID_GROUND_OBJECT;
        }
        
        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D43E4);

void useStrawberrySeeds(void) {
    
    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    }
    else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }

    vec = getOffsetTileCoordinates(0.0f, temp);

    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        // leaving for reference
        //groundObjectIndex = ((-(gBaseMapIndex != 0x56) & ~0x28) | 0x44);

        groundObjectIndex = (gBaseMapIndex == GREENHOUSE) ? STRAWBERRY_PLANTED_STAGE_1 : INVALID_GROUND_OBJECT;

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }

    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }

    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D45F4);

void useMoonDropSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    }
    else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {
        
        // alternate for reference: (gSeason - 1 < 2U)
        if ((gSeason == SPRING || gSeason == SUMMER) || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = MOONDROP_PLANTED_STAGE_1; 
        } else {
            groundObjectIndex = INVALID_GROUND_OBJECT;
        }

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);

    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4814);

void usePinkCatMintSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        // FIXME: should be range
        if ((gSeason - 1 < 2U) || gBaseMapIndex == GREENHOUSE) {
            groundObjectIndex = PINK_CAT_MINT_PLANTED_STAGE_1; 
        } else {
            groundObjectIndex = INVALID_GROUND_OBJECT;
        }

        addGroundObjectToMap(gBaseMapIndex, groundObjectIndex, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4A34);

void useBlueMistSeeds(void) {

    u8 direction;
    u8 temp;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {
        addGroundObjectToMap(gBaseMapIndex, 0xB2, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);
    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4C28);

void useChickenFeed(void) {
    
    u8 levelInteractionIndex;
    
    levelInteractionIndex = checkChickenFeedInteraction(gBaseMapIndex);
  
    if (levelInteractionIndex != 0xFF) {
        setChickenFeedSprite(levelInteractionIndex - 0x13);
        gChickens[levelInteractionIndex - 0x13].flags |= CHICKEN_FED;
    }
    
    toolUse.toolUseState = 0;  

}

void useBall(void) {}

void useFeedingBottle(void) {}

void useUnknownTool(void) {}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4CC0);

void useFishingPole(void) {
    toolUse.toolUseState = 0;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4CD0);

void useMiraclePotion(void) {

    if (handleUseMiraclePotion()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    toolUse.toolUseState = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4D1C);

void useCowMedicine(void) {

    if (handleAnimalMedicineUse()) {
        setDailyEventBit(0x14);
    } else {
        toggleDailyEventBit(0x14);
    }

    toolUse.toolUseState = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D4D68);

void useGrassSeeds(void) {

    u8 direction;
    u8 temp;

    Vec3f vec;

    direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < EAST) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    }
    else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if (gBaseMapIndex == FARM) {
            addGroundObjectToMap(FARM, 0x85, (u8)vec.x - groundObjectsGridX, (u8)vec.z - groundObjectsGridZ);    
        }

    }
    
    if (toolUse.stepIndex == 0) {
        playSfx(SEEDS_SFX);
    }
    
    toolUse.stepIndex++;
    
    if (toolUse.stepIndex == 9) {
        toolUse.toolUseState = 0;
    }

}

static const u8 D_80122340[4] = { 2, 8, 6, 0 };

static const u8 D_80122344[4] = { 6, 8, 2, 0 };

static const u8 D_80122348[4] = { 3, 8, 5, 0 };

static const u8 D_8012234C[4] = { 5, 8, 3, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122340);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122344);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122348);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_8012234C);

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122350);

static const u8 D_80122350[12] = { 8, 0, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_8012235C);

static const u8 D_8012235C[12] = { 8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122368);

static const u8 D_80122368[12] = { 3, 4, 5, 2, 8, 6, 1, 0, 7, 0, 0, 0 };

//INCLUDE_RODATA("asm/nonmatchings/game/itemHandlers", D_80122374);

static const u8 D_80122374[12] = { 5, 4, 3, 6, 8, 2, 7, 0, 1, 0, 0, 0 };
 
//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleBlueFeatherUse);

void handleBlueFeatherUse(void) {
     
    toggleDailyEventBit(SUCCESSFUL_PROPOSAL);
    
    if (npcTalkingTo == 0xFF) goto func_end;

    getBlueFeatherResponse(npcTalkingTo, 0);
    
    npcs[npcTalkingTo].animationMode = npcs[npcTalkingTo].defaultAnimationMode;
    
    setSpecialDialogueBit(0x135);
    
    switch (npcTalkingTo) {
        case MARIA:
            if (npcAffection[MARIA] >= 220) {
                gWife = MARIA;   
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(MARIA);
            }
            break;
        case POPURI:
            if (npcAffection[POPURI] >= 220) {
                gWife = POPURI;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(POPURI);
            }
            break;
        case ELLI:
            if (npcAffection[ELLI] >= 220) {
                gWife = ELLI;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ELLI);
            }
            break;
        case ANN:
            if (npcAffection[ANN] >= 220) {
                gWife = ANN;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ANN);
            }
            break;
        case KAREN:
            if (npcAffection[KAREN] >= 220) {
                gWife = KAREN;
                setDailyEventBit(SUCCESSFUL_PROPOSAL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(KAREN);
            }
            break;
        default:
            break;
        }

    if (checkDailyEventBit(SUCCESSFUL_PROPOSAL)) {

        // use up blue feather if successful proposal
        gPlayer.currentTool = 0;

    }
    
func_end:
        toolUse.toolUseState = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D5130);

void useEmptyBottle(void) {

    if (checkWaterableSoilInteraction(gBaseMapIndex)) {
        gPlayer.bottleContents = 1;
    } else if (checkWineBarrelInteraction(gBaseMapIndex) ) {
        gPlayer.bottleContents = 2;
    } else {
        handleGetMilkWithBottle();
    }

    toolUse.toolUseState = 0;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", clearAllItemContextSlots);

// inline, used by initializeGameVariables
void clearAllItemContextSlots(void) {

    u8 i;

    for (i = 0; i < 10; i++) {

        itemInfo[i].position.x = 0;
        itemInfo[i].position.y = 0;
        itemInfo[i].position.z = 0;

        itemInfo[i].movement.x = 0;
        itemInfo[i].movement.y = 0;
        itemInfo[i].movement.z = 0;

        itemInfo[i].attachmentOffset.x = 0;
        itemInfo[i].attachmentOffset.y = 0;
        itemInfo[i].attachmentOffset.z = 0;

        itemInfo[i].unk_24 = 0;
        itemInfo[i].itemAnimationFrameCounter = 0;
        itemInfo[i].heldItemIndex = 0;
        itemInfo[i].stateIndex = 0;
        itemInfo[i].flags = 0;

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", loadActiveItemEntities);

void loadActiveItemEntities(void) {

    u8 i;
    
    for (i = 0; i < 10; i++) {
        if (itemInfo[i].flags & ITEM_CONTEXT_ACTIVE) {
            loadHeldItemEntity(i, itemInfo[i].heldItemIndex);
        }    
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", initializeHeldItem);

inline u8 initializeHeldItem(u8 index, u8 arg1, u32 heldItemIndex, s32 hasDirectionFrame, s32 useAttachment) {

    itemInfo[index].stateIndex = arg1;
    itemInfo[index].heldItemIndex = heldItemIndex;
    itemInfo[index].itemAnimationFrameCounter = 0;
    itemInfo[index].flags = hasDirectionFrame | (useAttachment | 1);
    
    gItemBeingHeld = getHeldItemIndex(heldItemIndex);
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", allocateThrownItemSlot);

u8 allocateThrownItemSlot(u8 index, u8 arg1, u32 heldItemIndex, u16 arg3, u8 arg4) {
    
    u8 found = FALSE;
    int tempBit = 1;
    
    while (index < 10 && !found) {
        
        if (!(itemInfo[index].flags & ITEM_CONTEXT_ACTIVE)) {
          found = TRUE;
        }
        else {
          index++;
        }

    }
    
    if (found) {
        
        // FIXME: inline initializeHeldItem call
        itemInfo[index].stateIndex = arg1;
        itemInfo[index].heldItemIndex = heldItemIndex;
        itemInfo[index].flags = arg3 | (arg4 | tempBit);
        itemInfo[index].itemAnimationFrameCounter = 0;

        gItemBeingHeld = getHeldItemIndex(heldItemIndex);

        // index = initializeHeldItem(index, arg1, arg2, arg3, arg4);

    }
      
    else {
        index = 0xff;
    }
    
    return index;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", allocateGroundItemSlot);

u8 allocateGroundItemSlot(u8 index, u8 arg1, u32 heldItemIndex, u16 arg3, u8 arg4) {
    
    u8 found = FALSE;
    
    while (index < 10 && !found) {

        if (!(itemInfo[index].flags & ITEM_CONTEXT_ACTIVE)) {
          found = TRUE;
        }
        else {
          index++;
        }

    }

    if (found) {
        
        itemInfo[index].stateIndex = arg1;
        itemInfo[index].heldItemIndex = heldItemIndex;
        itemInfo[index].flags = arg3 | (arg4 | 1);
        itemInfo[index].itemAnimationFrameCounter = 0;

    } else {
        index = 0xff;
    }
    
    return index;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", clearHeldItemSlot);

void clearHeldItemSlot(u8 index) {

    if (itemInfo[index].flags & ITEM_CONTEXT_ACTIVE) {
        deactivateEntity(ENTITY_ITEM_BASE_INDEX + index);
        itemInfo[index].stateIndex = ITEM_STATE_INACTIVE;
        itemInfo[index].flags = 0;
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", func_800D55B8);

u16 func_800D55B8(u8 index) {
    return itemInfo[index].heldItemIndex;
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", setItemState);

void setItemState(u8 index, u8 arg1) {

    if (itemInfo[index].flags & ITEM_CONTEXT_ACTIVE) {
        itemInfo[index].stateIndex = arg1;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", setItemPosition);

void setItemPosition(u8 index, f32 arg1, f32 arg2, f32 arg3) {

    itemInfo[index].position.x = arg1;
    itemInfo[index].position.y = arg2;
    itemInfo[index].position.z = arg3;
    itemInfo[index].flags |= ITEM_CONTEXT_USE_POSITION;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", setItemAttachmentOffset);

void setItemAttachmentOffset(u8 index, f32 arg1, f32 arg2, f32 arg3) {

    itemInfo[index].attachmentOffset.x = arg1;
    itemInfo[index].attachmentOffset.y = arg2;
    itemInfo[index].attachmentOffset.z = arg3;
    itemInfo[index].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
     
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", loadHeldItemEntity);

void loadHeldItemEntity(u8 itemIndex, u16 heldItemIndex) {

    itemInfo[itemIndex].heldItemIndex = heldItemIndex;

    loadEntity(ENTITY_ITEM_BASE_INDEX + itemIndex, itemEntityMetadata[heldItemIndex][0], TRUE);
    setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);

    if (itemInfo[itemIndex].flags & ITEM_CONTEXT_USE_ATTACHMENT){
        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemIndex, 0, 0xFF);
    } else {
        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemIndex, 0xFFFF, 0xFF);
    }

    setEntityCollidable(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);
    setEntityYMovement(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);
    setEntityTracksCollisions(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);
    enableEntityMovement(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);

    setEntityColor(ENTITY_ITEM_BASE_INDEX + itemIndex, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);

    setEntityAttachmentOffset(ENTITY_ITEM_BASE_INDEX + itemIndex, 0, 0, 0);

    if (itemInfo[itemIndex].flags & ITEM_CONTEXT_HAS_DIRECTION_FRAME) {
        setEntityAnimationWithDirectionChange(ENTITY_ITEM_BASE_INDEX + itemIndex, itemEntityMetadata[heldItemIndex][gSeason]);
    } else {
        setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemIndex, itemEntityMetadata[heldItemIndex][gSeason]);
    }

    if (itemPaletteOverrides[heldItemIndex] != 0xFFFF) {
        setEntityPaletteIndex(ENTITY_ITEM_BASE_INDEX + itemIndex, itemPaletteOverrides[heldItemIndex]);
    }

    setEntityShadow(ENTITY_ITEM_BASE_INDEX + itemIndex, 2);

    itemInfo[itemIndex].stateIndex = ITEM_STATE_INACTIVE;

}

u16 getItemFlags(u16 index) {
    return itemFlags[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", getItemAnimationIndex);

u16 getItemAnimationIndex(u16 index) {

  u16 result;
  
  result = itemEntityMetadata[index][gSeason];

  if (185 < index && index < 194) {
    result = 249;
  }

  if (193 < index && index < 202) {
    result = 250;
  }

  if (170 < index && index < 179) {
    result = 251;
  }

  return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", getItemStaminaIncreaseValue);

u8 getItemStaminaIncreaseValue(u16 index) {
    return itemStaminaRestorationValue[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", getItemFatigueReductionValue);

u8 getItemFatigueReductionValue(u16 index) {
    return itemFatigueReductionValue[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handlePickUpGroundItem);

bool handlePickUpGroundItem(void) {

    u32 padding[4];
    
    u8 groundObjectIndex;
    u8 heldItemIndex;
    bool result = FALSE;
    
    if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

        if (gPlayer.heldItem == 0) {

            groundObjectIndex = getGroundObjectIndexFromPlayerPosition(1.0f, 8);

            if (groundObjectIndex != 0xFF) {
                heldItemIndex = getHeldItemIndexFromGroundObject(groundObjectIndex);
            }

            if (groundObjectIndex && (groundObjectIndex != 0xFF && heldItemIndex)) {

                gPlayer.heldItem = heldItemIndex;
                
                switch (groundObjectIndex) {

                    // replace with stalks
                    case TOMATO_RIPE ... TOMATO_RIPE_WATERED:
                        addGroundObjectToMapFromPlayerPosition(0x5F, 1.0f, 8);
                        break;
                    case CORN_RIPE ... CORN_RIPE_WATERED:
                        addGroundObjectToMapFromPlayerPosition(0x7A, 1.0f, 8);
                        break;
                    case EGGPLANT_RIPE ... EGGPLANT_RIPE_WATERED:
                        addGroundObjectToMapFromPlayerPosition(0x2A, 1.0f, 8);
                        break;
                    
                    default:
                        
                        switch (getGroundObjectMapAdditionIndex(groundObjectIndex)) {

                            case 4:
                                addGroundObjectToMapFromPlayerPosition(TILLED, 1.0f, 8);
                                break;
                            case 5:
                                addGroundObjectToMapFromPlayerPosition(TILLED_WATERED, 1.0f, 8);
                                break;
                            default:
                                addGroundObjectToMapFromPlayerPosition(BASE_TILE, 1.0f, 8);
                                break;
                            
                        }         

                        break;

                }
                
                result = TRUE;

            }
            
        }
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handlePutDownHeldItem);

// handle putting objects down
bool handlePutDownHeldItem(u8 itemIndex) {

    Vec3f vec;
    Vec3f vec2;
    Vec3f vec3;
    Vec3f vec4;
    u8 temp;
    u16 temp2;
    u16 heldItemIndex;
    u8 direction;
    u8 i;
    u8 levelInteractionIndex;
    
    bool result = FALSE;

    // FIXME: I am beyond clueless
    temp2 = *(u16*)0&itemInfo[itemIndex].heldItemIndex;
    
    if (temp2) {
        heldItemIndex = itemInfo[itemIndex].heldItemIndex;
    } else {
        heldItemIndex = itemInfo[itemIndex].heldItemIndex;
    }
    
    switch (itemInfo[itemIndex].heldItemIndex) {        
        
        case ROCK_HELD_ITEM:              
            addGroundObjectToMapFromPlayerPosition(4, 1.0f, 8);
            setGroundObjects(mapControllers[MAIN_MAP_INDEX].mainMapIndex);
            break;
        
        case LOG_HELD_ITEM:
            if ((getLevelInteractionIndexFromEntityPosition(0, 0, 32.0f) == 0x11) && (gBaseMapIndex == FARM)) {
                gLumber += adjustValue(gLumber, 1, MAX_LUMBER);
            } else {
                addGroundObjectToMapFromPlayerPosition(6, 1.0f, 8);
                setGroundObjects(mapControllers[MAIN_MAP_INDEX].mainMapIndex);
            }
            break;

        // chicken and chick (multiple sprites per direction)
        case CHICKEN_HELD_ITEM:                                     
        case 0x61:                                     
        case 0x62:                                     
        case 0x63:                                     
        case 0x64:                                     
        case 0x65:                                     
        case 0x66:                                     
        case 0x67:                                     
        case 0x68:                                     
        case 0x69:                                     
        case 0x6A:                                     
        case 0x6B:                                     
        case 0x6C:                                     
        case 0x6D:                                     
        case 0x6E:                                     
        case 0x6F:                                     

            vec2 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            gChickens[gPlayer.heldAnimalIndex].coordinates = vec2;

            gChickens[gPlayer.heldAnimalIndex].direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            gChickens[gPlayer.heldAnimalIndex].location = gBaseMapIndex;

            gChickens[gPlayer.heldAnimalIndex].flags &= ~8;

            initializeChickenEntity(gPlayer.heldAnimalIndex);
            
            break;

        case DOG_HELD_ITEM:                                     
        case 0x59:                                     
        case 0x5A:                                     
        case 0x5B:                                     
        case 0x5C:                                     
        case 0x5D:                                     
        case 0x5E:                                     
        case 0x5F:                              

            vec3 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            dogInfo.coordinates = vec3;

            dogInfo.direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            dogInfo.location = gBaseMapIndex;

            dogInfo.flags &= ~8;

            initializeDogEntity();
            
            break;

        case DOG_2_HELD_ITEM:                                     
        case 0xAC:                                     
        case 0xAD:                                     
        case 0xAE:                                     
        case 0xAF:                                     
        case 0xB0:                                     
        case 0xB1:                                     
        case 0xB2:     
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(0, direction, vec.x, vec.y, vec.z), 0);

            break;
        
        case PUPPY_1_HELD_ITEM:                                     
        case 0x7C:                                     
        case 0x7D:                                     
        case 0x7E:                                     
        case 0x7F:                                     
        case 0x80:                                     
        case 0x81:                                     
        case 0x82: 
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(1, direction, vec.x, vec.y, vec.z), 0);

            break;

        case CAT_HELD_ITEM:                                     
        case 0x84:                                     
        case 0x85:                                     
        case 0x86:                                     
        case 0x87:                                     
        case 0x88:                                     
        case 0x89:                                     
        case 0x8A:        
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(2, direction, vec.x, vec.y, vec.z), 0);

            break;

        case FOX_HELD_ITEM:                                     
        case 0x8C:                                     
        case 0x8D:                                     
        case 0x8E:                                     
        case 0x8F:                                     
        case 0x90:                                     
        case 0x91:                                     
        case 0x92:   
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(7, direction, vec.x, vec.y, vec.z), 0);

            break;       
                
        case SQUIRREL_HELD_ITEM:                                     
        case 0x94:                                     
        case 0x95:                                     
        case 0x96:                                     
        case 0x97:                                     
        case 0x98:                                     
        case 0x99:                                     
        case 0x9A:  
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(9, direction, vec.x, vec.y, vec.z), 0);

            break;     

        case RABBIT_HELD_ITEM:                                     
        case 0x9C:                                     
        case 0x9D:                                     
        case 0x9E:                                     
        case 0x9F:                                     
        case 0xA0:                                     
        case 0xA1:                                     
        case 0xA2:         
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(8, direction, vec.x, vec.y, vec.z), 0);
            break;     
        
        case MONKEY_HELD_ITEM:                                     
        case 0xA4:                                     
        case 0xA5:                                     
        case 0xA6:                                     
        case 0xA7:                                     
        case 0xA8:                                     
        case 0xA9:                                     
        case 0xAA:    
            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            initializeMiscAnimalEntity(spawnMiscAnimal(10, direction, vec.x, vec.y, vec.z), 0);
            break;     

        case BUTTERFLY_HELD_ITEM:                                     
        case LADYBUG_HELD_ITEM:                                     
        case CICADA_HELD_ITEM:                                     
        case HORNED_BEETLE_HELD_ITEM:                                     
        case STAG_BEETLE_HELD_ITEM:                                     
        case RED_DRAGONFLY_HELD_ITEM:                                     
        case CRICKET_HELD_ITEM:                  
            
            switch (heldItemIndex) {                          
                case 0xB3:                                  
                    temp = 0xF;
                    break;
                case LADYBUG_HELD_ITEM:                                  
                    temp = 0x10;
                    break;
                case CICADA_HELD_ITEM:                                  
                    temp = 0x11;
                    break;
                case HORNED_BEETLE_HELD_ITEM:                                  
                    temp = 0x12;
                    break;
                case STAG_BEETLE_HELD_ITEM:                                  
                    temp = 0x13;
                    break;
                case RED_DRAGONFLY_HELD_ITEM:                                  
                    temp = 0x14;
                    break;
                case CRICKET_HELD_ITEM:                                  
                    temp = 0x15;
                    break;
            }

            vec = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            vec.y = entities[ENTITY_PLAYER].coordinates.y;
            
            initializeMiscAnimalEntity(spawnMiscAnimal(temp, direction, vec.x, vec.y, vec.z), 1);

            break;

       case EGG_HELD_ITEM:                              

            gPlayer.heldAnimalIndex = initializeNewChicken(0, 0xFF);

            gChickens[gPlayer.heldAnimalIndex].coordinates.x = 92.0f;
            gChickens[gPlayer.heldAnimalIndex].coordinates.y = 0;
            gChickens[gPlayer.heldAnimalIndex].coordinates.z = -148.0f;
            
            gChickens[gPlayer.heldAnimalIndex].direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            gChickens[gPlayer.heldAnimalIndex].location = gBaseMapIndex;
            
            gChickens[gPlayer.heldAnimalIndex].flags &= ~8;
            gChickens[gPlayer.heldAnimalIndex].flags |= 0x20;

            initializeChickenEntity(gPlayer.heldAnimalIndex);

            break;

        case FODDER_HELD_ITEM:                    

            levelInteractionIndex = checkBarnAnimalFeedInteraction(gBaseMapIndex);

            if (levelInteractionIndex == 0x12) {
                
                setBarnFodderSprite(8);

                for (i = 0; i < MAX_FARM_ANIMALS; i++) {
                    gFarmAnimals[i].flags |= 0x10;
                }

            } else {
                
                setBarnFodderSprite(levelInteractionIndex - 0x13);
                gFarmAnimals[levelInteractionIndex - 0x13].flags |= FARM_ANIMAL_FED;
                
            }

            
            break;
        
        case BABY_HELD_ITEM:                                     
        case 0xBB:                                     
        case 0xBC:                                     
        case 0xBD:                                     
        case 0xBE:                                     
        case 0xBF:                                     
        case 0xC0:                                     
        case 0xC1:                  

            npcs[BABY].levelIndex = HOUSE;
            npcs[BABY].startingCoordinates.y = 0;
            npcs[BABY].direction = SOUTHWEST;
            npcs[BABY].defaultAnimationMode = NPC_ANIM_WANDER;
            npcs[BABY].animationIndex1 = 0;
            npcs[BABY].animationMode = NPC_ANIM_WANDER;

            npcs[BABY].startingCoordinates.x = -192.0f;
            npcs[BABY].startingCoordinates.z = -64.0f;
            
            npcs[BABY].flags |= NPC_ACTIVE;
            
            setDailyEventBit(PICKED_UP_BABY);
            setupNPCEntity(BABY, 0);

            break;
            
        case BABY_GROWN_HELD_ITEM:                                     
        case 0xC3:                                     
        case 0xC4:                                     
        case 0xC5:                                     
        case 0xC6:                                     
        case 0xC7:                                     
        case 0xC8:                                     
        case 0xC9:                               

            vec4 = projectEntityPosition(ENTITY_PLAYER, 0x20, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            npcs[BABY].startingCoordinates = vec4;

            npcs[BABY].levelIndex = gBaseMapIndex;
            npcs[BABY].flags |= 1;

            npcs[BABY].direction = convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            npcs[BABY].defaultAnimationMode = NPC_ANIM_CUSTOM;
            npcs[BABY].animationIndex1 = 57;
            npcs[BABY].animationMode = NPC_ANIM_CUSTOM;
            
            setDailyEventBit(PICKED_UP_BABY);
            setupNPCEntity(BABY, 0);
            
            break;
        
    }


    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", getHeldItemIndex);

u8 getHeldItemIndex(u8 index) {
    return heldItemIndices[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", showHeldItemText);

void showHeldItemText(u8 index) {

    u16 temp;

    switch (index) {
        case 0x60 ... 0x6F:
            temp =  gPlayer.heldAnimalIndex + 0xF5;
            break;
        default:
            temp = itemTextIndices[index];
            break;
    }

    showTextBox(1, SHOP_TEXT_INDEX, temp, 0, 2);

}

// alternate for reference
// void showHeldItemText(u8 index) {
    
//     s32 temp;
//     u16 temp2;

//     temp = index;
    
//     if (temp < 0x70) {
//         if (temp > 0x5F) {
//             temp2 = gPlayer.heldAnimalIndex + 0xF5;
//             goto func_end;
//         } 
//     } 

// 
//     temp2 = D_80118000[index];
    
// func_end:
//     showTextBox(1, SHOP_TEXT_INDEX, temp2, 0, 2);
// }

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", processItemShipping);

void processItemShipping(u8 index) {

    u32 result;
    
    dailyShippingBinValue += adjustValue(dailyShippingBinValue, itemShippingValues[index], MAX_TOTAL_SHIPPING);
     
    switch (index) {
        case TURNIP:
            totalTurnipsShipped += adjustValue(totalTurnipsShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case POTATO:
            totalPotatoesShipped += adjustValue(totalPotatoesShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case EGGPLANT:
            totalEggplantsShipped += adjustValue(totalEggplantsShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case CABBAGE:
            totalCabbageShipped += adjustValue(totalCabbageShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case STRAWBERRY:
            totalStrawberriesShipped += adjustValue(totalStrawberriesShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case TOMATO:
            totalTomatoesShipped += adjustValue(totalTomatoesShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case CORN:
            totalCornShipped += adjustValue(totalCornShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case SMALL_MILK:
        case MEDIUM_MILK:
        case LARGE_MILK:
        case GOLDEN_MILK:
            gTotalMilkShipped += adjustValue(gTotalMilkShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            break;
        case EGG_HELD_ITEM:
            gTotalEggsShipped += adjustValue(gTotalEggsShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            break;
            
    }

    handleHorseShippingItem();

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleFeedDog);

inline void handleFeedDog(void) {
    setMapObjectAnimation(MAIN_MAP_INDEX, 0, 19);
    setDailyEventBit(FED_DOG);
}

static inline void setVec3f(u8 index, f32 x, f32 y, f32 z) {

    itemInfo[index].position.x = x;
    itemInfo[index].position.y = y;
    itemInfo[index].position.z = z;

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", handleItemDroppedInWater);

void handleItemDroppedInWater(u8 arg0, u8 itemIndex) {

    u8 temp;
    u8 temp2;
    u8 flags;
    
    itemInfo[itemIndex].attachmentOffset.x = 0;
    itemInfo[itemIndex].attachmentOffset.y = 0;
    itemInfo[itemIndex].attachmentOffset.z = 0;
    itemInfo[itemIndex].flags |= ITEM_CONTEXT_USE_ATTACHMENT;

    setEntityMovementVector(ENTITY_ITEM_BASE_INDEX + itemIndex, 0.0f, 0.0f, 0.0f, 0.0f);
    setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemIndex, 0xFFFF, 0xFF);
    
    setEntityCollidable(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);
    setEntityYMovement(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);
    setEntityTracksCollisions(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);
    enableEntityMovement(ENTITY_ITEM_BASE_INDEX + itemIndex, FALSE);

    if (gBaseMapIndex == FARM) {
        
        temp = getLevelInteractionIndexFromEntityPosition(0, 0.0f, 32.0f);
        // FIXME: possibly fake
        temp2 = temp - 0x1B;
        
        if (temp2 < 2 || temp == 0x1D) {
            setVec3f(itemIndex, 288.0f, 80.0f, -404.0f);
            itemInfo[itemIndex].flags |= ITEM_CONTEXT_USE_POSITION;
            setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemIndex, 0xE9);
        } 

    }
    
    if (gBaseMapIndex == MOUNTAIN_1) {
        
        temp = getLevelInteractionIndexFromEntityPosition(0, 0.0f, 40.0f);
        
        if (temp == 0x10 || temp == 0x14) {
            setVec3f(itemIndex, 160.0f, 64.0f, -128.0f);
            itemInfo[itemIndex].flags |= ITEM_CONTEXT_USE_POSITION;
            setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemIndex, 0xE9);
        }
        
        if (!(gCutsceneFlags & 1)) {

            // FIXME: use a range
            // fish
            if (!checkLifeEventBit(MET_KAPPA) && !checkDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY) && (itemInfo[itemIndex].heldItemIndex - 0x25) < 2U && 8 < gHour && gHour < 17) {
                setDailyEventBit(KAPPA_FISH_OFFERING_DAILY);
            }

            // check kappa power nut
            if (!checkLifeEventBit(KAPPA_POWER_NUT) && !checkDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY) && itemInfo[itemIndex].heldItemIndex == LARGE_FISH && 8 < gHour && gHour < 17) {
                setDailyEventBit(KAPPA_LARGE_FISH_OFFERING_DAILY);
            }

        }
        
    }

    if (gBaseMapIndex == POND) {
        
        temp = getLevelInteractionIndexFromEntityPosition(ENTITY_PLAYER, 0.0f, 32.0f);
        
        if (temp == 0x10) {
            setVec3f(itemIndex, -16.0f, 80.0f, -224.0f);
            itemInfo[itemIndex].flags |= ITEM_CONTEXT_USE_POSITION;
            setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemIndex, 233);
        }
        
        if (!(gCutsceneFlags & 1)) {
            if (!checkDailyEventBit(HARVEST_GODDESS_INTERACTION) && (itemFlags[itemInfo[itemIndex].heldItemIndex] & ITEM_HARVEST_GODDESS_OFFERABLE) && 8 < gHour && gHour < 17) {
                setDailyEventBit(HARVEST_GODDESS_OFFERING);    
            }
        }

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/itemHandlers", updateHeldItemState);

// handle thrown/dropped items
void updateHeldItemState(void) {

    Vec3f vec;
    u8 i = 0;
    bool set;

    f32 x, y, z;
    f32 x2, y2, z2;
    f32 z3;

    for (i = 0; i < 10; i++) {

        if (itemInfo[i].flags & ITEM_CONTEXT_ACTIVE) {
            
            do {

                // FIXME: fake match/likely dead code
                if (itemInfo[i].stateIndex) {};
                
                y = 14.0f;
                z = 20.0f;
                x = -16.0f;

                z3 = 8.0f;
                y2 = 32.0f;
                z2 = 16.0f;

                set = FALSE;
                
                switch (itemInfo[i].stateIndex) {

                    case ITEM_STATE_HELD:
                        loadHeldItemEntity(i, itemInfo[i].heldItemIndex);
                        itemInfo[i].attachmentOffset.x = 0;
                        itemInfo[i].attachmentOffset.y = y;
                        itemInfo[i].attachmentOffset.z = z;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
                        break;

                    case ITEM_STATE_DIALOGUE_SELECTION:
                        loadHeldItemEntity(i, itemInfo[i].heldItemIndex);
                        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + i, 0, 0);
                        itemInfo[i].attachmentOffset.x = x;
                        itemInfo[i].attachmentOffset.y = 24.0f;
                        itemInfo[i].attachmentOffset.z = z3;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
                        break;

                    case ITEM_STATE_TOOL_ACQUIRED:
                        loadHeldItemEntity(i, itemInfo[i].heldItemIndex);
                        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + i, 0, 0);
                        itemInfo[i].attachmentOffset.x = x;
                        itemInfo[i].attachmentOffset.y = 64.0f;
                        itemInfo[i].attachmentOffset.z = z3;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
                        break;

                    case ITEM_STATE_THROW_START:
                        loadHeldItemEntity(i, itemInfo[i].heldItemIndex);
                        itemInfo[i].attachmentOffset.x = 0.0f;
                        itemInfo[i].attachmentOffset.y = y2;
                        itemInfo[i].attachmentOffset.z = z2;
                        itemInfo[i].stateIndex = ITEM_STATE_THROW_MOTION;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
                        break;

                    case ITEM_STATE_THROW_MOTION:

                        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + i, 0xFFFF, 0xFF);

                        vec = getMovementVectorFromDirection(4.0f, convertWorldToSpriteDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX), 0.0f);

                        itemInfo[i].movement = vec;

                        itemInfo[i].movement.y = entities[ENTITY_ITEM_BASE_INDEX + i].coordinates.y;

                        itemInfo[i].itemAnimationFrameCounter = 0;
                        itemInfo[i].stateIndex = ITEM_STATE_THROW_FLIGHT;

                        break;

                    case ITEM_STATE_THROW_FLIGHT:
                        
                        if (entities[ENTITY_ITEM_BASE_INDEX + i].flags & 0x800) {
                            
                            itemInfo[i].movement.x = 0;
                            itemInfo[i].movement.y = 0;
                            itemInfo[i].movement.z = 0;
                            
                            setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                            setEntityCollidable(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                            setEntityYMovement(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                            setEntityTracksCollisions(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                            enableEntityMovement(ENTITY_ITEM_BASE_INDEX + i, FALSE);

                            itemInfo[i].stateIndex = ITEM_STATE_THROW_LANDED;

                            set = TRUE;

                        } else {

                            // parabolic equation for throwing items
                            entities[ENTITY_ITEM_BASE_INDEX + i].coordinates.y = itemInfo[i].movement.y + (f32)(itemInfo[i].itemAnimationFrameCounter * itemInfo[i].itemAnimationFrameCounter) * -0.5f;
                            itemInfo[i].itemAnimationFrameCounter++;

                        }

                        break;

                    default:
                        break;

                    case ITEM_STATE_THROW_LANDED:

                        deactivateEntity(ENTITY_ITEM_BASE_INDEX + i);
                        loadEntity(ENTITY_ITEM_BASE_INDEX + i, 0x5E, 1);
                        setEntityColor(ENTITY_ITEM_BASE_INDEX + i, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
                        setEntityMovementVector(ENTITY_ITEM_BASE_INDEX + i, 0.0f, 0.0f, 0.0f, 0.0f);
                        setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + i, 0xFFFF, 0xFF);
                        setEntityCollidable(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                        setEntityYMovement(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                        setEntityTracksCollisions(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                        enableEntityMovement(ENTITY_ITEM_BASE_INDEX + i, FALSE);
                        setEntityAnimation(ENTITY_ITEM_BASE_INDEX + i, itemSpriteAnimations[itemInfo[i].heldItemIndex][gSeason-1]);

                        itemInfo[i].stateIndex = ITEM_STATE_CLEANUP;

                        if (itemFlags[itemInfo[i].heldItemIndex] & ITEM_EATABLE) {
                            gHappiness += adjustValue(gHappiness, -1, MAX_HAPPINESS);
                        }

                        break;

                    case ITEM_STATE_PICKUP:

                        itemInfo[i].attachmentOffset.x = 0;
                        itemInfo[i].attachmentOffset.y = z3;
                        itemInfo[i].attachmentOffset.z = z;

                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;

                        loadHeldItemEntity(i, itemInfo[i].heldItemIndex);

                        break;

                    case ITEM_STATE_PICKUP_DONE:
                        itemInfo[i].attachmentOffset.x = 0;
                        itemInfo[i].attachmentOffset.y = y;
                        itemInfo[i].attachmentOffset.z = z;
                        itemInfo[i].stateIndex = ITEM_STATE_INACTIVE;

                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;

                        break;

                    case ITEM_STATE_PLACE:
                        itemInfo[i].attachmentOffset.x = 0;
                        itemInfo[i].attachmentOffset.y = z3;
                        itemInfo[i].attachmentOffset.z = z;

                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;

                        break;

                    case ITEM_STATE_PUT_DOWN:

                        handlePutDownHeldItem(i);

                        if (itemInfo[i].flags & ITEM_CONTEXT_ACTIVE) {
                            deactivateEntity(ENTITY_ITEM_BASE_INDEX + i);
                            itemInfo[i].stateIndex = ITEM_STATE_INACTIVE;
                            itemInfo[i].flags = 0;
                        }

                        break;

                    case ITEM_STATE_EATING:
                        
                        setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + i, 0, 0);
                        
                        itemInfo[i].attachmentOffset.x = x;
                        itemInfo[i].attachmentOffset.y = y2;
                        itemInfo[i].attachmentOffset.z = z3;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;

                        break;

                    case ITEM_STATE_SHIP:

                        processItemShipping(itemInfo[i].heldItemIndex);

                        switch (gBaseMapIndex) {

                            // update shipping bin geometry
                            case FARM:
                                activateMapAddition(MAIN_MAP_INDEX, 18, 0);
                                playSfx(ITEM_PLUCK_SFX);
                                break;

                            case BARN:
                                activateMapAddition(MAIN_MAP_INDEX, 0, 0);
                                playSfx(ITEM_PLUCK_SFX);
                                break;

                            case COOP:
                                activateMapAddition(MAIN_MAP_INDEX, 0, 0);
                                playSfx(ITEM_PLUCK_SFX);
                                break;

                            case GREENHOUSE:
                                activateMapAddition(MAIN_MAP_INDEX, 0, 0);
                                playSfx(ITEM_PLUCK_SFX);
                                break;

                        }

                        deactivateEntity(ENTITY_ITEM_BASE_INDEX + i);
                        itemInfo[i].stateIndex = ITEM_STATE_CLEANUP;

                        break;

                    case ITEM_STATE_DOG_BOWL:
                        itemInfo[i].attachmentOffset.x = 0.0f;
                        itemInfo[i].attachmentOffset.y = 8.0f;
                        itemInfo[i].attachmentOffset.z = z;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
                        break;

                    case ITEM_STATE_FEED_DOG:
                        handleFeedDog();
                        deactivateEntity(ENTITY_ITEM_BASE_INDEX + i);
                        itemInfo[i].stateIndex = ITEM_STATE_CLEANUP;
                        break;

                    case ITEM_STATE_RAISED:
                    case ITEM_STATE_RAISED_ALT:
                        itemInfo[i].attachmentOffset.x = 0.0f;
                        itemInfo[i].attachmentOffset.y = y2;
                        itemInfo[i].attachmentOffset.z = z2;
                        itemInfo[i].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
                        break;

                    case ITEM_STATE_DROP_IN_WATER:
                        handleItemDroppedInWater(itemInfo[i].heldItemIndex, i);
                        playSfx(FISHING_ROD_CAST);
                        itemInfo[i].stateIndex = ITEM_STATE_CLEANUP;
                        break;

                    // after throwing item
                    case ITEM_STATE_CLEANUP:

                        // wait 30 frames before removing item
                        if (checkEntityAnimationStateChanged(ENTITY_ITEM_BASE_INDEX + i) || !(itemInfo[i].itemAnimationFrameCounter < 30)) {

                            if (itemInfo[i].flags & ITEM_CONTEXT_ACTIVE) {

                                deactivateEntity(ENTITY_ITEM_BASE_INDEX + i);

                                // FIXME: fake match in order to get float registers right
                                if (y2) {
                                    itemInfo[i].stateIndex = ITEM_STATE_INACTIVE;
                                } else {
                                    itemInfo[i].stateIndex = ITEM_STATE_INACTIVE;
                                }

                                if (z2) {
                                    itemInfo[i].flags = 0;
                                } else {
                                    itemInfo[i].flags = 0;
                                }

                            }
                            
                            if (checkDailyEventBit(HARVEST_GODDESS_OFFERING)) {
                                
                                if (checkSpecialDialogueBit(0x25)) {
                                    
                                    if (!checkSpecialDialogueBit(0x14B)) {
                                        // initial harvest goddess offering
                                         gCutsceneIndex = 344;
                                    } else {
                                         gCutsceneIndex = 341;
                                    }
                                    
                                } else {
                                    gCutsceneIndex = 341;
                                }
                                
                                loadCutscene(0);
                                
                            }
                            
                            if (checkDailyEventBit(KAPPA_FISH_OFFERING_DAILY)) {
                                gCutsceneIndex = 336;
                                loadCutscene(0);
                            }
                            
                            if (checkDailyEventBit(KAPPA_LARGE_FISH_OFFERING_DAILY)) {
                                gCutsceneIndex = 337;
                                loadCutscene(0);
                            } 
                            
                        } else {
                            itemInfo[i].itemAnimationFrameCounter++;
                        }
                        
                        break;
                    
                }
                
            } while (set);

            if (itemInfo[i].flags & ITEM_CONTEXT_USE_POSITION) {
                setEntityCoordinates(ENTITY_ITEM_BASE_INDEX + i, itemInfo[i].position.x, itemInfo[i].position.y, itemInfo[i].position.z);
            }
            
            if (itemInfo[i].flags & ITEM_CONTEXT_USE_ATTACHMENT) {
                setEntityAttachmentOffset(ENTITY_ITEM_BASE_INDEX + i, itemInfo[i].attachmentOffset.x, itemInfo[i].attachmentOffset.y, itemInfo[i].attachmentOffset.z);
            }
            
            setEntityMovementVector(ENTITY_ITEM_BASE_INDEX + i, itemInfo[i].movement.x, 0.0f, itemInfo[i].movement.z, 4.0f);
            setEntityDirection(ENTITY_ITEM_BASE_INDEX + i, entities[ENTITY_PLAYER].direction);
            
        }
        
    }
    
}