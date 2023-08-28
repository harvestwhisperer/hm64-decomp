#include "common.h"
#include "PR/os.h"

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
    
    bzero((void*)&SramHandle.transferInfo, sizeof(SramHandle.transferInfo));
    
    osEPiLinkHandle(&SramHandle);

    return (&SramHandle);
}

INCLUDE_ASM(const s32, "system/memory", func_8004DC48);

#ifdef PERMUTER
void func_8004DD7C(void *dramAddr, void *devAddr, u32 size) {

    OSPiHandle *ptr = &D_80181B18;
    OSIoMesg osIoMesg;
    OSMesgQueue queue;
    OSMesg mesg;

    if (ptr->baseAddress != PHYS_TO_K1(SRAM_START_ADDR)) {
        ptr->type = 3;
        ptr->latency = 5;
        ptr->pulse = 12;
        ptr->pageSize = 13;
        ptr->relDuration = 2;
        ptr->baseAddress = PHYS_TO_K1(SRAM_START_ADDR);
        ptr->domain = 1;
        ptr->speed = 0;
        _bzero((void*)&ptr->transferInfo, sizeof(ptr->transferInfo));
        osEPiLinkHandle(ptr);
    }

    osCreateMesgQueue(&queue, &mesg, 1);
    osWritebackDCache(devAddr, size);

    osIoMesg.hdr.pri = 0;
    osIoMesg.hdr.retQueue = &queue;
    osIoMesg.dramAddr = devAddr;
    osIoMesg.devAddr = dramAddr;
    osIoMesg.size = size;
    
    osEPiStartDma(ptr, &osIoMesg, 1);
    osRecvMesg(&queue, &mesg, 1);
    
}
#else
INCLUDE_ASM(const s32, "system/memory", func_8004DD7C);
#endif