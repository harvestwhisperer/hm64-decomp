#include "common.h"

#include "game/animals.h"
#include "game/game.h"
#include "game/gameStatus.h"
#include "game/fieldObjects.h"
#include "game/npc.h"
#include "game/overlayScreens.h"
#include "game/time.h"     
#include "game/weather.h"       

// possible bss
// unused scoring variables
extern u8 gSumGirlsWithHighAffection;
extern u8 gAverageFarmAnimalAffection;

//INCLUDE_ASM("asm/nonmatchings/game/load", func_800598E0);

void func_800598E0(void) {

    resetDailyBits();
    
    setDailyEventBit(DAY_START_6AM_GUARD);
    
    if ((checkLifeEventBit(ACQUIRED_CHICKEN)) && (!checkLifeEventBit(KENT_CHICKEN_VISIT))) {
        setDailyEventBit(KENT_ACQUIRED_CHICKEN_VISIT_DAILY);
    }
    if ((checkLifeEventBit(ACQUIRED_SHEEP)) && (!checkLifeEventBit(KENT_SHEEP_VISIT))) {
        setDailyEventBit(KENT_ACQUIRED_SHEEP_VISIT_DAILY);
    }
    if ((checkLifeEventBit(ACQUIRED_COW)) && (!checkLifeEventBit(KENT_COW_VISIT))) {
        setDailyEventBit(KENT_ACQUIRED_COW_VISIT_DAILY);
    }
    if ((checkLifeEventBit(CHICK_HATCHED)) && (!checkLifeEventBit(KENT_CHICK_VISIT))) {
        setDailyEventBit(KENT_CHICK_HATCHED_VISIT_DAILY);
    }
    if ((checkLifeEventBit(ACQUIRED_HORSE)) && (!checkLifeEventBit(KENT_HORSE_VISIT))) {
        setDailyEventBit(KENT_ACQUIRED_HORSE_VISIT_DAILY);
    }
    if ((checkLifeEventBit(HORSE_GROWN)) && (!checkLifeEventBit(KENT_HORSE_GROWN_VISIT))) {
        setDailyEventBit(KENT_HORSE_GROWN_VISIT_DAILY);
    }
    
    randomizeNPCLocations();
    randomizeMiscAnimalSpawnVariants();
    setDefaultGroundObjectsForLevel();
    
    handleHouseConstruction(gHouseExtensionSelection);
    
    setSpecialDialogues();
    
    gSumGirlsWithHighAffection = getBacholeretteWithHighestAffection(140);
    gTotalGrassTiles = getFarmMoondropFlowerCount() + getGreenhouseMoondropFlowerCount(); // farm field + greenhouse
    gTotalPinkCatMintFlowersGrowing = getFarmPinkCatMintFlowersCount() + getGreenhousePinkCatMintFlowersCount();
     
    if (checkLifeEventBit(MARRIED)) {
        setWifeNameString(gWife);
    }
    
    if (checkLifeEventBit(MRS_MANA_COWS_EVENT)) {

        if (gSeason == WINTER) {

            if (gDayOfMonth == 5) {
                gAverageFarmAnimalAffection = (gFarmAnimals[mrsManaCow1Index].affection + gFarmAnimals[mrsManaCow2Index].affection + gFarmAnimals[mrsManaCow3Index].affection) / 3;
                initializeWatchedCows();
            }

            if (gSeason == WINTER && gDayOfMonth == 6) {
                clearLifeEventBit(MRS_MANA_COWS_EVENT);
            }

        }

    }
    
    setSeasonName();
    decrementFamilyAndPetAffection();
    setFestivalDailyBits();
    
    if (checkDailyEventBit(FORCE_SUNNY_WEATHER_DAILY)) {
        gWeather = SUNNY;
    }
    
    if ((checkLifeEventBit(EARTHQUAKE_HAPPENED)) && (!checkLifeEventBit(EARTHQUAKE_CARPENTER_VISIT))) {
        setLifeEventBit(EARTHQUAKE_CARPENTER_VISIT);
        setDailyEventBit(EARTHQUAKE_CARPENTER_VISIT_DAILY);
    }
    
    if (gSeason == SPRING && gDayOfMonth == 17) {
        setDailyEventBit(HORSE_RACE);
        clearLifeEventBit(HORSE_RACE_WON_THIS_YEAR);
        initializeRaceContext();
    }
    
    if (gSeason == AUTUMN && gDayOfMonth == 28) {
        setDailyEventBit(HORSE_RACE);
        clearLifeEventBit(HORSE_RACE_WON_THIS_YEAR);
        initializeRaceContext();
    }
    
    if (gSeason == WINTER && gDayOfMonth == 19) {
        setDailyEventBit(DOG_RACE);
        initializeRaceContext();            
    }

    if (gSeason == AUTUMN) {

        if (gDayOfMonth == 3) {
            clearLifeEventBit(COW_FESTIVAL_WON_THIS_YEAR);
            // animal index
            gCowFestivalEnteredCowIndex = 0xFF;
        }

        // redundant code: macro or static inline?
        if (gSeason == AUTUMN && gDayOfMonth == 5) {  
            // animal index     
            gCowFestivalEnteredCowIndex = 0xFF;
        }

    }
    
    if (gSeason == SUMMER && gDayOfMonth == 9) {
        clearLifeEventBit(WON_VEGETABLE_FESTIVAL);
    }

    if (gYear == 1) {
        if (gSeason == AUTUMN && gDayOfMonth == 23) {
            mountainConstructionWorkDays = 1;
        }
    }
    
    if (gYear == 1) {
        if (gSeason == WINTER && gDayOfMonth == 12) {
            mountainConstructionWorkDays = 1;
        }
    }

}