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

// TODO: finish adding items
/* item indices */
#define NONE_HELD_ITEM1 0
#define WEED_HELD_ITEM 1
#define ROCK_HELD_ITEM 2
#define LOG_HELD_ITEM 3
#define MOONDROP_FLOWER_HELD_ITEM 4
#define PINK_CAT_MINT_FLOWER_HELD_ITEM 5  
#define BLUE_MIST_FLOWER_HELD_ITEM 6
#define CAKE 7
#define PIE 8
#define COOKIES 9
#define BLUE_FEATHER_HELD_ITEM 10
#define OLD_WINE_HELD_ITEM 11
#define CHOCOLATE_HELD_ITEM 12 // This one seems to be off as it uses sprite animation 0 for chocolate
#define TURNIP 13
#define POTATO 14
#define CABBAGE 15
#define TOMATO 16
#define CORN 17
#define EGGPLANT 18
#define STRAWBERRY 19
#define EGG_HELD_ITEM 20
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
#define WEATHER_VANE_HELD_ITEM 51
#define CHOCOLATE_HELD_ITEM1 52 // This one seems to be off as it uses sprite animation 0 for chocolate
#define NONE_HELD_ITEM2 53 // description is as diary
#define OCARINA_HELD_ITEM 54
#define NONE_HELD_ITEM3 55 // description is as diary
#define CHOCOLATE_HELD_ITEM2 56 // This one seems to be off as it uses sprite animation 0 for chocolate
#define FODDER_HELD_ITEM 57 // from this one we jump from index 58 to 97
// What and why is the gap here for indexes on shopItemsAnimationScripts?
#define TURNIP_SEEDS_HELD_ITEM 58 // we jump to index 97 in shopItemsAnimationScripts
#define POTATO_SEEDS_HELD_ITEM 59
#define CABBAGE_SEEDS_HELD_ITEM 60
#define TOMATO_SEEDS_HELD_ITEM 61
#define CORN_SEEDS_HELD_ITEM 62
#define EGGPLANT_SEEDS_HELD_ITEM 63
#define STRAWBERRY_SEEDS_HELD_ITEM 64
#define GRASS_SEEDS_HELD_ITEM 65
#define MOON_DROP_SEEDS_HELD_ITEM 66
#define PINK_CAT_MINT_SEEDS_HELD_ITEM 67
#define BLUE_MIST_SEEDS_HELD_ITEM 68 // seems seeds block ends here ends on index 107 on shopItemsAnimationScripts
// What and why is the gap here for indexes on shopItemsAnimationScripts?
#define CHICKEN_FEED_HELD_ITEM 69 // jumps to index 131 on shopItemsAnimationScripts
#define BARN_FEED_HELD_ITEM 70
#define MIRACLE_POTION_HELD_ITEM 71
#define MEDICINE_HELD_ITEM 72
#define SHEARS_HELD_ITEM 73
#define BRUSH_HELD_ITEM 74
#define MILKER_HELD_ITEM 75
#define CALENDAR_HELD_ITEM 76
#define RUG_HELD_ITEM 77 // seems this block ends here ends on index 139 on shopItemsAnimationScripts
// What and why is the gap here for indexes on shopItemsAnimationScripts?
#define BLUE_VASE_HELD_ITEM 78 // jumps to index 156 on shopItemsAnimationScripts
#define RED_VASE_HELD_ITEM 79 // really small block of 2 elements, breaks on index 157 on shopItemsAnimationScripts
#define RICE_CAKE 80 // returns to index 58 on shopItemsAnimationScripts
#define BREAD 81
#define EGGS 82
#define FISH_FILLET_HELD_ITEM 83
#define SALAD_HELD_ITEM 84
#define PASTA_HELD_ITEM 85
#define WATERMELON_HELD_ITEM 86 // block ends on index 64 on shopItemsAnimationScripts
#define POWER_NUT 87 // index 211 on shopItemsAnimationScripts
#define DOG_HELD_ITEM 88 // index 65 on shopItemsAnimationScripts ends on index 72
// multiple directions
#define CHICKEN_HELD_ITEM 96 // index 81 on shopItemsAnimationScripts ends on index 88
// multiple directions
#define CHICK_HELD_ITEM 104 // index 89 on shopItemsAnimationScripts ends on index 96
// multiple directions
#define EMPTY_BOTTLE_HELD_ITEM 112 // index 113 on shopItemsAnimationScripts
#define BOTTLE_WITH_WATER_HELD_ITEM 113 // index 114 on shopItemsAnimationScripts
#define BOTTLE_WITH_WINE_HELD_ITEM 114 // index 110 on shopItemsAnimationScripts
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
#define DOG_2_HELD_ITEM 171 // Starts in index 73 on shopItemsAnimationScripts and ends on index 80
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
 #define BABY_HELD_ITEM2 192
// multiple directions
#define BABY_GROWN_HELD_ITEM 194
// multiple directions
#define OPEN_BOOK 202
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
#define MAP_HELD_ITEM 219
#define PENDANT_HELD_ITEM 220
#define GOLDEN_EGG_HELD_ITEM 221 // needs to add PACK_ANIM_DATA(159, ANIM_TYPE_LOOP, 0), on slot 251 and PACK_ANIM_DATA(160, ANIM_TYPE_LOOP, 0), on slot 252 from shopItemsAnimationScripts it needs to be forced on make to take the change on that file
#define BABY_HELD_ITEM3 222
#define PLAYER_HELD_ITEM 223
#define PLAYER_HELD_ITEM2 224

/* bottle contents */
#define EMPTY 0

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