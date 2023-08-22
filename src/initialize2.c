#include "common.h"

#include "npc.h"
#include "sprite.h"

u16 getRandomNumberInRange(u16 min, u16 max);    

void setDailyEventBit(u16 bitIndex);                                  
u32 checkDailyEventBit(u16 bitIndex);   
void toggleDailyEventBit(u16);
void setLifeEventBit(u16 bitIndex);
u32 checkLifeEventBit(u16 bitIndex);
void toggleLifeEventBit(u16);      
void toggleSpecialDialogueBit(int);       

void setFlowerFestivalGoddess();
void setLetters();  
void setForecast();     

void setExit(u16);                                 
void setMainLoopCallbackFunctionIndex(u16);         

void setPlayerAction(u8, u8);                              
void setLevelGraphicsData(u8);         

void func_8002B710();                                  
void func_8002E1B8();            
void func_8002F6F0();                             
void func_8002F730();           
void func_8002F7C8(u8, u32, u32, u8);         
void func_8002FB7C();                                  
void func_8002FB3C();                                  
void func_8002FCB4(int, int);                               
u8 func_8002FD24(int);                            
u8 func_80030388(int);                   
void func_8003BE98(u16, u32, u32, u32, u8);                   
void func_8003C504(u32);                                 
void func_8003D970();                                  
void func_80046860();     
void func_80046C98();     
u32 func_8004D380(u8, u32);                                 
void func_80053088();                                  
void func_800563D0(s32);          
void func_800598E0();                                   
void func_80059D90();                                                                                                  
void func_80063AF0();                                   
void func_80064114();                                   
void func_800644B0();                                   
void func_80064814();    
void func_8003BC50(u8, u8);                             
void func_80046CF4();                                   
void func_8005AAE4();                                  
void func_8005CDCC();      
void func_800657D0(u8, u8);                                                                   
void func_80065AA0();                               
void removeKeyItem(u32);    
void func_80065F5C();   
void func_8006623C();     
void func_8006A2E8();                                  
void func_8006E840(u8);  
s32 func_80074C38(u8);                              
void func_800752C0();                                  
void func_800758B8();                                  
void func_80075910();                                  
void func_80085F70();                                  
void func_800876D0();                                                                   
void setNpcAnimations();  
void func_800879C8();                                     
void func_80087CD4();     
void func_80087D5C();                                   
void func_80087DEC();                                   
void func_8008841C();    
void func_80088D54();                                  
s32 func_8009B5E0();     
u8 func_8009B374();                                 
u8 func_8009B564();                             
void func_800A8F74();   
void func_800A7DFC();                                  
void loadCutscene(u32);   
void setAudio(u16);    
void func_800D0318();      
void func_800D51B0();                                  
void func_800D5290();  
void func_800D7010();                                     
void func_800D7C20();                                                                  
void func_800D86D8();                                   
void func_800DB424();                                   
void func_800DB858();                                   
void func_800DBAC4();                                     
void func_800E53E8(u8);    

extern u16 gCutsceneIndex;

extern u8 gHour;
extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gWeather;
extern u8 gForecast;

extern u8 gCurrentGameIndex;

extern u8 houseExtensionConstructionCounter;
extern u8 gElliGrievingCounter;
extern u8 gHarvestCoinFinder;

extern u8 gBaseMapIndex;
extern Vec4f globalLightingRgba;

extern u8 npcAffection[TOTAL_NPCS];

extern u32 D_8016FE00;

extern s32 D_80180718;
extern s32 D_8018071C;
extern s32 D_80180720;
extern s32 D_80180724;
extern s16 D_80182D8C;

extern u8 D_8018908C;
extern s32 D_801891D4;
extern s16 D_8018A062;
extern u8 D_80205635;
extern u8 D_802231C0;
extern s16 D_80237A20;

// calendar
extern u8 D_80189108[5][7];

extern RenderedSprite renderedSprites[];


//INCLUDE_ASM(const s32, "initialize2", mainGameLoopCallback);

void mainGameLoopCallback(void) {
    
    // for debugging?
    D_8018A062 = (s16)renderedSprites[PLAYER].startingCoordinates.x;
    D_80237A20 = (s16)renderedSprites[PLAYER].startingCoordinates.y;
    D_80182D8C = (s16)renderedSprites[PLAYER].startingCoordinates.z;
    
    // check sprite flags
    if (func_8002FD24(PLAYER) && !(func_80030388(PLAYER))) {

        // increment clock and handle audio changes
        func_800D7C20();
        // handle player animation
        func_8006623C();
        setNpcAnimations();
        func_80087CD4();
        // player animation
        func_8006A2E8();

    }

    // set additional info if needed for cutscene
    func_800A8F74();
    // check c right pressed during frame
    func_8004D380(0, 1);
    // check c down pressed during frame
    func_8004D380(0, 4);
    // check z pressed during frame
    func_8004D380(0, 0x2000);
}

//INCLUDE_ASM(const s32, "initialize2", func_80055F08);

void func_80055F08(s16 cutsceneIndex, u16 exitIndex, u8 arg2) {
    
    func_8002E1B8();
    func_8002B710();
    func_8003D970();
    func_80046860();
    func_80053088();
    
    // reset global lighting rgba vec4f
    globalLightingRgba.r = 0;
    globalLightingRgba.g = 0;
    globalLightingRgba.b = 0;
    globalLightingRgba.a = 0;

    // rgba vec4f
    // dialogue icon struct
    D_80180718 = 0;
    D_8018071C = 0;
    D_80180720 = 0;
    D_80180724 = 0;

    func_8002F7C8(0, 0, 0, 0);
    func_8003BE98(0, 0, 0, 0, 0);

    gHour = 12;

    // set ptrs to rom addresses for sprites
    func_800563D0(arg2);

    setExit(exitIndex);

    gCutsceneIndex = cutsceneIndex;

    // trigger cutscene and load cutscene assets
    loadCutscene(0);
    
    func_8006E840(D_802231C0);
    func_800657D0(D_802231C0, 0);

    func_8006A2E8();

    setMainLoopCallbackFunctionIndex(4);

}

#ifdef PERMUTER
void func_80056030(u8 arg0) {
    
    u16 var_a0;
    u8 temp_s0;

    D_801891D4 = 0;
    cutsceneIndex = 0;
    D_8016FE00 = 0;
    
    toggleDailyEventBit(0x28);
    toggleDailyEventBit(0x4B);
    
    func_8005CDCC();
    func_80065F5C();
    func_800879C8();
    func_80075910();
    func_8002E1B8();
    func_8002B710();
    func_80046860();
    func_800752C0();
    func_80085F70();
    func_800D51B0();
    
    if (checkDailyEventBit(0x2F) == 0) {
        func_800A7DFC();
    }
    
    setDailyEventBit(0x2F);
    
    temp_s0 = func_80074C38(D_802231C0);
    
    func_8006E840(D_802231C0);
    
    var_a0 = 0;

    if ((temp_s0 == 0x52) || (temp_s0 == 0x58) || (temp_s0 == 0x59)) goto block_end;

    
    if (cutsceneIndex == 0x41A) goto block_32;

    if (cutsceneIndex < 0x41B) {
        
        if (cutsceneIndex < 0x386) {
        
            if (cutsceneIndex < 0x384 && cutsceneIndex != 0x258) {
                if ((cutsceneIndex > 0x257) && (cutsceneIndex < 0x355) && (cutsceneIndex > 0x351)) {
                    var_a0 = 2;
                    goto block_end;    
                }
            }
        
            goto block_34;
        }
        
        if (cutsceneIndex != 0x3B6) {
            if (cutsceneIndex == 0x3E9) {
                
                goto block_end;    
            }
        }

block_32:
        var_a0 = 2;
        goto block_end;
    }

    if (cutsceneIndex < 0x516) {
        if (cutsceneIndex > 0x513 || cutsceneIndex == 0x4b0) {
            
            goto block_end;
        }
        if (cutsceneIndex < 0x4B1) {
            if (cutsceneIndex == 0x44C) {
                var_a0 = 2;
                goto block_end;
            }
        }
        
        goto block_end;
    }

    if (cutsceneIndex >= 0x546) {
        var_a0 = 2;
        if (cutsceneIndex >= 0x549) {
            if (cutsceneIndex == 0x578) {
                var_a0 = 2;
                goto block_end;
            }
            goto block_34;
        }
    }

block_34:
    var_a0 = 1;

block_end:
    
    func_800563D0(var_a0);
    
    if (checkDailyEventBit(0x54) != 0) {
        if (checkDailyEventBit(0x53) != 0) {
            setPlayerAction(6, 8);
            D_8018908C = 0x50;
        } else {
            setPlayerAction(0, 0);
            D_8018908C = 0;
        }
        if (!(D_8016FE00 & 1) && (func_8009B5E0() & 0xFF)) {
            setAudio(0x3F);
        }
    }

    toggleDailyEventBit(0x53);
    toggleDailyEventBit(0x54);
    
    func_800657D0(D_802231C0, arg0);
    func_80065AA0();
    func_800D5290();
    
    if (arg0 != 2) {
        if (!(D_8016FE00 & 6)) {
            if (checkDailyEventBit(0x4D) == 0) {
                func_80088D54();
            }
            if (!(D_8016FE00 & 6)) {
                func_8005AAE4();
                func_800758B8();
            }
        }
        if (!(D_8016FE00 & 4)) {
            func_800876D0();
        }
    }
    
    func_8006A2E8();
    func_800D7010();
    func_800D0318();
    func_8002F7C8(0, 0, 0, 0);
    func_8003BE98(0, 0, 0, 0, 0);
    
    D_80180718 = 0;
    D_8018071C = 0;
    D_80180720 = 0;
    D_80180724 = 0;
}
#else
INCLUDE_ASM(const s32, "initialize2", func_80056030);
#endif

INCLUDE_ASM(const s32, "initialize2", func_800563D0);

//INCLUDE_ASM(const s32, "initialize2", func_80059300);

void func_80059300(void) {
    func_8002F730();
    func_80046CF4();
    func_8002FCB4(0, 1);
}

//INCLUDE_ASM(const s32, "initialize2", func_80059334);

void func_80059334(void) {
    func_8002F6F0();
    func_80046C98();
    func_8002FCB4(0, 0);
}

//INCLUDE_ASM(const s32, "initialize2", func_80059368);

void func_80059368(void) {
    func_8002FB7C();
    func_80046CF4();
    func_8002FCB4(0, 1);
    func_80065AA0();
    func_8003BC50(0, D_80205635);
    setLevelGraphicsData(gBaseMapIndex);
    setPlayerAction(0xFE, 0);
    func_8006623C();
    setMainLoopCallbackFunctionIndex(1);
    setPlayerAction(0, 0);
}

//INCLUDE_ASM(const s32, "initialize2", func_800593EC);

void func_800593EC(void) {
    func_8002FB3C();
    func_80046C98();
    func_8002FCB4(0, 0);
    func_8003C504(0);
}

//INCLUDE_ASM(const s32, "initialize2", startNewDay);

void startNewDay(void) {
    
    u8 i, j;
    
    if (checkLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION)) {
        houseExtensionConstructionCounter++;
    }
    
    if (checkDailyEventBit(CARPENTER_ESTIMATE)) {
        setLifeEventBit(HOUSE_EXTENSION_CONSTRUCTION);
    }
     
    if (gSeason == FALL && gDayOfMonth == 28) {
        setLifeEventBit(0x48);
    }
    
    if (gSeason == WINTER && gDayOfMonth == 17) {
        setLifeEventBit(0x4C);
        setDailyEventBit(0x38);
    }
    
    if (gDayOfMonth == 1) {
        
        // calendar?
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 7; j++) {
                D_80189108[i][j] = 0xFF;
            }
        }
    }
    
    if (gSeason == SPRING && gDayOfMonth == 23) {
        setFlowerFestivalGoddess();
    }

    if (gSeason == FALL) {

        if (gDayOfMonth == 12) {
            gHarvestCoinFinder = getRandomNumberInRange(0, 5);
            if (checkLifeEventBit(0x2D) && gHarvestCoinFinder == 4) {
                gHarvestCoinFinder = 1;
            }
            if (checkLifeEventBit(0x2E) && gHarvestCoinFinder == 5) {
                gHarvestCoinFinder = 2;
            }
        }

        if (gSeason == FALL && gDayOfMonth == 13 && !checkDailyEventBit(0x5D) && gHarvestCoinFinder == PLAYER) {
            gHarvestCoinFinder = 3;
        }
    }
    
    if (checkLifeEventBit(ELLEN_DIED)) {
        if (gElliGrievingCounter >= 31) {
            toggleSpecialDialogueBit(ELLI_FINISH_GRIEVING);
        } else {
            gElliGrievingCounter++;
        }
    }
    
    if (gWeather == TYPHOON) {
        
        // get average of sprite affection
        if (((npcAffection[HARVEST_SPRITE_1] + npcAffection[HARVEST_SPRITE_2] + npcAffection[HARVEST_SPRITE_3]) / 3) >= 120) {
            
            // check animal score
            if ((func_8009B564() + func_8009B374()) >= 2) {
                setLifeEventBit(0x4D);
            }
        }
        
        if (gWeather == TYPHOON && checkLifeEventBit(0x4D)) {
            func_80087D5C();
        }
    }
    
    if (checkLifeEventBit(0x51) && checkDailyEventBit(0x41)) {
        toggleLifeEventBit(0x51);
    }
    
    if (checkLifeEventBit(0x52) && checkDailyEventBit(0x42)) {
        toggleLifeEventBit(0x52);
    }
    
    if (checkLifeEventBit(0x93)) {
        toggleLifeEventBit(0x93);
        setLifeEventBit(0x94);
        func_8008841C();
    }
    
    removeKeyItem(0xE);

    // equalize harvest sprite affection
    npcAffection[HARVEST_SPRITE_2] = npcAffection[HARVEST_SPRITE_1];
    npcAffection[HARVEST_SPRITE_3] = npcAffection[HARVEST_SPRITE_1];
    
    if (gWeather == RAIN) {
        // update animal status based on rain
        func_80064114();
    }
    
    if (gWeather == SNOW) {
        // update animal status based on snow
        func_800644B0();
    }
    
    if (gWeather == TYPHOON) {
        // update animal status based on typhoon
        func_80064814();
    }
    
    gWeather = gForecast;
    
    // set indices for weather channel graphics
    func_800D86D8();
    setForecast();
    
    if (checkLifeEventBit(0x7E)) {
        checkLifeEventBit(0x7F);
    }
    
    func_800DB424();
    func_80087DEC();
    func_800DB858();
    func_800DBAC4();
    func_80059D90();
    setLetters();
    
    if (checkLifeEventBit(MARRIED)) {
        func_80063AF0();
    }
    
    if (checkDailyEventBit(7)) {
        func_800E53E8(gCurrentGameIndex);
    }
    
    func_800598E0();
}
