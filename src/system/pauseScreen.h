#ifndef _PAUSE_SCREEN_H_
#define _PAUSE_SCREEN_H_

#include "common.h"

typedef struct {
    void *romAddr1;
    void *romAddr2;
    void *romAddr3;
    void *romAddr4;
    void *vaddr1;
    void *vaddr2;
    void *vaddr3;
    void *vaddr4;
    u32 unk_20;
    u16 specialItemPages;
    u8 unk_26;
    u8 unk_27;
    Vec3f unk_28;
    u32 value;
    u16 unk_38;
    u8 count;
    u8 unk_3B;
    u16 flags;
} PauseScreenSprites;

#endif