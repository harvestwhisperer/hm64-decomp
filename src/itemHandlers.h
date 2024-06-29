#ifndef _ITEM_HANDLERS_H_
#define _ITEM_HANDLERS_H

#include "common.h"

#define TOTAL_ITEMS 67
#define MAX_KEY_ITEMS 24 

/* tools */
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
#define CHICKEN_FEED 20
#define BALL 21
#define FEEDING_BOTTLE 22
#define FISHING_POLE 24
#define MIRACLE_POTION 25
#define COW_MEDICINE 26
#define GRASS_SEEDS 27
#define BLUE_FEATHER 28
#define EMPTY_BOTTLE 29

/* key/special items */
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

// TODO: finish adding items
/* item indices */
#define WEED_ITEM_INDEX 1
#define ROCK 2
#define LOG_ITEM_INDEX 3
#define MOONDROP_FLOWER 4
#define PINK_CAT_MINT_FLOWER 5  
#define BLUE_MIST_FLOWER 6
#define CAKE 7
#define PIE 8
#define COOKIES 9
// 10-13 not used?
#define TURNIP 13
#define POTATO 14
#define CABBAGE 15
#define TOMATO 16
#define CORN 17
#define EGGPLANT 18
#define STRAWBERRY 19
#define EGG 20
#define SMALL_MILK 21
#define MEDIUM_MILK 22
#define LARGE_MILK 23
#define GOLDEN_MILK 24
#define WOOL 25
#define HIGH_QUALITY_WOOL 26
#define WILD_GRAPES 27
#define VERYBERRY 28
#define TROPICAL_FRUIT 29
#define WALNUT 30
#define MUSHROOM 31
#define POISONOUS_MUSHROOM 32
#define BOOK 33
#define BERRY_OF_THE_FULLMOON 34
#define MEDICINAL_HERB 35
#define EDIBLE_HERB 36
#define SMALL_FISH 37
#define MEDIUM_FISH 38
#define LARGE_FISH 39
#define DUMPLING 40
#define COTTON_CANDY 41
#define FRIED_OCTUPUS 42
#define ROASTED_CORN 43
#define CANDY 44
#define CHOCOLATE 45
#define IRON_ORE 46
#define BLUE_ROCK 47
#define RARE_METAL 48
#define MOONLIGHT_STONE 49
#define PONTANA_ROOT 50
#define FODDER 57
#define SEEDS_ITEM_INDEX 64
#define MEDICINE 72
#define SHEARS_ITEM_INDEX 73
#define RICE_CAKE 80
#define BREAD 81
#define EGGS 82
#define POWER_NUT 87
#define DOG_ITEM_INDEX 89
#define CHICK_ITEM_INDEX 101
#define BOTTLE_WITH_WINE 114
#define PUPPY 128
#define CAT 137
#define SQUIRREL 154
#define MONKEY 165
#define DOG_2_ITEM_INDEX 176
#define INSECT 183
#define CRICKET 185
#define BABY_ITEM_INDEX 192
#define GROWN_UP_BABY 195
#define OPEN_BOOK 202
#define SICKLE_ITEM_INDEX 203
#define OCARINA_ITEM_INDEX 225

/* bottle contents */
#define EMPTY 0

/* power nut bits */
#define FISHING_POWER_NUT 4

// 0x80204DF8
typedef struct {
    Vec3f unk_0; // 0xDF8
    Vec3f unk_C;  // DFC
    Vec3f unk_18; // 0xE10
    u16 unk_24; // 0xE1C
    u16 unk_26; // 0xE1E
    u16 unk_28; // 0xE20 = animation flagss
    u8 unk_2A; // 0xE22
    u8 flags; // 0xE23
} ItemContext;

// 0x80189828
typedef struct {
    u16 unk_0; // 28
    u8 unk_2; // 2A
    u8 unk_3; // 2B
    u16 unk_4; // 2C
    u16 unk_6; // 2E
    u8 unk_8; // 30
    u16 unk_A; // 32
    u16 unk_C; // 34
    u8 unk_E; // 36
    u8 unk_F; // 37;
    u32 unk_10; // 38
} ToolUse;

extern void func_800CF850();     
extern void func_800CFB38(u8, u8, f32, f32, f32);   
extern void func_800CFD78(u8, u8);                         
extern void func_800CFDA0(u8);       
extern void func_800D0318(); 
extern u8 func_800D5308(u8 index, u8 arg1, u32 arg2, s32 arg3, s32 arg4);     
extern u8 func_800D5390(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4);
extern void func_800D5548(u8);           
extern void func_800D55E4(u8, u8);  
extern void func_800D5688(u8 index, f32 arg1, f32 arg2, f32 arg3); 
extern u16 func_800D5A88(u16 arg0);         
extern u8 func_800D5B00(u16 index);                          
extern u8 func_800D5B18(u16 index);   
extern void func_800D51B0();                  
extern void func_800D5290();  
extern u16 func_800D5A6C(u16 index);   
extern bool func_800D5B30();
extern void func_800D67FC(u8 index);
extern void func_800D7010();  

extern ItemContext itemInfo[10];
extern ToolUse toolUse;

// TODO: label
// tomatoes shipped
extern u32 D_80188F60;
// cabbages shipped
extern u32 D_801C3F80;
// corn shipped
extern u32 D_801FB5D0;
// turnips shipped
extern u32 D_801654F4;
// eggplants shipped
extern u32 D_801FB6FC;
// potatoes shipped
extern u32 D_80237414;
// strawberries shipped
extern u32 D_801806C0;

extern u8 D_801C3F35;
extern u8 D_801FD624;

extern u16 fodderQuantity;

#endif