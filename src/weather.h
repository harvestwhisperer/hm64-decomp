#ifndef _WEATHER_H_  
#define _WEATHER_H_

#include "common.h"

extern void func_800DC360(void);    

extern void setForecast(void);    
extern Vec4f setWeatherLighting(u8 weather);  

extern u8 gForecast;
extern u8 gWeather;

#endif