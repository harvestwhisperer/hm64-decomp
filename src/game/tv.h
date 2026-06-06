#ifndef _TV_H_
#define _TV_H_

#include "common.h"

extern void initializeTVAssets(void);
extern void incrementVarietyShowCounter(void); 
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
#define EDUCATIONAL_CHANNEL_1 9
#define EDUCATIONAL_CHANNEL_2 10
#define EDUCATIONAL_CHANNEL_3 11
#define VARIETY_CUT_THE_VILLAIN 12
#define VARIETY_THE_MONMON_SHOW 14
#define VARIETY_SPORTS 15
#define VARIETY_SECRET_SOLIDER 17
#define VARIETY_ICHIGO 18

#define TV_MODE_LOAD 0
#define TV_MODE_DIALOGUE 1
#define TV_MODE_WATCHING 2

// D_80170270
typedef struct {
    u16 textIndex;
    u8 contentIndex;
    u8 pictureIndex;
    u8 mode;
    u8 channelIndex;
    u8 varietyShowEpisodeCounters[7];
} TVContext;

extern TVContext tvContext;

#endif