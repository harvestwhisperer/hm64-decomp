#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "common.h"

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