#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "common.h"

// TODO: finish adding these
/* main loop callback function indices */
#define NO_OP 0
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
#define PAUSE_SCREEN_MAP_LOAD_1 0x1E
#define PAUSE_SCREEN_MAP_LOAD_2 0x1F
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
func_8005D0BC 
func_80060490 
func_8005C00C 
func_8005CA64 
func_8005CAA8 
func_8005CB50 
func_8005CBA4 
func_8005CBF0 
func_8005CEFC 
func_8005CF4C
func_8005D2B0 
func_80060624 
func_80060838 
func_80060454 
tvMainLoopCallback 
func_800605F0 
func_800D93CC 
func_800DCAB8 
func_800B881C 
func_800BADD0 
func_800BCA9C 
func_800BE808
func_800B82AC 
func_800B8018 
func_800B815C 
func_800B83F0 
func_800B8554 
func_800B86B8 
func_800C8424 
func_800C88F4 
func_800CC518 
func_800CD928 
func_800CE930 
func_800B9D3C
func_800BBEC0 
func_800BDB24 
func_800BF990 
func_800C1124 
func_800C224C 
func_800C2B8C 
func_800C3D20 
func_800C53C0 
func_800C7058 
func_800C8784 
func_800CA808 
func_800CD750
func_800CE068 
func_800CEDF0 
titleScreenMainLoopCallback
func_8005CF94 
func_800ED974 
func_800E1FAC 
func_800E8F08 
func_800657B4 
func_800657C4
*/

#endif