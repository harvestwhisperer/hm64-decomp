#include "common.h"

#include "game/items.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/graphic.h"
#include "system/map.h"
#include "system/mapController.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/groundObjects.h"
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

u16 itemSpriteAnimations[96][4] = {
    { 0xFFFF,  0xFFFF,  0xFFFF,  0xFFFF }, /* 0 */
    {      0,      61,      62,      62 }, /* WEED_HELD_ITEM */
    {      1,       1,       1,      66 }, /* ROCK_HELD_ITEM */
    {      2,       2,       2,       2 }, /* LOG_HELD_ITEM */
    {      3,       3,       3,       3 }, /* MOONDROP_FLOWER_HELD_ITEM */
    {      4,       4,       4,       4 }, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
    {      5,       5,       5,       5 }, /* BLUE_MIST_FLOWER_HELD_ITEM */
    {      6,       6,       6,       6 }, /* CAKE_HELD_ITEM */
    {      7,       7,       7,       7 }, /* PIE_HELD_ITEM */
    {      8,       8,       8,       8 }, /* COOKIES_HELD_ITEM */
    {      9,       9,       9,       9 }, /* BLUE_FEATHER_HELD_ITEM */
    {     10,      10,      10,      10 }, /* DOOR_TO_HEAVEN_HELD_ITEM */
    {     11,      11,      11,      11 }, /* 12 */
    {     12,      12,      12,      12 }, /* TURNIP_HELD_ITEM */
    {     13,      13,      13,      13 }, /* POTATO_HELD_ITEM */
    {     14,      14,      14,      14 }, /* CABBAGE_HELD_ITEM */
    {     15,      15,      15,      15 }, /* TOMATO_HELD_ITEM */
    {     16,      16,      16,      16 }, /* CORN_HELD_ITEM */
    {     17,      17,      17,      17 }, /* EGGPLANT_HELD_ITEM */
    {     18,      18,      18,      18 }, /* STRAWBERRY_HELD_ITEM */
    {     19,      19,      19,      19 }, /* EGG_HELD_ITEM */
    {     20,      20,      20,      20 }, /* SMALL_MILK_HELD_ITEM */
    {     21,      21,      21,      21 }, /* MEDIUM_MILK_HELD_ITEM */
    {     22,      22,      22,      22 }, /* LARGE_MILK_HELD_ITEM */
    {     23,      23,      23,      23 }, /* GOLDEN_MILK_HELD_ITEM */
    {     24,      24,      24,      24 }, /* WOOL_HELD_ITEM */
    {     25,      25,      25,      25 }, /* HIGH_QUALITY_WOOL_HELD_ITEM */
    {     26,      26,      26,      26 }, /* WILD_GRAPES_HELD_ITEM */
    {     27,      27,      27,      27 }, /* VERYBERRY_HELD_ITEM */
    {     28,      28,      28,      28 }, /* TROPICAL_FRUIT_HELD_ITEM */
    {     29,      29,      29,      29 }, /* WALNUT_HELD_ITEM */
    {     30,      30,      30,      30 }, /* MUSHROOM_HELD_ITEM */
    {     31,      31,      31,      31 }, /* POISONOUS_MUSHROOM_HELD_ITEM */
    {     32,      32,      32,      32 }, /* BOOK_HELD_ITEM */
    {     33,      33,      33,      33 }, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
    {     34,      34,      34,      34 }, /* MEDICINAL_HERB_HELD_ITEM */
    {     35,      35,      35,      35 }, /* EDIBLE_HERB_HELD_ITEM */
    {     36,      36,      36,      36 }, /* SMALL_FISH_HELD_ITEM */
    {     37,      37,      37,      37 }, /* MEDIUM_FISH_HELD_ITEM */
    {     38,      38,      38,      38 }, /* LARGE_FISH_HELD_ITEM */
    {     39,      39,      39,      39 }, /* DUMPLING_HELD_ITEM */
    {     40,      40,      40,      40 }, /* COTTON_CANDY_HELD_ITEM */
    {     41,      41,      41,      41 }, /* FRIED_OCTUPUS_HELD_ITEM */
    {     42,      42,      42,      42 }, /* ROASTED_CORN_HELD_ITEM */
    {     43,      43,      43,      43 }, /* CANDY_HELD_ITEM */
    {     44,      44,      44,      44 }, /* CHOCOLATE_HELD_ITEM */
    {     45,      45,      45,      45 }, /* IRON_ORE_HELD_ITEM */
    {     46,      46,      46,      46 }, /* BLUE_ROCK_HELD_ITEM */
    {     47,      47,      47,      47 }, /* RARE_METAL_HELD_ITEM */
    {     48,      48,      48,      48 }, /* MOONLIGHT_STONE_HELD_ITEM */
    {     49,      49,      49,      49 }, /* PONTATA_ROOT_HELD_ITEM */
    {     50,      50,      50,      50 }, /* 51 */
    {     51,      51,      51,      51 }, /* 52 */
    {     52,      52,      52,      52 }, /* 53 */
    {     53,      53,      53,      53 }, /* 54 */
    {     54,      54,      54,      54 }, /* 55 */
    {     55,      55,      55,      55 }, /* 56 */
    {     56,      56,      56,      56 }, /* FODDER_HELD_ITEM */
    {     57,      57,      57,      57 }, /* 58 */
    {     58,      58,      58,      58 }, /* 59 */
    {     59,      59,      59,      67 }, /* 60 */
    {     60,      60,      60,      68 }, /* 61 */
    {     63,      64,      65,       0 }, /* 62 */
    {      0,       0,       0,       0 }, /* 63 */
    {      0,       0,       0,       0 }, /* SEEDS_HELD_ITEM */
    {      0,       0,       0,       0 }, /* 65 */
    {      0,       0,       0,       0 }, /* 66 */
    {      0,       0,       0,       0 }, /* 67 */
    {      0,       0,       0,       0 }, /* 68 */
    {      0,       0,       0,       0 }, /* 69 */
    {      0,       0,       0,       0 }, /* 70 */
    {      0,       0,       0,       0 }, /* 71 */
    {      0,       0,       0,       0 }, /* MEDICINE_HELD_ITEM */
    {      0,       0,       0,       0 }, /* SHEARS_HELD_ITEM */
    {      0,       0,       0,       0 }, /* 74 */
    {      0,       0,       0,       0 }, /* 75 */
    {      0,       0,       0,       0 }, /* 76 */
    {      0,       0,       0,       0 }, /* 77 */
    {      0,       0,       0,       0 }, /* 78 */
    {      0,       0,       0,       0 }, /* 79 */
    {      0,       0,       0,       0 }, /* RICE_CAKE_HELD_ITEM */
    {      0,       0,       0,       0 }, /* BREAD_HELD_ITEM */
    {      0,       0,       0,       0 }, /* EGGS_HELD_ITEM */
    {      0,       0,       0,       0 }, /* 83 */
    {      0,       0,       0,       0 }, /* 84 */
    {      0,       0,       0,       0 }, /* 85 */
    {      0,       0,       0,       0 }, /* 86 */
    {      0,       0,       0,       0 }, /* POWER_NUT_HELD_ITEM */
    {      0,       0,       0,       0 }, /* DOG_HELD_ITEM */
    {      0,       0,       0,       0 }, /* 89 */
    {      0,       0,       0,       0 }, /* 90 */
    {      0,       0,       0,       0 }, /* 91 */
    {      0,       0,       0,       0 }, /* 92 */
    {      0,       0,       0,       0 }, /* 93 */
    {      0,       0,       0,       0 }, /* 94 */
    {      0,       0,       0,       0 }, /* 95 */
};

// entity metadata struct
//   entity asset index
//   animation index per season
u16 itemEntityMetadata[224][5] = {
    { ENTITY_ASSET_STATIC_ITEM, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }, /* 0 */
    { ENTITY_ASSET_STATIC_ITEM,     0,   139,   140,   140 }, /* WEED_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     1,     1,     1,     1 }, /* ROCK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     2,     2,     2,     2 }, /* LOG_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     3,     3,     3,     3 }, /* MOONDROP_FLOWER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     4,     4,     4,     4 }, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     5,     5,     5,     5 }, /* BLUE_MIST_FLOWER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     6,     6,     6,     6 }, /* CAKE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     7,     7,     7,     7 }, /* PIE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     8,     8,     8,     8 }, /* COOKIES_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,     9,     9,     9,     9 }, /* BLUE_FEATHER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    10,    10,    10,    10 }, /* DOOR_TO_HEAVEN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    11,    11,    11,    11 }, /* 12 */
    { ENTITY_ASSET_STATIC_ITEM,    12,    12,    12,    12 }, /* TURNIP_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    13,    13,    13,    13 }, /* POTATO_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    14,    14,    14,    14 }, /* CABBAGE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    15,    15,    15,    15 }, /* TOMATO_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    16,    16,    16,    16 }, /* CORN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    17,    17,    17,    17 }, /* EGGPLANT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    18,    18,    18,    18 }, /* STRAWBERRY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    19,    19,    19,    19 }, /* EGG_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    20,    20,    20,    20 }, /* SMALL_MILK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    21,    21,    21,    21 }, /* MEDIUM_MILK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    22,    22,    22,    22 }, /* LARGE_MILK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    23,    23,    23,    23 }, /* GOLDEN_MILK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    24,    24,    24,    24 }, /* WOOL_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    25,    25,    25,    25 }, /* HIGH_QUALITY_WOOL_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    26,    26,    26,    26 }, /* WILD_GRAPES_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    27,    27,    27,    27 }, /* VERYBERRY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    28,    28,    28,    28 }, /* TROPICAL_FRUIT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    29,    29,    29,    29 }, /* WALNUT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    30,    30,    30,    30 }, /* MUSHROOM_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    31,    31,    31,    31 }, /* POISONOUS_MUSHROOM_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    32,    32,    32,    32 }, /* BOOK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    33,    33,    33,    33 }, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    34,    34,    34,    34 }, /* MEDICINAL_HERB_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    35,    35,    35,    35 }, /* EDIBLE_HERB_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    36,    36,    36,    36 }, /* SMALL_FISH_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    37,    37,    37,    37 }, /* MEDIUM_FISH_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    38,    38,    38,    38 }, /* LARGE_FISH_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    39,    39,    39,    39 }, /* DUMPLING_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    40,    40,    40,    40 }, /* COTTON_CANDY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    41,    41,    41,    41 }, /* FRIED_OCTUPUS_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    42,    42,    42,    42 }, /* ROASTED_CORN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    43,    43,    43,    43 }, /* CANDY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    44,    44,    44,    44 }, /* CHOCOLATE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    45,    45,    45,    45 }, /* IRON_ORE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    46,    46,    46,    46 }, /* BLUE_ROCK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    47,    47,    47,    47 }, /* RARE_METAL_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    48,    48,    48,    48 }, /* MOONLIGHT_STONE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    49,    49,    49,    49 }, /* PONTATA_ROOT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    50,    50,    50,    50 }, /* 51 */
    { ENTITY_ASSET_STATIC_ITEM,    51,    51,    51,    51 }, /* 52 */
    { ENTITY_ASSET_STATIC_ITEM,    52,    52,    52,    52 }, /* 53 */
    { ENTITY_ASSET_STATIC_ITEM,    53,    53,    53,    53 }, /* 54 */
    { ENTITY_ASSET_STATIC_ITEM,    54,    54,    54,    54 }, /* 55 */
    { ENTITY_ASSET_STATIC_ITEM,    55,    55,    55,    55 }, /* 56 */
    { ENTITY_ASSET_STATIC_ITEM,    56,    56,    56,    56 }, /* FODDER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    96,    96,    96,    96 }, /* 58 */
    { ENTITY_ASSET_STATIC_ITEM,    97,    97,    97,    97 }, /* 59 */
    { ENTITY_ASSET_STATIC_ITEM,    98,    98,    98,    98 }, /* 60 */
    { ENTITY_ASSET_STATIC_ITEM,    99,    99,    99,    99 }, /* 61 */
    { ENTITY_ASSET_STATIC_ITEM,   100,   100,   100,   100 }, /* 62 */
    { ENTITY_ASSET_STATIC_ITEM,   101,   101,   101,   101 }, /* 63 */
    { ENTITY_ASSET_STATIC_ITEM,   102,   102,   102,   102 }, /* SEEDS_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   103,   103,   103,   103 }, /* 65 */
    { ENTITY_ASSET_STATIC_ITEM,   104,   104,   104,   104 }, /* 66 */
    { ENTITY_ASSET_STATIC_ITEM,   105,   105,   105,   105 }, /* 67 */
    { ENTITY_ASSET_STATIC_ITEM,   106,   106,   106,   106 }, /* 68 */
    { ENTITY_ASSET_STATIC_ITEM,   130,   130,   130,   130 }, /* 69 */
    { ENTITY_ASSET_STATIC_ITEM,   131,   131,   131,   131 }, /* 70 */
    { ENTITY_ASSET_STATIC_ITEM,   132,   132,   132,   132 }, /* 71 */
    { ENTITY_ASSET_STATIC_ITEM,   133,   133,   133,   133 }, /* MEDICINE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   134,   134,   134,   134 }, /* SHEARS_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   135,   135,   135,   135 }, /* 74 */
    { ENTITY_ASSET_STATIC_ITEM,   136,   136,   136,   136 }, /* 75 */
    { ENTITY_ASSET_STATIC_ITEM,   137,   137,   137,   137 }, /* 76 */
    { ENTITY_ASSET_STATIC_ITEM,   138,   138,   138,   138 }, /* 77 */
    { ENTITY_ASSET_STATIC_ITEM,   155,   155,   155,   155 }, /* 78 */
    { ENTITY_ASSET_STATIC_ITEM,   156,   156,   156,   156 }, /* 79 */
    { ENTITY_ASSET_STATIC_ITEM,    57,    57,    57,    57 }, /* RICE_CAKE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    58,    58,    58,    58 }, /* BREAD_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    59,    59,    59,    59 }, /* EGGS_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    60,    60,    60,    60 }, /* 83 */
    { ENTITY_ASSET_STATIC_ITEM,    61,    61,    61,    61 }, /* 84 */
    { ENTITY_ASSET_STATIC_ITEM,    62,    62,    62,    62 }, /* 85 */
    { ENTITY_ASSET_STATIC_ITEM,    63,    63,    63,    63 }, /* 86 */
    { ENTITY_ASSET_STATIC_ITEM,   210,   210,   210,   210 }, /* POWER_NUT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    64,    64,    64,    64 }, /* DOG_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    65,    65,    65,    65 }, /* 89 */
    { ENTITY_ASSET_STATIC_ITEM,    66,    66,    66,    66 }, /* 90 */
    { ENTITY_ASSET_STATIC_ITEM,    67,    67,    67,    67 }, /* 91 */
    { ENTITY_ASSET_STATIC_ITEM,    68,    68,    68,    68 }, /* 92 */
    { ENTITY_ASSET_STATIC_ITEM,    69,    69,    69,    69 }, /* 93 */
    { ENTITY_ASSET_STATIC_ITEM,    70,    70,    70,    70 }, /* 94 */
    { ENTITY_ASSET_STATIC_ITEM,    71,    71,    71,    71 }, /* 95 */
    { ENTITY_ASSET_STATIC_ITEM,    80,    80,    80,    80 }, /* CHICKEN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    81,    81,    81,    81 }, /* 97 */
    { ENTITY_ASSET_STATIC_ITEM,    82,    82,    82,    82 }, /* 98 */
    { ENTITY_ASSET_STATIC_ITEM,    83,    83,    83,    83 }, /* 99 */
    { ENTITY_ASSET_STATIC_ITEM,    84,    84,    84,    84 }, /* 100 */
    { ENTITY_ASSET_STATIC_ITEM,    85,    85,    85,    85 }, /* 101 */
    { ENTITY_ASSET_STATIC_ITEM,    86,    86,    86,    86 }, /* 102 */
    { ENTITY_ASSET_STATIC_ITEM,    87,    87,    87,    87 }, /* 103 */
    { ENTITY_ASSET_STATIC_ITEM,    88,    88,    88,    88 }, /* CHICK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    89,    89,    89,    89 }, /* 105 */
    { ENTITY_ASSET_STATIC_ITEM,    90,    90,    90,    90 }, /* 106 */
    { ENTITY_ASSET_STATIC_ITEM,    91,    91,    91,    91 }, /* 107 */
    { ENTITY_ASSET_STATIC_ITEM,    92,    92,    92,    92 }, /* 108 */
    { ENTITY_ASSET_STATIC_ITEM,    93,    93,    93,    93 }, /* 109 */
    { ENTITY_ASSET_STATIC_ITEM,    94,    94,    94,    94 }, /* 110 */
    { ENTITY_ASSET_STATIC_ITEM,    95,    95,    95,    95 }, /* 111 */
    { ENTITY_ASSET_STATIC_ITEM,   112,   112,   112,   112 }, /* EMPTY_BOTTLE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   141,   141,   141,   141 }, /* BOTTLE_WITH_WATER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   109,   109,   109,   109 }, /* BOTTLE_WITH_WINE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   111,   111,   111,   111 }, /* 115 */
    { ENTITY_ASSET_STATIC_ITEM,   110,   110,   110,   110 }, /* 116 */
    { ENTITY_ASSET_STATIC_ITEM,   110,   110,   110,   110 }, /* 117 */
    { ENTITY_ASSET_STATIC_ITEM,   113,   113,   113,   113 }, /* BOTTLE_WITH_MILK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   110,   110,   110,   110 }, /* 119 */
    { ENTITY_ASSET_STATIC_ITEM,   109,   109,   109,   109 }, /* BOTTLE_WITH_TEA_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   109,   109,   109,   109 }, /* 121 */
    { ENTITY_ASSET_STATIC_ITEM,   110,   110,   110,   110 }, /* 122 */
    { ENTITY_ASSET_STATIC_ITEM,    72,    72,    72,    72 }, /* PUPPY_1_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    73,    73,    73,    73 }, /* 124 */
    { ENTITY_ASSET_STATIC_ITEM,    74,    74,    74,    74 }, /* 125 */
    { ENTITY_ASSET_STATIC_ITEM,    75,    75,    75,    75 }, /* 126 */
    { ENTITY_ASSET_STATIC_ITEM,    76,    76,    76,    76 }, /* 127 */
    { ENTITY_ASSET_STATIC_ITEM,    77,    77,    77,    77 }, /* 128 */
    { ENTITY_ASSET_STATIC_ITEM,    78,    78,    78,    78 }, /* 129 */
    { ENTITY_ASSET_STATIC_ITEM,    79,    79,    79,    79 }, /* 130 */
    { ENTITY_ASSET_STATIC_ITEM,   195,   195,   195,   195 }, /* CAT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   196,   196,   196,   196 }, /* 132 */
    { ENTITY_ASSET_STATIC_ITEM,   197,   197,   197,   197 }, /* 133 */
    { ENTITY_ASSET_STATIC_ITEM,   198,   198,   198,   198 }, /* 134 */
    { ENTITY_ASSET_STATIC_ITEM,   199,   199,   199,   199 }, /* 135 */
    { ENTITY_ASSET_STATIC_ITEM,   200,   200,   200,   200 }, /* 136 */
    { ENTITY_ASSET_STATIC_ITEM,   201,   201,   201,   201 }, /* 137 */
    { ENTITY_ASSET_STATIC_ITEM,   202,   202,   202,   202 }, /* 138 */
    { ENTITY_ASSET_STATIC_ITEM,   163,   163,   163,   163 }, /* FOX_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   164,   164,   164,   164 }, /* 140 */
    { ENTITY_ASSET_STATIC_ITEM,   165,   165,   165,   165 }, /* 141 */
    { ENTITY_ASSET_STATIC_ITEM,   166,   166,   166,   166 }, /* 142 */
    { ENTITY_ASSET_STATIC_ITEM,   167,   167,   167,   167 }, /* 143 */
    { ENTITY_ASSET_STATIC_ITEM,   168,   168,   168,   168 }, /* 144 */
    { ENTITY_ASSET_STATIC_ITEM,   169,   169,   169,   169 }, /* 145 */
    { ENTITY_ASSET_STATIC_ITEM,   170,   170,   170,   170 }, /* 146 */
    { ENTITY_ASSET_STATIC_ITEM,   171,   171,   171,   171 }, /* SQUIRREL_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   172,   172,   172,   172 }, /* 148 */
    { ENTITY_ASSET_STATIC_ITEM,   173,   173,   173,   173 }, /* 149 */
    { ENTITY_ASSET_STATIC_ITEM,   174,   174,   174,   174 }, /* 150 */
    { ENTITY_ASSET_STATIC_ITEM,   175,   175,   175,   175 }, /* 151 */
    { ENTITY_ASSET_STATIC_ITEM,   176,   176,   176,   176 }, /* 152 */
    { ENTITY_ASSET_STATIC_ITEM,   177,   177,   177,   177 }, /* 153 */
    { ENTITY_ASSET_STATIC_ITEM,   178,   178,   178,   178 }, /* 154 */
    { ENTITY_ASSET_STATIC_ITEM,   179,   179,   179,   179 }, /* RABBIT_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   180,   180,   180,   180 }, /* 156 */
    { ENTITY_ASSET_STATIC_ITEM,   181,   181,   181,   181 }, /* 157 */
    { ENTITY_ASSET_STATIC_ITEM,   182,   182,   182,   182 }, /* 158 */
    { ENTITY_ASSET_STATIC_ITEM,   183,   183,   183,   183 }, /* 159 */
    { ENTITY_ASSET_STATIC_ITEM,   184,   184,   184,   184 }, /* 160 */
    { ENTITY_ASSET_STATIC_ITEM,   185,   185,   185,   185 }, /* 161 */
    { ENTITY_ASSET_STATIC_ITEM,   186,   186,   186,   186 }, /* 162 */
    { ENTITY_ASSET_STATIC_ITEM,   187,   187,   187,   187 }, /* MONKEY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   188,   188,   188,   188 }, /* 164 */
    { ENTITY_ASSET_STATIC_ITEM,   189,   189,   189,   189 }, /* 165 */
    { ENTITY_ASSET_STATIC_ITEM,   190,   190,   190,   190 }, /* 166 */
    { ENTITY_ASSET_STATIC_ITEM,   191,   191,   191,   191 }, /* 167 */
    { ENTITY_ASSET_STATIC_ITEM,   192,   192,   192,   192 }, /* 168 */
    { ENTITY_ASSET_STATIC_ITEM,   193,   193,   193,   193 }, /* 169 */
    { ENTITY_ASSET_STATIC_ITEM,   194,   194,   194,   194 }, /* 170 */
    { ENTITY_ASSET_STATIC_ITEM,    64,    64,    64,    64 }, /* DOG_2_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,    65,    65,    65,    65 }, /* 172 */
    { ENTITY_ASSET_STATIC_ITEM,    66,    66,    66,    66 }, /* 173 */
    { ENTITY_ASSET_STATIC_ITEM,    67,    67,    67,    67 }, /* 174 */
    { ENTITY_ASSET_STATIC_ITEM,    68,    68,    68,    68 }, /* 175 */
    { ENTITY_ASSET_STATIC_ITEM,    69,    69,    69,    69 }, /* 176 */
    { ENTITY_ASSET_STATIC_ITEM,    70,    70,    70,    70 }, /* 177 */
    { ENTITY_ASSET_STATIC_ITEM,    71,    71,    71,    71 }, /* 178 */
    { ENTITY_ASSET_STATIC_ITEM,   203,   203,   203,   203 }, /* BUTTERFLY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   204,   204,   204,   204 }, /* LADYBUG_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   205,   205,   205,   205 }, /* CICADA_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   206,   206,   206,   206 }, /* HORNED_BEETLE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   207,   207,   207,   207 }, /* STAG_BEETLE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   208,   208,   208,   208 }, /* RED_DRAGONFLY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   209,   209,   209,   209 }, /* CRICKET_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   213,   213,   213,   213 }, /* BABY_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   214,   214,   214,   214 }, /* 187 */
    { ENTITY_ASSET_STATIC_ITEM,   215,   215,   215,   215 }, /* 188 */
    { ENTITY_ASSET_STATIC_ITEM,   216,   216,   216,   216 }, /* 189 */
    { ENTITY_ASSET_STATIC_ITEM,   217,   217,   217,   217 }, /* 190 */
    { ENTITY_ASSET_STATIC_ITEM,   218,   218,   218,   218 }, /* 191 */
    { ENTITY_ASSET_STATIC_ITEM,   219,   219,   219,   219 }, /* 192 */
    { ENTITY_ASSET_STATIC_ITEM,   220,   220,   220,   220 }, /* 193 */
    { ENTITY_ASSET_STATIC_ITEM,   221,   221,   221,   221 }, /* BABY_GROWN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   222,   222,   222,   222 }, /* 195 */
    { ENTITY_ASSET_STATIC_ITEM,   223,   223,   223,   223 }, /* 196 */
    { ENTITY_ASSET_STATIC_ITEM,   224,   224,   224,   224 }, /* 197 */
    { ENTITY_ASSET_STATIC_ITEM,   225,   225,   225,   225 }, /* 198 */
    { ENTITY_ASSET_STATIC_ITEM,   226,   226,   226,   226 }, /* 199 */
    { ENTITY_ASSET_STATIC_ITEM,   227,   227,   227,   227 }, /* 200 */
    { ENTITY_ASSET_STATIC_ITEM,   228,   228,   228,   228 }, /* 201 */
    { ENTITY_ASSET_STATIC_ITEM,   231,   231,   231,   231 }, /* OPEN_BOOK_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   234,   234,   234,   234 }, /* SICKLE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   235,   235,   235,   235 }, /* SILVER_SICKLE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   236,   236,   236,   236 }, /* GOLDEN_SICKLE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   237,   237,   237,   237 }, /* HOE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   238,   238,   238,   238 }, /* SILVER_HOE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   239,   239,   239,   239 }, /* GOLDEN_HOE_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   240,   240,   240,   240 }, /* AX_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   241,   241,   241,   241 }, /* SILVER_AX_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   242,   242,   242,   242 }, /* GOLDEN_AX_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   243,   243,   243,   243 }, /* HAMMER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   244,   244,   244,   244 }, /* SILVER_HAMMER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   245,   245,   245,   245 }, /* GOLDEN_HAMMER_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   246,   246,   246,   246 }, /* WATERING_CAN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   247,   247,   247,   247 }, /* SILVER_WATERING_CAN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   248,   248,   248,   248 }, /* GOLDEN_WATERING_CAN_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   144,   144,   144,   144 }, /* BROKEN_MUSIC_BOX_HELD_ITEM */
    { ENTITY_ASSET_STATIC_ITEM,   149,   149,   149,   149 }, /* 219 */
    { ENTITY_ASSET_STATIC_ITEM,   147,   147,   147,   147 }, /* 220 */
    { 0, 0, 0, 0, 0 }, /* 221 */
    { 0, 0, 0, 0, 0 }, /* 222 */
    { 0, 0, 0, 0, 0 }, /* 223 */
};

// entity palette index overrides (0xFFFF = use default)
u16 itemPaletteOverrides[] = {
    0xFFFF, /* 0 */
    0xFFFF, /* WEED_HELD_ITEM */
    0xFFFF, /* ROCK_HELD_ITEM */
    0xFFFF, /* LOG_HELD_ITEM */
    0xFFFF, /* MOONDROP_FLOWER_HELD_ITEM */
    0xFFFF, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
    0xFFFF, /* BLUE_MIST_FLOWER_HELD_ITEM */
    0xFFFF, /* CAKE_HELD_ITEM */
    0xFFFF, /* PIE_HELD_ITEM */
    0xFFFF, /* COOKIES_HELD_ITEM */
    0xFFFF, /* BLUE_FEATHER_HELD_ITEM */
    0xFFFF, /* DOOR_TO_HEAVEN_HELD_ITEM */
    0xFFFF, /* 12 */
    0xFFFF, /* TURNIP_HELD_ITEM */
    0xFFFF, /* POTATO_HELD_ITEM */
    0xFFFF, /* CABBAGE_HELD_ITEM */
    0xFFFF, /* TOMATO_HELD_ITEM */
    0xFFFF, /* CORN_HELD_ITEM */
    0xFFFF, /* EGGPLANT_HELD_ITEM */
    0xFFFF, /* STRAWBERRY_HELD_ITEM */
    0xFFFF, /* EGG_HELD_ITEM */
    0xFFFF, /* SMALL_MILK_HELD_ITEM */
    0xFFFF, /* MEDIUM_MILK_HELD_ITEM */
    0xFFFF, /* LARGE_MILK_HELD_ITEM */
    0xFFFF, /* GOLDEN_MILK_HELD_ITEM */
    0xFFFF, /* WOOL_HELD_ITEM */
    0xFFFF, /* HIGH_QUALITY_WOOL_HELD_ITEM */
    0xFFFF, /* WILD_GRAPES_HELD_ITEM */
    0xFFFF, /* VERYBERRY_HELD_ITEM */
    0xFFFF, /* TROPICAL_FRUIT_HELD_ITEM */
    0xFFFF, /* WALNUT_HELD_ITEM */
    0xFFFF, /* MUSHROOM_HELD_ITEM */
    0xFFFF, /* POISONOUS_MUSHROOM_HELD_ITEM */
    0xFFFF, /* BOOK_HELD_ITEM */
    0xFFFF, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
    0xFFFF, /* MEDICINAL_HERB_HELD_ITEM */
    0xFFFF, /* EDIBLE_HERB_HELD_ITEM */
    0xFFFF, /* SMALL_FISH_HELD_ITEM */
    0xFFFF, /* MEDIUM_FISH_HELD_ITEM */
    0xFFFF, /* LARGE_FISH_HELD_ITEM */
    0xFFFF, /* DUMPLING_HELD_ITEM */
    0xFFFF, /* COTTON_CANDY_HELD_ITEM */
    0xFFFF, /* FRIED_OCTUPUS_HELD_ITEM */
    0xFFFF, /* ROASTED_CORN_HELD_ITEM */
    0xFFFF, /* CANDY_HELD_ITEM */
    0xFFFF, /* CHOCOLATE_HELD_ITEM */
    0xFFFF, /* IRON_ORE_HELD_ITEM */
    0xFFFF, /* BLUE_ROCK_HELD_ITEM */
    0xFFFF, /* RARE_METAL_HELD_ITEM */
    0xFFFF, /* MOONLIGHT_STONE_HELD_ITEM */
    0xFFFF, /* PONTATA_ROOT_HELD_ITEM */
    0xFFFF, /* 51 */
    0xFFFF, /* 52 */
    0xFFFF, /* 53 */
    0xFFFF, /* 54 */
    0xFFFF, /* 55 */
    0xFFFF, /* 56 */
    0xFFFF, /* FODDER_HELD_ITEM */
    0xFFFF, /* 58 */
    0xFFFF, /* 59 */
    0xFFFF, /* 60 */
    0xFFFF, /* 61 */
    0xFFFF, /* 62 */
    0xFFFF, /* 63 */
    0xFFFF, /* SEEDS_HELD_ITEM */
    0xFFFF, /* 65 */
    0xFFFF, /* 66 */
    0xFFFF, /* 67 */
    0xFFFF, /* 68 */
    0xFFFF, /* 69 */
    0xFFFF, /* 70 */
    0xFFFF, /* 71 */
    0xFFFF, /* MEDICINE_HELD_ITEM */
    0xFFFF, /* SHEARS_HELD_ITEM */
    0xFFFF, /* 74 */
    0xFFFF, /* 75 */
    0xFFFF, /* 76 */
    0xFFFF, /* 77 */
    0xFFFF, /* 78 */
    0xFFFF, /* 79 */
    0xFFFF, /* RICE_CAKE_HELD_ITEM */
    0xFFFF, /* BREAD_HELD_ITEM */
    0xFFFF, /* EGGS_HELD_ITEM */
    0xFFFF, /* 83 */
    0xFFFF, /* 84 */
    0xFFFF, /* 85 */
    0xFFFF, /* 86 */
    0xFFFF, /* POWER_NUT_HELD_ITEM */
    0xFFFF, /* DOG_HELD_ITEM */
    0xFFFF, /* 89 */
    0xFFFF, /* 90 */
    0xFFFF, /* 91 */
    0xFFFF, /* 92 */
    0xFFFF, /* 93 */
    0xFFFF, /* 94 */
    0xFFFF, /* 95 */
    0xFFFF, /* CHICKEN_HELD_ITEM */
    0xFFFF, /* 97 */
    0xFFFF, /* 98 */
    0xFFFF, /* 99 */
    0xFFFF, /* 100 */
    0xFFFF, /* 101 */
    0xFFFF, /* 102 */
    0xFFFF, /* 103 */
    0xFFFF, /* CHICK_HELD_ITEM */
    0xFFFF, /* 105 */
    0xFFFF, /* 106 */
    0xFFFF, /* 107 */
    0xFFFF, /* 108 */
    0xFFFF, /* 109 */
    0xFFFF, /* 110 */
    0xFFFF, /* 111 */
    0xFFFF, /* EMPTY_BOTTLE_HELD_ITEM */
    0xFFFF, /* BOTTLE_WITH_WATER_HELD_ITEM */
    0xFFFF, /* BOTTLE_WITH_WINE_HELD_ITEM */
    0xFFFF, /* 115 */
    0xFFFF, /* 116 */
    0xFFFF, /* 117 */
    0xFFFF, /* BOTTLE_WITH_MILK_HELD_ITEM */
    0xFFFF, /* 119 */
    0xFFFF, /* BOTTLE_WITH_TEA_HELD_ITEM */
    0xFFFF, /* 121 */
    0xFFFF, /* 122 */
    0xFFFF, /* PUPPY_1_HELD_ITEM */
    0xFFFF, /* 124 */
    0xFFFF, /* 125 */
    0xFFFF, /* 126 */
    0xFFFF, /* 127 */
    0xFFFF, /* 128 */
    0xFFFF, /* 129 */
    0xFFFF, /* 130 */
    0xFFFF, /* CAT_HELD_ITEM */
    0xFFFF, /* 132 */
    0xFFFF, /* 133 */
    0xFFFF, /* 134 */
    0xFFFF, /* 135 */
    0xFFFF, /* 136 */
    0xFFFF, /* 137 */
    0xFFFF, /* 138 */
    0xFFFF, /* FOX_HELD_ITEM */
    0xFFFF, /* 140 */
    0xFFFF, /* 141 */
    0xFFFF, /* 142 */
    0xFFFF, /* 143 */
    0xFFFF, /* 144 */
    0xFFFF, /* 145 */
    0xFFFF, /* 146 */
    0xFFFF, /* SQUIRREL_HELD_ITEM */
    0xFFFF, /* 148 */
    0xFFFF, /* 149 */
    0xFFFF, /* 150 */
    0xFFFF, /* 151 */
    0xFFFF, /* 152 */
    0xFFFF, /* 153 */
    0xFFFF, /* 154 */
    0xFFFF, /* RABBIT_HELD_ITEM */
    0xFFFF, /* 156 */
    0xFFFF, /* 157 */
    0xFFFF, /* 158 */
    0xFFFF, /* 159 */
    0xFFFF, /* 160 */
    0xFFFF, /* 161 */
    0xFFFF, /* 162 */
    0xFFFF, /* MONKEY_HELD_ITEM */
    0xFFFF, /* 164 */
    0xFFFF, /* 165 */
    0xFFFF, /* 166 */
    0xFFFF, /* 167 */
    0xFFFF, /* 168 */
    0xFFFF, /* 169 */
    0xFFFF, /* 170 */
       150, /* DOG_2_HELD_ITEM */
       150, /* 172 */
       150, /* 173 */
       150, /* 174 */
       150, /* 175 */
       150, /* 176 */
       150, /* 177 */
       150, /* 178 */
    0xFFFF, /* BUTTERFLY_HELD_ITEM */
    0xFFFF, /* LADYBUG_HELD_ITEM */
    0xFFFF, /* CICADA_HELD_ITEM */
    0xFFFF, /* HORNED_BEETLE_HELD_ITEM */
    0xFFFF, /* STAG_BEETLE_HELD_ITEM */
    0xFFFF, /* RED_DRAGONFLY_HELD_ITEM */
    0xFFFF, /* CRICKET_HELD_ITEM */
    0xFFFF, /* BABY_HELD_ITEM */
    0xFFFF, /* 187 */
    0xFFFF, /* 188 */
    0xFFFF, /* 189 */
    0xFFFF, /* 190 */
    0xFFFF, /* 191 */
    0xFFFF, /* 192 */
    0xFFFF, /* 193 */
    0xFFFF, /* BABY_GROWN_HELD_ITEM */
    0xFFFF, /* 195 */
    0xFFFF, /* 196 */
    0xFFFF, /* 197 */
    0xFFFF, /* 198 */
    0xFFFF, /* 199 */
    0xFFFF, /* 200 */
    0xFFFF, /* 201 */
    0xFFFF, /* OPEN_BOOK_HELD_ITEM */
    0xFFFF, /* SICKLE_HELD_ITEM */
    0xFFFF, /* SILVER_SICKLE_HELD_ITEM */
    0xFFFF, /* GOLDEN_SICKLE_HELD_ITEM */
    0xFFFF, /* HOE_HELD_ITEM */
    0xFFFF, /* SILVER_HOE_HELD_ITEM */
    0xFFFF, /* GOLDEN_HOE_HELD_ITEM */
    0xFFFF, /* AX_HELD_ITEM */
    0xFFFF, /* SILVER_AX_HELD_ITEM */
    0xFFFF, /* GOLDEN_AX_HELD_ITEM */
    0xFFFF, /* HAMMER_HELD_ITEM */
    0xFFFF, /* SILVER_HAMMER_HELD_ITEM */
    0xFFFF, /* GOLDEN_HAMMER_HELD_ITEM */
    0xFFFF, /* WATERING_CAN_HELD_ITEM */
    0xFFFF, /* SILVER_WATERING_CAN_HELD_ITEM */
    0xFFFF, /* GOLDEN_WATERING_CAN_HELD_ITEM */
    0xFFFF, /* BROKEN_MUSIC_BOX_HELD_ITEM */
    0xFFFF, /* 219 */
    0xFFFF, /* 220 */
        0, /* 221 */
        0, /* 222 */
        0, /* 223 */
};

u8 heldItemDialogueItemIndices[] = {
     0, /* 0 */
     HELD_ITEM_DIALOGUE_WEED, /* WEED_HELD_ITEM */
     HELD_ITEM_DIALOGUE_ROCK, /* ROCK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_LOG, /* LOG_HELD_ITEM */
     HELD_ITEM_DIALOGUE_MOONDROP_FLOWER, /* MOONDROP_FLOWER_HELD_ITEM */
     HELD_ITEM_DIALOGUE_PINK_CAT_MINT_FLOWER, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
     0, /* BLUE_MIST_FLOWER_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CAKE, /* CAKE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_PIE, /* PIE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_COOKIES, /* COOKIES_HELD_ITEM */
     50, /* BLUE_FEATHER_HELD_ITEM */
     0, /* DOOR_TO_HEAVEN_HELD_ITEM */
     0, /* 12 */
     HELD_ITEM_DIALOGUE_TURNIP, /* TURNIP_HELD_ITEM */
     HELD_ITEM_DIALOGUE_POTATO, /* POTATO_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CABBAGE, /* CABBAGE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_TOMATO, /* TOMATO_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CORN, /* CORN_HELD_ITEM */
     HELD_ITEM_DIALOGUE_EGGPLANT, /* EGGPLANT_HELD_ITEM */
     HELD_ITEM_DIALOGUE_STRAWBERRY, /* STRAWBERRY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_EGG, /* EGG_HELD_ITEM */
     HELD_ITEM_DIALOGUE_SMALL_MILK, /* SMALL_MILK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_MEDIUM_MILK, /* MEDIUM_MILK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_LARGE_MILK, /* LARGE_MILK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_GOLDEN_MILK, /* GOLDEN_MILK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_WOOL, /* WOOL_HELD_ITEM */
     HELD_ITEM_DIALOGUE_HIGH_QUALITY_WOOL, /* HIGH_QUALITY_WOOL_HELD_ITEM */
     HELD_ITEM_DIALOGUE_WILD_GRAPES, /* WILD_GRAPES_HELD_ITEM */
     HELD_ITEM_DIALOGUE_VERYBERRY, /* VERYBERRY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_TROPICAL_FRUIT, /* TROPICAL_FRUIT_HELD_ITEM */
     HELD_ITEM_DIALOGUE_WALNUT, /* WALNUT_HELD_ITEM */
     HELD_ITEM_DIALOGUE_MUSHROOM, /* MUSHROOM_HELD_ITEM */
     HELD_ITEM_DIALOGUE_POISONOUS_MUSHROOM, /* POISONOUS_MUSHROOM_HELD_ITEM */
     0, /* BOOK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_BERRY_OF_THE_FULLMOON, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
     HELD_ITEM_DIALOGUE_MEDICINAL_HERB, /* MEDICINAL_HERB_HELD_ITEM */
     HELD_ITEM_DIALOGUE_EDIBLE_HERB, /* EDIBLE_HERB_HELD_ITEM */
     HELD_ITEM_DIALOGUE_SMALL_FISH, /* SMALL_FISH_HELD_ITEM */
     HELD_ITEM_DIALOGUE_MEDIUM_FISH, /* MEDIUM_FISH_HELD_ITEM */
     HELD_ITEM_DIALOGUE_LARGE_FISH, /* LARGE_FISH_HELD_ITEM */
     HELD_ITEM_DIALOGUE_DUMPLING, /* DUMPLING_HELD_ITEM */
     HELD_ITEM_DIALOGUE_COTTON_CANDY, /* COTTON_CANDY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_FRIED_OCTUPUS, /* FRIED_OCTUPUS_HELD_ITEM */
     0, /* ROASTED_CORN_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CANDY, /* CANDY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CHOCOLATE, /* CHOCOLATE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_IRON_ORE, /* IRON_ORE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_BLUE_ROCK, /* BLUE_ROCK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_RARE_METAL, /* RARE_METAL_HELD_ITEM */
     HELD_ITEM_DIALOGUE_MOONLIGHT_STONE, /* MOONLIGHT_STONE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_PONTATA_ROOT, /* PONTATA_ROOT_HELD_ITEM */
     0, /* 51 */
     0, /* 52 */
     0, /* 53 */
     0, /* 54 */
     0, /* 55 */
     0, /* 56 */
     HELD_ITEM_DIALOGUE_FODDER, /* FODDER_HELD_ITEM */
     0, /* 58 */
     0, /* 59 */
     0, /* 60 */
     0, /* 61 */
     0, /* 62 */
     0, /* 63 */
     0, /* SEEDS_HELD_ITEM */
     0, /* 65 */
     0, /* 66 */
     0, /* 67 */
     0, /* 68 */
     0, /* 69 */
     0, /* 70 */
     0, /* 71 */
     0, /* MEDICINE_HELD_ITEM */
     0, /* SHEARS_HELD_ITEM */
     0, /* 74 */
     0, /* 75 */
     0, /* 76 */
     0, /* 77 */
     0, /* 78 */
     0, /* 79 */
     0, /* RICE_CAKE_HELD_ITEM */
     0, /* BREAD_HELD_ITEM */
     0, /* EGGS_HELD_ITEM */
     0, /* 83 */
     0, /* 84 */
     0, /* 85 */
     0, /* 86 */
     0, /* POWER_NUT_HELD_ITEM */
     HELD_ITEM_DIALOGUE_DOG, /* DOG_HELD_ITEM */
     HELD_ITEM_DIALOGUE_DOG, /* 89 */
     HELD_ITEM_DIALOGUE_DOG, /* 90 */
     HELD_ITEM_DIALOGUE_DOG, /* 91 */
     HELD_ITEM_DIALOGUE_DOG, /* 92 */
     HELD_ITEM_DIALOGUE_DOG, /* 93 */
     HELD_ITEM_DIALOGUE_DOG, /* 94 */
     HELD_ITEM_DIALOGUE_DOG, /* 95 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* CHICKEN_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 97 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 98 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 99 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 100 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 101 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 102 */
     HELD_ITEM_DIALOGUE_CHICKEN, /* 103 */
     HELD_ITEM_DIALOGUE_CHICK, /* CHICK_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CHICK, /* 105 */
     HELD_ITEM_DIALOGUE_CHICK, /* 106 */
     HELD_ITEM_DIALOGUE_CHICK, /* 107 */
     HELD_ITEM_DIALOGUE_CHICK, /* 108 */
     HELD_ITEM_DIALOGUE_CHICK, /* 109 */
     HELD_ITEM_DIALOGUE_CHICK, /* 110 */
     HELD_ITEM_DIALOGUE_CHICK, /* 111 */
     0, /* EMPTY_BOTTLE_HELD_ITEM */
     0, /* BOTTLE_WITH_WATER_HELD_ITEM */
     0, /* BOTTLE_WITH_WINE_HELD_ITEM */
     0, /* 115 */
     0, /* 116 */
     0, /* 117 */
     0, /* BOTTLE_WITH_MILK_HELD_ITEM */
     0, /* 119 */
     0, /* BOTTLE_WITH_TEA_HELD_ITEM */
     0, /* 121 */
     0, /* 122 */
     HELD_ITEM_DIALOGUE_PUPPY, /* PUPPY_1_HELD_ITEM */
     HELD_ITEM_DIALOGUE_PUPPY, /* 124 */
     HELD_ITEM_DIALOGUE_PUPPY, /* 125 */
     HELD_ITEM_DIALOGUE_PUPPY, /* 126 */
     HELD_ITEM_DIALOGUE_PUPPY, /* 127 */
     HELD_ITEM_DIALOGUE_PUPPY, /* 128 */
     HELD_ITEM_DIALOGUE_PUPPY, /* 129 */
     HELD_ITEM_DIALOGUE_PUPPY, /* 130 */
     64, /* CAT_HELD_ITEM */
     64, /* 132 */
     64, /* 133 */
     64, /* 134 */
     64, /* 135 */
     64, /* 136 */
     64, /* 137 */
     64, /* 138 */
     64, /* FOX_HELD_ITEM */
     64, /* 140 */
     64, /* 141 */
     64, /* 142 */
     64, /* 143 */
     64, /* 144 */
     64, /* 145 */
     64, /* 146 */
     64, /* SQUIRREL_HELD_ITEM */
     64, /* 148 */
     64, /* 149 */
     64, /* 150 */
     64, /* 151 */
     64, /* 152 */
     64, /* 153 */
     64, /* 154 */
     64, /* RABBIT_HELD_ITEM */
     64, /* 156 */
     64, /* 157 */
     64, /* 158 */
     64, /* 159 */
     64, /* 160 */
     64, /* 161 */
     64, /* 162 */
     64, /* MONKEY_HELD_ITEM */
     64, /* 164 */
     64, /* 165 */
     64, /* 166 */
     64, /* 167 */
     64, /* 168 */
     64, /* 169 */
     64, /* 170 */
     HELD_ITEM_DIALOGUE_DOG_2, /* DOG_2_HELD_ITEM */
     HELD_ITEM_DIALOGUE_DOG_2, /* 172 */
     HELD_ITEM_DIALOGUE_DOG_2, /* 173 */
     HELD_ITEM_DIALOGUE_DOG_2, /* 174 */
     HELD_ITEM_DIALOGUE_DOG_2, /* 175 */
     HELD_ITEM_DIALOGUE_DOG_2, /* 176 */
     HELD_ITEM_DIALOGUE_DOG_2, /* 177 */
     HELD_ITEM_DIALOGUE_DOG_2, /* 178 */
     HELD_ITEM_DIALOGUE_BUTTERFLY, /* BUTTERFLY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_LADYBUG, /* LADYBUG_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CICADA, /* CICADA_HELD_ITEM */
     HELD_ITEM_DIALOGUE_HORNED_BEETLE, /* HORNED_BEETLE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_STAG_BEETLE, /* STAG_BEETLE_HELD_ITEM */
     HELD_ITEM_DIALOGUE_RED_DRAGONFLY, /* RED_DRAGONFLY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_CRICKET, /* CRICKET_HELD_ITEM */
     HELD_ITEM_DIALOGUE_BABY, /* BABY_HELD_ITEM */
     HELD_ITEM_DIALOGUE_BABY, /* 187 */
     HELD_ITEM_DIALOGUE_BABY, /* 188 */
     HELD_ITEM_DIALOGUE_BABY, /* 189 */
     HELD_ITEM_DIALOGUE_BABY, /* 190 */
     HELD_ITEM_DIALOGUE_BABY, /* 191 */
     HELD_ITEM_DIALOGUE_BABY, /* 192 */
     HELD_ITEM_DIALOGUE_BABY, /* 193 */
     HELD_ITEM_DIALOGUE_BABY, /* BABY_GROWN_HELD_ITEM */
     HELD_ITEM_DIALOGUE_BABY, /* 195 */
     HELD_ITEM_DIALOGUE_BABY, /* 196 */
     HELD_ITEM_DIALOGUE_BABY, /* 197 */
     HELD_ITEM_DIALOGUE_BABY, /* 198 */
     HELD_ITEM_DIALOGUE_BABY, /* 199 */
     HELD_ITEM_DIALOGUE_BABY, /* 200 */
     HELD_ITEM_DIALOGUE_BABY, /* 201 */
     0, /* OPEN_BOOK_HELD_ITEM */
     0, /* SICKLE_HELD_ITEM */
     0, /* SILVER_SICKLE_HELD_ITEM */
     0, /* GOLDEN_SICKLE_HELD_ITEM */
     0, /* HOE_HELD_ITEM */
     0, /* SILVER_HOE_HELD_ITEM */
     0, /* GOLDEN_HOE_HELD_ITEM */
     0, /* AX_HELD_ITEM */
     0, /* SILVER_AX_HELD_ITEM */
     0, /* GOLDEN_AX_HELD_ITEM */
     0, /* HAMMER_HELD_ITEM */
     0, /* SILVER_HAMMER_HELD_ITEM */
     0, /* GOLDEN_HAMMER_HELD_ITEM */
     0, /* WATERING_CAN_HELD_ITEM */
     0, /* SILVER_WATERING_CAN_HELD_ITEM */
     0, /* GOLDEN_WATERING_CAN_HELD_ITEM */
     0, /* BROKEN_MUSIC_BOX_HELD_ITEM */
     0, /* 219 */
     0, /* 220 */
     0, /* 221 */
     0, /* 222 */
     0, /* 223 */
};

u16 itemTextIndices[] = {
       0, /* 0 */
     315, /* WEED_HELD_ITEM */
     314, /* ROCK_HELD_ITEM */
     309, /* LOG_HELD_ITEM */
     285, /* MOONDROP_FLOWER_HELD_ITEM */
     286, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
       0, /* BLUE_MIST_FLOWER_HELD_ITEM */
     287, /* CAKE_HELD_ITEM */
     288, /* PIE_HELD_ITEM */
     289, /* COOKIES_HELD_ITEM */
     317, /* BLUE_FEATHER_HELD_ITEM */
       0, /* DOOR_TO_HEAVEN_HELD_ITEM */
       0, /* 12 */
     273, /* TURNIP_HELD_ITEM */
     274, /* POTATO_HELD_ITEM */
     275, /* CABBAGE_HELD_ITEM */
     276, /* TOMATO_HELD_ITEM */
     277, /* CORN_HELD_ITEM */
     278, /* EGGPLANT_HELD_ITEM */
     279, /* STRAWBERRY_HELD_ITEM */
     280, /* EGG_HELD_ITEM */
     281, /* SMALL_MILK_HELD_ITEM */
     282, /* MEDIUM_MILK_HELD_ITEM */
     283, /* LARGE_MILK_HELD_ITEM */
     284, /* GOLDEN_MILK_HELD_ITEM */
     290, /* WOOL_HELD_ITEM */
     291, /* HIGH_QUALITY_WOOL_HELD_ITEM */
     292, /* WILD_GRAPES_HELD_ITEM */
     293, /* VERYBERRY_HELD_ITEM */
     294, /* TROPICAL_FRUIT_HELD_ITEM */
     295, /* WALNUT_HELD_ITEM */
     296, /* MUSHROOM_HELD_ITEM */
     316, /* POISONOUS_MUSHROOM_HELD_ITEM */
       0, /* BOOK_HELD_ITEM */
     297, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
     299, /* MEDICINAL_HERB_HELD_ITEM */
     300, /* EDIBLE_HERB_HELD_ITEM */
     301, /* SMALL_FISH_HELD_ITEM */
     302, /* MEDIUM_FISH_HELD_ITEM */
     303, /* LARGE_FISH_HELD_ITEM */
     298, /* DUMPLING_HELD_ITEM */
     304, /* COTTON_CANDY_HELD_ITEM */
     305, /* FRIED_OCTUPUS_HELD_ITEM */
     334, /* ROASTED_CORN_HELD_ITEM */
     307, /* CANDY_HELD_ITEM */
     306, /* CHOCOLATE_HELD_ITEM */
     310, /* IRON_ORE_HELD_ITEM */
     312, /* BLUE_ROCK_HELD_ITEM */
     311, /* RARE_METAL_HELD_ITEM */
     313, /* MOONLIGHT_STONE_HELD_ITEM */
     333, /* PONTATA_ROOT_HELD_ITEM */
       0, /* 51 */
       0, /* 52 */
     335, /* 53 */
       0, /* 54 */
       0, /* 55 */
       0, /* 56 */
     308, /* FODDER_HELD_ITEM */
       0, /* 58 */
       0, /* 59 */
       0, /* 60 */
       0, /* 61 */
       0, /* 62 */
       0, /* 63 */
       0, /* SEEDS_HELD_ITEM */
       0, /* 65 */
       0, /* 66 */
       0, /* 67 */
       0, /* 68 */
       0, /* 69 */
       0, /* 70 */
       0, /* 71 */
       0, /* MEDICINE_HELD_ITEM */
       0, /* SHEARS_HELD_ITEM */
       0, /* 74 */
       0, /* 75 */
       0, /* 76 */
       0, /* 77 */
       0, /* 78 */
       0, /* 79 */
       0, /* RICE_CAKE_HELD_ITEM */
       0, /* BREAD_HELD_ITEM */
       0, /* EGGS_HELD_ITEM */
       0, /* 83 */
       0, /* 84 */
       0, /* 85 */
       0, /* 86 */
       0, /* POWER_NUT_HELD_ITEM */
     318, /* DOG_HELD_ITEM */
     318, /* 89 */
     318, /* 90 */
     318, /* 91 */
     318, /* 92 */
     318, /* 93 */
     318, /* 94 */
     318, /* 95 */
       0, /* CHICKEN_HELD_ITEM */
       0, /* 97 */
       0, /* 98 */
       0, /* 99 */
       0, /* 100 */
       0, /* 101 */
       0, /* 102 */
       0, /* 103 */
       0, /* CHICK_HELD_ITEM */
       0, /* 105 */
       0, /* 106 */
       0, /* 107 */
       0, /* 108 */
       0, /* 109 */
       0, /* 110 */
       0, /* 111 */
       0, /* EMPTY_BOTTLE_HELD_ITEM */
       0, /* BOTTLE_WITH_WATER_HELD_ITEM */
       0, /* BOTTLE_WITH_WINE_HELD_ITEM */
       0, /* 115 */
       0, /* 116 */
       0, /* 117 */
       0, /* BOTTLE_WITH_MILK_HELD_ITEM */
       0, /* 119 */
       0, /* BOTTLE_WITH_TEA_HELD_ITEM */
       0, /* 121 */
       0, /* 122 */
     336, /* PUPPY_1_HELD_ITEM */
     336, /* 124 */
     336, /* 125 */
     336, /* 126 */
     336, /* 127 */
     336, /* 128 */
     336, /* 129 */
     336, /* 130 */
     330, /* CAT_HELD_ITEM */
     330, /* 132 */
     330, /* 133 */
     330, /* 134 */
     330, /* 135 */
     330, /* 136 */
     330, /* 137 */
     330, /* 138 */
     329, /* FOX_HELD_ITEM */
     329, /* 140 */
     329, /* 141 */
     329, /* 142 */
     329, /* 143 */
     329, /* 144 */
     329, /* 145 */
     329, /* 146 */
     331, /* SQUIRREL_HELD_ITEM */
     331, /* 148 */
     331, /* 149 */
     331, /* 150 */
     331, /* 151 */
     331, /* 152 */
     331, /* 153 */
     331, /* 154 */
     328, /* RABBIT_HELD_ITEM */
     328, /* 156 */
     328, /* 157 */
     328, /* 158 */
     328, /* 159 */
     328, /* 160 */
     328, /* 161 */
     328, /* 162 */
     332, /* MONKEY_HELD_ITEM */
     332, /* 164 */
     332, /* 165 */
     332, /* 166 */
     332, /* 167 */
     332, /* 168 */
     332, /* 169 */
     332, /* 170 */
     337, /* DOG_2_HELD_ITEM */
     337, /* 172 */
     337, /* 173 */
     337, /* 174 */
     337, /* 175 */
     337, /* 176 */
     337, /* 177 */
     337, /* 178 */
     324, /* BUTTERFLY_HELD_ITEM */
     325, /* LADYBUG_HELD_ITEM */
     323, /* CICADA_HELD_ITEM */
     320, /* HORNED_BEETLE_HELD_ITEM */
     321, /* STAG_BEETLE_HELD_ITEM */
     326, /* RED_DRAGONFLY_HELD_ITEM */
     327, /* CRICKET_HELD_ITEM */
     319, /* BABY_HELD_ITEM */
     319, /* 187 */
     319, /* 188 */
     319, /* 189 */
     319, /* 190 */
     319, /* 191 */
     319, /* 192 */
     319, /* 193 */
     319, /* BABY_GROWN_HELD_ITEM */
     319, /* 195 */
     319, /* 196 */
     319, /* 197 */
     319, /* 198 */
     319, /* 199 */
     319, /* 200 */
     319, /* 201 */
       0, /* OPEN_BOOK_HELD_ITEM */
       0, /* SICKLE_HELD_ITEM */
       0, /* SILVER_SICKLE_HELD_ITEM */
       0, /* GOLDEN_SICKLE_HELD_ITEM */
       0, /* HOE_HELD_ITEM */
       0, /* SILVER_HOE_HELD_ITEM */
       0, /* GOLDEN_HOE_HELD_ITEM */
       0, /* AX_HELD_ITEM */
       0, /* SILVER_AX_HELD_ITEM */
       0, /* GOLDEN_AX_HELD_ITEM */
       0, /* HAMMER_HELD_ITEM */
       0, /* SILVER_HAMMER_HELD_ITEM */
       0, /* GOLDEN_HAMMER_HELD_ITEM */
       0, /* WATERING_CAN_HELD_ITEM */
       0, /* SILVER_WATERING_CAN_HELD_ITEM */
       0, /* GOLDEN_WATERING_CAN_HELD_ITEM */
       0, /* BROKEN_MUSIC_BOX_HELD_ITEM */
       0, /* 219 */
       0, /* 220 */
       0, /* 221 */
       0, /* 222 */
       0, /* 223 */
};

u16 itemShippingValues[] = {
       0, /* 0 */
       0, /* WEED_HELD_ITEM */
       0, /* ROCK_HELD_ITEM */
       0, /* LOG_HELD_ITEM */
       0, /* MOONDROP_FLOWER_HELD_ITEM */
       0, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
       0, /* BLUE_MIST_FLOWER_HELD_ITEM */
       0, /* CAKE_HELD_ITEM */
       0, /* PIE_HELD_ITEM */
       0, /* COOKIES_HELD_ITEM */
       0, /* BLUE_FEATHER_HELD_ITEM */
       0, /* DOOR_TO_HEAVEN_HELD_ITEM */
       0, /* 12 */
      60, /* TURNIP_HELD_ITEM */
      80, /* POTATO_HELD_ITEM */
      90, /* CABBAGE_HELD_ITEM */
      90, /* TOMATO_HELD_ITEM */
     120, /* CORN_HELD_ITEM */
      60, /* EGGPLANT_HELD_ITEM */
     150, /* STRAWBERRY_HELD_ITEM */
      50, /* EGG_HELD_ITEM */
     100, /* SMALL_MILK_HELD_ITEM */
     150, /* MEDIUM_MILK_HELD_ITEM */
     300, /* LARGE_MILK_HELD_ITEM */
     500, /* GOLDEN_MILK_HELD_ITEM */
     900, /* WOOL_HELD_ITEM */
    1800, /* HIGH_QUALITY_WOOL_HELD_ITEM */
      50, /* WILD_GRAPES_HELD_ITEM */
      40, /* VERYBERRY_HELD_ITEM */
      70, /* TROPICAL_FRUIT_HELD_ITEM */
      40, /* WALNUT_HELD_ITEM */
      60, /* MUSHROOM_HELD_ITEM */
     100, /* POISONOUS_MUSHROOM_HELD_ITEM */
       0, /* BOOK_HELD_ITEM */
     300, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
      70, /* MEDICINAL_HERB_HELD_ITEM */
      30, /* EDIBLE_HERB_HELD_ITEM */
      30, /* SMALL_FISH_HELD_ITEM */
     100, /* MEDIUM_FISH_HELD_ITEM */
     180, /* LARGE_FISH_HELD_ITEM */
       0, /* DUMPLING_HELD_ITEM */
       0, /* COTTON_CANDY_HELD_ITEM */
       0, /* FRIED_OCTUPUS_HELD_ITEM */
       0, /* ROASTED_CORN_HELD_ITEM */
       0, /* CANDY_HELD_ITEM */
       0, /* CHOCOLATE_HELD_ITEM */
     100, /* IRON_ORE_HELD_ITEM */
     700, /* BLUE_ROCK_HELD_ITEM */
    1000, /* RARE_METAL_HELD_ITEM */
     500, /* MOONLIGHT_STONE_HELD_ITEM */
     800, /* PONTATA_ROOT_HELD_ITEM */
       0, /* 51 */
       0, /* 52 */
       0, /* 53 */
       0, /* 54 */
       0, /* 55 */
       0, /* 56 */
       0, /* FODDER_HELD_ITEM */
       0, /* 58 */
       0, /* 59 */
       0, /* 60 */
       0, /* 61 */
       0, /* 62 */
       0, /* 63 */
       0, /* SEEDS_HELD_ITEM */
       0, /* 65 */
       0, /* 66 */
       0, /* 67 */
       0, /* 68 */
       0, /* 69 */
       0, /* 70 */
       0, /* 71 */
       0, /* MEDICINE_HELD_ITEM */
       0, /* SHEARS_HELD_ITEM */
       0, /* 74 */
       0, /* 75 */
       0, /* 76 */
       0, /* 77 */
       0, /* 78 */
       0, /* 79 */
       0, /* RICE_CAKE_HELD_ITEM */
       0, /* BREAD_HELD_ITEM */
       0, /* EGGS_HELD_ITEM */
       0, /* 83 */
       0, /* 84 */
       0, /* 85 */
       0, /* 86 */
       0, /* POWER_NUT_HELD_ITEM */
       0, /* DOG_HELD_ITEM */
       0, /* 89 */
       0, /* 90 */
       0, /* 91 */
       0, /* 92 */
       0, /* 93 */
       0, /* 94 */
       0, /* 95 */
       0, /* CHICKEN_HELD_ITEM */
       0, /* 97 */
       0, /* 98 */
       0, /* 99 */
       0, /* 100 */
       0, /* 101 */
       0, /* 102 */
       0, /* 103 */
       0, /* CHICK_HELD_ITEM */
       0, /* 105 */
       0, /* 106 */
       0, /* 107 */
       0, /* 108 */
       0, /* 109 */
       0, /* 110 */
       0, /* 111 */
       0, /* EMPTY_BOTTLE_HELD_ITEM */
       0, /* BOTTLE_WITH_WATER_HELD_ITEM */
       0, /* BOTTLE_WITH_WINE_HELD_ITEM */
       0, /* 115 */
       0, /* 116 */
       0, /* 117 */
       0, /* BOTTLE_WITH_MILK_HELD_ITEM */
       0, /* 119 */
       0, /* BOTTLE_WITH_TEA_HELD_ITEM */
       0, /* 121 */
       0, /* 122 */
       0, /* PUPPY_1_HELD_ITEM */
       0, /* 124 */
       0, /* 125 */
       0, /* 126 */
       0, /* 127 */
       0, /* 128 */
       0, /* 129 */
       0, /* 130 */
       0, /* CAT_HELD_ITEM */
       0, /* 132 */
       0, /* 133 */
       0, /* 134 */
       0, /* 135 */
       0, /* 136 */
       0, /* 137 */
       0, /* 138 */
       0, /* FOX_HELD_ITEM */
       0, /* 140 */
       0, /* 141 */
       0, /* 142 */
       0, /* 143 */
       0, /* 144 */
       0, /* 145 */
       0, /* 146 */
       0, /* SQUIRREL_HELD_ITEM */
       0, /* 148 */
       0, /* 149 */
       0, /* 150 */
       0, /* 151 */
       0, /* 152 */
       0, /* 153 */
       0, /* 154 */
       0, /* RABBIT_HELD_ITEM */
       0, /* 156 */
       0, /* 157 */
       0, /* 158 */
       0, /* 159 */
       0, /* 160 */
       0, /* 161 */
       0, /* 162 */
       0, /* MONKEY_HELD_ITEM */
       0, /* 164 */
       0, /* 165 */
       0, /* 166 */
       0, /* 167 */
       0, /* 168 */
       0, /* 169 */
       0, /* 170 */
       0, /* DOG_2_HELD_ITEM */
       0, /* 172 */
       0, /* 173 */
       0, /* 174 */
       0, /* 175 */
       0, /* 176 */
       0, /* 177 */
       0, /* 178 */
       0, /* BUTTERFLY_HELD_ITEM */
       0, /* LADYBUG_HELD_ITEM */
       0, /* CICADA_HELD_ITEM */
       0, /* HORNED_BEETLE_HELD_ITEM */
       0, /* STAG_BEETLE_HELD_ITEM */
       0, /* RED_DRAGONFLY_HELD_ITEM */
       0, /* CRICKET_HELD_ITEM */
       0, /* BABY_HELD_ITEM */
       0, /* 187 */
       0, /* 188 */
       0, /* 189 */
       0, /* 190 */
       0, /* 191 */
       0, /* 192 */
       0, /* 193 */
       0, /* BABY_GROWN_HELD_ITEM */
       0, /* 195 */
       0, /* 196 */
       0, /* 197 */
       0, /* 198 */
       0, /* 199 */
       0, /* 200 */
       0, /* 201 */
       0, /* OPEN_BOOK_HELD_ITEM */
       0, /* SICKLE_HELD_ITEM */
       0, /* SILVER_SICKLE_HELD_ITEM */
       0, /* GOLDEN_SICKLE_HELD_ITEM */
       0, /* HOE_HELD_ITEM */
       0, /* SILVER_HOE_HELD_ITEM */
       0, /* GOLDEN_HOE_HELD_ITEM */
       0, /* AX_HELD_ITEM */
       0, /* SILVER_AX_HELD_ITEM */
       0, /* GOLDEN_AX_HELD_ITEM */
       0, /* HAMMER_HELD_ITEM */
       0, /* SILVER_HAMMER_HELD_ITEM */
       0, /* GOLDEN_HAMMER_HELD_ITEM */
       0, /* WATERING_CAN_HELD_ITEM */
       0, /* SILVER_WATERING_CAN_HELD_ITEM */
       0, /* GOLDEN_WATERING_CAN_HELD_ITEM */
       0, /* BROKEN_MUSIC_BOX_HELD_ITEM */
       0, /* 219 */
       0, /* 220 */
       0, /* 221 */
       0, /* 222 */
       0, /* 223 */
};

u16 itemFlags[] = {
    0, /* 0 */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE, /* WEED_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_PLACEABLE_ON_GROUND | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE, /* ROCK_HELD_ITEM */
    ITEM_LUMBER_STORABLE | ITEM_CABINET_STORABLE | ITEM_PLACEABLE_ON_GROUND | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE, /* LOG_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE, /* MOONDROP_FLOWER_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
    0, /* BLUE_MIST_FLOWER_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* CAKE_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* PIE_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* COOKIES_HELD_ITEM */
    0, /* BLUE_FEATHER_HELD_ITEM */
    0, /* DOOR_TO_HEAVEN_HELD_ITEM */
    0, /* 12 */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* TURNIP_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* POTATO_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* CABBAGE_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* TOMATO_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* CORN_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* EGGPLANT_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* STRAWBERRY_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_FREEZER_STORABLE | ITEM_INCUBATOR_PLACEABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* EGG_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* SMALL_MILK_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* MEDIUM_MILK_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* LARGE_MILK_HELD_ITEM */
    ITEM_HARVEST_GODDESS_OFFERABLE | ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* GOLDEN_MILK_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* WOOL_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* HIGH_QUALITY_WOOL_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* WILD_GRAPES_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* VERYBERRY_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* TROPICAL_FRUIT_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* WALNUT_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* MUSHROOM_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* POISONOUS_MUSHROOM_HELD_ITEM */
    0, /* BOOK_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* MEDICINAL_HERB_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* EDIBLE_HERB_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* SMALL_FISH_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* MEDIUM_FISH_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* LARGE_FISH_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* DUMPLING_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* COTTON_CANDY_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* FRIED_OCTUPUS_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_EATABLE, /* ROASTED_CORN_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* CANDY_HELD_ITEM */
    ITEM_FREEZER_STORABLE | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE | ITEM_EATABLE, /* CHOCOLATE_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* IRON_ORE_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* BLUE_ROCK_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* RARE_METAL_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* MOONLIGHT_STONE_HELD_ITEM */
    ITEM_CABINET_STORABLE | ITEM_THROWABLE | ITEM_SHIPPABLE | ITEM_RUCKSACK_STORABLE, /* PONTATA_ROOT_HELD_ITEM */
    0, /* 51 */
    0, /* 52 */
    0, /* 53 */
    0, /* 54 */
    0, /* 55 */
    0, /* 56 */
    ITEM_CABINET_STORABLE | ITEM_ANIMAL_FEED | ITEM_THROWABLE | ITEM_RUCKSACK_STORABLE, /* FODDER_HELD_ITEM */
    0, /* 58 */
    0, /* 59 */
    0, /* 60 */
    0, /* 61 */
    0, /* 62 */
    0, /* 63 */
    0, /* SEEDS_HELD_ITEM */
    0, /* 65 */
    0, /* 66 */
    0, /* 67 */
    0, /* 68 */
    0, /* 69 */
    0, /* 70 */
    0, /* 71 */
    0, /* MEDICINE_HELD_ITEM */
    0, /* SHEARS_HELD_ITEM */
    0, /* 74 */
    0, /* 75 */
    0, /* 76 */
    0, /* 77 */
    0, /* 78 */
    0, /* 79 */
    0, /* RICE_CAKE_HELD_ITEM */
    0, /* BREAD_HELD_ITEM */
    0, /* EGGS_HELD_ITEM */
    0, /* 83 */
    0, /* 84 */
    0, /* 85 */
    0, /* 86 */
    0, /* POWER_NUT_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* DOG_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 89 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 90 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 91 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 92 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 93 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 94 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 95 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* CHICKEN_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 97 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 98 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 99 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 100 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 101 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 102 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 103 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* CHICK_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 105 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 106 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 107 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 108 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 109 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 110 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 111 */
    0, /* EMPTY_BOTTLE_HELD_ITEM */
    0, /* BOTTLE_WITH_WATER_HELD_ITEM */
    ITEM_HAS_ALCOHOL, /* BOTTLE_WITH_WINE_HELD_ITEM */
    0, /* 115 */
    0, /* 116 */
    0, /* 117 */
    0, /* BOTTLE_WITH_MILK_HELD_ITEM */
    0, /* 119 */
    0, /* BOTTLE_WITH_TEA_HELD_ITEM */
    ITEM_HAS_ALCOHOL, /* 121 */
    ITEM_HAS_ALCOHOL, /* 122 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* PUPPY_1_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 124 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 125 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 126 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 127 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 128 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 129 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 130 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* CAT_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 132 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 133 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 134 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 135 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 136 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 137 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 138 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* FOX_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 140 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 141 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 142 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 143 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 144 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 145 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 146 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* SQUIRREL_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 148 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 149 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 150 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 151 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 152 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 153 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 154 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* RABBIT_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 156 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 157 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 158 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 159 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 160 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 161 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 162 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* MONKEY_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 164 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 165 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 166 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 167 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 168 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 169 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 170 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* DOG_2_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 172 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 173 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 174 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 175 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 176 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 177 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* 178 */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* BUTTERFLY_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* LADYBUG_HELD_ITEM */
    ITEM_BABY | ITEM_THROWABLE, /* CICADA_HELD_ITEM */
    ITEM_BABY | ITEM_THROWABLE, /* HORNED_BEETLE_HELD_ITEM */
    ITEM_BABY | ITEM_THROWABLE, /* STAG_BEETLE_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* RED_DRAGONFLY_HELD_ITEM */
    ITEM_SMALL_ANIMAL | ITEM_THROWABLE, /* CRICKET_HELD_ITEM */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* BABY_HELD_ITEM */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 187 */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 188 */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 189 */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 190 */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 191 */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 192 */
    ITEM_CRIB_PLACEABLE | ITEM_THROWABLE, /* 193 */
    ITEM_TOOL | ITEM_THROWABLE, /* BABY_GROWN_HELD_ITEM */
    ITEM_TOOL | ITEM_THROWABLE, /* 195 */
    ITEM_TOOL | ITEM_THROWABLE, /* 196 */
    ITEM_TOOL | ITEM_THROWABLE, /* 197 */
    ITEM_TOOL | ITEM_THROWABLE, /* 198 */
    ITEM_TOOL | ITEM_THROWABLE, /* 199 */
    ITEM_TOOL | ITEM_THROWABLE, /* 200 */
    ITEM_TOOL | ITEM_THROWABLE, /* 201 */
    0, /* OPEN_BOOK_HELD_ITEM */
    0, /* SICKLE_HELD_ITEM */
    0, /* SILVER_SICKLE_HELD_ITEM */
    0, /* GOLDEN_SICKLE_HELD_ITEM */
    0, /* HOE_HELD_ITEM */
    0, /* SILVER_HOE_HELD_ITEM */
    0, /* GOLDEN_HOE_HELD_ITEM */
    0, /* AX_HELD_ITEM */
    0, /* SILVER_AX_HELD_ITEM */
    0, /* GOLDEN_AX_HELD_ITEM */
    0, /* HAMMER_HELD_ITEM */
    0, /* SILVER_HAMMER_HELD_ITEM */
    0, /* GOLDEN_HAMMER_HELD_ITEM */
    0, /* WATERING_CAN_HELD_ITEM */
    0, /* SILVER_WATERING_CAN_HELD_ITEM */
    0, /* GOLDEN_WATERING_CAN_HELD_ITEM */
    0, /* BROKEN_MUSIC_BOX_HELD_ITEM */
    0, /* 219 */
    0, /* 220 */
    0, /* 221 */
    0, /* 222 */
    0, /* 223 */
};

u8 itemStaminaRestorationValue[] = {
     0, /* 0 */
     0, /* WEED_HELD_ITEM */
     0, /* ROCK_HELD_ITEM */
     0, /* LOG_HELD_ITEM */
     0, /* MOONDROP_FLOWER_HELD_ITEM */
     0, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
     0, /* BLUE_MIST_FLOWER_HELD_ITEM */
    20, /* CAKE_HELD_ITEM */
    20, /* PIE_HELD_ITEM */
    20, /* COOKIES_HELD_ITEM */
     0, /* BLUE_FEATHER_HELD_ITEM */
     0, /* DOOR_TO_HEAVEN_HELD_ITEM */
     0, /* 12 */
     8, /* TURNIP_HELD_ITEM */
    10, /* POTATO_HELD_ITEM */
     8, /* CABBAGE_HELD_ITEM */
     8, /* TOMATO_HELD_ITEM */
    10, /* CORN_HELD_ITEM */
     8, /* EGGPLANT_HELD_ITEM */
    12, /* STRAWBERRY_HELD_ITEM */
     8, /* EGG_HELD_ITEM */
     8, /* SMALL_MILK_HELD_ITEM */
    12, /* MEDIUM_MILK_HELD_ITEM */
    16, /* LARGE_MILK_HELD_ITEM */
    20, /* GOLDEN_MILK_HELD_ITEM */
     0, /* WOOL_HELD_ITEM */
     0, /* HIGH_QUALITY_WOOL_HELD_ITEM */
     6, /* WILD_GRAPES_HELD_ITEM */
     6, /* VERYBERRY_HELD_ITEM */
     6, /* TROPICAL_FRUIT_HELD_ITEM */
     6, /* WALNUT_HELD_ITEM */
     6, /* MUSHROOM_HELD_ITEM */
     0, /* POISONOUS_MUSHROOM_HELD_ITEM */
     0, /* BOOK_HELD_ITEM */
    20, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
     0, /* MEDICINAL_HERB_HELD_ITEM */
     6, /* EDIBLE_HERB_HELD_ITEM */
     6, /* SMALL_FISH_HELD_ITEM */
     8, /* MEDIUM_FISH_HELD_ITEM */
    16, /* LARGE_FISH_HELD_ITEM */
     2, /* DUMPLING_HELD_ITEM */
     8, /* COTTON_CANDY_HELD_ITEM */
     8, /* FRIED_OCTUPUS_HELD_ITEM */
     8, /* ROASTED_CORN_HELD_ITEM */
     2, /* CANDY_HELD_ITEM */
     2, /* CHOCOLATE_HELD_ITEM */
     0, /* IRON_ORE_HELD_ITEM */
     0, /* BLUE_ROCK_HELD_ITEM */
     0, /* RARE_METAL_HELD_ITEM */
     0, /* MOONLIGHT_STONE_HELD_ITEM */
     0, /* PONTATA_ROOT_HELD_ITEM */
     0, /* 51 */
     0, /* 52 */
     0, /* 53 */
     0, /* 54 */
     0, /* 55 */
     0, /* 56 */
     0, /* FODDER_HELD_ITEM */
     0, /* 58 */
     0, /* 59 */
     0, /* 60 */
     0, /* 61 */
     0, /* 62 */
     0, /* 63 */
     0, /* SEEDS_HELD_ITEM */
     0, /* 65 */
     0, /* 66 */
     0, /* 67 */
     0, /* 68 */
     0, /* 69 */
     0, /* 70 */
     0, /* 71 */
     0, /* MEDICINE_HELD_ITEM */
     0, /* SHEARS_HELD_ITEM */
     0, /* 74 */
     0, /* 75 */
     0, /* 76 */
     0, /* 77 */
     0, /* 78 */
     0, /* 79 */
    20, /* RICE_CAKE_HELD_ITEM */
    20, /* BREAD_HELD_ITEM */
    20, /* EGGS_HELD_ITEM */
    20, /* 83 */
    20, /* 84 */
    20, /* 85 */
    20, /* 86 */
     0, /* POWER_NUT_HELD_ITEM */
     0, /* DOG_HELD_ITEM */
     0, /* 89 */
     0, /* 90 */
     0, /* 91 */
     0, /* 92 */
     0, /* 93 */
     0, /* 94 */
     0, /* 95 */
     0, /* CHICKEN_HELD_ITEM */
     0, /* 97 */
     0, /* 98 */
     0, /* 99 */
     0, /* 100 */
     0, /* 101 */
     0, /* 102 */
     0, /* 103 */
     0, /* CHICK_HELD_ITEM */
     0, /* 105 */
     0, /* 106 */
     0, /* 107 */
     0, /* 108 */
     0, /* 109 */
     0, /* 110 */
     0, /* 111 */
     0, /* EMPTY_BOTTLE_HELD_ITEM */
     3, /* BOTTLE_WITH_WATER_HELD_ITEM */
     8, /* BOTTLE_WITH_WINE_HELD_ITEM */
     0, /* 115 */
     0, /* 116 */
     0, /* 117 */
    12, /* BOTTLE_WITH_MILK_HELD_ITEM */
   200, /* 119 */
     8, /* BOTTLE_WITH_TEA_HELD_ITEM */
     8, /* 121 */
     8, /* 122 */
     0, /* PUPPY_1_HELD_ITEM */
     0, /* 124 */
     0, /* 125 */
     0, /* 126 */
     0, /* 127 */
     0, /* 128 */
     0, /* 129 */
     0, /* 130 */
     0, /* CAT_HELD_ITEM */
     0, /* 132 */
     0, /* 133 */
     0, /* 134 */
     0, /* 135 */
     0, /* 136 */
     0, /* 137 */
     0, /* 138 */
     0, /* FOX_HELD_ITEM */
     0, /* 140 */
     0, /* 141 */
     0, /* 142 */
     0, /* 143 */
     0, /* 144 */
     0, /* 145 */
     0, /* 146 */
     0, /* SQUIRREL_HELD_ITEM */
     0, /* 148 */
     0, /* 149 */
     0, /* 150 */
     0, /* 151 */
     0, /* 152 */
     0, /* 153 */
     0, /* 154 */
     0, /* RABBIT_HELD_ITEM */
     0, /* 156 */
     0, /* 157 */
     0, /* 158 */
     0, /* 159 */
     0, /* 160 */
     0, /* 161 */
     0, /* 162 */
     0, /* MONKEY_HELD_ITEM */
     0, /* 164 */
     0, /* 165 */
     0, /* 166 */
     0, /* 167 */
     0, /* 168 */
     0, /* 169 */
     0, /* 170 */
     0, /* DOG_2_HELD_ITEM */
     0, /* 172 */
     0, /* 173 */
     0, /* 174 */
     0, /* 175 */
     0, /* 176 */
     0, /* 177 */
     0, /* 178 */
     0, /* BUTTERFLY_HELD_ITEM */
     0, /* LADYBUG_HELD_ITEM */
     0, /* CICADA_HELD_ITEM */
     0, /* HORNED_BEETLE_HELD_ITEM */
     0, /* STAG_BEETLE_HELD_ITEM */
     0, /* RED_DRAGONFLY_HELD_ITEM */
     0, /* CRICKET_HELD_ITEM */
     0, /* BABY_HELD_ITEM */
     0, /* 187 */
     0, /* 188 */
     0, /* 189 */
     0, /* 190 */
     0, /* 191 */
     0, /* 192 */
     0, /* 193 */
     0, /* BABY_GROWN_HELD_ITEM */
     0, /* 195 */
     0, /* 196 */
     0, /* 197 */
     0, /* 198 */
     0, /* 199 */
     0, /* 200 */
     0, /* 201 */
     0, /* OPEN_BOOK_HELD_ITEM */
     0, /* SICKLE_HELD_ITEM */
     0, /* SILVER_SICKLE_HELD_ITEM */
     0, /* GOLDEN_SICKLE_HELD_ITEM */
     0, /* HOE_HELD_ITEM */
     0, /* SILVER_HOE_HELD_ITEM */
     0, /* GOLDEN_HOE_HELD_ITEM */
     0, /* AX_HELD_ITEM */
     0, /* SILVER_AX_HELD_ITEM */
     0, /* GOLDEN_AX_HELD_ITEM */
     0, /* HAMMER_HELD_ITEM */
     0, /* SILVER_HAMMER_HELD_ITEM */
     0, /* GOLDEN_HAMMER_HELD_ITEM */
     0, /* WATERING_CAN_HELD_ITEM */
     0, /* SILVER_WATERING_CAN_HELD_ITEM */
     0, /* GOLDEN_WATERING_CAN_HELD_ITEM */
     0, /* BROKEN_MUSIC_BOX_HELD_ITEM */
     0, /* 219 */
     0, /* 220 */
     0, /* 221 */
     0, /* 222 */
     0, /* 223 */
};

u8 itemFatigueReductionValue[] = {
    0, /* 0 */
    0, /* WEED_HELD_ITEM */
    0, /* ROCK_HELD_ITEM */
    0, /* LOG_HELD_ITEM */
    0, /* MOONDROP_FLOWER_HELD_ITEM */
    0, /* PINK_CAT_MINT_FLOWER_HELD_ITEM */
    0, /* BLUE_MIST_FLOWER_HELD_ITEM */
    0, /* CAKE_HELD_ITEM */
    0, /* PIE_HELD_ITEM */
    0, /* COOKIES_HELD_ITEM */
    0, /* BLUE_FEATHER_HELD_ITEM */
    0, /* DOOR_TO_HEAVEN_HELD_ITEM */
    0, /* 12 */
    0, /* TURNIP_HELD_ITEM */
    0, /* POTATO_HELD_ITEM */
    0, /* CABBAGE_HELD_ITEM */
    0, /* TOMATO_HELD_ITEM */
    0, /* CORN_HELD_ITEM */
    0, /* EGGPLANT_HELD_ITEM */
    0, /* STRAWBERRY_HELD_ITEM */
    0, /* EGG_HELD_ITEM */
    0, /* SMALL_MILK_HELD_ITEM */
    0, /* MEDIUM_MILK_HELD_ITEM */
    0, /* LARGE_MILK_HELD_ITEM */
    0, /* GOLDEN_MILK_HELD_ITEM */
    0, /* WOOL_HELD_ITEM */
    0, /* HIGH_QUALITY_WOOL_HELD_ITEM */
    0, /* WILD_GRAPES_HELD_ITEM */
    0, /* VERYBERRY_HELD_ITEM */
    0, /* TROPICAL_FRUIT_HELD_ITEM */
    0, /* WALNUT_HELD_ITEM */
    0, /* MUSHROOM_HELD_ITEM */
    0, /* POISONOUS_MUSHROOM_HELD_ITEM */
    0, /* BOOK_HELD_ITEM */
    0, /* BERRY_OF_THE_FULLMOON_HELD_ITEM */
    2, /* MEDICINAL_HERB_HELD_ITEM */
    0, /* EDIBLE_HERB_HELD_ITEM */
    0, /* SMALL_FISH_HELD_ITEM */
    0, /* MEDIUM_FISH_HELD_ITEM */
    0, /* LARGE_FISH_HELD_ITEM */
    0, /* DUMPLING_HELD_ITEM */
    0, /* COTTON_CANDY_HELD_ITEM */
    0, /* FRIED_OCTUPUS_HELD_ITEM */
    0, /* ROASTED_CORN_HELD_ITEM */
    0, /* CANDY_HELD_ITEM */
    0, /* CHOCOLATE_HELD_ITEM */
    0, /* IRON_ORE_HELD_ITEM */
    0, /* BLUE_ROCK_HELD_ITEM */
    0, /* RARE_METAL_HELD_ITEM */
    0, /* MOONLIGHT_STONE_HELD_ITEM */
    0, /* PONTATA_ROOT_HELD_ITEM */
    0, /* 51 */
    0, /* 52 */
    0, /* 53 */
    0, /* 54 */
    0, /* 55 */
    0, /* 56 */
    0, /* FODDER_HELD_ITEM */
    0, /* 58 */
    0, /* 59 */
    0, /* 60 */
    0, /* 61 */
    0, /* 62 */
    0, /* 63 */
    0, /* SEEDS_HELD_ITEM */
    0, /* 65 */
    0, /* 66 */
    0, /* 67 */
    0, /* 68 */
    0, /* 69 */
    0, /* 70 */
    0, /* 71 */
    0, /* MEDICINE_HELD_ITEM */
    0, /* SHEARS_HELD_ITEM */
    0, /* 74 */
    0, /* 75 */
    0, /* 76 */
    0, /* 77 */
    0, /* 78 */
    0, /* 79 */
    0, /* RICE_CAKE_HELD_ITEM */
    0, /* BREAD_HELD_ITEM */
    0, /* EGGS_HELD_ITEM */
    0, /* 83 */
    0, /* 84 */
    0, /* 85 */
    0, /* 86 */
    0, /* POWER_NUT_HELD_ITEM */
    0, /* DOG_HELD_ITEM */
    0, /* 89 */
    0, /* 90 */
    0, /* 91 */
    0, /* 92 */
    0, /* 93 */
    0, /* 94 */
    0, /* 95 */
    0, /* CHICKEN_HELD_ITEM */
    0, /* 97 */
    0, /* 98 */
    0, /* 99 */
    0, /* 100 */
    0, /* 101 */
    0, /* 102 */
    0, /* 103 */
    0, /* CHICK_HELD_ITEM */
    0, /* 105 */
    0, /* 106 */
    0, /* 107 */
    0, /* 108 */
    0, /* 109 */
    0, /* 110 */
    0, /* 111 */
    0, /* EMPTY_BOTTLE_HELD_ITEM */
    0, /* BOTTLE_WITH_WATER_HELD_ITEM */
    0, /* BOTTLE_WITH_WINE_HELD_ITEM */
   20, /* 115 */
   10, /* 116 */
   30, /* 117 */
    0, /* BOTTLE_WITH_MILK_HELD_ITEM */
  100, /* 119 */
    0, /* BOTTLE_WITH_TEA_HELD_ITEM */
    0, /* 121 */
    0, /* 122 */
    0, /* PUPPY_1_HELD_ITEM */
    0, /* 124 */
    0, /* 125 */
    0, /* 126 */
    0, /* 127 */
    0, /* 128 */
    0, /* 129 */
    0, /* 130 */
    0, /* CAT_HELD_ITEM */
    0, /* 132 */
    0, /* 133 */
    0, /* 134 */
    0, /* 135 */
    0, /* 136 */
    0, /* 137 */
    0, /* 138 */
    0, /* FOX_HELD_ITEM */
    0, /* 140 */
    0, /* 141 */
    0, /* 142 */
    0, /* 143 */
    0, /* 144 */
    0, /* 145 */
    0, /* 146 */
    0, /* SQUIRREL_HELD_ITEM */
    0, /* 148 */
    0, /* 149 */
    0, /* 150 */
    0, /* 151 */
    0, /* 152 */
    0, /* 153 */
    0, /* 154 */
    0, /* RABBIT_HELD_ITEM */
    0, /* 156 */
    0, /* 157 */
    0, /* 158 */
    0, /* 159 */
    0, /* 160 */
    0, /* 161 */
    0, /* 162 */
    0, /* MONKEY_HELD_ITEM */
    0, /* 164 */
    0, /* 165 */
    0, /* 166 */
    0, /* 167 */
    0, /* 168 */
    0, /* 169 */
    0, /* 170 */
    0, /* DOG_2_HELD_ITEM */
    0, /* 172 */
    0, /* 173 */
    0, /* 174 */
    0, /* 175 */
    0, /* 176 */
    0, /* 177 */
    0, /* 178 */
    0, /* BUTTERFLY_HELD_ITEM */
    0, /* LADYBUG_HELD_ITEM */
    0, /* CICADA_HELD_ITEM */
    0, /* HORNED_BEETLE_HELD_ITEM */
    0, /* STAG_BEETLE_HELD_ITEM */
    0, /* RED_DRAGONFLY_HELD_ITEM */
    0, /* CRICKET_HELD_ITEM */
    0, /* BABY_HELD_ITEM */
    0, /* 187 */
    0, /* 188 */
    0, /* 189 */
    0, /* 190 */
    0, /* 191 */
    0, /* 192 */
    0, /* 193 */
    0, /* BABY_GROWN_HELD_ITEM */
    0, /* 195 */
    0, /* 196 */
    0, /* 197 */
    0, /* 198 */
    0, /* 199 */
    0, /* 200 */
    0, /* 201 */
    0, /* OPEN_BOOK_HELD_ITEM */
    0, /* SICKLE_HELD_ITEM */
    0, /* SILVER_SICKLE_HELD_ITEM */
    0, /* GOLDEN_SICKLE_HELD_ITEM */
    0, /* HOE_HELD_ITEM */
    0, /* SILVER_HOE_HELD_ITEM */
    0, /* GOLDEN_HOE_HELD_ITEM */
    0, /* AX_HELD_ITEM */
    0, /* SILVER_AX_HELD_ITEM */
    0, /* GOLDEN_AX_HELD_ITEM */
    0, /* HAMMER_HELD_ITEM */
    0, /* SILVER_HAMMER_HELD_ITEM */
    0, /* GOLDEN_HAMMER_HELD_ITEM */
    0, /* WATERING_CAN_HELD_ITEM */
    0, /* SILVER_WATERING_CAN_HELD_ITEM */
    0, /* GOLDEN_WATERING_CAN_HELD_ITEM */
    0, /* BROKEN_MUSIC_BOX_HELD_ITEM */
    0, /* 219 */
    0, /* 220 */
    0, /* 221 */
    0, /* 222 */
    0, /* 223 */
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
bool processStumpHit(u8 arg0, s16 x, s16 z);
bool processBoulderHit(u8 arg0, s16 x, s16 z);          
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
u8 allocateGroundItemSlot(u8 index, u8 stateIndex, u32 heldItemIndex, u16 hasDirectionFrame, bool useAttachment);
void setItemPosition(u8 index, f32 x, f32 y, f32 z);
void loadHeldItemEntity(u8, u16);   
bool handlePutDownHeldItem(u8 itemIndex);
void processItemShipping(u8 index);
void handleItemDroppedInWater(u8 arg0, u8 index);
u8 getHeldItemDialogueItemIndex(u8);
u16 getItemFlags(u16 index);

void resetToolUseState(void) {
    toolUse.unk_0 = 0;
    toolUse.stepIndex = 0;
    toolUse.toolUseState = 2;
}

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

void spawnToolEffectEntity(u8 itemOffset, u8 animationIndex, f32 x, f32 y, f32 z) {

    loadEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, ENTITY_ASSET_TOOL_EFFECTS, 1);
    setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemOffset, 0xFFFF, 0xFF);
    setEntityCollidable(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityYMovement(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityCoordinates(ENTITY_ITEM_BASE_INDEX + itemOffset, x, y, z);
    setEntityColor(ENTITY_ITEM_BASE_INDEX + itemOffset, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemOffset, animationIndex);
    
}

void spawnFishingRodEntity(u8 itemOffset, u8 animationIndex, f32 x, f32 y, f32 z) {

    loadEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, ENTITY_ASSET_STATIC_ITEM, 1);
    setCameraTrackingEntity(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityTrackingTarget(ENTITY_ITEM_BASE_INDEX + itemOffset, 0xFFFF, 0xFF);
    setEntityCollidable(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityYMovement(ENTITY_ITEM_BASE_INDEX + itemOffset, FALSE);
    setEntityCoordinates(ENTITY_ITEM_BASE_INDEX + itemOffset, x, y, z);
    setEntityColor(ENTITY_ITEM_BASE_INDEX + itemOffset, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.r, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.g, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.b, mainMap[MAIN_MAP_INDEX].mapGlobals.currentRGBA.a);
    setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemOffset, animationIndex);
    
}

void setItemEntityAnimation(u8 itemOffset, u8 animationIndex) {
    setEntityAnimation(ENTITY_ITEM_BASE_INDEX + itemOffset, animationIndex);
}

void deactivateItemEntity(u8 offset) {
    deactivateEntity(ENTITY_ITEM_BASE_INDEX + offset);
}

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

inline u8 handleMusicBoxDigging(s16 x, s16 z) {
    
    if (gBaseMapIndex == FARM) {
         
        setDailyEventBit(POWER_NUT_ELIGIBLE_TOOL_USE); 
        
        if (x == 12 && z == 2) {
            toolUse.musicBoxTileDigCounter++;
        } else {
            toolUse.musicBoxTileDigCounter = 0;    
        }
        
    } else {
        toolUse.musicBoxTileDigCounter = 0;
    }
    
    return toolUse.musicBoxTileDigCounter;
    
}

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

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);

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

void useHoe(void) {

    Vec3f vec;
    f32 tempF;
    u8 temp;
    s32 tempX, tempZ;
    
    u8 direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);

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

                if (!checkDailyEventBit(74)) {

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
                
                if (!checkDailyEventBit(74)) {

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
        setDailyEventBit(POWER_NUT_ELIGIBLE_TOOL_USE);
    }
    
    handleHitFarmAnimalWithTool();
    handleHitChickenWithTool();
    handleHitDogWithTool();
    handleHitHorseWithTool();
    
}

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
    
    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);

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

void useMilker(void) {
    handleMilkCow();
    toolUse.toolUseState = 0;
}

void useCowBell(void) {
    handleCallFarmAnimalsWithCowBell();
    toolUse.toolUseState = 0;
}

void useBrush(void) {
    handleBrushFarmAnimal();
    handleBrushHorse();
    toolUse.toolUseState = 0;
}

void useClippers(void) {
    handleShearSheep();
    toolUse.toolUseState = 0;
}

void useTurnipSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void usePotatoSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useCabbageSeeds(void) {
 
    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useTomatoSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useCornSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useEggplantSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useStrawberrySeeds(void) {
    
    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useMoonDropSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void usePinkCatMintSeeds(void) {

    u8 direction;
    u8 temp;
    u8 groundObjectIndex;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
        temp = toolSweepOffsetsLeft[toolUse.stepIndex];
    } else {
        temp = toolSweepOffsetsRight[toolUse.stepIndex];
    }
    
    vec = getOffsetTileCoordinates(0.0f, temp);
 
    if ((getGroundObjectToolInteractionFlags(getGroundObjectIndexFromPlayerPosition(0.0f, temp)) & GROUND_OBJECT_PLANTABLE) && vec.y != 65535.0f) {

        if ((gSeason >= SPRING && gSeason < AUTUMN) || gBaseMapIndex == GREENHOUSE) {
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

void useBlueMistSeeds(void) {

    u8 direction;
    u8 temp;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void useFishingPole(void) {
    toolUse.toolUseState = 0;
}

void useMiraclePotion(void) {

    if (handleUseMiraclePotion()) {
        setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
    } else {
        clearDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
    }

    toolUse.toolUseState = 0;

}

void useCowMedicine(void) {

    if (handleAnimalMedicineUse()) {
        setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
    } else {
        clearDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
    }

    toolUse.toolUseState = 0;

}

void useGrassSeeds(void) {

    u8 direction;
    u8 temp;

    Vec3f vec;

    direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
    
    if (direction < DIRECTION_NE) {
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

void handleBlueFeatherUse(void) {
     
    clearDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
    
    if (npcTalkingTo == 0xFF) goto func_end;

    getBlueFeatherResponse(npcTalkingTo, 0);
    
    npcs[npcTalkingTo].animationMode = npcs[npcTalkingTo].defaultAnimationMode;
    
    setSpecialDialogueBit(USED_BLUE_FEATHER_DIALOGUE);
    
    switch (npcTalkingTo) {
        case MARIA:
            if (npcAffection[MARIA] >= 220) {
                gWife = MARIA;   
                setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(MARIA);
            }
            break;
        case POPURI:
            if (npcAffection[POPURI] >= 220) {
                gWife = POPURI;
                setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(POPURI);
            }
            break;
        case ELLI:
            if (npcAffection[ELLI] >= 220) {
                gWife = ELLI;
                setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ELLI);
            }
            break;
        case ANN:
            if (npcAffection[ANN] >= 220) {
                gWife = ANN;
                setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(ANN);
            }
            break;
        case KAREN:
            if (npcAffection[KAREN] >= 220) {
                gWife = KAREN;
                setDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL);
                setLifeEventBit(ENGAGED);
                setDefaultBabyName(KAREN);
            }
            break;
        default:
            break;
        }

    if (checkDailyEventBit(SUCCESSFULLY_USED_CONSUMABLE_TOOL)) {

        // use up blue feather if successful proposal
        gPlayer.currentTool = 0;

    }
    
func_end:
        toolUse.toolUseState = 0;

}

void useEmptyBottle(void) {

    if (checkWaterableSoilInteraction(gBaseMapIndex)) {
        gPlayer.bottleContents = BOTTLE_CONTENTS_WATER;
    } else if (checkWineBarrelInteraction(gBaseMapIndex) ) {
        gPlayer.bottleContents = BOTTLE_CONTENTS_WINE;
    } else {
        handleGetMilkWithBottle();
    }

    toolUse.toolUseState = 0;

}

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

void loadActiveItemEntities(void) {

    u8 i;
    
    for (i = 0; i < 10; i++) {
        if (itemInfo[i].flags & ITEM_CONTEXT_ACTIVE) {
            loadHeldItemEntity(i, itemInfo[i].heldItemIndex);
        }    
    }

}

inline u8 initializeHeldItem(u8 index, u8 stateIndex, u32 heldItemIndex, u16 hasDirectionFrame, bool useAttachment) {

    itemInfo[index].stateIndex = stateIndex;
    itemInfo[index].heldItemIndex = heldItemIndex;
    itemInfo[index].itemAnimationFrameCounter = 0;
    itemInfo[index].flags = hasDirectionFrame | (useAttachment | 1);
    
    gItemBeingHeld = getHeldItemDialogueItemIndex(heldItemIndex);
    
    return index;

}

u8 allocateThrownItemSlot(u8 index, u8 stateIndex, u32 heldItemIndex, u16 hasDirectionFrame, bool useAttachment) {
    
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
        index = initializeHeldItem(index, stateIndex, heldItemIndex, hasDirectionFrame, useAttachment);
    } else {
        index = 0xff;
    }

    return index;

}

u8 allocateGroundItemSlot(u8 index, u8 stateIndex, u32 heldItemIndex, u16 hasDirectionFrame, bool useAttachment) {
    
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
        
        itemInfo[index].stateIndex = stateIndex;
        itemInfo[index].heldItemIndex = heldItemIndex;
        itemInfo[index].flags = hasDirectionFrame | (useAttachment | 1);
        itemInfo[index].itemAnimationFrameCounter = 0;

    } else {
        index = 0xff;
    }
    
    return index;

}

void clearHeldItemSlot(u8 index) {

    if (itemInfo[index].flags & ITEM_CONTEXT_ACTIVE) {
        deactivateEntity(ENTITY_ITEM_BASE_INDEX + index);
        itemInfo[index].stateIndex = ITEM_STATE_INACTIVE;
        itemInfo[index].flags = 0;
    }

}

u16 func_800D55B8(u8 index) {
    return itemInfo[index].heldItemIndex;
}

void setItemState(u8 index, u8 stateIndex) {

    if (itemInfo[index].flags & ITEM_CONTEXT_ACTIVE) {
        itemInfo[index].stateIndex = stateIndex;
    }
    
}

void setItemPosition(u8 index, f32 x, f32 y, f32 z) {

    itemInfo[index].position.x = x;
    itemInfo[index].position.y = y;
    itemInfo[index].position.z = z;
    itemInfo[index].flags |= ITEM_CONTEXT_USE_POSITION;

}

void setItemAttachmentOffset(u8 index, f32 x, f32 y, f32 z) {

    itemInfo[index].attachmentOffset.x = x;
    itemInfo[index].attachmentOffset.y = y;
    itemInfo[index].attachmentOffset.z = z;
    itemInfo[index].flags |= ITEM_CONTEXT_USE_ATTACHMENT;
     
}

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
        setEntityDirectionalAnimation(ENTITY_ITEM_BASE_INDEX + itemIndex, itemEntityMetadata[heldItemIndex][gSeason]);
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

u8 getItemStaminaIncreaseValue(u16 index) {
    return itemStaminaRestorationValue[index];
}

u8 getItemFatigueReductionValue(u16 index) {
    return itemFatigueReductionValue[index];
}

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

    heldItemIndex = itemInfo[itemIndex].heldItemIndex;
    
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

            vec2 = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            gChickens[gPlayer.heldAnimalIndex].coordinates = vec2;

            gChickens[gPlayer.heldAnimalIndex].direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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

            vec3 = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            dogInfo.coordinates = vec3;

            dogInfo.direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));
            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
                case BUTTERFLY_HELD_ITEM:                                  
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

            vec = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            vec.y = entities[ENTITY_PLAYER].coordinates.y;
            
            initializeMiscAnimalEntity(spawnMiscAnimal(temp, direction, vec.x, vec.y, vec.z), 1);

            break;

       case EGG_HELD_ITEM:                              

            gPlayer.heldAnimalIndex = initializeNewChicken(0, 0xFF);

            gChickens[gPlayer.heldAnimalIndex].coordinates.x = 92.0f;
            gChickens[gPlayer.heldAnimalIndex].coordinates.y = 0;
            gChickens[gPlayer.heldAnimalIndex].coordinates.z = -148.0f;
            
            gChickens[gPlayer.heldAnimalIndex].direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
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
            npcs[BABY].direction = DIRECTION_S;
            npcs[BABY].defaultAnimationMode = NPC_ANIMATION_WANDER;
            npcs[BABY].idleAnimation = 0;
            npcs[BABY].animationMode = NPC_ANIMATION_WANDER;

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

            vec4 = projectEntityPosition(ENTITY_PLAYER, 32, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX));

            npcs[BABY].startingCoordinates = vec4;

            npcs[BABY].levelIndex = gBaseMapIndex;
            npcs[BABY].flags |= 1;

            npcs[BABY].direction = convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX);
            npcs[BABY].defaultAnimationMode = NPC_ANIMATION_CUSTOM;
            npcs[BABY].idleAnimation = 57;
            npcs[BABY].animationMode = NPC_ANIMATION_CUSTOM;
            
            setDailyEventBit(PICKED_UP_BABY);
            setupNPCEntity(BABY, 0);
            
            break;
        
    }

return result;
    
}

u8 getHeldItemDialogueItemIndex(u8 index) {
    return heldItemDialogueItemIndices[index];
}

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

    showTextBox(1, LEVEL_INTERACTIONS_TEXT_INDEX, temp, 0, 2);

}

void processItemShipping(u8 index) {

    u32 result;
    
    dailyShippingBinValue += adjustValue(dailyShippingBinValue, itemShippingValues[index], MAX_TOTAL_SHIPPING);
     
    switch (index) {
        case TURNIP_HELD_ITEM:
            totalTurnipsShipped += adjustValue(totalTurnipsShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case POTATO_HELD_ITEM:
            totalPotatoesShipped += adjustValue(totalPotatoesShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case EGGPLANT_HELD_ITEM:
            totalEggplantsShipped += adjustValue(totalEggplantsShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case CABBAGE_HELD_ITEM:
            totalCabbageShipped += adjustValue(totalCabbageShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case STRAWBERRY_HELD_ITEM:
            totalStrawberriesShipped += adjustValue(totalStrawberriesShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case TOMATO_HELD_ITEM:
            totalTomatoesShipped += adjustValue(totalTomatoesShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case CORN_HELD_ITEM:
            totalCornShipped += adjustValue(totalCornShipped, 1, MAX_ITEM_SHIPPING_VALUE);
            gTotalCropsShipped += adjustValue(gTotalCropsShipped, 1, MAX_TOTAL_CROPS_SHIPPED);
            break;
        case SMALL_MILK_HELD_ITEM:
        case MEDIUM_MILK_HELD_ITEM:
        case LARGE_MILK_HELD_ITEM:
        case GOLDEN_MILK_HELD_ITEM:
            gTotalMilkShipped += adjustValue(gTotalMilkShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            break;
        case EGG_HELD_ITEM:
            gTotalEggsShipped += adjustValue(gTotalEggsShipped, 1, MAX_ANIMAL_ITEM_SHIPPED);
            break;
            
    }

    handleHorseShippingItem();

}

inline void handleFeedDog(void) {
    setMapObjectAnimation(MAIN_MAP_INDEX, 0, 19);
    setDailyEventBit(FED_DOG);
}

static inline void setVec3f(u8 index, f32 x, f32 y, f32 z) {

    itemInfo[index].position.x = x;
    itemInfo[index].position.y = y;
    itemInfo[index].position.z = z;

}

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
        
        if (26 < temp && temp2 < 28 || temp == 29) {
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
        
        if (!(gCutsceneFlags & CUTSCENE_ACTIVE)) {

            if (!checkLifeEventBit(MET_KAPPA) && !checkDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY)
                && (itemInfo[itemIndex].heldItemIndex == SMALL_FISH_HELD_ITEM || itemInfo[itemIndex].heldItemIndex == MEDIUM_FISH_HELD_ITEM)
                && 8 < gHour && gHour < 17) {
                setDailyEventBit(KAPPA_FISH_OFFERING_DAILY);
            }

            if (!checkLifeEventBit(KAPPA_POWER_NUT_GIFT) && !checkDailyEventBit(MOUNTAIN_1_CUTSCENE_DAILY) && itemInfo[itemIndex].heldItemIndex == LARGE_FISH_HELD_ITEM && 8 < gHour && gHour < 17) {
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
        
        if (!(gCutsceneFlags & CUTSCENE_ACTIVE)) {
            if (!checkDailyEventBit(HARVEST_GODDESS_INTERACTION) && (itemFlags[itemInfo[itemIndex].heldItemIndex] & ITEM_HARVEST_GODDESS_OFFERABLE) && 8 < gHour && gHour < 17) {
                setDailyEventBit(HARVEST_GODDESS_OFFERING);    
            }
        }

    }

}

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

                        vec = getMovementVectorFromDirection(4.0f, convertScreenDirectionToWorldDirection(entities[ENTITY_PLAYER].direction, MAIN_MAP_INDEX), 0.0f);

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

                                itemInfo[i].stateIndex = ITEM_STATE_INACTIVE;
                                itemInfo[i].flags = 0;

                            }
                            
                            if (checkDailyEventBit(HARVEST_GODDESS_OFFERING)) {
                                
                                if (checkSpecialDialogueBit(HARVEST_GODDESS_OFFERING_ADVICE_DIALOGUE)) {
                                    
                                    if (!checkSpecialDialogueBit(GRAPEVINE_SPIRIT_AWAKENED_DIALOGUE)) {
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