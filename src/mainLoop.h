#ifndef _MAIN_LOOP_H_
#define _MAIN_LOOP_H_

#include "common.h"

extern void mainLoop();

extern bool setMainLoopCallbackFunctionIndex(u16 callbackIndex); 
extern bool registerMainLoopCallback(u16 index, void *(func)());

extern void gfxRetraceCallback(void);
extern void gfxBufferSwap(void);
extern void gfxPreNMICallback(void);

extern void noOpCallback(void);

extern u16 mainLoopCallbackCurrentIndex;
extern void (*mainLoopCallbacksTable[MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE])();

#endif