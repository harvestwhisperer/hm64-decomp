#include "common.h"

#include "game/shop.h"

#include "ld_symbols.h"

#include "system/controller.h"
#include "system/entity.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/message.h"
#include "system/pauseScreen.h"
#include "system/globalSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/itemHandlers.h"
#include "game/level.h"
#include "game/overlayScreens.h"
#include "game/player.h"
#include "game/setCutscenes.h"
#include "game/spriteInfo.h"
 
#include "mainLoop.h"

#include "data/animationScripts/animationScripts.h"

// bss
extern ShopContext shopContext;
// offset to dialogue
extern u16 D_801FB9CC;

// text indices for items
u16 D_80118B70[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118BE0[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118C50[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118CC0[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118D30[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118DA0[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118E10[TOTAL_SHOP_ITEMS] = {
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

// dialogue info indices
u16 D_80118E80[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118F60[TOTAL_SHOP_ITEMS] = {
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

u16 D_80118FD0[TOTAL_SHOP_ITEMS] = {
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

Vec3f D_80119040[TOTAL_SHOP_ITEMS] = {
    {-16.0f, 32.0f, -144.0f}, 
    {-48.0f, 32.0f, -144.0f},
    {-80.0f, 32.0f, -144.0f}, 
    {-112.0f, 32.0f, -16.0f},
    {-112.0f, 32.0f, -112.0f}, 
    {-112.0f, 32.0f, -80.0f}, 
    {-112.0f, 32.0f, -48.0f}, 
    {-112.0f, 32.0f, 16.0f},
    {-112.0f, 32.0f, 16.0f},
    {0.0f, 0.0f, 0.0f},
    {-112.0f, 32.0f, 48.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {-112.0f, 32.0f, 80.0f}, 
    {-112.0f, 32.0f, 48.0f}, 
    {-112.0f, 32.0f, 16.0f}, 
    {-112.0f, 32.0f, 112.0f},
    {-112.0f, 16.0f, -24.0f}, 
    {-48.0f, 32.0f, -52.0f}, 
    {-48.0f, 32.0f, -52.0f},
    {16.0f, 32.0f, -144.0f}, 
    {-48.0f, 32.0f, -144.0f}, 
    {-80.0f, 32.0f, -144.0f}, 
    {-16.0f, 32.0f, -144.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f},
    {-16.0f, 32.0f, -112.0f}, 
    {-48.0f, 32.0f, -112.0f}, 
    {-48.0f, 32.0f, -112.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 0.0f}, 
    {-160.0f, 96.0f, -192.0f}, 
    {80.0f, 128.0f, 208.0f}, 
    {80.0f, 128.0f, -240.0f}
};

void *D_801192E0[TOTAL_SHOP_ITEMS][2] = {
    {(void*)0x8028D530, (void*)0x8028D738}, 
    {(void*)0x8028D940, (void*)0x8028DB48}, 
    {(void*)0x802A3E50, (void*)0x802A4058}, 
    {(void*)0x802A4260, (void*)0x802A4468},
    {(void*)0x802A4670, (void*)0x802A4878},
    {(void*)0x802A4A80, (void*)0x802A4C88}, 
    {(void*)0x802A4E90, (void*)0x802A5098}, 
    {(void*)0x802A52A0, (void*)0x802A54A8},
    {(void*)0x802A52A0, (void*)0x802A54A8}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x802A56B0, (void*)0x802A58B8},
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x8028D530, (void*)0x8028D738}, 
    {(void*)0x8028D940, (void*)0x8028DB48}, 
    {(void*)0x802A3E50, (void*)0x802A4058}, 
    {(void*)0x802A4260, (void*)0x802A4468},
    {(void*)0x802A4A80, (void*)0x802A4E90}, 
    {(void*)0x802A4670, (void*)0x802A4878}, 
    {(void*)0x802A4670, (void*)0x802A4878},
    {(void*)0x8028D530, (void*)0x8028D738}, 
    {(void*)0x8028D940, (void*)0x8028DB48}, 
    {(void*)0x802A3E50, (void*)0x802A4058}, 
    {(void*)0x802A4260, (void*)0x802A4468},
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000},
    {(void*)0x00000000, (void*)0x00000000},
    {(void*)0x8028D530, (void*)0x8028D738}, 
    {(void*)0x8028D940, (void*)0x8028DB48}, 
    {(void*)0x8028D940, (void*)0x8028DB48},
    {(void*)0x00000000, (void*)0x00000000},
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000},
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x00000000, (void*)0x00000000},
    {(void*)0x00000000, (void*)0x00000000}, 
    {(void*)0x8028D530, (void*)0x8028D738}, 
    {(void*)0x8028D940, (void*)0x8028DB48}, 
    {(void*)0x8028D940, (void*)0x8028DB48}
};

// sprite index offsets for shop items
u8 D_801194A0[TOTAL_SHOP_ITEMS] = {
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
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x00,
    0x01, 0x01, 0x00, 0x01,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01
};

u8 D_80119510[TOTAL_SHOP_ITEMS] = {
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01
};

u8 D_80119548[TOTAL_SHOP_ITEMS] = {
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

u8 D_80119580[TOTAL_SHOP_ITEMS] = {
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01
};

u8 D_801195B8[TOTAL_SHOP_ITEMS] = {
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

u8 D_801195F0[TOTAL_SHOP_ITEMS] = {
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

u8 D_80119628[TOTAL_SHOP_ITEMS] = {
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
AnimationIndices D_80119660[TOTAL_SHOP_ITEMS] = {
    {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D},
    {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D},
    {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x00, 0x00},
    {0x00, 0x00}, {0x00, 0x00}, {0x06, 0x08}, {0x06, 0x08},
    {0x14, 0x15}, {0x14, 0x15}, {0x00, 0x00}, {0x0B, 0x0D},
    {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D},
    {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D},
    {0x0B, 0x0D}, {0x0B, 0x0D}, {0x00, 0x00}, {0x00, 0x00},
    {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00}, {0x00, 0x00},
    {0x00, 0x00}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D},
    {0x14, 0x15}, {0x14, 0x15}, {0x14, 0x15}, {0x14, 0x15},
    {0x14, 0x15}, {0x14, 0x15}, {0x14, 0x15}, {0x14, 0x15},
    {0x14, 0x15}, {0x06, 0x08}, {0x06, 0x08}, {0x06, 0x08},
    {0x06, 0x08}, {0x0B, 0x0D}, {0x0B, 0x0D}, {0x0B, 0x0D}
};


extern u16 shopItemsAnimationScripts[];

// forward declarations
u8 handlePurchase(u16 storeItemIndex, s32 quantity);


//INCLUDE_ASM("asm/nonmatchings/game/shop", func_800DC750);

void func_800DC750(u8 storeItemIndex) {

    shopContext.unk_3 = D_801195B8[storeItemIndex];

    shopContext.storeItemIndex = storeItemIndex;
    
    shopContext.itemTextIndex = D_80118B70[storeItemIndex];
    
    shopContext.quantity = 1;
    shopContext.unk_5 = 0;

    setMainLoopCallbackFunctionIndex(SHOP_DIALOGUE);

}

//INCLUDE_ASM("asm/nonmatchings/game/shop", loadShopItemSprite);

void loadShopItemSprite(u8 index) {
    
    dmaSprite(D_801194A0[index] + 0x62, &_holdableItemsTextureSegmentRomStart, &_holdableItemsTextureSegmentRomEnd, &_holdableItemsAssetsIndexSegmentRomStart, &_holdableItemsAssetsIndexSegmentRomEnd, &_holdableItemsSpritesheetIndexSegmentRomStart, &_holdableItemsSpritesheetIndexSegmentRomEnd, D_801192E0[index][0], D_801192E0[index][1], 0x8028DD50, 0x80290550, 0x80293A50, 0x80293C50, 1, 1);
    setSpriteScale(D_801194A0[index] + 0x62, 1.0f, 1.0f, 1.0f);
    setSpriteRenderingLayer(D_801194A0[index] + 0x62, (1 | 2));
    setSpriteBaseRGBA(D_801194A0[index] + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);
    setSpriteColor(D_801194A0[index] + 0x62, 0xFF, 0xFF, 0xFF, 0xFF);

    setMapObject(MAIN_MAP_INDEX, D_801194A0[index], D_801194A0[index] + 0x62, func_80030BA0(&shopItemsAnimationScripts, func_800D5A88(D_80118FD0[index])), D_80119040[index].x, D_80119040[index].y, D_80119040[index].z, 0xFF, 0xFF, 0, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/shop", func_800DC9C0);

void func_800DC9C0(u8 index) {

    if (D_801194A0[index] != 0xFF) {
        deactivateMapObject(MAIN_MAP_INDEX, D_801194A0[index]);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", func_800DC9FC);

void func_800DC9FC(u8 arg0) {
    
    gPlayer.shopItemIndex = arg0;

    if (func_800DCAA0(arg0)) {

        gPlayer.itemInfoIndex = func_800D5308(D_80119548[arg0], 6, D_80118FD0[arg0], 0, 0);
        setAudio(0x24);

        if (D_801194A0[arg0] != 0xFF) {
            deactivateMapObject(MAIN_MAP_INDEX, D_801194A0[arg0]);
        }
    }

    setPlayerAction(0xB, 0xD);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", func_800DCAA0);

u8 func_800DCAA0(u8 index) {
    return D_80119510[index];
}

//INCLUDE_ASM("asm/nonmatchings/game/shop", func_800DCAB8);

void func_800DCAB8(void) {

    bool set = FALSE;
    u8 temp;
    u8 *temp2;
    u8 index;

    switch (shopContext.unk_3) {
        
        case 0:           
            
            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            
            if (D_80119580[shopContext.storeItemIndex] == 0) {
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[shopContext.storeItemIndex], shopContext.itemTextIndex, 0);
                shopContext.unk_3 = 3;
            } else {
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[shopContext.storeItemIndex], shopContext.itemTextIndex, 0x8000);
                shopContext.unk_3++;
            }
            
            break;
        
        case 1:
            
            if (messageBoxes[0].flags & 4) {
                
                func_800B3BD8();

                if (D_80118F60[shopContext.storeItemIndex] >= 2) {
                    func_80045E20(0, 0x8F, &_shopIconsTextureSegmentRomStart, &_shopIconsTextureSegmentRomEnd, &_shopIconsAssetsIndexSegmentRomStart, &_shopIconsAssetsIndexSegmentRomEnd, (void*)SHOP_ICONS_TEXTURE_VADDR_START, (void*)SHOP_ICONS_TEXTURE_VADDR_END, (void* )SHOP_ICONS_ASSETS_INDEX_VADDR_START, (void* )SHOP_ICONS_ASSETS_INDEX_VADDR_END, 0, 2, 0, -16.0f, 64.0f, 256.0f, 0xA);
                    dmaPauseScreenSprites(0, shopContext.quantity, 1, 3);
                    func_800B4238(D_80118F60[shopContext.storeItemIndex]);
                }
                
                func_8003F360(0, -4, 2);
                convertNumberToString(0x12, shopContext.quantity * prices[shopContext.storeItemIndex], 0);
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[shopContext.storeItemIndex], D_80118BE0[shopContext.storeItemIndex], 0x80000);
                
                shopContext.unk_3++;         
            }
            
            break;
        
        case 2:                        
            
            if (checkButtonRepeat(CONTROLLER_1, 0x40000)) {

                if (!set) {
                    
                    if (shopContext.unk_5) {
                        setAudio(2);
                    }
                    
                    shopContext.unk_5 = 0;
                    
                    func_800B4160();
                    set = TRUE;

                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, 0x400000)) {

                if (!set) {

                    if (shopContext.unk_5 == 0) {
                        setAudio(2);
                    }
                    
                    shopContext.unk_5 = 1;
                    
                    func_800B4160();
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, 0x100000)) {

                if (!set) {

                    if (shopContext.quantity < D_80118F60[shopContext.storeItemIndex]) {
                    
                        shopContext.quantity++;
                    
                        dmaPauseScreenSprites(0, shopContext.quantity, 1, 3);
                        convertNumberToString(0x12, shopContext.quantity * prices[shopContext.storeItemIndex], 0);
                    
                        if (D_80118F60[shopContext.storeItemIndex] >= 2) {
                            func_800B4238(D_80118F60[shopContext.storeItemIndex]);
                            setAudio(2);                           
                        }
                        
                    }

                    set = TRUE;       

                }

            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_UP)) {

                if (!set) {

                    if (shopContext.quantity< 2) {
                        set = TRUE;
                    } else {
                    
                        shopContext.quantity--;
                    
                        dmaPauseScreenSprites(0, shopContext.quantity, 1, 3);
                        convertNumberToString(0x12, shopContext.quantity * prices[shopContext.storeItemIndex], 0);

                        if (D_80118F60[shopContext.storeItemIndex] >= 2) {
                            func_800B4238(D_80118F60[shopContext.storeItemIndex]);
                            setAudio(2);
                        }

                    }

                    set = TRUE;  

                }

            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!set) {

                    deactivatePauseScreenSprites(0);

                    resetAnimationState(0x82);
                    resetAnimationState(0x83);
                    resetAnimationState(0x84);
                    deactivateSprite(0x85);
                    deactivateSprite(0x86);
                    deactivateSprite(0x87);
                    
                    if (shopContext.unk_5 == 0) {
                        shopContext.unk_3 = 3;
                        setAudio(0);
                    } else {
                        shopContext.unk_3 = 4;
                        setAudio(1);
                    }
                    
                    set = TRUE;

                }
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {

                if (!set) {
                    
                    deactivatePauseScreenSprites(0);

                    deactivateSprite(0x82);
                    deactivateSprite(0x83);
                    deactivateSprite(0x84);
                    deactivateSprite(0x85);
                    deactivateSprite(0x86);
                    deactivateSprite(0x87);
                    
                    shopContext.unk_5 = 1;
                    shopContext.unk_3 = 4;

                    setAudio(1);

                }
            }
            
            break;     

        case 3:             

            temp = handlePurchase(shopContext.storeItemIndex, shopContext.quantity);

            dmaPauseScreenSprites(1, gGold, 8, 3);
            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            func_8003F360(0, -4, 0);

            switch (temp) {                        
                
                case 0:                                     
                    initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[shopContext.storeItemIndex], D_80118D30[shopContext.storeItemIndex], 0);
                    shopContext.unk_3 = 7;
                    return;
                
                case 1:
                    // FIXME: this is a hack to force match
                    temp2 = &shopContext.storeItemIndex;
                    initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[temp2[0]], D_80118CC0[shopContext.storeItemIndex], 0);
                    break;
                
                case 2:                
                    temp2 = &shopContext.storeItemIndex;
                    if (D_80118E10[temp2[0]]) {
                        initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[temp2[0]], D_80118E10[shopContext.storeItemIndex], 0);
                    } 
                    
                    break;
                
                case 3:
                    initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[shopContext.storeItemIndex], D_80118DA0[shopContext.storeItemIndex], 0);
                    shopContext.unk_3 = 5;
                    return;
                
                case 4:
                    temp2 = &shopContext.storeItemIndex;
                    initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[temp2[0]], D_801FB9CC, 0);
                    break;
                
                default:
                    return;
            }

            if (D_801194D8[temp2[0]]) {
                shopContext.unk_3 = 6;
            } else {
                shopContext.unk_3 = 5;
            }

            break;

       case 4:        

            setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
            setMessageBoxSpriteIndices(0, 0, 0, 0);
            func_8003F360(0, -4, 0);
            initializeMessageBox(MAIN_DIALOGUE_BOX_INDEX, D_80118E80[shopContext.storeItemIndex], D_80118C50[shopContext.storeItemIndex], 0);
            shopContext.unk_3 = 5;

            break;
        
        case 5:         

            if (messageBoxes[0].flags & 4) {
                func_800B2CE0();
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                setPlayerAction(D_80119660[shopContext.storeItemIndex].animationIndex, D_80119660[shopContext.storeItemIndex].nextAnimationIndex);
                gPlayer.animationState = D_801195F0[shopContext.storeItemIndex];
                shopContext.unk_3 = 0xFF;
                func_80059300();
            }

            break;    

        case 6:             

            if (messageBoxes[0].flags & 4) {
                func_800B2CE0();
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                setPlayerAction(D_80119660[shopContext.storeItemIndex].animationIndex, D_80119660[shopContext.storeItemIndex].nextAnimationIndex);
                gPlayer.animationState = 3;
                shopContext.unk_3 = 0xFF;
                func_80059300();
            }

            break;

        case 7:             

            if (messageBoxes[0].flags & 4) {
                func_800B2CE0();
                setMainLoopCallbackFunctionIndex(MAIN_GAME);
                setPlayerAction(D_80119660[shopContext.storeItemIndex].animationIndex, D_80119660[shopContext.storeItemIndex].nextAnimationIndex);
                gPlayer.animationState = D_80119628[shopContext.storeItemIndex];
                shopContext.unk_3 = 0xFF;
                func_80059300();
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
            
            case 0:

                result = 3;
                
                if ((D_801FC154 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(TURNIP_SEEDS);
                    D_801FC154 += adjustValue(D_801FC154, quantity, MAX_SEEDS);
                }
                
                break;

            
            case 1:

                result = 3;
                
                if ((D_80204DF4 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(POTATO_SEEDS);
                    D_80204DF4 += adjustValue(D_80204DF4, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 2:

                result = 3;
                
                if ((D_8018A725 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(CABBAGE_SEEDS);
                    D_8018A725 += adjustValue(D_8018A725, quantity, MAX_SEEDS);
                }
                
                break;

            case 3:

                result = 3;
                
                if ((D_801C3E28 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(GRASS_SEEDS);
                    D_801C3E28 += adjustValue(D_801C3E28, quantity, MAX_SEEDS);
                }
                
                break;

            case 4:
                
                result = 3;
                
                if ((D_8013DC2C + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(TOMATO_SEEDS);
                    D_8013DC2C += adjustValue(D_8013DC2C, quantity, MAX_SEEDS);
                }
                
                break;

            case 5:

                result = 3;
                
                if ((D_801FAD91 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(CORN_SEEDS);
                    D_801FAD91 += adjustValue(D_801FAD91, quantity, MAX_SEEDS);
                }

                break;
            
            case 6:

                result = 3;
                
                if ((D_80237458 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(EGGPLANT_SEEDS);
                    D_80237458 += adjustValue(D_80237458, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 7:
                
                result = 3;
                
                if ((D_802373E8 + quantity) < MAX_SEEDS + 1) {
                    result = func_80065BCC(STRAWBERRY_SEEDS);
                    D_802373E8 += adjustValue(D_802373E8, quantity, MAX_SEEDS);
                }
                
                break;
            
            case 8:
                
                result = 3;
                
                if ((D_801C3F70 + quantity) < MAX_SEEDS + 1) {
                    
                    result = func_80065BCC(MOON_DROP_SEEDS);
                    D_801C3F70 += adjustValue(D_801C3F70, quantity, MAX_SEEDS);

                    if (!checkLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD)) {
                        
                        acquireKeyItem(FLOWER_SHOP_CARD);
                        D_801FB9CC = 0x24;
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
                    
                    result = func_80065BCC(BLUE_MIST_SEEDS);
                    D_802373A8 += adjustValue(D_802373A8, quantity, MAX_BLUE_MIST_SEEDS);

                    if (!checkLifeEventBit(HAVE_FLOWER_SHOP_GIFT_CARD)) {
                        
                        acquireKeyItem(FLOWER_SHOP_CARD);
                        D_801FB9CC = 0x24;
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

                handleEatingAndDrinking();

                if (!checkLifeEventBit(HAVE_BAKEY_CARD)) {
                    
                    acquireKeyItem(BAKERY_CARD);
                    
                    D_801FB9CC = 0x41;    
                    
                    setLifeEventBit(HAVE_BAKEY_CARD);
                    
                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);

                    result = 4;
                    
                } else {
                    
                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);
                    
                }

                break;
            
            case 0xC:

                gPlayer.heldItem = 8;
                handleEatingAndDrinking();

                if (!checkLifeEventBit(HAVE_BAKEY_CARD)) {
                    
                    acquireKeyItem(BAKERY_CARD);
                    
                    D_801FB9CC = 0x41;    
                    
                    setLifeEventBit(HAVE_BAKEY_CARD);
                    
                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);

                    result = 4;
                    
                } else {
                    bakeryCardPoints += adjustValue(bakeryCardPoints, quantity, MAX_BAKERY_CARD_POINTS);
                }   
                
                break;
            
            case 0xD:
                
                gPlayer.heldItem = COOKIES;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0xE:

                gPlayer.heldItem = CAKE;
                handleEatingAndDrinking();
                temp = 0;
                break;

            case 0xF:

                gPlayer.heldItem = PIE;
                handleEatingAndDrinking();
                temp = 0;
                break;

            case 0x10:

                gPlayer.heldItem = 0x78;
                handleEatingAndDrinking();
                temp = 0;
                break;

            case 0x11:

                gPlayer.heldItem = 0x71;
                handleEatingAndDrinking();
                temp = 0;
                break;

            case 0x13:

                result = func_80065BCC(MILKER);            
                break;
            
            case 0x14:

                result = func_80065BCC(BRUSH);               
                break;
            
            case 0x15:
                
                result = func_80065BCC(CLIPPERS);              
                break;
            
            case 0x16:
                            
                result = func_80065BCC(BLUE_FEATHER);                
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
                    result = func_80065BCC(CHICKEN_FEED);
                    chickenFeedQuantity += adjustValue(chickenFeedQuantity, quantity, MAX_CHICKEN_FEED);
                }

                break;

            case 0x1B:
                result = func_80065BCC(MIRACLE_POTION);
                break;
            
            case 0x1C:
                result = func_80065BCC(COW_MEDICINE);            
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
                if (func_8009B564() != 6) { 

                    result = 1;
                    
                    setDailyEventBit(1);
                    setDailyEventBit(5);
                    
                    D_801C4216 = 4;
                    
                } else {
                    result = 3;
                }

                break;

            case 0x1F:

                if (func_8009B374() != 8) {
                
                    result = 1;
                    
                    if (!checkLifeEventBit(HAVE_BELL)) {
                        
                        func_80065BCC(BELL);
                        D_801FB9CC = 0x53;
                        result = 4;
                        setLifeEventBit(HAVE_BELL);
                        
                    }

                    setDailyEventBit(1);
                    setDailyEventBit(5);
                    D_801C4216 = 3;
                    
                } else {
                    
                    result = 3;
                    
                }

                break;
                
            case 0x20:

                if (func_8009B374() != 8) {

                    result = 1;
                    
                    if (!checkLifeEventBit(HAVE_BELL)) {

                        func_80065BCC(BELL);
                        D_801FB9CC = 0x53;
                        result = 4;
                        setLifeEventBit(HAVE_BELL);
                        
                    }

                    setDailyEventBit(1);
                    setDailyEventBit(5);
                    D_801C4216 = 2;
                    
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
            
                gPlayer.heldItem = BOTTLE_WITH_WINE;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x29:
                        
                gPlayer.heldItem = 0x79;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x2A:
                                    
                gPlayer.heldItem = 0x7A;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x2B:
                                    
                gPlayer.heldItem = 0x76;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x2C:

                gPlayer.heldItem = 0x71;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x2D:

                gPlayer.heldItem = 0x78;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x2E:
          
                gPlayer.heldItem = 0x76;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x2F:
            
                gPlayer.heldItem = 0x78;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x30:
                      
                gPlayer.heldItem = 0x76;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x31:

                gPlayer.heldItem = COOKIES;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x32:

                gPlayer.heldItem = COTTON_CANDY;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x33:

                gPlayer.heldItem = FRIED_OCTUPUS;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x34:

                gPlayer.heldItem = ROASTED_CORN;
                handleEatingAndDrinking();
                temp = 0;
                break;
            
            case 0x35:

                if ((D_80205636 + quantity) < 20 + 1) {
                    
                    result = func_80065BCC(PINK_CAT_MINT_SEEDS);
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
                
                result =  func_80065BCC(EMPTY_BOTTLE);           
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

// check if should be selling items
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
            
            if (!func_8009B2BC(2) && !func_8009B2BC(1) && !func_8009B2BC(0) && !func_8009B2BC(3)) {

                break;
                
            }
            
            if (!checkHaveTool(MILKER)) {
                
                result = 1;
            }

            break;
        
        case 0x14:

            if (!checkHaveTool(BRUSH)) {
                result = 1;
            }

            break;

        case 0x15:

            if (!func_8009B2BC(5) && !func_8009B2BC(4) && !func_8009B2BC(6)) {
                break;
            }
            
            if (!checkHaveTool(CLIPPERS)) {
                result = 1;
            }

            break;

        case 0x16:

            // check if girl affection is high enough
            if (func_80061178() && !checkHaveTool(BLUE_FEATHER) && checkLifeEventBit(HAVE_KITCHEN) && !checkLifeEventBit(MARRIED) && !checkLifeEventBit(ENGAGED)) {
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

            if (func_8009B2BC(2) && !func_8009B2BC(3) && !func_8009B320()) {
                if (!checkHaveTool(MIRACLE_POTION)) {
                    result = 1;
                }
            }
            
            break;
        
        case 0x1C:

            if (func_8009B374()) {
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
