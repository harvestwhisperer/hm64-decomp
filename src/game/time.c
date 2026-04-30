#include "common.h"

#include "game/time.h"

#include "system/message.h"

#include "game/cutscenes.h"
#include "game/game.h"
#include "game/gameAudio.h"
#include "game/gameStatus.h"
#include "game/level.h"
#include "game/player.h"
#include "game/weather.h"

#include "mainLoop.h"

#include "assetIndices/maps.h"
                    
// forward declarations           
void setGlobalSeasonName(u8);
void clearMonthlyLetterBits(void);
void setupNewYear(void);
void updateClock(u8);

// shared bss
u8 gSeconds;
u8 gHour;
u8 gMinutes;
u8 gDayOfWeek;
u8 gDayOfMonth;
u8 gSeason;
u8 gSeasonTomorrow;
u8 gYear;

// shared bss
u8 gCurrentSeasonName[6];
u8 gGlobalSeasonName[6];

//INCLUDE_ASM("asm/nonmatchings/game/time", handleTimeUpdates);

void handleTimeUpdates(void) {
    
    if ((getLevelFlags(gBaseMapIndex) & LEVEL_TIME_FLOWS) && !checkDailyEventBit(SUSPEND_TIME_DURING_ANIMATION)) {
        updateClock(TRUE);
    }
    
    if (gHour == 0 && gMinutes == 0 && gSeconds == 0) {
        setSeasonName();
    }
    
    if (gHour == 6) {
        
        if (!checkDailyEventBit(DAY_START_6AM_GUARD)) {

            setPlayerAction(254, ANIM_DEFAULT);
            handleExitLevel(1, END_OF_DAY_2);
            
        }

    } else {
        
        clearDailyEventBit(DAY_START_6AM_GUARD);

        if (gCutsceneCompletionFlags >= 0) {
            setLevelLighting(1, NO_OP);
        }

    }
    
    if (NIGHTTIME) {
        
        if (!checkDailyEventBit(NIGHTTIME_AUDIO_STOPPED)) {

            stopAudioSequenceWithDefaultFadeOut(gCurrentAudioSequenceIndex);
            setDailyEventBit(NIGHTTIME_AUDIO_STOPPED);

        } else if (!checkDailyEventBit(NIGHTTIME_AUDIO_STARTED) && gCurrentAudioSequenceIndex != 0xFF && checkDefaultSequenceChannelOpen(gCurrentAudioSequenceIndex)) {

            setLevelAudio(gBaseMapIndex, gSeason, gHour);
            setAudioSequenceVolume(gCurrentAudioSequenceIndex, gAudioSequenceVolume);
 
            setDailyEventBit(NIGHTTIME_AUDIO_STARTED);

        }
        
    }
    
    if (gHour < 6 && gWeather != RAIN && gBaseMapIndex != BEACH && !checkDailyEventBit(PREDAWN_AUDIO_STOPPED)) {
        stopAudioSequenceWithDefaultFadeOut(gCurrentAudioSequenceIndex);
        setDailyEventBit(PREDAWN_AUDIO_STOPPED);
    }
    
    handleTimedDailyCutscenes(gBaseMapIndex);
    
}

//INCLUDE_ASM("asm/nonmatchings/game/time", updateClock);

void updateClock(u8 incrementSeconds) {

    if (incrementSeconds == TRUE) {
        gSeconds += 10;
    }
    
    if (gSeconds >= 60) {
        gSeconds = 0;
        gMinutes++;
    }
    
    if (gMinutes >= 60) {
        gMinutes = 0;
        gHour++;
        if (gHour == 6) {
            gDayOfMonth++;
            gDayOfWeek++;
        }
    }

    if (gHour >= 24) {
        gHour = 0;
    }

    if (gDayOfWeek >= 7) {
        gDayOfWeek = SUNDAY;
    }

    if (gDayOfMonth >= 31) {
        gDayOfMonth = 1;
        gSeason++;
        clearMonthlyLetterBits();
    }

    if (gSeason >= 5) {
        gSeason = 1;
        gYear++;
        setupNewYear();
    }
    
    if (gYear >= 100) {
        gYear = 99;
    }
    
    gSeasonTomorrow = gSeason;
    
    if ((gDayOfMonth + 1) >= 31) {
        gSeasonTomorrow = gSeason + 1;
    }
    
    if (gSeasonTomorrow >= 5) {
        gSeasonTomorrow = SPRING;
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/time", setupNewYear);

void setupNewYear(void) {

    gHarvestKing = gHarvestCoinFinder;  
    gFlowerFestivalGoddess = 0xFF;
    gHarvestCoinFinder = 0xFF;
    gVoteForFlowerFestivalGoddess = 0xFF;
    
    clearLifeEventBit(WEATHER_WISH);

    clearReadLetterBit(MAIL_MARIA_SPRING);
    clearReadLetterBit(MAIL_MARIA_SUMMER);
    clearReadLetterBit(MAIL_MARIA_AUTUMN);
    clearReadLetterBit(MAIL_MARIA_WINTER);
    clearReadLetterBit(MAIL_POPURI_SPRING);
    clearReadLetterBit(MAIL_POPURI_SUMMER);
    clearReadLetterBit(MAIL_POPURI_AUTUMN);
    clearReadLetterBit(MAIL_POPURI_WINTER);
    clearReadLetterBit(MAIL_ELLI_SPRING);
    clearReadLetterBit(MAIL_ELLI_SUMMER);
    clearReadLetterBit(MAIL_ELLI_AUTUMN);
    clearReadLetterBit(MAIL_ELLI_WINTER);
    clearReadLetterBit(MAIL_ANN_SPRING);
    clearReadLetterBit(MAIL_ANN_SUMMER);
    clearReadLetterBit(MAIL_ANN_AUTUMN);
    clearReadLetterBit(MAIL_ANN_WINTER);
    clearReadLetterBit(MAIL_KAREN_SPRING);
    clearReadLetterBit(MAIL_KAREN_SUMMER);
    clearReadLetterBit(MAIL_KAREN_AUTUMN);
    clearReadLetterBit(MAIL_KAREN_WINTER);
    clearReadLetterBit(MAIL_MAYOR_SPRING);
    clearReadLetterBit(MAIL_HARRIS_SPRING);
    clearReadLetterBit(MAIL_GRAY_SPRING);
    clearReadLetterBit(MAIL_JEFF_SPRING);
    clearReadLetterBit(MAIL_KAI_SPRING);
    clearReadLetterBit(MAIL_KENT_STU_SPRING);
    clearReadLetterBit(MAIL_RICK_SPRING);
    clearReadLetterBit(MAIL_BASIL_WINTER);
    clearReadLetterBit(MAIL_PINK_CAT_MINT_SEEDS);
    clearReadLetterBit(MAIL_HORSE_RACE_NOTICE);
    clearReadLetterBit(MAIL_GODDESS_VOTING);
    clearReadLetterBit(MAIL_VEGETABLE_FESTIVAL_NOTICE);
    clearReadLetterBit(MAIL_LIBRARY_CLOSURE_SPRING);
    clearReadLetterBit(MAIL_LIBRARY_CLOSURE_AUTUMN);
    clearReadLetterBit(MAIL_HARVEST_FESTIVAL_NOTICE);
    clearReadLetterBit(MAIL_ORE_MINE_OPENS);
    clearReadLetterBit(MAIL_DOG_RACE_NOTICE);
    clearReadLetterBit(MAIL_BLUE_MIST_SEEDS);

}

//INCLUDE_ASM("asm/nonmatchings/game/time", clearMonthlyLetterBits);

void clearMonthlyLetterBits(void) {
    // unused
    clearReadLetterBit(60);
    clearReadLetterBit(MAIL_MOUNTAIN_CARPENTERS_AD);
}

//INCLUDE_ASM("asm/nonmatchings/game/time", setClockNewDay);

void setClockNewDay(void) {
    
    gHour = 6;
    gMinutes = 0;
    gSeconds = 0;
    
    gDayOfMonth++;
    gDayOfWeek++;
   
    updateClock(FALSE);

}
 
//INCLUDE_ASM("asm/nonmatchings/game/time", checkFestivalDay);

// unused
bool checkFestivalDay(void) {

    bool result = FALSE;
    
    // not sure why && is used... should be ||?
    switch (gSeason) {                             
        case SPRING:
            if (gDayOfMonth == 1 && gDayOfMonth == 8 && gDayOfMonth == 17 && gDayOfMonth == 23) {
                result = TRUE;
            }
            break;
        case SUMMER:
            if (gDayOfMonth == 1 && gDayOfMonth == 9 && gDayOfMonth == 17 && gDayOfMonth == 24) {
                result = TRUE;
            }
            break;
        case AUTUMN:
            if (gDayOfMonth == 4 && gDayOfMonth == 12 && gDayOfMonth == 20 && gDayOfMonth == 28) {
                result = TRUE;
            }
            break;
        case WINTER:
            if (gDayOfMonth == 6 && gDayOfMonth == 19 && gDayOfMonth == 24) {
                result = TRUE;
            }
            break;
        }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/time", setSeasonName);

void setSeasonName(void) {

    setGlobalSeasonName(gSeason);

    gCurrentSeasonName[0] = gGlobalSeasonName[0];
    gCurrentSeasonName[1] = gGlobalSeasonName[1];
    gCurrentSeasonName[2] = gGlobalSeasonName[2];
    gCurrentSeasonName[3] = gGlobalSeasonName[3];
    gCurrentSeasonName[4] = gGlobalSeasonName[4];
    gCurrentSeasonName[5] = gGlobalSeasonName[5];
    
    convertNumberToGameVariableString(21, gDayOfMonth, 1);

}

//INCLUDE_ASM("asm/nonmatchings/game/time", setGlobalSeasonName);

void setGlobalSeasonName(u8 season) {

    switch(season) {        
        
        case SPRING:
            gGlobalSeasonName[0] = char_S; 
            gGlobalSeasonName[1] = char_p; 
            gGlobalSeasonName[2] = char_r; 
            gGlobalSeasonName[3] = char_i; 
            gGlobalSeasonName[4] = char_n; 
            gGlobalSeasonName[5] = char_g; 
            break;
        case SUMMER:
            gGlobalSeasonName[0] = char_S; 
            gGlobalSeasonName[1] = char_u; 
            gGlobalSeasonName[2] = char_m; 
            gGlobalSeasonName[3] = char_m; 
            gGlobalSeasonName[4] = char_e; 
            gGlobalSeasonName[5] = char_r;
            break;
        case AUTUMN:
            gGlobalSeasonName[0] = char_A; 
            gGlobalSeasonName[1] = char_u; 
            gGlobalSeasonName[2] = char_t; 
            gGlobalSeasonName[3] = char_u; 
            gGlobalSeasonName[4] = char_m; 
            gGlobalSeasonName[5] = char_n; 
            break;
        case WINTER:
            gGlobalSeasonName[0] = char_W; 
            gGlobalSeasonName[1] = char_i; 
            gGlobalSeasonName[2] = char_n; 
            gGlobalSeasonName[3] = char_t; 
            gGlobalSeasonName[4] = char_e; 
            gGlobalSeasonName[5] = char_r;
            break;
        default:
            break;

    }
    
}
