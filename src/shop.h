#ifndef _SHOP_H_
#define _SHOP_H_

#include "common.h"

extern void func_800DC750(u8);
extern void func_800DC7BC(u8);                            
extern void func_800DC9C0(u8);
extern void func_800DC9FC(u8);    
extern u8 func_800DCAA0(u8 index);
extern u8 func_800DDDFC(u16);

extern u8 D_8023740C[6];
// buy/don't buy animal
extern u8 D_8023740F;

extern u8 flowerShopPoints;
extern u8 bakeryCardPoints;

#endif