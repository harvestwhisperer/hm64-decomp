#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonsio", __rmonSendFault);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonsio", __gdbIOflush);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonsio", __rmonIOputw);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonsio", __rmonIOhandler);
