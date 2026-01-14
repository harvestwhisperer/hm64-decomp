#ifndef _OVERLAY_SCREEN_H
#define _OVERLAY_SCREEN_H

#include "common.h"

#define TOOL_SLOTS 5
#define BELONGINGS 6
#define KEY_ITEMS 7

// =============================================================================
// UI Coordinate System
// =============================================================================
// Screen resolution: 320x240
// Coordinate origin: center of screen (0, 0)
// X range: -160 (left) to +160 (right)
// Y range: -120 (bottom) to +120 (top)
#define SCREEN_CENTER_X     0.0f
#define SCREEN_CENTER_Y     0.0f
#define SCREEN_LEFT        -160.0f
#define SCREEN_RIGHT        160.0f
#define SCREEN_TOP          120.0f
#define SCREEN_BOTTOM      -120.0f

// =============================================================================
// Inventory Grid System (35px spacing)
// =============================================================================
// Used for tools, belongings, and storage screens
#define UI_GRID_SPACING     35.0f
// Pause screen inventory grid (left-aligned)
#define PAUSE_GRID_BASE_X  -74.0f
#define PAUSE_GRID_X(col)  (PAUSE_GRID_BASE_X + (col) * UI_GRID_SPACING)
// Toolbox/Freezer/Cabinet grid (center-aligned)
#define STORAGE_GRID_BASE_X -32.0f
#define STORAGE_GRID_X(col) (STORAGE_GRID_BASE_X + (col) * UI_GRID_SPACING)
// Y positions for inventory rows
#define UI_ROW_TOP          33.0f
#define UI_ROW_UPPER        -2.0f
#define UI_ROW_LOWER       -47.0f
#define UI_ROW_BOTTOM      -82.0f
// Current/held item positions
#define UI_HELD_ITEM_Y      80.0f
#define UI_CURRENT_TOOL_X  -28.0f
#define UI_HELD_ITEM_X      20.0f
#define UI_STORAGE_HELD_X   0.0f

// =============================================================================
// Key Items Grid (38px X spacing, 37px Y spacing)
// =============================================================================
#define KEY_ITEMS_BASE_X    77.0f
#define KEY_ITEMS_SPACING_X 38.0f
#define KEY_ITEMS_X(col)   (KEY_ITEMS_BASE_X + (col) * KEY_ITEMS_SPACING_X)
#define KEY_ITEMS_BASE_Y    31.0f
#define KEY_ITEMS_SPACING_Y 37.0f
#define KEY_ITEMS_Y(row)   (KEY_ITEMS_BASE_Y - (row) * KEY_ITEMS_SPACING_Y)

// =============================================================================
// Menu Cursor Positions
// =============================================================================
// Load game screen
#define LOAD_GAME_CURSOR_X_SELECT   -128.0f
#define LOAD_GAME_CURSOR_X_ACTION    96.0f
#define LOAD_GAME_SPACING_Y          44.0f
#define LOAD_GAME_BASE_Y             42.0f
#define LOAD_GAME_Y(row)            (LOAD_GAME_BASE_Y - (row) * LOAD_GAME_SPACING_Y)
// Farm rankings cursor
#define FARM_RANK_CURSOR_X          -128.0f
#define FARM_RANK_SPACING_Y          32.0f
#define FARM_RANK_BASE_Y             32.0f
#define FARM_RANK_Y(row)            (FARM_RANK_BASE_Y - (row) * FARM_RANK_SPACING_Y)
// House extension cursor
#define HOUSE_EXT_CURSOR_X          -112.0f
#define HOUSE_EXT_SPACING_Y          16.0f
#define HOUSE_EXT_BASE_Y             32.0f
#define HOUSE_EXT_Y(row)            (HOUSE_EXT_BASE_Y - (row) * HOUSE_EXT_SPACING_Y)

// =============================================================================
// Power Nuts Display (16px spacing)
// =============================================================================
#define POWER_NUT_SPACING           16.0f
#define POWER_NUT_BASE_X           -72.0f
#define POWER_NUT_X(col)           (POWER_NUT_BASE_X + (col) * POWER_NUT_SPACING)

// =============================================================================
// Calendar Stickers
// =============================================================================

#define CALENDAR_STICKER_X_LEFT     108.0f
#define CALENDAR_STICKER_X_RIGHT    126.0f
#define CALENDAR_STICKER_SPACING_Y   24.0f
#define CALENDAR_STICKER_BASE_Y      30.0f
#define CALENDAR_STICKER_Y(row)     (CALENDAR_STICKER_BASE_Y - (row) * CALENDAR_STICKER_SPACING_Y)
      
// 0x801FB6F0
typedef struct {
    u8 cellIndex;
    u8 previousCellIndex;
    u8 pageNumber;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 screenState;
    u8 unk_7;
    u8 unk_8;
    u8 unk_9;
    u8 unk_A;
    u8 unk_B;
    u8 unk_C;
} OverlayScreenTable;

extern void closeOverlayScreen(void);
extern void loadTVCButtonIcons(void);
extern void updateTVCButtonsByChannel(void);     
extern void loadShopIcons(void);
extern void updateShopBuyAnimation(void);
extern void updateShopQuantityArrows(u8);
extern void loadDiarySelectScreen(void);    
extern void fadeInLoadGameScreen(void);  
extern void setLoadGameScreenFullAlpha(void);
extern void fadeOutLoadGameScreen(void);
extern void setDiaryCursorPosition(u8 arg0, u8 arg1, u8 arg2);
extern void updateDiaryHighlighted(u8 arg0, u8 arg1, u8 arg2);
extern void animateDiarySelected(u8 arg0, u8 arg1);
extern void loadFarmRankingsListScreen(void);
extern void loadIndividualRankingScreen(u8);
extern void fadeInFarmRankingScreen(void);
extern void fadeOutFarmRankingScreen(void);
extern void hideUpArrow(void);
extern void hideDownArrow(void);
extern void setFarmRankingScreenFullAlpha(void);
extern void setFarmRankingCursorPosition(u8 arg0);
extern void updateHorseRaceContext(void);
extern void updateDogRaceContext(void);
extern void initializeRaceContext(void); 
extern void addRecipe(u16 bitIndex);
extern u32 checkRecipe(u16 bitIndex);
extern bool func_800CE714(bool);
extern bool func_800CE828(bool); 

// main loop callback functions
extern void loadEstimateScreenCallback();
extern void loadKitchenPictureCallback();
extern void loadHouseExtensionsSelectionCallback();
extern void loadCalendarScreenCallback();
extern void loadRecipeBookScreenCallback();
extern void loadAlbumScreenCallback();
extern void loadPauseScreenCallback();
extern void pauseScreenCallback();
extern void loadToolboxScreenCallback();
extern void toolboxScreenCallback();
extern void loadFreezerScreenCallback();
extern void freezerScreenCallback();
extern void loadCabinetScreenCallback();
extern void cabinetScreenCallback();
extern void houseExtensionsSelectionScreenCallback();
extern void estimateScreenCallback();
extern void kitchenPictureScreenCallback();
extern void calendarScreenCallback();
extern void recipeBookScreenCallback();
extern void albumScreenCallback();
extern void loadPauseScreenMapScreenCallback();
extern void pauseScreenMapCallback();
extern void loadRaceBettingScreenCallback();
extern void raceBettingScreenCallback();
extern void loadRaceResultsScreenCallback();
extern void raceResultsScreenCallback();
extern void loadRaceGiftsScreenCallback();
extern void raceGiftsScreenCallback();
extern void loadLotteryScreenCallback();
extern void lotteryScreenCallback();

extern u8 D_801890E8[31];
// calendar
// TODO: label
extern u8 D_80189108[5][7];
// refrigerator/freezer
// TODO: label
// FIXME: should be 2D array with [4][8]
extern u8 D_80237420[31];

extern u32 D_80205204;

extern OverlayScreenTable overlayScreenTable;

#endif