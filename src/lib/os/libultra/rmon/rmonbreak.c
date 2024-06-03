#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonSetBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", _rmonListBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonClearBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonGetBranchTarget);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonSetSingleStep);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonGetExceptionStatus);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonHitBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonHitSpBreak);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", __rmonHitCpuFault);

INCLUDE_ASM(const s32, "lib/os/libultra/rmon/rmonbreak", rmonFindFaultedThreads);
