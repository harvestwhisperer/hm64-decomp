#ifndef _LEVEL_INTERACTIONS_H_
#define _LEVEL_INTERACTIONS_H_

#include "common.h"

typedef struct {
    u8 mapAdditionsIndex;
    u8 interactionSfxIndex;
} LevelInteractionsInfo;

extern u8 func_800ACDF4(u16 mapIndex);
extern bool checkWineBarrelInteraction(u16);
extern bool func_800ACE50(u16 mapIndex);
extern bool func_800ACFE8(u16 mapIndex);   
extern bool func_800AD0C4(u16 mapIndex);
extern bool func_800AD1D0(u16 mapIndex);

extern LevelInteractionsInfo levelInteractionsInfo;

#endif