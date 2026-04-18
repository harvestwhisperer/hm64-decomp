#ifndef _ITEM_HANDLERS_H_
#define _ITEM_HANDLERS_H

#include "common.h"

#define TOTAL_ITEMS 67
#define MAX_KEY_ITEMS 24 
#define MAX_TOOLS 30

/* holdable item flags */
#define ITEM_EATABLE                    0x0001  
#define ITEM_RUCKSACK_STORABLE          0x0002   
#define ITEM_SHIPPABLE                  0x0004   
#define ITEM_THROWABLE                  0x0008  
#define ITEM_PLACEABLE_ON_GROUND        0x0010  
#define ITEM_INCUBATOR_PLACEABLE        0x0020   
#define ITEM_ANIMAL_FEED                0x0040   
#define ITEM_SMALL_ANIMAL               0x0080  
#define ITEM_BABY                       0x0100   
#define ITEM_FREEZER_STORABLE           0x0200   
#define ITEM_CRIB_PLACEABLE             0x0400
#define ITEM_TOOL                       0x0800  
#define ITEM_CABINET_STORABLE           0x1000  
#define ITEM_LUMBER_STORABLE            0x2000
#define ITEM_HAS_ALCOHOL                0x4000  
#define ITEM_HARVEST_GODDESS_OFFERABLE  0x8000 

/* ItemContext flags */
#define ITEM_CONTEXT_ACTIVE             0x01  
#define ITEM_CONTEXT_USE_POSITION       0x02  
#define ITEM_CONTEXT_HAS_DIRECTION_FRAME 0x04  
#define ITEM_CONTEXT_USE_ATTACHMENT     0x08 

/* Item state indices */
#define ITEM_STATE_INACTIVE             0   
#define ITEM_STATE_CLEANUP              1   // Wait then clear (after throw lands, shipping, etc.)
#define ITEM_STATE_HELD                 2   
#define ITEM_STATE_THROW_START          3  
#define ITEM_STATE_THROW_MOTION         4  
#define ITEM_STATE_THROW_FLIGHT         5   
#define ITEM_STATE_PICKUP               6   
#define ITEM_STATE_PICKUP_DONE          7   
#define ITEM_STATE_PLACE                8   
#define ITEM_STATE_PUT_DOWN             9   
#define ITEM_STATE_EATING               10  
#define ITEM_STATE_THROW_LANDED         12  
#define ITEM_STATE_RAISED               13  
#define ITEM_STATE_SHIP                 14 
#define ITEM_STATE_DIALOGUE_SELECTION          16 
#define ITEM_STATE_DOG_BOWL             17  
#define ITEM_STATE_FEED_DOG             18  
#define ITEM_STATE_RAISED_ALT           19 
#define ITEM_STATE_DROP_IN_WATER        20  
#define ITEM_STATE_TOOL_ACQUIRED        21  

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
// 23
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

/* item indices */
#define WEED_HELD_ITEM 1
#define ROCK_HELD_ITEM 2
#define LOG_HELD_ITEM 3
#define MOONDROP_FLOWER_HELD_ITEM 4
#define PINK_CAT_MINT_FLOWER_HELD_ITEM 5  
#define BLUE_MIST_FLOWER_HELD_ITEM 6
#define CAKE_HELD_ITEM 7
#define PIE_HELD_ITEM 8
#define COOKIES_HELD_ITEM 9
#define BLUE_FEATHER_HELD_ITEM 10
#define DOOR_TO_HEAVEN_HELD_ITEM 11
// 12 = chocolate
#define TURNIP_HELD_ITEM 13
#define POTATO_HELD_ITEM 14
#define CABBAGE_HELD_ITEM 15
#define TOMATO_HELD_ITEM 16
#define CORN_HELD_ITEM 17
#define EGGPLANT_HELD_ITEM 18
#define STRAWBERRY_HELD_ITEM 19
#define EGG_HELD_ITEM 20
#define SMALL_MILK_HELD_ITEM 21
#define MEDIUM_MILK_HELD_ITEM 22
#define LARGE_MILK_HELD_ITEM 23
#define GOLDEN_MILK_HELD_ITEM 24
#define WOOL_HELD_ITEM 25
#define HIGH_QUALITY_WOOL_HELD_ITEM 26
#define WILD_GRAPES_HELD_ITEM 27
#define VERYBERRY_HELD_ITEM 28
#define TROPICAL_FRUIT_HELD_ITEM 29
#define WALNUT_HELD_ITEM 30
#define MUSHROOM_HELD_ITEM 31
#define POISONOUS_MUSHROOM_HELD_ITEM 32
#define BOOK_HELD_ITEM 33
#define BERRY_OF_THE_FULLMOON_HELD_ITEM 34
#define MEDICINAL_HERB_HELD_ITEM 35
#define EDIBLE_HERB_HELD_ITEM 36
#define SMALL_FISH_HELD_ITEM 37
#define MEDIUM_FISH_HELD_ITEM 38
#define LARGE_FISH_HELD_ITEM 39
#define DUMPLING_HELD_ITEM 40
#define COTTON_CANDY_HELD_ITEM 41
#define FRIED_OCTUPUS_HELD_ITEM 42
#define ROASTED_CORN_HELD_ITEM 43
#define CANDY_HELD_ITEM 44
#define CHOCOLATE_HELD_ITEM 45
#define IRON_ORE_HELD_ITEM 46
#define BLUE_ROCK_HELD_ITEM 47
#define RARE_METAL_HELD_ITEM 48
#define MOONLIGHT_STONE_HELD_ITEM 49
#define PONTATA_ROOT_HELD_ITEM 50
#define FODDER_HELD_ITEM 57
#define SEEDS_HELD_ITEM 64
#define MEDICINE_HELD_ITEM 72
#define SHEARS_HELD_ITEM 73
#define RICE_CAKE_HELD_ITEM 80
#define BREAD_HELD_ITEM 81
#define EGGS_HELD_ITEM 82
#define POWER_NUT_HELD_ITEM 87
#define DOG_HELD_ITEM 88
// multiple directions
#define CHICKEN_HELD_ITEM 96
// multiple directions
#define CHICK_HELD_ITEM 104
// 105-111
#define EMPTY_BOTTLE_HELD_ITEM 112
#define BOTTLE_WITH_WATER_HELD_ITEM 113
#define BOTTLE_WITH_WINE_HELD_ITEM 114
// 115-117
#define BOTTLE_WITH_MILK_HELD_ITEM 118
// 119
#define BOTTLE_WITH_TEA_HELD_ITEM 120
// 121-122
#define PUPPY_1_HELD_ITEM 123
// multiple directions
#define CAT_HELD_ITEM 131
// multiple directions
#define FOX_HELD_ITEM 139
// multiple directions
#define SQUIRREL_HELD_ITEM 147
// multiple directions
#define RABBIT_HELD_ITEM 155
// multiple directions
#define MONKEY_HELD_ITEM 163
// multiple directions
#define DOG_2_HELD_ITEM 171
// multiple directions
#define BUTTERFLY_HELD_ITEM 179
#define LADYBUG_HELD_ITEM 180
#define CICADA_HELD_ITEM 181
#define HORNED_BEETLE_HELD_ITEM 182
#define STAG_BEETLE_HELD_ITEM 183
#define RED_DRAGONFLY_HELD_ITEM 184
#define CRICKET_HELD_ITEM 185
#define BABY_HELD_ITEM 186
// multiple directions
// #define BABY_HELD_ITEM 192
#define BABY_GROWN_HELD_ITEM 194
// multiple directions
// #define BABY_GROWN_HELD_ITEM 195
#define OPEN_BOOK_HELD_ITEM 202
#define SICKLE_HELD_ITEM 203
#define SILVER_SICKLE_HELD_ITEM 204
#define GOLDEN_SICKLE_HELD_ITEM 205
#define HOE_HELD_ITEM 206
#define SILVER_HOE_HELD_ITEM 207
#define GOLDEN_HOE_HELD_ITEM 208
#define AX_HELD_ITEM 209
#define SILVER_AX_HELD_ITEM 210
#define GOLDEN_AX_HELD_ITEM 211
#define HAMMER_HELD_ITEM 212
#define SILVER_HAMMER_HELD_ITEM 213
#define GOLDEN_HAMMER_HELD_ITEM 214
#define WATERING_CAN_HELD_ITEM 215
#define SILVER_WATERING_CAN_HELD_ITEM 216
#define GOLDEN_WATERING_CAN_HELD_ITEM 217
#define BROKEN_MUSIC_BOX_HELD_ITEM 218
#define OCARINA_HELD_ITEM 225

/* held item dialogue indices */
#define HELD_ITEM_DIALOGUE_TURNIP                1   // raw TURNIP (13)
#define HELD_ITEM_DIALOGUE_POTATO                2   // raw POTATO (14)
#define HELD_ITEM_DIALOGUE_CABBAGE               3   // raw CABBAGE (15)
#define HELD_ITEM_DIALOGUE_TOMATO                4   // raw TOMATO (16)
#define HELD_ITEM_DIALOGUE_CORN                  5   // raw CORN (17)
#define HELD_ITEM_DIALOGUE_EGGPLANT              6   // raw EGGPLANT (18)
#define HELD_ITEM_DIALOGUE_STRAWBERRY            7   // raw STRAWBERRY (19)
#define HELD_ITEM_DIALOGUE_EGG                   8   // raw EGG_HELD_ITEM (20)
#define HELD_ITEM_DIALOGUE_SMALL_MILK            9   // raw SMALL_MILK (21)
#define HELD_ITEM_DIALOGUE_MEDIUM_MILK           10  // raw MEDIUM_MILK (22)
#define HELD_ITEM_DIALOGUE_LARGE_MILK            11  // raw LARGE_MILK (23)
#define HELD_ITEM_DIALOGUE_GOLDEN_MILK           12  // raw GOLDEN_MILK (24)
#define HELD_ITEM_DIALOGUE_MOONDROP_FLOWER       13  // raw MOONDROP_FLOWER_HELD_ITEM (4)
#define HELD_ITEM_DIALOGUE_PINK_CAT_MINT_FLOWER  14  // raw PINK_CAT_MINT_FLOWER_HELD_ITEM (5)
#define HELD_ITEM_DIALOGUE_CAKE                  15  // raw CAKE (7)
#define HELD_ITEM_DIALOGUE_PIE                   16  // raw PIE (8)
#define HELD_ITEM_DIALOGUE_COOKIES               17  // raw COOKIES (9)
#define HELD_ITEM_DIALOGUE_WOOL                  18  // raw WOOL (25)
#define HELD_ITEM_DIALOGUE_HIGH_QUALITY_WOOL     19  // raw HIGH_QUALITY_WOOL (26)
#define HELD_ITEM_DIALOGUE_WILD_GRAPES           20  // raw WILD_GRAPES (27)
#define HELD_ITEM_DIALOGUE_VERYBERRY             21  // raw VERYBERRY (28)
#define HELD_ITEM_DIALOGUE_TROPICAL_FRUIT        22  // raw TROPICAL_FRUIT (29)
#define HELD_ITEM_DIALOGUE_WALNUT                23  // raw WALNUT (30)
#define HELD_ITEM_DIALOGUE_MUSHROOM              24  // raw MUSHROOM (31)
#define HELD_ITEM_DIALOGUE_BERRY_OF_THE_FULLMOON 25  // raw BERRY_OF_THE_FULLMOON (34)
#define HELD_ITEM_DIALOGUE_DUMPLING              26  // raw DUMPLING (40)
#define HELD_ITEM_DIALOGUE_MEDICINAL_HERB        27  // raw MEDICINAL_HERB (35)
#define HELD_ITEM_DIALOGUE_EDIBLE_HERB           28  // raw EDIBLE_HERB (36)
#define HELD_ITEM_DIALOGUE_SMALL_FISH            29  // raw SMALL_FISH (37)
#define HELD_ITEM_DIALOGUE_MEDIUM_FISH           30  // raw MEDIUM_FISH (38)
#define HELD_ITEM_DIALOGUE_LARGE_FISH            31  // raw LARGE_FISH (39)
#define HELD_ITEM_DIALOGUE_COTTON_CANDY          32  // raw COTTON_CANDY (41)
#define HELD_ITEM_DIALOGUE_FRIED_OCTUPUS         33  // raw FRIED_OCTUPUS (42)
#define HELD_ITEM_DIALOGUE_CHOCOLATE             34  // raw CHOCOLATE (45)
#define HELD_ITEM_DIALOGUE_CANDY                 35  // raw CANDY (44)
// 36-40: no raw items remap to these indices
#define HELD_ITEM_DIALOGUE_FODDER                41  // raw FODDER_HELD_ITEM (57)
#define HELD_ITEM_DIALOGUE_LOG                   42  // raw LOG_HELD_ITEM (3)
#define HELD_ITEM_DIALOGUE_IRON_ORE              43  // raw IRON_ORE (46)
#define HELD_ITEM_DIALOGUE_RARE_METAL            44  // raw RARE_METAL (48)
#define HELD_ITEM_DIALOGUE_BLUE_ROCK             45  // raw BLUE_ROCK (47)
#define HELD_ITEM_DIALOGUE_MOONLIGHT_STONE       46  // raw MOONLIGHT_STONE (49)
#define HELD_ITEM_DIALOGUE_ROCK                  47  // raw ROCK_HELD_ITEM (2)
#define HELD_ITEM_DIALOGUE_WEED                  48  // raw WEED_HELD_ITEM (1)
#define HELD_ITEM_DIALOGUE_POISONOUS_MUSHROOM    49  // raw POISONOUS_MUSHROOM (32)
// 50: raw 10 (unnamed in items.h); routed to .proposal in mariaDialogue
#define HELD_ITEM_DIALOGUE_DOG                   51  // raw DOG_HELD_ITEM (88) + 7 direction variants
#define HELD_ITEM_DIALOGUE_BABY                  52  // raw BABY_HELD_ITEM (186) + grown + 14 variants
#define HELD_ITEM_DIALOGUE_HORNED_BEETLE         53  // raw HORNED_BEETLE_HELD_ITEM (182)
#define HELD_ITEM_DIALOGUE_STAG_BEETLE           54  // raw STAG_BEETLE_HELD_ITEM (183)
// 55: no raw items remap to this index
#define HELD_ITEM_DIALOGUE_CICADA                56  // raw CICADA_HELD_ITEM (181)
#define HELD_ITEM_DIALOGUE_BUTTERFLY             57  // raw BUTTERFLY_HELD_ITEM (179)
#define HELD_ITEM_DIALOGUE_LADYBUG               58  // raw LADYBUG_HELD_ITEM (180)
#define HELD_ITEM_DIALOGUE_RED_DRAGONFLY         59  // raw RED_DRAGONFLY_HELD_ITEM (184)
#define HELD_ITEM_DIALOGUE_CRICKET               60  // raw CRICKET_HELD_ITEM (185)
#define HELD_ITEM_DIALOGUE_PONTATA_ROOT          61  // raw PONTATA_ROOT (50)
#define HELD_ITEM_DIALOGUE_CHICKEN               62  // raw CHICKEN_HELD_ITEM (96) + 7 direction variants
#define HELD_ITEM_DIALOGUE_CHICK                 63  // raw CHICK_HELD_ITEM (104) + 7 direction variants
// 64: aggregate wild-animal index (CAT, FOX, SQUIRREL, RABBIT, MONKEY — 40 raw IDs)
#define HELD_ITEM_DIALOGUE_PUPPY                 65  // raw PUPPY_1_HELD_ITEM (123) + 7 direction variants
#define HELD_ITEM_DIALOGUE_DOG_2                 66  // raw DOG_2_HELD_ITEM (171) + 7 direction variants

/* bottle contents */
#define BOTTLE_CONTENTS_EMPTY 0
#define BOTTLE_CONTENTS_WATER 1
#define BOTTLE_CONTENTS_WINE 2
#define BOTTLE_CONTENTS_CURE_ALL_MEDICINE 3
#define BOTTLE_CONTENTS_VITAMIN_PREPARATION 4
#define BOTTLE_CONTENTS_VITAMIN_GOLD 5
#define BOTTLE_CONTENTS_MILK 6
#define BOTTLE_CONTENTS_HONEY 7

// 0x80204DF8
typedef struct {
    Vec3f position;             
    Vec3f movement;              
    Vec3f attachmentOffset;      
    u16 unk_24;                  
    u16 itemAnimationFrameCounter; 
    u16 heldItemIndex;           
    u8 stateIndex;               
    u8 flags;                
} ItemContext;

// 0x80189828
typedef struct {
    u16 unk_0; // unused
    u8 stepIndex;
    u8 stumpHitCounter; 
    s16 stumpHitPositionX;
    s16 stumpHitPositionZ;
    u8 boulderHitCounter; 
    s16 boulderHitPositionX;
    s16 boulderHitPositionZ;
    u8 toolUseState; 
    u8 musicBoxTileDigCounter;
} ToolUse;

typedef struct {
    u16 entityAssetIndex;
    u16 animationIndex[4];
} ItemEntityMetadata;

extern void resetToolUseState(void);
extern void spawnToolEffectEntity(u8 itemOffset, u8 animationIndex, f32 x, f32 y, f32 z);
extern void spawnFishingRodEntity(u8, u8, f32, f32, f32);
extern void setItemEntityAnimation(u8, u8);
extern void deactivateItemEntity(u8);
extern void processToolUseState();
extern u8 initializeHeldItem(u8 index, u8 arg1, u32 arg2, s32 arg3, s32 arg4);
extern u8 allocateHeldItemSlot(u8 index, u8 arg1, u32 arg2, u16 arg3, u8 arg4);
extern void clearHeldItemSlot(u8);
extern void setItemState(u8, u8);
extern void setItemAttachmentOffset(u8 index, f32 arg1, f32 arg2, f32 arg3);
extern u16 getItemAnimationIndex(u16 arg0);
extern u8 getItemStaminaValue(u16 index);
extern u8 getItemFatigueValue(u16 index);
extern void clearAllItemContextSlots();
extern void loadActiveItemEntities();
extern u16 getItemFlags(u16 index);
extern bool handlePickUpGroundItem();
extern void showHeldItemText(u8 index);
extern void updateHeldItemState();
extern u8 getToolLevel(u8 tool);
extern u8 getItemStaminaIncreaseValue(u16 index);
extern u8 getItemFatigueReductionValue(u16 index);
extern u8 allocateThrownItemSlot(u8 index, u8 arg1, u32 heldItemIndex, u16 arg3, u8 arg4);

extern ItemContext itemInfo[10];
extern ToolUse toolUse;

extern u32 totalTomatoesShipped;
extern u32 totalCabbageShipped;
extern u32 totalCornShipped;
extern u32 totalTurnipsShipped;
extern u32 totalEggplantsShipped;
extern u32 totalPotatoesShipped;
extern u32 totalStrawberriesShipped;

extern u8 turnipSeedsQuantity;
extern u8 potatoSeedsQuantity;
extern u8 eggplantSeedsQuantity;
extern u8 cabbageSeedsQuantity;
extern u8 strawberrySeedsQuantity;
extern u8 tomatoSeedsQuantity;
extern u8 cornSeedsQuantity;
extern u8 grassSeedsQuantity;
extern u8 moondropSeedsQuantity;
extern u8 pinkCatMintSeedsQuantity;
extern u8 blueMistSeedsQuantity;

extern u16 fodderQuantity;
extern u16 chickenFeedQuantity;

extern u8 wateringCanUses;

extern u8 upgradedToolIndex;
extern u8 upgradedToolLevelIndex;

extern u8 gItemBeingHeld;

#endif