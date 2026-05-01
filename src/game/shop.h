#ifndef _SHOP_H_
#define _SHOP_H_

#include "common.h"

#define TOTAL_SHOP_ITEMS 56

/*
 *    0 -  10  Flower Shop
 *   11 -  18  Bakery
 *   19 -  25  Rick Store
 *   26 -  29  Ranch Store
 *   30 -  33  Animal purchase menu
 *   34 -  36  Potion Shop
 *   37 -  39  Souvenir Shop
 *   40 -  44  Tavern menu
 *   45 -  52  Festival vendors
 *   53 -  55  Flower Festival / Fireworks Festival vendors
 */

/* Flower shop */
#define FLOWER_SHOP_TURNIP_SEEDS_ITEM      0
#define FLOWER_SHOP_POTATO_SEEDS_ITEM      1
#define FLOWER_SHOP_CABBAGE_SEEDS_ITEM     2
#define FLOWER_SHOP_GRASS_SEEDS_ITEM       3
#define FLOWER_SHOP_TOMATO_SEEDS_ITEM      4
#define FLOWER_SHOP_CORN_SEEDS_ITEM        5
#define FLOWER_SHOP_EGGPLANT_SEEDS_ITEM    6
#define FLOWER_SHOP_STRAWBERRY_SEEDS_ITEM  7
#define FLOWER_SHOP_MOON_DROP_SEEDS_ITEM   8
// unused
#define FLOWER_SHOP_BLUE_MIST_SEEDS_ITEM  10

/* Bakery */
#define BAKERY_CAKE_ITEM 11
#define BAKERY_PIE_ITEM  12
#define BAKERY_COOKIES 13
#define BAKERY_CAKE 14
#define BAKERY_PIE 15
#define BAKERY_TEA 16
#define BAKERY_WATER 17

/* Rick's store */
#define RICK_STORE_MILKER_ITEM          19
#define RICK_STORE_BRUSH_ITEM           20
#define RICK_STORE_CLIPPERS_ITEM        21
#define RICK_STORE_BLUE_FEATHER_ITEM    22
#define RICK_STORE_RUG_ITEM             23
#define RICK_STORE_CABINET_ITEM         24
#define RICK_STORE_ICE_CREAM_MAKER_ITEM 25

/* Ranch store */
#define RANCH_STORE_CHICKEN_FEED_ITEM   26
#define RANCH_STORE_MIRACLE_POTION_ITEM 27
#define RANCH_STORE_COW_MEDICINE_ITEM   28
#define RANCH_STORE_FODDER_ITEM         29

/* Animal purchase menu */
#define RANCH_STORE_CHICKEN_ITEM 30
#define RANCH_STORE_SHEEP_ITEM   31
#define RANCH_STORE_COW_ITEM     32
#define RANCH_STORE_SHOP_CANCEL  33

/* Potion shop */
#define POTION_SHOP_CURE_ALL_ITEM        34
#define POTION_SHOP_VITAMIN_ITEM         35
#define POTION_SHOP_STRONG_VITAMINS_ITEM 36

/* Saibara's */
#define SOUVENIR_SHOP_OCARINA_ITEM    37
#define SOUVENIR_SHOP_VASE_EARLY_ITEM 38
#define SOUVENIR_SHOP_BLUE_VASE_ITEM  39

/* Tavern menu */
#define TAVERN_WINE_ITEM  40
#define TAVERN_LIQUOR_ITEM 41
#define TAVERN_BEER_ITEM 42
#define TAVERN_MILK_ITEM 43
#define TAVERN_WATERN_ITEM 44

/* Races */
#define HORSE_RACE_TEA 45
#define HORSE_RACE_MILK 46
#define DOG_RACE_TEA 47
#define DOR_RACE_MILK 48
#define RACE_COOKIES 49

/* Festival salls */
#define FESTIVAL_COTTON_CANDY 50
#define FESTIVAL_FRIED_OCTOPUS 51
#define FESTIVAL_ROASTED_CORN 52

/* Flower festival */
#define FLOWER_FESTIVAL_PINK_CAT_MINT_SEEDS_ITEM 53
#define FLOWER_FESTIVAL_EMPTY_BOTTLE_ITEM        54

/* Fireworks festival */
#define FIREWORKS_FESTIVAL_EMPTY_BOTTLE_ITEM     55

// shopContext.mode -- shop dialogue state machine, driven by shopDialogueCallback.
// Transitions documented at shopDialogueCallback in shop.c.
#define SHOP_MODE_OPEN_DESCRIPTION                  0  // open the item-description message box
#define SHOP_MODE_OPEN_PURCHASE_PROMPT              1  // after description closed, bring up the quantity widget + purchase prompt
#define SHOP_MODE_AWAIT_QUANTITY_INPUT              2  // player adjusts quantity / picks Yes / No
#define SHOP_MODE_COMMIT_PURCHASE                   3  // run handlePurchase() and route to a cleanup mode based on outcome
#define SHOP_MODE_DECLINE_PURCHASE                  4  // show the decline message
#define SHOP_MODE_EXIT_DEFAULT                      5  // wait for message box closed, exit overlay, actionPhase = playerShopActionPhase[i]
#define SHOP_MODE_EXIT_SKIP_HELD_DISPLAY            6  // same as EXIT_DEFAULT but actionPhase = 3 (selected by D_801194D8)
#define SHOP_MODE_EXIT_AFTER_INSUFFICIENT_FUNDS     7  // exit after "not enough money" message; actionPhase = playerShopEndActionPhase[i]
#define SHOP_MODE_DONE                           0xFF  // idle / no-op (default-case fall-through)

// D_8023740A
typedef struct {
    u16 itemTextIndex;
    u8 quantity;
    u8 mode; // SHOP_MODE_* state
    u8 storeItemIndex;
    u8 buySelected;
} ShopContext;

extern void intializeShopDialogue(u8 storeItemIndex);
extern void loadShopItemSprite(u8);                            
extern void deactivateShopItemMapObject(u8);
extern void handlePickUpShopItem(u8);    
extern u8 checkCanPickUpShopItem(u8 index);
extern void shopDialogueCallback(void);
extern u8 checkShopItemShouldBeDisplayed(u16);

extern ShopContext shopContext;

extern u8 flowerShopPoints;
extern u8 bakeryCardPoints;

#endif