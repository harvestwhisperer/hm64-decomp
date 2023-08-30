#include "common.h"

#include "system/audio.h"

// forward declaration
void func_800267A4(Volume* volumePtr, s16 arg1, s16 arg2);
s16 func_800267B4(Volume* volumePtr);       

//INCLUDE_ASM(const s32, "system/audioUtils", func_800266C0);

void func_800266C0(Volume* volumePtr, u32 mainVolume, u32 maxVolume, s16 arg3, s16 arg4) {
    volumePtr->mainVolume = mainVolume;
    volumePtr->maxVolume = maxVolume;
    func_800267A4(volumePtr, arg3, arg4);
}

// alternate:
/*
void func_800266C0(u32 arg0[], s16 *arg1, s16 *arg2, s16 arg3, s16 arg4) {
    arg0[2] = arg1;
    arg0[3] = arg2;
    func_800267A4(arg0, arg3, arg4);
}
*/

//INCLUDE_ASM(const s32, "system/audioUtils", func_800266F8);

u32 func_800266F8(Volume* volumePtr) {

    u32 result;
    s32 temp;

    result = 0;
    
    temp = func_800267B4(volumePtr);

    if (volumePtr->mainVolume < volumePtr->maxVolume) {
        volumePtr->mainVolume += temp;
        if (volumePtr->mainVolume >= volumePtr->maxVolume) {
            volumePtr->unk_0 = 0;
            volumePtr->mainVolume = volumePtr->maxVolume;
        } else {
            result = 1;
            
        }
    }

    if (volumePtr->mainVolume > volumePtr->maxVolume) {
        volumePtr->mainVolume -= temp;
        if (!(volumePtr->maxVolume < volumePtr->mainVolume)) {
            volumePtr->unk_0 = 0;
            volumePtr->mainVolume = volumePtr->maxVolume;
        } else {
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/audioUtils", func_800267A4);

void func_800267A4(Volume* volumePtr, s16 arg1, s16 arg2) {
    volumePtr->unk_0 = arg1;
    volumePtr->unk_2 = arg2;
    volumePtr->unk_4 = 0;
}

//INCLUDE_ASM(const s32, "system/audioUtils", func_800267B4);

s16 func_800267B4(Volume* volumePtr) {

    s16 temp = volumePtr;
    
    if (volumePtr->unk_0 < 0) {
        temp = (volumePtr->unk_2 % (~(volumePtr->unk_0) + 1)) == 0;
        volumePtr->unk_2 += 1;
    } else {
        temp = volumePtr->unk_0;
    }
    
    volumePtr->unk_4 += temp;
    
    return temp;
}

//INCLUDE_ASM(const s32, "system/audioUtils", func_8002683C);

void func_8002683C(Volume* volumePtr, s16 arg1) {
    volumePtr->unk_4 = arg1;
}

//INCLUDE_ASM(const s32, "system/audioUtils", func_80026844);

s16 func_80026844(Volume *volumePtr) {
    return volumePtr->unk_4;
}

// alternate if used with a different/generic struct (seems to be used by dialogue box struct)
/*
s16 func_80026844(void *arg0) {
    return *(s16 *)(arg0+4);
}
*/
