#include "common.h"

#include "game/overlayScreens.h"

#include "system/audio.h"
#include "system/controller.h"
#include "system/dialogue.h"
#include "system/entity.h"
#include "system/globalSprites.h"
#include "system/math.h"
#include "system/message.h"
#include "system/overlayScreenSprites.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/initialize.h"
#include "game/level.h"
#include "game/transition.h"
#include "game/items.h"
#include "game/gameFile.h"
#include "game/npc.h"
#include "game/player.h"
#include "game/shop.h"
#include "game/time.h"
#include "game/tv.h"

#include "mainLoop.h"

#include "assetIndices/maps.h"
#include "assetIndices/sprites.h"

#include "buffers/buffers.h"

#include "data/animationScripts/animationScripts.h"

#include "ld_symbols.h"

// bss
OverlayScreenTable overlayScreenTable;
u32 D_80189858;

u8 D_80205640[10];

// shared bss
extern u8 D_801890E8[31];
extern u8 D_80189108[5][7];
extern u8 D_801C3F70;
extern u32 D_801C3F78;
extern u32 D_80205204;
extern u8 D_80205636;

// game variable string indices
// set by cutscenes
extern u8 D_8018A72C[6];
extern u8 D_80237420[31];

// data

f32 pauseScreenToolXPositions[] = {
    -74.0f,
    -39.0f,
    -4.0f,
    31.0f,
};

f32 pauseScreenCurrentToolXPosition = -28.0f;

f32 pauseScreenToolYPositions[] = {
    33.0f,
    -2.0f
};

f32 pauseScreenCurrentToolYPosition = 80.0f;

f32 pauseScreenBelongingsXPosition[] = {
    -74.0f,
    -39.0f,
    -4.0f,
    31.0f
};

f32 pauseScreenHeldItemXPosition = 20.0f;
f32 pauseScreenBelongingsYPosition[] = {
    -47.0f,
    -82.0f
};

f32 pauseScreenHeldItemYPosition = 80.0f;
f32 pauseScreenKeyItemsXPosition[] = {
    77.0f,
    115.0f,
};

f32 pauseScreenKeyItemsYPosition[] = {
    31.0f,
    -6.0f,
    -43.0f,
    -80.0f,
};

f32 toolboxToolSlotsXPosition[] = {
    -32.0f,
    3.0f,
    38.0f,
    73.0f
};

f32 toolboxToolSlotsYPosition[] = {
    -47.0f,
    -82.0f,
};

f32 toolboxPlayerToolSlotsXPosition[] = {
    -32.0f,
    3.0f,
    38.0f,
    73.0f,
};

f32 toolboxCurrentToolXPosition = 0.0f;

f32 toolboxPlayerToolSlotsYPosition[] = {
    33.0f,
    -2.0f,
};

f32 toolboxCurrentToolYPosition = 80.0f;

// freezer
f32 D_80116D10[] = {
    -32.0f,
    3.0f,
    38.0f,
    73.0f,
};

f32 D_80116D20[] = {
    33.0f,
    -2.0f,
};

f32 D_80116D28[] = {
    -32.0f,
    3.0f,
    38.0f,
    73.0f,
};

f32 D_80116D38 = 0.0f;

f32 D_80116D3C[] = {
    -47.0f,
    -82.0f,
};

f32 D_80116D44 = 80.0f;

// cabinet
f32 D_80116D48[] = {
    -32.0f,
    3.0f,
    38.0f,
    73.0f,
};

f32 D_80116D58[] = {
    33.0f,
    -2.0f
};

f32 D_80116D60[] = {
    -32.0f,
    3.0f,
    38.0f,
    73.0f
};

f32 D_80116D70 = 0.0f;

f32 D_80116D74[] = {
    -47.0f,
    -82.0f,
};

f32 D_80116D7C = 80.0f;

f32 loadGameScreenCursorHandXPositionsGameSelect[] = {
    -128.0f,
    -128.0f,
    -128.0f,
    -128.0f,
};

f32 loadGameScreenCursorHandYPositionsGameSelect[] = {
    42.0f,
    -2.0f,
    -46.0f,
    -90.0f
};

f32 loadGameScreenCursorHandXPositionsActionSelect[] = {
    96.0f,
    96.0f,
    96.0f,
    96.0f,
};

f32 loadGameScreenCursorHandYPositionsActionSelect[] = {
    52.0f,
    20.0f,
    -12.0f,
    -92.0f,
};

f32 farmRankingsCursorHandXPositions[5] = {
    -128.0f,
    -128.0f,
    -128.0f,
    -128.0f,
    -128.0f,
};

f32 farmRankingsCursorHandYPositions[5] = {
    32.0f,
    0.0f,
    -32.0f,
    -64.0f,
    -96.0f,
};

f32 houseExtensionCursorHandXPositions[] = {
    -112.0f,
    -112.0f,
    -112.0f,
    -112.0f,
    -112.0f,
    -112.0f,
};

f32 houseExtensionCursorHandYPositions[] = {
    32.0f,
    16.0f,
    0.0f,
    -16.0f,
    -32.0f,
    -48.0f,
};

// kitchen picture
f32 D_80116E18[] = {
    -72.0f,
    -56.0f,
    -40.0f,
    -24.0f,
    -8.0f,
    8.0f,
    24.0f,
    40.0f,
    56.0f,
    72.0f,
};

f32 D_80116E40[] = {
    0.0f,
    48.0f,
    16.0f,
    32.0f,
    8.0f,
    56.0f,
    24.0f,
    40.0f,
    -16.0f,
    20.0f,
};

// calendar
f32 D_80116E68[] = {
    108.0f,
    126.0f,
    108.0f,
    126.0f,
    108.0f,
    126.0f,
    108.0f,
    126.0f,
    108.0f,
    126.0f,
};

f32 D_80116E90[] = {
    30.0f,
    30.0f,
    6.0f,
    6.0f,
    -18.0f,
    -18.0f,
    -42.0f,
    -42.0f,
    -66.0f,
    -66.0f,
};

// animation indices
// tools
volatile u16 D_80116EB8[] = {
     0xFFFF,
     0x0000,
     0x0003,
     0x0006,
     0x0009,
     0x000C,
     0x000F,
     0x0010,
     0x0011,
     0x0012,
     0x0013,
     0x0014,
     0x0015,
     0x0016,
     0x0017,
     0x0018,
     0x0019,
     0x001A,
     0x001B,
     0x001C,
     0x001D,
     0x001E,
     0x001F,
     0x0020,
     0x0021,
     0x0022,
     0x0023,
     0x0024,
     0x0025,
     0x0026,
};

// key items
volatile u16 D_80116EF4[] = {
     0xFFFF,
     0x0035,
     0x0072,
     0x0073,
     0x0090,
     0x0091,
     0x0092,
     0x0032,
     0x0093,
     0x0094,
     0x0095,
     0x0096,
     0x0097,
     0x0098,
     0x0099,
     0x009A,
     0x007E,
     0x007F,
     0x0080,
     0x0081,
     0x0020,
     0x00E8,
     0x0000,
     0x0000,
};


// power nut bits
u32 D_80116F24[] = { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200 };

u16 D_80116F4C[] = {
    0x0000,
    0x002F,
    0x0030,
    0x0031,
    0x0032,
    0x0033,
    0x0040,
    0x003F,
    0x0041,
    0x004A,
    0x0034,
    0x0036,
    0x0035,
    0x0037,
    0x0038,
    0x0039,
    0x0012,
    0x003A,
    0x003B,
    0x003C,
    0x003E,
    0x004B,
    0x004D,
    0x0052,
    0x004C,
    0x0050,
    0x004E,
    0x003D,
    0x0051,
    0x0042,
};

u16 D_80116F88[] = {
    0x0000,
    0x0085,
    0x0084,
    0x007F,
    0x0065,
    0x0066,
    0x0000,
    0x0067,
    0x0068,
    0x0069,
    0x0000,
    0x0000,
    0x0000,
    0x0053,
    0x0055,
    0x0057,
    0x0059,
    0x005B,
    0x005D,
    0x005F,
    0x0060,
    0x0061,
    0x0062,
    0x0063,
    0x0064,
    0x006A,
    0x006B,
    0x006C,
    0x006D,
    0x006E,
    0x006F,
    0x0070,
    0x0086,
    0x0000,
    0x0071,
    0x0073,
    0x0074,
    0x0075,
    0x0076,
    0x0077,
    0x0072,
    0x0078,
    0x0079,
    0x007C,
    0x007B,
    0x007A,
    0x0080,
    0x0082,
    0x0081,
    0x0083,
    0x007E,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x004F,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0099,
    0x0099,
    0x0099,
    0x0099,
    0x0099,
    0x0099,
    0x0099,
    0x0099,
    0x00AD,
    0x00AD,
    0x00AD,
    0x00AD,
    0x00AD,
    0x00AD,
    0x00AD,
    0x00AD,
    0x00AE,
    0x00AE,
    0x00AE,
    0x00AE,
    0x00AE,
    0x00AE,
    0x00AE,
    0x00AE,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x00A8,
    0x00A8,
    0x00A8,
    0x00A8,
    0x00A8,
    0x00A8,
    0x00A8,
    0x00A8,
    0x00A5,
    0x00A5,
    0x00A5,
    0x00A5,
    0x00A5,
    0x00A5,
    0x00A5,
    0x00A5,
    0x00A4,
    0x00A4,
    0x00A4,
    0x00A4,
    0x00A4,
    0x00A4,
    0x00A4,
    0x00A4,
    0x00A6,
    0x00A6,
    0x00A6,
    0x00A6,
    0x00A6,
    0x00A6,
    0x00A6,
    0x00A6,
    0x00A3,
    0x00A3,
    0x00A3,
    0x00A3,
    0x00A3,
    0x00A3,
    0x00A3,
    0x00A3,
    0x00A7,
    0x00A7,
    0x00A7,
    0x00A7,
    0x00A7,
    0x00A7,
    0x00A7,
    0x00A7,
    0x00A9,
    0x00A9,
    0x00A9,
    0x00A9,
    0x00A9,
    0x00A9,
    0x00A9,
    0x00A9,
    0x009F,
    0x00A0,
    0x009E,
    0x009B,
    0x009C,
    0x00A1,
    0x00A2,
    0x00B1,
    0x00B1,
    0x00B1,
    0x00B1,
    0x00B1,
    0x00B1,
    0x00B1,
    0x00B1,
    0x00B2,
    0x00B2,
    0x00B2,
    0x00B2,
    0x00B2,
    0x00B2,
    0x00B2,
    0x00B2,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

u16 D_80117148[] = {
    0x0000,
    0x00AA,
    0x0087,
    0x0088,
    0x0089,
    0x008A,
    0x008B,
    0x008C,
    0x008D,
    0x008E,
    0x008F,
    0x0090,
    0x0091,
    0x0092,
    0x0093,
    0x0094,
    0x0095,
    0x0096,
    0x0097,
    0x0098,
    0x00AC,
    0x00AB,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
};

// rodata forward declarations
static const u8 D_801220CC[19][6];
static const u8 D_80122140[19][6];

// forward declarations
void func_800B2F34(void); 
void func_800B4160(void);
void func_800B8844(void);
void func_800B8980(void);
void func_800B90E0(void);
void func_800B9914(void);
void func_800B9B8C(void);
void func_800B99EC(void);
f32 func_800BA928(u8, u32);                          
f32 func_800BA9E8(u8, u8);     
void func_800BAAA0(void);         
void func_800BAC7C(void);
void func_800BAF1C(void);
void func_800BB590(void);
void func_800BBAF0(void);
void func_800BBBC8(void);
void func_800BBD34(void);
void func_800BC7D8(void);  
void func_800BC918(void);
void func_800BCBE8(void);
void func_800BD25C(void);
void func_800BD754(void);
void func_800BD82C(void);
void func_800BD998(void);
f32 func_800BE3B8(u8, u8);                          
f32 func_800BE428(u8, u8);
void func_800BE490(void);           
bool func_800BE5D0(void);
void func_800BE684(void);
void func_800BE954(void);
void func_800BF0C8(void);
void func_800BF5C0(void);
void func_800BF698(void);
void func_800BF804(void);
f32 func_800C0238(u8, u8);             
f32 func_800C02A8(u8, u8);    
void func_800C0310(void);
bool func_800C0450(void);
void func_800C0504(void);
u8 func_800C0688(u8 tool);
void func_800C0714(void);
void func_800C0F24(void);
void func_800C1710(void);             
void func_800C2060(void);        
void func_800C2128(void);           
void func_800C2364(void);
void func_800C2504(void);
void func_800C2660(void);
u8 func_800C2780(u8 arg0, u8 arg1);
inline void func_800C292C(void);
void func_800C2A54(void);
void func_800C2F50(void);
void func_800C3108(void);
void func_800C34C0(u8, u8, u8); 
void func_800C39A0(void);
void func_800C3B1C(void);         
void func_800C4A74(void);
void func_800C4E90(void);
void func_800C5080(void);
void func_800C5218(void);
void func_800C5AA4(void);
void func_800C5DA4(void);
void func_800C6E98(void);
f32 func_800BC700(u8, u32);
f32 func_800BC770(u8, u8);
void func_800C7850(void);
void func_800C7C88(f32 x, f32 y);
void func_800C7CE4(f32 x, f32 y);
void func_800C7D40(f32 x, f32 y);      
void func_800C7D9C(f32 x, f32 y);  
void func_800C7DF8(void);
void func_800C7E18(void);
void func_800C7E38(void);
void func_800C7E58(void);
void func_800C7E78(void);
void func_800C80F0(f32 x, f32 y);
void func_800C814C(f32 x, f32 y);
void func_800C81A8(void);
void func_800C81C8(void);
void func_800C81E8(u8 arg0, u32 arg1);
void func_800C9FF0(void);
void func_800CA590(void);
void func_800CC6F0(void);
void func_800CCFC8(void);
void func_800CD628(void);
void func_800CDA6C(void);
void func_800CDFB0(void);
void func_800CE894(u8 arg0);
void func_800CE7A8(u8 arg0);
void func_800CEA74(void);
void func_800CED60(void);
bool func_800CF34C(u8 arg0);
void func_800CF3C4(u8 arg0);
bool func_800CF498(u8 arg0);
void func_800CF544(u8 arg0);
u32 checkRecipe(u16 bitIndex);


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B2CE0);

// close overlay screen
// TODO: seems to be static inline calls (repeated deactivations)
void func_800B2CE0(void) {

    u8 i, j, k;

    func_800C81A8();
    
    i = 0;
    
    func_800C81C8();
    func_800C7DF8();
    func_800C7E18();
    func_800C7E58();
    func_800C7E38();

    deactivateSprite(0x82);
    deactivateSprite(0x83);
    deactivateSprite(0x84);
    deactivateSprite(0x85);
    deactivateSprite(0x86);
    deactivateSprite(0x87);
    deactivateSprite(0x88);
    deactivateSprite(0x81);

    // cursor hand
    resetAnimationState(0x79);

    // clock
    deactivateSprite(0x89);
    deactivateSprite(0x8C);
    deactivateSprite(0x8D);
    deactivateSprite(0x8B);
    deactivateSprite(0x8A);
    deactivateOverlayScreenSprites(0);

    deactivateSprite(0x8E);
    deactivateOverlayScreenSprites(1);
    
    deactivateSprite(0x86);
    deactivateSprite(0x82);
    deactivateSprite(0x83);
    deactivateSprite(0x84);
    deactivateSprite(0x85);

    deactivateSprite(0xB9);
    deactivateSprite(0xBA);
    deactivateSprite(0xBB);
    deactivateSprite(0xBC);
    deactivateSprite(0xBD);
    deactivateSprite(0xBE);

    deactivateSprite(0x82);
    deactivateSprite(0x83);
    deactivateSprite(0x84);
    deactivateSprite(0x85);
    deactivateSprite(0x86);
    
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    deactivateOverlayScreenSprites(7);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);
    deactivateOverlayScreenSprites(0xA);
    deactivateOverlayScreenSprites(0xB);
    deactivateOverlayScreenSprites(0xC);
    deactivateOverlayScreenSprites(0xD);
    deactivateOverlayScreenSprites(0xE);

    for (i = 0; i < 40; i++) {
        deactivateSprite(0xA3 + i);
    }

    for (j = 0; j < 30; j++) {
        deactivateSprite(0x8F + j);
    }

    for (k = 0; k < 10; k++) {
        deactivateSprite(0xAD + k);
    }

}
 
//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B2F34);

// clock
void func_800B2F34(void) {

    // clock and background
    dmaSprite(CLOCK, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (AnimationFrameMetadata*)0x80282200, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOCK, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(CLOCK, 1);
    setSpriteColor(CLOCK, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(CLOCK, 2);

    if (checkLifeEventBit(0x59)) {
        // cow skin background
        startSpriteAnimation(CLOCK, 6, 0);
    } else {
        startSpriteAnimation(CLOCK, 0, 0);
    }

    // minute hand
    dmaSprite(0x8C, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (AnimationFrameMetadata*)0x80282200, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8C, -79.0f, 81.0f, 256.0f);
    setSpriteRotation(0x8C, 0.0f, 0.0f, (360 - (gMinutes * 6)));
    setBilinearFiltering(0x8C, 1);
    setSpriteColor(0x8C, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8C, 2);
    startSpriteAnimation(0x8C, 4, 0);
    
    // hour hand
    dmaSprite(0x8D, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (AnimationFrameMetadata*)0x80282200, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8D, -80.0f, 80.0f, 256.0f);
    setSpriteRotation(0x8D, 0.0f, 0.0f, ((360 - (gHour * 30)) - (gMinutes * 0.5)));
    setBilinearFiltering(0x8D, 1);
    setSpriteColor(0x8D, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8D, 2);
    startSpriteAnimation(0x8D, 5, 0);
    
    // season name
    dmaSprite(0x8B, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (AnimationFrameMetadata*)0x80282200, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8B, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x8B, 1);
    setSpriteColor(0x8B, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8B, 2);
    startSpriteAnimation(0x8B, 1, gSeason - 1);
    
    // day of week
    dmaSprite(0x8A, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8027EC00, NULL, (u16*)0x80281E00, (AnimationFrameMetadata*)0x80282200, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8A, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x8A, 1);
    setSpriteColor(0x8A, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8A, 2);
    startSpriteAnimation(0x8A, 3, gDayOfWeek);
    setOverlayScreenSprites(0, 0x8F, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, (void*)0x8027EC00, (void*)0x80281E00, (void*)0x80282200, (void*)0x80282400, 0, 2, 0, 0.0f, 0.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0, (u32)gDayOfMonth, 1, 3);
    setOverlayScreenSpritesRGBA(0, 255, 255, 255, 255);
    
}

static inline void fadeInClock() {
    updateSpriteAlpha(CLOCK, 0xFF, 24);
    updateSpriteAlpha(0x8C, 0xFF, 24);
    updateSpriteAlpha(0x8D, 0xFF, 24);
    updateSpriteAlpha(0x8B, 0xFF, 24);
    updateSpriteAlpha(0x8A, 0xFF, 24);
    updateSpriteAlpha(0x8E, 0xFF, 24);
}

static inline void fadeOutClock() {
    updateSpriteAlpha(CLOCK, 0, 24);
    updateSpriteAlpha(0x8C, 0, 24);
    updateSpriteAlpha(0x8D, 0, 24);
    updateSpriteAlpha(0x8B, 0, 24);
    updateSpriteAlpha(0x8A, 0, 24);
    updateSpriteAlpha(0x8E, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3498);

// unused or inline
void func_800B3498(f32 x, f32 y, f32 z, u16 arg3, u32 arg4) {

    // background and "g"
    dmaSprite(0x8E, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80253B00, NULL, (u16*)0x80254A00, (u16*)0x80254E00, (u8*)0x80254F00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8E, x, y, z);
    setBilinearFiltering(0x8E, 1);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8E, arg3);
    startSpriteAnimation(0x8E, 0, 0);

    setOverlayScreenSprites(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 
        (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 
        0, 1, 0, 
        x, y, z + 64.0f, 
        10);

    dmaOverlayScreenSprites(1, arg4, 5, 3);
    setOverlayScreenSpritesRGBA(1, 255, 255, 255, 255);

}


/* TV functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3694);

// c buttons while watching TV
void func_800B3694(void) {

    dmaSprite(0x86, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x86, 1);
    setSpriteColor(0x86, 255, 255, 255, 255);
    startSpriteAnimation(0x86, 0, 0);
    
    dmaSprite(0x82, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    startSpriteAnimation(0x82, 0, 1);
    
    dmaSprite(0x83, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x83, 1);
    setSpriteColor(0x83, 255, 255, 255, 255);
    startSpriteAnimation(0x83, 0, 3);
    
    dmaSprite(0x84, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x84, 1);
    setSpriteColor(0x84, 255, 255, 255, 255);
    startSpriteAnimation(0x84, 0, 5);
    
    dmaSprite(0x85, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x85, 1);
    setSpriteColor(0x85, 255, 255, 255, 255);
    startSpriteAnimation(0x85, 0, 7);
    
    func_800B3A60();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3A60);

// update C buttons based on channel currently being watched
void func_800B3A60(void) {

    resetAnimationState(0x82);
    resetAnimationState(0x83);
    resetAnimationState(0x84);
    resetAnimationState(0x85);
    
    switch (tvContext.channelIndex) {  

        case 0:
            startSpriteAnimation(0x82, 0, 2);
            startSpriteAnimation(0x83, 0, 3);
            startSpriteAnimation(0x84, 0, 5);
            startSpriteAnimation(0x85, 0, 7);
            break;
        case 1:
            startSpriteAnimation(0x82, 0, 1);
            startSpriteAnimation(0x83, 0, 4);
            startSpriteAnimation(0x84, 0, 5);
            startSpriteAnimation(0x85, 0, 7);
            break;
        case 2:
            startSpriteAnimation(0x82, 0, 1);
            startSpriteAnimation(0x83, 0, 3);
            startSpriteAnimation(0x84, 0, 6);
            startSpriteAnimation(0x85, 0, 7);
            break;
        case 3:
            startSpriteAnimation(0x82, 0, 1);
            startSpriteAnimation(0x83, 0, 3);
            startSpriteAnimation(0x84, 0, 5);
            startSpriteAnimation(0x85, 0, 8);
            break;
        }

}

/* end TV functions */


// set background plus + "g"
static inline void setMoneySprites() {

    u32 temp = gGold;

    dmaSprite(0x8E, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80253B00, 0, (u16*)0x80254A00, (u32*)0x80254E00, (u32*)0x80254F00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8E, 0, 0, 256.0f);
    setBilinearFiltering(0x8E, TRUE);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x8E, 2);
    startSpriteAnimation(0x8E, 0, 0);

    setOverlayScreenSprites(1, 0x91, 
        (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 
        (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 
        0, 1, 0, 
        0.0f, 0.0f, 320.0f, 
        10
    );
    
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 255, 255, 255, 255);

}


/* shop functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B3BD8);

void func_800B3BD8(void) {
    
    u32 temp = gGold;

    setMoneySprites();
    
    setSpriteAlpha(0x8E, 0xFF);
    
    // base/neutral player
    dmaSprite(0x82, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802E0EC0, NULL, (u16*)0x802E27C0, (u32*)0x802E28C0, (u32*)0x802E2BC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, -96.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x82, TRUE);
    setSpriteColor(0x82, 255, 255, 255, 255);
    startSpriteAnimation(0x82, 0, 0);
    
    // head shaking no
    dmaSprite(0x83, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802E0EC0, NULL, (u16*)0x802E27C0, (u32*)0x802E28C0, (u32*)0x802E2BC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, -56.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x83, TRUE);
    setSpriteColor(0x83, 255, 255, 255, 255);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802E0EC0, NULL, (u16*)0x802E27C0, (u32*)0x802E28C0, (u32*)0x802E2BC0, NULL, 0, FALSE);
    setBilinearFiltering(0x84, TRUE);
    setSpriteColor(0x84, 255, 255, 255, 255);
    
    // quantity arrows
    dmaSprite(0x85, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802E0EC0, NULL, (u16*)0x802E27C0, (u32*)0x802E28C0, (u32*)0x802E2BC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x85, TRUE);
    setSpriteColor(0x85, 255, 255, 255, 255);
    
    dmaSprite(0x86, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802E0EC0, NULL, (u16*)0x802E27C0, (u32*)0x802E28C0, (u32*)0x802E2BC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x86, TRUE);
    setSpriteColor(0x86, 255, 255, 255, 255);
    
    // background
    dmaSprite(0x87, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802E0EC0, NULL, (u16*)0x802E27C0, (u32*)0x802E28C0, (u32*)0x802E2BC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, -16.0f, 11.0f, 224.0f);
    setBilinearFiltering(0x87, TRUE);
    setSpriteColor(0x87, 255, 255, 255, 255);

    func_800B4160();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B4160);

// update animation for buy/don't buy
void func_800B4160(void) {

    if (!shopContext.buySelected) {
        resetAnimationState(0x82);
        resetAnimationState(0x83);
        resetAnimationState(0x84);
        setSpriteViewSpacePosition(0x84, -96.0f, 64.0f, 256.0f);
        startSpriteAnimation(0x82, 7, 0xFE);
        startSpriteAnimation(0x83, 1, 0);
        startSpriteAnimation(0x84, 5, 0);
    } else {
        resetAnimationState(0x82);
        resetAnimationState(0x83);
        resetAnimationState(0x84);
        setSpriteViewSpacePosition(0x84, -56.0f, 64.0f, 256.0f);
        startSpriteAnimation(0x82, 0, 0);
        startSpriteAnimation(0x83, 8, 0xFE);
        startSpriteAnimation(0x84, 6, 0);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B4238);

// quantity increase/decrease arrows
void func_800B4238(u8 arg0) {
    
    // background
    startSpriteAnimation(0x87, 9, 0xFE);
    
    if (shopContext.quantity != 0 && shopContext.quantity < arg0) {
        startSpriteAnimation(0x85, 3, 0);
    } else {
        resetAnimationState(0x85);
    }
    
    if (shopContext.quantity >= 2 && arg0 >= shopContext.quantity) {
        startSpriteAnimation(0x86, 4, 0);
    } else {
        resetAnimationState(0x86);
    }
    
}

/* end shop functions */


/* load game/select game functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B42E0);

void func_800B42E0(void) {

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 0, 0, 0, 0);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
        
        // select a diary
        dmaSprite(0x82, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (AnimationFrameMetadata*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x82, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x82, 3);
        setBilinearFiltering(0x82, 1);
        startSpriteAnimation(0x82, 0, 0);
        
        // DELETE button
        dmaSprite(0x83, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x83, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x83, 3);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, 3, 0);
        
        // MOVE button
        dmaSprite(0x84, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 1, 0);
        
        // diary book 1
        dmaSprite(0xA3, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA3, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xA3, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA3, 3);
        setBilinearFiltering(0xA3, 1);
        setSpritePaletteIndex(0xA3, 2);
        startSpriteAnimation(0xA3, 4, 0);
        
        // diary book
        dmaSprite(0xA4, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA4, 0.0f, -44.0f, 8.0f);
        setSpriteColor(0xA4, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA4, 3);
        setBilinearFiltering(0xA4, 1);
        setSpritePaletteIndex(0xA4, 3);
        startSpriteAnimation(0xA4, 4, 0);
        
        // diary book
        dmaSprite(0xA5, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA5, 0.0f, -88.0f, 8.0f);
        setSpriteColor(0xA5, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA5, 3);
        setBilinearFiltering(0xA5, 1);
        setSpritePaletteIndex(0xA5, 4);
        startSpriteAnimation(0xA5, 4, 0);
        
        // diary book
        dmaSprite(0xA6, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA6, 0.0f, -132.0f, 8.0f);
        setSpriteColor(0xA6, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA6, 3);
        setBilinearFiltering(0xA6, 1);
        setSpritePaletteIndex(0xA6, 5);
        startSpriteAnimation(0xA6, 4, 0);
        
        // numbers
        dmaSprite(0xA7, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA7, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA7, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA7, 3);
        setBilinearFiltering(0xA7, 1);
        startSpriteAnimation(0xA7, 5, 0);
        
        // numbers
        dmaSprite(0xA8, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA8, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA8, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA8, 3);
        setBilinearFiltering(0xA8, 1);
        startSpriteAnimation(0xA8, 5, 2);
        
        // numbers
        dmaSprite(0xA9, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA9, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA9, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA9, 3);
        setBilinearFiltering(0xA9, 1);
        startSpriteAnimation(0xA9, 5, 4);

        // numbers
        dmaSprite(0xAA, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAA, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xAA, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAA, 3);
        setBilinearFiltering(0xAA, 1);
        startSpriteAnimation(0xAA, 5, 6);
        
        // row background
        dmaSprite(0x85, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 4.0f);
        setSpriteColor(0x85, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x85, 3);
        setBilinearFiltering(0x85, 1);
        setSpritePaletteIndex(0x85, 1);
        startSpriteAnimation(0x85, 2, 0);
        
        // row background
        dmaSprite(0x86, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x86, 0.0f, -44.0f, 4.0f);
        setSpriteColor(0x86, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x86, 3);
        setBilinearFiltering(0x86, 1);
        setSpritePaletteIndex(0x86, 1);
        startSpriteAnimation(0x86, 2, 0);
        
        // row background
        dmaSprite(0x87, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x87, 0.0f, -88.0f, 4.0f);
        setSpriteColor(0x87, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x87, 3);
        setBilinearFiltering(0x87, 1);
        setSpritePaletteIndex(0x87, 1);
        startSpriteAnimation(0x87, 2, 0);
        
        // row background
        dmaSprite(0x88, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x88, 0.0f, -132.0f, 4.0f);
        setSpriteColor(0x88, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x88, 3);
        setBilinearFiltering(0x88, 1);
        setSpritePaletteIndex(0x88, 1);
        startSpriteAnimation(0x88, 2, 0);
        
        dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
        setSpriteViewSpacePosition(CURSOR_HAND, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(CURSOR_HAND, 1);
        setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
        setSpriteRenderingLayer(CURSOR_HAND, 3);
        
        dmaSprite(0xB2, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xB2, 1);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0x80);
        setSpriteRenderingLayer(0xB2, 2);

        dmaSprite(0xAC, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAC, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xAC, 1);
        setSpriteColor(0xAC, 255, 255, 255, 255);
        setSpriteRenderingLayer(0xAC, 3);
        
        // arrows
        func_800C7850();
        
    } else {
        
        // "data" book icon
        dmaSprite(0x82, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x82, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x82, 3);
        setBilinearFiltering(0x82, 1);
        startSpriteAnimation(0x82, 7, 0);
        
        // DELETE button
        dmaSprite(0x83, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x83, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x83, 3);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, 3, 0);
        
        // MOVE button
        dmaSprite(0x84, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 1, 0);
        
        // data book icons

        dmaSprite(0xA3, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA3, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xA3, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA3, 3);
        setBilinearFiltering(0xA3, 1);
        setSpritePaletteIndex(0xA3, 2);
        startSpriteAnimation(0xA3, 8, 0);
        
        dmaSprite(0xA4, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA4, 0.0f, -44.0f, 8.0f);
        setSpriteColor(0xA4, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA4, 3);
        setBilinearFiltering(0xA4, 1);
        setSpritePaletteIndex(0xA4, 3);
        startSpriteAnimation(0xA4, 8, 0);
        
        // numbers

        dmaSprite(0xA7, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA7, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA7, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA7, 3);
        setBilinearFiltering(0xA7, 1);
        startSpriteAnimation(0xA7, 5, 0);
        
        dmaSprite(0xA8, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA8, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA8, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xA8, 3);
        setBilinearFiltering(0xA8, 1);
        startSpriteAnimation(0xA8, 5, 2);
        
        // extra rows for game pak
        dmaSprite(0x85, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 4.0f);
        setSpriteColor(0x85, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x85, 3);
        setBilinearFiltering(0x85, 1);
        setSpritePaletteIndex(0x85, 1);
        startSpriteAnimation(0x85, 2, 0);
        
        dmaSprite(0x86, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x86, 0.0f, -44.0f, 4.0f);
        setSpriteColor(0x86, 0, 0, 0, 0);
        setSpriteRenderingLayer(0x86, 3);
        setBilinearFiltering(0x86, 1);
        setSpritePaletteIndex(0x86, 1);
        startSpriteAnimation(0x86, 2, 0);
        
        // "game will not start" message
        dmaSprite(0xAB, (u32)&_diaryTextureSegmentRomStart, (u32)&_diaryTextureSegmentRomEnd, (u32)&_diaryAssetsIndexSegmentRomStart, (u32)&_diaryAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAB, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xAB, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAB, 3);
        setBilinearFiltering(0xAB, 1);
        startSpriteAnimation(0xAB, 9, 0);
        
        dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
        setSpriteViewSpacePosition(CURSOR_HAND, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(CURSOR_HAND, 1);
        setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
        setSpriteRenderingLayer(CURSOR_HAND, 3);
        
        dmaSprite(0xB2, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xB2, 1);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0x80);
        setSpriteRenderingLayer(0xB2, 2);

        dmaSprite(0xAC, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAC, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xAC, 1);
        setSpriteColor(0xAC, 255, 255, 255, 255);
        setSpriteRenderingLayer(0xAC, 3);
        
        func_800C7850();
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B59AC);

// fade in
void func_800B59AC(void) {
    updateSpriteRGBA(0x80, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x82, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x83, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x84, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA3, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA4, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA5, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA6, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA7, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA8, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xA9, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xAA, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x85, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x86, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x87, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x88, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xAB, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xAD, 255, 255, 255, 255, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B5BD4);

void func_800B5BD4(void) {
    setSpriteColor(0x80, 255, 255, 255, 255);
    setSpriteColor(0x82, 255, 255, 255, 255);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteColor(0x84, 255, 255, 255, 255);
    setSpriteColor(0xA3, 255, 255, 255, 255);
    setSpriteColor(0xA4, 255, 255, 255, 255);
    setSpriteColor(0xA5, 255, 255, 255, 255);
    setSpriteColor(0xA6, 255, 255, 255, 255);
    setSpriteColor(0xA7, 255, 255, 255, 255);
    setSpriteColor(0xA8, 255, 255, 255, 255);
    setSpriteColor(0xA9, 255, 255, 255, 255);
    setSpriteColor(0xAA, 255, 255, 255, 255);
    setSpriteColor(0x85, 255, 255, 255, 255);
    setSpriteColor(0x86, 255, 255, 255, 255);
    setSpriteColor(0x87, 255, 255, 255, 255);
    setSpriteColor(0x88, 255, 255, 255, 255);
    setSpriteColor(0xAB, 255, 255, 255, 255);
    setSpriteColor(0xAD, 255, 255, 255, 255);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B5DA8);

// fade out
void func_800B5DA8(void) {
    updateSpriteRGBA(0x80, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x82, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x83, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x84, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA3, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA4, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA5, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA6, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA7, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA8, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xA9, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAA, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x85, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x86, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x87, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x88, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAB, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAD, 0, 0, 0, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B5FC4);

void func_800B5FC4(u8 arg0, u8 diaryHighlighted, u8 actionColumnHighlighted) {

    switch (arg0) {

        case 0:
            if (diaryHighlighted != 0xFF) {
                setSpriteViewSpacePosition(CURSOR_HAND, loadGameScreenCursorHandXPositionsGameSelect[diaryHighlighted], loadGameScreenCursorHandYPositionsGameSelect[diaryHighlighted], 32.0f);
            }

            if (actionColumnHighlighted != 0xFF) {
                setSpriteViewSpacePosition(CURSOR_HAND, loadGameScreenCursorHandXPositionsActionSelect[actionColumnHighlighted], loadGameScreenCursorHandYPositionsActionSelect[actionColumnHighlighted], 32.0f);
            }
            
            startSpriteAnimation(CURSOR_HAND, 3, 0);

            break;

        case 1:
            if (diaryHighlighted != 0xFF) {
                setSpriteViewSpacePosition(0xB2, loadGameScreenCursorHandXPositionsGameSelect[diaryHighlighted], loadGameScreenCursorHandYPositionsGameSelect[diaryHighlighted], 32.0f);
            }

            if (actionColumnHighlighted != 0xFF) {
                setSpriteViewSpacePosition(0xB2, actionColumnHighlighted, actionColumnHighlighted, 32.0f);
            }
            
            startSpriteAnimation(0xB2, 3, 0);

            break;

        }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B60E4);

// diary selection
void func_800B60E4(u8 diaryHighlighted, u8 actionColumnHighlighted, bool arg2) {

    if (loadGameScreenContext.showControllerPakScreen == FALSE) {

        if (diaryHighlighted != 0xFF) {

            resetAnimationState(0xA3 + diaryHighlighted);
            resetAnimationState(0xA7 + diaryHighlighted);

            if (arg2) {

                startSpriteAnimation(0xA3 + diaryHighlighted, 4, 1);
                setSpritePaletteIndex(0x85 + diaryHighlighted, 0);

                switch (diaryHighlighted) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 1);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 3);
                        break;
                    case 2:
                        startSpriteAnimation(0xA9, 5, 5);
                        break;
                    case 3:
                        startSpriteAnimation(0xAA, 5, 7);
                        break;
                    
                }
                
            } else {

                startSpriteAnimation(0xA3 + diaryHighlighted, 4, 0);
                setSpritePaletteIndex(0x85 + diaryHighlighted, 1);

                switch (diaryHighlighted) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 0);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 2);
                        break;
                    case 2:
                        startSpriteAnimation(0xA9, 5, 4);
                        break;
                    case 3:
                        startSpriteAnimation(0xAA, 5, 6);
                        break;
                    
                }
                
            }
            
        }

        if (loadGameScreenContext.gamePakEnabled) {
            func_800C7C88(64.0f, -108.0f);
        } else {
            func_800C7DF8();
        }
        
    } else {

        if (diaryHighlighted != 0xFF)  {

            resetAnimationState(0xA3 + diaryHighlighted);
            resetAnimationState(0xA7 + diaryHighlighted);

            if (arg2) {
                
                startSpriteAnimation(0xA3 + diaryHighlighted, 8, 1);
                setSpritePaletteIndex(0x85 + diaryHighlighted, 0);

                switch (diaryHighlighted) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 1);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 3);
                        break;
                    
                }
                
            } else {

                startSpriteAnimation(0xA3 + diaryHighlighted, 8, 0);
                setSpritePaletteIndex(0x85 + diaryHighlighted, 1);

                switch (diaryHighlighted) {

                    case 0:
                        startSpriteAnimation(0xA7, 5, 0);
                        break;
                    case 1:
                        startSpriteAnimation(0xA8, 5, 2);
                        break;
                    
                }
                
            }

        }
        
        if (loadGameScreenContext.gamePakEnabled) {
            func_800C7CE4(64.0f, 80.0f);
        } else {
            func_800C7E18();
        }

    }
    
    if (actionColumnHighlighted != 0xFF) {

        if (arg2) {

            switch (actionColumnHighlighted) {
                
                case 0:
                    resetAnimationState(0x83);
                    startSpriteAnimation(0x83, 3, 1);
                    break;

                case 1:
                    resetAnimationState(0x84);
                    startSpriteAnimation(0x84, 1, 1);
                    break;

                case 2:
                    resetAnimationState(0xAD);
                    startSpriteAnimation(0xAD, 6, 1);
                    break;

            }
            
        } else {

            switch (actionColumnHighlighted) {
                
                case 0:
                    resetAnimationState(0x83);
                    startSpriteAnimation(0x83, 3, 0);
                    break;

                case 1:
                    resetAnimationState(0x84);
                    startSpriteAnimation(0x84, 1, 0);
                    break;

                case 2:
                    resetAnimationState(0xAD);
                    startSpriteAnimation(0xAD, 6, 0);
                    break;

            }
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B64E4);

// select diary
void func_800B64E4(u8 diaryHighlighted, u8 actionColumnHighlighted) {

    if (!loadGameScreenContext.showControllerPakScreen) {

        if (diaryHighlighted != 0xFF) {

            resetAnimationState(0xA3 + diaryHighlighted);
            startSpriteAnimation(0xA3 + diaryHighlighted, 4, 0xFF);
            
            switch (diaryHighlighted) {

                case 0:
                    resetAnimationState(0xA7);
                    break; 
                case 1:
                    resetAnimationState(0xA8);
                    break;
                case 2:
                    resetAnimationState(0xA9);
                    break;
                case 3:
                    resetAnimationState(0xAA);
                    break;
                
            }
            
        }
        
    } else {

        if (diaryHighlighted != 0xFF) {

            resetAnimationState(0xA3 + diaryHighlighted);
            startSpriteAnimation(0xA3 + diaryHighlighted, 8, 0xFF);
            
            switch (diaryHighlighted) {

                case 0:
                    resetAnimationState(0xA7);
                    break; 
                case 1:
                    resetAnimationState(0xA8);
                    break;

            }
            
        }
        
    }

    if (actionColumnHighlighted != 0xFF) {
        
        switch (actionColumnHighlighted) {
            case 0:
                resetAnimationState(0x83);
                startSpriteAnimation(0x83, 3, 2);
                break;
            case 1:
                resetAnimationState(0x84);
                startSpriteAnimation(0x84, 1, 2);
                break;    
            case 2:
                resetAnimationState(0xAD);
                startSpriteAnimation(0xAD, 6, 2);
                break; 
        }
        
    }
    
}


/* farm ranking functions */

// INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B6674);

// farm ranking general screen
void func_800B6674(void) {
    
    // cloud text backdrop
    dmaSprite(0x80, (u32)&_namingScreenBackgroundTextureSegmentRomStart, (u32)&_namingScreenBackgroundTextureSegmentRomEnd, (u32)&_namingScreenBackgroundAssetsIndexSegmentRomStart, (u32)&_namingScreenBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (u32*)0x802E0CC0, (u32*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0, 0, 0, 0);
    startSpriteAnimation(0x80, 0, 0);
    
    // "Farm ranking" and background
    dmaSprite(0x82, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);
    
    // slots
    dmaSprite(0x83, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 40.0f, 8.0f);
    setSpriteColor(0x83, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x83, 3);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, 8.0f, 8.0f);
    setSpriteColor(0x84, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x84, 3);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 1, 1);
    
    dmaSprite(0x85, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, -24.0f, 8.0f);
    setSpriteColor(0x85, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x85, 3);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 2);
    
    dmaSprite(0x86, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 0.0f, -56.0f, 8.0f);
    setSpriteColor(0x86, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x86, 3);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 1, 3);
    
    dmaSprite(0x87, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, -88.0f, 8.0f);
    setSpriteColor(0x87, 0, 0, 0, 0);
    setSpriteRenderingLayer(0x87, 3);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 1, 4);

    // if slot has active game, load sprite

    if (gFarmRankingData.flags[0] & 1) {
        dmaSprite(0xAE, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAE, -4.0f, 40.0f, 16.0f);
        setSpriteColor(0xAE, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAE, 3);
        setBilinearFiltering(0xAE, 1);
        startSpriteAnimation(0xAE, 7, gFarmRankingData.seasons[0] - 1);
    }
    
    if (gFarmRankingData.flags[1] & 1) {
        dmaSprite(0xAF, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAF, -4.0f, 8.0f, 16.0f);
        setSpriteColor(0xAF, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xAF, 3);
        setBilinearFiltering(0xAF, 1);
        startSpriteAnimation(0xAF, 7, gFarmRankingData.seasons[1] - 1);
    }
    
    if (gFarmRankingData.flags[2] & 1) {
        dmaSprite(0xB0, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB0, -4.0f, -24.0f, 16.0f);
        setSpriteColor(0xB0, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xB0, 3);
        setBilinearFiltering(0xB0, 1);
        startSpriteAnimation(0xB0, 7, gFarmRankingData.seasons[2] - 1);
    }
    
    if (gFarmRankingData.flags[3] & 1) {
        dmaSprite(0xB1, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB1, -4.0f, -56.0f, 16.0f);
        setSpriteColor(0xB1, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xB1, 3);
        setBilinearFiltering(0xB1, 1);
        startSpriteAnimation(0xB1, 7, gFarmRankingData.seasons[3] - 1);
    }
    
    if (gFarmRankingData.flags[4] & 1) {
        dmaSprite(0xB2, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u32*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, -4.0f, -88.0f, 16.0f);
        setSpriteColor(0xB2, 0, 0, 0, 0);
        setSpriteRenderingLayer(0xB2, 3);
        setBilinearFiltering(0xB2, 1);
        startSpriteAnimation(0xB2, 7, gFarmRankingData.seasons[4] - 1);
    }
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, 0x8023B400, NULL, 0x8023CC00, 0x8023CE00, 0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CURSOR_HAND, 0.0f, 0.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    setSpriteRenderingLayer(CURSOR_HAND, 3);
    
}

// INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7070);

// individual ranking screen
void func_800B7070(u8 arg0) {

    // slot
    dmaSprite(0x82, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x82, 255, 255, 255, 255);
    setSpriteRenderingLayer(0x82, 3);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 1, arg0);
    
    // "Farm ranking" and background
    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x81, 255, 255, 255, 255);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    // player icon
    dmaSprite(0x83, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 2, 0);

    if (gFarmRankingData.flags[arg0] & RANKING_FLAG_MARRIED) {
        
        dmaSprite(0x84, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 255, 255, 255, 255);
        setSpriteRenderingLayer(0x84, 3);
        setBilinearFiltering(0x84, 1);
        // get frame based on wife
        startSpriteAnimation(0x84, 3, gFarmRankingData.wife[arg0]);
        
        dmaSprite(0xAE, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAE, 0.0f, 32.0f, 8.0f);
        setSpriteColor(0xAE, 255, 255, 255, 255);
        setSpriteRenderingLayer(0xAE, 3);
        setBilinearFiltering(0xAE, 1);
        
        if (gFarmRankingData.wifeAffection[arg0] == 0xFF) {
            startSpriteAnimation(0xAE, 6, 0xFE);
        } else {
            startSpriteAnimation(0xAE, 5, (gFarmRankingData.wifeAffection[arg0] * 9) / 255);
        }
        
    }

    if (gFarmRankingData.flags[arg0] & RANKING_FLAG_HAVE_BABY) {
        
        // baby icon
        dmaSprite(0x85, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x85, 255, 255, 255, 255);
        setSpriteRenderingLayer(0x85, 3);
        setBilinearFiltering(0x85, 1);
        startSpriteAnimation(0x85, 3, 5);
        
        // heart
        dmaSprite(0xAF, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAF, 0.0f, 16.0f, 8.0f);
        setSpriteColor(0xAF, 255, 255, 255, 255);
        setSpriteRenderingLayer(0xAF, 3);
        setBilinearFiltering(0xAF, 1);

        if (gFarmRankingData.babyAffection[arg0] == 0xFF) {
            startSpriteAnimation(0xAF, 6, 0xFE);
        } else {
            startSpriteAnimation(0xAF, 5, (gFarmRankingData.babyAffection[arg0] * 9) / 255);
        }
    
    }

    if (gFarmRankingData.flags[arg0] & RANKING_FLAG_HAVE_HORSE) {
        
        // horse icon
        dmaSprite(0x86, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x86, 255, 255, 255, 255);
        setSpriteRenderingLayer(0x86, 3);
        setBilinearFiltering(0x86, 1);
        startSpriteAnimation(0x86, 3, 6);
        
        // heart
        dmaSprite(0xB1, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB1, 0.0f, -16.0f, 8.0f);
        setSpriteColor(0xB1, 255, 255, 255, 255);
        setSpriteRenderingLayer(0xB1, 3);
        setBilinearFiltering(0xB1, 1);
        
        // hearts
        if (gFarmRankingData.horseAffection[arg0] == 0xFF) {
            // "MAX" heart
            startSpriteAnimation(0xB1, 6, 0xFE);
        } else {
            // get number of hearts
            startSpriteAnimation(0xB1, 5, (gFarmRankingData.horseAffection[arg0] * 9) / 255);
        }
        
    }
    
    dmaSprite(0xB0, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xB0, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0xB0, 255, 255, 255, 255);
    setSpriteRenderingLayer(0xB0, 3);
    setBilinearFiltering(0xB0, 1);
    
    if (gFarmRankingData.dogAffection[arg0] == 0xFF) {
        startSpriteAnimation(0xB0, 6, 0xFE);
    } else {
        startSpriteAnimation(0xB0, 5, (gFarmRankingData.dogAffection[arg0] * 9) / 255);
    }

    if (gFarmRankingData.flags[arg0] & 1) {
        dmaSprite(0xB2, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, -4.0f, 80.0f, 16.0f);
        setSpriteColor(0xB2, 255, 255, 255, 255);
        setSpriteRenderingLayer(0xB2, 3);
        setBilinearFiltering(0xB2, 1);
        startSpriteAnimation(0xB2, 7, gFarmRankingData.seasons[arg0] - 1);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7B34);

void func_800B7B34(void) {
    updateSpriteRGBA(0x80, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x82, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x83, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x84, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x85, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x86, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0x87, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xAE, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xAF, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xB0, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xB1, 255, 255, 255, 255, 24);
    updateSpriteRGBA(0xB2, 255, 255, 255, 255, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7CB4);

void func_800B7CB4(void) {
    updateSpriteRGBA(0x80, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x82, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x83, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x84, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x85, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x86, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0x87, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAE, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xAF, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xB0, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xB1, 0, 0, 0, 0, 24);
    updateSpriteRGBA(0xB2, 0, 0, 0, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7E28);

void func_800B7E28(void) {
    setSpriteColor(0x80, 255, 255, 255, 255);
    setSpriteColor(0x82, 255, 255, 255, 255);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteColor(0x84, 255, 255, 255, 255);
    setSpriteColor(0x85, 255, 255, 255, 255);
    setSpriteColor(0x86, 255, 255, 255, 255);
    setSpriteColor(0x87, 255, 255, 255, 255);
    setSpriteColor(0xAE, 255, 255, 255, 255);
    setSpriteColor(0xAF, 255, 255, 255, 255);
    setSpriteColor(0xB0, 255, 255, 255, 255);
    setSpriteColor(0xB1, 255, 255, 255, 255);
    setSpriteColor(0xB2, 255, 255, 255, 255);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7F6C);

void func_800B7F6C(u8 arg0) {

    if (arg0 != 0xFF) {
        setSpriteViewSpacePosition(CURSOR_HAND, farmRankingsCursorHandXPositions[arg0], farmRankingsCursorHandYPositions[arg0], 32.0f);
    }

    startSpriteAnimation(CURSOR_HAND, 3, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B7FC8);

void func_800B7FC8(u8 arg0, u8 arg1) {

    if (arg0 != 0xFF) {
        if (arg1) {
            setSpritePaletteIndex(0x83 + arg0, 1);
        } else {
            setSpritePaletteIndex(0x83 + arg0, 15);
        }
    }

}



static inline void initializeOverlayScreenTable(void) {

    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;

}


/* main loop load screen callbacks */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8018);
 
// farm ranking screen load callback
void func_800B8018(void) {
    
    func_800593EC();

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u32*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0, 0, 0);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0); 
     
    initializeOverlayScreenTable();
    
    setMainLoopCallbackFunctionIndex(ESTIMATE);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B815C);

// kitchen picture load main loop callback
void func_800B815C(void) {

    func_800593EC();

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u32*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);

    overlayScreenTable.cellIndex = func_800C2780(0, 0);
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B82AC);

// home extensions selection load callback
void func_800B82AC(void) {

    func_800593EC();
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u32*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);

    initializeOverlayScreenTable();
    
    setMainLoopCallbackFunctionIndex(HOME_EXTENSIONS_SELECT);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B83F0);

// calendar load main loop callback
void func_800B83F0(void) {

    func_800593EC();
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u32*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);

    initializeOverlayScreenTable();

    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;

    setMainLoopCallbackFunctionIndex(CALENDAR);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8554);

// recipe book load main loop callback
void func_800B8554(void) {
    
    func_800593EC();
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u32*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);

    initializeOverlayScreenTable();

    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;

    setMainLoopCallbackFunctionIndex(RECIPE_BOOK);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B86B8);

// album load callback
void func_800B86B8(void) {
    
    func_800593EC();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;
    
    setMainLoopCallbackFunctionIndex(ALBUM);
    
}



/* pause screen functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B881C);

// pause screen load callback
void func_800B881C(void) {
    func_800593EC();
    func_800B8844();
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8844);

void func_800B8844(void) {
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(PAUSE_SCREEN);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B8980);

void func_800B8980(void) {

    func_8005CDCC();
    // clock
    func_800B2F34();

    setMoneySprites();

    // tool/item slots
    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 0, 0);

    // tool/item slots
    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    // key item slots
    dmaSprite(0x85, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x85, 0x80, 0x80, 0x80, 0);
    setSpriteRenderingLayer(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 0);
    
    // tool and belongings labels
    dmaSprite(TOOL_AND_BELONGINGS_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(TOOL_AND_BELONGINGS_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(TOOL_AND_BELONGINGS_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(TOOL_AND_BELONGINGS_LABELS, 2);
    setBilinearFiltering(TOOL_AND_BELONGINGS_LABELS, TRUE);
    startSpriteAnimation(TOOL_AND_BELONGINGS_LABELS, 5, 0);
    
    // highlighted square borders
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, -28.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 3);
    
    dmaSprite(0x88, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x88, 20.0f, 80.0f, 8.0f);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x88, 2);
    setBilinearFiltering(0x88, 1);
    startSpriteAnimation(0x88, 2, 0);
    setSpritePaletteIndex(0x88, 2);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // selected tool/item
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    func_800C7850();
    func_800B90E0();
    func_800B99EC();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B90E0);

void func_800B90E0(void) {

    u8 i;
    u32 j, k;
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(TOOL_SPRITES_BASE + i);
    }

    for (i = 0; i < 8; i++) {
    
        f32* toolXCoords = pauseScreenToolXPositions;
        f32* toolYCoords = pauseScreenToolYPositions;
        j = i;

        if (gPlayer.toolSlots[i]) {
             
            dmaSprite(TOOL_SPRITES_BASE + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x8028FA00, (i * 0x600) + 0x8028FD00, (u16*)0x80299600, (u16*)0x8029BE00, (u8*)0x8029EE00, (u32*)0x8029F000, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(TOOL_SPRITES_BASE + i, toolXCoords[j % 4], *(f32*)((u8*)toolYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(TOOL_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(TOOL_SPRITES_BASE + i, 2);
            setBilinearFiltering(TOOL_SPRITES_BASE + i, 1);
            startSpriteAnimation(TOOL_SPRITES_BASE + i, func_80030BA0(toolsAnimationScripts, D_80116EB8[gPlayer.toolSlots[i]] + func_800C0688(gPlayer.toolSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.currentTool) {

        dmaSprite(TOOL_SPRITES_BASE + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x8028FA00, (i * 0x600) + 0x8028FD00, (u16*)0x80299600, (u16*)0x8029BE00, (u8*)0x8029EE00, (u32*)0x8029F000, 1, 0);
        setSpriteViewSpacePosition(TOOL_SPRITES_BASE + i, pauseScreenCurrentToolXPosition, pauseScreenCurrentToolYPosition, 256.0f);
        setSpriteColor(TOOL_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(TOOL_SPRITES_BASE + i, 2);
        setBilinearFiltering(TOOL_SPRITES_BASE + i, 1);
        
        startSpriteAnimation(TOOL_SPRITES_BASE + i, func_80030BA0(toolsAnimationScripts, D_80116EB8[gPlayer.currentTool] + func_800C0688(gPlayer.currentTool)), 0);

    }
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(HOLDABLE_ITEMS_BASE + i);
    }

    for (i = 0; i < 8; i++) {

        f32* belongingXCoords = pauseScreenBelongingsXPosition;
        f32* belongingYCoords = pauseScreenBelongingsYPosition;
        j = i;

        if (gPlayer.belongingsSlots[i]) {
            
            dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, belongingXCoords[j % 4], *(f32*)((u8*)belongingYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(HOLDABLE_ITEMS_BASE + i, 2);
            setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
            startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(gPlayer.belongingsSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.heldItem) {
        dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
        setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, pauseScreenHeldItemXPosition, pauseScreenHeldItemYPosition, 256.0f);
        setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(HOLDABLE_ITEMS_BASE + i, 2);
        setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
        startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(gPlayer.heldItem)), 0);
    }

    for (i = 0; i < 8; i++) {
        resetAnimationState(KEY_ITEMS_BASE + i);
    }

    for (i = 0; i < 8; i++) {

        k = i;
        
        if (gPlayer.keyItemSlots[overlayScreenTable.pageNumber * 8 + i]) {
            
            dmaSprite(KEY_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80296600, (i * 0x600) + 0x80296900, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
            setSpriteViewSpacePosition(KEY_ITEMS_BASE + i, pauseScreenKeyItemsXPosition [k & 1], pauseScreenKeyItemsYPosition[k >> 1], 256.0f);
            setSpriteColor(KEY_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(KEY_ITEMS_BASE + i, 2);
            setBilinearFiltering(KEY_ITEMS_BASE + i, 1);

            startSpriteAnimation(KEY_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, D_80116EF4[ gPlayer.keyItemSlots[overlayScreenTable.pageNumber * 8 + i]]), 0);
        
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9914);

// FIXME: fake matches
void func_800B9914(void) {
    
    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;

    if (*ptr == 7) {

        temp = overlayScreenTable.unk_3;

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                // show item/tool selected
                setSpriteViewSpacePosition(0x82, func_800BA928(temp2, temp), func_800BA9E8(*ptr, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
                return;
            } else {
                goto reset;
            }
            
        } 
        
        goto reset;
        
    }

    temp = overlayScreenTable.unk_3;
    
    if (overlayScreenTable.unk_3 != 0xFF) {
        // end animation by playing it one last time
        setSpriteViewSpacePosition(0x82, func_800BA928(temp2, temp), func_800BA9E8(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B99EC);

void func_800B99EC(void) {

    u8 i;
    
    fadeInClock();

    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);

    updateSpriteAlpha(TOOL_AND_BELONGINGS_LABELS, 0xFF, 24);
    updateSpriteAlpha(0x87, 0xFF, 24);
    updateSpriteAlpha(0x88, 0xFF, 24);

    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(TOOL_SPRITES_BASE + i, 0xFF, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(HOLDABLE_ITEMS_BASE + i, 0xFF, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.keyItemSlots[i]) {
            updateSpriteAlpha(KEY_ITEMS_BASE + i, 0xFF, 24);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9B8C);

void func_800B9B8C(void) {

    u8 i;
    
    fadeOutClock();

    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);

    updateSpriteAlpha(TOOL_AND_BELONGINGS_LABELS, 0, 24);

    updateSpriteAlpha(0x87, 0, 24);
    updateSpriteAlpha(0x88, 0, 24);
    
    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(TOOL_SPRITES_BASE + i, 0, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(HOLDABLE_ITEMS_BASE + i, 0, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.keyItemSlots[i]) {
            updateSpriteAlpha(KEY_ITEMS_BASE + i, 0, 24);
        }
    }
    
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800B9D3C);

// pause screen main loop callback
void func_800B9D3C(void) {

    bool set = FALSE;
    bool flag;

    u8 tempItem;
    u32 temp2;

    switch (overlayScreenTable.screenState) {

        case 0:
            func_800B8980();
            overlayScreenTable.screenState = 1;
            break;

        case 1:
            if (func_8002CBF8(TOOL_AND_BELONGINGS_LABELS)) {
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;

        case 2:
            if (func_8002CBF8(TOOL_AND_BELONGINGS_LABELS)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;    

        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = overlayScreenTable.unk_7;
            }
            break;

        case 5:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
            
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex == 8) {
                        
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        playSfx(2);
                        
                    } else {
                        
                        overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if (overlayScreenTable.cellIndex < 4) {
                            overlayScreenTable.cellIndex += 4;
                            playSfx(2);
                        } else if (overlayScreenTable.unk_3 == 0xFF) {
                            overlayScreenTable.cellIndex -= 4;
                            overlayScreenTable.screenState = 6;
                            playSfx(2);
                        }
                    
                    }
                    
                    set = TRUE;
                    
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if (overlayScreenTable.cellIndex >= 4) {
                            overlayScreenTable.cellIndex -= 4;
                            playSfx(2);
                        }
                    
                        set = TRUE;
                        
                    }
                        
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if ((overlayScreenTable.cellIndex != 3) && (overlayScreenTable.cellIndex != 7)) {
                            
                            overlayScreenTable.cellIndex++;
                            playSfx(2);
                            
                        } else if (overlayScreenTable.unk_3 == 0xFF) {
                            
                            if (overlayScreenTable.cellIndex == 3) {
                                overlayScreenTable.cellIndex = 0;
                            }
                            
                            if (overlayScreenTable.cellIndex == 7) {
                                overlayScreenTable.cellIndex = 2;
                            }
                        
                            overlayScreenTable.screenState = 7;
                            playSfx(2);
                        
                        }
                        
                        set = TRUE;
                        
                    }
                }
                
            }
            
            if ((checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST))) {

                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                    
                        overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if (overlayScreenTable.cellIndex && (overlayScreenTable.cellIndex != 4)) {
                            overlayScreenTable.cellIndex = overlayScreenTable.cellIndex - 1;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                    
                    }
                }
                
            }
            
            if (overlayScreenTable.pageNumber != 2) {
                func_800C7C88(96.0f, -104.0f);
            } else {
                func_800C7DF8();
            }
            
            if (overlayScreenTable.pageNumber) {
                func_800C7CE4(96.0f, 56.0f);
            } else {
                func_800C7E18();
            }
            
            break;

        case 6:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex == 8) {
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        playSfx(2);
                    } else if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                        playSfx(2);
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                    
                        if (overlayScreenTable.cellIndex >= 4) {
                            
                            overlayScreenTable.cellIndex -= 4;
                            playSfx(2);
                        
                        } else if (overlayScreenTable.unk_3 == 0xFF) {
                            overlayScreenTable.cellIndex += 4;
                            overlayScreenTable.screenState = 5;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if (overlayScreenTable.cellIndex != 3 && overlayScreenTable.cellIndex != 7) {
                            
                            overlayScreenTable.cellIndex++;
                            playSfx(2);
                            
                        } else if (overlayScreenTable.unk_3 == 0xFF) {
                            
                            if (overlayScreenTable.cellIndex == 3) {
                                overlayScreenTable.cellIndex = 4;
                            }
                            if (overlayScreenTable.cellIndex == 7) {
                                overlayScreenTable.cellIndex = 6;
                            }
                            
                            overlayScreenTable.screenState = 7;
                            playSfx(2);
                            
                        }
                        
                        set = TRUE;
                        
                    }
                    
                }
                
            }
            
            if ((checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST))) {

                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if (overlayScreenTable.cellIndex && overlayScreenTable.cellIndex != 4) {
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                    
                }
                
            }

            if (overlayScreenTable.pageNumber != 2) {
                func_800C7C88(96.0f, -104.0f);
            } else {
                func_800C7DF8();
            }
            
            if (overlayScreenTable.pageNumber) {
                func_800C7CE4(96.0f, 56.0f);
            } else {
                func_800C7E18();
            }
            
            break;

        case 7:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                
                    if (overlayScreenTable.cellIndex < 6) {
                        overlayScreenTable.cellIndex += 2;
                        playSfx(2);
                    } else if (overlayScreenTable.pageNumber != 2) {
                        overlayScreenTable.pageNumber++;
                        overlayScreenTable.cellIndex -= 6;
                        func_800B90E0();
                        func_800B9914();
                        playSfx(2);
                    }
                        
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex >= 2) {
                        overlayScreenTable.cellIndex -= 2;
                        playSfx(2);
                    } else if (overlayScreenTable.pageNumber) {
                        overlayScreenTable.pageNumber--;
                        overlayScreenTable.cellIndex += 6;
                        func_800B90E0();
                        func_800B9914();
                        playSfx(2);
                    }
                    
                    set = TRUE;
                    
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                
                    if (!(overlayScreenTable.cellIndex & 1)) {
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                    }
                    
                    set = TRUE;
                
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {

                if (!set) {

                    temp2 = overlayScreenTable.cellIndex;

                    if (temp2 & 1) {
                        
                        overlayScreenTable.cellIndex--;
                        playSfx(2);
                        
                    } else if (overlayScreenTable.unk_3 == 0xFF) {
                        
                        if (overlayScreenTable.cellIndex < 4) {
                            
                            if (overlayScreenTable.cellIndex == 0) {
                                overlayScreenTable.cellIndex = 3;
                            } else {
                                overlayScreenTable.cellIndex = 7;
                            }
                            
                            overlayScreenTable.screenState = 5;
                                
                        } else {
                            if (overlayScreenTable.cellIndex == 4) {
                                overlayScreenTable.cellIndex = 3;
                                overlayScreenTable.screenState = 6;
                            }  else {
                                overlayScreenTable.cellIndex = 7;
                                overlayScreenTable.screenState = 6;
                            }
                        }
                        
                        playSfx(2);
                        
                    }
                    
                    set = TRUE;

                }
                
            }
            
            if (overlayScreenTable.pageNumber != 2) {
                func_800C7C88(96.0f, -104.0f);
            } else {
                func_800C7DF8();
            }
            
            if (overlayScreenTable.pageNumber) {
                func_800C7CE4(96.0f, 56.0f);
            } else {
                func_800C7E18();
            }
            
            break;
        
        case 8:
            if (func_8002CBF8(TOOL_AND_BELONGINGS_LABELS)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 9;
            }
            break;
        
        // load village map (cut from NA release)
        case 9:
            deactivateSprite(0x80);
            setMainLoopCallbackFunctionIndex(PAUSE_SCREEN_MAP_LOAD_1);
            break;
        
    }

    if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 8) {

        if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
            
            if (!set) {
                
                if (overlayScreenTable.unk_3 != 0xFF) {
                    
                    overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                    overlayScreenTable.unk_3 = 0xFF;
                    overlayScreenTable.screenState = overlayScreenTable.unk_5;
                    overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                    func_800B9914();
                    
                } else {
                    func_800B9B8C();
                    resetAnimationState(CURSOR_HAND);
                    func_800C7DF8();
                    func_800C7E18();
                    deactivateSprite(0x82);
                    overlayScreenTable.screenState = 2;
                }
                
                playSfx(1);
                set = TRUE;

            }
            
        }

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            
            if (!set) {
                
                if (overlayScreenTable.unk_3 != 0xFF) {
                
                    tempItem = 0xFF;
                    
                    if (overlayScreenTable.screenState == 6) {
                        if (gPlayer.belongingsSlots[overlayScreenTable.cellIndex]) {
                            tempItem = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
                        }
                    }
                    
                    if ((tempItem == 0xFF) || (getItemFlags(tempItem) & 2)) {
                        
                        // swap item position
                        func_800BAC7C();
                        
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        overlayScreenTable.unk_3 = 0xFF;
                        overlayScreenTable.screenState = overlayScreenTable.unk_5;
                        overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
    
                        func_800B90E0();
                        func_800B9914();
                    
                    }
                    
                } else {
                    
                    tempItem = 0xFF;

                    if (overlayScreenTable.screenState == 6) {
                        if (gPlayer.belongingsSlots[overlayScreenTable.cellIndex]) {
                            tempItem = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
                        }
                    }
                    
                    if ((tempItem == 0xFF) || (getItemFlags(tempItem) & 2)) {

                        overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                        overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                        overlayScreenTable.unk_5 = overlayScreenTable.screenState; 
                        
                        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
                            overlayScreenTable.cellIndex = 8;
                        }
                        
                        func_800B9914();
                        
                    }
                    
                }
                
                playSfx(0);
                set = TRUE;
                
            }

        }
        
        if (checkButtonPressed(CONTROLLER_1, BUTTON_Z)) {
    
            if (!set) {

                tempItem = 0;
                
                switch (overlayScreenTable.screenState) {

                    case 5:
                        tempItem = gPlayer.toolSlots[overlayScreenTable.cellIndex];
                        flag = 0;
                        break;
                    case 6:
                        tempItem = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
                        flag = 1;
                        break;
                    case 7:
                        tempItem = gPlayer.keyItemSlots[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
                        flag = 2;
                        break;
                    
                }

                if (tempItem) {
                    
                    func_800C81E8(flag, tempItem);

                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                    
                }

            }
            
        }
        
        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 8) {
            setSpriteViewSpacePosition(CURSOR_HAND, (func_800BA928(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f), func_800BA9E8(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
            func_800BAAA0();
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BA928);

f32 func_800BA928(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:

            arg1 &= 0xFF;

            if (arg1 < 8) {
                res = pauseScreenToolXPositions[arg1 & 0x3];
            } else {
                res = pauseScreenCurrentToolXPosition;
            }
            break;
        
        case 6:

            arg1 &= 0xFF;

            if (arg1 < 8) {
                res = pauseScreenBelongingsXPosition[arg1 & 0x3];
            } else {
                res = pauseScreenHeldItemXPosition;
            }
            
            break;

        case 7:
            res = pauseScreenKeyItemsXPosition[arg1 & 1];
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BA9E8);

f32 func_800BA9E8(u8 arg0, u8 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            
            if (arg1 < 8) {
                res = pauseScreenToolYPositions[arg1 / 4];
            } else {
                res = pauseScreenCurrentToolYPosition;
            }
            break;
        
        case 6:

            if (arg1 < 8) {
                res = pauseScreenBelongingsYPosition[arg1 / 4];
            } else {
                res = pauseScreenHeldItemYPosition;
            }
            
            break;

        case 7:
            res = pauseScreenKeyItemsYPosition[(arg1 / 2) & LEFT_ARROW];
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAAA0);

void func_800BAAA0(void) {

    u8 i;
    
    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x85, 0x80, 0x80, 0x80, 0xFF);

    switch (overlayScreenTable.screenState) {

        case 5:
            setSpriteColor(0x83, 255, 255, 255, 255);
            break;
        case 6:
            setSpriteColor(0x84, 255, 255, 255, 255);
            break;
        case 7:
            setSpriteColor(0x85, 255, 255, 255, 255);
            break;
    }

    for (i = 0; i < 9; i++) {
         if (gPlayer.toolSlots[i]) {
             setSpriteColor(TOOL_SPRITES_BASE + i, 255, 255, 255, 255);
         }
    }

    for (i = 0; i < 9; i++) {
         if (gPlayer.belongingsSlots[i]) {
             setSpriteColor(HOLDABLE_ITEMS_BASE + i, 255, 255, 255, 255);
         }
    }

    for (i = 0; i < 9; i++) {
         if (gPlayer.keyItemSlots[(overlayScreenTable.pageNumber * 8) + i]) {
             setSpriteColor(KEY_ITEMS_BASE + i, 255, 255, 255, 255);
         }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAC7C);

// swap item/tool positions
void func_800BAC7C(void) {
    
    u8 temp;

    switch (overlayScreenTable.screenState) {          
        
        case TOOL_SLOTS:
            temp = gPlayer.toolSlots[overlayScreenTable.unk_3];
            gPlayer.toolSlots[overlayScreenTable.unk_3] = gPlayer.toolSlots[overlayScreenTable.cellIndex];
            gPlayer.toolSlots[overlayScreenTable.cellIndex] = temp;
            break;
        
        case BELONGINGS:
            temp = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
            gPlayer.belongingsSlots[overlayScreenTable.unk_3] = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = temp;
            break;
        
        case KEY_ITEMS:
            temp = gPlayer.keyItemSlots[overlayScreenTable.unk_4* 8 + overlayScreenTable.unk_3];
            gPlayer.keyItemSlots[overlayScreenTable.unk_4*8 + overlayScreenTable.unk_3] = gPlayer.keyItemSlots[overlayScreenTable.pageNumber*8 + overlayScreenTable.cellIndex];
            gPlayer.keyItemSlots[overlayScreenTable.pageNumber*8 + overlayScreenTable.cellIndex] = temp;
            break;

        }
    
}


/* end pause screen functions */


/* toolbox functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BADD0);

// toolbox load callback
void func_800BADD0(void) {
    
    func_800593EC();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, 
        (u32)&_checkerboardBackgroundTextureSegmentRomStart,
        (u32)&_checkerboardBackgroundTextureSegmentRomEnd, 
        (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, 
        (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, 
        NULL, 
        NULL, 
        (u8*)0x802DE5C0, 
        NULL, 
        (u16*)0x802E0BC0,
        (AnimationFrameMetadata*)0x802E0CC0, 
        (u32*)0x802E0DC0, 
        NULL, 
        0, 
        FALSE
    );
    
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.unk_4 = 0;
    overlayScreenTable.screenState = 0;
        
    setMainLoopCallbackFunctionIndex(TOOLBOX);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BAF1C);

void func_800BAF1C(void) {

    // clock
    func_800B2F34();
    
    setMoneySprites();
    
    // tool and item slots

    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 0, 0);

    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    // toolbox and rucksack labels
    dmaSprite(TOOL_AND_RUCKSACK_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(TOOL_AND_RUCKSACK_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(TOOL_AND_RUCKSACK_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(TOOL_AND_RUCKSACK_LABELS, 2);
    setBilinearFiltering(TOOL_AND_RUCKSACK_LABELS, 1);
    startSpriteAnimation(TOOL_AND_RUCKSACK_LABELS, 5, 1);

    // selected square
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 3);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CURSOR_HAND, func_800BC700(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, func_800BC770(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    // up and down arrows
    func_800C7850();

    // load tools
    func_800BB590();
    // fade in tools
    func_800BBBC8();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BB590);

void func_800BB590(void) {

    u8 i;
    u32 j;
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(TOOL_SPRITES_BASE + i);
    }

    for (i = 0; i < 8; i++) {
    
        j = i;

        if (gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]) {
             
            dmaSprite(TOOL_SPRITES_BASE + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (u8*)((i * 0x600) + 0x8028FA00), (u8*)((i * 0x600) + 0x8028FD00), (u16*)0x80299600, (u16*)0x8029BE00, (u8*)0x8029EE00, (u32*)0x8029F000, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(TOOL_SPRITES_BASE + i, toolboxToolSlotsXPosition[j % 4], *(f32*)((u8*)toolboxToolSlotsYPosition + (j & 0xFC)), 256.0f);

            setSpriteColor(TOOL_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(TOOL_SPRITES_BASE + i, 2);
            setBilinearFiltering(TOOL_SPRITES_BASE + i, 1);
            
            startSpriteAnimation(TOOL_SPRITES_BASE + i, func_80030BA0(toolsAnimationScripts,  D_80116EB8[gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]] + func_800C0688(gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i])), 0);
            
        }
        
    }

    for (i = 0; i < 9; i++) {
        resetAnimationState(0xAC + i);
    }

    for (i = 0; i < 8; i++) {
    
        f32* toolXCoords = toolboxPlayerToolSlotsXPosition;
        f32* toolYCoords = toolboxPlayerToolSlotsYPosition;
        j = i;

        if (gPlayer.toolSlots[i]) {
             
            dmaSprite(0xAC + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(0xAC + i, toolXCoords[j % 4], *(f32*)((u8*)toolYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(0xAC + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(0xAC + i, 2);
            setBilinearFiltering(0xAC + i, 1);
            startSpriteAnimation(0xAC + i, func_80030BA0(toolsAnimationScripts, D_80116EB8[gPlayer.toolSlots[i]] + func_800C0688(gPlayer.toolSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.currentTool) {

        dmaSprite(0xAC + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
        setSpriteViewSpacePosition(0xAC + i, toolboxCurrentToolXPosition, toolboxCurrentToolYPosition, 256.0f);
        setSpriteColor(0xAC + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0xAC + i, 2);
        setBilinearFiltering(0xAC + i, 1);

        startSpriteAnimation(0xAC + i, func_80030BA0(toolsAnimationScripts, D_80116EB8[gPlayer.currentTool] + func_800C0688(gPlayer.currentTool)), 0);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBAF0);

void func_800BBAF0(void) {

    f32 x;
    
    if (overlayScreenTable.unk_5 == 5) {

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                
                x = func_800BC700(overlayScreenTable.unk_5, overlayScreenTable.unk_3);
                setSpriteViewSpacePosition(0x82, x, func_800BC770(overlayScreenTable.unk_5, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
            
            } else {
                resetAnimationState(0x82);
            }
            
        } else {
            resetAnimationState(0x82);
        }
        
    } else {

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            x = func_800BC700(overlayScreenTable.unk_5, overlayScreenTable.unk_3);
            setSpriteViewSpacePosition(0x82, x, func_800BC770(overlayScreenTable.unk_5, overlayScreenTable.unk_3), 16.0f);
            startSpriteAnimation(0x82, 6, 0xFE);
            
        } else {
            resetAnimationState(0x82);
        }
        
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBBC8);

void func_800BBBC8(void) {
    
    u8 i;

    fadeInClock();

    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);

    updateSpriteAlpha(TOOL_AND_RUCKSACK_LABELS, 0xFF, 24);

    updateSpriteAlpha(0x87, 0xFF, 24);
    updateSpriteAlpha(0x88, 0xFF, 24);

    for (i = 0; i < 9; i++) {
        if (gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(TOOL_SPRITES_BASE + i, 0xFF, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(0xAC + i, 0xFF, 24);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBD34);

void func_800BBD34(void) {

    u8 i;

    fadeOutClock();

    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);

    updateSpriteAlpha(TOOL_AND_RUCKSACK_LABELS, 0, 24);

    updateSpriteAlpha(0x87, 0, 24);
    updateSpriteAlpha(0x88, 0, 24);

    for (i = 0; i < 9; i++) {
        if (gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(TOOL_SPRITES_BASE + i, 0, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.toolSlots[i]) {
            updateSpriteAlpha(0xAC + i, 0, 24);
        }
    }

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BBEC0);

// toolbox callback
void func_800BBEC0(void) {

    bool set = FALSE;

    u8 tempTool;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            func_800BAF1C();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (func_8002CBF8(TOOL_AND_RUCKSACK_LABELS)) {
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 6;
            }
            break;
        
        case 2:
            if (func_8002CBF8(TOOL_AND_RUCKSACK_LABELS)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059300();
            func_80059368();
            break;
        
        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = overlayScreenTable.unk_7;
            }
            break;

        case 5:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                        playSfx(2);
                    }

                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if (overlayScreenTable.cellIndex >= 4) {
                        overlayScreenTable.cellIndex -= 4;
                    } else {
                        overlayScreenTable.cellIndex += 4;
                        overlayScreenTable.screenState = 6;
                    }
                
                    playSfx(2);
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if ((overlayScreenTable.cellIndex != 3) && (overlayScreenTable.cellIndex != 7)) {
                        
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                        
                    } else if (overlayScreenTable.pageNumber != 3) {
                        
                        if (overlayScreenTable.cellIndex == 3) {
                            overlayScreenTable.cellIndex = 0;
                        } else {
                            overlayScreenTable.cellIndex = 4;
                        }
                        
                        overlayScreenTable.pageNumber++;
                        
                        func_800BB590();
                        func_800BBAF0();
                        
                        playSfx(2);
                        
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {

                if (!set) {

                    if (overlayScreenTable.cellIndex != 8) {

                       overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if (overlayScreenTable.cellIndex && overlayScreenTable.cellIndex != 4) {
                            
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                            
                        } else if (overlayScreenTable.pageNumber) {

                            if (overlayScreenTable.cellIndex == 0) {
                                overlayScreenTable.cellIndex = 3;
                            } else {
                                overlayScreenTable.cellIndex = 7;
                            }

                            overlayScreenTable.pageNumber--;
                            func_800BB590();
                            func_800BBAF0();

                            playSfx(2);
                            
                        }

                        set = TRUE;
                        
                    }
                
                }
                    
            }

            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-96.0f, -64.0f);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(96.0f, -64.0f);
            } else {
                func_800C7E58();
            }
    
            break;

        case 6:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex == 8) {
                        overlayScreenTable.cellIndex = 1;
                    } else if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                    } else {
                        overlayScreenTable.cellIndex -= 4;
                        overlayScreenTable.screenState = 5;
                    }
                    
                    playSfx(2);
                    set = TRUE;
                
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if (overlayScreenTable.cellIndex >= 4) {
                            overlayScreenTable.cellIndex -= 4;
                        } else {
                            overlayScreenTable.cellIndex = 8;
                        }
                        
                        playSfx(2);
                        set = TRUE;
                    
                    }

                }
                
            }
    
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                    
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if ((overlayScreenTable.cellIndex != 3) && (overlayScreenTable.cellIndex != 7)) {
                            overlayScreenTable.cellIndex++;
                            playSfx(2);
                        }
                    
                        set = TRUE;
                    
                    }
                    
                }
            }
    
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
    
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if (overlayScreenTable.cellIndex && overlayScreenTable.cellIndex != 4) {
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                    
                }
                        
            }
 
            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-96.0f, -64.0f);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(96.0f, -64.0f);
            } else {
                func_800C7E58();
            }
            
        break;
    
    }

    if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
                
        if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
        
            if (!set) {
                
                if (overlayScreenTable.unk_3 != 0xFF) {
                    
                    overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                    overlayScreenTable.unk_3 = 0xFF;
                    overlayScreenTable.screenState = overlayScreenTable.unk_5;
                    overlayScreenTable.pageNumber = overlayScreenTable.unk_4;

                    func_800BB590();
                    func_800BBAF0();
                    
                } else {
                    func_800BBD34();
                    resetAnimationState(CURSOR_HAND);
                    func_800C7E38();
                    func_800C7E58();
                    deactivateSprite(0x82);
                    overlayScreenTable.screenState = 2;
                }
    
                playSfx(1);
                set = TRUE;
            }
            
        }
    
        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            
            if (!set) {
                
                if (overlayScreenTable.unk_3 != 0xFF) {
                    
                    func_800BC918();
                    
                    overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                    overlayScreenTable.unk_3 = 0xFF;
                    overlayScreenTable.screenState =overlayScreenTable.unk_5;
                    overlayScreenTable.pageNumber = overlayScreenTable.unk_4;

                    func_800BB590();
                    
                } else {
                    overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                    overlayScreenTable.unk_5 = overlayScreenTable.screenState;
                    overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                }
                
                func_800BBAF0();
                
                playSfx(0);
                set = TRUE;
            
            }
            
        }
        
        if (checkButtonPressed(CONTROLLER_1, BUTTON_Z)) {

            if (!set) {

                tempTool = 0;
                
                if (overlayScreenTable.screenState != 5) {
                    if (overlayScreenTable.screenState == 6) {
                        tempTool = gPlayer.toolSlots[overlayScreenTable.cellIndex];
                    }
                } else {
                    tempTool = gToolboxSlots[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
                }
                
                if (tempTool) {
                    
                    func_800C81E8(0, tempTool);
                    
                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                    
                }
                
            }
            
        }
    
        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
            setSpriteViewSpacePosition(CURSOR_HAND, func_800BC700(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, func_800BC770(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
            func_800BC7D8();
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC700);

f32 func_800BC700(u8 arg0, u32 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            res = toolboxToolSlotsXPosition[arg1 & 0x3];
            break;
        
        case 6:

            arg1 &= 0xFF;
            if (arg1 < 8) {
                res = toolboxPlayerToolSlotsXPosition[arg1 & 0x3];
            } else {
                res = toolboxCurrentToolXPosition;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC770);

f32 func_800BC770(u8 arg0, u8 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            res = toolboxToolSlotsYPosition[arg1 / 4];
            break;
        
        case 6:

            if (arg1 < 8) {
                res = toolboxPlayerToolSlotsYPosition[arg1 / 4];
            } else {
                res = toolboxCurrentToolYPosition;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC7D8);

void func_800BC7D8(void) {

    u8 i;

    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteColor(0x84, 255, 255, 255, 255);
    
    for (i = 0; i < 9; i++) {

        if (gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]) {
            setSpriteColor(TOOL_SPRITES_BASE + i, 255, 255, 255, 255);
        }
        
    }

    for (i = 0; i < 9; i++) {

        if (gPlayer.toolSlots[i]) {
            setSpriteColor(0xAC + i, 255, 255, 255, 255);
        }
        
    }
    
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BC918);

// swap tools
void func_800BC918(void) {

    u8 temp;
    
    switch (overlayScreenTable.screenState) {

        case 5:
            
            temp = gToolboxSlots[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.screenState) {
                
                gToolboxSlots[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = 
                    gToolboxSlots[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                
                gToolboxSlots[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
                
            } else {
                gToolboxSlots[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = gPlayer.toolSlots[overlayScreenTable.unk_3];
                gPlayer.toolSlots[overlayScreenTable.unk_3] = temp;
            }

            break;

        case 6:

            temp = gPlayer.toolSlots[overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.screenState) {
                
                gPlayer.toolSlots[overlayScreenTable.cellIndex] = gPlayer.toolSlots[overlayScreenTable.unk_3];
                gPlayer.toolSlots[overlayScreenTable.unk_3] = temp;
                
            } else {
                gPlayer.toolSlots[overlayScreenTable.cellIndex] = gToolboxSlots[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                gToolboxSlots[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
            }
            
            break;
        
    }
    
}

/* end toolbox funcs */


/* freezer */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BCA9C);

// freezer load callback
void func_800BCA9C(void) {
    
    func_800593EC();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.unk_4 = 0;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(FREEZER);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BCBE8);

void func_800BCBE8(void) {

    // clock
    func_800B2F34();
    
    setMoneySprites();

    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, 
              (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, 
              (u8*)0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, FALSE);

    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, TRUE);
    startSpriteAnimation(0x83, 0, 0);

    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    dmaSprite(FREEZER_AND_RUCKSACK_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(FREEZER_AND_RUCKSACK_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(FREEZER_AND_RUCKSACK_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(FREEZER_AND_RUCKSACK_LABELS, 2);
    setBilinearFiltering(FREEZER_AND_RUCKSACK_LABELS, 1);
    startSpriteAnimation(FREEZER_AND_RUCKSACK_LABELS, 5, 2);
    
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 2);
    
    dmaSprite(CURSOR_HAND, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )0x8023B400, NULL, (u16* )0x8023CC00, (u16* )0x8023CE00, (u8* )0x8023D200, NULL, 0, 0);
    setSpriteViewSpacePosition(CURSOR_HAND, func_800BE3B8(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, func_800BE428(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd,  (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    func_800C7850();
    func_800BD25C();
    func_800BD82C();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD25C);

void func_800BD25C(void) {

    u8 i;
    u32 j, k;
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(0xA3 + i);
    }

    for (i = 0; i < 8; i++) {

        j = i;
        
        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            
            f32* xCoords = D_80116D10;
            f32* yCoords = D_80116D20;
             
            dmaSprite(0xA3 + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (((i * 2) + i) << 9) + 0x8028FA00, (((i * 2) + i) << 9) + 0x8028FD00, (u16* )0x80299600, (u16* )0x8029BE00, (u8* )0x8029EE00, (u32* )0x8029F000, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(0xA3 + i, xCoords[j % 4], *(f32*)((u8*)yCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(0xA3 + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(0xA3 + i, 2);
            setBilinearFiltering(0xA3 + i, 1);
            startSpriteAnimation(0xA3 + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(D_80237420[(overlayScreenTable.pageNumber * 8) + i])), 0);
        
        }
        
    }

    for (i = 0; i < 9; i++) {
        resetAnimationState(HOLDABLE_ITEMS_BASE + i);
    }

    for (i = 0; i < 8; i++) {

        f32* belongingXCoords = D_80116D28;
        f32* belongingYCoords = D_80116D3C;
        j = i;

        if (gPlayer.belongingsSlots[i]) {
            
            dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
            setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, belongingXCoords[j % 4], *(f32*)((u8*)belongingYCoords + (j & 0xFC)), 256.0f);
            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(HOLDABLE_ITEMS_BASE + i, 2);
            setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
            startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(gPlayer.belongingsSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.heldItem) {
        dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
        setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, D_80116D38, D_80116D44, 256.0f);
        setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(HOLDABLE_ITEMS_BASE + i, 2);
        setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
        startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(gPlayer.heldItem)), 0);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD754);

void func_800BD754(void) {

    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;
    if (*ptr == 5) {

        temp = overlayScreenTable.unk_3;
        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, func_800BE3B8(temp2, temp), func_800BE428(*ptr, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
                return;
            } else {
                goto reset;
            }
            
        } 
        
        goto reset;
        
    }

    temp = overlayScreenTable.unk_3;
    
    if (overlayScreenTable.unk_3 != 0xFF) {
startAnim:
        setSpriteViewSpacePosition(0x82, func_800BE3B8(temp2, temp), func_800BE428(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD82C);

void func_800BD82C(void) {
    
    u8 i;
    
    fadeInClock();
    
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    
    updateSpriteAlpha(FREEZER_AND_RUCKSACK_LABELS, 0xFF, 24);
    
    updateSpriteAlpha(0x87, 0xFF, 24);
    updateSpriteAlpha(0x88, 0xFF, 24);
    
    for (i = 0; i < 9; i++) {
        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(0xA3 + i, 0xFF, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(HOLDABLE_ITEMS_BASE + i, 0xFF, 24);
        }
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BD998);

void func_800BD998(void) {

    u8 i;

    fadeOutClock();
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(FREEZER_AND_RUCKSACK_LABELS, 0, 24);
    updateSpriteAlpha(0x87, 0, 24);
    updateSpriteAlpha(0x88, 0, 24);

    for (i = 0; i < 9; i++) {
        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(0xA3 + i, 0, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(HOLDABLE_ITEMS_BASE + i, 0, 24);
        }
    }

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BDB24);

// freezer callback function
void func_800BDB24(void) {

    bool set = FALSE;

    u8 temp;
    u8 type;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            func_800BCBE8();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (func_8002CBF8(FREEZER_AND_RUCKSACK_LABELS)) {
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
        
        case 2:
            if (func_8002CBF8(FREEZER_AND_RUCKSACK_LABELS)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059300();
            func_80059368();
            break;
        
        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = overlayScreenTable.unk_7;
            }
            break;

        case 5:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                    } else {
                        overlayScreenTable.cellIndex -= 4;
                        overlayScreenTable.screenState = 6;
                    }

                    playSfx(2);
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if (overlayScreenTable.cellIndex >= 4) {
                        overlayScreenTable.cellIndex -= 4;
                    } else {
                        overlayScreenTable.cellIndex = 8;
                        overlayScreenTable.screenState = 6;
                    }
                
                    playSfx(2);
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if ((overlayScreenTable.cellIndex != 3) && (overlayScreenTable.cellIndex != 7)) {
                        
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                        
                    } else if (overlayScreenTable.pageNumber != 3) {
                        
                        if (overlayScreenTable.cellIndex == 3) {
                            overlayScreenTable.cellIndex = 0;
                        } else {
                            overlayScreenTable.cellIndex = 4;
                        }
                        
                        overlayScreenTable.pageNumber++;
                        
                        func_800BD25C();
                        func_800BD754();
                        
                        playSfx(2);
                        
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {

                if (!set) {

                    if (overlayScreenTable.cellIndex != 8) {

                       overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if (overlayScreenTable.cellIndex && overlayScreenTable.cellIndex != 4) {
                            
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                            
                        } else if (overlayScreenTable.pageNumber) {

                            if (overlayScreenTable.cellIndex == 0) {
                                overlayScreenTable.cellIndex = 3;
                            } else {
                                overlayScreenTable.cellIndex = 7;
                            }

                            overlayScreenTable.pageNumber--;
                            
                            func_800BD25C();
                            func_800BD754();

                            playSfx(2);
                            
                        }

                        set = TRUE;
                        
                    }
                
                }
                    
            }

            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-96.0f, 16.0f);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(96.0f, 16.0f);
            } else {
                func_800C7E58();
            }
    
            break;

        case 6:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex == 8) {
                        overlayScreenTable.cellIndex = 1;
                        overlayScreenTable.screenState = 5;
                    } else if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                    }
                    
                    playSfx(2);
                    set = TRUE;
                
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if (overlayScreenTable.cellIndex >= 4) {
                            overlayScreenTable.cellIndex -= 4;
                        } else {
                            overlayScreenTable.cellIndex += 4;
                            overlayScreenTable.screenState = 5;
                        }
                        
                        playSfx(2);
                        set = TRUE;
                    
                    }
                }
                
            }
    
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                    
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if ((overlayScreenTable.cellIndex != 3) && (overlayScreenTable.cellIndex != 7)) {
                            overlayScreenTable.cellIndex++;
                            playSfx(2);
                        }
                    
                        set = TRUE;
                    
                    }
                    
                }
            }
    
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
    
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {
                        
                        if (overlayScreenTable.cellIndex && overlayScreenTable.cellIndex != 4) {
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                    
                }
                        
            }
 
            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-96.0f, 16.0f);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(96.0f, 16.0f);
            } else {
                func_800C7E58();
            }
        
        break;
    
    }

    if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
                
        if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
        
            if (!set) {
                
                if (overlayScreenTable.unk_3 != 0xFF) {
                    overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                    overlayScreenTable.unk_3 = 0xFF;
                    overlayScreenTable.screenState = overlayScreenTable.unk_5;
                    overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                    func_800BD25C();
                    func_800BD754();
                } else {
                    func_800BD998();
                    resetAnimationState(CURSOR_HAND);
                    func_800C7E38();
                    func_800C7E58();
                    deactivateSprite(0x82);
                    overlayScreenTable.screenState = 2;
                }
    
                playSfx(1);
                set = TRUE;
                
            }
            
        }
    
        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            
            if (!set) {

                if (overlayScreenTable.unk_3 != 0xFF) {
                    
                    if (func_800BE5D0()) {
                        func_800BE684();
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        overlayScreenTable.unk_3 = 0xFF;
                        overlayScreenTable.screenState = overlayScreenTable.unk_5;
                        overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                        func_800BD25C();
                        func_800BD754();
                        playSfx(0);
                    } else {
                        playSfx(3);
                    }
                    
                } else {

                    if (func_800BE5D0()) {
                        overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                        overlayScreenTable.unk_5 = overlayScreenTable.screenState;
                        overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                        func_800BD754();
                        playSfx(0);
                    } else {
                        playSfx(3);
                    }
                    
                    
                }
    
                set = TRUE;
            
            }
            
        }
        
        if (checkButtonPressed(CONTROLLER_1, BUTTON_Z)) {
            
            if (!set) {

                temp = 0;
                type = 1;
                
                switch (overlayScreenTable.screenState) {

                    case 6:
                        temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
                        break;
                    
                    case 5:
                        temp = D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
                        break;
                    
                }

                if (temp) {
                    func_800C81E8(type, temp);
                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                }
                 
            }
            
        }
    
        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
            setSpriteViewSpacePosition(CURSOR_HAND, func_800BE3B8(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, func_800BE428(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
            func_800BE490();
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE3B8);

inline f32 func_800BE3B8(u8 arg0, u8 arg1) {

    f32 res;
    
    switch (arg0) {
        
        case 5:
            res = D_80116D10[arg1 % 4];
            break;
        
        case 6:
            
            if (arg1 < 8) {
                res = D_80116D28[arg1 % 4];
            } else {
                res = D_80116D38;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE428);

inline f32 func_800BE428(u8 arg0, u8 arg1) {

    f32 res;
    
    switch (arg0) {

        case 5:
            res = D_80116D20[arg1 / 4];
            break;
        
        case 6:

            if (arg1 < 8) {
                res = D_80116D3C[arg1 / 4];
            } else {
                res = D_80116D44;
            }
            
            break;
        
    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE490);

void func_800BE490(void) {

    u8 i;

    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteColor(0x84, 255, 255, 255, 255);

    for (i = 0; i < 9; i++) {

        if (D_80237420[(overlayScreenTable.pageNumber * 8) + i]) {
            setSpriteColor(0xA3 + i, 255, 255, 255, 255);
        }
        
    }

    for (i = 0; i < 9; i++) {
        
        if (gPlayer.belongingsSlots[i] != 0) {
            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 255, 255, 255, 255);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE5D0);

bool func_800BE5D0(void) {
    
    bool result = FALSE;
    u8 temp;
    
    switch (overlayScreenTable.screenState) {

        case 5:
            temp = D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            break;
        case 6:
            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            break;
        
    }

    if (getItemFlags(temp) & 0x200) {
        result = TRUE;
    }

    if (temp == 0) {
        result = TRUE;
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE684);

void func_800BE684(void) {

    u8 temp;

    switch (overlayScreenTable.screenState) {

        case 5:

            temp = D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.screenState) {
                D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
            } else {
                D_80237420[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
                gPlayer.belongingsSlots[overlayScreenTable.unk_3] = temp;
            }

            break;

        case 6:

            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.screenState) {
                gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
                gPlayer.belongingsSlots[overlayScreenTable.unk_3] = temp;
            } else {
                gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                D_80237420[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
            }

        break;
        
    }
    
}

/* end of freezer functions */


/* cabinet functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE808);

// cabinet load callback
void func_800BE808(void) {
    
    func_800593EC();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.unk_4 = 0;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(CABINET);
    
}

// INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BE954);

void func_800BE954(void) {

    // clock
    func_800B2F34();

    setMoneySprites();
    
    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, TRUE);
    startSpriteAnimation(0x83, 0, 0);
    
    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, TRUE);
    startSpriteAnimation(0x84, 0, 0);

    dmaSprite(CABINET_AND_RUCKSACK_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CABINET_AND_RUCKSACK_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(CABINET_AND_RUCKSACK_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(CABINET_AND_RUCKSACK_LABELS, 2);
    setBilinearFiltering(CABINET_AND_RUCKSACK_LABELS, 1);
    startSpriteAnimation(CABINET_AND_RUCKSACK_LABELS, 5, 3);
    
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, TRUE);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 2);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CURSOR_HAND, func_800BE3B8(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, func_800BE428(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, 
              (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 
              NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, TRUE);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    func_800C7850();
    func_800BF0C8();
    func_800BF698();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF0C8);

// cabinet
void func_800BF0C8(void) {

    u8 i;
    u32 j, k;
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(0xA3 + i);
    }

    for (i = 0; i < 8; i++) {

        j = i;
        
        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            
            f32* xCoords = D_80116D48;
            f32* yCoords = D_80116D58;
             
            dmaSprite(0xA3 + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (((i * 2) + i) << 9) + 0x8028FA00, (((i * 2) + i) << 9) + 0x8028FD00, (u16* )0x80299600, (u16* )0x8029BE00, (u8* )0x8029EE00, (u32* )0x8029F000, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(0xA3 + i, xCoords[j % 4], *(f32*)((u8*)yCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(0xA3 + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(0xA3 + i, 2);
            setBilinearFiltering(0xA3 + i, 1);
            startSpriteAnimation(0xA3 + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(D_801890E8[(overlayScreenTable.pageNumber * 8) + i])), 0);
        
        }
        
    }

    for (i = 0; i < 9; i++) {
        resetAnimationState(HOLDABLE_ITEMS_BASE + i);
    }

    for (i = 0; i < 8; i++) {

        f32* belongingXCoords = D_80116D60;
        f32* belongingYCoords = D_80116D74;
        j = i;

        if (gPlayer.belongingsSlots[i]) {
            
            dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, belongingXCoords[j % 4], *(f32*)((u8*)belongingYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(HOLDABLE_ITEMS_BASE + i, 2);
            setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
            startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(gPlayer.belongingsSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.heldItem) {
        dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * 0x600) + 0x80293000, (i * 0x600) + 0x80293300, (u16*)0x8029F600, (u16*)0x802A1E00, (u8*)0x802A5300, (u32*)0x802A5500, 1, 0);
        setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, D_80116D70, D_80116D7C, 256.0f);
        setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(HOLDABLE_ITEMS_BASE + i, 2);
        setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
        startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, func_80030BA0(shopItemsAnimationScripts, func_800D5A88(gPlayer.heldItem)), 0);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF5C0);

void func_800BF5C0(void) {

    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;

    if (*ptr == 5) {

        temp = overlayScreenTable.unk_3;

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, func_800C0238(temp2, temp), func_800C02A8(*ptr, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
                return;
            } else {
                goto reset;
            }
            
        } 
        
        goto reset;
        
    }

    temp = overlayScreenTable.unk_3;
    
    if (overlayScreenTable.unk_3 != 0xFF) {
startAnim:
        setSpriteViewSpacePosition(0x82, func_800C0238(temp2, temp), func_800C02A8(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF698);

void func_800BF698(void) {
    
    u8 i;

    fadeInClock();

    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);

    updateSpriteAlpha(CABINET_AND_RUCKSACK_LABELS, 0xFF, 24);
    
    updateSpriteAlpha(0x87, 0xFF, 24);
    updateSpriteAlpha(0x88, 0xFF, 24);

    for (i = 0; i < 9; i++) {
        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(0xA3 + i, 0xFF, 24);
        }
    }

    for (i = 0; i < 9; i++) {
        if (gPlayer.belongingsSlots[i]) {
            updateSpriteAlpha(HOLDABLE_ITEMS_BASE + i, 0xFF, 24);
        }
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF804);

void func_800BF804(void) {

    u8 i;

    fadeOutClock();

    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(CABINET_AND_RUCKSACK_LABELS, 0, 24);
    updateSpriteAlpha(0x87, 0, 24);
    updateSpriteAlpha(0x88, 0, 24);

    for (i = 0; i < 9; i++) {

        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            updateSpriteAlpha(0xA3 + i, 0, 24);
        }
        
    }
    
    for (i = 0; i < 9; i++) {
        
        if (gPlayer.belongingsSlots[i] != 0) {
            updateSpriteAlpha(HOLDABLE_ITEMS_BASE + i, 0, 24);
        }
        
    }

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800BF990);

// cabinet screen main loop callback
void func_800BF990(void) {

    bool set = FALSE;
    
    u8 temp;
    u8 type;

    switch (overlayScreenTable.screenState) {

        case 0:
            func_800BE954();
            overlayScreenTable.screenState = 1;
            break;
            
        case 1:
            if (func_8002CBF8(CABINET_AND_RUCKSACK_LABELS)) {
                startSpriteAnimation(0x79, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
            
        case 2:
            if (func_8002CBF8(CABINET_AND_RUCKSACK_LABELS)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
            
        case 3:
            deactivateSprite(0x80);
            func_80059300();
            func_80059368();
            break;
            
        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = overlayScreenTable.unk_7;
            }
            break;
    
        case 5:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                    } else {
                        overlayScreenTable.cellIndex -= 4;
                        overlayScreenTable.screenState = 101;
                    }

                    playSfx(2);
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if (overlayScreenTable.cellIndex >= 4) {
                        overlayScreenTable.cellIndex -= 4;
                    } else {
                        overlayScreenTable.cellIndex = 8;
                        overlayScreenTable.screenState = 101;
                    }
                
                    playSfx(2);
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                    
                    if ((overlayScreenTable.cellIndex != 3) && (overlayScreenTable.cellIndex != 7)) {
                        
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                        
                    } else if (overlayScreenTable.pageNumber != 3) {
                        
                        if (overlayScreenTable.cellIndex == 3) {
                            overlayScreenTable.cellIndex = 0;
                        } else {
                            overlayScreenTable.cellIndex = 4;
                        }
                        
                        overlayScreenTable.pageNumber++;
                        
                        func_800BF0C8();
                        func_800BF5C0();
                        
                        playSfx(2);
                        
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {

                if (!set) {

                    if (overlayScreenTable.cellIndex != 8) {

                       overlayScreenTable.previousCellIndex = overlayScreenTable.cellIndex;
                        
                        if (overlayScreenTable.cellIndex && overlayScreenTable.cellIndex != 4) {
                            
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                            
                        } else if (overlayScreenTable.pageNumber) {

                            if (overlayScreenTable.cellIndex == 0) {
                                overlayScreenTable.cellIndex = 3;
                            } else {
                                overlayScreenTable.cellIndex = 7;
                            }

                            overlayScreenTable.pageNumber--;
                            func_800BF0C8();
                            func_800BF5C0();

                            playSfx(2);
                            
                        }

                        set = TRUE;
                        
                    }
                
                }
                    
            }

            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-96.0f, 16.0f);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(96.0f, 16.0f);
            } else {
                func_800C7E58();
            }
    
            break;

        case 101:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex == 8) {
                        overlayScreenTable.cellIndex = 1;
                        overlayScreenTable.screenState = 5;
                        playSfx(2);
                    } else if (overlayScreenTable.cellIndex < 4) {
                        overlayScreenTable.cellIndex += 4;
                        playSfx(2);
                    }

                    set = TRUE;
                    
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
            
                if (!set) {
                    
                    
                    if (overlayScreenTable.cellIndex != 8) {

                        if (overlayScreenTable.cellIndex >= 4) {
                            overlayScreenTable.cellIndex -= 4;
                        } else {
                            overlayScreenTable.cellIndex += 4;
                            overlayScreenTable.screenState = 5;
                        }
                        
                        playSfx(2);
                        set = TRUE;
                        
                    }
                
                }
                
            }

              if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 8) {

                        if (overlayScreenTable.cellIndex != 3 && overlayScreenTable.cellIndex != 7) {
                            overlayScreenTable.cellIndex++;
                            playSfx(2);
                        }
                        
                        set = TRUE;
    
                    }
                    
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {

                if (!set) {

                    if (overlayScreenTable.cellIndex != 8) {

                        if (overlayScreenTable.cellIndex != 0 && overlayScreenTable.cellIndex != 4) {
                            overlayScreenTable.cellIndex--;
                            playSfx(2);
                        }

                        set = TRUE;
                        
                    }
                    
                }
            
            }

            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-96.0f, 16.0f);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(96.0f, 16.0f);
            } else {
                func_800C7E58();
            }
            
            break;
    
    }

    if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 102) {

        if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
            
            if (!set) {

                if (overlayScreenTable.unk_3 != 0xFF) {

                    overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                    overlayScreenTable.unk_3 = 0xFF;
                    overlayScreenTable.screenState = overlayScreenTable.unk_5;
                    overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                    func_800BF0C8();
                    func_800BF5C0();
                    
                } else {
                    func_800BF804();
                    resetAnimationState(0x79);
                    func_800C7E38();
                    func_800C7E58();
                    deactivateSprite(0x82);
                    overlayScreenTable.screenState = 2;
                }

                playSfx(1);
                set = TRUE;
                
            }
            
        }

        if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            
            if (!set) {

                if (overlayScreenTable.unk_3 != 0xFF) {

                    if (func_800C0450()) {
                        
                        func_800C0504();
                        
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        overlayScreenTable.unk_3 = 0xFF;
                        overlayScreenTable.screenState = overlayScreenTable.unk_5;
                        overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                        
                        func_800BF0C8();
                        func_800BF5C0();

                        playSfx(0);
                        
                    } else {
                        playSfx(3);
                    }
                    
                } else if (func_800C0450()) {
                    overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                    overlayScreenTable.unk_5 = overlayScreenTable.screenState;
                    overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                    func_800BF5C0();
                    playSfx(0);
                } else {
                    playSfx(3);
                }

                set = TRUE;
                
            }
            
        }

        if (checkButtonPressed(CONTROLLER_1, BUTTON_Z)) {

            if (!set) {

                temp = 0;
                type = 1;

                if (overlayScreenTable.screenState != 5) {

                    if (overlayScreenTable.screenState == 101) {
                        temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
                    }
                    
                } else {
                    temp = D_801890E8[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
                }

                if (temp) {
                    func_800C81E8(type, temp);
                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                }
                
            }
            
        }

    }
    
    if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 102) {
        setSpriteViewSpacePosition(0x79, func_800C0238(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, func_800C02A8(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
        func_800C0310();
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0238);

f32 func_800C0238(u8 arg0, u8 arg1) {

   f32 res;
    
    switch (arg0) {

        case 5:

            arg1 &= 0xFF;
            res = D_80116D48[arg1 % 4];
            
            break;
        
        case 101:

            if (arg1 < 8) {
                res = D_80116D60[arg1 % 4];
            } else {
                res = D_80116D70;
            }
            
            break;

    }

    return res;

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C02A8);

f32 func_800C02A8(u8 arg0, u8 arg1) {

   f32 res;
    
    switch (arg0) {

        case 5:
            res = D_80116D58[arg1 / 4];
            break;
        
        case 101:

            if (arg1 < 8) {
                res = D_80116D74[arg1 / 4];
            } else {
                res = D_80116D7C;
            }
            
            break;

    }

    return res;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0310);

void func_800C0310(void) {

    u8 i;

    setSpriteColor(0x83, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0xFF);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteColor(0x84, 255, 255, 255, 255);

    for (i = 0; i < 9; i++) {

        if (D_801890E8[(overlayScreenTable.pageNumber * 8) + i]) {
            setSpriteColor(0xA3 + i, 255, 255, 255, 255);
        }
        
    }

    for (i = 0; i < 9; i++) {
        
        if (gPlayer.belongingsSlots[i] != 0) {
            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 255, 255, 255, 255);
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0450);

bool func_800C0450(void) {

    bool result = FALSE;
    u8 temp;
    
    switch (overlayScreenTable.screenState) {

        case 5:
            temp = D_801890E8[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            break;
        case 101:
            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            break;
        
    }

    if (getItemFlags(temp) & 0x1000) {
        result = TRUE;
    }

    if (temp == 0) {
        result = TRUE;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0504);

void func_800C0504(void) {

    u8 temp;
    
    switch (overlayScreenTable.screenState) {

        case 5:
            
            temp = D_801890E8[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.screenState) {
                
                D_801890E8[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = 
                    D_801890E8[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                
                D_801890E8[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
                
            } else {
                D_801890E8[(overlayScreenTable.pageNumber * 8) + overlayScreenTable.cellIndex] = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
                gPlayer.belongingsSlots[overlayScreenTable.unk_3] = temp;
            }

            break;

        case 101:

            temp = gPlayer.belongingsSlots[overlayScreenTable.cellIndex];
            
            if (overlayScreenTable.unk_5 == overlayScreenTable.screenState) {
                
                gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = gPlayer.belongingsSlots[overlayScreenTable.unk_3];
                gPlayer.belongingsSlots[overlayScreenTable.unk_3] = temp;
                
            } else {
                gPlayer.belongingsSlots[overlayScreenTable.cellIndex] = D_801890E8[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3];
                D_801890E8[(overlayScreenTable.unk_4 * 8) + overlayScreenTable.unk_3] = temp;
            }
            
            break;
        
    }
    
}

/* end of cabinet functions */


// misc. helper

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0688);

u8 func_800C0688(u8 tool) {
    
    u8 level;

    level = 0;
    
    switch (tool) {
        case 29:
            level = gPlayer.bottleContents;
            break;
        case 1:
            level = gPlayer.toolLevels[0];
            break;
        case 2:
            level = gPlayer.toolLevels[1];
            break;
        case 3:
            level = gPlayer.toolLevels[2];
            break;
        case 4:
            level = gPlayer.toolLevels[3];
            break;
        case 5:
            level = gPlayer.toolLevels[4];
            break;
        }
    
    return level;
    
}


/* house extension selection functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0714);

void func_800C0714(void) {
    
    // clock
    func_800B2F34();

    setMoneySprites();

    // extensions list
    dmaSprite(HOUSE_EXTENSIONS_LIST, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(HOUSE_EXTENSIONS_LIST, 0.0f, 0.0f, 8.0f);
    setSpriteColor(HOUSE_EXTENSIONS_LIST, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(HOUSE_EXTENSIONS_LIST, 2);
    setBilinearFiltering(HOUSE_EXTENSIONS_LIST, TRUE);
    startSpriteAnimation(HOUSE_EXTENSIONS_LIST, 0, 0);

    // price labels
    dmaSprite(0x83, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, TRUE);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, TRUE);
    startSpriteAnimation(0x84, 1, 0);
    
    dmaSprite(0x85, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, -32.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x85, 2);
    setBilinearFiltering(0x85, TRUE);
    startSpriteAnimation(0x85, 1, 0);
    
    dmaSprite(0x86, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 0.0f, -48.0f, 8.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x86, 2);
    setBilinearFiltering(0x86, TRUE);
    startSpriteAnimation(0x86, 1, 0);
    
    dmaSprite(0x87, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, -64.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x87, 2);
    setBilinearFiltering(0x87, TRUE);
    startSpriteAnimation(0x87, 1, 0);
    
    dmaSprite(0x88, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x88, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x88, 2);
    setBilinearFiltering(0x88, TRUE);
    startSpriteAnimation(0x88, 1, 0);
        
    dmaSprite(HOUSE_EXTENSIONS_BACKGROUND, (u32)&_photoSmallTextureSegmentRomStart, (u32)&_photoSmallTextureSegmentRomEnd, (u32)&_photoSmallAssetsIndexSegmentRomStart, (u32)&_photoSmallAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(HOUSE_EXTENSIONS_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(HOUSE_EXTENSIONS_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(HOUSE_EXTENSIONS_BACKGROUND, 2);
    setBilinearFiltering(HOUSE_EXTENSIONS_BACKGROUND, TRUE);
    startSpriteAnimation(HOUSE_EXTENSIONS_BACKGROUND, 0, 0);

    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);

    func_800C0F24();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C0F24);

void func_800C0F24(void) {
    
    fadeInClock();

    updateSpriteAlpha(HOUSE_EXTENSIONS_BACKGROUND, 0xFF, 24);
    updateSpriteAlpha(HOUSE_EXTENSIONS_LIST, 0xFF, 24);

    // price labels
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
    updateSpriteAlpha(0x86, 0xFF, 24);
    updateSpriteAlpha(0x87, 0xFF, 24);
    updateSpriteAlpha(0x88, 0xFF, 24);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C101C);

void func_800C101C(void) {

    fadeOutClock();
    
    updateSpriteAlpha(HOUSE_EXTENSIONS_BACKGROUND, 0, 24);
    updateSpriteAlpha(HOUSE_EXTENSIONS_LIST, 0, 24);
    
    // price labels
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(0x86, 0, 24);
    updateSpriteAlpha(0x87, 0, 24);
    updateSpriteAlpha(0x88, 0, 24);

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80121D74);

static const u16 D_80121D74[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C1124);

// house extension selection callback
void func_800C1124(void) {

    u16 buff[6];
    u16* ptr = D_80121D74;

    bool set;
    u8 temp;

    memcpy(buff, ptr, 12);

    set = FALSE;
    
    switch (overlayScreenTable.screenState) {

        case 0:
            func_800C0714();
            overlayScreenTable.screenState = 1;
            break;
        case 1:
            if (func_8002CBF8(CLOCK)) {
                setSpriteViewSpacePosition(CURSOR_HAND, houseExtensionCursorHandXPositions[overlayScreenTable.cellIndex], houseExtensionCursorHandYPositions[overlayScreenTable.cellIndex], 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 7;
            }
            break;
        
        case 2:
            if (func_8002CBF8(CLOCK)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;

        case 4:
            
            if (func_80043A88()) {
                
                temp = func_80043C6C(0);
                overlayScreenTable.pageNumber = temp;
                
                switch (overlayScreenTable.pageNumber) {                      
                    case 0:                                 
                        initializeMessageBox(0, 6, 0x83, 0);
                        break;
                    case 1:                                 
                        initializeMessageBox(0, 6, 0x84, 0);
                    }
                
                overlayScreenTable.screenState = 5;
            
            }
            
            break;

        case 5:

            if (func_8003F0DC()) {
                
                switch (overlayScreenTable.pageNumber) {                    
                
                    case 0:                                 
                        fadeOutClock();
                        updateSpriteAlpha(HOUSE_EXTENSIONS_BACKGROUND, 0, 24);
                        updateSpriteAlpha(HOUSE_EXTENSIONS_LIST, 0, 24);
                        updateSpriteAlpha(0x83, 0, 24);
                        updateSpriteAlpha(0x84, 0, 24);
                        updateSpriteAlpha(0x85, 0, 24);
                        updateSpriteAlpha(0x86, 0, 24);
                        updateSpriteAlpha(0x87, 0, 24);
                        updateSpriteAlpha(0x88, 0, 24);
                        deactivateOverlayScreenSprites(0);
                        deactivateOverlayScreenSprites(1);
                        resetAnimationState(CURSOR_HAND);
                        overlayScreenTable.screenState = 2;
                        gHouseExtensionSelection = overlayScreenTable.cellIndex;
                        setDailyEventBit(8);
                        break;

                    case 1:
                        setSpriteViewSpacePosition(CURSOR_HAND, houseExtensionCursorHandXPositions[overlayScreenTable.cellIndex], houseExtensionCursorHandYPositions[overlayScreenTable.cellIndex], 32.0f);
                        startSpriteAnimation(CURSOR_HAND, 3, 0);
                        overlayScreenTable.screenState = 7;
                        break;
                    
                }
                
            }

            break;

        case 6:
            if (func_8003F0DC()) {
                setSpriteViewSpacePosition(CURSOR_HAND, houseExtensionCursorHandXPositions[overlayScreenTable.cellIndex], houseExtensionCursorHandYPositions[overlayScreenTable.cellIndex], 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 7;
            }
            break;

        case 7:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 5) {
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                    }
            
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    if (overlayScreenTable.cellIndex != 0) {
                        overlayScreenTable.cellIndex--;
                        playSfx(2);
                    }
                    
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                
                resetAnimationState(CURSOR_HAND);
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                func_8003F360(0, -4, 0);
                
                if (checkLifeEventBit(buff[overlayScreenTable.cellIndex])) {
                    initializeMessageBox(0, 6, 0x6D, 0);
                    overlayScreenTable.screenState = 6;
                } else {
                    initializeDialogueSession(0, 0x3D, 0x21, 0x40);
                    overlayScreenTable.screenState = 4;
                }
                
                playSfx(0);
                set = TRUE;
                
            }
            
            setSpriteViewSpacePosition(CURSOR_HAND, houseExtensionCursorHandXPositions[overlayScreenTable.cellIndex], houseExtensionCursorHandYPositions[overlayScreenTable.cellIndex], 32.0f);
            
            break;
        
    }

    if (overlayScreenTable.screenState >= 7 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        fadeOutClock();
        updateSpriteAlpha(HOUSE_EXTENSIONS_BACKGROUND, 0, 24);
        updateSpriteAlpha(HOUSE_EXTENSIONS_LIST, 0, 24);
        updateSpriteAlpha(0x83, 0, 24);
        updateSpriteAlpha(0x84, 0, 24);
        updateSpriteAlpha(0x85, 0, 24);
        updateSpriteAlpha(0x86, 0, 24);
        updateSpriteAlpha(0x87, 0, 24);
        updateSpriteAlpha(0x88, 0, 24);
        deactivateOverlayScreenSprites(0);
        deactivateOverlayScreenSprites(1);
        resetAnimationState(CURSOR_HAND);
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

/* estimate functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C1710);

void func_800C1710(void) {

    u8 temp;
    
    // clock
    func_800B2F34();

    setMoneySprites();

    // labels
    dmaSprite(0x82, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);

    dmaSprite(ESTIMATE_BACKGROUND, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(ESTIMATE_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(ESTIMATE_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(ESTIMATE_BACKGROUND, 2);
    setBilinearFiltering(ESTIMATE_BACKGROUND, 1);
    startSpriteAnimation(ESTIMATE_BACKGROUND, 0, 0);

    temp = func_8009BC44();
    
    if (temp != 0xFF) {
        dmaSprite(0x83, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, 0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x83, 32.0f, 36.0f, 16.0f);
        setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0x83, 2);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, temp + 1, 0);
    }

    temp = func_8009BC54();
    
    if (temp != 0xFF) {
        dmaSprite(0x84, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, 0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 32.0f, 20.0f, 8.0f);
        setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0x84, 2);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, temp + 1, 0);
    }

    temp = func_8009BBAC();
    
    if (temp != 0xFF) {
        dmaSprite(0x85, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, 0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, -24.0f, 4.0f, 8.0f);
        setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
        setSpriteRenderingLayer(0x85, 2);
        setBilinearFiltering(0x85, 1);
        startSpriteAnimation(0x85, temp + 1, 0);
    }
    
    setOverlayScreenSprites(2, 0x9A, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)0x8028F200, (u16*)0x8028F880, (AnimationFrameMetadata*)0x8028F900, (u32*)0x8028F9C0, 0, 1, 0, -60.0f, 4.0f, 256.0f, 8);
    dmaOverlayScreenSprites(2, func_8009B3DC(), 1, 3);
    setOverlayScreenSpritesRGBA(2, 255, 255, 255, 255);
    
    setOverlayScreenSprites(3, 0x9B, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)0x8028F200, (u16*)0x8028F880, (AnimationFrameMetadata*)0x8028F900, (u32*)0x8028F9C0, 0, 1, 0, -60.0f, -12.0f, 256.0f, 8);
    dmaOverlayScreenSprites(3, func_8009B4EC(), 1, 3);
    setOverlayScreenSpritesRGBA(3, 255, 255, 255, 255);
    
    setOverlayScreenSprites(4, 0x9C, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)0x8028F200, (u16*)0x8028F880, (AnimationFrameMetadata*)0x8028F900, (u32*)0x8028F9C0, 0, 1, 0, -60.0f, -28.0f, 256.0f, 8);
    dmaOverlayScreenSprites(4, func_8009B564(), 1, 3);
    setOverlayScreenSpritesRGBA(4, 255, 255, 255, 255);
    
    setOverlayScreenSprites(5, 0x9D, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)0x8028F200, (u16*)0x8028F880, (AnimationFrameMetadata*)0x8028F900, (u32*)0x8028F9C0, 0, 1, 0, -16.0f, -44.0f, 256.0f, 8);
    dmaOverlayScreenSprites(5, gLumber, 3, 3);
    setOverlayScreenSpritesRGBA(5, 255, 255, 255, 255);
    
    setOverlayScreenSprites(6, 0xA0, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)0x8028F200, (u16*)0x8028F880, (AnimationFrameMetadata*)0x8028F900, (u32*)0x8028F9C0, 0, 1, 0, -16.0f, -60.0f, 256.0f, 8);
    dmaOverlayScreenSprites(6, fodderQuantity, 3, 3);
    setOverlayScreenSpritesRGBA(6, 255, 255, 255, 255);
    
    setMessageBoxViewSpacePosition(0, 44.0f, 20.0f, 352.0f);
    setMessageBoxSpriteIndices(0, 0xFF, 0xFF, 0xFF);
    func_8003F360(0, -4, 2);
    initializeMessageBox(0, 0, 5, 0x8000);
    
    func_800C2060();
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2060);

void func_800C2060(void) {

    fadeInClock();

    updateSpriteAlpha(ESTIMATE_BACKGROUND, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2128);

void func_800C2128(void) {

    fadeOutClock();

    updateSpriteAlpha(ESTIMATE_BACKGROUND, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    
    updateSpriteAlpha(0x85, 0, 24);
    
    messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags &= ~0x8000;
    
    func_8003F130(0);
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C224C);

void func_800C224C(void) {

    u8 temp = 0;

    switch (overlayScreenTable.screenState) {
        case 4:
            break;
        case 0:
            func_800C1710();
            overlayScreenTable.screenState = 1;
            break;
        case 1:
            if (func_8002CBF8(CLOCK)) {
                overlayScreenTable.screenState = 4;
            }
            break;
        case 2:
            if (func_8002CBF8(CLOCK)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;
    }
    
    if ((overlayScreenTable.screenState >= 4) && (checkButtonPressed(CONTROLLER_1, BUTTON_B)) && (temp == 0)) {
        func_800C2128();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}


/* kitchen picture functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2364);

void func_800C2364(void) {
    
    // clock
    func_800B2F34();
    
    dmaSprite(0x82, (u32)&_powerNutTreeDrawingTextureSegmentRomStart, (u32)&_powerNutTreeDrawingTextureSegmentRomEnd, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomStart, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomEnd,  0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, TRUE);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    func_800C2504();
    func_800C292C();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2504);

void func_800C2504(void) {

    u8 i;

    for (i = 0; i < 10; i++) {

        if (powerNutBits & D_80116F24[i]) {

            dmaSprite(0xA3 + i, (u32)&_powerNutTreeDrawingTextureSegmentRomStart, (u32)&_powerNutTreeDrawingTextureSegmentRomEnd, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomStart, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )0x8026F000, NULL, (u16* )0x8027E200, (u16* )0x8027E700, (u8* )0x8027EB00, NULL, 0, FALSE);
            
            setSpriteViewSpacePosition(0xA3 + i, D_80116E18[i], D_80116E40[i], 8.0f);
            setSpriteColor(0xA3 + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(0xA3 + i, 2);
            setBilinearFiltering(0xA3 + i, 1);
            startSpriteAnimation(0xA3 + i, 1, 0);
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2660);

void func_800C2660(void) {
    
    resetAnimationState(0xA3);
    resetAnimationState(0xA4);
    resetAnimationState(0xA5);
    resetAnimationState(0xA6);
    resetAnimationState(0xA7);
    resetAnimationState(0xA8);
    resetAnimationState(0xA9);
    resetAnimationState(0xAA);
    resetAnimationState(0xAB);
    resetAnimationState(0xAC);
    
    startSpriteAnimation(overlayScreenTable.cellIndex + 0xA3, 2, 0xFE);
    startSpriteAnimation(0xA3, 1, 0);
    startSpriteAnimation(0xA4, 1, 0);
    startSpriteAnimation(0xA5, 1, 0);
    startSpriteAnimation(0xA6, 1, 0);
    startSpriteAnimation(0xA7, 1, 0);
    startSpriteAnimation(0xA8, 1, 0);
    startSpriteAnimation(0xA9, 1, 0);
    startSpriteAnimation(0xAA, 1, 0);
    startSpriteAnimation(0xAB, 1, 0);
    startSpriteAnimation(0xAC, 1, 0);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2780);

u8 func_800C2780(u8 cellIndex, u8 arg1) {

    u8 result = 0xFF;
    
    s8 temp;
    
    switch (arg1) {

        case 0:

            temp = cellIndex;

            while (temp < 10 && result == 0xFF) {

                if (powerNutBits & D_80116F24[temp]) {
                    result = temp;
                }

                temp++;
                
            }
            
            break;
        
        case 1:

            temp = cellIndex;

            temp++;

            while (temp < 10 && result == 0xFF) {

                if (powerNutBits & D_80116F24[temp]) {
                    result = temp;
                }

                temp++;
                
            }
            
            break;

        case 2:

            temp = cellIndex;
            temp--;

            while (temp >= 0 && result == 0xFF) {

                if (powerNutBits & D_80116F24[temp]) {
                    result = temp;
                }
                
                temp--;
            
            }
                
            
            break;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C292C);

inline void func_800C292C(void) {
    
    fadeInClock();

    updateSpriteAlpha(0x82, 0xFF, 24);

    updateSpriteAlpha(0xA3, 0xFF, 24);
    updateSpriteAlpha(0xA4, 0xFF, 24);
    updateSpriteAlpha(0xA5, 0xFF, 24);
    updateSpriteAlpha(0xA6, 0xFF, 24);
    updateSpriteAlpha(0xA7, 0xFF, 24);
    updateSpriteAlpha(0xA8, 0xFF, 24);
    updateSpriteAlpha(0xA9, 0xFF, 24);
    updateSpriteAlpha(0xAA, 0xFF, 24);
    updateSpriteAlpha(0xAB, 0xFF, 24);
    updateSpriteAlpha(0xAC, 0xFF, 24);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2A54);

void func_800C2A54(void) {

    fadeOutClock();

    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0xA3, 0, 24);
    updateSpriteAlpha(0xA4, 0, 24);
    updateSpriteAlpha(0xA5, 0, 24);
    updateSpriteAlpha(0xA6, 0, 24);
    updateSpriteAlpha(0xA7, 0, 24);
    updateSpriteAlpha(0xA8, 0, 24);
    updateSpriteAlpha(0xA9, 0, 24);
    updateSpriteAlpha(0xAA, 0, 24);
    updateSpriteAlpha(0xAB, 0, 24);
    updateSpriteAlpha(0xAC, 0, 24);

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2B8C);

// kitchen picture main loop callbackk
void func_800C2B8C(void) {

    bool set = FALSE;
    u8 temp;

    switch (overlayScreenTable.screenState) {

        case 0:
            func_800C2364();
            overlayScreenTable.screenState = 1;
            break;

        case 1:
            if (func_8002CBF8(CLOCK)) {
                
                if (overlayScreenTable.cellIndex != 0xFF) {
                    setSpriteViewSpacePosition(CURSOR_HAND, D_80116E18[overlayScreenTable.cellIndex] - 8.0f, D_80116E40[overlayScreenTable.cellIndex] - 8.0f, 32.0f);
                    startSpriteAnimation(CURSOR_HAND, 3, 0);
                    func_800C2660();
                } 
                
                overlayScreenTable.screenState = 5;
                
            }
            
            break;
        
        case 2:
            if (func_8002CBF8(CLOCK)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;

        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;

        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = 5;
            }
            break;


        case 5:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 0xFF) {
                        
                        temp = func_800C2780(overlayScreenTable.cellIndex, 2);
                        
                        if (temp != 0xFF) {
                            overlayScreenTable.cellIndex = temp;
                            setSpriteViewSpacePosition(CURSOR_HAND, D_80116E18[temp] - 8.0f, D_80116E40[temp] - 8.0f, 32.0f);
                            func_800C2660();
                            playSfx(2);
                        }
    
                    }

                    set = TRUE;

                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 0xFF) {

                        temp = func_800C2780(overlayScreenTable.cellIndex, 1);

                        if (temp != 0xFF) {
                            
                            overlayScreenTable.cellIndex = temp;
                            setSpriteViewSpacePosition(CURSOR_HAND, D_80116E18[temp] - 8.0f, D_80116E40[temp] - 8.0f, 32.0f);
                            func_800C2660();
                            playSfx(2);
                            
                        }
                        
                
                    }
                
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!set) {
                    
                    if (func_800C2780(overlayScreenTable.cellIndex, 0) != 0xFF) {
                        
                        setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                        setMessageBoxSpriteIndices(0, 1, 0, 0);
                        func_8003F360(0, -4, 2);
                        initializeMessageBox(0, 2, overlayScreenTable.cellIndex + 0x23, 0);
                        
                        controllers[CONTROLLER_1].buttonPressed = 0;
                        overlayScreenTable.screenState = 4;
                        
                        playSfx(0);
                        
                    }
                    
                    set = TRUE;
                
                }
                
            }
            
            break;
            
    }

    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(CURSOR_HAND);
        func_800C2A54();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

/* end kitchen picture functions */


/* calendar functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C2F50);

void func_800C2F50(void) {
    
    // main calendar components
    dmaSprite(CALENDAR_BACKGROUND, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CALENDAR_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(CALENDAR_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(CALENDAR_BACKGROUND, 2);
    setBilinearFiltering(CALENDAR_BACKGROUND, TRUE);
    startSpriteAnimation(CALENDAR_BACKGROUND, 0, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    func_800C3108();
    func_800C34C0(gYear, gSeason, gDayOfMonth);
    func_800C39A0();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3108);

void func_800C3108(void) {

    u8 i, j, k;
    u8 temp;
    
    D_80205640[0] = 0;
    D_80205640[1] = 1;
    D_80205640[2] = 2;
    D_80205640[3] = 3;
    D_80205640[4] = 4;

    if (checkLifeEventBit(0x5B)) {
        D_80205640[5] = 5;
        D_80205640[6] = 6;
        D_80205640[7] = 7;
        D_80205640[8] = 8;
        D_80205640[9] = 9;
    } else {
        D_80205640[5] = 0xFF;
        D_80205640[6] = 0xFF;
        D_80205640[7] = 0xFF;
        D_80205640[8] = 0xFF;
        D_80205640[9] = 0xFF;
    }

    for (i = 0; i < 5; i++) {
        
        for (j = 0; j < 7; j++) {

            temp = D_80189108[i][j];

            if (temp != 0xFF) {

                D_80205640[temp] = 0xFF;
                
                dmaSprite(CALENDAR_STICKERS_BASE + temp, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
                setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + temp, D_80116E68[0], D_80116E90[0], 8.0f);
                setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + temp, (j << 5) - 122, -(i * 24) + 0x1A, 4.0f);
                setSpriteColor(CALENDAR_STICKERS_BASE + temp, 0xFF, 0xFF, 0xFF, 0);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + temp, 2);
                setBilinearFiltering(CALENDAR_STICKERS_BASE + temp, 1);
                startSpriteAnimation(CALENDAR_STICKERS_BASE + temp, 1, temp);
                
            }
            
        } 
    
    }

    
    for (k = 0; k < 10; k++) {

        if (D_80205640[k] != 0xFF) {

            dmaSprite(CALENDAR_STICKERS_BASE + k, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
            setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + k, D_80116E68[k], D_80116E90[k], 8.0f);
            setSpriteColor(CALENDAR_STICKERS_BASE + k, 0xFF, 0xFF, 0xFF, 0);
            setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + k, 2);
            setBilinearFiltering(CALENDAR_STICKERS_BASE + k, 1);
            startSpriteAnimation(CALENDAR_STICKERS_BASE + k, 1, k);
            
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C34C0);

void func_800C34C0(u8 year, u8 season, u8 dayOfMonth) {
    
    u8 row, col;
    
    u8 dayIndex;
    u8 startingWeekday;
    u8 skipCount;
    bool isSaturdayStart;
    
    dmaSprite(0x83, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, 
          (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, 
          NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, 
          (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 255, 255, 255, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, season - 1);

    skipCount = 0;
    dayIndex = 0;

    startingWeekday = (year % 7 + ((season - 1) * 30) % 7) % 7;
    isSaturdayStart = (startingWeekday == 6); 

    for (row = 0; row < 5; row++) {

        for (col = 0; col < 7; col++) {

            if (skipCount == startingWeekday && dayIndex < 30) {
                
                dmaSprite(0x8F + dayIndex, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, 
                          (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, 
                          NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, 
                          (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);

                setSpriteViewSpacePosition(0x8F + dayIndex, (f32)(col * 32), (f32)(-row * 24), 16.0f);
                setSpriteColor(0x8F + dayIndex, 255, 255, 255, 0);
                setSpriteRenderingLayer(0x8F + dayIndex, 2);
                setBilinearFiltering(0x8F + dayIndex, 1);

                if (isSaturdayStart) {
                    if (dayIndex == 22) {
                        startSpriteAnimation(0x8F + dayIndex, 2, 30);
                    } else {
                        startSpriteAnimation(0x8F + dayIndex, 2, dayIndex);
                    }
                } else {
                    startSpriteAnimation(0x8F + dayIndex, 2, dayIndex);
                }

                if (dayIndex == dayOfMonth - 1 || (isSaturdayStart && dayIndex == 22 && dayOfMonth == 30)) {

                    dmaSprite(0xB7, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, 
                        (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, 
                        NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, 
                        (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
                    
                    setSpriteViewSpacePosition(0xB7, (f32)(col * 32), (f32)(-row * 24), 32.0f);
                    setSpriteColor(0xB7, 255, 255, 255, 0);
                    setSpriteRenderingLayer(0xB7, 2);
                    setBilinearFiltering(0xB7, 1);
                    startSpriteAnimation(0xB7, 3, 0);
                }

                dayIndex++;
                
            } else {
                skipCount++;
            }
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C39A0);

void func_800C39A0(void) {
    
    u8 i;

    fadeInClock();

    updateSpriteAlpha(CALENDAR_BACKGROUND, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    
    updateSpriteAlpha(0xB7, 0xFF, 24);
    
    updateSpriteAlpha(CALENDAR_STICKERS_BASE, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 1, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 2, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 3, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 4, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 5, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 6, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 7, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 8, 0xFF, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 9, 0xFF, 24);
    
    for (i = 0; i < 30; i++) {
        updateSpriteAlpha(0x8F + i, 0xFF, 24);
    }
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3B1C);

void func_800C3B1C(void) {

    u8 i;
    
    fadeOutClock();

    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);

    updateSpriteAlpha(0xB7, 0, 24);

    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 1, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 2, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 3, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 4, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 5, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 6, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 7, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 8, 2);
    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 9, 2);

    updateSpriteAlpha(CALENDAR_STICKERS_BASE, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 1, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 2, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 3, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 4, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 5, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 6, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 7, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 8, 0, 24);
    updateSpriteAlpha(CALENDAR_STICKERS_BASE + 9, 0, 24);
    
    for (i = 0; i < 30; i++) {
        updateSpriteAlpha(0x8F + i, 0, 24);
    }
    
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C3D20);

// calendar main loop callback
void func_800C3D20(void) {

    bool set = FALSE;
    u8 temp;

    switch (overlayScreenTable.screenState) {

        case 0:
            func_800C2F50();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            
            if (func_8002CBF8(CALENDAR_BACKGROUND)) {

                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 1, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 2, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 3, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 4, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 5, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 6, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 7, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 8, 3);
                setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + 9, 3);

                overlayScreenTable.screenState = 5;
                
                setSpriteViewSpacePosition(CURSOR_HAND, 
                    (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                    (f32)(-(overlayScreenTable.unk_9 * 24) + 22), 
                    32.0f);
                
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                
            }
            
            break;
        
        case 2:
            if (func_8002CBF8(CALENDAR_BACKGROUND)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;

        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = 5;
            }
            break;

        case 5:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
                
                if (!set) {

                    if (overlayScreenTable.unk_8) {
                        
                        overlayScreenTable.unk_8--;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32) ((overlayScreenTable.unk_8 * 18) + 100), 
                            (f32) (-(overlayScreenTable.unk_9 * 24) + 22), 
                            32.0f);
                        
                        playSfx(2);
                        
                    }
                
                    set = TRUE;
                
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_8 != 1) {
                        
                        overlayScreenTable.unk_8++;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND,
                           (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                           (f32) (-(overlayScreenTable.unk_9 * 24) + 22), 
                           32.0f);
                    
                        playSfx(2);
                    
                    }
            
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_9) {
                        
                        overlayScreenTable.unk_9--;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND,
                           (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                           (f32) (-(overlayScreenTable.unk_9 * 24) + 22), 
                           32.0f);
                    
                        playSfx(2);
                    
                    }
            
                    set = TRUE;
                    
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_9 != 4) {
                        
                        overlayScreenTable.unk_9++;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND,
                           (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                           (f32) (-(overlayScreenTable.unk_9 * 24) + 22), 
                           32.0f);
                    
                        playSfx(2);
                    
                    }
            
                    set = TRUE;
                    
                }
                
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
                
                if (!set) {

                    overlayScreenTable.cellIndex = overlayScreenTable.unk_8 + (overlayScreenTable.unk_9 * 2);
                    
                    if (D_80205640[overlayScreenTable.cellIndex] != 0xFF) {
                        
                        overlayScreenTable.screenState = 6;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32)((overlayScreenTable.unk_A * 32) - 136), 
                            (f32)(-(overlayScreenTable.unk_B * 24) + 16), 32.0f);
                        
                        setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 
                            (f32)((overlayScreenTable.unk_A * 32) - 122), 
                            (f32)(-(overlayScreenTable.unk_B * 24) + 26), 
                            32.0f);
                        
                        setSpriteColor(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 0xFF, 0xFF, 0xFF, 0x80);
                        setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 3);
                        playSfx(0);
                    
                    }
            
                    set = TRUE;
                    
                }
                
            }
            
            break;

        case 6:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
                
                if (!set) {

                    if (overlayScreenTable.unk_A) {
                        
                        overlayScreenTable.unk_A--;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32) ((overlayScreenTable.unk_A * 32) - 136), 
                            (f32) (-(overlayScreenTable.unk_B * 24) + 16), 
                            32.0f);

                        setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 
                            (f32)((overlayScreenTable.unk_A * 32) - 122), 
                            (f32)(-(overlayScreenTable.unk_B * 24) + 26), 
                            32.0f);
                    
                        
                        playSfx(2);
                        
                    }
                
                    set = TRUE;
                
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {

                    if (overlayScreenTable.unk_A != 6) {
                        
                        overlayScreenTable.unk_A++;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32) ((overlayScreenTable.unk_A * 32) - 136), 
                            (f32) (-(overlayScreenTable.unk_B * 24) + 16), 
                            32.0f);

                        setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 
                            (f32)((overlayScreenTable.unk_A * 32) - 122), 
                            (f32)(-(overlayScreenTable.unk_B * 24) + 26), 
                            32.0f);
                    
                        
                        playSfx(2);
                        
                    }
                
                    set = TRUE;
                
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {

                    if (overlayScreenTable.unk_B) {
                        
                        overlayScreenTable.unk_B--;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32) ((overlayScreenTable.unk_A * 32) - 136), 
                            (f32) (-(overlayScreenTable.unk_B * 24) + 16), 
                            32.0f);

                        setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 
                            (f32)((overlayScreenTable.unk_A * 32) - 122), 
                            (f32)(-(overlayScreenTable.unk_B * 24) + 26), 
                            32.0f);
                    
                        
                        playSfx(2);
                        
                    }
                
                    set = TRUE;
                
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {

                    if (overlayScreenTable.unk_B != 4) {
                        
                        overlayScreenTable.unk_B++;
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32) ((overlayScreenTable.unk_A * 32) - 136), 
                            (f32) (-(overlayScreenTable.unk_B * 24) + 16), 
                            32.0f);

                        setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 
                            (f32)((overlayScreenTable.unk_A * 32) - 122), 
                            (f32)(-(overlayScreenTable.unk_B * 24) + 26), 
                            32.0f);
                    
                        
                        playSfx(2);
                        
                    }
                
                    set = TRUE;
                
                }
                
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {
            
                if (!set) {

                    temp = D_80189108[overlayScreenTable.unk_B][overlayScreenTable.unk_A];
                    
                    if (temp == 0xFF) {
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, 
                            (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                            (f32)(-(overlayScreenTable.unk_9 * 24) + 22), 
                            32.0f);
                        
                        setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 
                           (f32)((overlayScreenTable.unk_A * 32) - 122), 
                           (f32)(-(overlayScreenTable.unk_B * 24) + 26), 
                           4.0f);
                        
                        setSpriteColor(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 0xFF, 0xFF, 0xFF, temp);
                        
                        D_80189108[overlayScreenTable.unk_B][overlayScreenTable.unk_A] = overlayScreenTable.cellIndex;
                        D_80205640[overlayScreenTable.cellIndex] = 0xFF;
                        
                        overlayScreenTable.screenState = 5;
                        playSfx(0);
                        
                    }
                    
                    set = TRUE;
                    
                }
                
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {

                if (!set) {

                    setSpriteViewSpacePosition(CURSOR_HAND, 
                        (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                        (f32)(-(overlayScreenTable.unk_9 * 24) + 22), 
                        32.0f);
                    
                    setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, D_80116E68[overlayScreenTable.cellIndex], D_80116E90[overlayScreenTable.cellIndex], 8.0f);
                    setSpriteColor(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 255, 255, 255, 255);
                    setSpriteRenderingLayer(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 3);
                    overlayScreenTable.screenState = 5;
        
                    playSfx(1);
            
                    set = TRUE;
                
                }
                
            }

            break;
        
    }

    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(CURSOR_HAND);
        func_800C3B1C();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}


/* recipe book functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C4A74);

void func_800C4A74(void) {
    
    dmaSprite(RECIPES_BACKGROUNDS, (u32)&_bookTextureSegmentRomStart, (u32)&_bookTextureSegmentRomEnd, (u32)&_bookAssetsIndexSegmentRomStart, (u32)&_bookAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(RECIPES_BACKGROUNDS, 0.0f, 0.0f, 8.0f);
    setSpriteColor(RECIPES_BACKGROUNDS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(RECIPES_BACKGROUNDS, 2);
    setBilinearFiltering(RECIPES_BACKGROUNDS, TRUE);
    startSpriteAnimation(RECIPES_BACKGROUNDS, 0, 0);
    
    // "Recipe" label
    dmaSprite(0x82, (u32)&_bookScreenLabelsTextureSegmentRomStart, (u32)&_bookScreenLabelsTextureSegmentRomEnd, (u32)&_bookScreenLabelsAssetsIndexSegmentRomStart, (u32)&_bookScreenLabelsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, TRUE);
    startSpriteAnimation(0x82, 1, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // up and down arrows
    func_800C7850();
    // up and down arrows
    func_800C7E78();
    func_800C4E90();
    
    setSpriteColor(0xA3, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA4, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA5, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA6, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA7, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA8, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xA9, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAA, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAB, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAC, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAD, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAE, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xAF, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xB0, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xB1, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0);
    
    func_800C5080();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C4E90);

void func_800C4E90(void) {

    u8 i, j, k;

    for (i = 0; i < 0x10; i++) {
        resetAnimationState(0xA3 + i);
    }

    i = 0;

    for (k = 0; k < 2; k++) {
        
        for (j = 0; j < 8; j++) {

            if (checkRecipe(overlayScreenTable.pageNumber * 16 + i)) {

                dmaSprite(0xA3 + i, (u32)&_overlayScreenNamesTextureSegmentRomStart, (u32)&_overlayScreenNamesTextureSegmentRomEnd, (u32)&_overlayScreenNamesAssetsIndexSegmentRomStart, (u32)&_overlayScreenNamesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8027EC00, NULL, (u16*)0x80289400, (u16*)0x80289500, (u8*)0x80289800, NULL, 0, FALSE);
                setSpriteViewSpacePosition(0xA3 + i, (k * 128) + -120.0f, 64.0f - (j * 16), 8.0f);
                setSpriteColor(0xA3 + i, 255, 255, 255, 255);
                setSpriteRenderingLayer(0xA3 + i, 2);
                setBilinearFiltering(0xA3 + i, 1);
                startSpriteAnimation(0xA3 + i, 1, (i + (overlayScreenTable.pageNumber * 16)));
                
            }

            i++;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5080);

void func_800C5080(void) {

    fadeInClock();

    updateSpriteAlpha(RECIPES_BACKGROUNDS, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);

    updateSpriteAlpha(0xA3, 0xFF, 24);
    updateSpriteAlpha(0xA4, 0xFF, 24);
    updateSpriteAlpha(0xA5, 0xFF, 24);
    updateSpriteAlpha(0xA6, 0xFF, 24);
    updateSpriteAlpha(0xA7, 0xFF, 24);
    updateSpriteAlpha(0xA8, 0xFF, 24);
    updateSpriteAlpha(0xA9, 0xFF, 24);
    updateSpriteAlpha(0xAA, 0xFF, 24);
    updateSpriteAlpha(0xAB, 0xFF, 24);
    updateSpriteAlpha(0xAC, 0xFF, 24);
    updateSpriteAlpha(0xAD, 0xFF, 24);
    updateSpriteAlpha(0xAE, 0xFF, 24);
    updateSpriteAlpha(0xAF, 0xFF, 24);
    updateSpriteAlpha(0xB0, 0xFF, 24);
    updateSpriteAlpha(0xB1, 0xFF, 24);
    updateSpriteAlpha(0xB2, 0xFF, 24);
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5218);

void func_800C5218(void) {

    fadeOutClock();

    updateSpriteAlpha(RECIPES_BACKGROUNDS, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);

    updateSpriteAlpha(0xA3, 0, 24);
    updateSpriteAlpha(0xA4, 0, 24);
    updateSpriteAlpha(0xA5, 0, 24);
    updateSpriteAlpha(0xA6, 0, 24);
    updateSpriteAlpha(0xA7, 0, 24);
    updateSpriteAlpha(0xA8, 0, 24);
    updateSpriteAlpha(0xA9, 0, 24);
    updateSpriteAlpha(0xAA, 0, 24);
    updateSpriteAlpha(0xAB, 0, 24);
    updateSpriteAlpha(0xAC, 0, 24);
    updateSpriteAlpha(0xAD, 0, 24);
    updateSpriteAlpha(0xAE, 0, 24);
    updateSpriteAlpha(0xAF, 0, 24);
    updateSpriteAlpha(0xB0, 0, 24);
    updateSpriteAlpha(0xB1, 0, 24);
    updateSpriteAlpha(0xB2, 0, 24);

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C53C0);

// recipe book callback
void func_800C53C0(void) {

    bool set = FALSE;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            func_800C4A74();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (func_8002CBF8(0x82)) {
                setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;
        
        case 4:
            
            if (!(messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & (0x40 | 0x80))) {
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {
                        if (func_8003FE9C(0)) {
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                
                }
                
                if ((checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST))) {
                    
                    if (!set) {
                        
                        if (func_8003FFF4(0)) {
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                }
                
                if (func_80040140(0)) {
                    func_800C80F0(0, -40.0f);
                } else {
                    func_800C81A8();
                }
                if (func_800401C8(0)) {
                    func_800C814C(0, 40.0f);
                } else {
                    func_800C81C8();
                }
                
                if (checkButtonPressed(CONTROLLER_1, (BUTTON_B | BUTTON_A))) {

                    if (!set) {

                        messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags &= ~0x8000;
                        
                        func_8003F130(0);
                        
                        set = TRUE;
                        
                        func_800C81A8();
                        func_800C81C8();
                        func_8003FB4C(0, 1);
                        overlayScreenTable.screenState = 5;
                        
                        playSfx(0);
                        
                        setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                        startSpriteAnimation(CURSOR_HAND, 3, 0);
                        
                    }
                    
                }
            
            }
            
            break;

        case 5:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
                
                if (!set) {
                    if (overlayScreenTable.unk_8) {
                        overlayScreenTable.unk_8--;
                        playSfx(2);
                    } else if (overlayScreenTable.pageNumber) {
                        overlayScreenTable.pageNumber--;
                        func_800C4E90();
                        overlayScreenTable.unk_8 = 1;
                        playSfx(2);
                    }
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8* 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                    set = TRUE;
                
                }
            
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_8 != 1) {
                        overlayScreenTable.unk_8++;
                        playSfx(2);
                    } else if (overlayScreenTable.pageNumber != 2) {
                        overlayScreenTable.pageNumber++;
                        func_800C4E90();
                        overlayScreenTable.unk_8 = 0;
                        playSfx(2);
                    }
                
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8* 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                    set = TRUE;
                
                }
            }
            

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_9) {
                        overlayScreenTable.unk_9--;
                        playSfx(2);
                    }
                
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8* 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                    set = TRUE;
               
                }
                
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {

                if (!set) {
                    
                    if (overlayScreenTable.unk_9 != 7) {
                        overlayScreenTable.unk_9++;
                        playSfx(2);
                    }
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8* 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                    set = TRUE;
                    
                }
                
            }
            
            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-128.0f, 0);
            } else {
                func_800C7E38();
            }
            if (overlayScreenTable.pageNumber != 2) {
                func_800C7D9C(128.0f, 0);
            } else {
                func_800C7E58();
            }

            break;
    
    
    }

    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        func_800C7E58();
        func_800C7E38();
        resetAnimationState(CURSOR_HAND);
        func_800C5218();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}


/* album functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5AA4);

void func_800C5AA4(void) {
    
    dmaSprite(ALBUM_BACKGROUND, (u32)&_bookTextureSegmentRomStart, (u32)&_bookTextureSegmentRomEnd, (u32)&_bookAssetsIndexSegmentRomStart, (u32)&_bookAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(ALBUM_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(ALBUM_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(ALBUM_BACKGROUND, 2);
    setBilinearFiltering(ALBUM_BACKGROUND, TRUE);
    setSpritePaletteIndex(ALBUM_BACKGROUND, 1);
    startSpriteAnimation(ALBUM_BACKGROUND, 0, 0);
    
    // "Album" label
    dmaSprite(0x82, (u32)&_bookScreenLabelsTextureSegmentRomStart, (u32)&_bookScreenLabelsTextureSegmentRomEnd, (u32)&_bookScreenLabelsAssetsIndexSegmentRomStart, (u32)&_bookScreenLabelsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, TRUE);
    startSpriteAnimation(0x82, 3, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // arrows
    func_800C7850();
    func_800C5DA4();
    
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    
    func_800C6E98();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C5DA4);

void func_800C5DA4(void) {

    resetAnimationState(0x83);
    resetAnimationState(0x84);
    resetAnimationState(0x85);
    resetAnimationState(0x86);

    switch (overlayScreenTable.pageNumber) {

        case 0:

            if (albumBits & 1) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 0, 0);
            }
            
            if (albumBits & 2) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 1, 0);
            }
            
            if (albumBits & 4) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 2, 0);
            }
            
            if (albumBits & 8) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 3, 0);
            }
            
            break;

        case 1:

            if (albumBits & 0x10) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 4, 0);
            }
            
            if (albumBits & 0x20) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 5, 0);
            }
            
            if (albumBits & 0x40) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 6, 0);
            }
            
            if (albumBits & 0x80) {
                
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                
                switch (gWife) {                        
                    case 0:                                 
                        startSpriteAnimation(0x86, 8, 0);
                        break;
                    case 1:                                 
                        startSpriteAnimation(0x86, 9, 0);
                        break;
                    case 2:                                 
                        startSpriteAnimation(0x86, 7, 0);
                        break;
                    case 3:                                 
                        startSpriteAnimation(0x86, 11, 0);
                        break;
                    case 4:                                 
                        startSpriteAnimation(0x86, 10, 0);
                        break;
                    }
                
            }
            
            break;

        case 2:

            if (albumBits & 0x100) {
                
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);

                switch (gWife) {           
                    
                    case 0:                                 
                        startSpriteAnimation(0x83, 0xD, 0);
                        break;
                    case 1:                                 
                        startSpriteAnimation(0x83, 0xE, 0);
                        break;
                    case 2:                                 
                        startSpriteAnimation(0x83, 0xC, 0);
                        break;
                    case 3:                                 
                        startSpriteAnimation(0x83, 0x10, 0);
                        break;
                    case 4:                                 
                        startSpriteAnimation(0x83, 0xF, 0);
                        break;
                    }
                
            }
            
            if (albumBits & 0x200) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 0x11, 0);
            }
            
            if (albumBits & 0x400) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 0x12, 0);
            }
            
            if (albumBits & 0x800) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 0x13, 0);
            }
            
            break;

        case 3:

            if (albumBits & 0x1000) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8028FA00, (u8*)0x80292A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x83, 2);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 0x14, 0);
            }
            
            if (albumBits & 0x2000) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x80295A00, (u8*)0x80298A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x84, 2);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 0x15, 0);
            }
            
            if (albumBits & 0x4000) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x8029BA00, (u8*)0x8029EA00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x85, 2);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 0x16, 0);
            }
            
            if (albumBits & 0x8000) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)0x802A1A00, (u8*)0x802A4A00, (u16*)0x802A7A00, (u16*)0x802AA200, (u8*)0x802AA600, (u32*)0x802AA800, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteRenderingLayer(0x86, 2);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 0x17, 0);
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C6E98);

void func_800C6E98(void) {

    fadeInClock();

    updateSpriteAlpha(ALBUM_BACKGROUND, 0xFF, 24);
    
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
    updateSpriteAlpha(0x86, 0xFF, 24);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C6F70);

void func_800C6F70(void) {

    fadeOutClock();

    updateSpriteAlpha(ALBUM_BACKGROUND, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(0x86, 0, 24);

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80121E34);

static const u8 D_80121E34[20] = { 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x2D, 0x2E, 0, 0, 0, 0 };

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7058);

// album callback
void func_800C7058(void) {

    bool set;
    u8 *ptr = D_80121E34;
    
    u8 buff[16];

    memcpy(buff, ptr, 16);
    
    set = FALSE;
    
    switch (overlayScreenTable.screenState) {

        case 0:
            func_800C5AA4();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (func_8002CBF8(0x82)) {
                setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 120), (f32)(40 - (overlayScreenTable.unk_9 * 80)), 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;
        
        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = 5;
            }
            break;

        case 5:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_8) {
                        overlayScreenTable.unk_8--;
                        playSfx(2);
                    } else if (overlayScreenTable.pageNumber) {
                        overlayScreenTable.pageNumber--;
                        func_800C5DA4();
                        overlayScreenTable.unk_8 = 1;
                        playSfx(2);
                    
                    }
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 120), (f32)(40 - (overlayScreenTable.unk_9 * 80)), 32.0f);

                    set = TRUE;
                    
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                
                    if (overlayScreenTable.unk_8 != 1) {
                        overlayScreenTable.unk_8++;
                        playSfx(2);
                    } else if (overlayScreenTable.pageNumber != 3) {
                        overlayScreenTable.pageNumber++;
                        func_800C5DA4();
                        overlayScreenTable.unk_8 = 0;
                        playSfx(2);
                    }
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 120), (f32)(40 - (overlayScreenTable.unk_9 * 80)), 32.0f);

                    set = TRUE;
                    
                }
            
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.unk_9) {
                        overlayScreenTable.unk_9--;
                        playSfx(2);
                    }
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 120), (f32)(40 - (overlayScreenTable.unk_9 * 80)), 32.0f);

                    set = TRUE;
                
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                
                    if (overlayScreenTable.unk_9 != 1) {
                        overlayScreenTable.unk_9++;
                        playSfx(2);
                    }
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 120), (f32)(40 - (overlayScreenTable.unk_9 * 80)), 32.0f);

                    set = TRUE;
                
                }
            
            }

            if ((checkButtonPressed(0, BUTTON_A))) {

                if (!set) {

                    // TODO: probably can refactor this
                    if (albumBits & (1 << ((overlayScreenTable.unk_8 * 2) + overlayScreenTable.unk_9 + (overlayScreenTable.pageNumber * 4)))) {
                        
                        setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                        setMessageBoxSpriteIndices(0, 1, 0, 0);
                        func_8003F5D0(0, 0x10, 3);
                        func_8003F360(0, -4, 2);
    
                        // TODO: probably can refactor this
                        initializeMessageBox(0, 2, buff[overlayScreenTable.pageNumber * 4 + ((overlayScreenTable.unk_8 * 2) + overlayScreenTable.unk_9)], 0);
                        
                        controllers[CONTROLLER_1].buttonPressed = 0;
                        overlayScreenTable.screenState = 4;
                        
                        playSfx(0);
                        
                    }
                    
                    set = TRUE;

                }
            
            }

            if (overlayScreenTable.pageNumber) {
                func_800C7D40(-128.0f, 8.0f);
            } else {
                func_800C7E38();
            }
                
            if (overlayScreenTable.pageNumber != 3) {
                func_800C7D9C(128.0f, 8.0f);
            } else {
                func_800C7E58();
            }
            
            break;
    
    }
    
    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        func_800C7E58();
        func_800C7E38();
        resetAnimationState(CURSOR_HAND);
        fadeOutClock();
        updateSpriteAlpha(ALBUM_BACKGROUND, 0, 24);
        updateSpriteAlpha(0x82, 0, 24);
        updateSpriteAlpha(0x83, 0, 24);
        updateSpriteAlpha(0x84, 0, 24);
        updateSpriteAlpha(0x85, 0, 24);
        updateSpriteAlpha(0x86, 0, 24);
        deactivateOverlayScreenSprites(0);
        deactivateOverlayScreenSprites(1);
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

/* end of album functions */


// misc. recipe helpers

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", addRecipe);

void addRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    recipesBits[temp >> 5] |= 1 << (temp & 0x1F);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", toggleRecipe);

void toggleRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    recipesBits[temp >> 5] &= ~(1 << (temp & 0x1F));
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", checkRecipe);

u32 checkRecipe(u16 bitIndex) {
    u32 temp = bitIndex;
    return recipesBits[temp >> 5] & (1 << (temp & 0x1F));
}


/* shared gold arrow functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7850);

void func_800C7850(void) {

    dmaSprite(DOWN_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(DOWN_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(DOWN_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DOWN_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(DOWN_ARROW, 1);
    setSpriteColor(DOWN_ARROW, 255, 255, 255, 255);
    func_8002C680(DOWN_ARROW, 2, 2);
    setSpriteRenderingLayer(DOWN_ARROW, 3);
    
    dmaSprite(UP_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(UP_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(UP_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(UP_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(UP_ARROW, 1);
    setSpriteColor(UP_ARROW, 255, 255, 255, 255);
    func_8002C680(UP_ARROW, 2, 2);
    setSpriteRenderingLayer(UP_ARROW, 3);
    
    dmaSprite(RIGHT_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(RIGHT_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(RIGHT_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(RIGHT_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(RIGHT_ARROW, 1);
    setSpriteColor(RIGHT_ARROW, 255, 255, 255, 255);
    func_8002C680(RIGHT_ARROW, 2, 2);
    setSpriteRenderingLayer(RIGHT_ARROW, 3);
    
    dmaSprite(LEFT_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(LEFT_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(LEFT_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(LEFT_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(LEFT_ARROW, 1);
    setSpriteColor(LEFT_ARROW, 255, 255, 255, 255);
    func_8002C680(LEFT_ARROW, 2, 2);
    setSpriteRenderingLayer(LEFT_ARROW, 3);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7C88);

void func_800C7C88(f32 x, f32 y) {
    startSpriteAnimation(DOWN_ARROW, 11, 0xFE);
    setSpriteViewSpacePosition(DOWN_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7CE4);

void func_800C7CE4(f32 x, f32 y) {
    startSpriteAnimation(UP_ARROW, 10, 0xFE);
    setSpriteViewSpacePosition(UP_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7D40);

void func_800C7D40(f32 x, f32 y) {
    startSpriteAnimation(LEFT_ARROW, 1, 0xFE);
    setSpriteViewSpacePosition(LEFT_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7D9C);

void func_800C7D9C(f32 x, f32 y) {
    startSpriteAnimation(RIGHT_ARROW, 12, 0xFE);
    setSpriteViewSpacePosition(RIGHT_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7DF8);

void func_800C7DF8(void) {
    resetAnimationState(DOWN_ARROW);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E18);

void func_800C7E18(void) {
    resetAnimationState(UP_ARROW);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E38);

void func_800C7E38(void) {
    resetAnimationState(LEFT_ARROW);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E58);

void func_800C7E58(void) {
    resetAnimationState(RIGHT_ARROW);
}


// up and down arrows

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C7E78);

void func_800C7E78(void) {
    
    dmaSprite(0x7A, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x7A, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7A, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7A, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7A, 1);
    setSpriteColor(0x7A, 255, 255, 255, 255);
    func_8002C680(0x7A, 2, 2);
    setSpriteRenderingLayer(0x7A, 3);

    dmaSprite(0x7B, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x7B, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7B, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7B, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7B, 1);
    setSpriteColor(0x7B, 255, 255, 255, 255);
    func_8002C680(0x7B, 2, 2);
    setSpriteRenderingLayer(0x7B, 3);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C80F0);

void func_800C80F0(f32 x, f32 y) {
    startSpriteAnimation(0x7B, 11, 0xFE);
    setSpriteViewSpacePosition(0x7B, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C814C);

void func_800C814C(f32 x, f32 y) {
    startSpriteAnimation(0x7A, 10, 0xFE);
    setSpriteViewSpacePosition(0x7A, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81A8);

void func_800C81A8(void) {
    resetAnimationState(0x7B);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81C8);

void func_800C81C8(void) {
    resetAnimationState(0x7A);
}

/* end of shared arrow functions */


/* shared helper */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C81E8);

void func_800C81E8(u8 type, u32 itemOrToolIndex) {

    u16 temp;
    u32 idx;
    u16* ptr;
    
    switch (type) {
        // tool type
        case 0:
            idx = itemOrToolIndex;
            idx &= 0xFF;
            ptr = D_80116F4C;
            break;
        case 1:
            idx = itemOrToolIndex;
            idx &= 0xFF;
            ptr = D_80116F88;
            break;
        case 2:
            idx = itemOrToolIndex;
            idx &= 0xFF;
            ptr = D_80117148;
            break;
        // FIXME: seems fake
        default:
            goto skip;
    }

    temp = ptr[idx];

skip:
    switch (temp) {

        case 0x12:
            convertNumberToGameVariableString(0x11, D_802373E8, 0);
            break;

        case 0x33:
            convertNumberToGameVariableString(0x11, wateringCanUses, 0);
            break;        

        case 0x34:
            convertNumberToGameVariableString(0x11, D_801FC154, 0);
            break;
 
        case 0x35:
            convertNumberToGameVariableString(0x11, D_8018A725, 0);
            break;
        
        case 0x36:
            convertNumberToGameVariableString(0x11, D_80204DF4, 0);
            break;
        
        case 0x37:
            convertNumberToGameVariableString(0x11, D_8013DC2C, 0);
            break;
        
        case 0x38:
            convertNumberToGameVariableString(0x11, D_801FAD91, 0);
            break;
        
        case 0x39:
            convertNumberToGameVariableString(0x11, D_80237458, 0);
            break;
        
        case 0x3A:
            convertNumberToGameVariableString(0x11, D_801C3F70, 0);
            break;
        
        case 0x3B:
            convertNumberToGameVariableString(0x11, D_80205636, 0);
            break;

        case 0x3C:
            convertNumberToGameVariableString(0x11, D_802373A8, 0);
            break;
        
        case 0x3D:
            convertNumberToGameVariableString(0x11, D_801C3E28, 0);
            break;

        case 0x3E:
            convertNumberToGameVariableString(0x11, chickenFeedQuantity, 0);
            break;

        case 0x42:
            temp += gPlayer.bottleContents;
            break;

    }

    convertNumberToGameVariableString(0x1F, D_80205204, 0);
    convertNumberToGameVariableString(0x30, flowerShopPoints, 0);
    convertNumberToGameVariableString(0x31, bakeryCardPoints, 0);
    
    setMessageBoxViewSpacePosition(0, 0, -64.0f, 352);
    setMessageBoxSpriteIndices(0, 1, 0, 0);
    func_8003F5D0(0, 0x10, 3);
    func_8003F360(0, -4, 2);
    initializeMessageBox(0, 2, temp, 0);
    
    controllers[CONTROLLER_1].buttonPressed = 0;
    
}


/* pause screen map functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8424);

// main loop callback 30
// seems like load village map (cut from NA release) 
void func_800C8424(void) {
    
    dmaSprite(0x82, (u32)&_emptyTexture2SegmentRomStart, (u32)&_emptyTexture2SegmentRomEnd, (u32)&_emptyTexture2AssetsIndexSegmentRomStart, (u32)&_emptyTexture2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x80282000, (u16*)0x80282300, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x82, 1.0f, 1.0f, 1.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 0, 0, 0, 0);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0xA3, (u32)&_emptyTexture2SegmentRomStart, (u32)&_emptyTexture2SegmentRomEnd, (u32)&_emptyTexture2AssetsIndexSegmentRomStart, (u32)&_emptyTexture2AssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x80282000, (u16*)0x80282300, (u32*)0x80282400, NULL, 0, FALSE);
    setSpriteScale(0xA3, 1.0f, 1.0f, 1.0f);
    setBilinearFiltering(0xA3, 1);
    setSpriteColor(0xA3, 0, 0, 0, 0);
    startSpriteAnimation(0xA3, 1, 0xFE);
    
    switch (gBaseMapIndex) {
        
        case RANCH:
        case ANN_ROOM:
        case RANCH_BARN:
        case RANCH_STORE:
        case RANCH_HOUSE:
            setSpriteViewSpacePosition(0xA3, 0.0f, -72.0f, 8.0f);
            break;

        case BEACH:
            setSpriteViewSpacePosition(0xA3, 56.0f, -96.0f, 8.0f);
            break;

        case MOUNTAIN_1:
            setSpriteViewSpacePosition(0xA3, 88.0f, 12.0f, 8.0f);
            break;

        case MOUNTAIN_2:
        case CARPENTER_HUT:
            setSpriteViewSpacePosition(0xA3, 32.0f, 20.0f, 8.0f);
            break;
        
        case TOP_OF_MOUNTAIN_1:
            setSpriteViewSpacePosition(0xA3, 104.0f, 48.0f, 8.0f);
            break;

        case MOON_MOUNTAIN:
        case DUMPLING_HOUSE:
            setSpriteViewSpacePosition(0xA3, 104.0f, 84.0f, 8.0f);
            break;

        case POND:
            setSpriteViewSpacePosition(0xA3, -16.0f, 36.0f, 8.0f);
            break;

        case ELLI_ROOM:
        case BAKERY:
        case VILLAGE_1:
        case POPURI_ROOM:
        case FLOWER_SHOP:
        case CHURCH:
        case SOUVENIR_SHOP:
        case RICK_STORE:
        case TAVERN:
        case EMPTY_MAP_2:
            setSpriteViewSpacePosition(0xA3, -20.0f, -24.0f, 8.0f);
            break;

        case VILLAGE_2:
        case MIDWIFE_HOUSE:
        case LIBRARY:
        case MARIA_ROOM:
        case MAYOR_HOUSE:
        case POTION_SHOP_BEDROOM:
        case POTION_SHOP:
            setSpriteViewSpacePosition(0xA3, -44.0f, -56.0f, 8.0f);
            break;

        case SQUARE:
            setSpriteViewSpacePosition(0xA3, -72.0f, -16.0f, 8.0f);
            break;

        case RACE_TRACK:
            setSpriteViewSpacePosition(0xA3, -124.0f, -24.0f, 8.0f);
            break;
        
        case HARVEST_SPRITE_CAVE:
        case CAVE:
        case MINE:
        case MINE_2:
            setSpriteViewSpacePosition(0xA3, 56.0f, 32.0f, 8.0f);
            break;
                
        case KAREN_ROOM:
        case VINEYARD:
        case VINEYARD_HOUSE:
        case VINEYARD_CELLAR:
        case VINEYARD_CELLAR_BASEMENT:
            setSpriteViewSpacePosition(0xA3, 16.0f, -4.0f, 8.0f);
            break;

        case ROAD:
            setSpriteViewSpacePosition(0xA3, 24.0f, -44.0f, 8.0f);
            break;
        
        case FARM:
        case GREENHOUSE:
        case HOUSE:
        case BARN:
        case COOP:
        case KITCHEN:
        case BATHROOM:
            setSpriteViewSpacePosition(0xA3, 64.0f, -56.0f, 8.0f);
            break;
        
    }
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    overlayScreenTable.unk_8 = 0;
    overlayScreenTable.unk_9 = 0;
    overlayScreenTable.unk_A = 0;
    overlayScreenTable.unk_B = 0;
    
    setMainLoopCallbackFunctionIndex(PAUSE_SCREEN_MAP);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8784);

// main loop callback 45
// handle map scren navigation
void func_800C8784(void) {    

    u8 temp = 0;

    switch (overlayScreenTable.screenState) {
        
        case 0:
            updateSpriteRGBA(0x82, 255, 255, 255, 255, 24);
            updateSpriteRGBA(0xA3, 255, 255, 255, 255, 24);
            overlayScreenTable.screenState = 1;
            break;
            
        case 1:
            if (func_8002CBF8(0x82)) {
                overlayScreenTable.screenState = 2;
            }
            break;
            
        case 2:
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_B)) && (temp == 0)) {
                updateSpriteRGBA(0x82, 0, 0, 0, 0, 24);
                updateSpriteRGBA(0xA3, 0, 0, 0, 0, 24);
                overlayScreenTable.screenState = 3;
                playSfx(1);
            }
            break;
            
        case 3:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 4;
            }
            break;
            
        case 4:
            func_800B8844();
            break;

        }
    
}

/* end pause screen map functions */


/* horse/dog race functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C88F4);

// main loop callback function number 31
// triggered from square level interaction
void func_800C88F4(void) {

    func_800593EC();
    
    if (checkDailyEventBit(HORSE_RACE)) {
        removeKeyItem(0xE);
    } else {
        removeKeyItem(0xF);
    }

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][0] = 0;
    overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][1] = 0;
    overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][2] = 0;
    overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][3] = 0;
    overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][4] = 0;
    overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][5] = 0;

    setMainLoopCallbackFunctionIndex(HORSE_RACE_BETTING);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C8B04);

// load race betting sprites
void func_800C8B04(void) {
    
    D_80189858 = gGold;

    setMoneySprites();
    
    dmaSprite(0x81, (u32)&_photoSmallTextureSegmentRomStart, (u32)&_photoSmallTextureSegmentRomEnd, (u32)&_photoSmallAssetsIndexSegmentRomStart, (u32)&_photoSmallAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);

    dmaSprite(0x82, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);

    // check horse or dog race
    if (checkDailyEventBit(HORSE_RACE)) {
        startSpriteAnimation(0x82, 0, 0);
    } else {
        startSpriteAnimation(0x82, 0, 1);
    }
    
    dmaSprite(0x83, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);

    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 1, 0);

    dmaSprite(0x84, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 2, 0);
    
    dmaSprite(0xB9, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xB9, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0xB9, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0xB9, 2);
    setBilinearFiltering(0xB9, 1);
    startSpriteAnimation(0xB9, 1, 1);

    dmaSprite(0xBA, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBA, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0xBA, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0xBA, 2);
    setBilinearFiltering(0xBA, 1);
    startSpriteAnimation(0xBA, 1, 1);
    
    dmaSprite(0xBB, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBB, 0.0f, -32.0f, 8.0f);
    setSpriteColor(0xBB, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0xBB, 2);
    setBilinearFiltering(0xBB, 1);
    startSpriteAnimation(0xBB, 1, 1);
    
    dmaSprite(0xBC, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBC, 0.0f, -48.0f, 8.0f);
    setSpriteColor(0xBC, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0xBC, 2);
    setBilinearFiltering(0xBC, 1);
    startSpriteAnimation(0xBC, 1, 1);
    
    dmaSprite(0xBD, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBD, 0.0f, -64.0f, 8.0f);
    setSpriteColor(0xBD, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0xBD, 2);
    setBilinearFiltering(0xBD, 1);
    startSpriteAnimation(0xBD, 1, 1);
    
    dmaSprite(0xBE, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBE, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0xBE, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0xBE, 2);
    setBilinearFiltering(0xBE, 1);
    startSpriteAnimation(0xBE, 1, 1);
    
    dmaSprite(0x85, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 2, 1);

    dmaSprite(0x86, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 32.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x86, 2);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 2, 0);

    dmaSprite(CURSOR_HAND, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    setOverlayScreenSprites(2, 0x97, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -98.0f, -153.0f, 256.0f, 10);
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 255, 255, 255, 255);

    setOverlayScreenSprites(3, 0x9B, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -48.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(3, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][0], 1, 3);
    setOverlayScreenSpritesRGBA(3, 255, 255, 255, 255);

    setOverlayScreenSprites(4, 0x9D, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -64.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(4, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][1], 1, 3);
    setOverlayScreenSpritesRGBA(4, 255, 255, 255, 255);
    
    setOverlayScreenSprites(5, 0x9F, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -80.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(5, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][2], 1, 3);
    setOverlayScreenSpritesRGBA(5, 255, 255, 255, 255);
    
    setOverlayScreenSprites(6, 0xA1, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -96.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(6, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][3], 1, 3);
    setOverlayScreenSpritesRGBA(6, 255, 255, 255, 255);
    
    setOverlayScreenSprites(7, 0xA3, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -112.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(7, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][4], 1, 3);
    setOverlayScreenSpritesRGBA(7, 255, 255, 255, 255);
    
    setOverlayScreenSprites(8, 0xA5, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -128.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(8, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][5], 1, 3);
    setOverlayScreenSpritesRGBA(8, 255, 255, 255, 255);

    setOverlayScreenSprites(9, 0xA7, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)0x8026F000, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, 0, 6, 0, 14.0f, 34.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(9, overlayScreenStrings.unk_0[0], 2, 3);
    setOverlayScreenSpritesRGBA(9, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xA, 0xAA, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)0x8026F000, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, 0, 6, 0, 14.0f, 18.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xA, overlayScreenStrings.unk_0[1], 2, 3);
    setOverlayScreenSpritesRGBA(0xA, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xB, 0xAD, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)0x8026F000, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, 0, 6, 0, 14.0f, 2.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xB, overlayScreenStrings.unk_0[2], 2, 3);
    setOverlayScreenSpritesRGBA(0xB, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xC, 0xB0, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)0x8026F000, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, 0, 6, 0, 14.0f, -14.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xC, overlayScreenStrings.unk_6.unk_3[0], 2, 3);
    setOverlayScreenSpritesRGBA(0xC, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xD, 0xB3, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)0x8026F000, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, 0, 6, 0, 14.0f, -30.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xD, overlayScreenStrings.unk_6.unk_3[1], 2, 3);
    setOverlayScreenSpritesRGBA(0xD, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xE, 0xB6, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)0x8026F000, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, 0, 6, 0, 14.0f, -46.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xE, overlayScreenStrings.unk_6.unk_3[2], 2, 3);
    setOverlayScreenSpritesRGBA(0xE, 255, 255, 255, 255);
    
    func_800C9FF0();
    func_800CA590();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800C9FF0);

// initialize race betting screen message boxes
void func_800C9FF0(void) {
    
    setGameVariableString(0x2A, overlayScreenStrings.gameVariableStrings[0], 6);
    setGameVariableString(0x2B, overlayScreenStrings.gameVariableStrings[1], 6);
    setGameVariableString(0x2C, overlayScreenStrings.gameVariableStrings[2], 6);
    setGameVariableString(0x2D, overlayScreenStrings.gameVariableStrings[3], 6);
    setGameVariableString(0x2E, overlayScreenStrings.gameVariableStrings[4], 6);
    setGameVariableString(0x2F, overlayScreenStrings.gameVariableStrings[5], 6);
    
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -22.0f, 24.0f, 0);
    func_8003F5D0(0, 0xA, 2);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    func_8003FB4C(0, 1);
    initializeMessageBox(0, 0, 0x12, 0x80000);
    func_8003E77C(0, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -22.0f, 8.0f, 0);
    func_8003F5D0(1, 0xA, 2);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    func_8003FB4C(1, 1);
    initializeMessageBox(1, 0, 0x13, 0x80000);
    func_8003E77C(1, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(2, -22.0f, -8.0f, 0);
    func_8003F5D0(2, 0xA, 2);
    func_8003F630(2, 0, 2);
    func_8003F464(2, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    func_8003FB4C(2, 1);
    initializeMessageBox(2, 0, 0x14, 0x80000);
    func_8003E77C(2, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, -22.0f, -24.0f, 0);
    func_8003F5D0(3, 0xA, 2);
    func_8003F630(3, 0, 2);
    func_8003F464(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    func_8003FB4C(3, 1);
    initializeMessageBox(3, 0, 0x15, 0x80000);
    func_8003E77C(3, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, -22.0f, -40.0f, 0);
    func_8003F5D0(4, 0xA, 2);
    func_8003F630(4, 0, 2);
    func_8003F464(4, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    func_8003FB4C(4, 1);
    initializeMessageBox(4, 0, 0x16, 0x80000);
    func_8003E77C(4, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(5, (u8*)MESSAGE_BOX_6_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(5, -22.0f, -56.0f, 0);
    func_8003F5D0(5, 0xA, 2);
    func_8003F630(5, 0, 2);
    func_8003F464(5, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(5, -4, 2);
    setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
    setMessageBoxButtonMask(5, BUTTON_A);
    func_8003FB4C(5, 1);
    initializeMessageBox(5, 0, 0x17, 0x80000);
    func_8003E77C(5, 255, 255, 255, 255);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA590);

void func_800CA590(void) {
    updateSpriteAlpha(0x8E, 0xFF, 24);
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
    updateSpriteAlpha(0x86, 0xFF, 24);
    updateSpriteAlpha(0xB9, 0xFF, 24);
    updateSpriteAlpha(0xBA, 0xFF, 24);
    updateSpriteAlpha(0xBB, 0xFF, 24);
    updateSpriteAlpha(0xBC, 0xFF, 24);
    updateSpriteAlpha(0xBD, 0xFF, 24);
    updateSpriteAlpha(0xBE, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA678);

void func_800CA678(void) {
    updateSpriteAlpha(0x8E, 0, 24);
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);
    updateSpriteAlpha(0x86, 0, 24);
    updateSpriteAlpha(0xB9, 0, 24);
    updateSpriteAlpha(0xBA, 0, 24);
    updateSpriteAlpha(0xBB, 0, 24);
    updateSpriteAlpha(0xBC, 0, 24);
    updateSpriteAlpha(0xBD, 0, 24);
    updateSpriteAlpha(0xBE, 0, 24);
    func_8003DD14(0);
    func_8003DD14(1);
    func_8003DD14(2);
    func_8003DD14(3);
    func_8003DD14(4);
    func_8003DD14(5);
    deactivateOverlayScreenSprites(1);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    deactivateOverlayScreenSprites(7);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);
    deactivateOverlayScreenSprites(0xA);
    deactivateOverlayScreenSprites(0xB);
    deactivateOverlayScreenSprites(0xC);
    deactivateOverlayScreenSprites(0xD);
    deactivateOverlayScreenSprites(0xE);
    func_80053088();
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CA808);

// race betting screen callback
void func_800CA808(void) {

    u8 temp = 0;
    bool set = FALSE;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            func_800C8B04();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (func_8002CBF8(0x82)) {
                setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 4;
            }
            
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;
        
        case 4:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST) && !set) {
                
                if (overlayScreenTable.cellIndex) {
                    overlayScreenTable.cellIndex--;
                    playSfx(2);
                    setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                } else {
                    overlayScreenTable.cellIndex = 0xFF;
                    overlayScreenTable.screenState = 6;
                    playSfx(2);
                    setSpriteViewSpacePosition(CURSOR_HAND, 96.0f, -96.0f, 32.0f);
                }
                
                set = TRUE;
                    
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST) && !set) {
                
                    if ((overlayScreenTable.cellIndex) != 5) {
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                        setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    } else {
                        overlayScreenTable.cellIndex = 0xFF;
                        overlayScreenTable.screenState = 6;
                        playSfx(2);
                        setSpriteViewSpacePosition(CURSOR_HAND, 96.0f, -96.0f, 32.0f);
                    }
                
                    set = TRUE;
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A) && !set) {
                overlayScreenTable.screenState = 5;
                setSpriteViewSpacePosition(CURSOR_HAND, 32.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                set = TRUE;
                playSfx(0);
            }
            
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_START)) && !set) {
                overlayScreenTable.cellIndex = 0xFF;
                overlayScreenTable.screenState = 6;
                playSfx(2);
                setSpriteViewSpacePosition(CURSOR_HAND, 96.0f, -96.0f, 32.0f);
            }
            
            break;

        case 5:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    
                    if (adjustValue(D_80189858, 50, gGold) == 50) {
                        
                        if (overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenTable.cellIndex]) {
                            D_80189858 += adjustValue(D_80189858, 50, gGold);
                            overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenTable.cellIndex]--;
                            playSfx(2);
                        }
                        
                    }
                    
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, 32.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    
                    set = TRUE;
                    
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {

                if (!set) {

                    if (adjustValue(D_80189858, -50, gGold) == -50) {

                        if (overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenTable.cellIndex] != 99) {
                            D_80189858 += adjustValue(D_80189858, -50, gGold);
                            overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenTable.cellIndex]++;
                            playSfx(2);
                        }
                    }
                    
                    
                    setSpriteViewSpacePosition(CURSOR_HAND, 32.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                        
                }
                    
            }

            if ((checkButtonPressed(CONTROLLER_1, BUTTON_B | BUTTON_A))) {

                if (!set) {
                    overlayScreenTable.screenState = 4;
                    setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                    playSfx(1);
                }
                
            }

            dmaOverlayScreenSprites(1, (u32)D_80189858, 5, 3);
            dmaOverlayScreenSprites(overlayScreenTable.cellIndex + 3, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenTable.cellIndex], 1, 3);
                
            break;

        case 6:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    overlayScreenTable.cellIndex = 5;
                    playSfx(2);
                    setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                    overlayScreenTable.screenState = 4;
                }
                
            }

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                
                if (!set) {
                    overlayScreenTable.cellIndex = 0;
                    playSfx(2);
                    setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                    set = TRUE;
                    overlayScreenTable.screenState = 4;
                }
                
            }

            if ((checkButtonPressed(CONTROLLER_1, BUTTON_A))) {

                if (!set) {
                    
                    if (overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][0] 
                        || overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][1] 
                        || overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][2] 
                        || overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][3] 
                        || overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][4] 
                        || overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][5]) {
                        
                        overlayScreenStrings.unk_6F[overlayScreenStrings.unk_6C] = 0xFF;
                        gGold = D_80189858;
                        
                        if (checkDailyEventBit(HORSE_RACE)) {
                            acquireKeyItem(0xE);
                        } else {
                            acquireKeyItem(0xF);
                        }
                        
                    }
                    
                    resetAnimationState(CURSOR_HAND);
                    set = TRUE;
                    
                    func_800CA678();
                    overlayScreenTable.screenState = 2;
                    
                    playSfx(0);
                    
                }
            }

            break;
        
        }
    
    if (overlayScreenTable.screenState >= 4 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(CURSOR_HAND);
        func_800CA678();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CB0C4);

void func_800CB0C4(void) {

    u8 buffer[20][6];
    u8 temp;

    memcpy(buffer, D_801220CC, 114);

    if (gHour < 24) {
        overlayScreenStrings.unk_6C = 3;
    }
    if (gHour < 16) {
        overlayScreenStrings.unk_6C = 2;
    }
    if (gHour < 13) {
        overlayScreenStrings.unk_6C = 1;
    }
    if (gHour < 10) {
        overlayScreenStrings.unk_6C = 0;
    }

    overlayScreenStrings.gameVariableStrings[0][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][0];
    overlayScreenStrings.gameVariableStrings[0][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][1];
    overlayScreenStrings.gameVariableStrings[0][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][2];
    overlayScreenStrings.gameVariableStrings[0][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][3];
    overlayScreenStrings.gameVariableStrings[0][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][4];
    overlayScreenStrings.gameVariableStrings[0][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][5];

    overlayScreenStrings.gameVariableStrings[1][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][0];
    overlayScreenStrings.gameVariableStrings[1][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][1];
    overlayScreenStrings.gameVariableStrings[1][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][2];
    overlayScreenStrings.gameVariableStrings[1][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][3];
    overlayScreenStrings.gameVariableStrings[1][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][4];
    overlayScreenStrings.gameVariableStrings[1][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][5];

    overlayScreenStrings.gameVariableStrings[2][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][0];
    overlayScreenStrings.gameVariableStrings[2][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][1];
    overlayScreenStrings.gameVariableStrings[2][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][2];
    overlayScreenStrings.gameVariableStrings[2][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][3];
    overlayScreenStrings.gameVariableStrings[2][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][4];
    overlayScreenStrings.gameVariableStrings[2][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][5];

    overlayScreenStrings.gameVariableStrings[3][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][0];
    overlayScreenStrings.gameVariableStrings[3][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][1];
    overlayScreenStrings.gameVariableStrings[3][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][2];
    overlayScreenStrings.gameVariableStrings[3][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][3];
    overlayScreenStrings.gameVariableStrings[3][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][4];
    overlayScreenStrings.gameVariableStrings[3][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][5];

    overlayScreenStrings.gameVariableStrings[4][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][0];
    overlayScreenStrings.gameVariableStrings[4][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][1];
    overlayScreenStrings.gameVariableStrings[4][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][2];
    overlayScreenStrings.gameVariableStrings[4][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][3];
    overlayScreenStrings.gameVariableStrings[4][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][4];
    overlayScreenStrings.gameVariableStrings[4][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][5];

    overlayScreenStrings.gameVariableStrings[5][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][0];
    overlayScreenStrings.gameVariableStrings[5][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][1];
    overlayScreenStrings.gameVariableStrings[5][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][2];
    overlayScreenStrings.gameVariableStrings[5][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][3];
    overlayScreenStrings.gameVariableStrings[5][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][4];
    overlayScreenStrings.gameVariableStrings[5][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][5];

    overlayScreenStrings.unk_0[0] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][0];
    overlayScreenStrings.unk_0[1] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][1];
    overlayScreenStrings.unk_0[2] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][2];
    overlayScreenStrings.unk_6.unk_3[0] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][3];
    overlayScreenStrings.unk_6.unk_3[1]= overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][4];
    overlayScreenStrings.unk_6.unk_3[2] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][5];

    if (overlayScreenStrings.unk_6D == overlayScreenStrings.unk_6C) {
        
        overlayScreenStrings.gameVariableStrings[2][0] = horseInfo.name[0];
        overlayScreenStrings.gameVariableStrings[2][1] = horseInfo.name[1];
        overlayScreenStrings.gameVariableStrings[2][2] = horseInfo.name[2];
        overlayScreenStrings.gameVariableStrings[2][3] = horseInfo.name[3];
        overlayScreenStrings.gameVariableStrings[2][4] = horseInfo.name[4];
        overlayScreenStrings.gameVariableStrings[2][5] = horseInfo.name[5];
        
        D_801C3F78 |= 0x20;
        
    } else {
        D_801C3F78 &= ~0x20;
    }
    
    if (overlayScreenStrings.unk_6E == overlayScreenStrings.unk_6C) {
        D_801C3F78 |= 0x40;
    } else {
        D_801C3F78 &= ~0x40;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CB8F0);

void func_800CB8F0(void) {

    u8 buffer[20][6];
    u8 temp;

    memcpy(buffer, D_80122140, 114);

    if (gHour < 24) {
        overlayScreenStrings.unk_6C = 3;
    }
    if (gHour < 16) {
        overlayScreenStrings.unk_6C = 2;
    }
    if (gHour < 13) {
        overlayScreenStrings.unk_6C = 1;
    }
    if (gHour < 10) {
        overlayScreenStrings.unk_6C = 0;
    }

    overlayScreenStrings.gameVariableStrings[0][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][0];
    overlayScreenStrings.gameVariableStrings[0][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][1];
    overlayScreenStrings.gameVariableStrings[0][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][2];
    overlayScreenStrings.gameVariableStrings[0][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][3];
    overlayScreenStrings.gameVariableStrings[0][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][4];
    overlayScreenStrings.gameVariableStrings[0][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][0]][5];

    overlayScreenStrings.gameVariableStrings[1][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][0];
    overlayScreenStrings.gameVariableStrings[1][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][1];
    overlayScreenStrings.gameVariableStrings[1][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][2];
    overlayScreenStrings.gameVariableStrings[1][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][3];
    overlayScreenStrings.gameVariableStrings[1][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][4];
    overlayScreenStrings.gameVariableStrings[1][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][1]][5];

    overlayScreenStrings.gameVariableStrings[2][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][0];
    overlayScreenStrings.gameVariableStrings[2][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][1];
    overlayScreenStrings.gameVariableStrings[2][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][2];
    overlayScreenStrings.gameVariableStrings[2][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][3];
    overlayScreenStrings.gameVariableStrings[2][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][4];
    overlayScreenStrings.gameVariableStrings[2][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][2]][5];

    overlayScreenStrings.gameVariableStrings[3][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][0];
    overlayScreenStrings.gameVariableStrings[3][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][1];
    overlayScreenStrings.gameVariableStrings[3][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][2];
    overlayScreenStrings.gameVariableStrings[3][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][3];
    overlayScreenStrings.gameVariableStrings[3][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][4];
    overlayScreenStrings.gameVariableStrings[3][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][3]][5];

    overlayScreenStrings.gameVariableStrings[4][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][0];
    overlayScreenStrings.gameVariableStrings[4][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][1];
    overlayScreenStrings.gameVariableStrings[4][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][2];
    overlayScreenStrings.gameVariableStrings[4][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][3];
    overlayScreenStrings.gameVariableStrings[4][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][4];
    overlayScreenStrings.gameVariableStrings[4][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][4]][5];

    overlayScreenStrings.gameVariableStrings[5][0] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][0];
    overlayScreenStrings.gameVariableStrings[5][1] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][1];
    overlayScreenStrings.gameVariableStrings[5][2] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][2];
    overlayScreenStrings.gameVariableStrings[5][3] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][3];
    overlayScreenStrings.gameVariableStrings[5][4] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][4];
    overlayScreenStrings.gameVariableStrings[5][5] = buffer[overlayScreenStrings.unk_54[overlayScreenStrings.unk_6C * 2][5]][5];

    overlayScreenStrings.unk_0[0] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][0];
    overlayScreenStrings.unk_0[1] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][1];
    overlayScreenStrings.unk_0[2] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][2];
    overlayScreenStrings.unk_6.unk_3[0] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][3];
    overlayScreenStrings.unk_6.unk_3[1]= overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][4];
    overlayScreenStrings.unk_6.unk_3[2] = overlayScreenStrings.strings3[overlayScreenStrings.unk_6C][5];

    if (overlayScreenStrings.unk_6D == overlayScreenStrings.unk_6C) {
        
        overlayScreenStrings.gameVariableStrings[0][0] = dogInfo.name[0];
        overlayScreenStrings.gameVariableStrings[0][1] = dogInfo.name[1];
        overlayScreenStrings.gameVariableStrings[0][2] = dogInfo.name[2];
        overlayScreenStrings.gameVariableStrings[0][3] = dogInfo.name[3];
        overlayScreenStrings.gameVariableStrings[0][4] = dogInfo.name[4];
        overlayScreenStrings.gameVariableStrings[0][5] = dogInfo.name[5];
        
        D_801C3F78 |= 0x20;
        
    } else {
        D_801C3F78 &= ~0x20;
    }
    
    if (overlayScreenStrings.unk_6E == overlayScreenStrings.unk_6C) {
        D_801C3F78 |= 0x40;
    } else {
        D_801C3F78 &= ~0x40;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC11C);

// unused or inline
bool func_800CC11C(u8 arg0, u8 arg1, u8 arg2) {

    bool result = FALSE;
    u8 i;
    
    for (i = 0; i < arg1; i++) {

        if (overlayScreenStrings.strings3[arg0][i] == arg2) {
            result = TRUE;
        }
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC17C);

void func_800CC17C(void) {

    u8 arr[19];
    
    u8 i;
    
    u8 temp;
    u8 temp2;
    u8 temp3;
    u8 temp4;

    for (i = 0; i < 19; i++) {
        arr[i] = 0;
    }

    overlayScreenStrings.unk_6D = 0xFF;

    if ((checkLifeEventBit(0x51)) || (checkLifeEventBit(0x52))) {
        overlayScreenStrings.unk_6D = getRandomNumberInRange(0, 2);
    }

    i = 0;

    temp4 = getRandomNumberInRange(40, 100);
    
    for (i = 0; i < 6; ) {

        temp = getRandomNumberInRange(1, 18); 

        if (arr[temp] == 0) {

            overlayScreenStrings.unk_54[0][i] = temp;

            temp2 = getRandomNumberInRange(1, temp4 / 2);

            temp4 -= temp2;

            overlayScreenStrings.strings2[3][i] = temp2;

            if (temp4 < 2) {
                temp4 = 2;
            }

            arr[temp] = 0xFF;

            i++;
            
        }
        
    }

    i = 0;

    temp4 = getRandomNumberInRange(40, 100);
    
    for (i = 0; i < 6; ) {

        temp = getRandomNumberInRange(1, 18);

        if (arr[temp] == 0) {

            overlayScreenStrings.unk_54[2][i] = temp;

            temp2 = getRandomNumberInRange(1, temp4 / 2);

            temp4 -= temp2;

            overlayScreenStrings.strings2[4][i] = temp2;

            if (temp4 < 2) {
                temp4 = 2;
            }

            arr[temp] = 0xFF;

            i++;
            
        }
        
    }

    i = 0;

    temp4 = getRandomNumberInRange(40, 100);
    
    for (i = 0; i < 6; ) {

        temp = getRandomNumberInRange(1, 18);
        
        if (arr[temp] == 0) {

            overlayScreenStrings.unk_54[4][i] = temp;

            temp2 = getRandomNumberInRange(1, temp4 / 2);

            temp4 -= temp2;

            overlayScreenStrings.strings2[5][i] = temp2;

            if (temp4 < 2) {
                temp4 = 2;
            }

            arr[temp] = 0xFF;

            i++;
            
        }
        
    }

    if (checkLifeEventBit(0x51)) {
        temp3 = getRandomNumberInRange(0, 2);
        overlayScreenStrings.unk_54[1][temp3 * 6] = 0;
        overlayScreenStrings.unk_6E = temp3;
    }

    if (checkLifeEventBit(0x52)) {
        temp3 = getRandomNumberInRange(0, 2);
        overlayScreenStrings.unk_54[0][temp3 * 6 + 1] = 0;
        overlayScreenStrings.unk_6E = temp3;
    }
    
    overlayScreenStrings.strings2[0][0] = 0;
    overlayScreenStrings.strings2[0][1] = 0;
    overlayScreenStrings.strings2[0][2] = 0;
    overlayScreenStrings.strings2[0][3] = 0;
    overlayScreenStrings.strings2[0][4] = 0;
    overlayScreenStrings.strings2[0][5] = 0;
    
    overlayScreenStrings.strings2[1][0] = 0;
    overlayScreenStrings.strings2[1][1] = 0;
    overlayScreenStrings.strings2[1][2] = 0;
    overlayScreenStrings.strings2[1][3] = 0;
    overlayScreenStrings.strings2[1][4] = 0;
    overlayScreenStrings.strings2[1][5] = 0;
    
    overlayScreenStrings.strings2[2][0] = 0;
    overlayScreenStrings.strings2[2][1] = 0;
    overlayScreenStrings.strings2[2][2] = 0;
    overlayScreenStrings.strings2[2][3] = 0;
    overlayScreenStrings.strings2[2][4] = 0;
    overlayScreenStrings.strings2[2][5] = 0;
    
    overlayScreenStrings.unk_6F[0] = 0;
    overlayScreenStrings.unk_6F[1] = 0;
    overlayScreenStrings.unk_6F[2] = 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC518);

// race results load callback
void func_800CC518(void) {

    func_800593EC();
    
    if (checkDailyEventBit(HORSE_RACE)) {
        removeKeyItem(0xE);
    } else {
        removeKeyItem(0xF);
    }
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(HORSE_RACE_RESULTS);
    
    overlayScreenStrings.gameVariableStringsIndices[0] = D_8018A72C[0] - 1;
    overlayScreenStrings.gameVariableStringsIndices[1] = D_8018A72C[1] - 1;
    overlayScreenStrings.gameVariableStringsIndices[2] = D_8018A72C[2] - 1;
    overlayScreenStrings.gameVariableStringsIndices[3] = D_8018A72C[3] - 1;
    overlayScreenStrings.gameVariableStringsIndices[4] = D_8018A72C[4] - 1;
    overlayScreenStrings.gameVariableStringsIndices[5] = D_8018A72C[5] - 1;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CC6F0);

// load race results sprites
void func_800CC6F0(void) {

    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u8*)0x8028F100, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 3, overlayScreenStrings.unk_6C);
    
    dmaSprite(0x83, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, 0);
    
    dmaSprite(0x84, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 4, 1);
    
    dmaSprite(0x85, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (u16*)0x8027E700, (u8*)0x8027EB00, NULL, 0, 0);
    setSpriteViewSpacePosition(0x85, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 4, 1);
    
    setOverlayScreenSprites(2, 0x97, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -153.0f, 256.0f, 10);
    
    D_80205204 += adjustValue(D_80205204, 
          overlayScreenStrings.unk_0[overlayScreenStrings.gameVariableStringsIndices[0]] * overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenStrings.gameVariableStringsIndices[0]], 
          9999);
    
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setOverlayScreenSprites(3, 0x9B, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -48.0f, -137.0f, 256.0f, 10);
    dmaOverlayScreenSprites(3, overlayScreenStrings.unk_0[overlayScreenStrings.gameVariableStringsIndices[0]] * overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenStrings.gameVariableStringsIndices[0]], 3, 3);
    setOverlayScreenSpritesRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setOverlayScreenSprites(8, 0xA5, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)0x80253B00, (u16*)0x80254A00, (AnimationFrameMetadata*)0x80254E00, (u32*)0x80254F00, 0, 1, 0, -24.0f, -112.0f, 256.0f, 10);
    dmaOverlayScreenSprites(8, overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenStrings.gameVariableStringsIndices[0]], 1, 3);
    setOverlayScreenSpritesRGBA(8, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setOverlayScreenSprites(9, 0xA7, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, (void*)0x8026F000, (void*)0x8027E200, (void*)0x8027E700, (void*)0x8027EB00, 0, 6, 0, 40.0f, -28.0f, 256.0f, 8);
    dmaOverlayScreenSprites(9, overlayScreenStrings.unk_0[overlayScreenStrings.gameVariableStringsIndices[0]], 2, 3);
    setOverlayScreenSpritesRGBA(9, 0xFF, 0xFF, 0xFF, 0xFF);
    
    func_800CCFC8();
    func_800CD628();
    
    if (overlayScreenStrings.strings2[overlayScreenStrings.unk_6C][overlayScreenStrings.gameVariableStringsIndices[0]] && !(checkHaveKeyItem(0xD))) {
        acquireKeyItem(0xD);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CCFC8);

// race results initialization
void func_800CCFC8(void) {

    setGameVariableString(0x2A, overlayScreenStrings.gameVariableStrings[overlayScreenStrings.gameVariableStringsIndices[0]], 6);
    setGameVariableString(0x2B, overlayScreenStrings.gameVariableStrings[overlayScreenStrings.gameVariableStringsIndices[1]], 6);
    setGameVariableString(0x2C, overlayScreenStrings.gameVariableStrings[overlayScreenStrings.gameVariableStringsIndices[2]], 6);
    setGameVariableString(0x2D, overlayScreenStrings.gameVariableStrings[overlayScreenStrings.gameVariableStringsIndices[3]], 6);
    setGameVariableString(0x2E, overlayScreenStrings.gameVariableStrings[overlayScreenStrings.gameVariableStringsIndices[4]], 6);
    setGameVariableString(0x2F, overlayScreenStrings.gameVariableStrings[overlayScreenStrings.gameVariableStringsIndices[5]], 6);
    
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -32.0f, 32.0f, 0.0f);
    func_8003F5D0(0, 0xA, 2);
    func_8003F630(0, 0, 2);
    func_8003F464(0, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    func_8003FB4C(0, 1);
    initializeMessageBox(0, 0, 0x12, 0x80000);
    func_8003E77C(0, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -32.0f, 16.0f, 0.0f);
    func_8003F5D0(1, 0xA, 2);
    func_8003F630(1, 0, 2);
    func_8003F464(1, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    func_8003FB4C(1, 1);
    initializeMessageBox(1, 0, 0x13, 0x80000);
    func_8003E77C(1, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(2, -32.0f, 0.0f, 0.0f);
    func_8003F5D0(2, 0xA, 2);
    func_8003F630(2, 0, 2);
    func_8003F464(2, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    func_8003FB4C(2, 1);
    initializeMessageBox(2, 0, 0x14, 0x80000);
    func_8003E77C(2, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, 96.0f, 32.0f, 0.0f);
    func_8003F5D0(3, 0xA, 2);
    func_8003F630(3, 0, 2);
    func_8003F464(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    func_8003FB4C(3, 1);
    initializeMessageBox(3, 0, 0x15, 0x80000);
    func_8003E77C(3, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, 96.0f, 16.0f, 0.0f);
    func_8003F5D0(4, 0xA, 2);
    func_8003F630(4, 0, 2);
    func_8003F464(4, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    func_8003FB4C(4, 1);
    initializeMessageBox(4, 0, 0x16, 0x80000);
    func_8003E77C(4, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(5, (u8*)MESSAGE_BOX_6_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(5, 96.0f, 0.0f, 0.0f);
    func_8003F5D0(5, 0xA, 2);
    func_8003F630(5, 0, 2);
    func_8003F464(5, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    func_8003F360(5, -4, 2);
    setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
    setMessageBoxButtonMask(5, BUTTON_A);
    func_8003FB4C(5, 1);
    initializeMessageBox(5, 0, 0x17, 0x80000);
    func_8003E77C(5, 255, 255, 255, 255);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD628);

void func_800CD628(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD690);

void func_800CD690(void) {

    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);

    func_8003DD14(0);
    func_8003DD14(1);
    func_8003DD14(2);
    func_8003DD14(3);
    func_8003DD14(4);
    func_8003DD14(5);

    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);

    func_80053088();

}

// INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801220CC);

// race strings
static const u8 D_801220CC[19][6] = {
    { 0xA7, 0xCF, 0xBB, 0xCC, 0xBE, 0xFF }, 
    { 0xB4, 0xBF, 0xC3, 0xC9, 0xC2, 0xFF }, 
    { 0xB4, 0xC3, 0xC8, 0xC1, 0xBF, 0xC8 }, 
    { 0xA2, 0xCC, 0xC9, 0xBD, 0xBD, 0xC9 }, 
    { 0xA7, 0xCC, 0xBF, 0xBF, 0xC8, 0xFF }, 
    { 0xA5, 0xC8, 0xC9, 0xC5, 0xC3, 0xFF }, 
    { 0xB0, 0xBB, 0xCD, 0xCD, 0xC3, 0xC9 }, 
    { 0xB0, 0xC3, 0xC5, 0xC6, 0xCD, 0xFF }, 
    { 0xB4, 0xBB, 0xCC, 0xBB, 0xBC, 0xBB }, 
    { 0xB4, 0xBB, 0xCE, 0xBB, 0xC7, 0xC3 }, 
    { 0xB5, 0xC7, 0xBF, 0xCD, 0xC2, 0xCF }, 
    { 0xAD, 0xBF, 0xC6, 0xC9, 0xBE, 0xD3 }, 
    { 0xB3, 0xCE, 0xBB, 0xCC, 0xE5, 0xA3 }, 
    { 0xAD, 0xC9, 0xD4, 0xD4, 0xFF, 0xFF }, 
    { 0xB3, 0xC2, 0xC3, 0xC9, 0xC5, 0xBB }, 
    { 0xB9, 0xBB, 0xCD, 0xCF, 0xC7, 0xBB }, 
    { 0xB3, 0xBB, 0xC5, 0xCF, 0xCC, 0xBB }, 
    { 0xB0, 0xC9, 0xD3, 0xBB, 0xC8, 0xC9 }, 
    { 0xAF, 0xC4, 0xBB, 0xC5, 0xC3, 0xFF },
};

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_80122140);

static const u8 D_80122140[19][6] = {
    { 0xB4, 0xBB, 0xCC, 0xC9, 0xFF, 0xFF }, 
    { 0xB7, 0xC9, 0xC8, 0xBE, 0xBF, 0xCC }, 
    { 0xB0, 0xC9, 0xBD, 0xC2, 0xC3, 0xFF }, 
    { 0xAD, 0xBB, 0xCC, 0xC9, 0xC8, 0xFF }, 
    { 0xAA, 0xC9, 0xC2, 0xC3, 0xFF, 0xFF }, 
    { 0xAD, 0xC3, 0xC6, 0xC5, 0xE5, 0xA3 }, 
    { 0xAD, 0xBB, 0xCA, 0xC6, 0xBF, 0xFF }, 
    { 0xA2, 0xC3, 0xCD, 0xBD, 0xC2, 0xFF }, 
    { 0xA3, 0xC2, 0xC9, 0xBD, 0xC9, 0xFF }, 
    { 0xA9, 0xBD, 0xC2, 0xC3, 0xC1, 0xC9 }, 
    { 0xA1, 0xCA, 0xCA, 0xC6, 0xBF, 0xFF }, 
    { 0xA1, 0xC6, 0xC7, 0xC9, 0xC8, 0xBE }, 
    { 0xA3, 0xC9, 0xBD, 0xBD, 0xC9, 0xFF }, 
    { 0xB0, 0xC3, 0xCD, 0xCE, 0xBB, 0xFF }, 
    { 0xAB, 0xCF, 0xCC, 0xC3, 0xFF, 0xFF }, 
    { 0xAB, 0xBB, 0xCC, 0xC3, 0xC8, 0xFF }, 
    { 0xAD, 0xBB, 0xC8, 0xC4, 0xCF, 0xFF }, 
    { 0xAB, 0xC9, 0xCD, 0xC2, 0xC3, 0xFF }, 
    { 0xB5, 0xC1, 0xCF, 0xC3, 0xCD, 0xCF } 
};

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD750);

// race results callback
void func_800CD750(void) {
    
    u16 temp = 0;

    switch (overlayScreenTable.screenState) {
        
        case 4:
            break;
        
        case 0:
            func_800CC6F0();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (func_8002CBF8(0x82)) {
                overlayScreenTable.screenState = 4;
            }
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            setDailyEventBit(0x40);
            setEntrance(0x6C);
            setPlayerAction(0, 0);
            setMainLoopCallbackFunctionIndex(MAP_LOAD);
            break;
        }
    
        if (overlayScreenTable.screenState >= 4 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && temp == 0) {
            
            updateSpriteAlpha(0x81, 0, 24);
            updateSpriteAlpha(0x82, 0, 24);
            updateSpriteAlpha(0x83, 0, 24);
            updateSpriteAlpha(0x84, 0, 24);
            updateSpriteAlpha(0x85, 0, 24);
            func_8003DD14(0);
            func_8003DD14(1);
            func_8003DD14(2);
            func_8003DD14(3);
            func_8003DD14(4);
            func_8003DD14(5);
            deactivateOverlayScreenSprites(2);
            deactivateOverlayScreenSprites(3);
            deactivateOverlayScreenSprites(8);
            deactivateOverlayScreenSprites(9);
            func_80053088();
            
            overlayScreenTable.screenState = 2;
            
            playSfx(1);
            
        }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CD928);

void func_800CD928(void) {
    
    func_800593EC();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(HORSE_RACE_GIFTS);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CDA6C);

void func_800CDA6C(void) {

    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    
    // horse or dog race
    if (checkDailyEventBit(HORSE_RACE)) {
        startSpriteAnimation(0x82, 2, 0);
    } else {
        startSpriteAnimation(0x82, 5, 0);
    }
    
    // medal count display
    dmaSprite(0x83, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, 0);
    
    // medal count display 2
    dmaSprite(0x84, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 4, 1);
    
    setOverlayScreenSprites(2, 0x97, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 0x80253B00, 0x80254A00, 0x80254E00, 0x80254F00, 0, 1, 0, -24.0f, 6.0f, 256.0f, 0xA);
    
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 255, 255, 255, 255);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8023B400, NULL, (u16*)0x8023CC00, (AnimationFrameMetadata*)0x8023CE00, (u32*)0x8023D200, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // fade in
    func_800CDFB0();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CDFB0);

void func_800CDFB0(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE008);

void func_800CE008(void) {
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    deactivateOverlayScreenSprites(2);
}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801221CC);

static const u16 D_801221CC[6] = { 3000, 1000, 500, 0, 0, 0 };

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE068);

// race gifts callback
void func_800CE068(void) {

    u16 buff[4];
    u16 buff2[4];

    u16* ptr = D_801221CC;

    bool set;

    memcpy(buff, ptr, 6);
    memcpy(buff2, ptr, 6);

    set = FALSE;
    
    switch (overlayScreenTable.screenState) {

        case 0:
            func_800CDA6C();
            overlayScreenTable.screenState = 1;
            break;

        case 1:
            if (func_8002CBF8(0x82)) {
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                func_8003F360(0, -4, 0);
                initializeDialogueSession(0, 0x3E, 5, 0x40);
                overlayScreenTable.screenState = 5;
            }
            break;

        case 5:    
            
            if (func_80043A88()) {
            
                if (!(func_80043C6C(0))) {
                    
                    dialogues[0].sessionManager.flags &= ~0x40;
                    func_80043AD8(0);
                    setSpriteViewSpacePosition(CURSOR_HAND, -104.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 16), 32.0f);
                    startSpriteAnimation(CURSOR_HAND, 3, 0);
                    overlayScreenTable.screenState = 9;
                    
                } else {
                    initializeMessageBox(0, 4, 15, 0);
                    overlayScreenTable.screenState = 3;
                }
                
            }

            break;

        case 3:
            
            if (func_8003F0DC()) {
                func_8003F130(0);
                resetAnimationState(CURSOR_HAND);
                updateSpriteAlpha(0x81, 0, 24);
                updateSpriteAlpha(0x82, 0, 24);
                updateSpriteAlpha(0x83, 0, 24);
                updateSpriteAlpha(0x84, 0, 24);
                deactivateOverlayScreenSprites(2);
                overlayScreenTable.screenState = 2;
            }
            
            break;

        case 4:
            if (func_8003F0DC()) {
                overlayScreenTable.screenState = 9;
            }
            break;
        
        case 2:
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 6;
            }
            break;
        
        case 6:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();
            break;

        case 7:
            
            if (func_80043A88()) {
                
                if (!(func_80043C6C(0))) {
                    
                    resetAnimationState(0x86);
                    overlayScreenTable.screenState = 8;
                    goto label3;
                    
                } else {
                    func_80043AD8(0);
                    setSpriteViewSpacePosition(CURSOR_HAND, -104.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 16), 32.0f);
                    startSpriteAnimation(CURSOR_HAND, 3, 0);
                    overlayScreenTable.screenState = 9;
                }
                
            }
            
            break;

        case 8:
            
            if (checkDailyEventBit(HORSE_RACE)) {
                
                if (adjustValue(D_80205204, -buff[overlayScreenTable.cellIndex], 9999) == -buff[overlayScreenTable.cellIndex]) {
                    D_80205204 += adjustValue(D_80205204, -buff[overlayScreenTable.cellIndex], 9999);
                    initializeMessageBox(0, 4, 13, 0);
                    overlayScreenTable.screenState = 3;
                    func_800CE7A8(overlayScreenTable.cellIndex);
                } else {
                    initializeMessageBox(0, 4, 14, 0);
                    overlayScreenTable.screenState = 3;
                }
                
            } else {

                if (adjustValue(D_80205204, -buff2[overlayScreenTable.cellIndex ], 9999) == -buff2[overlayScreenTable.cellIndex]) {
                    D_80205204 += adjustValue(D_80205204, -buff2[overlayScreenTable.cellIndex], 9999);
                    initializeMessageBox(0, 4, 13, 0);
                    overlayScreenTable.screenState = 3;
                    func_800CE894(overlayScreenTable.cellIndex);
                } else {
                    initializeMessageBox(0, 4, 14, 0);
                    overlayScreenTable.screenState = 3;
                }
            
            }
            
            break;

        case 9:
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex) {
                        overlayScreenTable.cellIndex--;
                        playSfx(2);
                        setSpriteViewSpacePosition(CURSOR_HAND, -104.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 16), 32.0f);
                    }
                    
                    set = TRUE;
                    
                }
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                if (!set) {
                    if (overlayScreenTable.cellIndex != 2) {
                        overlayScreenTable.cellIndex++;
                        playSfx(2);
                        setSpriteViewSpacePosition(CURSOR_HAND, -104.0f, (f32)(-(overlayScreenTable.cellIndex * 16) + 16), 32.0f);
                    }
                    set = TRUE;
                }
            }
        
            if ((checkButtonPressed(CONTROLLER_1, BUTTON_A))) {

                if (!set) {
                    
                    if (checkDailyEventBit(HORSE_RACE)) {
                        
                        if (!func_800CE714(overlayScreenTable.cellIndex)) {
                            goto label1;
                        } else {
                            goto label2;
                        }
                        
                    }
                    
                    if (!(func_800CE828(overlayScreenTable.cellIndex))) {
label1:
                        initializeDialogueSession(0, 0x3E, 0xB, 0x40);
                        overlayScreenTable.screenState = 7;
label3:
                        playSfx(0);
                        set = TRUE;
                        
                    } else {
label2:
                        initializeMessageBox(0, 4, 0x24, 0);
                        overlayScreenTable.screenState = 4;
                        set = TRUE;
                    }
                    
                }
                
            }
            
            break;
                
    }

    if (overlayScreenTable.screenState >= 9 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(CURSOR_HAND);
        updateSpriteAlpha(0x81, 0, 24);
        updateSpriteAlpha(0x82, 0, 24);
        updateSpriteAlpha(0x83, 0, 24);
        updateSpriteAlpha(0x84, 0, 24);
        deactivateOverlayScreenSprites(2);
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE714);

bool func_800CE714(u8 arg0) {

    bool result = FALSE;
    
    switch (arg0) {
        
        case 0:
            if (checkLifeEventBit(0x56)) {
                result = TRUE;
            }
            break;
        case 1:
            if (checkLifeEventBit(0x58)) {
                result = TRUE;
            }
            break;
        case 2:
            if (checkHaveKeyItem(0x15)) {
                result = TRUE;
            }
            break;
        
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE7A8);

void func_800CE7A8(u8 arg0) {

    switch (arg0) {                              
        case 0:
            setLifeEventBit(0x56);
            adjustHorseAffection(10);
            break;
        case 1:
            setLifeEventBit(0x58);
            break;
        case 2:
            acquireKeyItem(STAMINA_CARROT);
            break;
        }
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE828);

bool func_800CE828(u8 arg0) {

    bool result = FALSE;

    switch (arg0) {
        case 0:
            if (checkLifeEventBit(0x57)) {
                result = TRUE;
            }
            break;
        case 1:
            if (checkLifeEventBit(0x59)) {
                result = TRUE;
            }
            break;
        case 2:
            break;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE894);

void func_800CE894(u8 arg0) {

    switch (arg0) {                              
        case 0:
            setLifeEventBit(0x57);
            adjustDogAffection(10);
            return;
        case 1:
            setLifeEventBit(0x59);
            return;
        case 2:
            gLumber += adjustValue(gLumber, 500, MAX_LUMBER);
            return;
        }
    
}

/* end horse/dog race functions */


/* lottery/raffle */ 

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CE930);

void func_800CE930(void) {
    
    func_800593EC();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x802DE5C0, NULL, (u16*)0x802E0BC0, (AnimationFrameMetadata*)0x802E0CC0, (u8*)0x802E0DC0, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    overlayScreenTable.cellIndex = 0;
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(LOTTERY);
    
}

// winter lottery

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEA74);

void func_800CEA74(void) {
    
    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x80289900, NULL, (u16*)0x8028EF00, (u16*)0x8028F000, (u32*)0x8028F100, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 3, 0);
    
    dmaSprite(0x83, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)0x8026F000, NULL, (u16*)0x8027E200, (AnimationFrameMetadata*)0x8027E700, (u32*)0x8027EB00, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteRenderingLayer(0x83, 2);
    setBilinearFiltering(0x83, 1);
    
    if (gBaseMapIndex == FLOWER_SHOP) {
        startSpriteAnimation(0x83, 0, 0);
    } else {
        startSpriteAnimation(0x83, 1, 0);
    }
    
    func_800CED60();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CED60);

void func_800CED60(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEDA8);

void func_800CEDA8(void) {
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CEDF0);

// lottery callback
void func_800CEDF0(void) {

    switch (overlayScreenTable.screenState) {

        case 0:
            func_800CEA74();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            
            if (func_8002CBF8(0x82)) {
                
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                func_8003F360(0, -4, 0);
                
                if (gBaseMapIndex == 0x32) {
                    initializeDialogueSession(0, 0x3E, 4, 0x40);
                } else {
                    initializeDialogueSession(0, 0x3E, 0xA, 0x40);
                }
                
                overlayScreenTable.screenState = 4;

            }
            
            break;
        
        case 4:

            if (func_80043A88()) {
                
                if (!(func_80043C6C(0))) {
                    
                    dialogues[0].sessionManager.flags &= ~0x40;
                    
                    func_80043AD8(0);
                    overlayScreenTable.screenState = 6;
                    
                    playSfx(0x20);
                    
                    if (gBaseMapIndex == 0x32) {
                        flowerShopPoints += adjustValue(flowerShopPoints, -10, 100);
                    } else {
                        bakeryCardPoints += adjustValue(bakeryCardPoints, -10, 100);
                    }
                    

                } else {
                    
                    if (gBaseMapIndex == 0x32) {
                        initializeMessageBox(0, 4, 0x3D, 0);
                    } else {
                        initializeMessageBox(0, 4, 0x45, 0);
                    }
                
                    overlayScreenTable.screenState = 3;

                }
                
            
            }
            
            break;
        
        case 3:

            // check if a message box has flag 4 set
            if (func_8003F0DC()) {
                
                func_8003F130(0);
                
                resetAnimationState(CURSOR_HAND);
                
                updateSpriteAlpha(0x81, 0, 24);
                updateSpriteAlpha(0x82, 0, 24);
                updateSpriteAlpha(0x83, 0, 24);
                
                overlayScreenTable.screenState = 2;

            }
            
            break;
        
        case 2:
        
            if (func_8002CBF8(0x82)) {
                func_800B2CE0();
                overlayScreenTable.screenState = 5;
            }

            break;
        
        case 5:
            deactivateSprite(0x80);
            func_80059368();
            func_80059300();

            break;
        
        case 6:
            
            if (gAudioSequences[1].flags == 0) {

                if (gBaseMapIndex == FLOWER_SHOP) {

                    switch (getRandomNumberInRange(0, 4)) {

                        case 0:     
                            
                            if (!(func_800CF34C(0))) {
                                initializeMessageBox(0, 4, 0x38, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF3C4(0);
                            } else {
                                initializeMessageBox(0, 4, 0x3C, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF3C4(4);
                            }
                            
                            break;
                        
                    case 1:               
                        
                        if (!(func_800CF34C(1))) {
                            initializeMessageBox(0, 4, 0x39, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF3C4(1);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF3C4(4);
                        }
                        
                        break;
                        
                    case 2:                             
                        
                        if (!(func_800CF34C(2))) {
                            initializeMessageBox(0, 4, 0x3A, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF3C4(2);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF3C4(4);
                        }
                        
                        break;
                        
                    case 3: 
                        
                        if (!(func_800CF34C(3))) {
                            initializeMessageBox(0, 4, 0x3B, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF3C4(3);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF3C4(4);
                        }
                        
                        break;

                    case 4:   
                        initializeMessageBox(0, 4, 0x3C, 0);
                        overlayScreenTable.screenState = 3;
                        func_800CF3C4(4);
                        
                        break;                        
                    
                    }
                    
                } else {
                    
                    switch (getRandomNumberInRange(0, 4)) {      
                        
                        case 0:                             
                            
                            if (!(func_800CF498(0))) {
                                initializeMessageBox(0, 4, 0x40, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(0);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(4);
                            }
                            
                            break;
                        
                        case 1:                             
                            
                            if (!(func_800CF498(1))) {
                                initializeMessageBox(0, 4, 0x41, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(1);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(4);
                            }
                        
                            break;
                        
                        case 2:                             
                            
                            if (!(func_800CF498(2))) {
                                initializeMessageBox(0, 4, 0x42, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(2);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(4);
                            }
                            
                            break;
                        
                        case 3:            
                            
                            if (!(func_800CF498(3))) {
                                initializeMessageBox(0, 4, 0x43, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(3);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                func_800CF544(4);
                            }
                        
                            break;
                        
                        case 4:                             
                            initializeMessageBox(0, 4, 0x44, 0);
                            overlayScreenTable.screenState = 3;
                            func_800CF544(4);

                            break;
                            
                        }
                }
                
            }
            
            break;
        
        default:
            break;
   
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF34C);

bool func_800CF34C(u8 arg0) {

    bool result = FALSE;
    
    switch (arg0) {
        case 3:
        case 4:
            break;
        case 0:
            if (checkLifeEventBit(0x5A)) {
                result = TRUE;
            }
            break;
        case 1:
            if (checkHaveKeyItem(FLOWER_BATH_CRYSTALS)) {
                result = TRUE;
            }
            break;
        case 2:
            if (checkLifeEventBit(0x5B)) {
                result = TRUE;
            }
            break;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF3C4);

void func_800CF3C4(u8 arg0) {

    switch (arg0) {
        case 0:
            setLifeEventBit(0x5A);
            break;
        case 1:
            acquireKeyItem(0x14);
            break;
        case 2:
            setLifeEventBit(0x5B);
            break;
        case 3:
            func_80065BCC(0x12);
            D_80205636 += adjustValue(D_80205636, 1, 20);
            break;
        case 4:
            func_80065BCC(0x11);
            D_801C3F70 += adjustValue(D_801C3F70, 1, 20);
        default:
            break;
    }
} 

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF498);

bool func_800CF498(u8 arg0) {

    bool result = FALSE;
    
    switch (arg0) {

        case 0:
            if (checkLifeEventBit(HAVE_TABLECLOTH)) {
                result = TRUE;
            }
            break;
 
        case 1:
             if (checkLifeEventBit(0x5C)) {
                 result = TRUE;
             }
            break;
        
        case 2:
             if (checkLifeEventBit(0x5D)) {
                 result = TRUE;
             }
            break;
        case 3:
             if (!checkHaveTool(EMPTY_BOTTLE)) {
                 result = TRUE;
             }
            break;
        case 4:
            break;
    }

    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", func_800CF544);

void func_800CF544(u8 arg0) {

    switch (arg0) {
        
        case 0:
            
            setLifeEventBit(0x17);
            
            if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                npcAffection[MARIA] += adjustValue(npcAffection[MARIA], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                npcAffection[POPURI] += adjustValue(npcAffection[POPURI], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                npcAffection[ELLI] += adjustValue(npcAffection[ELLI], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                npcAffection[ANN] += adjustValue(npcAffection[ANN], 10, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                npcAffection[4] += adjustValue(npcAffection[4], 10, MAX_AFFECTION);
            }
            
            break;

        case 1:
            
            setLifeEventBit(0x5C);
            
            if (checkLifeEventBit(MARRIED) && gWife == MARIA) {
                npcAffection[MARIA] += adjustValue(npcAffection[MARIA], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == POPURI) {
                npcAffection[POPURI] += adjustValue(npcAffection[POPURI], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ELLI) {
                npcAffection[ELLI] += adjustValue(npcAffection[ELLI], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == ANN) {
                npcAffection[ANN] += adjustValue(npcAffection[ANN], 8, MAX_AFFECTION);
            }
            if (checkLifeEventBit(MARRIED) && gWife == KAREN) {
                npcAffection[4] += adjustValue(npcAffection[4], 8, MAX_AFFECTION);
            }
            
            break;

        case 2:
            setLifeEventBit(0x5D);
            break;
        
        case 3:
            
            if (checkHaveTool(EMPTY_BOTTLE)) {
                gPlayer.bottleContents = 7;
            } else {
                gPlayer.heldItem = COOKIES;
            }
            break;
        
        case 4:
            gPlayer.heldItem = COOKIES;
            break;
    
    }
    
}