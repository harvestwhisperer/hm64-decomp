#ifndef _SHOP_H_
#define _SHOP_H_

#include "common.h"

extern void func_800DC750(u8);
extern void func_800DC7BC(u8);                            
extern void func_800DC9C0(u8);
extern void func_800DC9FC(u8);    
extern u8 func_800DCAA0(u8 index);
extern u8 func_800DDDFC(u16);

// D_8023740A
typedef struct {
    u16 unk_0;
    u8 quantity;
    u8 unk_3; // index into dialogues for shops: 1 = item description, 2 = purchasing, 5 = decline/accept, 7 = not enough money, 0xFF = done
    u8 storeItemIndex;
    u8 unk_5; // buy/don't buy
} ShopContext;

extern ShopContext shopContext;

extern u8 flowerShopPoints;
extern u8 bakeryCardPoints;

#endif