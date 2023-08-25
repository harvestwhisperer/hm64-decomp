#ifndef _GAME_MACROS_H	
#define _GAME_MACROS_H 

#define NULL 0

#define TRUE 1
#define FALSE 0

#define SUNNY 1
#define RAIN 2
#define SNOW 3
#define TYPHOON 5

#define SPRING 1
#define SUMMER 2
#define FALL 3
#define WINTER 4

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

/* character indices */
#define PLAYER 0

#define MARIA 0
#define POPURI 1
#define ELLI 2
#define ANN 3
#define KAREN 4
#define BABY 5
#define HARRIS 6
#define GRAY 7
#define JEFF 8
#define CLIFF 9
#define KAI 0xA
#define MAYOR 0xB
#define MAYOR_WIFE 0xC
#define LILLIA 0xD
#define BASIL 0xE
#define ELLEN 0xF
#define DOUG 0x10
#define GOTZ 0x11
#define SASHA 0x12
#define POTION_SHOP_DEALER 0x13
#define KENT 0x14
#define STU 0x15
#define MIDWIFE 0x16
#define MAY 0x17
#define RICK 0x18
#define PASTOR 0x19
#define SHIPPER 0x1A
#define SAIBARA 0x1B
#define DUKE 0x1C
#define GREG 0x1D
#define CARPENTER_1 0x1E
#define CARPENTER_2 0x1F
#define MASTER_CARPENTER 0x20
#define HARVEST_SPRITE_1 0x21
#define HARVEST_SPRITE_2 0x22
#define HARVEST_SPRITE_3 0x23
#define SYDNEY 0x24
#define BARLEY 0x25
#define GOURMET_JUDGE 0x26

#define MAX_NPC_SPRITES 102
#define TOTAL_GAME_VARIABLES 80
#define MAX_TILES 96
#define TOTAL_ITEMS 67
#define MAX_RUCKSACK_SLOTS 9
#define MAX_TOOL_SLOTS_RUCKSACK 9
#define MAX_TOOLCHEST_SLOTS 0x20
#define MAX_KEY_ITEMS 24 

#define MAX_AFFECTION 0xFF

/* item indices */
#define TOMATO 0x10

/* field tiles */
#define UNTILLED 1
#define TILLED 2	
#define TILLED_WATERED 3
#define SMALL_ROCK 4
#define WEED 5
#define LOG 6
#define BROKEN_LOG 7
// to-do: add crops
#define BOULDER 0xC4
#define STUMP 0xC8
#define CORN_RIPE 0x83
#define CORN_DEAD 0x84
#define CORN_PLANT_WITHERED 0x85
#define GRASS_PLANTED 0x88
#define GRASS_GROWING 0x8B
#define GRASS_GROWN 0x8E
#define GRASS_CUT 0x8F
#define MOONDROP_PLANTED 0x90
// to-do: add foragables

/* daily event bit indices */
#define CARPENTER_ESTIMATE 9
#define CARPENTER_FINISHED 0xB
#define STORES_CLOSED_FOR_FESTIVAL 0x2D
#define FESTIVAL 0x4D

/* life event bit indices */
#define MARRIED 0
#define HAVE_BABY 1
#define HAVE_HORSE 5
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
#define MARIA_HARRIS_BABY 0x26
#define POPURI_GRAY_BABY 0x27
#define ELLI_JEFF_BABY 0x28
#define ANN_CLIFF_BABY 0x29
#define KAREN_KAI_BABY 0x2A
#define KAREN_GONE 0x2F
#define ELLEN_DIED 0x31
#define NAMED_HORSE 0x3A
#define WON_CALENDAR_STICKERS_FROM_RAFFLE 0x5B
#define ANN_RICK_CUTSCENE 0xB0
#define POPURI_SICK_CUTSCENE 0xB4
#define ANN_SICK_VISIT 0xC2
#define KAREN_SICK_VISIT 0xC7
#define KAREN_YELLOW_HEART_EVENT_CUTSCENE 0xC8
#define SHADY_SALESMAN_MARIA_CUTSCENE 0xCE
#define ANN_SPRAINED_ANKLE_CUTSCENE 0xCF
#define WIFE_LEFT 0xD9
// 0x4d = sprites taking care of animals during typhoon

/* special dialogue bit indices */
#define ELLI_FINISH_GRIEVING 0x43
#define MARIA_BIRTHDAY 0x7E
#define POPURI_BIRTHDAY 0x7F
#define ELLI_BIRTHDAY 0x80
#define ANN_BIRTHDAY 0x81
#define KAREN_BIRTHDAY 0x82
#define THANKSGIVING 0x145
#define NEW_YEARS_EVE 0x146
#define STARRY_NIGHT_FESTIVAL_DIALOGUE_BIT 0x147

/* main loop callback function indices */
#define MAIN_GAME 1
#define MAP_LOAD 2
// unsure: DREAM_CUTSCENES 3
// unsure: INTRO_LOAD 4
#define LEVEL_LOAD_1 5
#define LEVEL_LOAD_2 6
#define OVERLAY_SCREEN_LOAD 7
#define ROTATING 8
#define DIALOGUE 9
#define TEXT 0xA
// 0xb unknown
#define PINK_OVERLAY_TEXT 0xC
#define END_OF_DAY_1 0xD
#define END_OF_DAY_2 0xE
// 0xf unknown
#define END_OF_FESTIVAL_DAY_1 0x10
#define END_OF_FESTIVAL_DAY_2 0x11
#define TV 0x12
#define SHOP_DIALOGUE 0x13
#define PAUSE_SCREEN_LOAD 0x14
#define TOOLBOX_LOAD 0x15
#define FREEZER_LOAD 0x16
#define CABINET_LOAD 0x17
#define HOME_EXTENSIONS_SELECT_LOAD 0x18
#define ESTIMATE_LOAD 0x19
#define HORSE_RACE_RESULTS_LOAD 0x20
#define HORSE_RACE_GIFTS_LOAD 0x21
#define LOTTERY_LOAD 0x22
#define PAUSE_SCREEN 0x23
#define TOOLBOX 0x24
#define FREEZER 0x25
#define CABINET 0x26
#define HOME_EXTENSIONS_SELECT 0x27
#define ESTIMATE 0x28
#define KITCHEN_PICTURE 0x29
#define CALENDAR 0x2A
#define RECIPE_BOOK 0x2B
#define ALBUM 0x2C
// 0x2d unknown
#define HORSE_RACE_BETTING 0x2E
#define HORSE_RACE_RESULTS 0x2F
#define HORSE_RACE_GIFTS 0x30
#define LOTTERY_2 0x31
#define TITLE_SCREEN 0x32
#define NAMING_SCREEN 0x33
#define SELECT_GAME 0x35
#define ESTIMATE_SCREEN 0x36
// 0x37 unknown
// 0x38 unknown

/* map indices */
#define RANCH 0
#define RANCH_BARN 5
#define RANCH_STORE 6
#define RANCH_HOUSE 7
#define BEACH 9
#define RACE_TRACK 0xD
#define MOUNTAIN_1 0x11
#define MOUNTAIN_1_SUMMER 0x12
#define MOUNTAIN_2 0x15
#define TOP_OF_MOUNTAIN_1 0x19
#define TOP_OF_MOUNTAIN_1_SUMMER 0x1A
#define MOON_MOUNTAIN 0x1D
#define CARPENTER_HUT 0x21
#define DUMPLING_HOUSE 0x22
#define POND 0x23
#define BAKERY 0x28
#define VILLAGE_1 0x29
#define VILLAGE_2 0x2D
#define FLOWER_SHOP 0x32
#define CHURCH 0x33
#define SQUARE 0x35
#define RICK_STORE 0x39
#define TAVERN 0x3B
#define LIBRARY 0x3C
#define MAYOR_HOUSE 0x3E
#define HARVEST_SPRITE_CAVE 0x42
#define CAVE 0x43
#define MINE 0x44
#define VINEYARD 0x47
#define VINEYARD_HOUSE 0x4B
#define VINEYARD_CELLAR 0x4C
#define VINEYARD_CELLAR_BASEMENT 0x4D
#define ROAD 0x4E
#define FARM 0x52
#define GREENHOUSE 0x56
#define HOUSE 0x57
#define BARN 0x58
#define COOP 0x59
#define KITCHEN 0x5A

#define FARM_FIELD_WIDTH 24
#define FARM_FIELD_HEIGHT 20

/* map flags */
#define INDOORS 1
#define OUTDOORS 2

#define MAIN_LOOP_CALLBACK_FUNCTION_TABLE_SIZE 0x39

#define SICKLE 1
#define HOE 2
#define AX 3
#define HAMMER 4
#define WATERING_CAN 5
#define MILKER 6
#define BELL 7
#define BRUSH 8
#define CLIPPERS 9
#define TURNIP_SEEDS 10
#define POTATO_SEEDS 11
#define CABBAGE_SEEDS 12
#define TOMATO_SEEDS 13
#define CORN_SEEDS 14
#define EGGPLANT_SEEDS 15
#define STRAWBERRY_SEEDS 16
#define MOON_DROP_SEEDS 17
#define PINK_CAT_MINT_SEEDS 18
#define BLUE_MIST_SEEDS 19
#define BIRD_FEED 20
#define BALL 21
#define FEEDING_BOTTLE 22
#define FISHING_POLE 24
#define MIRACLE_POTION 25
#define COW_MEDICINE 26
#define GRASS_SEEDS 27
#define BLUE_FEATHER 28
#define EMPTY_BOTTLE 29

#define OCARINA 1
#define FLOWER_SHOP_CARD 2
#define BAKERY_CARD 3
#define BROKEN_MUSIC_BOX 4
#define FIXED_MUSIC_BOX 5
#define DOOR_TO_HEAVEN 6
#define WEATHER_VANE 7
#define GOLD_PENDANT 8
#define LIBRARY_BOOK 9
#define TREASURE_MAP 10
#define MARBLES 11
#define GOODLUCK_CHARM 12
#define MEDAL_BAG 13
#define HORSE_RACE_TICKET 14
#define DOG_RACE_TICKET 15
#define POTPURRI 16
#define EMBROIDERED_HANDKERCHIEF 17
#define HAND_KNIT_SOCKS 18
#define LUCKY_BRACELET 19
#define FLOWER_BATH_CRYSTALS 20
#define STAMINA_CARROT 21

#define char_A 0xA1
#define char_C 0xA3
#define char_E 0xA5
#define char_G 0xA7
#define char_H 0xA8
#define char_J 0xAA
#define char_K 0xAB
#define char_L 0xAC
#define char_M 0xAD
#define char_P 0xB0
#define char_S 0xB3
#define char_W 0xB7
#define char_Z 0xB9
#define char_a 0xBB
#define char_b 0xBC
#define char_c 0xBD
#define char_d 0xBE
#define char_e 0xBF
#define char_f 0xC0
#define char_g 0xC1
#define char_h 0xC2
#define char_i 0xC3
#define char_j 0xC4
#define char_k 0xC5
#define char_l 0xC6
#define char_m 0xC7
#define char_n 0xC8
#define char_o 0xC9
#define char_p 0xCA
#define char_q 0xCB
#define char_r 0xCC
#define char_s 0xCD
#define char_t 0xCE
#define char_u 0xCF
#define char_v 0xD0
#define char_w 0xD1
#define char_x 0xD2
#define char_y 0xD3
#define char_z 0xD4

#define TOOL_USE 1
#define THROWING 2
#define PICKING_UP 4
#define PUTTING_DOWN 5
#define EATING 6
#define ABOUT_TO_DRINK 12
#define WHISTLING 19
#define DRINKING 20
#define CASTING_FISHING_ROD 22

// exit indices
#define HOUSE_EXIT 0
#define BATHROOM_TO_KITCHEN 0xB
#define MIDDLE_OF_HOUSE 0xC
#define ROAD_FROM_VILLAGE_1 0x17
#define CARPENTER_HUT_EXIT 0x20
#define MOON_MOUNTAIN_EXIT 0x21
#define DUMPLING_HOUSE_EXIT 0x26
#define RANCH_FROM_RANCH_STORE 0x2B
#define RANCH_FROM_RANCH_HOUSE 0x2C
#define MINE_EXIT 0x47
#define SPRING_ENTER 0x48
#define CARPERNTER_HUT_ENTER 0x49
#define ANN_ROOM_ENTER 0x4D
#define FLOWER_SHOP_ENTRANCE 0x57

#endif