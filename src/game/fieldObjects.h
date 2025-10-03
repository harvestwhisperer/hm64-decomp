#ifndef _MAP_OBJECTS_H_
#define _MAP_OBJECTS_H_

#include "common.h"

// TODO: finish adding field objects
/* field tiles */
#define UNTILLED 1
#define TILLED 2	
#define TILLED_WATERED 3
#define SMALL_ROCK 4
#define WEED 5
#define LOG 6
#define BROKEN_LOG 7
#define CORN_RIPE 0x83
#define CORN_DEAD 0x84
#define CORN_PLANT_WITHERED 0x85
#define GRASS_PLANTED 0x88
#define GRASS_GROWING 0x8B
#define GRASS_GROWN 0x8E
#define GRASS_CUT 0x8F
#define MOONDROP_PLANTED 0x90
#define BOULDER 0xC4
#define STUMP 0xC8

// FIXME: might need to be reversed
#define FIELD_HEIGHT 24
#define FIELD_WIDTH 20

#define MAX_FIELD_OBJECTS 218


typedef struct {
    u8 mapAdditionIndex;
    u8 spriteIndex;
    u8 heldItemIndex;
    u8 flag2;
    u8 flag3;
} GroundObjectInfo;

extern u8 topOfMountain1FieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 moonMountainFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 pondFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 caveFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 mineFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 greenhouseFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 mountain1FieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 mountain2FieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 ranchFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];
extern u8 farmFieldTiles[FIELD_HEIGHT][FIELD_WIDTH];

extern void setMapGroundObjects(u8);   
extern void func_800D9BFC(); 
extern u8 getGroundObjectMapAdditionIndex(u8);                             
extern u8 getHeldItemIndexFromGroundObject(u8);                 
extern u8 func_800DA948(u8);
extern u8 getGroundObjectFlags(u8 index);     
extern u8 setFieldTile(u8, u8, u8, u8); 
extern void addGroundObjectToMap(u8, u8, u8, u8);
extern void addGroundObjectToMapFromPlayerPosition(u8, f32, u8);    
extern u8 getGroundObjectIndexFromPlayerPosition(f32, u8);  
extern u8 getGroundObjectIndexFromCoordinates(f32, f32); 
extern void func_800DB424();                
extern void updateCropsIfRain();                                   
extern void randomlyAddWeedsToFarmField();     
extern void randomlyBreakLogPieces(u8);             
extern void func_800DBC9C(u8);       
extern void func_800DBD88(u8);   
extern void func_800DBE8C(u8);         
extern u16 getFarmGrassTilesSum();     
extern u16 func_800DC008();       
extern u16 func_800DC080(); 

extern u16 getFarmPinkCatMintFlowersTilesSum(void);     
extern u16 getGreenhousePinkCatMintFlowersTilesSum(void);  

extern u8 blueMistFlowerPlot;

#endif