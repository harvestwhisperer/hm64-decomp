#include "common.h"
#include "PR/os.h"

#include "memory.h"

OSPiHandle SramHandle;

//INCLUDE_ASM(const s32, "system/memory", sramInit);

inline OSPiHandle* sramInit(void) {
    
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

//INCLUDE_ASM(const s32, "system/memory", func_8004DC48);

void func_8004DC48(void* devAddr, void* dramAddr, u32 size) {

    OSPiHandle* handle = sramInit();
    OSIoMesg osIoMesg;
    OSMesgQueue queue;
    OSMesg mesg;

    osCreateMesgQueue(&queue, &mesg, 1);
    osInvalDCache(dramAddr, size);

    osIoMesg.hdr.pri = 0;
    osIoMesg.hdr.retQueue = &queue;
    osIoMesg.dramAddr = dramAddr;
    osIoMesg.devAddr = devAddr;
    osIoMesg.size = size;

    osEPiStartDma(handle, &osIoMesg, OS_READ);
    osRecvMesg(&queue, &mesg, 1);
    
}

//INCLUDE_ASM(const s32, "system/memory", func_8004DD7C);

void func_8004DD7C(void *devAddr, void *dramAddr, u32 size) {

    OSPiHandle *handle = sramInit();
    OSIoMesg osIoMesg; 
    OSMesgQueue queue;
    OSMesg mesg;

    osCreateMesgQueue(&queue, &mesg, 1);
    osWritebackDCache(dramAddr, size);

    osIoMesg.hdr.pri = 0;
    osIoMesg.hdr.retQueue = &queue;
    osIoMesg.dramAddr = dramAddr;
    osIoMesg.devAddr = devAddr;
    osIoMesg.size = size;
    
    osEPiStartDma(handle, &osIoMesg, OS_WRITE);
    osRecvMesg(&queue, &mesg, OS_MESG_BLOCK);
    
}