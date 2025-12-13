#include "common.h"

#include "game/weather.h"

#include "ld_symbols.h"

#include "system/globalSprites.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"

#include "game/game.h"
#include "game/gameStatus.h"

#include "buffers/buffers.h"

// bss
u8 gWeather;
u8 gForecast;

u8 weatherLightingAdjustments[5][4] = { 
    { 255, 255, 255, 255 }, // sunny
    { 176, 176, 192, 176 }, // rain
    { 255, 255, 255, 255 }, // rain alternate
    { 176, 176, 192, 176 }, // snow
    { 255, 255, 255, 255 } // typhoon
};

static const u8 weatherForecastProbabilities[4][5];
static const u8 weatherForecastProbabilitiesAlternate[4][5];

//INCLUDE_ASM("asm/nonmatchings/game/weather", setWeatherSprites);

// load rain/snow sprites
void setWeatherSprites(void) {
    
    u8 i;
    
    u16 animationIndex = ((gWeather == RAIN || gWeather == 4) == FALSE) ? 0xFF : 0;

    if (gWeather == SNOW) {
        animationIndex = 1;
    }
    
    if (animationIndex != 0xFF) {

        for (i = 0; i < 10; i++) {
        
            dmaSprite(0x6B + i, &_rainTextureSegmentRomStart, &_rainTextureSegmentRomEnd, &_rainAssetsIndexSegmentRomStart, &_rainAssetsIndexSegmentRomEnd, 0, 0, (void*)WEATHER_SPRITE_BUFFER, NULL, 0x802A5DC0, 0x802A5EC0, 0x802A5FC0, NULL, 0, TRUE);
            setSpriteScale(0x6B + i, 1.0f, 1.0f, 1.0f);
            setSpriteRenderingLayer(0x6B + i, 4);
            setSpriteColor(0x6B + i, 0xFF, 0xFF, 0xFF, 0xFF);
            
            setMapWeatherSprite(MAIN_MAP_INDEX, i + 3, 0x6B + i, animationIndex);

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/weather", setForecast);

void setForecast(void) {
    
    u8 i;
    u8 currentScore;
    u8 score;
    
    u8 buffer[4][5];
    u8 buffer2[4][5];

    memcpy(buffer, weatherForecastProbabilities, 20);
    memcpy(buffer2, weatherForecastProbabilitiesAlternate, 20);
    
    score = getRandomNumberInRange(1, 100);
    
    currentScore = 0;
    i = 0;

    do {
        
        if (checkLifeEventBit(0x44) || checkLifeEventBit(0x46)) {
           currentScore += buffer2[gSeasonTomorrow-1][i];
        } else {
            currentScore += buffer[gSeasonTomorrow-1][i];
        }
        
        i++;
        
    } while (i < 5 && currentScore < score);
    
    gForecast = i;

}

//INCLUDE_ASM("asm/nonmatchings/game/weather", setWeatherLighting);

Vec4f setWeatherLighting(u8 weather) {

    Vec4f vec;
    
    vec.r = weatherLightingAdjustments[weather - 1][0];
    vec.g = weatherLightingAdjustments[weather - 1][1];
    vec.b = weatherLightingAdjustments[weather - 1][2];
    vec.a = weatherLightingAdjustments[weather - 1][3];
        
    return vec;

}

//INCLUDE_RODATA("asm/nonmatchings/gameweather", weatherForecastProbabilities);

// weights for weather types depending on season for forecasting
// percents out of 100
// i.e., sunny will be 85% of the forecasts in spring
static const u8 weatherForecastProbabilities[4][5] = { 
            { 85, 15, 0, 0, 0 },
            { 85, 10, 0, 0, 5 },
            { 85, 15, 0, 0, 0 },
            { 70, 0, 30, 0, 0 } 
            };

//INCLUDE_RODATA("asm/nonmatchings/gameweather", weatherForecastProbabilitiesAlternate);

// alternate weights; weather vane? or every other year?
static const u8 weatherForecastProbabilitiesAlternate[4][5] = { 
            { 85, 15, 0, 0, 0 },
            { 85, 13, 0, 0, 2 },
            { 85, 15, 0, 0, 0 },
            { 70, 0, 30, 0, 0 } 
            };