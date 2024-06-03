#include "common.h"

// FIXME: nuaudma.c + nuaumgr.c

// nuAuDmaCallBack
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumisc", func_800F2820);

// nuAuDmaNew
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumisc", func_800F29C8);

// nuAuCleanDMABuffers
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumisc", func_800F2B08);


// unaligned file split

// nuAuMgrInit
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumisc", nuAuMgrInit);

// nuAuMgr
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumisc", func_800F2E44);

INCLUDE_RODATA(const s32, "lib/nusys-1/nuaumisc", D_8011E590);
