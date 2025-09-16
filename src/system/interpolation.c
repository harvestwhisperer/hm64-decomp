#include "common.h"

#include "system/interpolation.h"

// forward declaration
void initializeInterpolator(Interpolator* interpolator, s16 rate, s16 frameCount);     

//INCLUDE_ASM("asm/nonmatchings/system/interpolator", func_800266C0);

void func_800266C0(Interpolator* interpolator, u32 currentValue, u32 max, s16 rate, s16 frameRate) {
    interpolator->currentValue = currentValue;
    interpolator->targetValue = max;
    initializeInterpolator(interpolator, rate, frameRate);
}

//INCLUDE_ASM("asm/nonmatchings/system/interpolator", interpolateToTarget);

bool interpolateToTarget(Interpolator* interpolator) {

    bool result;
    s32 temp;

    result = FALSE;
    
    temp = func_800267B4(interpolator);

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

//INCLUDE_ASM("asm/nonmatchings/system/interpolator", initializeInterpolator);

void initializeInterpolator(Interpolator* interpolator, s16 rate, s16 frameRate) {
    interpolator->rate = rate;
    interpolator->frameRate = frameRate;
    interpolator->accumulatedValue = 0;
}

//INCLUDE_ASM("asm/nonmatchings/system/interpolator", func_800267B4);

s16 func_800267B4(Interpolator* interpolator) {

    s16 temp = interpolator;
    
    if (interpolator->rate < 0) {
        temp = (interpolator->frameRate % (~(interpolator->rate) + 1)) == 0;
        interpolator->frameRate += 1;
    } else {
        temp = interpolator->rate;
    }
    
    interpolator->accumulatedValue += temp;
    
    return temp;
    
}

//INCLUDE_ASM("asm/nonmatchings/system/interpolator", setAccumulatedInterpolatorValue);

// unused
void setAccumulatedInterpolatorValue(Interpolator* interpolator, s16 value) {
    interpolator->accumulatedValue = value;
}

//INCLUDE_ASM("asm/nonmatchings/system/interpolator", getInterpolatorValue);

s16 getInterpolatorValue(Interpolator* interpolator) {
    return interpolator->accumulatedValue;
}