#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", __rmonSendHeader);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", __rmonSendReply);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", __rmonSendData);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmain", rmonMain);

INCLUDE_RODATA(const s32, "lib/os/libultra/rmon/rmonmain", rmonmain_data_0000);
