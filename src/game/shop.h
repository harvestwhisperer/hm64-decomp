#ifndef _SHOP_H_
#define _SHOP_H_

#include "common.h"

#define TOTAL_SHOP_ITEMS 56

/* shop item indices */
// 0x13-0x19 rick's
// 0x1A-1C = tavern
#define WINE_SHOP_ITEM 0x28
#define WATER_SHOP_ITEM 0x2C

// D_8023740A
typedef struct {
    u16 itemTextIndex;
    u8 quantity;
    u8 mode; // index into dialogues for shops: 1 = item description, 2 = purchasing, 5 = decline/accept, 7 = not enough money, 0xFF = done
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