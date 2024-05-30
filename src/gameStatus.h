#ifndef _GAME_STATUS_H_
#define _GAME_STATUS_H_

#include "common.h"

// TODO: finish adding these
/* daily event bit indices */
#define DAILY_SHIPMENT 0
#define CARPENTER_ESTIMATE 9
#define CARPENTER_FINISHED 0xB
// 0xD = something to do with carrying
#define SUCCESSFUL_PROPOSAL 0x14
// 0x18 = thanksgiving mayor house
#define FARM_MORNING_VISIT 0x17
#define STORES_CLOSED_FOR_FESTIVAL 0x2D
#define FESTIVAL_DAY 0x30
#define FESTIVAL 0x4D
#define FESTIVAL_2 0x4E
// sick/sprained ankle
#define MARIA_ABSENT 0x56

// TODO: confirm and finish adding these
/* life event bit indices */
#define MARRIED 0
#define HAVE_BABY 1
// #define BLUE_MIST 2
// #define CHICKEN_BORN 3
// #define FARM_ANIMAL_BORN 4
#define HAVE_HORSE 5
// #define GIVE_RICK_RARE_METAL 6
#define HAVE_VASE 8
#define HOUSE_EXTENSION_CONSTRUCTION 0xD
#define ENGAGED 0xF
#define HAVE_KITCHEN 0x10
#define HAVE_BATHROOM 0x11
#define HAVE_STAIRS 0x12
#define HAVE_GREENHOUSE 0x13
#define HAVE_LOG_TERRACE 0x14
#define HAVE_BABY_BED 0x15
#define HAVE_CABINET 0x16
#define HAVE_TABLECLOTH 0x17
#define HAVE_RUG 0x18
#define MARIA_HARRIS_MARRIED 0x1A
#define POPURI_GRAY_MARRIED 0x1B
#define ELLI_JEFF_MARRIED 0x1C
#define ANN_CLIFF_MARRIED 0x1D
#define KAREN_KAI_MARRIED 0x1E
#define WIFE_PREGNANT 0x1F
#define MARIA_PREGNANT 0x21
#define POPURI_PREGNANT 0x22
#define ELLI_PREGNANT 0x23
#define ANN_PREGNANT 0x24
#define KAREN_PREGNANT 0x25
#define MARIA_HARRIS_BABY 0x26
#define POPURI_GRAY_BABY 0x27
#define ELLI_JEFF_BABY 0x28
#define ANN_CLIFF_BABY 0x29
#define KAREN_KAI_BABY 0x2A
#define ANIMAL_DIED 0x2B
#define BASIL_GONE 0x2C
#define CLIFF_GONE 0x2D
#define KAI_GONE 0x2E
#define KAREN_GONE 0x2F
#define ELLEN_DIED 0x31
#define NAMED_HORSE 0x3A
#define MAYOR_TOUR 0x3E
#define PAINTING_FROM_MARIA 0x3F
#define BASIL_IN_TOWN 0x40
#define CLIFF_ARRIVED 0x41
// #define SMASHED_POND_BOULDER 0x46
// #define HAVE_TURTLE 0x47
#define BRIDGE_COMPLETED 0x48
#define HOT_SPRINGS_COMPLETED 0x4C
// 0x4D have more than 2 animals
// #define ENTERED_HORSE_RACE 0x51
// #define ENTERED_DOG_RACE 0x52
// #define HAVE_STUFFED_HORSE 0x58
// #define HAVE_CUSHION 0x5A
#define WON_CALENDAR_STICKERS_FROM_RAFFLE 0x5B
#define PUPPIES 0x69
#define MARIA_FARM_GIFT_1 0x6D
#define MARIA_FARM_GIFT_2 0x6E
#define POPURI_FARM_GIFT_1 0x70
#define POPURI_FARM_GIFT_2 0x71
// 0x9C karen pink heart event/beach cutscene
// 0x9D kai beach cutscene
// #define CARPENTER_WINTER_CUTSCENE 0xAC
#define ANN_RICK_CUTSCENE 0xB0
// FIXME: make labels consistent (girls visit player vs. player visits girl cutscenes vs. cutscene index vs. bit flags)
#define POPURI_SICK_CUTSCENE 0xB4
#define MARIA_SICK_CUTSCENE 0xBB
#define MARIA_SPRAINED_ANKLE_CUTSCENE 0xBC
#define ANN_SICK_VISIT 0xC2
// 0xC3-C6 vineyard cutscenes
#define KAREN_SICK_VISIT 0xC7
#define KAREN_YELLOW_HEART_EVENT_CUTSCENE 0xC8
#define KAREN_AND_ELLI_BEACH_CUTSCENE 0xCB
#define SHADY_SALESMAN_MARIA_CUTSCENE 0xCE
#define ANN_SPRAINED_ANKLE_CUTSCENE 0xCF
#define WIFE_LEFT 0xD9
// 0x4D = sprites taking care of animals during typhoon

// TODO: finish adding these
/* special dialogue bit indices */
// 3 = wedding or newlywed
// 0xB-F = rivals' weddings
#define HAVE_HOUSE_EXTENSION 0x12
#define RAIN_FORECAST_DIALOGUE 0x13
#define HAVE_GOLD_PENDANT 0x17
#define MARRIED_TO_KAREN_DIALOGUE 0x21
#define MARRIED_TO_ANN_DIALOGUE 0x26
#define HAVE_HORSE_DIALOGUE 0x27
#define MARIA_HARRIS_BABY_DIALOGUE 0x32
#define POPURI_GRAY_BABY_DIALOGUE 0x33
#define ELLI_JEFF_BABY_DIALOGUE 0x34
#define ANN_CLIFF_BABY_DIALOGUE 0x35
#define KAREN_KAI_BABY_DIALOGUE 0x36
#define MARIA_PREGNANT_DIALOGUE 0x37
#define POPURI_PREGNANT_DIALOGUE 0x38
#define ELLI_PREGNANT_DIALOGUE 0x39
#define ANN_PREGNANT_DIALOGUE 0x3A
#define KAREN_PREGNANT_DIALOGUE 0x3B
#define ELLI_FINISH_GRIEVING 0x43
#define MARRIED_TO_MARIA_DIALOGUE 0x7D
#define MARIA_BIRTHDAY 0x7E
#define POPURI_BIRTHDAY 0x7F
#define ELLI_BIRTHDAY 0x80
#define ANN_BIRTHDAY 0x81
#define KAREN_BIRTHDAY 0x82
// could be newlywed dialogue bit
#define GOT_MARRIED_DIALOGUE 0x83
#define MARRIED_TO_POPURI_DIALOGUE 0x8C
#define MARRIED_TO_ELLI_DIALOGUE 0x8D
#define PLAYER_HARVEST_KING 0x142
#define THANKSGIVING 0x145
#define NEW_YEARS_EVE 0x146
#define STARRY_NIGHT_FESTIVAL_DIALOGUE_BIT 0x147
#define WIFE_PREGNANT_MARIA 0x14C
#define WIFE_PREGNANT_POPURI 0x14D
#define WIFE_PREGNANT_ELLI 0x14E
#define WIFE_PREGNANT_ANN 0x14F
#define WIFE_PREGNANT_KAREN 0x150

// TODO: add mail bits

extern u32 checkDailyEventBit(u16 bitIndex);
extern void setDailyEventBit(u16 bitIndex);
extern void toggleDailyEventBit(u16 bitIndex);   

extern void setLifeEventBit(u16);                    
extern void toggleLifeEventBit(u16);                            
extern u32 checkLifeEventBit(u16 bitIndex);   

extern u32 checkSpecialDialogueBit(u16 bitIndex);
extern void setSpecialDialogueBit(u16 bitIndex);
extern void toggleSpecialDialogueBit(u16 bitIndex);    

extern u32 checkMailRead(u16);                            
extern void setMail(u16);      
extern void toggleReadLetterBit(u16);      

void setAlbumPicture(u8 pictureBitIndex);
extern void setRecipes(void);  

extern void handleWifeMorningHelp(void); 
extern void func_80064114(void);
extern void func_800644B0(void);     
extern void func_80064814(void);     
// recipes
extern u8 func_80065518(void);
extern u8 func_8006536C();
// might be s32 return value
extern s32 getSumNpcAffection(void);

extern u32 dailyEventBits[0x20];
extern u32 lifeEventBits[0x20];
extern u32 specialDialogueBits[0x10];
extern u32 recipesBits[1];
extern u32 readMailBits[];
extern u32 mailboxBits[];
// FIXME: maybe should be array [1]?
extern u32 albumBits;

extern u8 D_80113760[0x14][0x18];

#endif