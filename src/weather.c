#include "common.h"

#include "weather.h"

#include "system/map.h"
#include "system/sprite.h"

// likely bss
extern u8 gWeather;

// data
extern u8 D_80118B50[5][4];

extern u32 rainTexture_ROM_START;
extern u32 rainTexture_ROM_END;
extern u32 rainIndex_ROM_START;
extern u32 rainIndex_ROM_END;

//INCLUDE_ASM(const s32, "weather", func_800DC360);

void func_800DC360(void) {
    
    u8 i;
    
    u16 check = ((gWeather == RAIN || gWeather == 4) == FALSE) ? 0xFF : 0;

    if (gWeather == SNOW) {
        check = 1;
    }
    
    if (check != 0xFF) {

        for (i = 0; i < 10; i++) {
        
            // sprite funcs
            func_8002B138(i+0x6B, &rainTexture_ROM_START, &rainTexture_ROM_END, &rainIndex_ROM_START, &rainIndex_ROM_END, 0, 0, (void*)RAIN_TEXTURE_VADDR, NULL, 0x802A5DC0, 0x802A5EC0, 0x802A5FC0, 0, 0, 1);
            func_8002BD90(i+0x6B, 1.0f, 1.0f, 1.0f);
            func_8002C7EC(i+0x6B, 4);
            func_8002C914(i+0x6B, 0xFF, 0xFF, 0xFF, 0xFF);

            // map
            func_80034D64(0, i+3, i+0x6B, check);
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
    
    vec.r = D_80118B50[weather - 1][0];
    vec.g = D_80118B50[weather - 1][1];
    vec.b = D_80118B50[weather - 1][2];
    vec.a = D_80118B50[weather - 1][3];
        
    return vec;
}

// static const u8 D_80123438[20];
INCLUDE_RODATA(const s32, "weather", D_80123438);

INCLUDE_RODATA(const s32, "weather", D_8012344C);
