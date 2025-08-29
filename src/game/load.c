#include "common.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/npc.h"
#include "game/mapObjects.h"
#include "game/overlayScreens.h"
#include "game/updateGame.h"     
#include "game/weather.h"       

// bss
// unused scoring variables
extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;
extern u8 gSumGirlsWithHighAffection;
extern u8 gAverageFarmAnimalAffection;

// shared
extern u8 gHouseExtensionSelection;
extern u8 mountainConstructionWorkDays;

//INCLUDE_ASM("asm/nonmatchings/game/load", func_800598E0);

void func_800598E0(void) {

    // FIXME: shouldn't be necessary
    u32 tempSeason;

    resetDailyBits();
    
    setDailyEventBit(0xE);
    
    if ((checkLifeEventBit(0x32)) && (!checkLifeEventBit(0x33))) {
        setDailyEventBit(0x22);
    }
    if ((checkLifeEventBit(0x34)) && (!checkLifeEventBit(0x35))) {
        setDailyEventBit(0x23);
    }
    if ((checkLifeEventBit(0x36)) && (!checkLifeEventBit(0x37))) {
        setDailyEventBit(0x24);
    }
    if ((checkLifeEventBit(0x38)) && (!checkLifeEventBit(0x39))) {
        setDailyEventBit(0x25);
    }
    if ((checkLifeEventBit(0x3A)) && (!checkLifeEventBit(0x3B))) {
        setDailyEventBit(0x26);
    }
    if ((checkLifeEventBit(0x3C)) && (!checkLifeEventBit(0x3D))) {
        setDailyEventBit(0x27);
    }
    
    // set npc location indices
    func_80075A18();
    // animals
    func_8009BC64();
    // sets foragable item defaults
    func_800D9BFC();
    
    handleHouseConstruction(gHouseExtensionSelection);
    
    setSpecialDialogues();
    
    gSumGirlsWithHighAffection = getBacholeretteWithHighestAffection(140);
    gTotalGrassTiles = func_800DC008() + func_800DC080(); // farm field + greenhouse
    gTotalPinkCatMintFlowersGrowing = countPinkCatMintFlowersFarm() + countPinkCatMintFlowersGreenhouse(); // farm field + greenhouse
    
    if (checkLifeEventBit(MARRIED)) {
        setWifeNameString(gWife);
    }
    
    if (checkLifeEventBit(0x94)) {

        if (gSeason == WINTER) {

            if (gDayOfMonth == 5) {
                gAverageFarmAnimalAffection = (gFarmAnimals[mrsManaCow1Index].affection + gFarmAnimals[mrsManaCow2Index].affection + gFarmAnimals[mrsManaCow3Index].affection) / 3;
                func_800886D0();
            }

            if (gSeason == WINTER && gDayOfMonth == 6) {
                toggleLifeEventBit(0x94);
            }
        }
    }
    
    // set season name string
    setSeasonName();
    // decrease wife/baby/horse/dog affection
    func_8005A60C();
    // set festival daily event bits 
    setFestivalDailyBits();
    
    if (checkDailyEventBit(0x4E)) {
        gWeather = SUNNY;
    }
    
    if ((checkLifeEventBit(0x4E)) && (!checkLifeEventBit(0x4F))) {
        setLifeEventBit(0x4F);
        setDailyEventBit(0x3F);
    }
    
    if (gSeason == SPRING) {
        if (gDayOfMonth == 17) {
            setDailyEventBit(0x41);
            toggleLifeEventBit(0x90);
            func_800CC17C();
        }
    }
    
    if (gSeason == AUTUMN && gDayOfMonth == 28) {
        setDailyEventBit(0x41);
        toggleLifeEventBit(0x90);
        func_800CC17C();
    }
    
    if (gSeason == WINTER && gDayOfMonth == 19) {
        setDailyEventBit(0x42);
        func_800CC17C();            
    }

    if (gSeason == AUTUMN) {
        if (gDayOfMonth == 3) {
            toggleLifeEventBit(0x91);
            // animal index
            D_80189054 = 0xFF;
        }
        if (gSeason == AUTUMN) {
            if (gDayOfMonth == 5) {  
                // animal index     
                D_80189054 = 0xFF;
            }
        }
    }
    
    if (gSeason == SUMMER && gDayOfMonth == 9) {
        toggleLifeEventBit(0x92);
    }

    // FIXME: possible fake match
    tempSeason = gSeason;
    
    if (gYear == 1) {
        if (tempSeason == AUTUMN && gDayOfMonth == 23) {
          mountainConstructionWorkDays = 1;
        }
    }
    
    if (gYear == 1) {
        if (gSeason == WINTER && gDayOfMonth == 12) {
            mountainConstructionWorkDays = 1;
        }
    }
}


