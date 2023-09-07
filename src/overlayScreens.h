#ifndef _OVERLAY_SCREENS_H
#define _OVERLAY_SCREENS_H

#include "common.h"

extern void func_800B2CE0(void);
extern void func_800B3A60(void);     
extern void func_800B42E0(void);    
extern void func_800B59AC(void);  
extern void func_800B5BD4(void);
extern void func_800CC17C(void); 

// calendar
extern u8 D_80189108[5][7];

extern void *checkerboardBackgroundTextureStart;
extern void *checkerboardBackgroundTextureEnd;
extern void *checkerboardBackgroundPaletteStart;
extern void *checkerboardBackgroundPaletteEnd;

// sprite rom addresses
extern void *holdableItemsTexturesStart;
extern void *holdableItemsTexturesEnd;
extern void *holdableItemsPaletteStart;
extern void *holdableItemsPaletteEnd;
extern void *holdableItemsTableStart;
extern void *holdableItemsTableEnd;

#endif