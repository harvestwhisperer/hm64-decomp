#ifndef _TV_H_
#define _TV_H_

#include "common.h"

extern void func_800D8540(void);
extern void func_800D86D8(void); 
extern void tvMainLoopCallback(void);

#define WEATHER_CHANNEL 0
#define NEWS_CHANNEL 1
#define EDUCATION_CHANNEL 2
#define STATIC_CHANNEL 3
#define VARIETY_CHANNEL 3

// TODO: finish adding these
// content
#define BLUE_SCREEN 0
#define SUNNY_FORECAST 1
#define RAIN_FORECAST 2
#define SNOW_FORECAST 3
#define TYPHOON_FORECAST 4
#define STATIC 19

#define STATIC_PICTURE 0
#define BLUE_SCREEN_PICTURE 1
#define EDUCATION_CHANNEL_PICTURE 2
#define NEWS_SPRING 5
#define NEWS_SUMMER 6
#define NEWS_AUTUMN 7
#define NEWS_WINTER 8

#define TV_MODE_LOAD 0
#define TV_MODE_DIALOGUE 1
#define TV_MODE_WATCHING 2

// D_80170270
typedef struct {
    u16 dialogueIndex;
    u8 contentIndex;
    u8 pictureIndex;
    u8 mode;
    u8 channelIndex;
    u8 varietyShowEpisodeCounters[7];
} TVContext;

extern TVContext tvContext;

#endif