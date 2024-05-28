#ifndef _VOLUME_H_
#define _VOLUME_H_

#include "common.h"

#define MAX_VOLUME 256

typedef struct {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
    s32 mainVolume;
    s32 maxVolume;
} Volume;

extern void func_800266C0(Volume* volumePtr, u32 mainVolume, u32 maxVolume, s16 arg3, s16 arg4);
extern u32 func_800266F8(Volume* volumePtr);
extern void func_800267A4(Volume* volumePtr, s16, s16);   

#endif