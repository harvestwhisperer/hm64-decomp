#include <nusys.h>

void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size) {

    OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
    OSPiHandle*	cartHandle;
    
#ifdef	USE_EPI
    
    cartHandle = osCartRomInit();

    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    
    osInvalDCache((void*)buf_ptr, (s32)size);

    dmaIoMesgBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
    dmaIoMesgBuf.dramAddr     = buf_ptr;
    dmaIoMesgBuf.devAddr      = (u32)rom_addr;
    dmaIoMesgBuf.size         = size;

    osEPiStartDma(cartHandle, &dmaIoMesgBuf, OS_READ);
    
    (void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);
    
#else

    osInvalDCache((void*)buf_ptr, (s32)size);

    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    
    osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr,
		 buf_ptr, size, &dmaMesgQ);

    
    (void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);

#endif 

}

void nuPiReadRom2(u32 rom_addr, void* buf_ptr, u32 size) {

	OSIoMesg	dmaIoMesgBuf;
	OSMesgQueue dmaMesgQ;
	OSMesg	dmaMesgBuf;
	OSPiHandle* cartHandle;

#ifdef	USE_EPI

	cartHandle = osCartRomInit();
	
	osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);

	osInvalDCache((void*)buf_ptr, (s32)size);

	dmaIoMesgBuf.hdr.pri = OS_MESG_PRI_NORMAL;
	dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
	dmaIoMesgBuf.dramAddr = buf_ptr;
	dmaIoMesgBuf.devAddr = (u32)rom_addr;
	dmaIoMesgBuf.size = size;

	osEPiStartDma(cartHandle, &dmaIoMesgBuf, OS_READ);

	(void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);

#else

	osInvalDCache((void*)buf_ptr, (s32)size);
	
	osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
	
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, rom_addr,
		buf_ptr, size, &dmaMesgQ);
	
	(void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);

#endif 
}
