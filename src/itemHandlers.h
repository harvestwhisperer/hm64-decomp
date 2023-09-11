
#ifndef _ITEM_HANDLERS_H_
#define _ITEM_HANDLERS_H

#include "common.h"

typedef struct {
    Vec3f unk_0;
    u32 unk_C;
    u32 unk_10;
    u32 unk_14;
    Vec3i unk_18;
    u16 unk_24;
    u16 unk_26;
    u16 unk_28;
    u8 unk_2A;
    u8 flags;
} ToolStatus;

typedef struct {
    u16 unk_0; // 28
    u8 unk_2; // 2A
    u8 unk_3; // 2B
    u16 unk_4; // 2C
    u16 unk_6; // 2E
    u8 unk_8; // 30
    u16 unk_A; // 32
    u16 unk_C; // 34
    u8 unk_E; // 36
    u8 unk_F; // 37;
} ToolUse;


extern void func_800CF850();     
extern void func_800D0318(); 
extern u8 func_800D5308(u8 index, u8 arg1, u32 arg2, s32 arg3, s32 arg4);     
extern u8 func_800D5390(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4);
extern void func_800D5548(u8);            
extern void func_800D55E4(u8, u8);  
extern u16 func_800D5A88(u16 arg0);         
extern u8 func_800D5B00(u16 index);                          
extern u8 func_800D5B18(u16 index);   
extern void func_800D51B0();                                  
extern void func_800D5290();  
extern u16 func_800D5A6C(u16 index);   
extern bool func_800D5B30();
extern void func_800D67FC(u8 index);
extern void func_800D7010();  


extern ToolStatus D_80204DF8[10];
extern ToolUse D_80189828;

extern u32 D_80188F60;
extern u32 D_801C3F80;
extern u32 D_801FB5D0;
extern u32 D_801FB6FC;
extern u32 D_80237414;

#endif