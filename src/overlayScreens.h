#ifndef _OVERLAY_SCREENS_H
#define _OVERLAY_SCREENS_H

#include "common.h"

extern void func_800B2CE0(void);
extern void func_800B3694(void);
extern void func_800B3A60(void);     
extern void func_800B3BD8(void);
extern void func_800B4238(u8);
extern void func_800B42E0(void);    
extern void func_800B59AC(void);  
extern void func_800B5BD4(void);
extern void func_800CC17C(void); 

extern u8 D_801890E8[31];
// calendar
extern u8 D_80189108[5][7];
extern u8 D_80237420[31];

extern u32 checkerboardBackgroundTexture_ROM_START;
extern u32 checkerboardBackgroundTexture_ROM_END;
extern u32 checkerboardBackgroundIndex_ROM_START;
extern u32 checkerboardBackgroundIndex_ROM_END;

extern u32 holdableItemsTexture_ROM_START;
extern u32 holdableItemsTexture_ROM_END;
extern u32 holdableItemsAssetsIndex_ROM_START;
extern u32 holdableItemsAssetsIndex_ROM_END;
extern u32 holdableItemsSpritesheetIndex_ROM_START;
extern u32 holdableItemsSpritesheetIndex_ROM_END;

extern u16 D_80180714;

// bss
// struct
// cell index
extern u8 D_801FB6F0;
// previous cell index
extern u8 D_801FB6F1;
// page number
extern u8 D_801FB6F2;
extern u8 D_801FB6F3;
extern u8 D_801FB6F6;
extern u8 D_801FB6F8;
extern u8 D_801FB6F9;
extern u8 D_801FB6FA;
extern u8 D_801FB6FB;

#endif