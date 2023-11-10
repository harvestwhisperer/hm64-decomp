#include "common.h"

#include "tv.h"

#include "system/controller.h"
#include "system/map.h"
#include "system/mathUtils.h"
#include "system/message.h"
#include "system/sprite.h"

#include "game.h"
#include "gameAudio.h"
#include "mainLoop.h"
#include "overlayScreens.h"
#include "weather.h"
       
// forward declarations   
void func_800D879C();                                  
void func_800D897C();       
void func_800D9298();            
void func_800D8540();      

TVContext tvContext;

extern u32 tvContentTexture_ROM_START;
extern u32 tvContentTexture_ROM_END;
extern u32 tvContentAssetsIndex_ROM_START;
extern u32 tvContentAssetsIndex_ROM_END;
extern u32 tvContentSpritesheetIndex_ROM_START;
extern u32 tvContentSpritesheetIndex_ROM_END;

//INCLUDE_ASM(const s32, "tv", func_800D8540);

void func_800D8540(void) {

    func_800D879C();
    func_800D897C();
    func_800D9298();
    
    func_80034E64(0, 9);

    func_8002B138(0x6A, &tvContentTexture_ROM_START, &tvContentTexture_ROM_END, &tvContentAssetsIndex_ROM_START, &tvContentAssetsIndex_ROM_END, &tvContentSpritesheetIndex_ROM_START, &tvContentSpritesheetIndex_ROM_END, (void*)OBJECT_SPRITE_BANK_1_START, 0x802EC800, 0x802ED800, 0x802EDB00, 0x802EDE00, 0x802EDF00, 1, 1);
    
    func_8002BD90(0x6A, 2.0f, 2.0f, 1.0f);
    func_8002BE14(0x6A, 45.0f, -45.0f, 0.0f);
    func_8002C7EC(0x6A, 3);
    func_8002C914(0x6A, 0xFF, 0xFF, 0xFF, 0xFF);

    // set map struct 4
    func_80034C40(0, 9, 0x6A, tvContext.pictureIndex, -12.0f, 36.0f, -154.0f, 0xFF, 0xFE, 0, 0);

    tvContext.mode = 0;

}

// jtbl_80122760
//INCLUDE_ASM(const s32, "tv", func_800D86D8);

// variety show episode incrementer
void func_800D86D8(void) {
    
    switch (gDayOfWeek) {
        
        case TUESDAY:
            tvContext.varietyShowEpisodeCounters[0]++;
            if (tvContext.varietyShowEpisodeCounters[0] >= 10) {
                tvContext.varietyShowEpisodeCounters[0] = 0;
            }
            break;
        
        case WEDNESDAY:
            tvContext.varietyShowEpisodeCounters[1]++;
            if (tvContext.varietyShowEpisodeCounters[1] >= 10) {
                tvContext.varietyShowEpisodeCounters[1] = 0;
            }
            break;
        
        case THURSDAY:
            tvContext.varietyShowEpisodeCounters[2]++;
            if (tvContext.varietyShowEpisodeCounters[2] >= 10) {
                tvContext.varietyShowEpisodeCounters[2] = 0;
            }
            break;
        
        case FRIDAY:
            tvContext.varietyShowEpisodeCounters[3]++;
            if (tvContext.varietyShowEpisodeCounters[3] >= 10) {
                tvContext.varietyShowEpisodeCounters[3] = 0;
            }            
            break;

        case SATURDAY:

            tvContext.varietyShowEpisodeCounters[4]++;
            if (tvContext.varietyShowEpisodeCounters[4] >= 10) {
                tvContext.varietyShowEpisodeCounters[4] = 0;
            }
            break;
        
        case SUNDAY:
            tvContext.varietyShowEpisodeCounters[5]++;
            if (tvContext.varietyShowEpisodeCounters[5] >= 20) {
                tvContext.varietyShowEpisodeCounters[5] = 0;
            }
            break;
        
        case MONDAY:
            tvContext.varietyShowEpisodeCounters[6]++;
            if (tvContext.varietyShowEpisodeCounters[6] >= 10) {
                tvContext.varietyShowEpisodeCounters[6] = 0;
            }
            break;

        default:
            break;
    }
}

// jtbl_80122780
//INCLUDE_ASM(const s32, "tv", func_800D879C);

void func_800D879C(void) {

    tvContext.contentIndex = 0xFF;
    
    if (gHour < 6) {
        tvContext.contentIndex = BLUE_SCREEN;
        return;
    }

    if (gWeather != TYPHOON) {

        switch (tvContext.channelIndex) {

            case WEATHER_CHANNEL:

                switch (gForecast) {
                    case SUNNY:
                        tvContext.contentIndex = SUNNY_FORECAST;
                        break;
                    case RAIN:
                        tvContext.contentIndex = RAIN_FORECAST;
                        break;
                    case SNOW:
                        tvContext.contentIndex = SNOW_FORECAST;
                        break;
                    case TYPHOON:
                        tvContext.contentIndex = TYPHOON_FORECAST;
                        break;
                }

                break;

            case NEWS_CHANNEL:

                switch (gSeason) {

                    case SPRING:
                        tvContext.contentIndex = NEWS_SPRING;
                        break;
                    case SUMMER:
                        tvContext.contentIndex = NEWS_SUMMER;
                        break;
                    case AUTUMN:
                        tvContext.contentIndex = NEWS_AUTUMN;
                        break;
                    case WINTER:
                        tvContext.contentIndex = NEWS_WINTER;
                        break;
                }

                break;

            case EDUCATION_CHANNEL:

                switch (gDayOfWeek) {

                    case SUNDAY:
                       switch (getRandomNumberInRange(0, 2)) {
                           case 0:
                               tvContext.contentIndex = 9;
                               return;
                           case 1:
                               tvContext.contentIndex = 10;
                               return;
                           case 2:    
                               break;
                           default:
                               return;
                       } 

                        break;

                    case MONDAY:
                    case TUESDAY:
                        tvContext.contentIndex = 9;
                        return;

                    case WEDNESDAY:
                    case THURSDAY:
                       tvContext.contentIndex = 10;
                       return;

                    case FRIDAY:
                    case SATURDAY:
                        break;

                    default:
                        return;
                }
                
                tvContext.contentIndex = 11;
                break;
            
            case STATIC_CHANNEL:
                tvContext.contentIndex = STATIC;
                break;
        }
    } else {
        tvContext.contentIndex = 19;
    }
}

// jtbl_801227A0
// jtbl_801227F0
//INCLUDE_ASM(const s32, "tv", func_800D897C);

void func_800D897C(void) {

    tvContext.dialogueIndex = 0xFF;

    switch (tvContext.contentIndex) {
        case BLUE_SCREEN:
            tvContext.dialogueIndex = 0xFF;
            break;
        default:
            break;
        case STATIC:
            tvContext.dialogueIndex = 0xFF;
            break;
        case SUNNY_FORECAST:
            switch (gNextSeason) {                      
                default:                                    
                    break;
                case SPRING:                                     
                    tvContext.dialogueIndex = 0;
                    break;
                case SUMMER:                                     
                    tvContext.dialogueIndex = 3;
                    break;
                case AUTUMN:                                     
                    tvContext.dialogueIndex = 5;
                    break;
                case WINTER:
                    tvContext.dialogueIndex = 8;
                    break;
                }
            break;
        case RAIN_FORECAST:
             switch (gNextSeason) {                      
                default:                                    
                    break;
                case SPRING:                                     
                    tvContext.dialogueIndex = 1;
                    break;
                case SUMMER:                                     
                    tvContext.dialogueIndex = 4;
                    break;
                case AUTUMN:                                     
                    tvContext.dialogueIndex = 6;
                    break;
                }
            break;
        case SNOW_FORECAST:
            tvContext.dialogueIndex = 9;
            break;
        case TYPHOON_FORECAST:
            tvContext.dialogueIndex = 10;
            break;
        case NEWS_SPRING:
            if (gDayOfMonth == 1) {
                tvContext.dialogueIndex = 0xB;
            }
            if (1 < gDayOfMonth && gDayOfMonth < 7) {
                tvContext.dialogueIndex = 12;
            }
            if (gDayOfMonth == 7) {
                tvContext.dialogueIndex = 0xD;
            }
            if (gDayOfMonth == 8) {
                tvContext.dialogueIndex = 0xE;
            }
            if (8 < gDayOfMonth && gDayOfMonth < 16) {
                tvContext.dialogueIndex = 0xF;
            }
            if (gDayOfMonth == 16) {
                tvContext.dialogueIndex = 16;
            }
            if (gDayOfMonth == 17) {
                tvContext.dialogueIndex = 17;
            }
            if (17 < gDayOfMonth && gDayOfMonth < 22) {
                tvContext.dialogueIndex = 18;
            }
            if (gDayOfMonth == 22) {
                tvContext.dialogueIndex = 19;
            }
            if (gDayOfMonth == 23) {
                tvContext.dialogueIndex = 0x14;
            }
            if (23 < gDayOfMonth && gDayOfMonth < 30) {
                tvContext.dialogueIndex = 0x15;
            }
            
            if (gDayOfMonth == 30) {
                 tvContext.dialogueIndex = 22;
            }
            break;
        case NEWS_SUMMER:
            if (gDayOfMonth == 1) {
                tvContext.dialogueIndex = 23;
            }
            if (1 < gDayOfMonth && gDayOfMonth < 8) {
                tvContext.dialogueIndex = 24;
            }
            if (gDayOfMonth == 8) {
                tvContext.dialogueIndex = 25;
            }
            if (gDayOfMonth == 9) {
                tvContext.dialogueIndex = 26;
            }
            if (9 < gDayOfMonth && gDayOfMonth < 16) {
                tvContext.dialogueIndex = 27;
            }
            if (gDayOfMonth == 16) {
                tvContext.dialogueIndex = 0x1C;
            }
            if (gDayOfMonth == 17) {
                tvContext.dialogueIndex = 0x1D;
            }
            if (17 < gDayOfMonth && gDayOfMonth < 23) {
                tvContext.dialogueIndex = 0x1E;
            }
            if (gDayOfMonth == 23) {
                tvContext.dialogueIndex = 0x1F;
            }
            if (gDayOfMonth == 24) {
                tvContext.dialogueIndex = 0x20;
            }
            if (24 < gDayOfMonth && gDayOfMonth < 31) {
                tvContext.dialogueIndex = 0x21;
            }
            break;
        case NEWS_AUTUMN:
            if (0 < gDayOfMonth && gDayOfMonth < 3) {
                tvContext.dialogueIndex = 0x22;
            }
            if (gDayOfMonth == 3) {
                tvContext.dialogueIndex = 0x23;
            }
            if (gDayOfMonth == 4) {
                tvContext.dialogueIndex = 0x24;
            }
            if (4 < gDayOfMonth && gDayOfMonth < 10) {
                tvContext.dialogueIndex = 0x25;
            }
            if (9 < gDayOfMonth && gDayOfMonth < 12) {
                tvContext.dialogueIndex = 0x26;
            }
            if (gDayOfMonth == 12) {
                tvContext.dialogueIndex = 0x27;
            }
            if (12 < gDayOfMonth && gDayOfMonth < 19) {
                tvContext.dialogueIndex = 0x28;
            }
            if (gDayOfMonth == 19) {
                tvContext.dialogueIndex = 0x29;
            }
            if (gDayOfMonth == 0x14) {
                tvContext.dialogueIndex = 0x2A;
            }
            if (20 < gDayOfMonth && gDayOfMonth < 27) {
                tvContext.dialogueIndex = 0x2B;
            }
            if (gDayOfMonth == 27) {
                tvContext.dialogueIndex = 16;
            }
            if (gDayOfMonth == 28) {
                tvContext.dialogueIndex = 17;
            }
            if (28 < gDayOfMonth && gDayOfMonth < 31) {
                tvContext.dialogueIndex = 0x2C;
            }
            break;
        case NEWS_WINTER:
            if (gDayOfMonth == 1) {
                tvContext.dialogueIndex = 0x2D;
            }
            if (1 < gDayOfMonth && gDayOfMonth < 6) {
                tvContext.dialogueIndex = 0x2E;
            }
            if (5 < gDayOfMonth && gDayOfMonth < 9) {
                tvContext.dialogueIndex = 0x2F;
            }
            if (gDayOfMonth == 9) {
                tvContext.dialogueIndex = 0x30;
            }
            if (gDayOfMonth == 10) {
                tvContext.dialogueIndex = 0x31;
            }
            if (10 < gDayOfMonth && gDayOfMonth < 18) {
                tvContext.dialogueIndex = 0x32;
            }
            if (gDayOfMonth == 18) {
                tvContext.dialogueIndex = 0x33;
            }
            if (gDayOfMonth == 19) {
                tvContext.dialogueIndex = 0x34;
            }
            if (19 < gDayOfMonth && gDayOfMonth < 23) {
                tvContext.dialogueIndex = 0x35;
            }
            if (gDayOfMonth == 23) {
                tvContext.dialogueIndex = 0x36;
            }
            if (gDayOfMonth == 24) {
                tvContext.dialogueIndex = 0x37;
            }
            if (gDayOfMonth >= 25) {
                tvContext.dialogueIndex = 0x38;
            }
            if (gDayOfMonth == 26) {
                tvContext.dialogueIndex = 0x39;
            }
            if (gDayOfMonth == 27) {
                tvContext.dialogueIndex = 0x3A;
            }
            if (27 < gDayOfMonth && gDayOfMonth < 31) {
                tvContext.dialogueIndex = 0x3B;
            }
            break;
        case 9:
            if (gDayOfWeek == SUNDAY) {
                tvContext.dialogueIndex = 0x4D;
                break;
            }
            switch (getRandomNumberInRange(0, 2)) {    
                case 0:
                    tvContext.dialogueIndex = 0x3C;
                    break;
                case 1:
                    tvContext.dialogueIndex = 0x3D;
                    break;
                case 2:
                    tvContext.dialogueIndex = 0x3E;
                    break;
                default:
                    return;
            }
            break;
        case 10:                                 
            if (gDayOfWeek == 0) {
                tvContext.dialogueIndex = 0x4E;
                break;
            }
            switch (gSeason) {                          
                case 1:                          
                    if (gDayOfWeek == WEDNESDAY) {
                        tvContext.dialogueIndex = 0x3F;
                    } else {
                        tvContext.dialogueIndex = 0x40;
                    }
                    break;
                case 2:                                     
                    if (gDayOfWeek == WEDNESDAY) {
                        tvContext.dialogueIndex = 0x41;
                    } else {
                        tvContext.dialogueIndex = 0x42;
                    }
                    break;
                case 3:                                     
                    if (gDayOfWeek == WEDNESDAY) {
                        tvContext.dialogueIndex = 0x43;
                    } else {
                        tvContext.dialogueIndex = 0x44;
                    }
                    break;
                case 4:                                     
                    if (gDayOfWeek == WEDNESDAY) {
                        tvContext.dialogueIndex = 0x45;
                    } else {
                        tvContext.dialogueIndex = 0x46;
                    }
                    break;
                }
            break;
        case 11:
            if (gDayOfWeek == 0) {
                tvContext.dialogueIndex = 0x4F;
                break;
            }
            switch (getRandomNumberInRange(0, 5)) {                        
                case 0:                                     
                    tvContext.dialogueIndex = 0x47;
                    break;
                case 1:                                     
                    tvContext.dialogueIndex = 0x48;
                    break;
                case 2:                                     
                    tvContext.dialogueIndex = 0x49;
                    break;
                case 3:                                     
                    tvContext.dialogueIndex = 0x4A;
                    break;
                case 4:                                     
                    tvContext.dialogueIndex = 0x4B;
                    break;
                case 5:                                     
                    tvContext.dialogueIndex = 0x4C;
                    break;
                }
            break;
        case 12:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[0] + 0x50;
            break;
        case 13:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[1] + 0x5A;
            break;
        case 14:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[2] + 0x64;
            break;
        case 15:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[3] + 0x6E;
            break;
        case 16:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[4] + 0x78;
            break;
        case 17:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[5] + 0x82;
            break;
        case 18:
            tvContext.dialogueIndex = tvContext.varietyShowEpisodeCounters[6] + 0x96;
            break;        
    }
    
}

// jtbl_80122808
//INCLUDE_ASM(const s32, "tv", func_800D9298);

void func_800D9298(void) {

    switch (tvContext.contentIndex) {

        case BLUE_SCREEN:
            tvContext.pictureIndex = BLUE_SCREEN_PICTURE;
            break;
        case STATIC:
            tvContext.pictureIndex = STATIC_PICTURE;
            break;
        case SUNNY_FORECAST:
            tvContext.pictureIndex = 3;
            break;
        case RAIN_FORECAST:
            tvContext.pictureIndex = 4;
            break;
        case SNOW_FORECAST: 
            tvContext.pictureIndex = 5;
            break;
        case TYPHOON_FORECAST:
            tvContext.pictureIndex = 6;
            break;
        case NEWS_SPRING:
            tvContext.pictureIndex = 7;
            break;
        case NEWS_SUMMER:
            tvContext.pictureIndex = 8;
            break;
        case NEWS_AUTUMN:
            tvContext.pictureIndex = 9;
            break;
        case NEWS_WINTER:
            tvContext.pictureIndex = 10;
            break;
        case 9:
            tvContext.pictureIndex = EDUCATION_CHANNEL_PICTURE;
            break;
        case 10:
            tvContext.pictureIndex = EDUCATION_CHANNEL_PICTURE;
            break;
        case 11:
            tvContext.pictureIndex = EDUCATION_CHANNEL_PICTURE;
            break;

        // variety show: unused in US version
        case 12:
            tvContext.pictureIndex = 11;
            break;
        case 13:
            if (tvContext.varietyShowEpisodeCounters[1] == 3 || tvContext.varietyShowEpisodeCounters[1] == 6) {
                tvContext.pictureIndex = 13;
                break;
            }
            if (tvContext.varietyShowEpisodeCounters[1] == 7) {
                tvContext.pictureIndex = 13;
                break;
            }
            tvContext.pictureIndex = 12;
            break;
        case 14:
            tvContext.pictureIndex = 14;
            break;        
        case 15:
            tvContext.pictureIndex = 15;
            break;
        case 16:
            if (!getRandomNumberInRange(0, 1)) {
                tvContext.pictureIndex = 17;
                break;
            }
            tvContext.pictureIndex = 16;
            break;
        case 17:
            tvContext.pictureIndex = 18;
            break;
        case 18:
            tvContext.pictureIndex = 19;
            break;
            
        default:
            break;
    }
}
 
//INCLUDE_ASM(const s32, "tv", tvMainLoopCallback);

void tvMainLoopCallback(void) {

    u8 set = 0;

    switch (tvContext.mode) {

        case TV_MODE_LOAD:

            if (tvContext.dialogueIndex != 0xFF) {
                func_8003F54C(0, 0, -64.0f, 352.0f);
                func_8003F690(0, 1, 0, 0);
                func_8003F360(0, ~(1 | 2), 0);
                func_8003DDF8(0, 8, tvContext.dialogueIndex, 0);
                tvContext.mode++;
            } else {                
                tvContext.mode = TV_MODE_WATCHING;
            }

            break;


        case TV_MODE_DIALOGUE:

            if (dialogueBoxes[0].flags & 4) {
                tvContext.mode++;
            }

            break;

        case TV_MODE_WATCHING:

            if (func_8004D380(CONTROLLER_1, BUTTON_C_UP)) {
                tvContext.channelIndex = 0;
                set = 1;
                func_800B3A60();
                func_800D8540();
                setAudio(0x2F);
            }

            if (func_8004D380(CONTROLLER_1, BUTTON_C_RIGHT)) {
                // probably needs a static inline call
                do {} while (0);
                if (!set) {
                    tvContext.channelIndex = 1;
                    func_800B3A60();
                    set = 1;
                    func_800D8540();
                    setAudio(0x2F);
                }
            }

            if (func_8004D380(CONTROLLER_1, BUTTON_C_DOWN)) {
                if (!set) {
                    tvContext.channelIndex = 2;
                    set = 1;
                    func_800B3A60();
                    func_800D8540();
                    setAudio(0x2F);
                }
            }

            if (func_8004D380(CONTROLLER_1, BUTTON_C_LEFT)) {
                if (!set) {
                    tvContext.channelIndex = 3;
                    set = 1;
                    func_800B3A60();
                    func_800D8540();
                    setAudio(0x2F);
                }
            }

            if (func_8004D380(CONTROLLER_1, BUTTON_B)) {
                if (!set) {
                    func_80034E64(0, 9);
                    func_800B2CE0();
                    setMainLoopCallbackFunctionIndex(1);
                    setAudio(0x2F);
                }
            }

            break;
    }
    
}