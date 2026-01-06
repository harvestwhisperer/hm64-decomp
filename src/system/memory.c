#include "common.h"
#include <PR/os.h>

#include "system/memory.h"

OSPiHandle sramHandle;

//INCLUDE_ASM("asm/nonmatchings/system/memory", sramInit);

inline OSPiHandle* sramInit(void) {
    
    if (sramHandle.baseAddress == PHYS_TO_K1(SRAM_START_ADDR)) {
        return (&sramHandle);
    }
    
    sramHandle.type = 3;
    sramHandle.baseAddress = PHYS_TO_K1(SRAM_START_ADDR);
    sramHandle.latency = (u8)SRAM_LATENCY;
    sramHandle.pulse = (u8)SRAM_PULSE;
    sramHandle.pageSize = (u8)SRAM_PAGE_SIZE;
    sramHandle.relDuration = (u8)SRAM_REL_DURATION;
    sramHandle.domain = PI_DOMAIN2;
    
    sramHandle.speed = 0;
    
    bzero((void*)&sramHandle.transferInfo, sizeof(sramHandle.transferInfo));
    
    osEPiLinkHandle(&sramHandle);

    return (&sramHandle);

}

//INCLUDE_ASM("asm/nonmatchings/system/memory", sramLoad);

void sramLoad(void* devAddr, void* dramAddr, u32 size) {

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

//INCLUDE_ASM("asm/nonmatchings/system/memory", sramWrite);

void sramWrite(void *devAddr, void *dramAddr, u32 size) {

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