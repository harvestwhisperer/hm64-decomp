#include "common.h"

#include "memory.h"

extern OSPiHandle SramHandle;

//INCLUDE_ASM(const s32, "system/memory", sramInit);

OSPiHandle* sramInit(void) {
    
    if (SramHandle.baseAddress == PHYS_TO_K1(SRAM_START_ADDR)) {
        return (&SramHandle);
    }
    
    SramHandle.type = 3;
    SramHandle.baseAddress = PHYS_TO_K1(SRAM_START_ADDR);
    SramHandle.latency = (u8)SRAM_LATENCY;
    SramHandle.pulse = (u8)SRAM_PULSE;
    SramHandle.pageSize = (u8)SRAM_PAGE_SIZE;
    SramHandle.relDuration = (u8)SRAM_REL_DURATION;
    SramHandle.domain = PI_DOMAIN2;
    
    SramHandle.speed = 0;
    
    _bzero((void*)&SramHandle.transferInfo, sizeof(SramHandle.transferInfo));
    
    osEPiLinkHandle(&SramHandle);

    return (&SramHandle);
}

INCLUDE_ASM(const s32, "system/memory", func_8004DC48);

INCLUDE_ASM(const s32, "system/memory", func_8004DD7C);
