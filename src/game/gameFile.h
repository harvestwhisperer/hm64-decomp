#ifndef _LOAD_GAME_SCREEN_H_
#define _LOAD_GAME_SCREEN_H_

#include "common.h"

#define FARM_RANKING_SLOTS 5
#define FARM_RANKING_SLOTS_WITH_TEMP 7
#define FARM_RANKING_NAME_LENGTH 6
#define RANKING_TEMP_SLOT 5
// Swap slot index for sorting operations
#define RANKING_SWAP_SLOT 6

#define RANKING_FLAG_ACTIVE   1  
#define RANKING_FLAG_MARRIED  2  
#define RANKING_FLAG_HAVE_BABY 4  
#define RANKING_FLAG_HAVE_HORSE 8

// D_801D6238
typedef struct {
    u8 unk_0[15];
    u8 numGamePaks; // 0xF, D_801D624C
    u32 unk_10;
    u8 unk_14;
    u8 unk_15[20]; //  D_801D624D
    u8 unk_29; // D_801D6261
    u8 unk_2A[6]; // D_801D6262
    u8 unk_30;
    u32 unk_34;
    u16 unk_38;
    u16 unk_3A;
    u8 unk_3C;
    u8 gamePakEnabled; // 0x3D, D_801D6275
    u8 unk_3E;
    u8 unk_3F;
    u8 playerNames[4][6]; // D_801D6278
    u8 dateNumbers[4][2]; // D_801D6290
    u8 seasonNames[4][6]; // D_801D6298
    u8 dateEndings[4][2];  // "st" or "rd" strings /* 0x78 */ D_801D62B0
    u8 flags[4]; // D_801D62B8
    u8 diaryHighlighted; /* 0x84 */  // D_801D62BC
    u8 actionColumnHighlighted; /* 0x85 */ // D_801D62BD
    u8 unk_86;
    u8 showControllerPakScreen; /* 0x87 */
    u8 unk_88;
    u8 action; // move/delete /* 0x89 */
} LoadGameScreenContext;


// 0x801FB708
typedef struct {
    u8 farmNames[FARM_RANKING_SLOTS][FARM_RANKING_NAME_LENGTH];   
    u8 tempFarmName[FARM_RANKING_NAME_LENGTH];                    
    u8 swapFarmName[FARM_RANKING_NAME_LENGTH];                    
    u8 playerNames[FARM_RANKING_SLOTS][FARM_RANKING_NAME_LENGTH];        
    u8 tempPlayerName[FARM_RANKING_NAME_LENGTH];                         
    u8 swapPlayerName[FARM_RANKING_NAME_LENGTH];                         
    u8 wifeNames[FARM_RANKING_SLOTS][FARM_RANKING_NAME_LENGTH];          
    u8 tempWifeName[FARM_RANKING_NAME_LENGTH];                           
    u8 swapWifeName[FARM_RANKING_NAME_LENGTH];                           
    u8 babyNames[FARM_RANKING_SLOTS][FARM_RANKING_NAME_LENGTH];          
    u8 tempBabyName[FARM_RANKING_NAME_LENGTH];                           
    u8 swapBabyName[FARM_RANKING_NAME_LENGTH];                           
    u8 dogNames[FARM_RANKING_SLOTS][FARM_RANKING_NAME_LENGTH];           
    u8 tempDogName[FARM_RANKING_NAME_LENGTH];                            
    u8 swapDogName[FARM_RANKING_NAME_LENGTH];                            
    u8 horseNames[FARM_RANKING_SLOTS][FARM_RANKING_NAME_LENGTH];         
    u8 tempHorseName[FARM_RANKING_NAME_LENGTH];                          
    u8 swapHorseName[FARM_RANKING_NAME_LENGTH];                          
    u8 years[FARM_RANKING_SLOTS_WITH_TEMP];                              
    u8 seasons[FARM_RANKING_SLOTS_WITH_TEMP];                            
    u8 mariaAffection[FARM_RANKING_SLOTS_WITH_TEMP];                     
    u8 popuriAffection[FARM_RANKING_SLOTS_WITH_TEMP];                    
    u8 elliAffection[FARM_RANKING_SLOTS_WITH_TEMP];                      
    u8 annAffection[FARM_RANKING_SLOTS_WITH_TEMP];                       
    u8 karenAffection[FARM_RANKING_SLOTS_WITH_TEMP];                     
    u8 harrisAffection[FARM_RANKING_SLOTS_WITH_TEMP];                    
    u8 grayAffection[FARM_RANKING_SLOTS_WITH_TEMP];                      
    u8 jeffAffection[FARM_RANKING_SLOTS_WITH_TEMP];                      
    u8 cliffAffection[FARM_RANKING_SLOTS_WITH_TEMP];                     
    u8 kaiAffection[FARM_RANKING_SLOTS_WITH_TEMP];                       
    u8 wifeAffection[FARM_RANKING_SLOTS_WITH_TEMP];               
    u8 babyAffection[FARM_RANKING_SLOTS_WITH_TEMP];                      
    u8 dogAffection[FARM_RANKING_SLOTS_WITH_TEMP];                       
    u8 horseAffection[FARM_RANKING_SLOTS_WITH_TEMP];                     
    u8 farmAnimalAffection[FARM_RANKING_SLOTS][8];                       
    u8 tempFarmAnimalAffection[8];                                       
    u8 swapFarmAnimalAffection1[4];                                      
    u8 swapFarmAnimalAffection2[4];                                      
    u8 chickenCounts[FARM_RANKING_SLOTS_WITH_TEMP];                      
    u32 cropsShipped[FARM_RANKING_SLOTS_WITH_TEMP];                      
    u32 eggsShipped[FARM_RANKING_SLOTS_WITH_TEMP];                       
    u32 milkShipped[FARM_RANKING_SLOTS_WITH_TEMP];                       
    u32 fishCaught[FARM_RANKING_SLOTS_WITH_TEMP];                        
    u32 gold[FARM_RANKING_SLOTS_WITH_TEMP];                              
    u16 grassTiles[FARM_RANKING_SLOTS_WITH_TEMP];                        
    u8 maxStamina[FARM_RANKING_SLOTS_WITH_TEMP];                         
    u8 photoCount[FARM_RANKING_SLOTS_WITH_TEMP];                         
    u8 recipeCount[FARM_RANKING_SLOTS_WITH_TEMP];                        
    u8 houseExtensions[FARM_RANKING_SLOTS_WITH_TEMP];                    
    u8 happiness[FARM_RANKING_SLOTS_WITH_TEMP];                          
    u8 wife[FARM_RANKING_SLOTS_WITH_TEMP];                          
    f32 scores[FARM_RANKING_SLOTS_WITH_TEMP];                            
    u8 flags[FARM_RANKING_SLOTS];                                        
    u8 tempFlags;                                                        
    u8 swapFlags;                                                        
    u8 cursorIndex;                                                      
    u8 pad2[4];                                                          
    u8 screenState;                                                      
} FarmRankingData;

extern FarmRankingData gFarmRankingData;

extern void func_800E1380(u8);            
extern bool func_800E53E8(u8);    
extern void func_800E1FAC(void);
extern void func_800E8F08(void);

extern u8 gCurrentGameIndex;

extern LoadGameScreenContext loadGameScreenContext;

#endif