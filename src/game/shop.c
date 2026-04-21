#include "common.h"

#include "game/shop.h"

#include "ld_symbols.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/message.h"
#include "system/numberSprites.h"
#include "system/globalSprites.h"
#include "system/sprite.h"

#include "game/animals.h"
#include "game/cutscenes.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/items.h"
#include "game/level.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/time.h"
#include "game/transition.h"

#include "mainLoop.h"

#include "assetIndices/cutscenes.h" 
#include "assetIndices/maps.h"
#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

#include "buffers/buffers.h"

#include "data/animationScripts/animationScripts.h"

// bss
ShopContext shopContext;
u16 shopGiftTextIndex;

// shared bss
u8 flowerShopPoints;
u8 bakeryCardPoints;

// data

/* All arrays below are indexed by the TOTAL_SHOP_ITEMS-sized shop-item-index
 * space defined in shop.h. Rows of 4 align with the physical shop groupings:
 *
 *   row 0  (0-3)    flower shop: turnip/potato/cabbage/grass seeds
 *   row 1  (4-7)    flower shop: tomato/corn/eggplant/strawberry seeds
 *   row 2  (8-11)   flower shop: moon-drop, (gap 9), blue-mist; bakery cake
 *   row 3  (12-15)  bakery pie; inn/diner slots 13-15
 *   row 4  (16-19)  inn/diner slots 16-18; Rick store milker
 *   row 5  (20-23)  Rick store: brush/clippers/blue feather/rug
 *   row 6  (24-27)  Rick store cabinet/pot; ranch chicken feed/miracle potion
 *   row 7  (28-31)  ranch cow medicine/fodder; animal shop chicken/sheep
 *   row 8  (32-35)  ranch store animals; potion shop A/B
 *   row 9  (36-39)  potion shop C; souvenir shop ocarina/vase-early/vase
 *   row 10 (40-43)  inn menu: wine, liquor, beer, milk
 *   row 11 (44-47)  inn menu water; festival vendor slots
 *   row 12 (48-51)  festival vendor slots
 *   row 13 (52-55)  festival vendor slot; flower-fest seeds; flower-fest / fireworks empty-bottle grants
 */

u16 itemDescriptionTextIndices[TOTAL_SHOP_ITEMS] = {
    21, 22, 23, 28,
    24, 25, 26, 27,
    29, 30, 31, 0,
    0, 0, 0, 0,
    0, 0, 0, 42,
    43, 44, 45, 49,
    50, 51, 69, 70,
    71, 72, 0, 0,
    0, 0, 101, 102,
    103, 114, 115, 116,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 39, 37, 37
};

u16 purchaseConfirmationTextIndices[TOTAL_SHOP_ITEMS] = {
    32, 32, 32, 32,
    32, 32, 32, 32,
    32, 32, 32, 0,
    0, 0, 0, 0,
    0, 0, 0, 339,
    339, 339, 339, 339,
    339, 339, 338, 338,
    338, 338, 338, 338,
    338, 338, 343, 343,
    343, 341, 341, 341,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 40, 78, 78
};

u16 declinePurchaseTextIndices[TOTAL_SHOP_ITEMS] = {
    34, 34, 34, 34,
    34, 34, 34, 34,
    34, 34, 34, 63,
    63, 63, 63, 63,
    63, 63, 63, 47,
    47, 47, 47, 47,
    47, 47, 74, 74,
    74, 74, 84, 84,
    84, 84, 105, 105,
    105, 118, 118, 118,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 42, 44, 44
};

u16 postPurchaseTextIndices[TOTAL_SHOP_ITEMS] = {
    33, 33, 33, 33,
    33, 33, 33, 33,
    33, 33, 33, 62,
    62, 62, 58, 58,
    58, 58, 58, 46,
    46, 46, 46, 53,
    53, 53, 73, 73,
    73, 73, 80, 80,
    80, 80, 104, 104,
    104, 117, 115, 117,
    123, 123, 123, 124,
    124, 19, 19, 19,
    19, 19, 30, 30,
    30, 46, 47, 47
};

u16 notEnoughMoneyTextIndices[TOTAL_SHOP_ITEMS] = {
    35, 35, 35, 35,
    35, 35, 35, 35,
    35, 35, 35, 64,
    64, 64, 64, 64,
    64, 64, 64, 48,
    48, 48, 48, 48,
    48, 48, 75, 75,
    75, 75, 75, 75,
    75, 75, 106, 106,
    106, 119, 119, 119,
    57, 57, 57, 57,
    57, 21, 21, 21,
    21, 21, 32, 32,
    32, 43, 41, 41
};

u16 exceedMaxQuantityTextIndices[TOTAL_SHOP_ITEMS] = {
    344, 344, 344, 344,
    344, 344, 344, 344,
    344, 344, 344, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 345, 345,
    345, 345, 86, 86,
    86, 86, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 45, 0, 0
};

// no space in rucksack
u16 sendToFarmTextIndices[TOTAL_SHOP_ITEMS] = {
    346, 346, 346, 346,
    346, 346, 346, 346,
    346, 346, 346, 0,
    0, 0, 0, 0,
    0, 0, 0, 347,
    347, 347, 347, 347,
    347, 347, 348, 348,
    348, 348, 0, 0,
    0, 0, 0, 0,
    0, 349, 349, 349,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 46, 47, 47
};

u16 shopItemTextAddressesIndex[TOTAL_SHOP_ITEMS] = {
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 6, 6, 6,
    6, 4, 4, 4,
    4, 4, 4, 4,
    4, 4, 4, 4
};

u16 prices[TOTAL_SHOP_ITEMS] = {
    200, 200, 200, 500,
    300, 300, 300, 500,
    300, 0, 500, 300,
    350, 300, 300, 350,
    200, 0, 0, 1800,
    600, 1000, 980, 3000,
    2000, 10000, 10, 3000,
    1000, 30, 1500, 4000,
    6000, 0, 1500, 1000,
    2000, 1200, 0, 2000,
    300, 300, 350, 150,
    0, 150, 200, 150,
    200, 300, 120, 200,
    250, 300, 200, 200
};

u16 shopItemMaxQuantities[TOTAL_SHOP_ITEMS] = {
    20, 20, 20, 20,
    20, 20, 20, 20,
    20, 20, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 99, 1,
    1, 99, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 20, 1, 1
};

u16 shopItemAnimationIndices[TOTAL_SHOP_ITEMS] = {
    58, 59, 60, 65,
    61, 62, 63, 64,
    66, 67, 68, 0,
    0, 0, 0, 0,
    0, 0, 0, 75,
    74, 73, 10, 77,
    76, 76, 69, 71,
    72, 70, 0, 0,
    0, 0, 0, 0,
    0, 54, 79, 78,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 67, 112, 112
};

Vec3f shopItemMapCoordinates[TOTAL_SHOP_ITEMS] = {
    { -16.0f, 32.0f, -144.0f }, 
    { -48.0f, 32.0f, -144.0f },
    { -80.0f, 32.0f, -144.0f }, 
    { -112.0f, 32.0f, -16.0f },
    { -112.0f, 32.0f, -112.0f }, 
    { -112.0f, 32.0f, -80.0f }, 
    { -112.0f, 32.0f, -48.0f }, 
    { -112.0f, 32.0f, 16.0f },
    { -112.0f, 32.0f, 16.0f },
    { 0.0f, 0.0f, 0.0f },
    { -112.0f, 32.0f, 48.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { -112.0f, 32.0f, 80.0f }, 
    { -112.0f, 32.0f, 48.0f }, 
    { -112.0f, 32.0f, 16.0f }, 
    { -112.0f, 32.0f, 112.0f },
    { -112.0f, 16.0f, -24.0f }, 
    { -48.0f, 32.0f, -52.0f }, 
    { -48.0f, 32.0f, -52.0f },
    { 16.0f, 32.0f, -144.0f }, 
    { -48.0f, 32.0f, -144.0f }, 
    { -80.0f, 32.0f, -144.0f }, 
    { -16.0f, 32.0f, -144.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f },
    { -16.0f, 32.0f, -112.0f }, 
    { -48.0f, 32.0f, -112.0f }, 
    { -48.0f, 32.0f, -112.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { 0.0f, 0.0f, 0.0f }, 
    { -160.0f, 96.0f, -192.0f }, 
    { 80.0f, 128.0f, 208.0f }, 
    { 80.0f, 128.0f, -240.0f}
};

u8 *shopItemTextureBuffers[TOTAL_SHOP_ITEMS][2] = {
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 0) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 0) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 1) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 2), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 2) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 3), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 3) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 4), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 4) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 5), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 5) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 5), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 5) },  
    { 0, 0 },
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 6), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 6) },  
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 0) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 0) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 1) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 3), SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 4) },
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 2), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 2) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 2), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 2) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 0) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 0) },  
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_2_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_2_BASE, 1) },  
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 0) },  // display slot 1
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) },  // display slot 2
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) },  // display slot 2 (duplicate)
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 0), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 0) },  // display slot 1
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) },  // display slot 2
    { SHOP_DISPLAY_TEX1(SHOP_DISPLAY_REGION_1_BASE, 1), SHOP_DISPLAY_TEX2(SHOP_DISPLAY_REGION_1_BASE, 1) }   // display slot 2 (duplicate)
};

// sprite index offsets for shop items
u8 shopItemSpriteIndexOffsets[TOTAL_SHOP_ITEMS] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    7, 255, 8, 255,
    255, 255, 255, 255,
    255, 255, 255, 0,
    1, 2, 3, 5,
    4, 4, 0, 1,
    2, 3, 255, 255,
    255, 255, 255, 255,
    255, 0, 1, 1,
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 1, 2, 2
};

u8 D_801194D8[TOTAL_SHOP_ITEMS] = {
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, TRUE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, TRUE,
    TRUE, TRUE, TRUE, FALSE,
    TRUE, TRUE, FALSE, TRUE,
    TRUE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, TRUE, FALSE, TRUE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, TRUE, TRUE
};

u8 shopItemPickUpEnabled[TOTAL_SHOP_ITEMS] = {
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, TRUE,
    TRUE, TRUE, TRUE, FALSE,
    FALSE, FALSE, TRUE, TRUE,
    TRUE, TRUE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, TRUE, TRUE, TRUE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE,
    FALSE, TRUE, TRUE, TRUE
};

u8 shopItemToItemInfoIndices[TOTAL_SHOP_ITEMS] = {
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 9, 9, 9,
    9, 0, 0, 0
};

// check if need message box flag 0x8000
u8 D_80119580[TOTAL_SHOP_ITEMS] = {
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, FALSE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE,
    TRUE, TRUE, TRUE, TRUE
};

u8 shopItemStartingMode[TOTAL_SHOP_ITEMS] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 3,
    3, 3, 3, 3,
    3, 3, 4, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 3, 3,
    3, 4, 0, 0,
    0, 0, 0, 0,
    3, 3, 3, 3,
    3, 3, 3, 3,
    3, 3, 3, 3,
    3, 0, 0, 0
};

u8 playerShopActionPhase[TOTAL_SHOP_ITEMS] = {
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 0, 0,
    0, 0, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 0, 0,
    0, 2, 2, 2,
    2, 2, 2, 2,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 2, 2, 2
};

u8 playerShopEndActionPhase[TOTAL_SHOP_ITEMS] = {
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 255, 255,
    255, 255, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 0, 0,
    0, 2, 2, 2,
    2, 2, 2, 2,
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 255, 255, 255,
    255, 2, 2, 2
};

// player animations table when interacting with shop items
AnimationIndices playerShopAnimationIndices[TOTAL_SHOP_ITEMS] = {
    { 11, 13 }, { 11, 13 }, { 11, 13 }, { 11, 13 },
    { 11, 13 }, { 11, 13 }, { 11, 13 }, { 11, 13 },
    { 11, 13 }, { 11, 13 }, { 11, 13 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 6, 8 }, { 6, 8 },
    { 20, 21 }, { 20, 21 }, { 0, 0 }, { 11, 13 },
    { 11, 13 }, { 11, 13 }, { 11, 13 }, { 11, 13 },
    { 11, 13 }, { 11, 13 }, { 11, 13 }, { 11, 13 },
    { 11, 13 }, { 11, 13 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 11, 13 }, { 11, 13 }, { 11, 13 },
    { 20, 21 }, { 20, 21 }, { 20, 21 }, { 20, 21 },
    { 20, 21 }, { 20, 21 }, { 20, 21 }, { 20, 21 },
    { 20, 21 }, { 6, 8 }, { 6, 8 }, { 6, 8 },
    { 6, 8 }, { 11, 13 }, { 11, 13 }, { 11, 13 }
};


// forward declarations
u8 handlePurchase(u16 storeItemIndex, s32 quantity);


//INCLUDE_ASM("asm/nonmatchings/game/shop", intializeShopDialogue);

void intializeShopDialogue(u8 storeItemIndex) {

    shopContext.mode = shopItemStartingMode[storeItemIndex];

    shopContext.storeItemIndex = storeItemIndex;
    
    shopContext.itemTextIndex = itemDescriptionTextIndices[storeItemIndex];
    
    shopContext.quantity = 1;
    shopContext.buySelected = FALSE;

    setMainLoopCallbackFunctionIndex(SHOP_DIALOGUE);

}

//INCLUDE_ASM("asm/nonmatchings/game/shop", loadShopItemSprite);

void loadShopItemSprite(u8 index) {
    
    // TODO: label 98 with offset macro
    dmaSprite(shopItemSpriteIndexOffsets[index] + 98,
        &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, 
        &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, 
        &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, 
        shopItemTextureBuffers[index][0], shopItemTextureBuffers[index][1], 
        (u16*)ENTITY_SLOTS_8_13_PALETTE,
        (AnimationFrameMetadata*)ENTITY_SLOTS_8_13_ANIM_METADATA,
        (u32*)ENTITY_SLOTS_8_13_SPRITESHEET_INDEX,
        (u32*)ENTITY_SLOTS_8_13_TEXTURE_TO_PALETTE_LOOKUP, 
        1, TRUE);

    setSpriteScale(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], 1.0f, 1.0f, 1.0f);
    setSpriteBlendMode(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], SPRITE_BLEND_ALPHA_DECAL);
    setSpriteBaseRGBA(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], 255, 255, 255, 255);
    setSpriteColor(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], 255, 255, 255, 255);

    setMapObject(MAIN_MAP_INDEX, shopItemSpriteIndexOffsets[index], SHOP_ITEMS + shopItemSpriteIndexOffsets[index], getAnimationOffsetFromScript(&heldItemsAnimationScripts, getItemAnimationIndex(shopItemAnimationIndices[index])), shopItemMapCoordinates[index].x, shopItemMapCoordinates[index].y, shopItemMapCoordinates[index].z, 0xFF, 0xFF, 0, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/shop", deactivateShopItemMapObject);

void deactivateShopItemMapObject(u8 index) {

    if (shopItemSpriteIndexOffsets[index] != 0xFF) {
        deactivateMapObject(MAIN_MAP_INDEX, shopItemSpriteIndexOffsets[index]);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/shop", handlePickUpShopItem);

void handlePickUpShopItem(u8 shopItemIndex) {
    
    gPlayer.shopItemIndex = shopItemIndex;

    if (checkCanPickUpShopItem(shopItemIndex)) {

        gPlayer.itemInfoIndex = initializeHeldItem(shopItemToItemInfoIndices[shopItemIndex], ITEM_STATE_PICKUP, shopItemAnimationIndices[shopItemIndex], 0, 0);
        playSfx(PICKING_UP_SFX);

        if (shopItemSpriteIndexOffsets[shopItemIndex] != 0xFF) {
            deactivateMapObject(MAIN_MAP_INDEX, shopItemSpriteIndexOffsets[shopItemIndex]);
        }
    }

    setPlayerAction(SHOPPING, ANIM_SHOPPING);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", checkCanPickUpShopItem);

u8 checkCanPickUpShopItem(u8 index) {
    return shopItemPickUpEnabled[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", shopDialogueCallback);

void shopDialogueCallback(void) {

    bool set = FALSE;
    u8 temp;
    u8 *temp2;
    u8 index;

    switch (shopContext.mode) {
        
        case 0:           
            
            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            setMessageBoxInterpolationWithFlags(0, -4, 0);
            
            if (D_80119580[shopContext.storeItemIndex] == FALSE) {
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[shopContext.storeItemIndex], shopContext.itemTextIndex, 0);
                shopContext.mode = 3;
            } else {
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[shopContext.storeItemIndex], shopContext.itemTextIndex, 0x8000);
                shopContext.mode++;
            }
            
            break;
        
        // purchase confirmation
        case 1:
            
            if (messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & 4) {
                
                loadShopIcons();

                if (shopItemMaxQuantities[shopContext.storeItemIndex] >= 2) {
                    setNumberSprites(0, 0x8F, &_shopIconsTextureSegmentRomStart, &_shopIconsTextureSegmentRomEnd, &_shopIconsAssetsIndexSegmentRomStart, &_shopIconsAssetsIndexSegmentRomEnd, (u8*)SHOP_ICONS_TEXTURE_BUFFER, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 2, 0, -16.0f, 64.0f, 256.0f, 10);
                    dmaNumberSprites(0, shopContext.quantity, 1, 3);
                    updateShopQuantityArrows(shopItemMaxQuantities[shopContext.storeItemIndex]);
                }
                
                setMessageBoxInterpolationWithFlags(0, -4, 2);
                convertNumberToGameVariableString(0x12, shopContext.quantity * prices[shopContext.storeItemIndex], 0);
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[shopContext.storeItemIndex], purchaseConfirmationTextIndices[shopContext.storeItemIndex], MESSAGE_BOX_MODE_NO_INPUT);
                
                shopContext.mode++;         

            }
            
            break;
        
        // handle purchase
        case 2:                        
            
            if (checkButtonRepeat(CONTROLLER_1, 0x40000)) {

                if (!set) {
                    
                    if (shopContext.buySelected) {
                        playSfx(2);
                    }
                    
                    shopContext.buySelected = FALSE;
                    
                    updateShopBuyAnimation();
                    set = TRUE;

                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {

                if (!set) {

                    if (shopContext.buySelected == FALSE) {
                        playSfx(2);
                    }
                    
                    shopContext.buySelected = TRUE;
                    
                    updateShopBuyAnimation();
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {

                if (!set) {

                    if (shopContext.quantity < shopItemMaxQuantities[shopContext.storeItemIndex]) {
                    
                        shopContext.quantity++;
                    
                        dmaNumberSprites(0, shopContext.quantity, 1, 3);
                        convertNumberToGameVariableString(0x12, shopContext.quantity * prices[shopContext.storeItemIndex], 0);
                    
                        if (shopItemMaxQuantities[shopContext.storeItemIndex] >= 2) {
                            updateShopQuantityArrows(shopItemMaxQuantities[shopContext.storeItemIndex]);
                            playSfx(2);                           
                        }
                        
                    }

                    set = TRUE;       

                }

            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {

                if (!set) {

                    if (shopContext.quantity< 2) {
                        set = TRUE;
                    } else {
                    
                        shopContext.quantity--;
                    
                        dmaNumberSprites(0, shopContext.quantity, 1, 3);
                        convertNumberToGameVariableString(0x12, shopContext.quantity * prices[shopContext.storeItemIndex], 0);

                        if (shopItemMaxQuantities[shopContext.storeItemIndex] >= 2) {
                            updateShopQuantityArrows(shopItemMaxQuantities[shopContext.storeItemIndex]);
                            playSfx(2);
                        }

                    }

                    set = TRUE;  

                }

            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!set) {

                    deactivateNumberSprites(0);

                    resetAnimationState(0x82);
                    resetAnimationState(0x83);
                    resetAnimationState(0x84);
                    deactivateSprite(0x85);
                    deactivateSprite(0x86);
                    deactivateSprite(0x87);
                    
                    if (shopContext.buySelected == FALSE) {
                        shopContext.mode = 3;
                        playSfx(0);
                    } else {
                        shopContext.mode = 4;
                        playSfx(1);
                    }
                    
                    set = TRUE;

                }
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {

                if (!set) {
                    
                    deactivateNumberSprites(0);

                    deactivateSprite(0x82);
                    deactivateSprite(0x83);
                    deactivateSprite(0x84);
                    deactivateSprite(0x85);
                    deactivateSprite(0x86);
                    deactivateSprite(0x87);
                    
                    shopContext.buySelected = TRUE;
                    shopContext.mode = 4;

                    playSfx(1);

                }
                
            }
            
            break;     

        case 3:             

            temp = handlePurchase(shopContext.storeItemIndex, shopContext.quantity);

            dmaNumberSprites(1, gGold, 8, 3);
            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            setMessageBoxInterpolationWithFlags(0, -4, 0);

            switch (temp) {                        
                
                // not enough money
                case 0:                                     
                    initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[shopContext.storeItemIndex], notEnoughMoneyTextIndices[shopContext.storeItemIndex], 0);
                    shopContext.mode = 7;
                    return;
                
                // successful purchase
                case 1:
                    // FIXME: fake
                    temp2 = &shopContext.storeItemIndex;
                    initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[temp2[0]], postPurchaseTextIndices[shopContext.storeItemIndex], 0);
                    break;
                
                // no space in rucksack
                case 2:                
                    temp2 = &shopContext.storeItemIndex;
                    if (sendToFarmTextIndices[temp2[0]]) {
                        initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[temp2[0]], sendToFarmTextIndices[shopContext.storeItemIndex], 0);
                    } 
                    
                    break;
                
                // exceed max quantity
                case 3:
                    initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[shopContext.storeItemIndex], exceedMaxQuantityTextIndices[shopContext.storeItemIndex], 0);
                    shopContext.mode = 5;
                    return;
                
                case 4:
                    temp2 = &shopContext.storeItemIndex;
                    initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[temp2[0]], shopGiftTextIndex, 0);
                    break;
                
                default:
                    return;
            }

            if (D_801194D8[temp2[0]]) {
                shopContext.mode = 6;
            } else {
                shopContext.mode = 5;
            }

            break;

        case 4:        

            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            setMessageBoxInterpolationWithFlags(0, -4, 0);
            initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, shopItemTextAddressesIndex[shopContext.storeItemIndex], declinePurchaseTextIndices[shopContext.storeItemIndex], 0);
            shopContext.mode = 5;

            break;
        
        case 5:         

            if (messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & 4) {
                closeOverlayScreen();
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                setPlayerAction(playerShopAnimationIndices[shopContext.storeItemIndex].animationIndex, playerShopAnimationIndices[shopContext.storeItemIndex].nextAnimationIndex);
                gPlayer.actionPhase = playerShopActionPhase[shopContext.storeItemIndex];
                shopContext.mode = 0xFF;
                resumeGameplay();
            }

            break;    

        case 6:             

            if (messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & 4) {
                closeOverlayScreen();
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                setPlayerAction(playerShopAnimationIndices[shopContext.storeItemIndex].animationIndex, playerShopAnimationIndices[shopContext.storeItemIndex].nextAnimationIndex);
                gPlayer.actionPhase = 3;
                shopContext.mode = 0xFF;
                resumeGameplay();
            }

            break;

        case 7:             

            if (messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & 4) {
                closeOverlayScreen();
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                setPlayerAction(playerShopAnimationIndices[shopContext.storeItemIndex].animationIndex, playerShopAnimationIndices[shopContext.storeItemIndex].nextAnimationIndex);
                gPlayer.actionPhase = playerShopEndActionPhase[shopContext.storeItemIndex];
                shopContext.mode = 0xFF;
                resumeGameplay();
            }

            break;

        default:
            break;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", handlePurchase);

u8 handlePurchase(u16 storeItemIndex, s32 quantity) {

    u8 result = 0;
    u8 temp;

    if (gGold >= quantity*prices[storeItemIndex]) {

        result = 1;

        switch (storeItemIndex) {

            case FLOWER_SHOP_TURNIP_SEEDS_ITEM:

                result = 3;

                if ((turnipSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(TURNIP_SEEDS);
                    turnipSeedsQuantity += adjustValue(turnipSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_POTATO_SEEDS_ITEM:

                result = 3;

                if ((potatoSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(POTATO_SEEDS);
                    potatoSeedsQuantity += adjustValue(potatoSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_CABBAGE_SEEDS_ITEM:

                result = 3;

                if ((cabbageSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(CABBAGE_SEEDS);
                    cabbageSeedsQuantity += adjustValue(cabbageSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_GRASS_SEEDS_ITEM:

                result = 3;

                if ((grassSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(GRASS_SEEDS);
                    grassSeedsQuantity += adjustValue(grassSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_TOMATO_SEEDS_ITEM:

                result = 3;

                if ((tomatoSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(TOMATO_SEEDS);
                    tomatoSeedsQuantity += adjustValue(tomatoSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_CORN_SEEDS_ITEM:

                result = 3;

                if ((cornSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(CORN_SEEDS);
                    cornSeedsQuantity += adjustValue(cornSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_EGGPLANT_SEEDS_ITEM:

                result = 3;

                if ((eggplantSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(EGGPLANT_SEEDS);
                    eggplantSeedsQuantity += adjustValue(eggplantSeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_STRAWBERRY_SEEDS_ITEM:

                result = 3;

                if ((strawberrySeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(STRAWBERRY_SEEDS);
                    strawberrySeedsQuantity += adjustValue(strawberrySeedsQuantity, quantity, MAX_SEEDS);
                }

                break;

            case FLOWER_SHOP_MOON_DROP_SEEDS_ITEM:
                
                result = 3;
                
                if ((moondropSeedsQuantity + quantity) < MAX_SEEDS + 1) {
                    
                    result = storeTool(MOON_DROP_SEEDS);
                    moondropSeedsQuantity += adjustValue(moondropSeedsQuantity, quantity, MAX_SEEDS);

                    if (!checkLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD)) {
                        
                        acquireKeyItem(FLOWER_SHOP_CARD);
                        shopGiftTextIndex = 0x24;
                        setLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD);
                        flowerShopPoints += adjustValue(flowerShopPoints, quantity, MAX_FLOWER_SHOP_POINTS);
                        result = 4;
                        
                    } else {
                        
                        flowerShopPoints += adjustValue(flowerShopPoints, quantity, MAX_FLOWER_SHOP_POINTS);
                    }
                    
                }
                
                break;
            
            case FLOWER_SHOP_BLUE_MIST_SEEDS_ITEM:

                result = 3;

                if ((blueMistSeedsQuantity + quantity) < MAX_BLUE_MIST_SEEDS + 1) {

                    result = storeTool(BLUE_MIST_SEEDS);
                    blueMistSeedsQuantity += adjustValue(blueMistSeedsQuantity, quantity, MAX_BLUE_MIST_SEEDS);

                    if (!checkLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD)) {

                        acquireKeyItem(FLOWER_SHOP_CARD);
                        shopGiftTextIndex = 0x24;
                        setLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD);
                        flowerShopPoints += adjustValue(flowerShopPoints, quantity, MAX_FLOWER_SHOP_POINTS);
                        result = 4;

                    } else {

                        flowerShopPoints += adjustValue(flowerShopPoints, quantity, MAX_FLOWER_SHOP_POINTS);

                    }

                    setLifeEventBit(BOUGHT_BLUE_MIST_SEEDS);

                }

                break;

            case BAKERY_CAKE_ITEM:

                gPlayer.heldItem = CAKE_HELD_ITEM;

                initializePlayerHeldItem();

                if (!checkLifeEventBit(HAVE_BAKEY_CARD)) {

                    acquireKeyItem(BAKERY_CARD);
                    shopGiftTextIndex = 0x41;
                    setLifeEventBit(HAVE_BAKEY_CARD);

                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);

                    result = 4;

                } else {

                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);

                }

                break;

            case BAKERY_PIE_ITEM:

                gPlayer.heldItem = PIE_HELD_ITEM;
                initializePlayerHeldItem();

                if (!checkLifeEventBit(HAVE_BAKEY_CARD)) {

                    acquireKeyItem(BAKERY_CARD);

                    shopGiftTextIndex = 0x41;

                    setLifeEventBit(HAVE_BAKEY_CARD);

                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);

                    result = 4;

                } else {
                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);
                }

                break;

            case BAKERY_COOKIES:
                gPlayer.heldItem = COOKIES_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case BAKERY_CAKE:
                gPlayer.heldItem = CAKE_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case BAKERY_PIE:
                gPlayer.heldItem = PIE_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case BAKERY_TEA:
                gPlayer.heldItem = BOTTLE_WITH_TEA_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case BAKERY_WATER:
                gPlayer.heldItem = BOTTLE_WITH_WATER_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case RICK_STORE_MILKER_ITEM:
                result = storeTool(MILKER);
                break;

            case RICK_STORE_BRUSH_ITEM:
                result = storeTool(BRUSH);
                break;

            case RICK_STORE_CLIPPERS_ITEM:
                result = storeTool(CLIPPERS);
                break;

            case RICK_STORE_BLUE_FEATHER_ITEM:
                result = storeTool(BLUE_FEATHER);
                break;

            case RICK_STORE_RUG_ITEM:
                setLifeEventBit(HAVE_RUG);
                result = 1;
                temp = 0;
                break;

            case RICK_STORE_CABINET_ITEM:
                setLifeEventBit(HAVE_CABINET);
                setAnimalLocations(HOUSE);
                result = 1;
                temp = 0;
                break;

            case RICK_STORE_ICE_CREAM_MAKER_ITEM:

                setLifeEventBit(HAVE_ICE_CREAM_MAKER);
                setAnimalLocations(FARM);
                result = 1;
                temp = 0;

                break;

            case RANCH_STORE_CHICKEN_FEED_ITEM:

                result = 3;

                if ((chickenFeedQuantity + quantity) < MAX_CHICKEN_FEED + 1) {
                    result = storeTool(CHICKEN_FEED);
                    chickenFeedQuantity += adjustValue(chickenFeedQuantity, quantity, MAX_CHICKEN_FEED);
                }

                break;

            case RANCH_STORE_MIRACLE_POTION_ITEM:
                result = storeTool(MIRACLE_POTION);
                break;

            case RANCH_STORE_COW_MEDICINE_ITEM:
                result = storeTool(COW_MEDICINE);
                break;

            case RANCH_STORE_FODDER_ITEM:

                if ((fodderQuantity + quantity) < 999 + 1) {
                    fodderQuantity += adjustValue(fodderQuantity, quantity, 999);
                    result = 2;
                } else {
                    result = 3;
                }

                break;

            case RANCH_STORE_CHICKEN_ITEM:

                // check animal count
                if (getTotalChickenCount() != 6) {

                    result = 1;

                    setDailyEventBit(PURCHASED_ANIMAL);
                    setDailyEventBit(DAILY_ANIMAL_TRANSACTION);

                    selectedAnimalType = CHICKEN_TYPE;

                } else {
                    result = 3;
                }

                break;

            case RANCH_STORE_SHEEP_ITEM:

                if (getTotalFarmAnimalsCount() != 8) {
                
                    result = 1;
                    
                    if (!checkLifeEventBit(HAVE_BELL)) {
                        
                        storeTool(BELL);
                        shopGiftTextIndex = 83;
                        result = 4;
                        setLifeEventBit(HAVE_BELL);
                        
                    }

                    setDailyEventBit(PURCHASED_ANIMAL);
                    setDailyEventBit(DAILY_ANIMAL_TRANSACTION);
                    selectedAnimalType = SHEEP_TYPE;
                    
                } else {
                    
                    result = 3;
                    
                }

                break;

            case RANCH_STORE_COW_ITEM:

                if (getTotalFarmAnimalsCount() != 8) {

                    result = 1;

                    if (!checkLifeEventBit(HAVE_BELL)) {

                        storeTool(BELL);
                        shopGiftTextIndex = 83;
                        result = 4;
                        setLifeEventBit(HAVE_BELL);

                    }

                    setDailyEventBit(PURCHASED_ANIMAL);
                    setDailyEventBit(DAILY_ANIMAL_TRANSACTION);
                    selectedAnimalType = COW_TYPE;

                } else {
                    result = 3;
                }

                break;

            case POTION_SHOP_CURE_ALL_ITEM:
                gPlayer.bottleContents = BOTTLE_CONTENTS_CURE_ALL_MEDICINE;
                result = 1;

                break;

            case POTION_SHOP_VITAMIN_ITEM:
                gPlayer.bottleContents = BOTTLE_CONTENTS_VITAMIN_PREPARATION;
                result = 1;

                break;

            case POTION_SHOP_STRONG_VITAMINS_ITEM:
                gPlayer.bottleContents = BOTTLE_CONTENTS_VITAMIN_GOLD;
                result = 1;

                break;

            case SOUVENIR_SHOP_OCARINA_ITEM:
                acquireKeyItem(OCARINA);
                setLifeEventBit(HAVE_OCARINA);
                result = 1;
                temp = 0;

                break;

            case SOUVENIR_SHOP_BLUE_VASE_ITEM:
                setLifeEventBit(HAVE_VASE);
                temp = 0;
                break;

            case TAVERN_WINE_ITEM:
                gPlayer.heldItem = BOTTLE_WITH_WINE_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case TAVERN_LIQUOR_ITEM:
                gPlayer.heldItem = 121;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case TAVERN_BEER_ITEM:
                gPlayer.heldItem = 122;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case TAVERN_MILK_ITEM:
                gPlayer.heldItem = BOTTLE_WITH_MILK_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case TAVERN_WATERN_ITEM:
                gPlayer.heldItem = BOTTLE_WITH_WATER_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case HORSE_RACE_TEA:
                gPlayer.heldItem = BOTTLE_WITH_TEA_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case HORSE_RACE_MILK:
                gPlayer.heldItem = BOTTLE_WITH_MILK_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case DOG_RACE_TEA:
                gPlayer.heldItem = BOTTLE_WITH_TEA_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case DOR_RACE_MILK:
                gPlayer.heldItem = BOTTLE_WITH_MILK_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case RACE_COOKIES:
                gPlayer.heldItem = COOKIES_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case FESTIVAL_COTTON_CANDY:
                gPlayer.heldItem = COTTON_CANDY_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case FESTIVAL_FRIED_OCTOPUS:
                gPlayer.heldItem = FRIED_OCTUPUS_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case FESTIVAL_ROASTED_CORN:
                gPlayer.heldItem = ROASTED_CORN_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case FLOWER_FESTIVAL_PINK_CAT_MINT_SEEDS_ITEM:

                if ((pinkCatMintSeedsQuantity + quantity) < 20 + 1) {

                    result = storeTool(PINK_CAT_MINT_SEEDS);
                    pinkCatMintSeedsQuantity += adjustValue(pinkCatMintSeedsQuantity, quantity, 20);

                    if (checkLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD)) {
                        flowerShopPoints += adjustValue(flowerShopPoints, quantity, MAX_FLOWER_SHOP_POINTS);
                    }

                } else {
                    result = 3;
                }

                break;

            case FLOWER_FESTIVAL_EMPTY_BOTTLE_ITEM:
            case FIREWORKS_FESTIVAL_EMPTY_BOTTLE_ITEM:
                result = storeTool(EMPTY_BOTTLE);
                break;

            default:
                break;
       
        }

        // FIXME: shouldn't be necessary
        temp = result - 1;

        // should be:        
        // if (0 < result && result < 3 || result == 4) {
        if (temp < 2 || result == 4) {
            gGold += adjustValue(gGold, (-quantity * prices[storeItemIndex]), MAX_GOLD);
        }

    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", checkShopItemShouldBeDisplayed);

bool checkShopItemShouldBeDisplayed(u16 itemIndex) {

    bool result = FALSE;

    switch (itemIndex) {

        case FLOWER_SHOP_TURNIP_SEEDS_ITEM:

            if (gSeason != SPRING) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {
                result = 1;
            }

            break;

        case FLOWER_SHOP_POTATO_SEEDS_ITEM:

            if (gSeason != SPRING) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {
                result = 1;
            }

            break;

        case FLOWER_SHOP_CABBAGE_SEEDS_ITEM:

            if (gSeason != SPRING) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {
                result = 1;
            }

            break;


        case FLOWER_SHOP_GRASS_SEEDS_ITEM:

            // FIXME: should be a range
            if ((u32)(gSeason - 1) < SUMMER) {
                result = 1;
            }

            break;

        case FLOWER_SHOP_TOMATO_SEEDS_ITEM:

            if (gSeason != SUMMER) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {
                result = 1;
            }

            break;

        case FLOWER_SHOP_CORN_SEEDS_ITEM:

            if (gSeason != SUMMER) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {
                result = 1;
            }

            break;

       case FLOWER_SHOP_EGGPLANT_SEEDS_ITEM:

            if (gSeason != AUTUMN) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {
                result = 1;
            }

            break;

       case FLOWER_SHOP_STRAWBERRY_SEEDS_ITEM:

            if (gSeason == WINTER) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            }

            break;

        case FLOWER_SHOP_MOON_DROP_SEEDS_ITEM:

            if (gSeason == SPRING) {
                result = 1;
            }

            break;

        case FLOWER_SHOP_BLUE_MIST_SEEDS_ITEM:

            if (!checkLifeEventBit(BOUGHT_BLUE_MIST_SEEDS)) {

                if (gYear >= 2) {

                    // FIXME: probably should be a switch
                    if (gSeason != SUMMER) {

                        if (gSeason == SPRING) {

                            if (gDayOfMonth >= 24) {
                                result = 1;
                                break;
                            }
                        }

                        break;

                    }

                    result = 1;

                }
            }

            break;

        case RICK_STORE_MILKER_ITEM:

            if (!getTotalFarmAnimalsByType(2) && !getTotalFarmAnimalsByType(1) && !getTotalFarmAnimalsByType(0) && !getTotalFarmAnimalsByType(3)) {
                // removed code
            } else if (!checkHaveTool(MILKER)) {
                result = 1;
            }

            break;

        case RICK_STORE_BRUSH_ITEM:

            if (!checkHaveTool(BRUSH)) {
                result = 1;
            }

            break;

        case RICK_STORE_CLIPPERS_ITEM:

            if (!getTotalFarmAnimalsByType(5) && !getTotalFarmAnimalsByType(4) && !getTotalFarmAnimalsByType(6)) {
                // removed code
            } else if (!checkHaveTool(CLIPPERS)) {
                result = 1;
            }

            break;

        case RICK_STORE_BLUE_FEATHER_ITEM:

            if (checkBacheloretteReadyForMarriage() && !checkHaveTool(BLUE_FEATHER) && checkLifeEventBit(HAVE_KITCHEN) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ENGAGED)) {
                result = 1;
            }

            break;

        case RICK_STORE_RUG_ITEM:

            if (gYear == 1 && !(gSeason < AUTUMN) || !(gYear < 2)) {
                if (!checkLifeEventBit(HAVE_RUG)) {
                    result = 1;
                }
            }

            break;

        case RICK_STORE_CABINET_ITEM:

            if (gYear == 1 && !(gSeason < SUMMER) || !(gYear < 2)) {
                if (!checkLifeEventBit(HAVE_CABINET)) {
                    result = 1;
                }
            }

            break;

        case RANCH_STORE_CHICKEN_FEED_ITEM:
            result = 1;
            break;

        case RANCH_STORE_MIRACLE_POTION_ITEM:

            if (getTotalFarmAnimalsByType(2) && !getTotalFarmAnimalsByType(3) && getTotalPregnantFamAnimals() == 0) {
                if (!checkHaveTool(MIRACLE_POTION)) {
                    result = 1;
                }
            }

            break;

        case RANCH_STORE_COW_MEDICINE_ITEM:

            if (getTotalFarmAnimalsCount() != 0) {
                if (!checkHaveTool(COW_MEDICINE)) {
                    result = 1;
                }
            }

            break;

        case RANCH_STORE_FODDER_ITEM:
            result = 1;
            break;

        case POTION_SHOP_CURE_ALL_ITEM:

            if (gYear == 1 && !(gSeason < SUMMER) || !(gYear < 2)) {
                if (checkHaveTool(EMPTY_BOTTLE) == TRUE) {
                    if (gPlayer.bottleContents == BOTTLE_CONTENTS_EMPTY) {
                        result = 1;
                    } else {
                        result = 2;
                    }
                } else {
                    result = 3;
                }
            }

            break;

        case POTION_SHOP_VITAMIN_ITEM:

            if (gYear == 1 && !(gSeason < AUTUMN) || !(gYear < 2)) {
                if (checkHaveTool(EMPTY_BOTTLE) == TRUE) {
                    if (gPlayer.bottleContents == BOTTLE_CONTENTS_EMPTY) {
                        result = 1;
                    } else {
                        result = 2;
                    }
                } else {
                    result = 3;
                }
            }

            break;

        case POTION_SHOP_STRONG_VITAMINS_ITEM:

            if (checkLifeEventBit(GAVE_PONTATA_ROOT_TO_POTION_SHOP_DEALER)) {

                if (checkHaveTool(EMPTY_BOTTLE) == TRUE) {

                    if (gPlayer.bottleContents == BOTTLE_CONTENTS_EMPTY) {
                        result = 1;
                    } else {
                        result = 2;
                    }

                } else {
                    result = 3;
                }
            }

            break;

        case SOUVENIR_SHOP_OCARINA_ITEM:

            if (gYear == 1 && !(gSeason < AUTUMN) || !(gYear < SUMMER)) {
                if (!checkHaveKeyItem(OCARINA)) {
                    result = 1;
                }
            }

            break;

        case SOUVENIR_SHOP_VASE_EARLY_ITEM:

            if (!checkLifeEventBit(GAVE_BLUE_ROCK_TO_SAIBARA) && !checkLifeEventBit(HAVE_VASE)) {
                result = 1;
            }

            break;

        case SOUVENIR_SHOP_BLUE_VASE_ITEM:

            if (checkLifeEventBit(GAVE_BLUE_ROCK_TO_SAIBARA) && !checkLifeEventBit(HAVE_VASE)) {
                result = 1;
            }

            break;

        case FLOWER_FESTIVAL_PINK_CAT_MINT_SEEDS_ITEM:

            if (gCutsceneIndex == FLOWER_FESTIVAL) {
                result = 1;
            }

            break;

        case FLOWER_FESTIVAL_EMPTY_BOTTLE_ITEM:

            if (gCutsceneIndex == FLOWER_FESTIVAL) {
                if (!checkHaveTool(EMPTY_BOTTLE)) {
                    result = 1;
                }
            }

            break;

        case FIREWORKS_FESTIVAL_EMPTY_BOTTLE_ITEM:

            if (gCutsceneIndex == FIREWORKS_FESTIVAL) {
                if (!checkHaveTool(EMPTY_BOTTLE)) {
                    result = 1;
                }
            }

            break;


        default:
            break;

    }

    return result;
    
}
