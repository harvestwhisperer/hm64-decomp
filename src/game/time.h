#ifndef _UPDATE_GAME_H_  
#define _UPDATE_GAME_H_

#include "common.h"

#define SPRING 1
#define SUMMER 2
#define AUTUMN 3
#define WINTER 4

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define DAYTIME (5 < gHour && gHour < 18)
#define NIGHTTIME (17 < gHour && gHour < 24)
#define RAINING (2 <= gWeather && gWeather < 4)

extern void setClockNewDay(void);
extern void func_800D7C20(void);

extern void setSeasonName();  
void setGlobalSeasonName(u8);  

extern u8 gCurrentSeasonName[6];
extern u8 gGlobalSeasonName[6];

extern u8 gSeconds;
extern u8 gHour;
extern u8 gMinutes;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gSeasonTomorrow;
extern u8 gYear;

#endif