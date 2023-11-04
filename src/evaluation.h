#ifndef _EVALUATION_H_
#define _EVALUATION_H_

#include "common.h"

extern u16 calculateFieldScore();
extern u16 calculateGrassScore();                         
extern u8 calculateHouseExtensionScore();    
extern u16 calculateNPCAffectionScore();               
extern u32 calculateShippingScore();                      

// field score
extern u16 D_8013D438;
// grass score
extern u16 D_80205638;

// shipping score
extern u16 D_80189824;
// combined shipping totals
extern u32 D_801C3E1C;

// sick days total
extern u16 D_801886E0;

// npc affection score
extern u16 D_801C3B62;
// sum of girls' affection
extern u16 D_80215DF0;

// animal scores
extern u8 D_801886D0;
extern u8 D_801D62C6;
extern u8 D_80205209;
// dead animals
extern u16 D_801886A8;

// recipes
extern u8 D_80188F68;

// house extension score
extern u8 D_8017026E;

// fish score
extern u16 D_801D62C4;

#endif