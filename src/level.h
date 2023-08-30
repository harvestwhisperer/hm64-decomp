#ifndef _LEVEL_H_  
#define _LEVEL_H_

#include "common.h"

extern void func_8006E840(u8);  
extern void func_8006EA44(u8, u8, u8);     
extern void func_8006EB94(Vec4f*, u16);      
extern void func_80073244(u8); 
extern u8 func_80074C38(u8);
extern s32 func_80074C50(u8);    

extern void setLevelGraphicsData(u16);        
extern u8 getLevelFlags(u16);    
extern void setExit(u16 extiIndex);  

extern u8 gBaseMapIndex;
extern u8 gEntranceIndex;
extern u8 previousMapIndex;

extern u8 D_80205635;

#endif