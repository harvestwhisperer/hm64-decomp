
#ifndef _MAIN_PROC_H_
#define _MAIN_PROC_H_

#include "common.h"

extern volatile u8 D_8013DC30;
extern volatile u16 unknownFlag;
extern volatile u8 drawnFrameCount;
extern volatile u8 retraceCount;
extern volatile u8 loopStepsPerCycle;
extern volatile u8 framebufferCount;
extern volatile u8 D_801C4215;
extern volatile u8 currentFramebufferIndex;
extern u16 gMainMapIndex;
extern volatile u8 D_80204B38;
extern volatile u8 stepMainLoop;
extern volatile u32 pendingGfxNum;
extern volatile u8 D_802226E2;
extern volatile u8 D_80222730;
extern volatile u8 frameRate;
extern volatile u8 D_80237A04;
extern volatile u8 frameCount;

extern volatile u32 gDisplayContextIndex;
extern volatile u8 gfxTaskNo;

#endif