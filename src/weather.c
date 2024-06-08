#include "common.h"

#include "weather.h"

#include "system/map.h"
#include "system/sprite.h"

// bss
extern u8 gWeather;

// data
extern u8 weatherLightingAdjustments[5][4] = { 
    { 0xFF, 0xFF, 0xFF, 0xFF }, // sunny
    { 0xB0, 0xB0, 0xC0, 0xB0 }, // rain
    { 0xFF, 0xFF, 0xFF, 0xFF }, // rain alternate
    { 0xB0, 0xB0, 0xC0, 0xB0 }, // snow
    { 0xFF, 0xFF, 0xFF, 0xFF } // typhoon
};

extern u32 _rainTextureSegmentRomStart;
extern u32 _rainTextureSegmentRomEnd;
extern u32 _rainIndexSegmentRomStart;
extern u32 _rainIndexSegmentRomEnd;

//INCLUDE_ASM(const s32, "weather", func_800DC360);

void func_800DC360(void) {
    
    u8 i;
    
    u16 check = ((gWeather == RAIN || gWeather == 4) == FALSE) ? 0xFF : 0;

    if (gWeather == SNOW) {
        check = 1;
    }
    
    if (check != 0xFF) {

        for (i = 0; i < 10; i++) {
        
            func_8002B138(i+0x6B, &_rainTextureSegmentRomStart, &_rainTextureSegmentRomEnd, &_rainIndexSegmentRomStart, &_rainIndexSegmentRomEnd, 0, 0, (void*)RAIN_TEXTURE_VADDR, NULL, 0x802A5DC0, 0x802A5EC0, 0x802A5FC0, 0, 0, 1);
            func_8002BD90(i+0x6B, 1.0f, 1.0f, 1.0f);
            func_8002C7EC(i+0x6B, 4);
            func_8002C914(i+0x6B, 0xFF, 0xFF, 0xFF, 0xFF);

            func_80034D64(MAIN_MAP_INDEX, i+3, i+0x6B, check);
        }
    }
}

#ifdef PERMUTER
void setForecast(void) {

    u8 i;
    u8 j;
    u8 temp;
    
    u8 *ptr1;
    u8 *ptr2;
    
    UnknownStruct struct1;
    UnknownStruct struct2;

    // this is memcpy
    ptr1 = (u8*)D_80123438;
    struct1 = *(UnknownStruct*)ptr1;
    ptr2 = (u8*)D_8012344C;
    struct2 =* (UnknownStruct*)ptr2;
    
    temp = getRandomNumberInRange(1, 100);
    
    i = 0;
    j = 0;
    
    while (i < 5 && j < temp) {
        if (checkLifeEventBit(0x44) || checkLifeEventBit(0x46)) {
            j += ptr2[gNextSeason*5 + i];
        } else {
            j += ptr1[gNextSeason*5 + i-5];
        }
        j+=i;
        i++;
    } 
    
    gForecast = i;
    
}
#else
INCLUDE_ASM(const s32, "weather", setForecast);
#endif

//INCLUDE_ASM(const s32, "weather", setWeatherLighting);

Vec4f setWeatherLighting(u8 weather) {

    Vec4f vec;
    
    vec.r = weatherLightingAdjustments[weather - 1][0];
    vec.g = weatherLightingAdjustments[weather - 1][1];
    vec.b = weatherLightingAdjustments[weather - 1][2];
    vec.a = weatherLightingAdjustments[weather - 1][3];
        
    return vec;
}

INCLUDE_RODATA(const s32, "weather", D_80123438);

INCLUDE_RODATA(const s32, "weather", D_8012344C);