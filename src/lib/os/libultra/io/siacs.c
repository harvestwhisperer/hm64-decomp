#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/io/siacs", __osSiCreateAccessQueue);

INCLUDE_ASM(const s32, "lib/os/libultra/io/siacs", __osSiGetAccess);

INCLUDE_ASM(const s32, "lib/os/libultra/io/siacs", __osSiRelAccess);

INCLUDE_RODATA(const s32, "lib/os/libultra/io/siacs", __osSiAccessQueueEnabled);
