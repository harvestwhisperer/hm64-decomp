#ifndef _WEATHER_H_
#define _WEATHER_H_

#include "common.h"

#define SUNNY 1
#define RAIN 2
#define SNOW 3
// 4 = unused rain
#define TYPHOON 5

extern void setWeatherSprites(void);

extern void setForecast(void);
extern Vec4f setWeatherLighting(u8 weather);

extern u8 gForecast;
extern u8 gWeather;

#endif