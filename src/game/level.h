#ifndef _LEVEL_H_  
#define _LEVEL_H_

#include "common.h"

#define LEVEL_TIME_FLOWS        1
#define LEVEL_OUTDOORS          2
#define LEVEL_WEATHER_LIGHTING  4
#define LEVEL_HAS_SEASON_MAP    8
#define LEVEL_INDOOR_BUILDING   0x10
#define LEVEL_FARM              0x20

extern void loadMapAtSpawnPoint(u16);  
extern void setLevelAudio(u16, u8, u8);     
extern Vec4f* getDefaultLevelLighting(Vec4f*, u16);      
extern void setChickenFeedSprite(u8); 
extern void setBarnFodderSprite(u8 itemIndex);
extern u8 getMapForSpawnPoint(u8);
extern u16 getCantEnterTextIndex(u16);    

extern void setupLevelMap(u16);        
extern u8 getLevelFlags(u16);    
extern void setSpawnPoint(u16 entranceIndex);  

extern u8 gBaseMapIndex;
extern u8 gSpawnPointIndex;
extern u8 previousSpawnPoint;
extern u8 previousMapIndex;
extern u8 gMapWithSeasonIndex;
extern u8 gCameraRotationOffset;

extern u8 groundObjectsGridZ;
extern u8 groundObjectsGridX;

#endif