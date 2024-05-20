#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osTimerServicesInit);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osTimerInterrupt);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osSetTimerIntr);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osInsertTimer);
