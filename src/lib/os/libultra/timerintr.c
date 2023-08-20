#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osTimerServicesInit);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osTimerInterrupt);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", timerintr_text_0074);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", timerintr_text_01F0);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", timerintr_text_0228);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osSetTimerIntr);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", __osInsertTimer);

INCLUDE_ASM(const s32, "lib/os/libultra/timerintr", timerintr_text_0300);
