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
void toggleMonthlyLetterBits(void);
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

void handleTimeUpdates(void) {
    
    if ((getLevelFlags(gBaseMapIndex) & LEVEL_TIME_FLOWS) && !checkDailyEventBit(SUSPEND_TIME_DURING_ANIMATION)) {
        updateClock(TRUE);
    }
    
    if (gHour == 0 && gMinutes == 0 && gSeconds == 0) {
        setSeasonName();
    }
    
    if (gHour == 6) {
        
        if (!checkDailyEventBit(0xE)) {

            setPlayerAction(254, ANIM_DEFAULT);
            handleExitLevel(1, END_OF_DAY_2);
            
        }

    } else {
        
        toggleDailyEventBit(0xE);

        if (gCutsceneCompletionFlags >= 0) {
            setLevelLighting(1, NO_OP);
        }

    }
    
    if (NIGHTTIME) {
        
        if (!checkDailyEventBit(0xF)) {

            stopAudioSequenceWithDefaultFadeOut(gCurrentAudioSequenceIndex);
            setDailyEventBit(0xF);

        } else if (!checkDailyEventBit(0x10) && gCurrentAudioSequenceIndex != 0xFF && checkDefaultSequenceChannelOpen(gCurrentAudioSequenceIndex)) {

            setLevelAudio(gBaseMapIndex, gSeason, gHour);
            setAudioSequenceVolume(gCurrentAudioSequenceIndex, gAudioSequenceVolume);
 
            setDailyEventBit(0x10);

        }
        
    }
    
    if (gHour < 6 && gWeather != RAIN && gBaseMapIndex != BEACH && !checkDailyEventBit(0x11)) {
        stopAudioSequenceWithDefaultFadeOut(gCurrentAudioSequenceIndex);
        setDailyEventBit(0x11);
    }
    
    handleTimedDailyCutscenes(gBaseMapIndex);
    
}

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
        toggleMonthlyLetterBits();
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

void setupNewYear(void) {

    gHarvestKing = gHarvestCoinFinder;  
    gFlowerFestivalGoddess = 0xFF;
    gHarvestCoinFinder = 0xFF;
    gVoteForFlowerFestivalGoddess = 0xFF;
    
    toggleLifeEventBit(0x44);

    toggleReadLetterBit(0);
    toggleReadLetterBit(1);
    toggleReadLetterBit(2);
    toggleReadLetterBit(3);
    toggleReadLetterBit(4);
    toggleReadLetterBit(5);
    toggleReadLetterBit(6);
    toggleReadLetterBit(7);
    toggleReadLetterBit(8);
    toggleReadLetterBit(9);
    toggleReadLetterBit(0xA);
    toggleReadLetterBit(0xB);
    toggleReadLetterBit(0xC);
    toggleReadLetterBit(0xD);
    toggleReadLetterBit(0xE);
    toggleReadLetterBit(0xF);
    toggleReadLetterBit(0x10);
    toggleReadLetterBit(0x11);
    toggleReadLetterBit(0x12);
    toggleReadLetterBit(0x13);
    toggleReadLetterBit(0x14);
    toggleReadLetterBit(0x15);
    toggleReadLetterBit(0x17);
    toggleReadLetterBit(0x19);
    toggleReadLetterBit(0x1D);
    toggleReadLetterBit(0x20);
    toggleReadLetterBit(0x21);
    toggleReadLetterBit(0x22);
    toggleReadLetterBit(0x3B);
    toggleReadLetterBit(0x3D);
    toggleReadLetterBit(0x3E);
    toggleReadLetterBit(0x3F);
    toggleReadLetterBit(0x41);
    toggleReadLetterBit(0x42);
    toggleReadLetterBit(0x43);
    toggleReadLetterBit(0x45);
    toggleReadLetterBit(0x46);
    toggleReadLetterBit(0x49);

}

void toggleMonthlyLetterBits(void) {
    toggleReadLetterBit(0x3C);
    toggleReadLetterBit(0x40);
}

void setClockNewDay(void) {
    
    gHour = 6;
    gMinutes = 0;
    gSeconds = 0;
    
    gDayOfMonth++;
    gDayOfWeek++;
   
    updateClock(FALSE);

}
 
// unused
// bool checkFestivalDay(void) {

//     bool result = FALSE;
    
//     // not sure why && is used... should be ||?
//     switch (gSeason) {                             
//         case SPRING:
//             if (gDayOfMonth == 1 && gDayOfMonth == 8 && gDayOfMonth == 17 && gDayOfMonth == 23) {
//                 result = TRUE;
//             }
//             break;
//         case SUMMER:
//             if (gDayOfMonth == 1 && gDayOfMonth == 9 && gDayOfMonth == 17 && gDayOfMonth == 24) {
//                 result = TRUE;
//             }
//             break;
//         case AUTUMN:
//             if (gDayOfMonth == 4 && gDayOfMonth == 12 && gDayOfMonth == 20 && gDayOfMonth == 28) {
//                 result = TRUE;
//             }
//             break;
//         case WINTER:
//             if (gDayOfMonth == 6 && gDayOfMonth == 19 && gDayOfMonth == 24) {
//                 result = TRUE;
//             }
//             break;
//         }

//     return result;

// }

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
