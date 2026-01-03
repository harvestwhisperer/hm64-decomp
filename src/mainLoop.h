#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "common.h"

/* main loop callback function indices */
#define NO_OP 0
#define MAIN_GAME 1
#define MAP_LOAD 2
// unsure: DREAM_CUTSCENES 3
#define SET_AUDIO_AND_LIGHTING 4
#define LEVEL_LOAD 5
#define EXIT_LEVEL 6
#define OVERLAY_SCREEN_LOAD 7
#define ROTATING 8
#define DIALOGUE 9
#define MESSAGE_BOX 10
#define END_CUTSCENE 0xB
#define PINK_OVERLAY_TEXT 0xC
#define END_OF_DAY_1 0xD
#define END_OF_DAY_2 0xE
#define WAIT_AUDIO_FINISH 0xF
#define END_OF_FESTIVAL_DAY_1 0x10
#define END_OF_FESTIVAL_DAY_2 0x11
#define TV 0x12
#define SHOP_DIALOGUE 0x13
#define PAUSE_SCREEN_LOAD 0x14
#define TOOLBOX_LOAD 0x15
#define FREEZER_LOAD 0x16
#define CABINET_LOAD 0x17
#define HOME_EXTENSIONS_SELECT_LOAD 0x18
#define ESTIMATE_LOAD 0x19
#define KITCHEN_PICTURE_LOAD 0x1A
#define CALENDAR_LOAD 0x1B
#define RECIPE_BOOK_LOAD 0x1C
#define ALBUM_LOAD 0x1D
#define PAUSE_SCREEN_MAP_LOAD 0x1E
#define RACE_BETTING_LOAD 0x1F
#define RACE_RESULTS_LOAD 0x20
#define RACE_GIFTS_LOAD 0x21
#define LOTTERY_LOAD 0x22
#define PAUSE_SCREEN 0x23
#define TOOLBOX 0x24
#define FREEZER 0x25
#define CABINET 0x26
#define HOME_EXTENSIONS_SELECT 0x27
#define ESTIMATE 0x28
#define KITCHEN_PICTURE 0x29
#define CALENDAR 0x2A
#define RECIPE_BOOK 0x2B
#define ALBUM 0x2C
#define PAUSE_SCREEN_MAP 0x2D
#define HORSE_RACE_BETTING 0x2E
#define HORSE_RACE_RESULTS 0x2F
#define HORSE_RACE_GIFTS 0x30
#define LOTTERY 0x31
#define TITLE_SCREEN 0x32
#define NAMING_SCREEN 0x33
#define NAMING_SCREEN_LOAD 0x34
#define SELECT_GAME 0x35
#define FARM_RANKING_SCREEN 0x36
// 0x37 unknown
// 0x38 unknown

#define MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE 57

extern void mainLoop();

extern u32 setMainLoopCallbackFunctionIndex(u16 callbackIndex); 
extern bool registerMainLoopCallback(u16 index, void *(func)());

extern u8 gfxRetraceCallback(int);
extern void gfxBufferSwap(void *gfxTask);
extern void gfxPreNMICallback(void);

extern void noOpCallback(void);

extern volatile u16 mainLoopCallbackCurrentIndex;
extern void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();

/*
noOpCallback 
mainGameLoopCallback
mapLoadCallback 
cutsceneCompletionCallback 
setMapAudioAndLighting 
levelLoadCallback 
exitLevelCallback 
func_8005CB50 
handleRotationCallback 
handleDialogueCallback 
messageBoxCallback 
endCutsceneCallback
pinkOverlayMenuCallback 
endOfDayCallback1 
endOfDayCallback2 
waitForAudioFinishCallback 
tvMainLoopCallback 
endOfFestivalDayCallback2 
func_800D93CC 
shopDialogueCallback 
loadPauseScreenCallback 
loadToolboxScreenCallback 
loadFreezerScreenCallback 
loadCabinetScreenCallback
loadHouseExtensionsSelectionCallback 
loadEstimateScreenCallback 
loadKitchenPictureCallback 
loadCalendarScreenCallback 
loadRecipeBookScreenCallback 
loadAlbumScreenCallback 
loadPauseScreenMapScreenCallback 
loadRaceBettingScreenCallback 
loadRaceResultsScreenCallback 
loadRaceGiftsScreenCallback 
loadLotteryScreenCallback 
pauseScreenCallback
toolboxScreenCallback 
freezerScreenCallback 
cabinetScreenCallback 
houseExtensionsSelectionScreenCallback 
estimateScreenCallback 
kitchenPictureScreenCallback 
calendarScreenCallback 
recipeBookScreenCallback 
albumScreenCallback 
pauseScreenMapCallback 
raceBettingScreenCallback 
raceResultsScreenCallback
raceGiftsScreenCallback 
lotteryScreenCallback 
titleScreenMainLoopCallback
loadNamingScreenCallback 
namingScreenCallback
gameSelectCallback 
farmRankingScreenCallback 
func_800657B4 
func_800657C4
*/

#endif