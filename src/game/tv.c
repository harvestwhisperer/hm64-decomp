#include "common.h"

#include "game/tv.h"

#include "ld_symbols.h"

#include "system/controller.h"
#include "system/globalSprites.h"
#include "system/map.h"
#include "system/mapController.h"
#include "system/math.h"
#include "system/message.h"
#include "system/sprite.h"

#include "game/game.h"
#include "game/gameAudio.h"
#include "game/overlayScreens.h"
#include "game/time.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "assetIndices/sfxs.h"
#include "assetIndices/sprites.h"

#include "buffers/buffers.h"
       
// forward declarations   
void setTVContentIndex();                                  
void setTVDialogueIndex ();       
void setTVPictureIndex();            
void initializeTVAssets();      

TVContext tvContext;

#define CHANGE_CHANNEL(channel) \
    do {                                  \
        if (set) break; \
        tvContext.channelIndex = channel;                        \
        updateTVCButtonsByChannel();                       \
        initializeTVAssets();    \
        playSfx(TV_OFF_SFX); \
        set = TRUE; \
    } while(0)

#define TURN_OFF_TV() \
    do { \
        if (set) break; \
        deactivateMapObject(MAIN_MAP_INDEX, 9); \
        closeOverlayScreen(); \
        setMainLoopCallbackFunctionIndex(MAIN_GAME); \
        playSfx(TV_OFF_SFX); \
    } while(0)

// alternate static inline version
static inline bool changeChannel(u8 channelIndex) {
    tvContext.channelIndex = channelIndex;
    updateTVCButtonsByChannel();
    initializeTVAssets();
    playSfx(TV_OFF_SFX);
    return TRUE;
}

void initializeTVAssets(void) {

    setTVContentIndex();
    setTVDialogueIndex ();
    setTVPictureIndex();
    
    deactivateMapObject(MAIN_MAP_INDEX, 9);

    dmaSprite(TV_CONTENT, &_tvContentTextureSegmentRomStart, &_tvContentTextureSegmentRomEnd, &_tvContentAssetsIndexSegmentRomStart, &_tvContentAssetsIndexSegmentRomEnd, &_tvContentSpritesheetIndexSegmentRomStart, &_tvContentSpritesheetIndexSegmentRomEnd, (u8*)MAP_OBJECT_TV_TEXTURE_1, (u8*)MAP_OBJECT_TV_TEXTURE_2, (u16*)MAP_OBJECT_TV_PALETTE, (AnimationFrameMetadata*)MAP_OBJECT_TV_ANIM_METADATA, (u32*)MAP_OBJECT_TV_SPRITESHEET_INDEX, (u32*)MAP_OBJECT_TV_TEXTURE_TO_PALETTE_LOOKUP, 1, TRUE);

    setSpriteScale(TV_CONTENT, 2.0f, 2.0f, 1.0f);
    setSpriteRotation(TV_CONTENT, 45.0f, -45.0f, 0.0f);
    setSpriteBlendMode(TV_CONTENT, SPRITE_BLEND_ALPHA_DECAL);
    setSpriteColor(TV_CONTENT, 255, 255, 255, 255);
    
    setMapObject(MAIN_MAP_INDEX, 9, TV_CONTENT, tvContext.pictureIndex, -12.0f, 36.0f, -154.0f, 0xFF, 0xFE, 0, 0);

    // TV content sprites share the same vaddr as GROUND_OBJECTS_TEXTURE_BUFFER (0x802EB800)
    mapControllers[MAIN_MAP_INDEX].flags &= ~MAP_CONTROLLER_GROUND_OBJECTS_CACHED;

    tvContext.mode = 0;

}

// variety show episode incrementer
void incrementVarietyShowCounter(void) {
    
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

void setTVContentIndex(void) {

    tvContext.contentIndex = 0xFF;

    if (gHour < 6) {
        tvContext.contentIndex = BLUE_SCREEN;
        return;
    }

    if (gWeather == TYPHOON) {
        tvContext.contentIndex = 19;
        return;
    }
    
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

                case MONDAY:
                case TUESDAY:
                    tvContext.contentIndex = EDUCATIONAL_CHANNEL_1;
                    return;

                case WEDNESDAY:
                case THURSDAY:
                   tvContext.contentIndex = EDUCATIONAL_CHANNEL_2;
                   return;
                
                case FRIDAY:
                case SATURDAY:
                    tvContext.contentIndex = EDUCATIONAL_CHANNEL_3;
                    break;

                case SUNDAY:
                   switch (getRandomNumberInRange(0, 2)) {
                       case 0:
                           tvContext.contentIndex = EDUCATIONAL_CHANNEL_1;
                           return;
                       case 1:
                           tvContext.contentIndex = EDUCATIONAL_CHANNEL_2;
                           return;
                       case 2:
                           tvContext.contentIndex = EDUCATIONAL_CHANNEL_3;
                           break;
                       default:
                           break;
                   }

                    break;
                
                default:
                    return;

            }
            
            break;
        
        case STATIC_CHANNEL:
            tvContext.contentIndex = STATIC;
            break;
    }
    
}

void setTVDialogueIndex (void) {

    tvContext.textIndex = 0xFF;

    switch (tvContext.contentIndex) {

        case BLUE_SCREEN:
            tvContext.textIndex = 0xFF;
            break;

        default:
            break;

        case STATIC:
            tvContext.textIndex = 0xFF;
            break;

        case SUNNY_FORECAST:
            switch (gSeasonTomorrow) {                      
                default:                                    
                    break;
                case SPRING:                                     
                    tvContext.textIndex = 0;
                    break;
                case SUMMER:                                     
                    tvContext.textIndex = 3;
                    break;
                case AUTUMN:                                     
                    tvContext.textIndex = 5;
                    break;
                case WINTER:
                    tvContext.textIndex = 8;
                    break;
                }
            break;

        case RAIN_FORECAST:
             switch (gSeasonTomorrow) {                      
                default:                                    
                    break;
                case SPRING:                                     
                    tvContext.textIndex = 1;
                    break;
                case SUMMER:                                     
                    tvContext.textIndex = 4;
                    break;
                case AUTUMN:                                     
                    tvContext.textIndex = 6;
                    break;
                }
            break;

        case SNOW_FORECAST:
            tvContext.textIndex = 9;
            break;

        case TYPHOON_FORECAST:
            tvContext.textIndex = 10;
            break;

        case NEWS_SPRING:
            if (gDayOfMonth == 1) {
                tvContext.textIndex = 11;
            }
            if (1 < gDayOfMonth && gDayOfMonth < 7) {
                tvContext.textIndex = 12;
            }
            if (gDayOfMonth == 7) {
                tvContext.textIndex = 13;
            }
            if (gDayOfMonth == 8) {
                tvContext.textIndex = 14;
            }
            if (8 < gDayOfMonth && gDayOfMonth < 16) {
                tvContext.textIndex = 15;
            }
            if (gDayOfMonth == 16) {
                tvContext.textIndex = 16;
            }
            if (gDayOfMonth == 17) {
                tvContext.textIndex = 17;
            }
            if (17 < gDayOfMonth && gDayOfMonth < 22) {
                tvContext.textIndex = 18;
            }
            if (gDayOfMonth == 22) {
                tvContext.textIndex = 19;
            }
            if (gDayOfMonth == 23) {
                tvContext.textIndex = 20;
            }
            if (23 < gDayOfMonth && gDayOfMonth < 30) {
                tvContext.textIndex = 21;
            }
            if (gDayOfMonth == 30) {
                 tvContext.textIndex = 22;
            }
            break;

        case NEWS_SUMMER:
            if (gDayOfMonth == 1) {
                tvContext.textIndex = 23;
            }
            if (1 < gDayOfMonth && gDayOfMonth < 8) {
                tvContext.textIndex = 24;
            }
            if (gDayOfMonth == 8) {
                tvContext.textIndex = 25;
            }
            if (gDayOfMonth == 9) {
                tvContext.textIndex = 26;
            }
            if (9 < gDayOfMonth && gDayOfMonth < 16) {
                tvContext.textIndex = 27;
            }
            if (gDayOfMonth == 16) {
                tvContext.textIndex = 28;
            }
            if (gDayOfMonth == 17) {
                tvContext.textIndex = 29;
            }
            if (17 < gDayOfMonth && gDayOfMonth < 23) {
                tvContext.textIndex = 30;
            }
            if (gDayOfMonth == 23) {
                tvContext.textIndex = 31;
            }
            if (gDayOfMonth == 24) {
                tvContext.textIndex = 32;
            }
            if (24 < gDayOfMonth && gDayOfMonth < 31) {
                tvContext.textIndex = 33;
            }
            break;

        case NEWS_AUTUMN:
            if (0 < gDayOfMonth && gDayOfMonth < 3) {
                tvContext.textIndex = 34;
            }
            if (gDayOfMonth == 3) {
                tvContext.textIndex = 35;
            }
            if (gDayOfMonth == 4) {
                tvContext.textIndex = 36;
            }
            if (4 < gDayOfMonth && gDayOfMonth < 10) {
                tvContext.textIndex = 37;
            }
            if (9 < gDayOfMonth && gDayOfMonth < 12) {
                tvContext.textIndex = 38;
            }
            if (gDayOfMonth == 12) {
                tvContext.textIndex = 39;
            }
            if (12 < gDayOfMonth && gDayOfMonth < 19) {
                tvContext.textIndex = 40;
            }
            if (gDayOfMonth == 19) {
                tvContext.textIndex = 41;
            }
            if (gDayOfMonth == 0x14) {
                tvContext.textIndex = 42;
            }
            if (20 < gDayOfMonth && gDayOfMonth < 27) {
                tvContext.textIndex = 43;
            }
            if (gDayOfMonth == 27) {
                tvContext.textIndex = 16;
            }
            if (gDayOfMonth == 28) {
                tvContext.textIndex = 17;
            }
            if (28 < gDayOfMonth && gDayOfMonth < 31) {
                tvContext.textIndex = 44;
            }
            break;

        case NEWS_WINTER:
            if (gDayOfMonth == 1) {
                tvContext.textIndex = 45;
            }
            if (1 < gDayOfMonth && gDayOfMonth < 6) {
                tvContext.textIndex = 46;
            }
            if (5 < gDayOfMonth && gDayOfMonth < 9) {
                tvContext.textIndex = 47;
            }
            if (gDayOfMonth == 9) {
                tvContext.textIndex = 48;
            }
            if (gDayOfMonth == 10) {
                tvContext.textIndex = 49;
            }
            if (10 < gDayOfMonth && gDayOfMonth < 18) {
                tvContext.textIndex = 50;
            }
            if (gDayOfMonth == 18) {
                tvContext.textIndex = 51;
            }
            if (gDayOfMonth == 19) {
                tvContext.textIndex = 52;
            }
            if (19 < gDayOfMonth && gDayOfMonth < 23) {
                tvContext.textIndex = 53;
            }
            if (gDayOfMonth == 23) {
                tvContext.textIndex = 54;
            }
            if (gDayOfMonth == 24) {
                tvContext.textIndex = 55;
            }
            if (gDayOfMonth >= 25) {
                tvContext.textIndex = 56;
            }
            if (gDayOfMonth == 26) {
                tvContext.textIndex = 57;
            }
            if (gDayOfMonth == 27) {
                tvContext.textIndex = 58;
            }
            if (27 < gDayOfMonth && gDayOfMonth < 31) {
                tvContext.textIndex = 59;
            }
            break;

        case EDUCATIONAL_CHANNEL_1:
            if (gDayOfWeek == SUNDAY) {
                tvContext.textIndex = 77;
            } else {
                switch (getRandomNumberInRange(0, 2)) {    
                    case 0:
                        tvContext.textIndex = 60;
                        break;
                    case 1:
                        tvContext.textIndex = 61;
                        break;
                    case 2:
                        tvContext.textIndex = 62;
                        break;
                    default:
                        return;
                }
            }
            break;

        case EDUCATIONAL_CHANNEL_2:                                 
            if (gDayOfWeek == SUNDAY) {
                tvContext.textIndex = 78;
            } else {
                switch (gSeason) {                          
                    case 1:                          
                        if (gDayOfWeek == WEDNESDAY) {
                            tvContext.textIndex = 63;
                        } else {
                            tvContext.textIndex = 64;
                        }
                        break;
                    case 2:                                     
                        if (gDayOfWeek == WEDNESDAY) {
                            tvContext.textIndex = 65;
                        } else {
                            tvContext.textIndex = 66;
                        }
                        break;
                    case 3:                                     
                        if (gDayOfWeek == WEDNESDAY) {
                            tvContext.textIndex = 67;
                        } else {
                            tvContext.textIndex = 68;
                        }
                        break;
                    case 4:                                     
                        if (gDayOfWeek == WEDNESDAY) {
                            tvContext.textIndex = 69;
                        } else {
                            tvContext.textIndex = 70;
                        }
                        break;
                }
            }
            break;

        case EDUCATIONAL_CHANNEL_3:
            if (gDayOfWeek == SUNDAY) {
                tvContext.textIndex = 79;
            } else {
                switch (getRandomNumberInRange(0, 5)) {                        
                    case 0:                                     
                        tvContext.textIndex = 71;
                        break;
                    case 1:                                     
                        tvContext.textIndex = 72;
                        break;
                    case 2:                                     
                        tvContext.textIndex = 73;
                        break;
                    case 3:                                     
                        tvContext.textIndex = 74;
                        break;
                    case 4:                                     
                        tvContext.textIndex = 75;
                        break;
                    case 5:                                     
                        tvContext.textIndex = 76;
                        break;
                }
            }
            break;

        case VARIETY_CUT_THE_VILLAIN:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[0] + 80;
            break;

        case 13:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[1] + 90;
            break;

        case VARIETY_THE_MONMON_SHOW:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[2] + 100;
            break;

        case VARIETY_SPORTS:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[3] + 110;
            break;

        case 16:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[4] + 120;
            break;

        case VARIETY_SECRET_SOLIDER:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[5] + 130;
            break;

        case VARIETY_ICHIGO:
            tvContext.textIndex = tvContext.varietyShowEpisodeCounters[6] + 150;
            break;        

    }
    
}

void setTVPictureIndex(void) {

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
        case EDUCATIONAL_CHANNEL_1:
            tvContext.pictureIndex = EDUCATION_CHANNEL_PICTURE;
            break;
        case EDUCATIONAL_CHANNEL_2:
            tvContext.pictureIndex = EDUCATION_CHANNEL_PICTURE;
            break;
        case EDUCATIONAL_CHANNEL_3:
            tvContext.pictureIndex = EDUCATION_CHANNEL_PICTURE;
            break;

        // variety show: unused in US version
        case VARIETY_CUT_THE_VILLAIN:
            // Cut the Villain!
            tvContext.pictureIndex = 11;
            break;

        case 13:
            if (tvContext.varietyShowEpisodeCounters[1] == 3 || tvContext.varietyShowEpisodeCounters[1] == 6) {
                tvContext.pictureIndex = 13;
            } else if (tvContext.varietyShowEpisodeCounters[1] == 7) {
                tvContext.pictureIndex = 13;
                break;
            } else {
                // Sing Tonight!
                tvContext.pictureIndex = 12;
            }
            break;
            
        case VARIETY_THE_MONMON_SHOW:
            // The Monmon Show
            tvContext.pictureIndex = 14;
            break;        

        case VARIETY_SPORTS:
            // sports
            tvContext.pictureIndex = 15;
            break;

        case 16:
            if (!getRandomNumberInRange(0, 1)) {
                // Red String of Fate
                tvContext.pictureIndex = 17;
                break;
            } else {
                tvContext.pictureIndex = 16;
            }
            break;

        case VARIETY_SECRET_SOLIDER:
            // Secret Solider: Gon!
            tvContext.pictureIndex = 18;
            break;

        case VARIETY_ICHIGO:
            // Ichigo, Girl Detective
            tvContext.pictureIndex = 19;
            break;
            
        default:
            break;

    }
    
}

void tvMainLoopCallback(void) {

    bool set = FALSE;

    switch (tvContext.mode) {

        case TV_MODE_LOAD:

            if (tvContext.textIndex != 0xFF) {
                
                setMessageBoxViewSpacePosition(0, 0, -64.0f, 352.0f);
                setMessageBoxSpriteIndices(0, 1, 0, 0);
                setMessageBoxInterpolationWithFlags(0, -4, 0);
                initializeMessageBox(MAIN_MESSAGE_BOX_INDEX, 8, tvContext.textIndex, 0);

                tvContext.mode++;

            } else {                
                tvContext.mode = TV_MODE_WATCHING;
            }

            break;

        case TV_MODE_DIALOGUE:

            if (messageBoxes[MAIN_MESSAGE_BOX_INDEX].flags & 4) {
                tvContext.mode++;
            }

            break;

        case TV_MODE_WATCHING:

            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_UP)) {
                CHANGE_CHANNEL(WEATHER_CHANNEL);
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_RIGHT)) {
                CHANGE_CHANNEL(NEWS_CHANNEL);
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_DOWN)) {
               CHANGE_CHANNEL(EDUCATION_CHANNEL);
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_C_LEFT)) {
                CHANGE_CHANNEL(STATIC_CHANNEL);
            }

            if (checkButtonPressed(CONTROLLER_1, BUTTON_B)) {
                TURN_OFF_TV();
            }

         break;
    }
}