#include "common.h"

#include "system/interpolation.h"

// forward declaration
void initializeInterpolator(Interpolator* interpolator, s16 rate, s16 frameCount);     

void setupInterpolator(Interpolator* interpolator, u32 currentValue, u32 max, s16 rate, s16 frameRate) {
    interpolator->currentValue = currentValue;
    interpolator->targetValue = max;
    initializeInterpolator(interpolator, rate, frameRate);
}

bool interpolateToTarget(Interpolator* interpolator) {

    bool result;
    s32 temp;

    result = FALSE;
    
    temp = calculateInterpolatorStep(interpolator);

    if (interpolator->currentValue < interpolator->targetValue) {
        
        interpolator->currentValue += temp;

        if (interpolator->currentValue >= interpolator->targetValue) {

            interpolator->rate = 0;
            interpolator->currentValue = interpolator->targetValue;

        } else {
            result = TRUE;
        }

    }

    if (interpolator->currentValue > interpolator->targetValue) {

        interpolator->currentValue -= temp;

        if (!(interpolator->targetValue < interpolator->currentValue)) {

            interpolator->rate = 0;
            interpolator->currentValue = interpolator->targetValue;

        } else {
            result = TRUE;
        }

    }

    return result;

}

void initializeInterpolator(Interpolator* interpolator, s16 rate, s16 frameRate) {
    interpolator->rate = rate;
    interpolator->frameRate = frameRate;
    interpolator->accumulatedValue = 0;
}

s16 calculateInterpolatorStep(Interpolator* interpolator) {

    // ??
    s16 temp = (s16)interpolator;
    
    if (interpolator->rate < 0) {
        temp = (interpolator->frameRate % (~(interpolator->rate) + 1)) == 0;
        interpolator->frameRate += 1;
    } else {
        temp = interpolator->rate;
    }
    
    interpolator->accumulatedValue += temp;
    
    return temp;
    
}

// unused
// void setAccumulatedInterpolatorValue(Interpolator* interpolator, s16 value) {
//     interpolator->accumulatedValue = value;
// }

s16 getInterpolatorValue(Interpolator* interpolator) {
    return interpolator->accumulatedValue;
}