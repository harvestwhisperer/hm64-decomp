#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonWriteWordTo);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonReadWordAt);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonMemcpy);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonCopyWords);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonReadMem);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonWriteMem);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonListProcesses);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonLoadProgram);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonGetExeName);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonGetRegionCount);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonmem", __rmonGetRegions);

// strings
//"imem"
INCLUDE_RODATA(const s32, "lib/os/libultra/rmon/rmonmem", D_801240D8);

// "rmon"
INCLUDE_RODATA(const s32, "lib/os/libultra/rmon/rmonmem", D_801240E0);
