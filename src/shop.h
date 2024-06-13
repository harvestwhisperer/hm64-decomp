#ifndef _SHOP_H_
#define _SHOP_H_

#include "common.h"

/* shop item indices */
// 0x13-0x19 rick's
// 0x1A-1C = tavern
#define WINE_SHOP_ITEM 0x28
#define WATER_SHOP_ITEM 0x2C

// D_8023740A
typedef struct {
    u16 unk_0;
    u8 quantity;
    u8 unk_3; // index into dialogues for shops: 1 = item description, 2 = purchasing, 5 = decline/accept, 7 = not enough money, 0xFF = done
    u8 storeItemIndex;
    u8 unk_5; // buy/don't buy
} ShopContext;

extern void func_800DC750(u8 storeItemIndex);
extern void loadShopItemSprite(u8);                            
extern void func_800DC9C0(u8);
extern void func_800DC9FC(u8);    
extern u8 func_800DCAA0(u8 index);
extern u8 checkShopItemShouldBeDisplayed(u16);

extern ShopContext shopContext;

extern u8 flowerShopPoints;
extern u8 bakeryCardPoints;

#endif