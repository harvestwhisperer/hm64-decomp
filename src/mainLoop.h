#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "common.h"

/* main loop callback function indices */
#define MAIN_GAME 1
#define MAP_LOAD 2
// unsure: DREAM_CUTSCENES 3
// unsure: INTRO_LOAD 4
#define LEVEL_LOAD_1 5
#define LEVEL_LOAD_2 6
#define OVERLAY_SCREEN_LOAD 7
#define ROTATING 8
#define DIALOGUE 9
#define TEXT 0xA
// 0xb unknown
#define PINK_OVERLAY_TEXT 0xC
#define END_OF_DAY_1 0xD
#define END_OF_DAY_2 0xE
// 0xf unknown
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
#define HORSE_RACE_RESULTS_LOAD 0x20
#define HORSE_RACE_GIFTS_LOAD 0x21
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
// 0x2d unknown
#define HORSE_RACE_BETTING 0x2E
#define HORSE_RACE_RESULTS 0x2F
#define HORSE_RACE_GIFTS 0x30
#define LOTTERY_2 0x31
#define TITLE_SCREEN 0x32
#define NAMING_SCREEN 0x33
#define SELECT_GAME 0x35
#define ESTIMATE_SCREEN 0x36
// 0x37 unknown
// 0x38 unknown

#define MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE 0x39

extern void mainLoop();

extern u32 setMainLoopCallbackFunctionIndex(u16 callbackIndex); 
extern bool registerMainLoopCallback(u16 index, void *(func)());

extern u8 gfxRetraceCallback(int);
extern void gfxBufferSwap(void *gfxTask);
extern void gfxPreNMICallback(void);

extern void noOpCallback(void);

extern volatile u16 mainLoopCallbackCurrentIndex;
extern void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();

#endif