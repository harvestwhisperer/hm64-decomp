#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/initialize", osInitialize);

INCLUDE_ASM(const s32, "lib/os/libultra/initialize", initialize_text_01D0);

INCLUDE_ASM(const s32, "lib/os/libultra/initialize", initialize_text_01E8);

INCLUDE_ASM(const s32, "lib/os/libultra/initialize", createSpeedParam);

INCLUDE_RODATA(const s32, "lib/os/libultra/initialize", initialize_data_0000);

INCLUDE_RODATA(const s32, "lib/os/libultra/initialize", initialize_data_0004);

INCLUDE_RODATA(const s32, "lib/os/libultra/initialize", osViClock);

INCLUDE_RODATA(const s32, "lib/os/libultra/initialize", __osShutdown);

INCLUDE_RODATA(const s32, "lib/os/libultra/initialize", __OSGlobalIntMask);
