#include "common.h"


INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonMaskIdleThreadInts);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonGetTCB);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonStopUserThreads);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonListThreads);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonGetThreadStatus);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonThreadStatus);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonStopThread);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmontask", __rmonRunThread);
