#include "common.h"
#include "animals.h"

                        
void func_8005A60C();                                  
void func_8006096C();                                  
                   
u8 handleHouseConstruction(u8);                                
void func_80075A18();                                  
void func_800886D0();                   
void func_8009BC64();                                  
void func_800CC17C();                                  
void setSeasonName();                                  
void func_800D9BFC();                                  
u32 func_800DC008();                                
u32 func_800DC080();                                
u32 func_800DC0F8();                                
u32 func_800DC170();                

u8 getSumBacholerettesWithAffectionThreshold(u8 affectionLevel);

u32 checkDailyEventBit(u16 bitIndex);                          
u32 checkLifeEventBit(u16 bitIndex);                       
void setDailyEventBit(u16 bitIndex);                              
void setLifeEventBit(u16 bitIndex);                               
void toggleLifeEventBit(u16 bitIndex);      
void resetDailyBits();    

void setSpecialDialogues();           
void setWifeNameString(u8 wife);          

extern u8 D_8016F898;
extern u8 D_8016FAF8;
extern u8 D_80189054;
extern u8 D_801C3F46;

// unused scoring variables
extern u16 gTotalGrassTiles;
extern u16 gTotalPinkCatMintFlowersGrowing;
extern u8 gSumGirlsWithHighAffection;
extern u8 gAverageFarmAnimalAffection;

extern u8 mountainConstructionWorkDays;
extern u8 gHouseExtensionSelection;

extern u8 gDayOfMonth;
extern u8 gSeason;
extern u8 gWeather;
extern u8 gWife;
extern u8 gYear;

extern FarmAnimal gFarmAnimals[];

//INCLUDE_ASM(const s32, "load", func_800598E0);

void func_800598E0(void) {

    u32 tempSeason;
    u32 temp2;

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
    // sets member on unknown struct to random value
    func_8009BC64();
    // sets foragable item defaults
    func_800D9BFC();
    
    handleHouseConstruction(gHouseExtensionSelection);
    
    setSpecialDialogues();
    
    gSumGirlsWithHighAffection = getSumBacholerettesWithAffectionThreshold(140);
    gTotalGrassTiles = func_800DC008() + func_800DC080(); // farm field + greenhouse
    gTotalPinkCatMintFlowersGrowing = func_800DC0F8() + func_800DC170(); // farm field + greenhouse
    
    if (checkLifeEventBit(MARRIED)) {
        setWifeNameString(gWife);
    }
    
    if (checkLifeEventBit(0x94)) {
        
        tempSeason = gSeason;

        if (tempSeason == WINTER) {

            if (gDayOfMonth == 5) {
                gAverageFarmAnimalAffection = (gFarmAnimals[D_8016FAF8].affection + gFarmAnimals[D_801C3F46].affection + gFarmAnimals[D_8016F898].affection) / 3;
                // reset farm animal structs; why on winter 5?
                func_800886D0();
            }

            if (tempSeason == gSeason && gDayOfMonth == 6) {
                toggleLifeEventBit(0x94);
            }
        }
    }
    
    // set season name string
    setSeasonName();
    // decrease wife/baby/horse/dog affection
    func_8005A60C();
    // set festival daily event bits 
    func_8006096C();
    
    if (checkDailyEventBit(0x4E)) {
        gWeather = SUNNY;
    }
    
    if ((checkLifeEventBit(0x4E)) && (!checkLifeEventBit(0x4F))) {
        setLifeEventBit(0x4F);
        setDailyEventBit(0x3F);
    }
    
    if (gSeason == SPRING) {
        if (gDayOfMonth == 0x11) {
            setDailyEventBit(0x41);
            toggleLifeEventBit(0x90);
            func_800CC17C();
        }
    }
    
    if ((gSeason == FALL) && (gDayOfMonth == 28)) {
        setDailyEventBit(0x41);
        toggleLifeEventBit(0x90);
        func_800CC17C();
    }
    
    if (gSeason == WINTER && gDayOfMonth == 19) {
        setDailyEventBit(0x42);
        func_800CC17C();            
    }

    if (gSeason == FALL) {
        if (gDayOfMonth == 3) {
            toggleLifeEventBit(0x91);
            // some kind of index
            D_80189054 = 0xFF;
        }
        if (gSeason == FALL) {
            if (gDayOfMonth == 5) {       
                D_80189054 = 0xFF;
            }
        }
    }
    
    if (gSeason == SUMMER && gDayOfMonth == 9) {
        toggleLifeEventBit(0x92);
    }

    temp2 = gSeason;
    
    if (gYear == 1) {
        if (temp2 == FALL && gDayOfMonth == 23) {
          mountainConstructionWorkDays = 1;
        }
    }
    
    if (gYear == 1) {
        if (gSeason == WINTER && gDayOfMonth == 12) {
            mountainConstructionWorkDays = 1;
        }
    }
}


