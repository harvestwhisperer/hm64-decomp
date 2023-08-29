#ifndef _MAP_OBJECTS_H_
#define _MAP_OBJECTS_H_

#include "common.h"

extern void func_800D9600(u8);   
extern void func_800D9BFC(); 
extern u8 func_800DA8B8(u8);                             
extern u8 func_800DA918(u8);                              
extern u16 func_800DA948(u8);
extern u8 func_800DA978(u8 index);     
extern void func_800DAA90(u8, u8, u8, u8); 
extern void func_800DAC70(u8, u8, u8, u8);
extern f32* func_800DAD74(u8, f32, u8);    
extern u8 func_800DAF58(f32, u8);  
extern u8 func_800DB1BC(f32, f32); 
extern void func_800DB424();                                   
extern void func_800DB858();                                   
extern void func_800DBAC4();     
extern u16 func_800DBF90();     
extern u16 func_800DC008();       
extern u32 func_800DC080(); 

extern u16 countPinkCatMintFlowersFarm(void);     
extern u16 countPinkCatMintFlowersGreenhouse(void);  

extern u8 blueMistFlowerPlot;

extern u8 farmFieldTiles[FIELD_WIDTH][FIELD_HEIGHT];

#endif