#ifndef _MAP_OBJECTS_H_
#define _MAP_OBJECTS_H_

#include "common.h"

// TODO: finish adding map objects
/* field tiles */
#define UNTILLED 1
#define TILLED 2	
#define TILLED_WATERED 3
#define SMALL_ROCK 4
#define WEED 5
#define LOG 6
#define BROKEN_LOG 7
// TODO: add crops
#define BOULDER 0xC4
#define STUMP 0xC8
#define CORN_RIPE 0x83
#define CORN_DEAD 0x84
#define CORN_PLANT_WITHERED 0x85
#define GRASS_PLANTED 0x88
#define GRASS_GROWING 0x8B
#define GRASS_GROWN 0x8E
#define GRASS_CUT 0x8F
#define MOONDROP_PLANTED 0x90
// TODO: add foragables

#define FIELD_WIDTH 24
#define FIELD_HEIGHT 20

// TODO: label these appropriate
// object maps for levels
// top of mountain 1
extern u8 D_8013D258[FIELD_WIDTH][FIELD_HEIGHT];
// moon mountain
extern u8 D_8016FBF0[FIELD_WIDTH][FIELD_HEIGHT];
// pond
extern u8 D_8016FE08[FIELD_WIDTH][FIELD_HEIGHT];
// cave
extern u8 D_8016F8F8[FIELD_WIDTH][FIELD_HEIGHT];
// mine
extern u8 D_80170468[FIELD_WIDTH][FIELD_HEIGHT];
// greenhouse
extern u8 D_80182BA8[FIELD_WIDTH][FIELD_HEIGHT];
// mountain 1
extern u8 D_80182FD8[FIELD_WIDTH][FIELD_HEIGHT];
// mountain 2
extern u8 D_80189868[FIELD_WIDTH][FIELD_HEIGHT];
// ranch
extern u8 D_801C3FC0[FIELD_WIDTH][FIELD_HEIGHT];

extern u8 farmFieldTiles[FIELD_WIDTH][FIELD_HEIGHT];

extern void func_800D9600(u8);   
extern void func_800D9BFC(); 
extern u8 func_800DA8B8(u8);                             
extern u8 func_800DA918(u8);                 
extern u8 func_800DA948(u8);
extern u8 func_800DA978(u8 index);     
extern u8 func_800DAA90(u8, u8, u8, u8); 
extern void func_800DAC70(u8, u8, u8, u8);
extern void func_800DAD74(u8, f32, u8);    
extern u8 func_800DAF58(f32, u8);  
extern u8 func_800DB1BC(f32, f32); 
extern void func_800DB424();                                   
extern void func_800DB858();                                   
extern void func_800DBAC4();     
extern void func_800DBBB0(u8);             
extern u16 func_800DBF90();     
extern u16 func_800DC008();       
extern u32 func_800DC080(); 

extern u16 countPinkCatMintFlowersFarm(void);     
extern u16 countPinkCatMintFlowersGreenhouse(void);  

extern u8 blueMistFlowerPlot;

#endif