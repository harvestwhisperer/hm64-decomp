#ifndef _INTERPOLATOR_H_
#define _INTERPOLATOR_H_

#include "common.h"

typedef struct {
    s16 rate;
    s16 frameRate;
    s16 accumulatedValue;
    s16 unused;
    s32 currentValue;
    s32 targetValue;
} Interpolator;

extern void setupInterpolator(Interpolator* interpolator, u32 current, u32 target, s16 arg3, s16 arg4);
extern bool interpolateToTarget(Interpolator* interpolator);
extern void initializeInterpolator(Interpolator* interpolator, s16, s16);
extern s16 calculateInterpolatorStep(Interpolator* interpolator);
extern s16 getInterpolatorValue(Interpolator* interpolator);

#endif