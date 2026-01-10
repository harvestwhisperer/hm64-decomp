#ifndef _LEVEL_H_  
#define _LEVEL_H_

#include "common.h"

// TODO: finish adding these
/* map flags */
#define INDOORS 1
#define OUTDOORS 2
#define HAS_SEASON_MAP 8

extern void loadLevelFromEntrance(u16);  
extern void setLevelAudio(u16, u8, u8);     
extern Vec4f* getDefaultLevelLighting(Vec4f*, u16);      
extern void setChickenFeedSprite(u8); 
extern void setBarnFodderSprite(u8 itemIndex);
extern u8 getMapFromExit(u8);
extern u16 getCantEnterTextIndex(u16);    

extern void setupLevelMap(u16);        
extern u8 getLevelFlags(u16);    
extern void setEntrance(u16 entranceIndex);  

extern u8 gBaseMapIndex;
extern u8 gEntranceIndex;
extern u8 previousEntranceIndex;
extern u8 previousMapIndex;
extern u8 gMapWithSeasonIndex;
extern u8 gCameraRotationOffset;

extern u8 groundObjectsGridZ;
extern u8 groundObjectsGridX;

#endif