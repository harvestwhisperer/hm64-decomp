#include "common.h"

#include "game/shop.h"

#include "ld_symbols.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/message.h"
#include "system/overlayScreenSprites.h"
#include "system/globalSprites.h"

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

u16 itemDescriptionTextIndices[TOTAL_SHOP_ITEMS] = {
    0x15, 0x16, 0x17, 0x1C,
    0x18, 0x19, 0x1A, 0x1B,
    0x1D, 0x1E, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x2A,
    0x2B, 0x2C, 0x2D, 0x31,
    0x32, 0x33, 0x45, 0x46,
    0x47, 0x48, 0x00, 0x00,
    0x00, 0x00, 0x65, 0x66,
    0x67, 0x72, 0x73, 0x74,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x27, 0x25, 0x25
};

u16 purchaseConfirmationTextIndices[TOTAL_SHOP_ITEMS] = {
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x153,
    0x153, 0x153, 0x153, 0x153,
    0x153, 0x153, 0x152, 0x152,
    0x152, 0x152, 0x152, 0x152,
    0x152, 0x152, 0x157, 0x157,
    0x157, 0x155, 0x155, 0x155,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x28, 0x4E, 0x4E
};

u16 declinePurchaseTextIndices[TOTAL_SHOP_ITEMS] = {
    0x22, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x22,
    0x22, 0x22, 0x22, 0x3F,
    0x3F, 0x3F, 0x3F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x2F,
    0x2F, 0x2F, 0x2F, 0x2F,
    0x2F, 0x2F, 0x4A, 0x4A,
    0x4A, 0x4A, 0x54, 0x54,
    0x54, 0x54, 0x69, 0x69,
    0x69, 0x76, 0x76, 0x76,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x2A, 0x2C, 0x2C
};

u16 postPurchaseTextIndices[TOTAL_SHOP_ITEMS] = {
    0x21, 0x21, 0x21, 0x21,
    0x21, 0x21, 0x21, 0x21,
    0x21, 0x21, 0x21, 0x3E,
    0x3E, 0x3E, 0x3A, 0x3A,
    0x3A, 0x3A, 0x3A, 0x2E,
    0x2E, 0x2E, 0x2E, 0x35,
    0x35, 0x35, 0x49, 0x49,
    0x49, 0x49, 0x50, 0x50,
    0x50, 0x50, 0x68, 0x68,
    0x68, 0x75, 0x73, 0x75,
    0x7B, 0x7B, 0x7B, 0x7C,
    0x7C, 0x13, 0x13, 0x13,
    0x13, 0x13, 0x1E, 0x1E,
    0x1E, 0x2E, 0x2F, 0x2F
};

u16 notEnoughMoneyTextIndices[TOTAL_SHOP_ITEMS] = {
    0x23, 0x23, 0x23, 0x23,
    0x23, 0x23, 0x23, 0x23,
    0x23, 0x23, 0x23, 0x40,
    0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x30,
    0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x6A, 0x6A,
    0x6A, 0x77, 0x77, 0x77,
    0x39, 0x39, 0x39, 0x39,
    0x39, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x20, 0x20,
    0x20, 0x2B, 0x29, 0x29
};

u16 exceedMaxQuantityTextIndices[TOTAL_SHOP_ITEMS] = {
    0x158, 0x158, 0x158, 0x158,
    0x158, 0x158, 0x158, 0x158,
    0x158, 0x158, 0x158, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x159, 0x159,
    0x159, 0x159, 0x56, 0x56,
    0x56, 0x56, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x2D, 0x00, 0x00
};

// no space in rucksack
u16 sendToFarmTextIndices[TOTAL_SHOP_ITEMS] = {
    0x15A, 0x15A, 0x15A, 0x15A,
    0x15A, 0x15A, 0x15A, 0x15A,
    0x15A, 0x15A, 0x15A, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x15B,
    0x15B, 0x15B, 0x15B, 0x15B,
    0x15B, 0x15B, 0x15C, 0x15C,
    0x15C, 0x15C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x15D, 0x15D, 0x15D,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x2E, 0x2F, 0x2F
};

u16 shopItemTextAddressesIndex[TOTAL_SHOP_ITEMS] = {
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06,
    0x06, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04
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
    0x14, 0x14, 0x14, 0x14, 
    0x14, 0x14, 0x14, 0x14, 
    0x14, 0x14, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x63, 0x01, 
    0x01, 0x63, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 
    0x01, 0x14, 0x01, 0x01
};

u16 shopItemAnimationIndices[TOTAL_SHOP_ITEMS] = {
    0x3A, 0x3B, 0x3C, 0x41,
    0x3D, 0x3E, 0x3F, 0x40,
    0x42, 0x43, 0x44, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x4B,
    0x4A, 0x49, 0x0A, 0x4D,
    0x4C, 0x4C, 0x45, 0x47,
    0x48, 0x46, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x36, 0x4F, 0x4E,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x43, 0x70, 0x70
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
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x07, 0xFF, 0x08, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x00,
    0x01, 0x02, 0x03, 0x05,
    0x04, 0x04, 0x00, 0x01,
    0x02, 0x03, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x01, 0x01,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x01, 0x02, 0x02
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
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09,
    0x09, 0x00, 0x00, 0x00
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
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03,
    0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x03,
    0x03, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03,
    0x03, 0x00, 0x00, 0x00
};

u8 playerShopActionPhase[TOTAL_SHOP_ITEMS] = {
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x02
};

u8 playerShopEndActionPhase[TOTAL_SHOP_ITEMS] = {
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0xFF, 0xFF,
    0xFF, 0xFF, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x02, 0x02, 0x02
};

// player animations table when interacting with shop items
AnimationIndices playerShopAnimationIndices[TOTAL_SHOP_ITEMS] = {
    { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D },
    { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D },
    { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x00, 0x00 },
    { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x06, 0x08 }, { 0x06, 0x08 },
    { 0x14, 0x15 }, { 0x14, 0x15 }, { 0x00, 0x00 }, { 0x0B, 0x0D },
    { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D },
    { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D },
    { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x00, 0x00 }, { 0x00, 0x00 },
    { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 },
    { 0x00, 0x00 }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D },
    { 0x14, 0x15 }, { 0x14, 0x15 }, { 0x14, 0x15 }, { 0x14, 0x15 },
    { 0x14, 0x15 }, { 0x14, 0x15 }, { 0x14, 0x15 }, { 0x14, 0x15 },
    { 0x14, 0x15 }, { 0x06, 0x08 }, { 0x06, 0x08 }, { 0x06, 0x08 },
    { 0x06, 0x08 }, { 0x0B, 0x0D }, { 0x0B, 0x0D }, { 0x0B, 0x0D }
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
    
    dmaSprite(0x62 + shopItemSpriteIndexOffsets[index], 
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
    setSpriteBlendMode(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], (1 | 2));
    setSpriteBaseRGBA(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(SHOP_ITEMS + shopItemSpriteIndexOffsets[index], 0xFF, 0xFF, 0xFF, 0xFF);

    setMapObject(MAIN_MAP_INDEX, shopItemSpriteIndexOffsets[index], SHOP_ITEMS + shopItemSpriteIndexOffsets[index], getAnimationOffsetFromScript(&shopItemsAnimationScripts, getItemAnimationIndex(shopItemAnimationIndices[index])), shopItemMapCoordinates[index].x, shopItemMapCoordinates[index].y, shopItemMapCoordinates[index].z, 0xFF, 0xFF, 0, 0);

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

        gPlayer.itemInfoIndex = initializeHeldItem(shopItemToItemInfoIndices[shopItemIndex], 6, shopItemAnimationIndices[shopItemIndex], 0, 0);
        playSfx(PICKING_UP_SFX);

        if (shopItemSpriteIndexOffsets[shopItemIndex] != 0xFF) {
            deactivateMapObject(MAIN_MAP_INDEX, shopItemSpriteIndexOffsets[shopItemIndex]);
        }
    }

    setPlayerAction(0xB, 0xD);
    
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
                    setOverlayScreenSprites(0, 0x8F, &_shopIconsTextureSegmentRomStart, &_shopIconsTextureSegmentRomEnd, &_shopIconsAssetsIndexSegmentRomStart, &_shopIconsAssetsIndexSegmentRomEnd, (u8*)SHOP_ICONS_TEXTURE_BUFFER, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 2, 0, -16.0f, 64.0f, 256.0f, 10);
                    dmaOverlayScreenSprites(0, shopContext.quantity, 1, 3);
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
                    
                        dmaOverlayScreenSprites(0, shopContext.quantity, 1, 3);
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
                    
                        dmaOverlayScreenSprites(0, shopContext.quantity, 1, 3);
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

                    deactivateOverlayScreenSprites(0);

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
                    
                    deactivateOverlayScreenSprites(0);

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

            dmaOverlayScreenSprites(1, gGold, 8, 3);
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

// TODO: label shop item indices
u8 handlePurchase(u16 storeItemIndex, s32 quantity) {

    u8 result = 0;
    u8 temp;

    if (gGold >= quantity*prices[storeItemIndex]) {
        
        result = 1;
        
        switch (storeItemIndex) {
            
            case 0:

                result = 3;
                
                if ((D_801FC154 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(TURNIP_SEEDS);
                    D_801FC154 += adjustValue(D_801FC154, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 1:

                result = 3;
                
                if ((D_80204DF4 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(POTATO_SEEDS);
                    D_80204DF4 += adjustValue(D_80204DF4, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 2:

                result = 3;
                
                if ((D_8018A725 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(CABBAGE_SEEDS);
                    D_8018A725 += adjustValue(D_8018A725, quantity, MAX_SEEDS);
                }
                
                break;

            case 3:

                result = 3;
                
                if ((D_801C3E28 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(GRASS_SEEDS);
                    D_801C3E28 += adjustValue(D_801C3E28, quantity, MAX_SEEDS);
                }
                
                break;

            case 4:
                
                result = 3;
                
                if ((D_8013DC2C + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(TOMATO_SEEDS);
                    D_8013DC2C += adjustValue(D_8013DC2C, quantity, MAX_SEEDS);
                }
                
                break;

            case 5:

                result = 3;
                
                if ((D_801FAD91 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(CORN_SEEDS);
                    D_801FAD91 += adjustValue(D_801FAD91, quantity, MAX_SEEDS);
                }

                break;
            
            case 6:

                result = 3;
                
                if ((D_80237458 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(EGGPLANT_SEEDS);
                    D_80237458 += adjustValue(D_80237458, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 7:
                
                result = 3;
                
                if ((D_802373E8 + quantity) < MAX_SEEDS + 1) {
                    result = storeTool(STRAWBERRY_SEEDS);
                    D_802373E8 += adjustValue(D_802373E8, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 8:
                
                result = 3;
                
                if ((D_801C3F70 + quantity) < MAX_SEEDS + 1) {
                    
                    result = storeTool(MOON_DROP_SEEDS);
                    D_801C3F70 += adjustValue(D_801C3F70, quantity, MAX_SEEDS);

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
            
            case 0xA:

                result = 3;
                
                if ((D_802373A8 + quantity) < MAX_BLUE_MIST_SEEDS + 1) {
                    
                    result = storeTool(BLUE_MIST_SEEDS);
                    D_802373A8 += adjustValue(D_802373A8, quantity, MAX_BLUE_MIST_SEEDS);

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

            case 0xB:

                gPlayer.heldItem = CAKE;

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
            
            case 0xC:

                gPlayer.heldItem = 8;
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
            
            case 0xD:
                
                gPlayer.heldItem = COOKIES;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0xE:

                gPlayer.heldItem = CAKE;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case 0xF:

                gPlayer.heldItem = PIE;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case 0x10:

                gPlayer.heldItem = 0x78;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case 0x11:

                gPlayer.heldItem = 0x71;
                initializePlayerHeldItem();
                temp = 0;
                break;

            case 0x13:

                result = storeTool(MILKER);            
                break;
            
            case 0x14:

                result = storeTool(BRUSH);               
                break;
            
            case 0x15:
                
                result = storeTool(CLIPPERS);              
                break;
            
            case 0x16:
                            
                result = storeTool(BLUE_FEATHER);                
                break;
            
            case 0x17:

                setLifeEventBit(HAVE_RUG);
                result = 1;
                temp = 0;
                break;

            case 0x18:

                setLifeEventBit(HAVE_CABINET);
                // ??
                setAnimalLocations(HOUSE);
                result = 1;
                temp = 0;
                break;

            case 0x19:

                // blue pot from Saibara?
                setLifeEventBit(0x19);
                // ??
                setAnimalLocations(FARM);
                result = 1;
                temp = 0;
                
                break;
            
            case 0x1A:

                result = 3;

                if ((chickenFeedQuantity + quantity) < MAX_CHICKEN_FEED + 1) {
                    result = storeTool(CHICKEN_FEED);
                    chickenFeedQuantity += adjustValue(chickenFeedQuantity, quantity, MAX_CHICKEN_FEED);
                }

                break;

            case 0x1B:
                result = storeTool(MIRACLE_POTION);
                break;
            
            case 0x1C:
                result = storeTool(COW_MEDICINE);            
                break;
            
            case 0x1D:

                if ((fodderQuantity + quantity) < 999 + 1) {
                    fodderQuantity += adjustValue(fodderQuantity, quantity, 999);
                    result = 2;
                } else {
                    result = 3;
                }

                break;

            case 0x1E:

                // check animal count
                if (getTotalChickenCount() != 6) { 

                    result = 1;
                    
                    setDailyEventBit(1);
                    setDailyEventBit(5);
                    
                    selectedAnimalType = CHICKEN_TYPE;
                    
                } else {
                    result = 3;
                }

                break;

            case 0x1F:

                if (getTotalFarmAnimalsCount() != 8) {
                
                    result = 1;
                    
                    if (!checkLifeEventBit(HAVE_BELL)) {
                        
                        storeTool(BELL);
                        shopGiftTextIndex = 0x53;
                        result = 4;
                        setLifeEventBit(HAVE_BELL);
                        
                    }

                    setDailyEventBit(1);
                    setDailyEventBit(5);
                    selectedAnimalType = SHEEP_TYPE;
                    
                } else {
                    
                    result = 3;
                    
                }

                break;
                
            case 0x20:

                if (getTotalFarmAnimalsCount() != 8) {

                    result = 1;
                    
                    if (!checkLifeEventBit(HAVE_BELL)) {

                        storeTool(BELL);
                        shopGiftTextIndex = 0x53;
                        result = 4;
                        setLifeEventBit(HAVE_BELL);
                        
                    }

                    setDailyEventBit(1);
                    setDailyEventBit(5);
                    selectedAnimalType = COW_TYPE;
                    
                } else {
                    result = 3;
                }

                break;
            
            case 0x22:

                gPlayer.bottleContents = 3;
                result = 1;
                
                break;

            case 0x23:
                
                gPlayer.bottleContents = 4;
                result = 1;
                
                break;

            case 0x24:
                
                gPlayer.bottleContents = 5;
                result = 1;
                
                break;

            case 0x25:

                acquireKeyItem(OCARINA);
                setLifeEventBit(HAVE_OCARINA);
                result = 1;
                temp = 0;
                
                break;
            
            case 0x27:
                setLifeEventBit(HAVE_VASE);
                temp = 0;
                break;
            
            case 0x28:
            
                gPlayer.heldItem = BOTTLE_WITH_WINE_HELD_ITEM;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x29:
                        
                gPlayer.heldItem = 0x79;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x2A:
                                    
                gPlayer.heldItem = 0x7A;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x2B:
                                    
                gPlayer.heldItem = 0x76;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x2C:

                gPlayer.heldItem = 0x71;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x2D:

                gPlayer.heldItem = 0x78;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x2E:
          
                gPlayer.heldItem = 0x76;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x2F:
            
                gPlayer.heldItem = 0x78;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x30:
                      
                gPlayer.heldItem = 0x76;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x31:

                gPlayer.heldItem = COOKIES;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x32:

                gPlayer.heldItem = COTTON_CANDY;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x33:

                gPlayer.heldItem = FRIED_OCTUPUS;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x34:

                gPlayer.heldItem = ROASTED_CORN;
                initializePlayerHeldItem();
                temp = 0;
                break;
            
            case 0x35:

                if ((D_80205636 + quantity) < 20 + 1) {
                    
                    result = storeTool(PINK_CAT_MINT_SEEDS);
                    D_80205636 += adjustValue(D_80205636, quantity, 20);

                    if (checkLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD)) {
                        flowerShopPoints += adjustValue(flowerShopPoints, quantity, MAX_FLOWER_SHOP_POINTS);
                    }
                    
                } else {
                    result = 3;
                }

                break;
            
            case 0x36:
            case 0x37:
                
                result =  storeTool(EMPTY_BOTTLE);           
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

// TODO: label store item indices
bool checkShopItemShouldBeDisplayed(u16 itemIndex) {

    bool result = FALSE;
    
    switch (itemIndex) {
        
        case 0:
            
            if (gSeason != SPRING) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {   
                result = 1;
            }

            break;
        
        case 1:
            
            if (gSeason != SPRING) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {   
                result = 1;
            }

            break;

        case 2:
            
            if (gSeason != SPRING) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {   
                result = 1;
            }
            
            break;


        case 3:

            // FIXME: should be a range
            if ((u32)(gSeason - 1) < SUMMER) {
                result = 1;
            }

            break;

        case 4:
            
            if (gSeason != SUMMER) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {   
                result = 1;
            }
            
            break;

        case 5:
            
            if (gSeason != SUMMER) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {   
                result = 1;
            }
            
            break;

       case 6:
            
            if (gSeason != AUTUMN) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            } else {   
                result = 1;
            }
            
            break;
        
       case 7:
            
            if (gSeason == WINTER) {
                if (checkLifeEventBit(HAVE_GREENHOUSE)) {
                    result = 1;
                }
            }
            
            break;
        
        case 8:
            
            if (gSeason == SPRING) {
                result = 1;
            }
            
            break;

        case 0xA:

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
        
        case 0x13:
            
            if (!getTotalFarmAnimalsByType(2) && !getTotalFarmAnimalsByType(1) && !getTotalFarmAnimalsByType(0) && !getTotalFarmAnimalsByType(3)) {
                // removed code
            } else if (!checkHaveTool(MILKER)) {
                result = 1;
            }

            break;
        
        case 0x14:

            if (!checkHaveTool(BRUSH)) {
                result = 1;
            }

            break;

        case 0x15:

            if (!getTotalFarmAnimalsByType(5) && !getTotalFarmAnimalsByType(4) && !getTotalFarmAnimalsByType(6)) {
                // removed code
            } else if (!checkHaveTool(CLIPPERS)) {
                result = 1;
            }

            break;

        case 0x16:

            if (checkBacheloretteReadyForMarriage() && !checkHaveTool(BLUE_FEATHER) && checkLifeEventBit(HAVE_KITCHEN) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ENGAGED)) {
                result = 1;
            }

            break;   

        case 0x17:

            if (gYear == 1 && !(gSeason < AUTUMN) || !(gYear < 2)) {
                if (!checkLifeEventBit(HAVE_RUG)) {
                    result = 1;
                }
            }

            break;

        case 0x18:
            
            if (gYear == 1 && !(gSeason < SUMMER) || !(gYear < 2)) {
                if (!checkLifeEventBit(HAVE_CABINET)) {
                    result = 1;
                }
            }

            break;
        
        case 0x1A:
            result = 1;
            break;

        case 0x1B:

            if (getTotalFarmAnimalsByType(2) && !getTotalFarmAnimalsByType(3) && getTotalPregnantFamAnimals() == 0) {
                if (!checkHaveTool(MIRACLE_POTION)) {
                    result = 1;
                }
            }
            
            break;
        
        case 0x1C:

            if (getTotalFarmAnimalsCount() != 0) {
                if (!checkHaveTool(COW_MEDICINE)) {
                    result = 1;
                }
            }
        
            break;

        case 0x1D:
            result = 1;
            break;
        
        case 0x22:

            if (gYear == 1 && !(gSeason < SUMMER) || !(gYear < 2)) {
                if (checkHaveTool(EMPTY_BOTTLE) == TRUE) {
                    if (gPlayer.bottleContents == EMPTY) {
                        result = 1;
                    } else {
                        result = 2;
                    }
                } else {
                    result = 3;
                }
            }

            break;

        case 0x23:

            if (gYear == 1 && !(gSeason < AUTUMN) || !(gYear < 2)) {
                if (checkHaveTool(EMPTY_BOTTLE) == TRUE) {
                    if (gPlayer.bottleContents == EMPTY) {
                        result = 1;
                    } else {
                        result = 2;
                    }
                } else {
                    result = 3;
                }
            }

            break;

        case 0x24:

            if (checkLifeEventBit(9)) {
                
                if (checkHaveTool(EMPTY_BOTTLE) == TRUE) {
                    
                    if (gPlayer.bottleContents == EMPTY) {
                        result = 1;
                    } else {
                        result = 2;
                    }
                        
                } else {
                    result = 3;
                }
            }
            
            break;

        case 0x25:

            if (gYear == 1 && !(gSeason < AUTUMN) || !(gYear < SUMMER)) {
                if (!checkHaveKeyItem(OCARINA)) {
                    result = 1;
                }
            }
            
            break;

        case 0x26:
        
            if (!checkLifeEventBit(GAVE_BLUE_ROCK_TO_SAIBARA) && !checkLifeEventBit(HAVE_VASE)) {
                result = 1;
            }

            break;

        case 0x27:

            if (checkLifeEventBit(GAVE_BLUE_ROCK_TO_SAIBARA) && !checkLifeEventBit(HAVE_VASE)) {
                result = 1;
            }

            break;

        case 0x35:

            if (gCutsceneIndex == FLOWER_FESTIVAL) {
                result = 1;
            }

            break;

        case 0x36:

            if (gCutsceneIndex == FLOWER_FESTIVAL) {
                if (!checkHaveTool(EMPTY_BOTTLE)) {
                    result = 1;
                }
            }

            break;

        case 0x37:

            if (gCutsceneIndex == SQUARE_FIREWORKS) {
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
