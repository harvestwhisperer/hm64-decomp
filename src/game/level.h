#ifndef _LEVEL_H_  
#define _LEVEL_H_

#include "common.h"

// TODO: finish adding these
/* map flags */
#define INDOORS 1
#define OUTDOORS 2
#define HAS_SEASON_MAP 8

extern void func_8006E840(u16);  
extern void setLevelAudio(u16, u8, u8);     
extern Vec4f* func_8006EB94(Vec4f*, u16);      
extern void func_80073244(u8); 
extern void func_8007341C(u8 itemIndex);
extern u8 getMapFromExit(u8);
extern u16 func_80074C50(u16);    

extern void setupLevelMap(u16);        
extern u8 getLevelFlags(u16);    
extern void setEntrance(u16 entranceIndex);  

extern u8 gBaseMapIndex;
extern u8 gEntranceIndex;
extern u8 previousEntranceIndex;
extern u8 previousMapIndex;
extern u8 gMapWithSeasonIndex;
extern u8 D_8021E6D0;

extern u8 groundObjectsGridZ;
extern u8 groundObjectsGridX;

#endif