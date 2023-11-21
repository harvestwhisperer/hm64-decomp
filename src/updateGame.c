#include "common.h"

#include "updateGame.h"

#include "system/message.h"

#include "game.h"
#include "gameStatus.h"
#include "gameAudio.h"
#include "level.h"
#include "player.h"
#include "setCutscenes.h"
#include "weather.h"
                    
// forward declarations           
void setGlobalSeasonName(u8);

// likely bss
extern u8 gSeconds;
extern u8 gHour;
extern u8 gMinutes;
extern u8 gDayOfWeek;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gNextSeason;
extern u8 gYear;

extern u8 gCurrentSeasonName[6];
extern u8 gGlobalSeasonName[6];

//INCLUDE_ASM(const s32, "updateGame", func_800D7C20);

void func_800D7C20(void) {
    
    if ((getLevelFlags(gBaseMapIndex) & 1) && !checkDailyEventBit(6)) {
        updateClock(1);
    }
    
    if (gHour == 0 && gMinutes == 0 && gSeconds == 0) {
        setSeasonName();
    }
    
    if (gHour == 6) {
        
        if (!checkDailyEventBit(0xE)) {

            setPlayerAction(0xFE, 0);
            // stop audio and set callback for loading
            func_8005C940(1, 0xE);
            
        }

    } else {
        
        toggleDailyEventBit(0xE);

        if (D_801891D4 >= 0) {
            // do global lighting
            func_8005C07C(1, 0);
        }
    }
    
    if (NIGHTTIME) {
        
        if (!checkDailyEventBit(0xF)) {

            setSongWithDefaultSpeed(gCurrentSongIndex);
            setDailyEventBit(0xF);

        } else if (!(checkDailyEventBit(0x10)) && gCurrentSongIndex != 0xFF && checkDefaultSongChannelOpen(gCurrentSongIndex)) {

            setLevelAudio(gBaseMapIndex, gSeason, gHour);
            setSongWithVolume(gCurrentSongIndex, gSongVolume);
 
            setDailyEventBit(0x10);

        }
        
    }
    
    if (gHour < 6 && gWeather != RAIN && gBaseMapIndex != BEACH && !checkDailyEventBit(0x11)) {
        setSongWithDefaultSpeed(gCurrentSongIndex);
        setDailyEventBit(0x11);
    }
    
    // check/load cutscenes
    func_800A7AE8(gBaseMapIndex);
}

//INCLUDE_ASM(const s32, "updateGame", updateClock);

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
    
    gNextSeason = gSeason;
    
    if ((gDayOfMonth + 1) >= 31) {
        gNextSeason = gSeason + 1;
    }
    
    if (gNextSeason >= 5) {
        gNextSeason = SPRING;
    }
}

//INCLUDE_ASM(const s32, "updateGame", setupNewYear);

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

//INCLUDE_ASM(const s32, "updateGame", toggleMonthlyLetterBits);

void toggleMonthlyLetterBits(void) {
    toggleReadLetterBit(0x3C);
    toggleReadLetterBit(0x40);
}

//INCLUDE_ASM(const s32, "updateGame", setClockNewDay);

void setClockNewDay(void) {
    
    gHour = 6;
    gMinutes = 0;
    gSeconds = 0;
    
    gDayOfMonth++;
    gDayOfWeek++;
   
    updateClock(FALSE);

}
 
//INCLUDE_ASM(const s32, "updateGame", checkFestivalDay);

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
            if (gDayOfMonth == 4 && gDayOfMonth == 0xC && gDayOfMonth == 20 && gDayOfMonth == 28) {
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

//INCLUDE_ASM(const s32, "updateGame", setSeasonName);

void setSeasonName(void) {

    setGlobalSeasonName(gSeason);

    gCurrentSeasonName[0] = gGlobalSeasonName[0];
    gCurrentSeasonName[1] = gGlobalSeasonName[1];
    gCurrentSeasonName[2] = gGlobalSeasonName[2];
    gCurrentSeasonName[3] = gGlobalSeasonName[3];
    gCurrentSeasonName[4] = gGlobalSeasonName[4];
    gCurrentSeasonName[5] = gGlobalSeasonName[5];
    
    // message.c
    func_8003FBD8(0x15, gDayOfMonth, 1);
}

//INCLUDE_ASM(const s32, "updateGame", setGlobalSeasonName);

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
