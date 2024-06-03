#include "common.h"

// nuAuDmaCallBack
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumgr", func_800F2820);

// nuAuDmaNew
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumgr", func_800F29C8);

// nuAuCleanDMABuffers
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumgr", func_800F2B08);

// nuAuMgrInit
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumgr", nuAuMgrInit);

// nuAuMgr
INCLUDE_ASM(const s32, "lib/nusys-1/nuaumgr", func_800F2E44);

// n_aspMainDataStart?
INCLUDE_RODATA(const s32, "lib/nusys-1/nuaumgr", D_8011E590);
