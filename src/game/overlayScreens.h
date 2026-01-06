#ifndef _OVERLAY_SCREEN_H
#define _OVERLAY_SCREEN_H

#include "common.h"

#define TOOL_SLOTS 5
#define BELONGINGS 6
#define KEY_ITEMS 7

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