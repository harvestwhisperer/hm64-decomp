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
#include "system/sprite.h"

#include "game/animals.h"
#include "game/cutscenes.h"
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
RacingContext gRacingContext;
u32 D_80189858;

u8 D_80205640[10];
u8 D_80189108[5][7];
u32 D_80205204;
u8 D_80237420[31];

// shared bss
u8 D_801890E8[31];

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
void loadClockSprites(void);
void updateShopBuyAnimation(void);
void initializePauseScreenBackground(void);
void loadPauseScreenSprites(void);
void loadPauseScreenItemSprites(void);
void updatePauseScreenSelectionHighlight(void);
void fadeOutPauseScreenSprites(void);
void fadeInPauseScreenSprites(void);
f32 getPauseScreenItemXCoordinate(u8, u32);
f32 getPauseScreenItemYCoordinate(u8, u8);
void resetPauseScreenSpriteColors(void);
void swapPauseScreenItems(void);
void loadToolboxScreenSprites(void);
void loadToolboxToolSprites(void);
void updateToolboxSelectionHighlight(void);
void fadeInToolboxSprites(void);
void fadeOutToolboxSprites(void);
void resetToolboxSpriteColors(void);  
void swapToolboxTools(void);
void loadFreezerScreenSprites(void);
void loadFreezerItemSprites(void);
void updateFreezerSelectionHighlight(void);
void fadeInFreezerSprites(void);
void fadeOutFreezerSprites(void);
f32 getFreezerItemXCoordinate(u8, u8);                          
f32 getFreezerItemYCoordinate(u8, u8);
void resetFreezerSpriteColors(void);           
bool isFreezerItemSwappable(void);
void swapFreezerItems(void);
void loadCabinetScreenSprites(void);
void loadCabinetItemSprites(void);
void updateCabinetSelectionHighlight(void);
void fadeInCabinetSprites(void);
void fadeOutCabinetSprites(void);
f32 getCabinetItemXCoordinate(u8, u8);             
f32 getCabinetItemYCoordinate(u8, u8);    
void resetCabinetSpriteColors(void);
bool isCabinetItemSwappable(void);
void swapCabinetItems(void);
u8 getToolLevelForAnimation(u8 tool);
void loadHouseExtensionsScreenSprites(void);
void fadeInHouseExtensionsSelectionSprites(void);
void loadEstimateScreenSprites(void);
void fadeInEstimateSprites(void);
void fadeOutEstimateSprites(void);
void loadKitchenPictureScreenSprites(void);
void loadPowerNutTreeSprites(void);
void updatePowerNutCellAnimations(void);
u8 findPowerNutCellIndex(u8 arg0, u8 arg1);
inline void fadeInKitchenPictureSprites(void);
void fadeOutKitchenPictureSprites(void);
void loadCalendarScreenSprites(void);
void loadCalendarStickers(void);
void loadCalendarDateNumbers(u8, u8, u8);
void fadeInCalendarSprites(void);
void fadeOutCalendarSprites(void);
void loadRecipeBookScreenSprites(void);
void loadRecipeBookLabels(void);
void fadeInRecipeBookSprites(void);
void fadeOutRecipeBookSprites(void);
void loadAlbumScreenSprites(void);
void loadAlbumPhotos(void);
void fadeInAlbumSprites(void);
f32 getToolboxItemXCoordinate(u8, u32);
f32 getToolboxItemYCoordinate(u8, u8);
void loadNavigationArrows(void);
void showDownArrow(f32 x, f32 y);
void showUpArrow(f32 x, f32 y);
void showLeftArrow(f32 x, f32 y);      
void showRightArrow(f32 x, f32 y);  
void hideDownArrow(void);
void hideUpArrow(void);
void hideLeftArrow(void);
void hideRightArrow(void);
void loadDialogueArrowSprites(void);
void showDialogueDownArrow(f32 x, f32 y);
void showDialogueUpArrow(f32 x, f32 y);
void hideDialogueDownArrow(void);
void hideDialogueUpArrow(void);
void setItemDescriptionVariable(u8 arg0, u32 arg1);
void initializeRaceBettingMessageBoxes(void);
void fadeInRaceBettingSprites(void);
void loadRaceResultsScreenSprites(void);
void initializeRaceResultsMessageBoxes(void);
void fadeInRaceResultsSprites(void);
void loadRaceGiftsScreenSprites(void);
void fadeInRaceGiftsSprites(void);
void handleGetDogRacePrize(u8 arg0);
void handleGetHorseRacePrize(u8 arg0);
void loadLotteryScreenSprites(void);
void fadeInLotterySprites(void);
bool checkAvailableFlowerShopPrize(u8 arg0);
void handleGetFlowerShopPrize(u8 arg0);
bool checkAvailableBakeryPrize(u8 arg0);
void handleGetBakeryPrize(u8 arg0);
u32 checkRecipe(u16 bitIndex);


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", closeOverlayScreen);

// close overlay screen
// TODO: seems to be static inline calls (repeated deactivations)
void closeOverlayScreen(void) {

    u8 i, j, k;

    hideDialogueDownArrow();
    
    i = 0;
    
    hideDialogueUpArrow();
    hideDownArrow();
    hideUpArrow();
    hideRightArrow();
    hideLeftArrow();

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
 
//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadClockSprites);

void loadClockSprites(void) {

    // clock and background
    dmaSprite(CLOCK, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_CLOCK_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_CLOCK_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_CLOCK_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_CLOCK_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CLOCK, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(CLOCK, 1);
    setSpriteColor(CLOCK, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(CLOCK, 2);

    if (checkLifeEventBit(0x59)) {
        // cow skin background
        startSpriteAnimation(CLOCK, 6, 0);
    } else {
        startSpriteAnimation(CLOCK, 0, 0);
    }

    // minute hand
    dmaSprite(0x8C, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_CLOCK_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_CLOCK_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_CLOCK_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_CLOCK_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8C, -79.0f, 81.0f, 256.0f);
    setSpriteRotation(0x8C, 0.0f, 0.0f, (360 - (gMinutes * 6)));
    setBilinearFiltering(0x8C, 1);
    setSpriteColor(0x8C, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x8C, SPRITE_BLEND_ALPHA_MODULATED);
    startSpriteAnimation(0x8C, 4, 0);
    
    // hour hand
    dmaSprite(0x8D, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_CLOCK_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_CLOCK_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_CLOCK_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_CLOCK_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8D, -80.0f, 80.0f, 256.0f);
    setSpriteRotation(0x8D, 0.0f, 0.0f, ((360 - (gHour * 30)) - (gMinutes * 0.5)));
    setBilinearFiltering(0x8D, 1);
    setSpriteColor(0x8D, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x8D, SPRITE_BLEND_ALPHA_MODULATED);
    startSpriteAnimation(0x8D, 5, 0);
    
    // season name
    dmaSprite(0x8B, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_CLOCK_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_CLOCK_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_CLOCK_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_CLOCK_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8B, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x8B, 1);
    setSpriteColor(0x8B, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x8B, SPRITE_BLEND_ALPHA_MODULATED);
    startSpriteAnimation(0x8B, 1, gSeason - 1);
    
    // day of week
    dmaSprite(0x8A, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_CLOCK_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_CLOCK_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_CLOCK_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_CLOCK_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8A, 0.0f, 0.0f, 256.0f);
    setBilinearFiltering(0x8A, 1);
    setSpriteColor(0x8A, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x8A, SPRITE_BLEND_ALPHA_MODULATED);
    startSpriteAnimation(0x8A, 3, gDayOfWeek);
    setOverlayScreenSprites(0, 0x8F, (u32)&_timeUiTextureSegmentRomStart, (u32)&_timeUiTextureSegmentRomEnd, (u32)&_timeUiAssetsIndexSegmentRomStart, (u32)&_timeUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_CLOCK_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_CLOCK_PALETTE_BUFFER, (void*)OVERLAY_SCREEN_CLOCK_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_CLOCK_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 2, 0, 0.0f, 0.0f, 256.0f, 8);
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadMoneySprites);

// unused or inline
void loadMoneySprites(f32 x, f32 y, f32 z, u16 renderingLayer, u32 arg4) {

    // background and "g"
    dmaSprite(0x8E, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)GOLD_UI_TEXTURE_BUFFER, NULL, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8E, x, y, z);
    setBilinearFiltering(0x8E, 1);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x8E, renderingLayer);
    startSpriteAnimation(0x8E, 0, 0);

    setOverlayScreenSprites(1, 0x91, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 
        (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 
        0, 1, 0, 
        x, y, z + 64.0f, 
        10);

    dmaOverlayScreenSprites(1, arg4, 5, 3);
    setOverlayScreenSpritesRGBA(1, 255, 255, 255, 255);

}


/* TV functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadTVCButtonIcons);

void loadTVCButtonIcons(void) {

    dmaSprite(0x86, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x86, 1);
    setSpriteColor(0x86, 255, 255, 255, 255);
    startSpriteAnimation(0x86, 0, 0);
    
    dmaSprite(0x82, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    startSpriteAnimation(0x82, 0, 1);
    
    dmaSprite(0x83, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x83, 1);
    setSpriteColor(0x83, 255, 255, 255, 255);
    startSpriteAnimation(0x83, 0, 3);
    
    dmaSprite(0x84, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x84, 1);
    setSpriteColor(0x84, 255, 255, 255, 255);
    startSpriteAnimation(0x84, 0, 5);
    
    dmaSprite(0x85, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 80.0f, 32.0f, 256.0f);
    setBilinearFiltering(0x85, 1);
    setSpriteColor(0x85, 255, 255, 255, 255);
    startSpriteAnimation(0x85, 0, 7);
    
    updateTVCButtonsByChannel();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateTVCButtonsByChannel);

// update C buttons based on channel currently being watched
void updateTVCButtonsByChannel(void) {

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

    dmaSprite(0x8E, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)GOLD_UI_TEXTURE_BUFFER, 0, (u16*)GOLD_UI_PALETTE_BUFFER, (u32*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x8E, 0, 0, 256.0f);
    setBilinearFiltering(0x8E, TRUE);
    setSpriteColor(0x8E, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x8E, SPRITE_BLEND_ALPHA_MODULATED);
    startSpriteAnimation(0x8E, 0, 0);

    setOverlayScreenSprites(1, 0x91, 
        (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, 
        (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 
        0, 1, 0, 
        0.0f, 0.0f, 320.0f, 
        10
    );
    
    dmaOverlayScreenSprites(1, temp, 5, 3);
    setOverlayScreenSpritesRGBA(1, 255, 255, 255, 255);

}


/* shop functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadShopIcons);

void loadShopIcons(void) {
    
    u32 temp = gGold;

    setMoneySprites();
    
    setSpriteAlpha(0x8E, 0xFF);
    
    // base/neutral player
    dmaSprite(0x82, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)SHOP_ICONS_TEXTURE_BUFFER, NULL, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, -96.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x82, TRUE);
    setSpriteColor(0x82, 255, 255, 255, 255);
    startSpriteAnimation(0x82, 0, 0);
    
    // head shaking no
    dmaSprite(0x83, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)SHOP_ICONS_TEXTURE_BUFFER, NULL, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, -56.0f, 64.0f, 256.0f);
    setBilinearFiltering(0x83, TRUE);
    setSpriteColor(0x83, 255, 255, 255, 255);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)SHOP_ICONS_TEXTURE_BUFFER, NULL, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(0x84, TRUE);
    setSpriteColor(0x84, 255, 255, 255, 255);
    
    // quantity arrows
    dmaSprite(0x85, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)SHOP_ICONS_TEXTURE_BUFFER, NULL, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x85, TRUE);
    setSpriteColor(0x85, 255, 255, 255, 255);
    
    dmaSprite(0x86, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)SHOP_ICONS_TEXTURE_BUFFER, NULL, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, -12.0f, 13.0f, 256.0f);
    setBilinearFiltering(0x86, TRUE);
    setSpriteColor(0x86, 255, 255, 255, 255);
    
    // background
    dmaSprite(0x87, (u32)&_shopIconsTextureSegmentRomStart, (u32)&_shopIconsTextureSegmentRomEnd, (u32)&_shopIconsAssetsIndexSegmentRomStart, (u32)&_shopIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)SHOP_ICONS_TEXTURE_BUFFER, NULL, (u16*)SHOP_ICONS_PALETTE_BUFFER, (AnimationFrameMetadata*)SHOP_ICONS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)SHOP_ICONS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, -16.0f, 11.0f, 224.0f);
    setBilinearFiltering(0x87, TRUE);
    setSpriteColor(0x87, 255, 255, 255, 255);

    updateShopBuyAnimation();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateShopBuyAnimation);

void updateShopBuyAnimation(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateShopQuantityArrows);

void updateShopQuantityArrows(u8 arg0) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadDiarySelectScreen);

void loadDiarySelectScreen(void) {

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, 1);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 0, 0, 0, 0);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);
    
    if (loadGameScreenContext.showControllerPakScreen == FALSE) {
        
        // select a diary
        dmaSprite(0x82, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x82, 0, 0, 0, 0);
        setSpriteBlendMode(0x82, 3);
        setBilinearFiltering(0x82, 1);
        startSpriteAnimation(0x82, 0, 0);
        
        // DELETE button
        dmaSprite(0x83, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x83, 0, 0, 0, 0);
        setSpriteBlendMode(0x83, 3);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, 3, 0);
        
        // MOVE button
        dmaSprite(0x84, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0, 0, 0, 0);
        setSpriteBlendMode(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 1, 0);
        
        // diary book 1
        dmaSprite(0xA3, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA3, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xA3, 0, 0, 0, 0);
        setSpriteBlendMode(0xA3, 3);
        setBilinearFiltering(0xA3, 1);
        setSpritePaletteIndex(0xA3, 2);
        startSpriteAnimation(0xA3, 4, 0);
        
        // diary book
        dmaSprite(0xA4, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA4, 0.0f, -44.0f, 8.0f);
        setSpriteColor(0xA4, 0, 0, 0, 0);
        setSpriteBlendMode(0xA4, 3);
        setBilinearFiltering(0xA4, 1);
        setSpritePaletteIndex(0xA4, 3);
        startSpriteAnimation(0xA4, 4, 0);
        
        // diary book
        dmaSprite(0xA5, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA5, 0.0f, -88.0f, 8.0f);
        setSpriteColor(0xA5, 0, 0, 0, 0);
        setSpriteBlendMode(0xA5, 3);
        setBilinearFiltering(0xA5, 1);
        setSpritePaletteIndex(0xA5, 4);
        startSpriteAnimation(0xA5, 4, 0);
        
        // diary book
        dmaSprite(0xA6, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA6, 0.0f, -132.0f, 8.0f);
        setSpriteColor(0xA6, 0, 0, 0, 0);
        setSpriteBlendMode(0xA6, 3);
        setBilinearFiltering(0xA6, 1);
        setSpritePaletteIndex(0xA6, 5);
        startSpriteAnimation(0xA6, 4, 0);
        
        // numbers
        dmaSprite(0xA7, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA7, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA7, 0, 0, 0, 0);
        setSpriteBlendMode(0xA7, 3);
        setBilinearFiltering(0xA7, 1);
        startSpriteAnimation(0xA7, 5, 0);
        
        // numbers
        dmaSprite(0xA8, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA8, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA8, 0, 0, 0, 0);
        setSpriteBlendMode(0xA8, 3);
        setBilinearFiltering(0xA8, 1);
        startSpriteAnimation(0xA8, 5, 2);
        
        // numbers
        dmaSprite(0xA9, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA9, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA9, 0, 0, 0, 0);
        setSpriteBlendMode(0xA9, 3);
        setBilinearFiltering(0xA9, 1);
        startSpriteAnimation(0xA9, 5, 4);

        // numbers
        dmaSprite(0xAA, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAA, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xAA, 0, 0, 0, 0);
        setSpriteBlendMode(0xAA, 3);
        setBilinearFiltering(0xAA, 1);
        startSpriteAnimation(0xAA, 5, 6);
        
        // row background
        dmaSprite(0x85, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 4.0f);
        setSpriteColor(0x85, 0, 0, 0, 0);
        setSpriteBlendMode(0x85, 3);
        setBilinearFiltering(0x85, 1);
        setSpritePaletteIndex(0x85, 1);
        startSpriteAnimation(0x85, 2, 0);
        
        // row background
        dmaSprite(0x86, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x86, 0.0f, -44.0f, 4.0f);
        setSpriteColor(0x86, 0, 0, 0, 0);
        setSpriteBlendMode(0x86, 3);
        setBilinearFiltering(0x86, 1);
        setSpritePaletteIndex(0x86, 1);
        startSpriteAnimation(0x86, 2, 0);
        
        // row background
        dmaSprite(0x87, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x87, 0.0f, -88.0f, 4.0f);
        setSpriteColor(0x87, 0, 0, 0, 0);
        setSpriteBlendMode(0x87, 3);
        setBilinearFiltering(0x87, 1);
        setSpritePaletteIndex(0x87, 1);
        startSpriteAnimation(0x87, 2, 0);
        
        // row background
        dmaSprite(0x88, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x88, 0.0f, -132.0f, 4.0f);
        setSpriteColor(0x88, 0, 0, 0, 0);
        setSpriteBlendMode(0x88, 3);
        setBilinearFiltering(0x88, 1);
        setSpritePaletteIndex(0x88, 1);
        startSpriteAnimation(0x88, 2, 0);
        
        dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(CURSOR_HAND, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(CURSOR_HAND, 1);
        setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
        setSpriteBlendMode(CURSOR_HAND, 3);
        
        dmaSprite(0xB2, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xB2, 1);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0x80);
        setSpriteBlendMode(0xB2, SPRITE_BLEND_ALPHA_MODULATED);

        dmaSprite(0xAC, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAC, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xAC, 1);
        setSpriteColor(0xAC, 255, 255, 255, 255);
        setSpriteBlendMode(0xAC, 3);
        
        // arrows
        loadNavigationArrows();
        
    } else {
        
        // "data" book icon
        dmaSprite(0x82, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x82, 0, 0, 0, 0);
        setSpriteBlendMode(0x82, 3);
        setBilinearFiltering(0x82, 1);
        startSpriteAnimation(0x82, 7, 0);
        
        // DELETE button
        dmaSprite(0x83, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x83, 0, 0, 0, 0);
        setSpriteBlendMode(0x83, 3);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, 3, 0);
        
        // MOVE button
        dmaSprite(0x84, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 0, 0, 0, 0);
        setSpriteBlendMode(0x84, 3);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, 1, 0);
        
        // data book icons

        dmaSprite(0xA3, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA3, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xA3, 0, 0, 0, 0);
        setSpriteBlendMode(0xA3, 3);
        setBilinearFiltering(0xA3, 1);
        setSpritePaletteIndex(0xA3, 2);
        startSpriteAnimation(0xA3, 8, 0);
        
        dmaSprite(0xA4, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA4, 0.0f, -44.0f, 8.0f);
        setSpriteColor(0xA4, 0, 0, 0, 0);
        setSpriteBlendMode(0xA4, 3);
        setBilinearFiltering(0xA4, 1);
        setSpritePaletteIndex(0xA4, 3);
        startSpriteAnimation(0xA4, 8, 0);
        
        // numbers

        dmaSprite(0xA7, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA7, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA7, 0, 0, 0, 0);
        setSpriteBlendMode(0xA7, 3);
        setBilinearFiltering(0xA7, 1);
        startSpriteAnimation(0xA7, 5, 0);
        
        dmaSprite(0xA8, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xA8, 0.0f, 0.0f, 16.0f);
        setSpriteColor(0xA8, 0, 0, 0, 0);
        setSpriteBlendMode(0xA8, 3);
        setBilinearFiltering(0xA8, 1);
        startSpriteAnimation(0xA8, 5, 2);
        
        // extra rows for game pak
        dmaSprite(0x85, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 4.0f);
        setSpriteColor(0x85, 0, 0, 0, 0);
        setSpriteBlendMode(0x85, 3);
        setBilinearFiltering(0x85, 1);
        setSpritePaletteIndex(0x85, 1);
        startSpriteAnimation(0x85, 2, 0);
        
        dmaSprite(0x86, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x86, 0.0f, -44.0f, 4.0f);
        setSpriteColor(0x86, 0, 0, 0, 0);
        setSpriteBlendMode(0x86, 3);
        setBilinearFiltering(0x86, 1);
        setSpritePaletteIndex(0x86, 1);
        startSpriteAnimation(0x86, 2, 0);
        
        // "game will not start" message
        dmaSprite(0xAB, (u32)&_loadGameScreenTextureSegmentRomStart, (u32)&_loadGameScreenTextureSegmentRomEnd, (u32)&_loadGameScreenAssetsIndexSegmentRomStart, (u32)&_loadGameScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAB, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0xAB, 0, 0, 0, 0);
        setSpriteBlendMode(0xAB, 3);
        setBilinearFiltering(0xAB, 1);
        startSpriteAnimation(0xAB, 9, 0);
        
        dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(CURSOR_HAND, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(CURSOR_HAND, 1);
        setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
        setSpriteBlendMode(CURSOR_HAND, 3);
        
        dmaSprite(0xB2, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xB2, 1);
        setSpriteColor(0xB2, 0xFF, 0xFF, 0xFF, 0x80);
        setSpriteBlendMode(0xB2, SPRITE_BLEND_ALPHA_MODULATED);

        dmaSprite(0xAC, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAC, 0.0f, 0.0f, 32.0f);
        setBilinearFiltering(0xAC, 1);
        setSpriteColor(0xAC, 255, 255, 255, 255);
        setSpriteBlendMode(0xAC, 3);
        
        loadNavigationArrows();
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInLoadGameScreen);

void fadeInLoadGameScreen(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", setLoadGameScreenFullAlpha);

void setLoadGameScreenFullAlpha(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutLoadGameScreen);

void fadeOutLoadGameScreen(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", setDiaryCursorPosition);

void setDiaryCursorPosition(u8 arg0, u8 diaryHighlighted, u8 actionColumnHighlighted) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateDiaryHighlighted);

void updateDiaryHighlighted(u8 diaryHighlighted, u8 actionColumnHighlighted, bool arg2) {

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
            showDownArrow(64.0f, -108.0f);
        } else {
            hideDownArrow();
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
            showUpArrow(64.0f, 80.0f);
        } else {
            hideUpArrow();
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", animateDiarySelected);

void animateDiarySelected(u8 diaryHighlighted, u8 actionColumnHighlighted) {

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

// INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadFarmRankingsListScreen);

void loadFarmRankingsListScreen(void) {
    
    // cloud text backdrop
    dmaSprite(0x80, (u32)&_namingScreenBackgroundTextureSegmentRomStart, (u32)&_namingScreenBackgroundTextureSegmentRomEnd, (u32)&_namingScreenBackgroundAssetsIndexSegmentRomStart, (u32)&_namingScreenBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x80, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x80, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(0x80, 1);
    setSpriteColor(0x80, 0, 0, 0, 0);
    startSpriteAnimation(0x80, 0, 0);
    
    // "Farm ranking" and background
    dmaSprite(0x82, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0, 0, 0, 0);
    setSpriteBlendMode(0x82, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);
    
    // slots
    dmaSprite(0x83, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 40.0f, 8.0f);
    setSpriteColor(0x83, 0, 0, 0, 0);
    setSpriteBlendMode(0x83, 3);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, 8.0f, 8.0f);
    setSpriteColor(0x84, 0, 0, 0, 0);
    setSpriteBlendMode(0x84, 3);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 1, 1);
    
    dmaSprite(0x85, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, -24.0f, 8.0f);
    setSpriteColor(0x85, 0, 0, 0, 0);
    setSpriteBlendMode(0x85, 3);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 2);
    
    dmaSprite(0x86, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 0.0f, -56.0f, 8.0f);
    setSpriteColor(0x86, 0, 0, 0, 0);
    setSpriteBlendMode(0x86, 3);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 1, 3);
    
    dmaSprite(0x87, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, -88.0f, 8.0f);
    setSpriteColor(0x87, 0, 0, 0, 0);
    setSpriteBlendMode(0x87, 3);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 1, 4);

    // if slot has active game, load sprite

    if (gFarmRankingData.flags[0] & 1) {
        dmaSprite(0xAE, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAE, -4.0f, 40.0f, 16.0f);
        setSpriteColor(0xAE, 0, 0, 0, 0);
        setSpriteBlendMode(0xAE, 3);
        setBilinearFiltering(0xAE, 1);
        startSpriteAnimation(0xAE, 7, gFarmRankingData.seasons[0] - 1);
    }
    
    if (gFarmRankingData.flags[1] & 1) {
        dmaSprite(0xAF, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAF, -4.0f, 8.0f, 16.0f);
        setSpriteColor(0xAF, 0, 0, 0, 0);
        setSpriteBlendMode(0xAF, 3);
        setBilinearFiltering(0xAF, 1);
        startSpriteAnimation(0xAF, 7, gFarmRankingData.seasons[1] - 1);
    }
    
    if (gFarmRankingData.flags[2] & 1) {
        dmaSprite(0xB0, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB0, -4.0f, -24.0f, 16.0f);
        setSpriteColor(0xB0, 0, 0, 0, 0);
        setSpriteBlendMode(0xB0, 3);
        setBilinearFiltering(0xB0, 1);
        startSpriteAnimation(0xB0, 7, gFarmRankingData.seasons[2] - 1);
    }
    
    if (gFarmRankingData.flags[3] & 1) {
        dmaSprite(0xB1, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB1, -4.0f, -56.0f, 16.0f);
        setSpriteColor(0xB1, 0, 0, 0, 0);
        setSpriteBlendMode(0xB1, 3);
        setBilinearFiltering(0xB1, 1);
        startSpriteAnimation(0xB1, 7, gFarmRankingData.seasons[3] - 1);
    }
    
    if (gFarmRankingData.flags[4] & 1) {
        dmaSprite(0xB2, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, -4.0f, -88.0f, 16.0f);
        setSpriteColor(0xB2, 0, 0, 0, 0);
        setSpriteBlendMode(0xB2, 3);
        setBilinearFiltering(0xB2, 1);
        startSpriteAnimation(0xB2, 7, gFarmRankingData.seasons[4] - 1);
    }
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CURSOR_HAND, 0.0f, 0.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    setSpriteBlendMode(CURSOR_HAND, 3);
    
}

// INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadIndividualRankingScreen);

// individual ranking screen
void loadIndividualRankingScreen(u8 arg0) {

    // slot
    dmaSprite(0x82, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x82, 255, 255, 255, 255);
    setSpriteBlendMode(0x82, 3);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 1, arg0);
    
    // "Farm ranking" and background
    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x81, 255, 255, 255, 255);
    setSpriteBlendMode(0x81, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    // player icon
    dmaSprite(0x83, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 255, 255, 255, 255);
    setSpriteBlendMode(0x83, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 2, 0);

    if (gFarmRankingData.flags[arg0] & RANKING_FLAG_MARRIED) {
        
        dmaSprite(0x84, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x84, 255, 255, 255, 255);
        setSpriteBlendMode(0x84, 3);
        setBilinearFiltering(0x84, 1);
        // get frame based on wife
        startSpriteAnimation(0x84, 3, gFarmRankingData.wife[arg0]);
        
        dmaSprite(0xAE, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAE, 0.0f, 32.0f, 8.0f);
        setSpriteColor(0xAE, 255, 255, 255, 255);
        setSpriteBlendMode(0xAE, 3);
        setBilinearFiltering(0xAE, 1);
        
        if (gFarmRankingData.wifeAffection[arg0] == 0xFF) {
            startSpriteAnimation(0xAE, 6, 0xFE);
        } else {
            startSpriteAnimation(0xAE, 5, (gFarmRankingData.wifeAffection[arg0] * 9) / 255);
        }
        
    }

    if (gFarmRankingData.flags[arg0] & RANKING_FLAG_HAVE_BABY) {
        
        // baby icon
        dmaSprite(0x85, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x85, 255, 255, 255, 255);
        setSpriteBlendMode(0x85, 3);
        setBilinearFiltering(0x85, 1);
        startSpriteAnimation(0x85, 3, 5);
        
        // heart
        dmaSprite(0xAF, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xAF, 0.0f, 16.0f, 8.0f);
        setSpriteColor(0xAF, 255, 255, 255, 255);
        setSpriteBlendMode(0xAF, 3);
        setBilinearFiltering(0xAF, 1);

        if (gFarmRankingData.babyAffection[arg0] == 0xFF) {
            startSpriteAnimation(0xAF, 6, 0xFE);
        } else {
            startSpriteAnimation(0xAF, 5, (gFarmRankingData.babyAffection[arg0] * 9) / 255);
        }
    
    }

    if (gFarmRankingData.flags[arg0] & RANKING_FLAG_HAVE_HORSE) {
        
        // horse icon
        dmaSprite(0x86, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 8.0f);
        setSpriteColor(0x86, 255, 255, 255, 255);
        setSpriteBlendMode(0x86, 3);
        setBilinearFiltering(0x86, 1);
        startSpriteAnimation(0x86, 3, 6);
        
        // heart
        dmaSprite(0xB1, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB1, 0.0f, -16.0f, 8.0f);
        setSpriteColor(0xB1, 255, 255, 255, 255);
        setSpriteBlendMode(0xB1, 3);
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
    
    dmaSprite(0xB0, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xB0, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0xB0, 255, 255, 255, 255);
    setSpriteBlendMode(0xB0, 3);
    setBilinearFiltering(0xB0, 1);
    
    if (gFarmRankingData.dogAffection[arg0] == 0xFF) {
        startSpriteAnimation(0xB0, 6, 0xFE);
    } else {
        startSpriteAnimation(0xB0, 5, (gFarmRankingData.dogAffection[arg0] * 9) / 255);
    }

    if (gFarmRankingData.flags[arg0] & 1) {
        dmaSprite(0xB2, (u32)&_rankingsTextureSegmentRomStart, (u32)&_rankingsTextureSegmentRomEnd, (u32)&_rankingsAssetsIndexSegmentRomStart, (u32)&_rankingsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0xB2, -4.0f, 80.0f, 16.0f);
        setSpriteColor(0xB2, 255, 255, 255, 255);
        setSpriteBlendMode(0xB2, 3);
        setBilinearFiltering(0xB2, 1);
        startSpriteAnimation(0xB2, 7, gFarmRankingData.seasons[arg0] - 1);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInFarmRankingScreen);

void fadeInFarmRankingScreen(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutFarmRankingScreen);

void fadeOutFarmRankingScreen(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", setFarmRankingScreenFullAlpha);

void setFarmRankingScreenFullAlpha(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", setFarmRankingCursorPosition);

void setFarmRankingCursorPosition(u8 arg0) {

    if (arg0 != 0xFF) {
        setSpriteViewSpacePosition(CURSOR_HAND, farmRankingsCursorHandXPositions[arg0], farmRankingsCursorHandYPositions[arg0], 32.0f);
    }

    startSpriteAnimation(CURSOR_HAND, 3, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", setFarmRankingItemPalette);

void setFarmRankingItemPalette(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadEstimateScreenCallback);
 
void loadEstimateScreenCallback(void) {
    
    openOverlayScreen();

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0, 0, 0);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0); 
     
    initializeOverlayScreenTable();
    
    setMainLoopCallbackFunctionIndex(ESTIMATE);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadKitchenPictureCallback);

void loadKitchenPictureCallback(void) {

    openOverlayScreen();

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);

    overlayScreenTable.cellIndex = findPowerNutCellIndex(0, 0);
    overlayScreenTable.previousCellIndex = 0;
    overlayScreenTable.pageNumber = 0;
    overlayScreenTable.unk_3 = 0xFF;
    overlayScreenTable.screenState = 0;
    
    setMainLoopCallbackFunctionIndex(KITCHEN_PICTURE);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadHouseExtensionsSelectionCallback);

void loadHouseExtensionsSelectionCallback(void) {

    openOverlayScreen();

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CHECKERBOARD_BACKGROUND, 0.0f, 0.0f, 0.0f);
    setSpriteScale(CHECKERBOARD_BACKGROUND, 2.0f, 2.0f, 1.0f);
    setBilinearFiltering(CHECKERBOARD_BACKGROUND, TRUE);
    setSpriteColor(CHECKERBOARD_BACKGROUND, 255, 255, 255, 255);
    startSpriteAnimation(CHECKERBOARD_BACKGROUND, 0, 0);

    initializeOverlayScreenTable();
    
    setMainLoopCallbackFunctionIndex(HOME_EXTENSIONS_SELECT);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCalendarScreenCallback);

void loadCalendarScreenCallback(void) {

    openOverlayScreen();
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRecipeBookScreenCallback);

void loadRecipeBookScreenCallback(void) {
    
    openOverlayScreen();
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadAlbumScreenCallback);

void loadAlbumScreenCallback(void) {
    
    openOverlayScreen();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadPauseScreenCallback);

// pause screen load callback
void loadPauseScreenCallback(void) {
    openOverlayScreen();
    initializePauseScreenBackground();
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", initializePauseScreenBackground);

void initializePauseScreenBackground(void) {
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadPauseScreenSprites);

void loadPauseScreenSprites(void) {

    // update misc. game state
    func_8005CDCC();

    loadClockSprites();
    setMoneySprites();

    // tool/item slots
    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 0, 0);

    // tool/item slots
    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0x80, 0x80, 0x80, 0);
    setSpriteBlendMode(0x84, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    // key item slots
    dmaSprite(0x85, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x85, 0x80, 0x80, 0x80, 0);
    setSpriteBlendMode(0x85, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 1, 0);
    
    // tool and belongings labels
    dmaSprite(TOOL_AND_BELONGINGS_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(TOOL_AND_BELONGINGS_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(TOOL_AND_BELONGINGS_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(TOOL_AND_BELONGINGS_LABELS, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(TOOL_AND_BELONGINGS_LABELS, TRUE);
    startSpriteAnimation(TOOL_AND_BELONGINGS_LABELS, 5, 0);
    
    // highlighted square borders
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, -28.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x87, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 3);
    
    dmaSprite(0x88, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x88, 20.0f, 80.0f, 8.0f);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x88, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x88, 1);
    startSpriteAnimation(0x88, 2, 0);
    setSpritePaletteIndex(0x88, 2);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // selected tool/item
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    loadNavigationArrows();
    loadPauseScreenItemSprites();
    fadeInPauseScreenSprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadPauseScreenItemSprites);

void loadPauseScreenItemSprites(void) {

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
             
            dmaSprite(TOOL_SPRITES_BASE + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_PALETTE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_ANIM_METADATA, (u8*)OVERLAY_SCREEN_TOOL_SPRITES_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(TOOL_SPRITES_BASE + i, toolXCoords[j % 4], *(f32*)((u8*)toolYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(TOOL_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(TOOL_SPRITES_BASE + i, 2);
            setBilinearFiltering(TOOL_SPRITES_BASE + i, 1);
            startSpriteAnimation(TOOL_SPRITES_BASE + i, getAnimationOffsetFromScript(toolsAnimationScripts, D_80116EB8[gPlayer.toolSlots[i]] + getToolLevelForAnimation(gPlayer.toolSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.currentTool) {

        dmaSprite(TOOL_SPRITES_BASE + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_PALETTE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_ANIM_METADATA, (u8*)OVERLAY_SCREEN_TOOL_SPRITES_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
        setSpriteViewSpacePosition(TOOL_SPRITES_BASE + i, pauseScreenCurrentToolXPosition, pauseScreenCurrentToolYPosition, 256.0f);
        setSpriteColor(TOOL_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(TOOL_SPRITES_BASE + i, 2);
        setBilinearFiltering(TOOL_SPRITES_BASE + i, 1);
        
        startSpriteAnimation(TOOL_SPRITES_BASE + i, getAnimationOffsetFromScript(toolsAnimationScripts, D_80116EB8[gPlayer.currentTool] + getToolLevelForAnimation(gPlayer.currentTool)), 0);

    }
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(HOLDABLE_ITEMS_BASE + i);
    }

    for (i = 0; i < 8; i++) {

        f32* belongingXCoords = pauseScreenBelongingsXPosition;
        f32* belongingYCoords = pauseScreenBelongingsYPosition;
        j = i;

        if (gPlayer.belongingsSlots[i]) {
            
            dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, belongingXCoords[j % 4], *(f32*)((u8*)belongingYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(HOLDABLE_ITEMS_BASE + i, 2);
            setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
            startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(gPlayer.belongingsSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.heldItem) {
        dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
        setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, pauseScreenHeldItemXPosition, pauseScreenHeldItemYPosition, 256.0f);
        setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(HOLDABLE_ITEMS_BASE + i, 2);
        setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
        startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(gPlayer.heldItem)), 0);
    }

    for (i = 0; i < 8; i++) {
        resetAnimationState(KEY_ITEMS_BASE + i);
    }

    for (i = 0; i < 8; i++) {

        k = i;
        
        if (gPlayer.keyItemSlots[overlayScreenTable.pageNumber * 8 + i]) {
            
            dmaSprite(KEY_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_KEY_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_KEY_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            setSpriteViewSpacePosition(KEY_ITEMS_BASE + i, pauseScreenKeyItemsXPosition [k & 1], pauseScreenKeyItemsYPosition[k >> 1], 256.0f);
            setSpriteColor(KEY_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(KEY_ITEMS_BASE + i, 2);
            setBilinearFiltering(KEY_ITEMS_BASE + i, 1);

            startSpriteAnimation(KEY_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, D_80116EF4[ gPlayer.keyItemSlots[overlayScreenTable.pageNumber * 8 + i]]), 0);
        
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updatePauseScreenSelectionHighlight);

// FIXME: fake matches
void updatePauseScreenSelectionHighlight(void) {
    
    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;

    if (*ptr == 7) {

        temp = overlayScreenTable.unk_3;

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                // show item/tool selected
                setSpriteViewSpacePosition(0x82, getPauseScreenItemXCoordinate(temp2, temp), getPauseScreenItemYCoordinate(*ptr, overlayScreenTable.unk_3), 16.0f);
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
        setSpriteViewSpacePosition(0x82, getPauseScreenItemXCoordinate(temp2, temp), getPauseScreenItemYCoordinate(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInPauseScreenElements);

void fadeInPauseScreenSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutPauseScreenElements);

void fadeOutPauseScreenSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", pauseScreenCallback);

void pauseScreenCallback(void) {

    bool set = FALSE;
    bool flag;

    u8 tempItem;
    u32 temp2;

    switch (overlayScreenTable.screenState) {

        case 0:
            loadPauseScreenSprites();
            overlayScreenTable.screenState = 1;
            break;

        case 1:
            if (checkSpriteRGBAUpdateFinished(TOOL_AND_BELONGINGS_LABELS)) {
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;

        case 2:
            if (checkSpriteRGBAUpdateFinished(TOOL_AND_BELONGINGS_LABELS)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;    

        case 4:
            if (checkAnyMessageBoxTextFinished()) {
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
                showDownArrow(96.0f, -104.0f);
            } else {
                hideDownArrow();
            }
            
            if (overlayScreenTable.pageNumber) {
                showUpArrow(96.0f, 56.0f);
            } else {
                hideUpArrow();
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
                showDownArrow(96.0f, -104.0f);
            } else {
                hideDownArrow();
            }
            
            if (overlayScreenTable.pageNumber) {
                showUpArrow(96.0f, 56.0f);
            } else {
                hideUpArrow();
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
                        loadPauseScreenItemSprites();
                        updatePauseScreenSelectionHighlight();
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
                        loadPauseScreenItemSprites();
                        updatePauseScreenSelectionHighlight();
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
                showDownArrow(96.0f, -104.0f);
            } else {
                hideDownArrow();
            }
            
            if (overlayScreenTable.pageNumber) {
                showUpArrow(96.0f, 56.0f);
            } else {
                hideUpArrow();
            }
            
            break;
        
        case 8:
            if (checkSpriteRGBAUpdateFinished(TOOL_AND_BELONGINGS_LABELS)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 9;
            }
            break;
        
        // load village map (cut from NA release)
        case 9:
            deactivateSprite(0x80);
            setMainLoopCallbackFunctionIndex(PAUSE_SCREEN_MAP_LOAD);
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
                    updatePauseScreenSelectionHighlight();
                    
                } else {
                    fadeOutPauseScreenSprites();
                    resetAnimationState(CURSOR_HAND);
                    hideDownArrow();
                    hideUpArrow();
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
                        
                        swapPauseScreenItems();
                        
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        overlayScreenTable.unk_3 = 0xFF;
                        overlayScreenTable.screenState = overlayScreenTable.unk_5;
                        overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
    
                        loadPauseScreenItemSprites();
                        updatePauseScreenSelectionHighlight();
                    
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
                        
                        updatePauseScreenSelectionHighlight();
                        
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
                    
                    setItemDescriptionVariable(flag, tempItem);

                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                    
                }

            }
            
        }
        
        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 8) {
            setSpriteViewSpacePosition(CURSOR_HAND, (getPauseScreenItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f), getPauseScreenItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
            resetPauseScreenSpriteColors();
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getPauseScreenItemXCoordinate);

f32 getPauseScreenItemXCoordinate(u8 arg0, u32 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getPauseScreenItemYCoordinate);

f32 getPauseScreenItemYCoordinate(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", resetPauseScreenSpriteColors);

void resetPauseScreenSpriteColors(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", wapPauseScreenItems);

void swapPauseScreenItems(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadToolboxScreenCallback);

void loadToolboxScreenCallback(void) {
    
    openOverlayScreen();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadToolboxScreenSprites);

void loadToolboxScreenSprites(void) {

    
    loadClockSprites();
    
    setMoneySprites();
    
    // tool and item slots

    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 0, 0);

    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    // toolbox and rucksack labels
    dmaSprite(TOOL_AND_RUCKSACK_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(TOOL_AND_RUCKSACK_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(TOOL_AND_RUCKSACK_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(TOOL_AND_RUCKSACK_LABELS, 2);
    setBilinearFiltering(TOOL_AND_RUCKSACK_LABELS, 1);
    startSpriteAnimation(TOOL_AND_RUCKSACK_LABELS, 5, 1);

    // selected square
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 3);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CURSOR_HAND, getToolboxItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, getToolboxItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    // up and down arrows
    loadNavigationArrows();

    // load tools
    loadToolboxToolSprites();
    // fade in tools
    fadeInToolboxSprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadToolboxToolSprites);

void loadToolboxToolSprites(void) {

    u8 i;
    u32 j;
    
    for (i = 0; i < 9; i++) {
        resetAnimationState(TOOL_SPRITES_BASE + i);
    }

    for (i = 0; i < 8; i++) {
    
        j = i;

        if (gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]) {
             
            dmaSprite(TOOL_SPRITES_BASE + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (u8*)((i * (OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_1_BASE), (u8*)((i * (OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_2_BASE), (u16*)OVERLAY_SCREEN_TOOL_SPRITES_PALETTE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_ANIM_METADATA, (u8*)OVERLAY_SCREEN_TOOL_SPRITES_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(TOOL_SPRITES_BASE + i, toolboxToolSlotsXPosition[j % 4], *(f32*)((u8*)toolboxToolSlotsYPosition + (j & 0xFC)), 256.0f);

            setSpriteColor(TOOL_SPRITES_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(TOOL_SPRITES_BASE + i, 2);
            setBilinearFiltering(TOOL_SPRITES_BASE + i, 1);
            
            startSpriteAnimation(TOOL_SPRITES_BASE + i, getAnimationOffsetFromScript(toolsAnimationScripts,  D_80116EB8[gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i]] + getToolLevelForAnimation(gToolboxSlots[(overlayScreenTable.pageNumber * 8) + i])), 0);
            
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
             
            dmaSprite(0xAC + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(0xAC + i, toolXCoords[j % 4], *(f32*)((u8*)toolYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(0xAC + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(0xAC + i, 2);
            setBilinearFiltering(0xAC + i, 1);
            startSpriteAnimation(0xAC + i, getAnimationOffsetFromScript(toolsAnimationScripts, D_80116EB8[gPlayer.toolSlots[i]] + getToolLevelForAnimation(gPlayer.toolSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.currentTool) {

        dmaSprite(0xAC + i, (u32)&_toolsTextureSegmentRomStart, (u32)&_toolsTextureSegmentRomEnd, (u32)&_toolsAssetsIndexSegmentRomStart, (u32)&_toolsAssetsIndexSegmentRomEnd, (u32) &_toolsSpritesheetIndexSegmentRomStart, (u32)&_toolsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
        setSpriteViewSpacePosition(0xAC + i, toolboxCurrentToolXPosition, toolboxCurrentToolYPosition, 256.0f);
        setSpriteColor(0xAC + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(0xAC + i, 2);
        setBilinearFiltering(0xAC + i, 1);

        startSpriteAnimation(0xAC + i, getAnimationOffsetFromScript(toolsAnimationScripts, D_80116EB8[gPlayer.currentTool] + getToolLevelForAnimation(gPlayer.currentTool)), 0);

    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateToolboxSelectionHighlight);

void updateToolboxSelectionHighlight(void) {

    f32 x;
    
    if (overlayScreenTable.unk_5 == 5) {

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                
                x = getToolboxItemXCoordinate(overlayScreenTable.unk_5, overlayScreenTable.unk_3);
                setSpriteViewSpacePosition(0x82, x, getToolboxItemYCoordinate(overlayScreenTable.unk_5, overlayScreenTable.unk_3), 16.0f);
                startSpriteAnimation(0x82, 6, 0xFE);
            
            } else {
                resetAnimationState(0x82);
            }
            
        } else {
            resetAnimationState(0x82);
        }
        
    } else {

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            x = getToolboxItemXCoordinate(overlayScreenTable.unk_5, overlayScreenTable.unk_3);
            setSpriteViewSpacePosition(0x82, x, getToolboxItemYCoordinate(overlayScreenTable.unk_5, overlayScreenTable.unk_3), 16.0f);
            startSpriteAnimation(0x82, 6, 0xFE);
            
        } else {
            resetAnimationState(0x82);
        }
        
    } 
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInToolboxElements);

void fadeInToolboxSprites(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutToolboxElements);

void fadeOutToolboxSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", toolboxScreenCallback);

void toolboxScreenCallback(void) {

    bool set = FALSE;

    u8 tempTool;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            loadToolboxScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (checkSpriteRGBAUpdateFinished(TOOL_AND_RUCKSACK_LABELS)) {
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 6;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(TOOL_AND_RUCKSACK_LABELS)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            resumeGameplay();
            exitOverlayScreen();
            break;
        
        case 4:
            if (checkAnyMessageBoxTextFinished()) {
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
                        
                        loadToolboxToolSprites();
                        updateToolboxSelectionHighlight();
                        
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
                            loadToolboxToolSprites();
                            updateToolboxSelectionHighlight();

                            playSfx(2);
                            
                        }

                        set = TRUE;
                        
                    }
                
                }
                    
            }

            if (overlayScreenTable.pageNumber) {
                showLeftArrow(-96.0f, -64.0f);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(96.0f, -64.0f);
            } else {
                hideRightArrow();
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
                showLeftArrow(-96.0f, -64.0f);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(96.0f, -64.0f);
            } else {
                hideRightArrow();
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

                    loadToolboxToolSprites();
                    updateToolboxSelectionHighlight();
                    
                } else {
                    fadeOutToolboxSprites();
                    resetAnimationState(CURSOR_HAND);
                    hideLeftArrow();
                    hideRightArrow();
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
                    
                    swapToolboxTools();
                    
                    overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                    overlayScreenTable.unk_3 = 0xFF;
                    overlayScreenTable.screenState =overlayScreenTable.unk_5;
                    overlayScreenTable.pageNumber = overlayScreenTable.unk_4;

                    loadToolboxToolSprites();
                    
                } else {
                    overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                    overlayScreenTable.unk_5 = overlayScreenTable.screenState;
                    overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                }
                
                updateToolboxSelectionHighlight();
                
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
                    
                    setItemDescriptionVariable(0, tempTool);
                    
                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                    
                }
                
            }
            
        }
    
        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
            setSpriteViewSpacePosition(CURSOR_HAND, getToolboxItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, getToolboxItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
            resetToolboxSpriteColors();
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getToolboxItemXCoordinate);

f32 getToolboxItemXCoordinate(u8 arg0, u32 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getToolboxItemYCoordinate);

f32 getToolboxItemYCoordinate(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", resetToolboxSpriteColors);

void resetToolboxSpriteColors(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", swapToolboxTools);

// swap tools
void swapToolboxTools(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadFreezerScreenCallback);

// freezer load callback
void loadFreezerScreenCallback(void) {
    
    openOverlayScreen();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadFreezerScreenSprites);

void loadFreezerScreenSprites(void) {
    
    loadClockSprites();
    
    setMoneySprites();

    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);

    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, TRUE);
    startSpriteAnimation(0x83, 0, 0);

    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 0, 0);
    
    dmaSprite(FREEZER_AND_RUCKSACK_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(FREEZER_AND_RUCKSACK_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(FREEZER_AND_RUCKSACK_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(FREEZER_AND_RUCKSACK_LABELS, 2);
    setBilinearFiltering(FREEZER_AND_RUCKSACK_LABELS, 1);
    startSpriteAnimation(FREEZER_AND_RUCKSACK_LABELS, 5, 2);
    
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x87, 2);
    setBilinearFiltering(0x87, 1);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 2);
    
    dmaSprite(CURSOR_HAND, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8* )DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16* )(u16*)DIALOGUE_ICON_PALETTE_BUFFER, (u16* )(AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u8* )DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(CURSOR_HAND, getFreezerItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, getFreezerItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd,  (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    loadNavigationArrows();
    loadFreezerItemSprites();
    fadeInFreezerSprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadFreezerItemSprites);

void loadFreezerItemSprites(void) {

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
             
            dmaSprite(0xA3 + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (((i * 2) + i) << 9) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_1_BASE, (((i * 2) + i) << 9) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_PALETTE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_ANIM_METADATA, (u8*)OVERLAY_SCREEN_TOOL_SPRITES_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(0xA3 + i, xCoords[j % 4], *(f32*)((u8*)yCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(0xA3 + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(0xA3 + i, 2);
            setBilinearFiltering(0xA3 + i, 1);
            startSpriteAnimation(0xA3 + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(D_80237420[(overlayScreenTable.pageNumber * 8) + i])), 0);
        
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
            
            dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, belongingXCoords[j % 4], *(f32*)((u8*)belongingYCoords + (j & 0xFC)), 256.0f);
            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(HOLDABLE_ITEMS_BASE + i, 2);
            setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
            startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(gPlayer.belongingsSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.heldItem) {
        dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
        setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, D_80116D38, D_80116D44, 256.0f);
        setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(HOLDABLE_ITEMS_BASE + i, 2);
        setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
        startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(gPlayer.heldItem)), 0);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateFreezerSelectionHighlight);

void updateFreezerSelectionHighlight(void) {

    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;
    if (*ptr == 5) {

        temp = overlayScreenTable.unk_3;
        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, getFreezerItemXCoordinate(temp2, temp), getFreezerItemYCoordinate(*ptr, overlayScreenTable.unk_3), 16.0f);
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
        setSpriteViewSpacePosition(0x82, getFreezerItemXCoordinate(temp2, temp), getFreezerItemYCoordinate(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInFreezerElements);

void fadeInFreezerSprites(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutFreezerElements);

void fadeOutFreezerSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", freezerScreenCallback);

// freezer callback function
void freezerScreenCallback(void) {

    bool set = FALSE;

    u8 temp;
    u8 type;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            loadFreezerScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (checkSpriteRGBAUpdateFinished(FREEZER_AND_RUCKSACK_LABELS)) {
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(FREEZER_AND_RUCKSACK_LABELS)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            resumeGameplay();
            exitOverlayScreen();
            break;
        
        case 4:
            if (checkAnyMessageBoxTextFinished()) {
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
                        
                        loadFreezerItemSprites();
                        updateFreezerSelectionHighlight();
                        
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
                            
                            loadFreezerItemSprites();
                            updateFreezerSelectionHighlight();

                            playSfx(2);
                            
                        }

                        set = TRUE;
                        
                    }
                
                }
                    
            }

            if (overlayScreenTable.pageNumber) {
                showLeftArrow(-96.0f, 16.0f);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(96.0f, 16.0f);
            } else {
                hideRightArrow();
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
                showLeftArrow(-96.0f, 16.0f);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(96.0f, 16.0f);
            } else {
                hideRightArrow();
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
                    loadFreezerItemSprites();
                    updateFreezerSelectionHighlight();
                } else {
                    fadeOutFreezerSprites();
                    resetAnimationState(CURSOR_HAND);
                    hideLeftArrow();
                    hideRightArrow();
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
                    
                    if (isFreezerItemSwappable()) {
                        swapFreezerItems();
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        overlayScreenTable.unk_3 = 0xFF;
                        overlayScreenTable.screenState = overlayScreenTable.unk_5;
                        overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                        loadFreezerItemSprites();
                        updateFreezerSelectionHighlight();
                        playSfx(0);
                    } else {
                        playSfx(3);
                    }
                    
                } else {

                    if (isFreezerItemSwappable()) {
                        overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                        overlayScreenTable.unk_5 = overlayScreenTable.screenState;
                        overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                        updateFreezerSelectionHighlight();
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
                    setItemDescriptionVariable(type, temp);
                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                }
                 
            }
            
        }
    
        if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 7) {
            setSpriteViewSpacePosition(CURSOR_HAND, getFreezerItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, getFreezerItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
            resetFreezerSpriteColors();
        }
    
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getFreezerItemXCoordinate);

inline f32 getFreezerItemXCoordinate(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getFreezerItemYCoordinate);

inline f32 getFreezerItemYCoordinate(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", resetFreezerSpriteColors);

void resetFreezerSpriteColors(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", isFreezerItemSwappable);

bool isFreezerItemSwappable(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", swapFreezerItems);

void swapFreezerItems(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCabinetScreenCallback);

// cabinet load callback
void loadCabinetScreenCallback(void) {
    
    openOverlayScreen();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

// INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCabinetScreenSprites);

void loadCabinetScreenSprites(void) {

    loadClockSprites();

    setMoneySprites();
    
    dmaSprite(0x83, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 42.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, TRUE);
    startSpriteAnimation(0x83, 0, 0);
    
    dmaSprite(0x84, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 42.0f, -80.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, TRUE);
    startSpriteAnimation(0x84, 0, 0);

    dmaSprite(CABINET_AND_RUCKSACK_LABELS, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CABINET_AND_RUCKSACK_LABELS, 0.0f, 0.0f, 16.0f);
    setSpriteColor(CABINET_AND_RUCKSACK_LABELS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(CABINET_AND_RUCKSACK_LABELS, 2);
    setBilinearFiltering(CABINET_AND_RUCKSACK_LABELS, 1);
    startSpriteAnimation(CABINET_AND_RUCKSACK_LABELS, 5, 3);
    
    dmaSprite(0x87, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, 80.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x87, 2);
    setBilinearFiltering(0x87, TRUE);
    startSpriteAnimation(0x87, 2, 0);
    setSpritePaletteIndex(0x87, 2);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CURSOR_HAND, getFreezerItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, getFreezerItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 32.0f);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    dmaSprite(0x82, (u32)&_inventoryScreenTextureSegmentRomStart, (u32)&_inventoryScreenTextureSegmentRomEnd, 
              (u32)&_inventoryScreenAssetsIndexSegmentRomStart, (u32)&_inventoryScreenAssetsIndexSegmentRomEnd, 
              NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 24.0f);
    setBilinearFiltering(0x82, TRUE);
    setSpriteColor(0x82, 255, 255, 255, 255);
    
    loadNavigationArrows();
    loadCabinetItemSprites();
    fadeInCabinetSprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCabinetItemSprites);

// cabinet
void loadCabinetItemSprites(void) {

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
             
            dmaSprite(0xA3 + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (((i * 2) + i) << 9) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_1_BASE, (((i * 2) + i) << 9) + OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_PALETTE, (u16*)OVERLAY_SCREEN_TOOL_SPRITES_ANIM_METADATA, (u8*)OVERLAY_SCREEN_TOOL_SPRITES_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_TOOL_SPRITES_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(0xA3 + i, xCoords[j % 4], *(f32*)((u8*)yCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(0xA3 + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(0xA3 + i, 2);
            setBilinearFiltering(0xA3 + i, 1);
            startSpriteAnimation(0xA3 + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(D_801890E8[(overlayScreenTable.pageNumber * 8) + i])), 0);
        
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
            
            dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
            
            // FIXME: should be / 4 instead of & 0xFC
            setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, belongingXCoords[j % 4], *(f32*)((u8*)belongingYCoords + (j & 0xFC)), 256.0f);

            setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(HOLDABLE_ITEMS_BASE + i, 2);
            setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
            startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(gPlayer.belongingsSlots[i])), 0);
        
        }
        
    }

    if (gPlayer.heldItem) {
        dmaSprite(HOLDABLE_ITEMS_BASE + i, (u32)&_holdableItemsTextureSegmentRomStart, (u32)&_holdableItemsTextureSegmentRomEnd, (u32)&_holdableItemsAssetsIndexSegmentRomStart, (u32)&_holdableItemsAssetsIndexSegmentRomEnd, (u32) &_holdableItemsSpritesheetIndexSegmentRomStart, (u32)&_holdableItemsSpritesheetIndexSegmentRomEnd, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_1_BASE, (i * (OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_SIZE * 2)) + OVERLAY_SCREEN_HOLDABLE_ITEMS_TEXTURE_2_BASE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_PALETTE, (u16*)OVERLAY_SCREEN_ITEMS_GRID_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ITEMS_GRID_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ITEMS_GRID_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
        setSpriteViewSpacePosition(HOLDABLE_ITEMS_BASE + i, D_80116D70, D_80116D7C, 256.0f);
        setSpriteColor(HOLDABLE_ITEMS_BASE + i, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(HOLDABLE_ITEMS_BASE + i, 2);
        setBilinearFiltering(HOLDABLE_ITEMS_BASE + i, 1);
        startSpriteAnimation(HOLDABLE_ITEMS_BASE + i, getAnimationOffsetFromScript(shopItemsAnimationScripts, getItemAnimationIndex(gPlayer.heldItem)), 0);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateCabinetSelectionHighlight);

void updateCabinetSelectionHighlight(void) {

    u32 temp;
    u32 temp2;
    u8 *ptr = &overlayScreenTable.unk_5;

    temp2 = *ptr;

    if (*ptr == 5) {

        temp = overlayScreenTable.unk_3;

        if (overlayScreenTable.unk_3 != 0xFF) {
            
            if (overlayScreenTable.unk_4 == overlayScreenTable.pageNumber) {
                setSpriteViewSpacePosition(0x82, getCabinetItemXCoordinate(temp2, temp), getCabinetItemYCoordinate(*ptr, overlayScreenTable.unk_3), 16.0f);
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
        setSpriteViewSpacePosition(0x82, getCabinetItemXCoordinate(temp2, temp), getCabinetItemYCoordinate(*ptr, overlayScreenTable.unk_3), 16.0f);
        startSpriteAnimation(0x82, 6, 0xFE);
    } else {
reset:
        resetAnimationState(0x82);
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInCabinetElements);

void fadeInCabinetSprites(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutCabinetElements);

void fadeOutCabinetSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", cabinetScreenCallback);

// cabinet screen main loop callback
void cabinetScreenCallback(void) {

    bool set = FALSE;
    
    u8 temp;
    u8 type;

    switch (overlayScreenTable.screenState) {

        case 0:
            loadCabinetScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
            
        case 1:
            if (checkSpriteRGBAUpdateFinished(CABINET_AND_RUCKSACK_LABELS)) {
                startSpriteAnimation(0x79, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
            
        case 2:
            if (checkSpriteRGBAUpdateFinished(CABINET_AND_RUCKSACK_LABELS)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
            
        case 3:
            deactivateSprite(0x80);
            resumeGameplay();
            exitOverlayScreen();
            break;
            
        case 4:
            if (checkAnyMessageBoxTextFinished()) {
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
                        
                        loadCabinetItemSprites();
                        updateCabinetSelectionHighlight();
                        
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
                            loadCabinetItemSprites();
                            updateCabinetSelectionHighlight();

                            playSfx(2);
                            
                        }

                        set = TRUE;
                        
                    }
                
                }
                    
            }

            if (overlayScreenTable.pageNumber) {
                showLeftArrow(-96.0f, 16.0f);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(96.0f, 16.0f);
            } else {
                hideRightArrow();
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
                showLeftArrow(-96.0f, 16.0f);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(96.0f, 16.0f);
            } else {
                hideRightArrow();
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
                    loadCabinetItemSprites();
                    updateCabinetSelectionHighlight();
                    
                } else {
                    fadeOutCabinetSprites();
                    resetAnimationState(0x79);
                    hideLeftArrow();
                    hideRightArrow();
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

                    if (isCabinetItemSwappable()) {
                        
                        swapCabinetItems();
                        
                        overlayScreenTable.cellIndex = overlayScreenTable.unk_3;
                        overlayScreenTable.unk_3 = 0xFF;
                        overlayScreenTable.screenState = overlayScreenTable.unk_5;
                        overlayScreenTable.pageNumber = overlayScreenTable.unk_4;
                        
                        loadCabinetItemSprites();
                        updateCabinetSelectionHighlight();

                        playSfx(0);
                        
                    } else {
                        playSfx(3);
                    }
                    
                } else if (isCabinetItemSwappable()) {
                    overlayScreenTable.unk_3 = overlayScreenTable.cellIndex;
                    overlayScreenTable.unk_5 = overlayScreenTable.screenState;
                    overlayScreenTable.unk_4 = overlayScreenTable.pageNumber;
                    updateCabinetSelectionHighlight();
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
                    setItemDescriptionVariable(type, temp);
                    overlayScreenTable.unk_7 = overlayScreenTable.screenState;
                    overlayScreenTable.screenState = 4;
                }
                
            }
            
        }

    }
    
    if (4 < overlayScreenTable.screenState && overlayScreenTable.screenState < 102) {
        setSpriteViewSpacePosition(0x79, getCabinetItemXCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 16.0f, getCabinetItemYCoordinate(overlayScreenTable.screenState, overlayScreenTable.cellIndex) - 8.0f, 256.0f);
        resetCabinetSpriteColors();
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getCabinetItemXCoordinate);

f32 getCabinetItemXCoordinate(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getCabinetItemYCoordinate);

f32 getCabinetItemYCoordinate(u8 arg0, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", resetCabinetSpriteColors);

void resetCabinetSpriteColors(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", isCabinetItemSwappable);

bool isCabinetItemSwappable(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", swapCabinetItems);

void swapCabinetItems(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", getToolLevelForAnimation);

u8 getToolLevelForAnimation(u8 tool) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadHouseExtensionsScreenSprites);

void loadHouseExtensionsScreenSprites(void) {
    
    loadClockSprites();

    setMoneySprites();

    // extensions list
    dmaSprite(HOUSE_EXTENSIONS_LIST, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(HOUSE_EXTENSIONS_LIST, 0.0f, 0.0f, 8.0f);
    setSpriteColor(HOUSE_EXTENSIONS_LIST, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(HOUSE_EXTENSIONS_LIST, 2);
    setBilinearFiltering(HOUSE_EXTENSIONS_LIST, TRUE);
    startSpriteAnimation(HOUSE_EXTENSIONS_LIST, 0, 0);

    // price labels
    dmaSprite(0x83, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, TRUE);
    startSpriteAnimation(0x83, 1, 0);
    
    dmaSprite(0x84, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, TRUE);
    startSpriteAnimation(0x84, 1, 0);
    
    dmaSprite(0x85, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, -32.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x85, 2);
    setBilinearFiltering(0x85, TRUE);
    startSpriteAnimation(0x85, 1, 0);
    
    dmaSprite(0x86, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 0.0f, -48.0f, 8.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x86, 2);
    setBilinearFiltering(0x86, TRUE);
    startSpriteAnimation(0x86, 1, 0);
    
    dmaSprite(0x87, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x87, 0.0f, -64.0f, 8.0f);
    setSpriteColor(0x87, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x87, 2);
    setBilinearFiltering(0x87, TRUE);
    startSpriteAnimation(0x87, 1, 0);
    
    dmaSprite(0x88, (u32)&_houseExtensionSelectionTextureSegmentRomStart, (u32)&_houseExtensionSelectionTextureSegmentRomEnd, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomStart, (u32)&_houseExtensionSelectionAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x88, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0x88, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x88, 2);
    setBilinearFiltering(0x88, TRUE);
    startSpriteAnimation(0x88, 1, 0);
        
    dmaSprite(HOUSE_EXTENSIONS_BACKGROUND, (u32)&_photoSmallTextureSegmentRomStart, (u32)&_photoSmallTextureSegmentRomEnd, (u32)&_photoSmallAssetsIndexSegmentRomStart, (u32)&_photoSmallAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(HOUSE_EXTENSIONS_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(HOUSE_EXTENSIONS_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(HOUSE_EXTENSIONS_BACKGROUND, 2);
    setBilinearFiltering(HOUSE_EXTENSIONS_BACKGROUND, TRUE);
    startSpriteAnimation(HOUSE_EXTENSIONS_BACKGROUND, 0, 0);

    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);

    fadeInHouseExtensionsSelectionSprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInHouseExtensionsSelectionSprites);

void fadeInHouseExtensionsSelectionSprites(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutHouseExtensionsSprites);

void fadeOutHouseExtensionsSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", houseExtensionsSelectionScreenCallback);

void houseExtensionsSelectionScreenCallback(void) {

    u16 buff[6];
    u16* ptr = D_80121D74;

    bool set;
    u8 temp;

    memcpy(buff, ptr, 12);

    set = FALSE;
    
    switch (overlayScreenTable.screenState) {

        case 0:
            loadHouseExtensionsScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        case 1:
            if (checkSpriteRGBAUpdateFinished(CLOCK)) {
                setSpriteViewSpacePosition(CURSOR_HAND, houseExtensionCursorHandXPositions[overlayScreenTable.cellIndex], houseExtensionCursorHandYPositions[overlayScreenTable.cellIndex], 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 7;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(CLOCK)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;

        case 4:
            
            if (isDialogueClosing()) {
                
                temp = getSelectedMenuRow(0);
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

            if (checkAnyMessageBoxTextFinished()) {
                
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
            if (checkAnyMessageBoxTextFinished()) {
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
                setMessageBoxInterpolationWithFlags(0, -4, 0);
                
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens",  loadEstimateScreenSprites);

void loadEstimateScreenSprites(void) {

    u8 temp;
    
    loadClockSprites();

    setMoneySprites();

    // labels
    dmaSprite(0x82, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 0, 0);

    dmaSprite(ESTIMATE_BACKGROUND, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(ESTIMATE_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(ESTIMATE_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(ESTIMATE_BACKGROUND, 2);
    setBilinearFiltering(ESTIMATE_BACKGROUND, 1);
    startSpriteAnimation(ESTIMATE_BACKGROUND, 0, 0);

    temp = getDogBestRacePlacement();
    
    if (temp != 0xFF) {
        dmaSprite(0x83, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x83, 32.0f, 36.0f, 16.0f);
        setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(0x83, 2);
        setBilinearFiltering(0x83, 1);
        startSpriteAnimation(0x83, temp + 1, 0);
    }

    temp = getHorseBestRacePlacement();
    
    if (temp != 0xFF) {
        dmaSprite(0x84, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x84, 32.0f, 20.0f, 8.0f);
        setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(0x84, 2);
        setBilinearFiltering(0x84, 1);
        startSpriteAnimation(0x84, temp + 1, 0);
    }

    temp = getBestCowMilkType();
    
    if (temp != 0xFF) {
        dmaSprite(0x85, (u32)&_estimateScreenTextureSegmentRomStart, (u32)&_estimateScreenTextureSegmentRomEnd, (u32)&_estimateScreenAssetsIndexSegmentRomStart, (u32)&_estimateScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
        setSpriteViewSpacePosition(0x85, -24.0f, 4.0f, 8.0f);
        setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
        setSpriteBlendMode(0x85, 2);
        setBilinearFiltering(0x85, 1);
        startSpriteAnimation(0x85, temp + 1, 0);
    }
    
    setOverlayScreenSprites(2, 0x9A, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -60.0f, 4.0f, 256.0f, 8);
    dmaOverlayScreenSprites(2, getTotalCowsCount(), 1, 3);
    setOverlayScreenSpritesRGBA(2, 255, 255, 255, 255);
    
    setOverlayScreenSprites(3, 0x9B, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -60.0f, -12.0f, 256.0f, 8);
    dmaOverlayScreenSprites(3, getTotalSheepCount(), 1, 3);
    setOverlayScreenSpritesRGBA(3, 255, 255, 255, 255);
    
    setOverlayScreenSprites(4, 0x9C, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -60.0f, -28.0f, 256.0f, 8);
    dmaOverlayScreenSprites(4, getTotalChickenCount(), 1, 3);
    setOverlayScreenSpritesRGBA(4, 255, 255, 255, 255);
    
    setOverlayScreenSprites(5, 0x9D, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -16.0f, -44.0f, 256.0f, 8);
    dmaOverlayScreenSprites(5, gLumber, 3, 3);
    setOverlayScreenSpritesRGBA(5, 255, 255, 255, 255);
    
    setOverlayScreenSprites(6, 0xA0, (u32)&_bulletNumeralTextureSegmentRomStart, (u32)&_bulletNumeralTextureSegmentRomEnd, (u32)&_bulletNumeralAssetsIndexSegmentRomStart, (u32)&_bulletNumeralAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_ESIMATE_SCREEN_NUMBERS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -16.0f, -60.0f, 256.0f, 8);
    dmaOverlayScreenSprites(6, fodderQuantity, 3, 3);
    setOverlayScreenSpritesRGBA(6, 255, 255, 255, 255);
    
    setMessageBoxViewSpacePosition(0, 44.0f, 20.0f, 352.0f);
    setMessageBoxSpriteIndices(0, 0xFF, 0xFF, 0xFF);
    setMessageBoxInterpolationWithFlags(0, -4, 2);
    initializeMessageBox(0, 0, 5, 0x8000);
    
    fadeInEstimateSprites();
    
}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInEstimateSprites);

void fadeInEstimateSprites(void) {

    fadeInClock();

    updateSpriteAlpha(ESTIMATE_BACKGROUND, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutEstimateSprites);

void fadeOutEstimateSprites(void) {

    fadeOutClock();

    updateSpriteAlpha(ESTIMATE_BACKGROUND, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    
    updateSpriteAlpha(0x85, 0, 24);
    
    messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags &= ~0x8000;
    
    resetMessageBoxAnimation(0);
    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);
    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(4);
    deactivateOverlayScreenSprites(5);
    deactivateOverlayScreenSprites(6);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", estimateScreenCallback);

void estimateScreenCallback(void) {

    u8 temp = 0;

    switch (overlayScreenTable.screenState) {
        case 4:
            break;
        case 0:
             loadEstimateScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        case 1:
            if (checkSpriteRGBAUpdateFinished(CLOCK)) {
                overlayScreenTable.screenState = 4;
            }
            break;
        case 2:
            if (checkSpriteRGBAUpdateFinished(CLOCK)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;
    }
    
    if ((overlayScreenTable.screenState >= 4) && (checkButtonPressed(CONTROLLER_1, BUTTON_B)) && (temp == 0)) {
        fadeOutEstimateSprites();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}


/* kitchen picture functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadKitchenPictureScreenSprites);

void loadKitchenPictureScreenSprites(void) {
    
    loadClockSprites();
    
    dmaSprite(0x82, (u32)&_powerNutTreeDrawingTextureSegmentRomStart, (u32)&_powerNutTreeDrawingTextureSegmentRomEnd, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomStart, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomEnd,  0, 0, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, 2);
    setBilinearFiltering(0x82, TRUE);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    loadPowerNutTreeSprites();
    fadeInKitchenPictureSprites();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadPowerNutTreeSprites);

void loadPowerNutTreeSprites(void) {

    u8 i;

    for (i = 0; i < 10; i++) {

        if (powerNutBits & D_80116F24[i]) {

            dmaSprite(0xA3 + i, (u32)&_powerNutTreeDrawingTextureSegmentRomStart, (u32)&_powerNutTreeDrawingTextureSegmentRomEnd, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomStart, (u32)&_powerNutTreeDrawingAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
            
            setSpriteViewSpacePosition(0xA3 + i, D_80116E18[i], D_80116E40[i], 8.0f);
            setSpriteColor(0xA3 + i, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(0xA3 + i, 2);
            setBilinearFiltering(0xA3 + i, 1);
            startSpriteAnimation(0xA3 + i, 1, 0);
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updatePowerNutCellAnimations);

void updatePowerNutCellAnimations(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", findPowerNutCellIndex);

u8 findPowerNutCellIndex(u8 cellIndex, u8 arg1) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInKitchenPictureSprites);

inline void fadeInKitchenPictureSprites(void) {
    
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutKitchenPictureSprites);

void fadeOutKitchenPictureSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", kitchenPictureScreenCallback);

void kitchenPictureScreenCallback(void) {

    bool set = FALSE;
    u8 temp;

    switch (overlayScreenTable.screenState) {

        case 0:
            loadKitchenPictureScreenSprites();
            overlayScreenTable.screenState = 1;
            break;

        case 1:
            if (checkSpriteRGBAUpdateFinished(CLOCK)) {
                
                if (overlayScreenTable.cellIndex != 0xFF) {
                    setSpriteViewSpacePosition(CURSOR_HAND, D_80116E18[overlayScreenTable.cellIndex] - 8.0f, D_80116E40[overlayScreenTable.cellIndex] - 8.0f, 32.0f);
                    startSpriteAnimation(CURSOR_HAND, 3, 0);
                    updatePowerNutCellAnimations();
                } 
                
                overlayScreenTable.screenState = 5;
                
            }
            
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(CLOCK)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;

        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;

        case 4:
            if (checkAnyMessageBoxTextFinished()) {
                overlayScreenTable.screenState = 5;
            }
            break;


        case 5:

            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHWEST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 0xFF) {
                        
                        temp = findPowerNutCellIndex(overlayScreenTable.cellIndex, 2);
                        
                        if (temp != 0xFF) {
                            overlayScreenTable.cellIndex = temp;
                            setSpriteViewSpacePosition(CURSOR_HAND, D_80116E18[temp] - 8.0f, D_80116E40[temp] - 8.0f, 32.0f);
                            updatePowerNutCellAnimations();
                            playSfx(2);
                        }
    
                    }

                    set = TRUE;

                }
                
            }
            
            if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHEAST)) {
                
                if (!set) {
                    
                    if (overlayScreenTable.cellIndex != 0xFF) {

                        temp = findPowerNutCellIndex(overlayScreenTable.cellIndex, 1);

                        if (temp != 0xFF) {
                            
                            overlayScreenTable.cellIndex = temp;
                            setSpriteViewSpacePosition(CURSOR_HAND, D_80116E18[temp] - 8.0f, D_80116E40[temp] - 8.0f, 32.0f);
                            updatePowerNutCellAnimations();
                            playSfx(2);
                            
                        }
                        
                
                    }
                
                    set = TRUE;
                    
                }
                
            }
            
            if (checkButtonPressed(CONTROLLER_1, BUTTON_A)) {

                if (!set) {
                    
                    if (findPowerNutCellIndex(overlayScreenTable.cellIndex, 0) != 0xFF) {
                        
                        setMessageBoxViewSpacePosition(0, 0.0f, -64.0f, 352.0f);
                        setMessageBoxSpriteIndices(0, 1, 0, 0);
                        setMessageBoxInterpolationWithFlags(0, -4, 2);
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
        fadeOutKitchenPictureSprites();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

/* end kitchen picture functions */


/* calendar functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCalendarScreenSprites);

void loadCalendarScreenSprites(void) {
    
    // main calendar components
    dmaSprite(CALENDAR_BACKGROUND, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(CALENDAR_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(CALENDAR_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(CALENDAR_BACKGROUND, 2);
    setBilinearFiltering(CALENDAR_BACKGROUND, TRUE);
    startSpriteAnimation(CALENDAR_BACKGROUND, 0, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    loadCalendarStickers();
    loadCalendarDateNumbers(gYear, gSeason, gDayOfMonth);
    fadeInCalendarSprites();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCalendarStickers);

void loadCalendarStickers(void) {

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
                
                dmaSprite(CALENDAR_STICKERS_BASE + temp, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
                setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + temp, D_80116E68[0], D_80116E90[0], 8.0f);
                setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + temp, (j << 5) - 122, -(i * 24) + 0x1A, 4.0f);
                setSpriteColor(CALENDAR_STICKERS_BASE + temp, 0xFF, 0xFF, 0xFF, 0);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + temp, 2);
                setBilinearFiltering(CALENDAR_STICKERS_BASE + temp, 1);
                startSpriteAnimation(CALENDAR_STICKERS_BASE + temp, 1, temp);
                
            }
            
        } 
    
    }

    
    for (k = 0; k < 10; k++) {

        if (D_80205640[k] != 0xFF) {

            dmaSprite(CALENDAR_STICKERS_BASE + k, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
            setSpriteViewSpacePosition(CALENDAR_STICKERS_BASE + k, D_80116E68[k], D_80116E90[k], 8.0f);
            setSpriteColor(CALENDAR_STICKERS_BASE + k, 0xFF, 0xFF, 0xFF, 0);
            setSpriteBlendMode(CALENDAR_STICKERS_BASE + k, 2);
            setBilinearFiltering(CALENDAR_STICKERS_BASE + k, 1);
            startSpriteAnimation(CALENDAR_STICKERS_BASE + k, 1, k);
            
        }
        
    }

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadCalendarDateNumbers);

void loadCalendarDateNumbers(u8 year, u8 season, u8 dayOfMonth) {
    
    u8 row, col;
    
    u8 dayIndex;
    u8 startingWeekday;
    u8 skipCount;
    bool isSaturdayStart;
    
    dmaSprite(0x83, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 255, 255, 255, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, season - 1);

    skipCount = 0;
    dayIndex = 0;

    startingWeekday = (year % 7 + ((season - 1) * 30) % 7) % 7;
    isSaturdayStart = (startingWeekday == 6); 

    for (row = 0; row < 5; row++) {

        for (col = 0; col < 7; col++) {

            if (skipCount == startingWeekday && dayIndex < 30) {
                
                dmaSprite(0x8F + dayIndex, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);

                setSpriteViewSpacePosition(0x8F + dayIndex, (f32)(col * 32), (f32)(-row * 24), 16.0f);
                setSpriteColor(0x8F + dayIndex, 255, 255, 255, 0);
                setSpriteBlendMode(0x8F + dayIndex, 2);
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

                    dmaSprite(0xB7, (u32)&_calendarTextureSegmentRomStart, (u32)&_calendarTextureSegmentRomEnd, (u32)&_calendarAssetsIndexSegmentRomStart, (u32)&_calendarAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
                    
                    setSpriteViewSpacePosition(0xB7, (f32)(col * 32), (f32)(-row * 24), 32.0f);
                    setSpriteColor(0xB7, 255, 255, 255, 0);
                    setSpriteBlendMode(0xB7, 2);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInCalendarElements);

void fadeInCalendarSprites(void) {
    
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


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutCalendarElements);

void fadeOutCalendarSprites(void) {

    u8 i;
    
    fadeOutClock();

    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);

    updateSpriteAlpha(0xB7, 0, 24);

    setSpriteBlendMode(CALENDAR_STICKERS_BASE, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 1, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 2, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 3, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 4, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 5, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 6, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 7, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 8, 2);
    setSpriteBlendMode(CALENDAR_STICKERS_BASE + 9, 2);

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", calendarScreenCallback);

void calendarScreenCallback(void) {

    bool set = FALSE;
    u8 temp;

    switch (overlayScreenTable.screenState) {

        case 0:
            loadCalendarScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            
            if (checkSpriteRGBAUpdateFinished(CALENDAR_BACKGROUND)) {

                setSpriteBlendMode(CALENDAR_STICKERS_BASE, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 1, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 2, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 3, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 4, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 5, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 6, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 7, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 8, 3);
                setSpriteBlendMode(CALENDAR_STICKERS_BASE + 9, 3);

                overlayScreenTable.screenState = 5;
                
                setSpriteViewSpacePosition(CURSOR_HAND, 
                    (f32)((overlayScreenTable.unk_8 * 18) + 100), 
                    (f32)(-(overlayScreenTable.unk_9 * 24) + 22), 
                    32.0f);
                
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                
            }
            
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(CALENDAR_BACKGROUND)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;

        case 4:
            if (checkAnyMessageBoxTextFinished()) {
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
                        setSpriteBlendMode(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 3);
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
                    setSpriteBlendMode(CALENDAR_STICKERS_BASE + overlayScreenTable.cellIndex, 3);
                    overlayScreenTable.screenState = 5;
        
                    playSfx(1);
            
                    set = TRUE;
                
                }
                
            }

            break;
        
    }

    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(CURSOR_HAND);
        fadeOutCalendarSprites();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}


/* recipe book functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRecipeBookScreenSprites);

void loadRecipeBookScreenSprites(void) {
    
    dmaSprite(RECIPES_BACKGROUNDS, (u32)&_bookTextureSegmentRomStart, (u32)&_bookTextureSegmentRomEnd, (u32)&_bookAssetsIndexSegmentRomStart, (u32)&_bookAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(RECIPES_BACKGROUNDS, 0.0f, 0.0f, 8.0f);
    setSpriteColor(RECIPES_BACKGROUNDS, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(RECIPES_BACKGROUNDS, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(RECIPES_BACKGROUNDS, TRUE);
    startSpriteAnimation(RECIPES_BACKGROUNDS, 0, 0);
    
    // "Recipe" label
    dmaSprite(0x82, (u32)&_bookScreenLabelsTextureSegmentRomStart, (u32)&_bookScreenLabelsTextureSegmentRomEnd, (u32)&_bookScreenLabelsAssetsIndexSegmentRomStart, (u32)&_bookScreenLabelsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x82, TRUE);
    startSpriteAnimation(0x82, 1, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // up and down arrows
    loadNavigationArrows();
    // up and down arrows
    loadDialogueArrowSprites();
    loadRecipeBookLabels();
    
    setSpriteColor(RECIPE_LABELS_BASE, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 1, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 2, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 3, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 4, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 5, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 6, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 7, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 8, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 9, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 10, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 11, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 12, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 13, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 14, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(RECIPE_LABELS_BASE + 15, 0xFF, 0xFF, 0xFF, 0);
    
    fadeInRecipeBookSprites();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRecipeBookLabels);

// load recipe label sprites for obtained recipes
void loadRecipeBookLabels(void) {

    u8 i, j, k;

    for (i = 0; i < 16; i++) {
        resetAnimationState(RECIPE_LABELS_BASE + i);
    }

    i = 0;

    for (k = 0; k < 2; k++) {
        
        for (j = 0; j < 8; j++) {

            if (checkRecipe(overlayScreenTable.pageNumber * 16 + i)) {

                dmaSprite(RECIPE_LABELS_BASE + i, (u32)&_overlayScreenNamesTextureSegmentRomStart, (u32)&_overlayScreenNamesTextureSegmentRomEnd, (u32)&_overlayScreenNamesAssetsIndexSegmentRomStart, (u32)&_overlayScreenNamesAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_RECIPE_LABELS_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_RECIPE_LABELS_PALETTE_BUFFER, (u16*)OVERLAY_SCREEN_RECIPE_LABELS_ANIMATION_FRAME_METADATA_BUFFER, (u8*)OVERLAY_SCREEN_RECIPE_LABELS_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
                setSpriteViewSpacePosition(RECIPE_LABELS_BASE + i, (k * 128) + -120.0f, 64.0f - (j * 16), 8.0f);
                setSpriteColor(RECIPE_LABELS_BASE + i, 255, 255, 255, 255);
                setSpriteBlendMode(RECIPE_LABELS_BASE + i, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(RECIPE_LABELS_BASE + i, 1);
                startSpriteAnimation(RECIPE_LABELS_BASE + i, 1, (i + (overlayScreenTable.pageNumber * 16)));
                
            }

            i++;
            
        }
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInRecipeBookElements);

void fadeInRecipeBookSprites(void) {

    fadeInClock();

    updateSpriteAlpha(RECIPES_BACKGROUNDS, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);

    updateSpriteAlpha(RECIPE_LABELS_BASE, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 1, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 2, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 3, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 4, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 5, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 6, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 7, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 8, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 9, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 10, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 11, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 12, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 13, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 14, 0xFF, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 15, 0xFF, 24);

}


//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutRecipeBookElements);

void fadeOutRecipeBookSprites(void) {

    fadeOutClock();

    updateSpriteAlpha(RECIPES_BACKGROUNDS, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);

    updateSpriteAlpha(RECIPE_LABELS_BASE, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 1, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 2, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 3, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 4, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 5, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 6, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 7, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 8, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 9, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 10, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 11, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 12, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 13, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 14, 0, 24);
    updateSpriteAlpha(RECIPE_LABELS_BASE + 15, 0, 24);

    deactivateOverlayScreenSprites(0);
    deactivateOverlayScreenSprites(1);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", recipeBookScreenCallback);

void recipeBookScreenCallback(void) {

    bool set = FALSE;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            loadRecipeBookScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 128), (f32)(56 - (overlayScreenTable.unk_9 * 16)), 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;
        
        case 4:
            
            if (!(messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & (0x40 | 0x80))) {
                
                if (checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_SOUTHWEST)) {
                    
                    if (!set) {
                        if (scrollMessageBoxDown(0)) {
                            playSfx(2);
                        }
                        set = TRUE;
                    }
                
                }
                
                if ((checkButtonRepeat(CONTROLLER_1, BUTTON_STICK_NORTHEAST))) {
                    
                    if (!set) {
                        
                        if (scrollMessageBoxUp(0)) {
                            playSfx(2);
                        }
                        
                        set = TRUE;
                        
                    }
                }
                
                if (checkMoreLinesToPrint(0)) {
                    showDialogueDownArrow(0, -40.0f);
                } else {
                    hideDialogueDownArrow();
                }
                if (checkMessageBoxScrolled(0)) {
                    showDialogueUpArrow(0, 40.0f);
                } else {
                    hideDialogueUpArrow();
                }
                
                if (checkButtonPressed(CONTROLLER_1, (BUTTON_B | BUTTON_A))) {

                    if (!set) {

                        messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags &= ~0x8000;
                        
                        resetMessageBoxAnimation(0);
                        
                        set = TRUE;
                        
                        hideDialogueDownArrow();
                        hideDialogueUpArrow();
                        setMessageBoxScrollSpeed(0, 1);
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
                        loadRecipeBookLabels();
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
                        loadRecipeBookLabels();
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
                showLeftArrow(-128.0f, 0);
            } else {
                hideLeftArrow();
            }
            if (overlayScreenTable.pageNumber != 2) {
                showRightArrow(128.0f, 0);
            } else {
                hideRightArrow();
            }

            break;
    
    
    }

    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        hideRightArrow();
        hideLeftArrow();
        resetAnimationState(CURSOR_HAND);
        fadeOutRecipeBookSprites();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}


/* album functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadAlbumScreenSprites);

void loadAlbumScreenSprites(void) {
    
    dmaSprite(ALBUM_BACKGROUND, (u32)&_bookTextureSegmentRomStart, (u32)&_bookTextureSegmentRomEnd, (u32)&_bookAssetsIndexSegmentRomStart, (u32)&_bookAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(ALBUM_BACKGROUND, 0.0f, 0.0f, 8.0f);
    setSpriteColor(ALBUM_BACKGROUND, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(ALBUM_BACKGROUND, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(ALBUM_BACKGROUND, TRUE);
    setSpritePaletteIndex(ALBUM_BACKGROUND, 1);
    startSpriteAnimation(ALBUM_BACKGROUND, 0, 0);
    
    // "Album" label
    dmaSprite(0x82, (u32)&_bookScreenLabelsTextureSegmentRomStart, (u32)&_bookScreenLabelsTextureSegmentRomEnd, (u32)&_bookScreenLabelsAssetsIndexSegmentRomStart, (u32)&_bookScreenLabelsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x82, TRUE);
    startSpriteAnimation(0x82, 3, 0);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, TRUE);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // arrows
    loadNavigationArrows();
    loadAlbumPhotos();
    
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    
    fadeInAlbumSprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadAlbumPhotos);

void loadAlbumPhotos(void) {

    resetAnimationState(0x83);
    resetAnimationState(0x84);
    resetAnimationState(0x85);
    resetAnimationState(0x86);

    switch (overlayScreenTable.pageNumber) {

        case 0:

            if (albumBits & 1) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteBlendMode(0x83, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 0, 0);
            }
            
            if (albumBits & 2) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteBlendMode(0x84, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 1, 0);
            }
            
            if (albumBits & 4) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteBlendMode(0x85, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 2, 0);
            }
            
            if (albumBits & 8) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteBlendMode(0x86, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 3, 0);
            }
            
            break;

        case 1:

            if (albumBits & 0x10) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteBlendMode(0x83, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 4, 0);
            }
            
            if (albumBits & 0x20) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteBlendMode(0x84, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 5, 0);
            }
            
            if (albumBits & 0x40) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteBlendMode(0x85, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 6, 0);
            }
            
            if (albumBits & 0x80) {
                
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteBlendMode(0x86, SPRITE_BLEND_ALPHA_MODULATED);
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
                
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteBlendMode(0x83, SPRITE_BLEND_ALPHA_MODULATED);
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
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteBlendMode(0x84, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 0x11, 0);
            }
            
            if (albumBits & 0x400) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteBlendMode(0x85, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 0x12, 0);
            }
            
            if (albumBits & 0x800) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteBlendMode(0x86, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 0x13, 0);
            }
            
            break;

        case 3:

            if (albumBits & 0x1000) {
                dmaSprite(0x83, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_1_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x83, -64.0f, 40.0f, 8.0f);
                setSpriteColor(0x83, 255, 255, 255, 255);
                setSpriteBlendMode(0x83, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x83, 1);
                startSpriteAnimation(0x83, 0x14, 0);
            }
            
            if (albumBits & 0x2000) {
                dmaSprite(0x84, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_2_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x84, -64.0f, -30.0f, 8.0f);
                setSpriteColor(0x84, 255, 255, 255, 255);
                setSpriteBlendMode(0x84, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x84, 1);
                startSpriteAnimation(0x84, 0x15, 0);
            }
            
            if (albumBits & 0x4000) {
                dmaSprite(0x85, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_3_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x85, 64.0f, 40.0f, 8.0f);
                setSpriteColor(0x85, 255, 255, 255, 255);
                setSpriteBlendMode(0x85, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x85, 1);
                startSpriteAnimation(0x85, 0x16, 0);
            }
            
            if (albumBits & 0x8000) {
                dmaSprite(0x86, (u32)&_albumPhotosTextureSegmentRomStart, (u32)&_albumPhotosTextureSegmentRomEnd, (u32)&_albumPhotosAssetsIndexSegmentRomStart, (u32)&_albumPhotosAssetsIndexSegmentRomEnd, (u32)&_albumPhotosSpritesheetIndexSegmentRomStart, (u32)&_albumPhotosSpritesheetIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_1, (u8*)OVERLAY_SCREEN_ALBUM_PHOTO_SLOT_4_TEXTURE_2, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_PALETTE, (u16*)OVERLAY_SCREEN_ALBUM_PHOTOS_ANIM_METADATA, (u8*)OVERLAY_SCREEN_ALBUM_PHOTOS_SPRITESHEET_INDEX, (u32*)OVERLAY_SCREEN_ALBUM_PHOTOS_TEXTURE_TO_PALETTE_LOOKUP, 1, 0);
                setSpriteViewSpacePosition(0x86, 64.0f, -30.0f, 8.0f);
                setSpriteColor(0x86, 255, 255, 255, 255);
                setSpriteBlendMode(0x86, SPRITE_BLEND_ALPHA_MODULATED);
                setBilinearFiltering(0x86, 1);
                startSpriteAnimation(0x86, 0x17, 0);
            }
            
            break;
        
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInAlbumSprites);

void fadeInAlbumSprites(void) {

    fadeInClock();

    updateSpriteAlpha(ALBUM_BACKGROUND, 0xFF, 24);
    
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
    updateSpriteAlpha(0x86, 0xFF, 24);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutAlbumSprites);

void fadeOutAlbumSprites(void) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", albumScreenCallback);

void albumScreenCallback(void) {

    bool set;
    u8 *ptr = D_80121E34;
    
    u8 buff[16];

    memcpy(buff, ptr, 16);
    
    set = FALSE;
    
    switch (overlayScreenTable.screenState) {

        case 0:
            loadAlbumScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                setSpriteViewSpacePosition(CURSOR_HAND, (f32)((overlayScreenTable.unk_8 * 128) - 120), (f32)(40 - (overlayScreenTable.unk_9 * 80)), 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 5;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;
        
        case 4:
            if (checkAnyMessageBoxTextFinished()) {
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
                        loadAlbumPhotos();
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
                        loadAlbumPhotos();
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
                        setMessageBoxLineAndRowSizes(0, 0x10, 3);
                        setMessageBoxInterpolationWithFlags(0, -4, 2);
    
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
                showLeftArrow(-128.0f, 8.0f);
            } else {
                hideLeftArrow();
            }
                
            if (overlayScreenTable.pageNumber != 3) {
                showRightArrow(128.0f, 8.0f);
            } else {
                hideRightArrow();
            }
            
            break;
    
    }
    
    if (overlayScreenTable.screenState >= 5 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        hideRightArrow();
        hideLeftArrow();
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadNavigationArrows);

void loadNavigationArrows(void) {

    dmaSprite(DOWN_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(DOWN_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(DOWN_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(DOWN_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(DOWN_ARROW, 1);
    setSpriteColor(DOWN_ARROW, 255, 255, 255, 255);
    setSpriteAnchorAlignment(DOWN_ARROW, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
    setSpriteBlendMode(DOWN_ARROW, 3);
    
    dmaSprite(UP_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(UP_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(UP_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(UP_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(UP_ARROW, 1);
    setSpriteColor(UP_ARROW, 255, 255, 255, 255);
    setSpriteAnchorAlignment(UP_ARROW, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
    setSpriteBlendMode(UP_ARROW, 3);
    
    dmaSprite(RIGHT_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(RIGHT_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(RIGHT_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(RIGHT_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(RIGHT_ARROW, 1);
    setSpriteColor(RIGHT_ARROW, 255, 255, 255, 255);
    setSpriteAnchorAlignment(RIGHT_ARROW, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
    setSpriteBlendMode(RIGHT_ARROW, 3);
    
    dmaSprite(LEFT_ARROW, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(LEFT_ARROW, 0.0f, 0.0f, 0.0f);
    setSpriteScale(LEFT_ARROW, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(LEFT_ARROW, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(LEFT_ARROW, 1);
    setSpriteColor(LEFT_ARROW, 255, 255, 255, 255);
    setSpriteAnchorAlignment(LEFT_ARROW, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
    setSpriteBlendMode(LEFT_ARROW, 3);

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", showDownArrow);

void showDownArrow(f32 x, f32 y) {
    startSpriteAnimation(DOWN_ARROW, 11, 0xFE);
    setSpriteViewSpacePosition(DOWN_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", showUpArrow);

void showUpArrow(f32 x, f32 y) {
    startSpriteAnimation(UP_ARROW, 10, 0xFE);
    setSpriteViewSpacePosition(UP_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", showLeftArrow);

void showLeftArrow(f32 x, f32 y) {
    startSpriteAnimation(LEFT_ARROW, 1, 0xFE);
    setSpriteViewSpacePosition(LEFT_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", showRightArrow);

void showRightArrow(f32 x, f32 y) {
    startSpriteAnimation(RIGHT_ARROW, 12, 0xFE);
    setSpriteViewSpacePosition(RIGHT_ARROW, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", hideDownArrow);

void hideDownArrow(void) {
    resetAnimationState(DOWN_ARROW);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", hideUpArrow);

void hideUpArrow(void) {
    resetAnimationState(UP_ARROW);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", hideLeftArrow);

void hideLeftArrow(void) {
    resetAnimationState(LEFT_ARROW);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", hideRightArrow);

void hideRightArrow(void) {
    resetAnimationState(RIGHT_ARROW);
}


// up and down arrows

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadDialogueArrowSprites);

void loadDialogueArrowSprites(void) {
    
    dmaSprite(0x7A, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x7A, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7A, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7A, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7A, 1);
    setSpriteColor(0x7A, 255, 255, 255, 255);
    setSpriteAnchorAlignment(0x7A, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
    setSpriteBlendMode(0x7A, 3);

    dmaSprite(0x7B, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x7B, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x7B, 1.0f, 1.0f, 1.0f);
    setSpriteRotation(0x7B, 0.0f, 0.0f, 0.0f);
    setBilinearFiltering(0x7B, 1);
    setSpriteColor(0x7B, 255, 255, 255, 255);
    setSpriteAnchorAlignment(0x7B, SPRITE_ANCHOR_CENTER, SPRITE_ANCHOR_CENTER);;
    setSpriteBlendMode(0x7B, 3);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", showDialogueDownArrow);

void showDialogueDownArrow(f32 x, f32 y) {
    startSpriteAnimation(0x7B, 11, 0xFE);
    setSpriteViewSpacePosition(0x7B, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", showDialogueUpArrow);

void showDialogueUpArrow(f32 x, f32 y) {
    startSpriteAnimation(0x7A, 10, 0xFE);
    setSpriteViewSpacePosition(0x7A, x, y, 32.0f);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", hideDialogueDownArrow);

void hideDialogueDownArrow(void) {
    resetAnimationState(0x7B);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", hideDialogueUpArrow);

void hideDialogueUpArrow(void) {
    resetAnimationState(0x7A);
}

/* end of shared arrow functions */


/* shared helper */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", setItemDescriptionVariable);

void setItemDescriptionVariable(u8 type, u32 itemOrToolIndex) {

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
    setMessageBoxLineAndRowSizes(0, 0x10, 3);
    setMessageBoxInterpolationWithFlags(0, -4, 2);
    initializeMessageBox(0, 2, temp, 0);
    
    controllers[CONTROLLER_1].buttonPressed = 0;
    
}


/* pause screen map functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadPauseScreenMapScreenCallback);

// main loop callback 30
// seems like load village map (cut from NA release) 
void loadPauseScreenMapScreenCallback(void) {
    
    dmaSprite(0x82, (u32)&_villageMapScreenTextureSegmentRomStart, (u32)&_villageMapScreenTextureSegmentRomEnd, (u32)&_villageMapScreenAssetsIndexSegmentRomStart, (u32)&_villageMapScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 0.0f);
    setSpriteScale(0x82, 1.0f, 1.0f, 1.0f);
    setBilinearFiltering(0x82, 1);
    setSpriteColor(0x82, 0, 0, 0, 0);
    startSpriteAnimation(0x82, 0, 0);
    
    dmaSprite(0xA3, (u32)&_villageMapScreenTextureSegmentRomStart, (u32)&_villageMapScreenTextureSegmentRomEnd, (u32)&_villageMapScreenAssetsIndexSegmentRomStart, (u32)&_villageMapScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_PAUSE_SCREEN_MAP_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", pauseScreenMapCallback);

void pauseScreenMapCallback(void) {    

    u8 temp = 0;

    switch (overlayScreenTable.screenState) {
        
        case 0:
            updateSpriteRGBA(0x82, 255, 255, 255, 255, 24);
            updateSpriteRGBA(0xA3, 255, 255, 255, 255, 24);
            overlayScreenTable.screenState = 1;
            break;
            
        case 1:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
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
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 4;
            }
            break;
            
        case 4:
            initializePauseScreenBackground();
            break;

        }
    
}

/* end pause screen map functions */


/* horse/dog race functions */

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRaceBettingScreenCallback);

void loadRaceBettingScreenCallback(void) {

    openOverlayScreen();
    
    if (checkDailyEventBit(HORSE_RACE)) {
        removeKeyItem(0xE);
    } else {
        removeKeyItem(0xF);
    }

    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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
    
    gRacingContext.playerBets[gRacingContext.currentRaceIndex][0] = 0;
    gRacingContext.playerBets[gRacingContext.currentRaceIndex][1] = 0;
    gRacingContext.playerBets[gRacingContext.currentRaceIndex][2] = 0;
    gRacingContext.playerBets[gRacingContext.currentRaceIndex][3] = 0;
    gRacingContext.playerBets[gRacingContext.currentRaceIndex][4] = 0;
    gRacingContext.playerBets[gRacingContext.currentRaceIndex][5] = 0;

    setMainLoopCallbackFunctionIndex(HORSE_RACE_BETTING);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRaceBettingScreenSprites);

void loadRaceBettingScreenSprites(void) {
    
    D_80189858 = gGold;

    setMoneySprites();
    
    dmaSprite(0x81, (u32)&_photoSmallTextureSegmentRomStart, (u32)&_photoSmallTextureSegmentRomEnd, (u32)&_photoSmallAssetsIndexSegmentRomStart, (u32)&_photoSmallAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);

    dmaSprite(0x82, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, SPRITE_BLEND_ALPHA_MODULATED);
    setBilinearFiltering(0x82, 1);

    // check horse or dog race
    if (checkDailyEventBit(HORSE_RACE)) {
        startSpriteAnimation(0x82, 0, 0);
    } else {
        startSpriteAnimation(0x82, 0, 1);
    }
    
    dmaSprite(0x83, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);

    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 1, 0);

    dmaSprite(0x84, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 2, 0);
    
    dmaSprite(0xB9, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xB9, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0xB9, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0xB9, 2);
    setBilinearFiltering(0xB9, 1);
    startSpriteAnimation(0xB9, 1, 1);

    dmaSprite(0xBA, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBA, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0xBA, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0xBA, 2);
    setBilinearFiltering(0xBA, 1);
    startSpriteAnimation(0xBA, 1, 1);
    
    dmaSprite(0xBB, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBB, 0.0f, -32.0f, 8.0f);
    setSpriteColor(0xBB, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0xBB, 2);
    setBilinearFiltering(0xBB, 1);
    startSpriteAnimation(0xBB, 1, 1);
    
    dmaSprite(0xBC, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBC, 0.0f, -48.0f, 8.0f);
    setSpriteColor(0xBC, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0xBC, 2);
    setBilinearFiltering(0xBC, 1);
    startSpriteAnimation(0xBC, 1, 1);
    
    dmaSprite(0xBD, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBD, 0.0f, -64.0f, 8.0f);
    setSpriteColor(0xBD, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0xBD, 2);
    setBilinearFiltering(0xBD, 1);
    startSpriteAnimation(0xBD, 1, 1);
    
    dmaSprite(0xBE, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0xBE, 0.0f, -80.0f, 8.0f);
    setSpriteColor(0xBE, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0xBE, 2);
    setBilinearFiltering(0xBE, 1);
    startSpriteAnimation(0xBE, 1, 1);
    
    dmaSprite(0x85, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x85, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 2, 1);

    dmaSprite(0x86, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x86, 0.0f, 0.0f, 32.0f);
    setSpriteColor(0x86, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x86, 2);
    setBilinearFiltering(0x86, 1);
    startSpriteAnimation(0x86, 2, 0);

    dmaSprite(CURSOR_HAND, &_dialogueIconsTextureSegmentRomStart, &_dialogueIconsTextureSegmentRomEnd, &_dialogueIconsAssetsIndexSegmentRomStart, &_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    setOverlayScreenSprites(2, 0x97, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -98.0f, -153.0f, 256.0f, 10);
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 255, 255, 255, 255);

    setOverlayScreenSprites(3, 0x9B, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -48.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(3, gRacingContext.playerBets[gRacingContext.currentRaceIndex][0], 1, 3);
    setOverlayScreenSpritesRGBA(3, 255, 255, 255, 255);

    setOverlayScreenSprites(4, 0x9D, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -64.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(4, gRacingContext.playerBets[gRacingContext.currentRaceIndex][1], 1, 3);
    setOverlayScreenSpritesRGBA(4, 255, 255, 255, 255);
    
    setOverlayScreenSprites(5, 0x9F, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -80.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(5, gRacingContext.playerBets[gRacingContext.currentRaceIndex][2], 1, 3);
    setOverlayScreenSpritesRGBA(5, 255, 255, 255, 255);
    
    setOverlayScreenSprites(6, 0xA1, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -96.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(6, gRacingContext.playerBets[gRacingContext.currentRaceIndex][3], 1, 3);
    setOverlayScreenSpritesRGBA(6, 255, 255, 255, 255);
    
    setOverlayScreenSprites(7, 0xA3, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -112.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(7, gRacingContext.playerBets[gRacingContext.currentRaceIndex][4], 1, 3);
    setOverlayScreenSpritesRGBA(7, 255, 255, 255, 255);
    
    setOverlayScreenSprites(8, 0xA5, &_moneyTextureSegmentRomStart, &_moneyTextureSegmentRomEnd, &_moneyAssetsIndexSegmentRomStart, &_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -128.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(8, gRacingContext.playerBets[gRacingContext.currentRaceIndex][5], 1, 3);
    setOverlayScreenSpritesRGBA(8, 255, 255, 255, 255);

    setOverlayScreenSprites(9, 0xA7, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 14.0f, 34.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(9, gRacingContext.displayOddsTop[0], 2, 3);
    setOverlayScreenSpritesRGBA(9, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xA, 0xAA, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 14.0f, 18.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xA, gRacingContext.displayOddsTop[1], 2, 3);
    setOverlayScreenSpritesRGBA(0xA, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xB, 0xAD, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 14.0f, 2.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xB, gRacingContext.displayOddsTop[2], 2, 3);
    setOverlayScreenSpritesRGBA(0xB, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xC, 0xB0, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 14.0f, -14.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xC, gRacingContext.oddsAndNames.displayOddsBottom[0], 2, 3);
    setOverlayScreenSpritesRGBA(0xC, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xD, 0xB3, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 14.0f, -30.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xD, gRacingContext.oddsAndNames.displayOddsBottom[1], 2, 3);
    setOverlayScreenSpritesRGBA(0xD, 255, 255, 255, 255);
    
    setOverlayScreenSprites(0xE, 0xB6, &_raceBettingUiTextureSegmentRomStart, &_raceBettingUiTextureSegmentRomEnd, &_raceBettingUiAssetsIndexSegmentRomStart, &_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 14.0f, -46.0f, 256.0f, 8);
    
    dmaOverlayScreenSprites(0xE, gRacingContext.oddsAndNames.displayOddsBottom[2], 2, 3);
    setOverlayScreenSpritesRGBA(0xE, 255, 255, 255, 255);
    
    initializeRaceBettingMessageBoxes();
    fadeInRaceBettingSprites();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", initializeRaceBettingMessageBoxes);

void initializeRaceBettingMessageBoxes(void) {
    
    setGameVariableString(0x2A, gRacingContext.racerNames[0], 6);
    setGameVariableString(0x2B, gRacingContext.racerNames[1], 6);
    setGameVariableString(0x2C, gRacingContext.racerNames[2], 6);
    setGameVariableString(0x2D, gRacingContext.racerNames[3], 6);
    setGameVariableString(0x2E, gRacingContext.racerNames[4], 6);
    setGameVariableString(0x2F, gRacingContext.racerNames[5], 6);
    
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -22.0f, 24.0f, 0);
    setMessageBoxLineAndRowSizes(0, 0xA, 2);
    setMessageBoxSpacing(0, 0, 2);
    setMessageBoxFont(0, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    setMessageBoxScrollSpeed(0, 1);
    initializeMessageBox(0, 0, 0x12, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(0, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -22.0f, 8.0f, 0);
    setMessageBoxLineAndRowSizes(1, 0xA, 2);
    setMessageBoxSpacing(1, 0, 2);
    setMessageBoxFont(1, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    setMessageBoxScrollSpeed(1, 1);
    initializeMessageBox(1, 0, 0x13, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(1, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(2, -22.0f, -8.0f, 0);
    setMessageBoxLineAndRowSizes(2, 0xA, 2);
    setMessageBoxSpacing(2, 0, 2);
    setMessageBoxFont(2, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    setMessageBoxScrollSpeed(2, 1);
    initializeMessageBox(2, 0, 0x14, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(2, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, -22.0f, -24.0f, 0);
    setMessageBoxLineAndRowSizes(3, 0xA, 2);
    setMessageBoxSpacing(3, 0, 2);
    setMessageBoxFont(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    setMessageBoxScrollSpeed(3, 1);
    initializeMessageBox(3, 0, 0x15, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(3, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, -22.0f, -40.0f, 0);
    setMessageBoxLineAndRowSizes(4, 0xA, 2);
    setMessageBoxSpacing(4, 0, 2);
    setMessageBoxFont(4, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    setMessageBoxScrollSpeed(4, 1);
    initializeMessageBox(4, 0, 0x16, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(4, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(5, (u8*)MESSAGE_BOX_6_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(5, -22.0f, -56.0f, 0);
    setMessageBoxLineAndRowSizes(5, 0xA, 2);
    setMessageBoxSpacing(5, 0, 2);
    setMessageBoxFont(5, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(5, -4, 2);
    setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
    setMessageBoxButtonMask(5, BUTTON_A);
    setMessageBoxScrollSpeed(5, 1);
    initializeMessageBox(5, 0, 0x17, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(5, 255, 255, 255, 255);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInRaceBettingElements);

void fadeInRaceBettingSprites(void) {
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutRaceBettingElements);

void fadeOutRaceBettingSprites(void) {

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

    deactivateMessageBox(0);
    deactivateMessageBox(1);
    deactivateMessageBox(2);
    deactivateMessageBox(3);
    deactivateMessageBox(4);
    deactivateMessageBox(5);

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

    initializeMainMessageBoxes();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", raceBettingScreenCallback);

void raceBettingScreenCallback(void) {

    u8 temp = 0;
    bool set = FALSE;
    
    switch (overlayScreenTable.screenState) {
        
        case 0:
            loadRaceBettingScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                setSpriteViewSpacePosition(CURSOR_HAND, -112.0f, (-(overlayScreenTable.cellIndex * 16) + 24), 32.0f);
                startSpriteAnimation(CURSOR_HAND, 3, 0);
                overlayScreenTable.screenState = 4;
            }
            
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
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
                        
                        if (gRacingContext.playerBets[gRacingContext.currentRaceIndex][overlayScreenTable.cellIndex]) {
                            D_80189858 += adjustValue(D_80189858, 50, gGold);
                            gRacingContext.playerBets[gRacingContext.currentRaceIndex][overlayScreenTable.cellIndex]--;
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

                        if (gRacingContext.playerBets[gRacingContext.currentRaceIndex][overlayScreenTable.cellIndex] != 99) {
                            D_80189858 += adjustValue(D_80189858, -50, gGold);
                            gRacingContext.playerBets[gRacingContext.currentRaceIndex][overlayScreenTable.cellIndex]++;
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
            dmaOverlayScreenSprites(overlayScreenTable.cellIndex + 3, gRacingContext.playerBets[gRacingContext.currentRaceIndex][overlayScreenTable.cellIndex], 1, 3);
                
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
                    
                    if (gRacingContext.playerBets[gRacingContext.currentRaceIndex][0] 
                        || gRacingContext.playerBets[gRacingContext.currentRaceIndex][1] 
                        || gRacingContext.playerBets[gRacingContext.currentRaceIndex][2] 
                        || gRacingContext.playerBets[gRacingContext.currentRaceIndex][3] 
                        || gRacingContext.playerBets[gRacingContext.currentRaceIndex][4] 
                        || gRacingContext.playerBets[gRacingContext.currentRaceIndex][5]) {
                        
                        gRacingContext.betPlacedFlags[gRacingContext.currentRaceIndex] = 0xFF;
                        gGold = D_80189858;
                        
                        if (checkDailyEventBit(HORSE_RACE)) {
                            acquireKeyItem(HORSE_RACE_TICKET);
                        } else {
                            acquireKeyItem(DOG_RACE_TICKET);
                        }
                        
                    }
                    
                    resetAnimationState(CURSOR_HAND);
                    set = TRUE;
                    
                    fadeOutRaceBettingSprites();
                    overlayScreenTable.screenState = 2;
                    
                    playSfx(0);
                    
                }
            }

            break;
        
        }
    
    if (overlayScreenTable.screenState >= 4 && checkButtonPressed(CONTROLLER_1, BUTTON_B) && !set) {
        resetAnimationState(CURSOR_HAND);
        fadeOutRaceBettingSprites();
        overlayScreenTable.screenState = 2;
        playSfx(1);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateHorseRaceContext);

void updateHorseRaceContext(void) {

    u8 buffer[20][6];
    u8 temp;

    memcpy(buffer, D_801220CC, 114);

    if (gHour < 24) {
        gRacingContext.currentRaceIndex = 3;
    }
    if (gHour < 16) {
        gRacingContext.currentRaceIndex = 2;
    }
    if (gHour < 13) {
        gRacingContext.currentRaceIndex = 1;
    }
    if (gHour < 10) {
        gRacingContext.currentRaceIndex = 0;
    }

    gRacingContext.racerNames[0][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][0];
    gRacingContext.racerNames[0][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][1];
    gRacingContext.racerNames[0][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][2];
    gRacingContext.racerNames[0][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][3];
    gRacingContext.racerNames[0][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][4];
    gRacingContext.racerNames[0][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][5];

    gRacingContext.racerNames[1][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][0];
    gRacingContext.racerNames[1][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][1];
    gRacingContext.racerNames[1][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][2];
    gRacingContext.racerNames[1][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][3];
    gRacingContext.racerNames[1][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][4];
    gRacingContext.racerNames[1][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][5];

    gRacingContext.racerNames[2][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][0];
    gRacingContext.racerNames[2][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][1];
    gRacingContext.racerNames[2][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][2];
    gRacingContext.racerNames[2][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][3];
    gRacingContext.racerNames[2][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][4];
    gRacingContext.racerNames[2][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][5];

    gRacingContext.racerNames[3][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][0];
    gRacingContext.racerNames[3][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][1];
    gRacingContext.racerNames[3][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][2];
    gRacingContext.racerNames[3][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][3];
    gRacingContext.racerNames[3][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][4];
    gRacingContext.racerNames[3][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][5];

    gRacingContext.racerNames[4][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][0];
    gRacingContext.racerNames[4][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][1];
    gRacingContext.racerNames[4][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][2];
    gRacingContext.racerNames[4][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][3];
    gRacingContext.racerNames[4][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][4];
    gRacingContext.racerNames[4][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][5];

    gRacingContext.racerNames[5][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][0];
    gRacingContext.racerNames[5][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][1];
    gRacingContext.racerNames[5][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][2];
    gRacingContext.racerNames[5][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][3];
    gRacingContext.racerNames[5][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][4];
    gRacingContext.racerNames[5][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][5];

    gRacingContext.displayOddsTop[0] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][0];
    gRacingContext.displayOddsTop[1] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][1];
    gRacingContext.displayOddsTop[2] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][2];
    gRacingContext.oddsAndNames.displayOddsBottom[0] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][3];
    gRacingContext.oddsAndNames.displayOddsBottom[1]= gRacingContext.racerOdds[gRacingContext.currentRaceIndex][4];
    gRacingContext.oddsAndNames.displayOddsBottom[2] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][5];

    if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
        
        gRacingContext.racerNames[2][0] = horseInfo.name[0];
        gRacingContext.racerNames[2][1] = horseInfo.name[1];
        gRacingContext.racerNames[2][2] = horseInfo.name[2];
        gRacingContext.racerNames[2][3] = horseInfo.name[3];
        gRacingContext.racerNames[2][4] = horseInfo.name[4];
        gRacingContext.racerNames[2][5] = horseInfo.name[5];
        
        D_801C3F78 |= 0x20;
        
    } else {
        D_801C3F78 &= ~0x20;
    }
    
    if (gRacingContext.unk_6E == gRacingContext.currentRaceIndex) {
        D_801C3F78 |= 0x40;
    } else {
        D_801C3F78 &= ~0x40;
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", updateDogRaceContext);

void updateDogRaceContext(void) {

    u8 buffer[20][6];
    u8 temp;

    memcpy(buffer, D_80122140, 114);

    if (gHour < 24) {
        gRacingContext.currentRaceIndex = 3;
    }
    if (gHour < 16) {
        gRacingContext.currentRaceIndex = 2;
    }
    if (gHour < 13) {
        gRacingContext.currentRaceIndex = 1;
    }
    if (gHour < 10) {
        gRacingContext.currentRaceIndex = 0;
    }

    gRacingContext.racerNames[0][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][0];
    gRacingContext.racerNames[0][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][1];
    gRacingContext.racerNames[0][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][2];
    gRacingContext.racerNames[0][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][3];
    gRacingContext.racerNames[0][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][4];
    gRacingContext.racerNames[0][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][0]][5];

    gRacingContext.racerNames[1][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][0];
    gRacingContext.racerNames[1][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][1];
    gRacingContext.racerNames[1][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][2];
    gRacingContext.racerNames[1][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][3];
    gRacingContext.racerNames[1][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][4];
    gRacingContext.racerNames[1][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][1]][5];

    gRacingContext.racerNames[2][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][0];
    gRacingContext.racerNames[2][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][1];
    gRacingContext.racerNames[2][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][2];
    gRacingContext.racerNames[2][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][3];
    gRacingContext.racerNames[2][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][4];
    gRacingContext.racerNames[2][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][2]][5];

    gRacingContext.racerNames[3][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][0];
    gRacingContext.racerNames[3][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][1];
    gRacingContext.racerNames[3][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][2];
    gRacingContext.racerNames[3][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][3];
    gRacingContext.racerNames[3][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][4];
    gRacingContext.racerNames[3][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][3]][5];

    gRacingContext.racerNames[4][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][0];
    gRacingContext.racerNames[4][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][1];
    gRacingContext.racerNames[4][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][2];
    gRacingContext.racerNames[4][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][3];
    gRacingContext.racerNames[4][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][4];
    gRacingContext.racerNames[4][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][4]][5];

    gRacingContext.racerNames[5][0] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][0];
    gRacingContext.racerNames[5][1] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][1];
    gRacingContext.racerNames[5][2] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][2];
    gRacingContext.racerNames[5][3] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][3];
    gRacingContext.racerNames[5][4] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][4];
    gRacingContext.racerNames[5][5] = buffer[gRacingContext.racerNameIndices[gRacingContext.currentRaceIndex * 2][5]][5];

    gRacingContext.displayOddsTop[0] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][0];
    gRacingContext.displayOddsTop[1] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][1];
    gRacingContext.displayOddsTop[2] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][2];
    gRacingContext.oddsAndNames.displayOddsBottom[0] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][3];
    gRacingContext.oddsAndNames.displayOddsBottom[1]= gRacingContext.racerOdds[gRacingContext.currentRaceIndex][4];
    gRacingContext.oddsAndNames.displayOddsBottom[2] = gRacingContext.racerOdds[gRacingContext.currentRaceIndex][5];

    if (gRacingContext.playerRaceNumber == gRacingContext.currentRaceIndex) {
        
        gRacingContext.racerNames[0][0] = dogInfo.name[0];
        gRacingContext.racerNames[0][1] = dogInfo.name[1];
        gRacingContext.racerNames[0][2] = dogInfo.name[2];
        gRacingContext.racerNames[0][3] = dogInfo.name[3];
        gRacingContext.racerNames[0][4] = dogInfo.name[4];
        gRacingContext.racerNames[0][5] = dogInfo.name[5];
        
        D_801C3F78 |= 0x20;
        
    } else {
        D_801C3F78 &= ~0x20;
    }
    
    if (gRacingContext.unk_6E == gRacingContext.currentRaceIndex) {
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

        if (gRacingContext.racerOdds[arg0][i] == arg2) {
            result = TRUE;
        }
        
    }
    
    return result;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", initializeRaceContext);

void initializeRaceContext(void) {

    u8 arr[19];
    
    u8 i;
    
    u8 temp;
    u8 temp2;
    u8 temp3;
    u8 temp4;

    for (i = 0; i < 19; i++) {
        arr[i] = 0;
    }

    gRacingContext.playerRaceNumber = 0xFF;

    if ((checkLifeEventBit(0x51)) || (checkLifeEventBit(0x52))) {
        gRacingContext.playerRaceNumber = getRandomNumberInRange(0, 2);
    }

    i = 0;

    temp4 = getRandomNumberInRange(40, 100);
    
    for (i = 0; i < 6; ) {

        temp = getRandomNumberInRange(1, 18); 

        if (arr[temp] == 0) {

            gRacingContext.racerNameIndices[0][i] = temp;

            temp2 = getRandomNumberInRange(1, temp4 / 2);

            temp4 -= temp2;

            gRacingContext.playerBets[3][i] = temp2;

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

            gRacingContext.racerNameIndices[2][i] = temp;

            temp2 = getRandomNumberInRange(1, temp4 / 2);

            temp4 -= temp2;

            gRacingContext.playerBets[4][i] = temp2;

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

            gRacingContext.racerNameIndices[4][i] = temp;

            temp2 = getRandomNumberInRange(1, temp4 / 2);

            temp4 -= temp2;

            gRacingContext.playerBets[5][i] = temp2;

            if (temp4 < 2) {
                temp4 = 2;
            }

            arr[temp] = 0xFF;

            i++;
            
        }
        
    }

    if (checkLifeEventBit(0x51)) {
        temp3 = getRandomNumberInRange(0, 2);
        gRacingContext.racerNameIndices[1][temp3 * 6] = 0;
        gRacingContext.unk_6E = temp3;
    }

    if (checkLifeEventBit(0x52)) {
        temp3 = getRandomNumberInRange(0, 2);
        gRacingContext.racerNameIndices[0][temp3 * 6 + 1] = 0;
        gRacingContext.unk_6E = temp3;
    }
    
    gRacingContext.playerBets[0][0] = 0;
    gRacingContext.playerBets[0][1] = 0;
    gRacingContext.playerBets[0][2] = 0;
    gRacingContext.playerBets[0][3] = 0;
    gRacingContext.playerBets[0][4] = 0;
    gRacingContext.playerBets[0][5] = 0;
    
    gRacingContext.playerBets[1][0] = 0;
    gRacingContext.playerBets[1][1] = 0;
    gRacingContext.playerBets[1][2] = 0;
    gRacingContext.playerBets[1][3] = 0;
    gRacingContext.playerBets[1][4] = 0;
    gRacingContext.playerBets[1][5] = 0;
    
    gRacingContext.playerBets[2][0] = 0;
    gRacingContext.playerBets[2][1] = 0;
    gRacingContext.playerBets[2][2] = 0;
    gRacingContext.playerBets[2][3] = 0;
    gRacingContext.playerBets[2][4] = 0;
    gRacingContext.playerBets[2][5] = 0;
    
    gRacingContext.betPlacedFlags[0] = 0;
    gRacingContext.betPlacedFlags[1] = 0;
    gRacingContext.betPlacedFlags[2] = 0;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRaceResultsScreenCallback);

void loadRaceResultsScreenCallback(void) {

    openOverlayScreen();
    
    if (checkDailyEventBit(HORSE_RACE)) {
        removeKeyItem(0xE);
    } else {
        removeKeyItem(0xF);
    }
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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
    
    gRacingContext.finishOrder[0] = gRaceFinishOrder[0] - 1;
    gRacingContext.finishOrder[1] = gRaceFinishOrder[1] - 1;
    gRacingContext.finishOrder[2] = gRaceFinishOrder[2] - 1;
    gRacingContext.finishOrder[3] = gRaceFinishOrder[3] - 1;
    gRacingContext.finishOrder[4] = gRaceFinishOrder[4] - 1;
    gRacingContext.finishOrder[5] = gRaceFinishOrder[5] - 1;
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRaceResultsScreenSprites);

void loadRaceResultsScreenSprites(void) {

    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, 0, 0, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u8*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, 0);
    setSpriteViewSpacePosition(0x81, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 3, gRacingContext.currentRaceIndex);
    
    dmaSprite(0x83, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, 0);
    
    dmaSprite(0x84, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 4, 1);
    
    dmaSprite(0x85, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, 0, 0, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, 0);
    setSpriteViewSpacePosition(0x85, 0.0f, -16.0f, 8.0f);
    setSpriteColor(0x85, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x85, 2);
    setBilinearFiltering(0x85, 1);
    startSpriteAnimation(0x85, 4, 1);
    
    setOverlayScreenSprites(2, 0x97, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -153.0f, 256.0f, 10);
    
    D_80205204 += adjustValue(D_80205204, 
          gRacingContext.displayOddsTop[gRacingContext.finishOrder[0]] * gRacingContext.playerBets[gRacingContext.currentRaceIndex][gRacingContext.finishOrder[0]], 
          9999);
    
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setOverlayScreenSprites(3, 0x9B, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -48.0f, -137.0f, 256.0f, 10);
    dmaOverlayScreenSprites(3, gRacingContext.displayOddsTop[gRacingContext.finishOrder[0]] * gRacingContext.playerBets[gRacingContext.currentRaceIndex][gRacingContext.finishOrder[0]], 3, 3);
    setOverlayScreenSpritesRGBA(3, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setOverlayScreenSprites(8, 0xA5, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -24.0f, -112.0f, 256.0f, 10);
    dmaOverlayScreenSprites(8, gRacingContext.playerBets[gRacingContext.currentRaceIndex][gRacingContext.finishOrder[0]], 1, 3);
    setOverlayScreenSpritesRGBA(8, 0xFF, 0xFF, 0xFF, 0xFF);
    
    setOverlayScreenSprites(9, 0xA7, (u32)&_raceBettingUiTextureSegmentRomStart, (u32)&_raceBettingUiTextureSegmentRomEnd, (u32)&_raceBettingUiAssetsIndexSegmentRomStart, (u32)&_raceBettingUiAssetsIndexSegmentRomEnd, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 6, 0, 40.0f, -28.0f, 256.0f, 8);
    dmaOverlayScreenSprites(9, gRacingContext.displayOddsTop[gRacingContext.finishOrder[0]], 2, 3);
    setOverlayScreenSpritesRGBA(9, 0xFF, 0xFF, 0xFF, 0xFF);
    
    initializeRaceResultsMessageBoxes();
    fadeInRaceResultsSprites();
    
    if (gRacingContext.playerBets[gRacingContext.currentRaceIndex][gRacingContext.finishOrder[0]] && !(checkHaveKeyItem(0xD))) {
        acquireKeyItem(MEDAL_BAG);
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", initializeRaceResultsMessageBoxes);

void initializeRaceResultsMessageBoxes(void) {

    setGameVariableString(0x2A, gRacingContext.racerNames[gRacingContext.finishOrder[0]], 6);
    setGameVariableString(0x2B, gRacingContext.racerNames[gRacingContext.finishOrder[1]], 6);
    setGameVariableString(0x2C, gRacingContext.racerNames[gRacingContext.finishOrder[2]], 6);
    setGameVariableString(0x2D, gRacingContext.racerNames[gRacingContext.finishOrder[3]], 6);
    setGameVariableString(0x2E, gRacingContext.racerNames[gRacingContext.finishOrder[4]], 6);
    setGameVariableString(0x2F, gRacingContext.racerNames[gRacingContext.finishOrder[5]], 6);
    
    initializeEmptyMessageBox(0, (u8*)MESSAGE_BOX_1_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(0, -32.0f, 32.0f, 0.0f);
    setMessageBoxLineAndRowSizes(0, 0xA, 2);
    setMessageBoxSpacing(0, 0, 2);
    setMessageBoxFont(0, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(0, -4, 2);
    setMessageBoxSpriteIndices(0, 0xFF, 0, 0);
    setMessageBoxButtonMask(0, BUTTON_A);
    setMessageBoxScrollSpeed(0, 1);
    initializeMessageBox(0, 0, 0x12, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(0, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(1, (u8*)MESSAGE_BOX_2_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(1, -32.0f, 16.0f, 0.0f);
    setMessageBoxLineAndRowSizes(1, 0xA, 2);
    setMessageBoxSpacing(1, 0, 2);
    setMessageBoxFont(1, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(1, -4, 2);
    setMessageBoxSpriteIndices(1, 0xFF, 0, 0);
    setMessageBoxButtonMask(1, BUTTON_A);
    setMessageBoxScrollSpeed(1, 1);
    initializeMessageBox(1, 0, 0x13, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(1, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(2, (u8*)MESSAGE_BOX_3_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(2, -32.0f, 0.0f, 0.0f);
    setMessageBoxLineAndRowSizes(2, 0xA, 2);
    setMessageBoxSpacing(2, 0, 2);
    setMessageBoxFont(2, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(2, -4, 2);
    setMessageBoxSpriteIndices(2, 0xFF, 0, 0);
    setMessageBoxButtonMask(2, BUTTON_A);
    setMessageBoxScrollSpeed(2, 1);
    initializeMessageBox(2, 0, 0x14, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(2, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(3, (u8*)MESSAGE_BOX_4_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(3, 96.0f, 32.0f, 0.0f);
    setMessageBoxLineAndRowSizes(3, 0xA, 2);
    setMessageBoxSpacing(3, 0, 2);
    setMessageBoxFont(3, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(3, -4, 2);
    setMessageBoxSpriteIndices(3, 0xFF, 0, 0);
    setMessageBoxButtonMask(3, BUTTON_A);
    setMessageBoxScrollSpeed(3, 1);
    initializeMessageBox(3, 0, 0x15, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(3, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(4, (u8*)MESSAGE_BOX_5_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(4, 96.0f, 16.0f, 0.0f);
    setMessageBoxLineAndRowSizes(4, 0xA, 2);
    setMessageBoxSpacing(4, 0, 2);
    setMessageBoxFont(4, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(4, -4, 2);
    setMessageBoxSpriteIndices(4, 0xFF, 0, 0);
    setMessageBoxButtonMask(4, BUTTON_A);
    setMessageBoxScrollSpeed(4, 1);
    initializeMessageBox(4, 0, 0x16, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(4, 255, 255, 255, 255);
    
    initializeEmptyMessageBox(5, (u8*)MESSAGE_BOX_6_TEXT_BUFFER);
    setMessageBoxViewSpacePosition(5, 96.0f, 0.0f, 0.0f);
    setMessageBoxLineAndRowSizes(5, 0xA, 2);
    setMessageBoxSpacing(5, 0, 2);
    setMessageBoxFont(5, 0xE, 0xE, (u8*)FONT_TEXTURE_BUFFER, (u16*)FONT_PALETTE_1_BUFFER);
    setMessageBoxInterpolationWithFlags(5, -4, 2);
    setMessageBoxSpriteIndices(5, 0xFF, 0, 0);
    setMessageBoxButtonMask(5, BUTTON_A);
    setMessageBoxScrollSpeed(5, 1);
    initializeMessageBox(5, 0, 0x17, MESSAGE_BOX_MODE_NO_INPUT);
    setMessageBoxRGBA(5, 255, 255, 255, 255);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInRaceResultsElements);

void fadeInRaceResultsSprites(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
    updateSpriteAlpha(0x85, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutRaceResultsElements);

void fadeOutRaceResultsSprites(void) {

    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    updateSpriteAlpha(0x85, 0, 24);

    deactivateMessageBox(0);
    deactivateMessageBox(1);
    deactivateMessageBox(2);
    deactivateMessageBox(3);
    deactivateMessageBox(4);
    deactivateMessageBox(5);

    deactivateOverlayScreenSprites(2);
    deactivateOverlayScreenSprites(3);
    deactivateOverlayScreenSprites(8);
    deactivateOverlayScreenSprites(9);

    initializeMainMessageBoxes();

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", raceResultsScreenCallback);

void raceResultsScreenCallback(void) {
    
    u16 temp = 0;

    switch (overlayScreenTable.screenState) {
        
        case 4:
            break;
        
        case 0:
            loadRaceResultsScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                overlayScreenTable.screenState = 4;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 3;
            }
            break;
        
        case 3:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
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
            deactivateMessageBox(0);
            deactivateMessageBox(1);
            deactivateMessageBox(2);
            deactivateMessageBox(3);
            deactivateMessageBox(4);
            deactivateMessageBox(5);
            deactivateOverlayScreenSprites(2);
            deactivateOverlayScreenSprites(3);
            deactivateOverlayScreenSprites(8);
            deactivateOverlayScreenSprites(9);
            initializeMainMessageBoxes();
            
            overlayScreenTable.screenState = 2;
            
            playSfx(1);
            
        }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRaceGiftsScreenCallback);

void loadRaceGiftsScreenCallback(void) {
    
    openOverlayScreen();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadRaceGiftsScreenSprites);

void loadRaceGiftsScreenSprites(void) {

    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, 2);
    setBilinearFiltering(0x82, 1);
    
    // horse or dog race
    if (checkDailyEventBit(HORSE_RACE)) {
        startSpriteAnimation(0x82, 2, 0);
    } else {
        startSpriteAnimation(0x82, 5, 0);
    }
    
    // medal count display
    dmaSprite(0x83, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, 1);
    startSpriteAnimation(0x83, 4, 0);
    
    // medal count display 2
    dmaSprite(0x84, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x84, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x84, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x84, 2);
    setBilinearFiltering(0x84, 1);
    startSpriteAnimation(0x84, 4, 1);
    
    setOverlayScreenSprites(2, 0x97, (u32)&_moneyTextureSegmentRomStart, (u32)&_moneyTextureSegmentRomEnd, (u32)&_moneyAssetsIndexSegmentRomStart, (u32)&_moneyAssetsIndexSegmentRomEnd, (u8*)GOLD_UI_TEXTURE_BUFFER, (u16*)GOLD_UI_PALETTE_BUFFER, (AnimationFrameMetadata*)GOLD_UI_ANIMATION_FRAME_METADATA_BUFFER, (u32*)GOLD_UI_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, 0, 1, 0, -24.0f, 6.0f, 256.0f, 10);
    
    dmaOverlayScreenSprites(2, D_80205204, 3, 3);
    setOverlayScreenSpritesRGBA(2, 255, 255, 255, 255);
    
    dmaSprite(CURSOR_HAND, (u32)&_dialogueIconsTextureSegmentRomStart, (u32)&_dialogueIconsTextureSegmentRomEnd, (u32)&_dialogueIconsAssetsIndexSegmentRomStart, (u32)&_dialogueIconsAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)DIALOGUE_ICON_TEXTURE_BUFFER, NULL, (u16*)DIALOGUE_ICON_PALETTE_BUFFER, (AnimationFrameMetadata*)DIALOGUE_ICON_ANIMATION_FRAME_METADATA_BUFFER, (u32*)DIALOGUE_ICON_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setBilinearFiltering(CURSOR_HAND, 1);
    setSpriteColor(CURSOR_HAND, 255, 255, 255, 255);
    
    // fade in
    fadeInRaceGiftsSprites();

}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInRaceGiftsElements);

void fadeInRaceGiftsSprites(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
    updateSpriteAlpha(0x84, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutRaceGiftsElements);

void fadeOutRaceGiftsSprites(void) {
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
    updateSpriteAlpha(0x84, 0, 24);
    deactivateOverlayScreenSprites(2);
}

//INCLUDE_RODATA("asm/nonmatchings/game/overlayScreens", D_801221CC);

static const u16 D_801221CC[6] = { 3000, 1000, 500, 0, 0, 0 };

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", raceGiftsScreenCallback);

void raceGiftsScreenCallback(void) {

    u16 buff[4];
    u16 buff2[4];

    u16* ptr = D_801221CC;

    bool set;

    memcpy(buff, ptr, 6);
    memcpy(buff2, ptr, 6);

    set = FALSE;
    
    switch (overlayScreenTable.screenState) {

        case 0:
            loadRaceGiftsScreenSprites();
            overlayScreenTable.screenState = 1;
            break;

        case 1:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                setMessageBoxInterpolationWithFlags(0, -4, 0);
                initializeDialogueSession(0, 0x3E, 5, 0x40);
                overlayScreenTable.screenState = 5;
            }
            break;

        case 5:    
            
            if (isDialogueClosing()) {
            
                if (!(getSelectedMenuRow(0))) {
                    
                    dialogues[0].sessionManager.flags &= ~0x40;
                    closeDialogueSession(0);
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
            
            if (checkAnyMessageBoxTextFinished()) {
                resetMessageBoxAnimation(0);
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
            if (checkAnyMessageBoxTextFinished()) {
                overlayScreenTable.screenState = 9;
            }
            break;
        
        case 2:
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 6;
            }
            break;
        
        case 6:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();
            break;

        case 7:
            
            if (isDialogueClosing()) {
                
                if (!(getSelectedMenuRow(0))) {
                    
                    resetAnimationState(0x86);
                    overlayScreenTable.screenState = 8;
                    goto label3;
                    
                } else {
                    closeDialogueSession(0);
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
                    handleGetHorseRacePrize(overlayScreenTable.cellIndex);
                } else {
                    initializeMessageBox(0, 4, 14, 0);
                    overlayScreenTable.screenState = 3;
                }
                
            } else {

                if (adjustValue(D_80205204, -buff2[overlayScreenTable.cellIndex ], 9999) == -buff2[overlayScreenTable.cellIndex]) {
                    D_80205204 += adjustValue(D_80205204, -buff2[overlayScreenTable.cellIndex], 9999);
                    initializeMessageBox(0, 4, 13, 0);
                    overlayScreenTable.screenState = 3;
                    handleGetDogRacePrize(overlayScreenTable.cellIndex);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", handleGetHorseRacePrize);

void handleGetHorseRacePrize(u8 arg0) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", handleGetDogRacePrize);

void handleGetDogRacePrize(u8 arg0) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadLotteryScreenCallback);

void loadLotteryScreenCallback(void) {
    
    openOverlayScreen();
    
    dmaSprite(CHECKERBOARD_BACKGROUND, (u32)&_checkerboardBackgroundTextureSegmentRomStart, (u32)&_checkerboardBackgroundTextureSegmentRomEnd, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomStart, (u32)&_checkerboardBackgroundAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_BACKGROUND_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_BACKGROUND_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_BACKGROUND_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", loadLotteryScreenSprites);

void loadLotteryScreenSprites(void) {
    
    dmaSprite(0x81, (u32)&_snapshotLargeTextureSegmentRomStart, (u32)&_snapshotLargeTextureSegmentRomEnd, (u32)&_snapshotLargeAssetsIndexSegmentRomStart, (u32)&_snapshotLargeAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_SNAPSHOT_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_SNAPSHOT_PALETTE, (u16*)OVERLAY_SCREEN_SNAPSHOT_ANIM_METADATA, (u32*)OVERLAY_SCREEN_SNAPSHOT_SPRITESHEET_INDEX, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x81, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x81, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x81, 2);
    setBilinearFiltering(0x81, 1);
    startSpriteAnimation(0x81, 0, 0);
    
    dmaSprite(0x82, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x82, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x82, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x82, 2);
    setBilinearFiltering(0x82, 1);
    startSpriteAnimation(0x82, 3, 0);
    
    dmaSprite(0x83, (u32)&_prizeScreenTextureSegmentRomStart, (u32)&_prizeScreenTextureSegmentRomEnd, (u32)&_prizeScreenAssetsIndexSegmentRomStart, (u32)&_prizeScreenAssetsIndexSegmentRomEnd, NULL, NULL, (u8*)OVERLAY_SCREEN_TEXTURE_BUFFER, NULL, (u16*)OVERLAY_SCREEN_PALETTE_BUFFER, (AnimationFrameMetadata*)OVERLAY_SCREEN_ANIMATION_FRAME_METADATA_BUFFER, (u32*)OVERLAY_SCREEN_TEXTURE_TO_PALETTE_LOOKUP_BUFFER, NULL, 0, FALSE);
    setSpriteViewSpacePosition(0x83, 0.0f, 0.0f, 8.0f);
    setSpriteColor(0x83, 0xFF, 0xFF, 0xFF, 0);
    setSpriteBlendMode(0x83, 2);
    setBilinearFiltering(0x83, 1);
    
    if (gBaseMapIndex == FLOWER_SHOP) {
        startSpriteAnimation(0x83, 0, 0);
    } else {
        startSpriteAnimation(0x83, 1, 0);
    }
    
    fadeInLotterySprites();
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeInLotteryElements);

void fadeInLotterySprites(void) {
    updateSpriteAlpha(0x81, 0xFF, 24);
    updateSpriteAlpha(0x82, 0xFF, 24);
    updateSpriteAlpha(0x83, 0xFF, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", fadeOutLotteryElements);

void fadeOutLotterySprites(void) {
    updateSpriteAlpha(0x81, 0, 24);
    updateSpriteAlpha(0x82, 0, 24);
    updateSpriteAlpha(0x83, 0, 24);
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", lotteryScreenCallback);

void lotteryScreenCallback(void) {

    switch (overlayScreenTable.screenState) {

        case 0:
            loadLotteryScreenSprites();
            overlayScreenTable.screenState = 1;
            break;
        
        case 1:
            
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                
                setMessageBoxViewSpacePosition(0, 24.0f, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 0, 0, 0);
                setMessageBoxInterpolationWithFlags(0, -4, 0);
                
                if (gBaseMapIndex == 0x32) {
                    initializeDialogueSession(0, 0x3E, 4, 0x40);
                } else {
                    initializeDialogueSession(0, 0x3E, 0xA, 0x40);
                }
                
                overlayScreenTable.screenState = 4;

            }
            
            break;
        
        case 4:

            if (isDialogueClosing()) {
                
                if (!(getSelectedMenuRow(0))) {
                    
                    dialogues[0].sessionManager.flags &= ~0x40;
                    
                    closeDialogueSession(0);
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
            if (checkAnyMessageBoxTextFinished()) {
                
                resetMessageBoxAnimation(0);
                
                resetAnimationState(CURSOR_HAND);
                
                updateSpriteAlpha(0x81, 0, 24);
                updateSpriteAlpha(0x82, 0, 24);
                updateSpriteAlpha(0x83, 0, 24);
                
                overlayScreenTable.screenState = 2;

            }
            
            break;
        
        case 2:
        
            if (checkSpriteRGBAUpdateFinished(0x82)) {
                closeOverlayScreen();
                overlayScreenTable.screenState = 5;
            }

            break;
        
        case 5:
            deactivateSprite(0x80);
            exitOverlayScreen();
            resumeGameplay();

            break;
        
        case 6:
            
            if (gAudioSequences[1].flags == 0) {

                if (gBaseMapIndex == FLOWER_SHOP) {

                    switch (getRandomNumberInRange(0, 4)) {

                        case 0:     
                            
                            if (!(checkAvailableFlowerShopPrize(0))) {
                                initializeMessageBox(0, 4, 0x38, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetFlowerShopPrize(0);
                            } else {
                                initializeMessageBox(0, 4, 0x3C, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetFlowerShopPrize(4);
                            }
                            
                            break;
                        
                    case 1:               
                        
                        if (!(checkAvailableFlowerShopPrize(1))) {
                            initializeMessageBox(0, 4, 0x39, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetFlowerShopPrize(1);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetFlowerShopPrize(4);
                        }
                        
                        break;
                        
                    case 2:                             
                        
                        if (!(checkAvailableFlowerShopPrize(2))) {
                            initializeMessageBox(0, 4, 0x3A, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetFlowerShopPrize(2);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetFlowerShopPrize(4);
                        }
                        
                        break;
                        
                    case 3: 
                        
                        if (!(checkAvailableFlowerShopPrize(3))) {
                            initializeMessageBox(0, 4, 0x3B, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetFlowerShopPrize(3);
                        } else {
                            initializeMessageBox(0, 4, 0x3C, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetFlowerShopPrize(4);
                        }
                        
                        break;

                    case 4:   
                        initializeMessageBox(0, 4, 0x3C, 0);
                        overlayScreenTable.screenState = 3;
                        handleGetFlowerShopPrize(4);
                        
                        break;                        
                    
                    }
                    
                } else {
                    
                    switch (getRandomNumberInRange(0, 4)) {      
                        
                        case 0:                             
                            
                            if (!(checkAvailableBakeryPrize(0))) {
                                initializeMessageBox(0, 4, 0x40, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(0);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(4);
                            }
                            
                            break;
                        
                        case 1:                             
                            
                            if (!(checkAvailableBakeryPrize(1))) {
                                initializeMessageBox(0, 4, 0x41, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(1);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(4);
                            }
                        
                            break;
                        
                        case 2:                             
                            
                            if (!(checkAvailableBakeryPrize(2))) {
                                initializeMessageBox(0, 4, 0x42, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(2);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(4);
                            }
                            
                            break;
                        
                        case 3:            
                            
                            if (!(checkAvailableBakeryPrize(3))) {
                                initializeMessageBox(0, 4, 0x43, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(3);
                            } else {
                                initializeMessageBox(0, 4, 0x44, 0);
                                overlayScreenTable.screenState = 3;
                                handleGetBakeryPrize(4);
                            }
                        
                            break;
                        
                        case 4:                             
                            initializeMessageBox(0, 4, 0x44, 0);
                            overlayScreenTable.screenState = 3;
                            handleGetBakeryPrize(4);

                            break;
                            
                        }
                }
                
            }
            
            break;
        
        default:
            break;
   
    }
    
}

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", checkAvailableFlowerShopPrize);

bool checkAvailableFlowerShopPrize(u8 arg0) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", handleGetFlowerShopPrize);

void handleGetFlowerShopPrize(u8 arg0) {

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
            storeTool(0x12);
            D_80205636 += adjustValue(D_80205636, 1, 20);
            break;
        case 4:
            storeTool(0x11);
            D_801C3F70 += adjustValue(D_801C3F70, 1, 20);
        default:
            break;
    }
} 

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", checkAvailableBakeryPrize);

bool checkAvailableBakeryPrize(u8 arg0) {

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

//INCLUDE_ASM("asm/nonmatchings/game/overlayScreens", handleGetBakeryPrize);

void handleGetBakeryPrize(u8 arg0) {

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