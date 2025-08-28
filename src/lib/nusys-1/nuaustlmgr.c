#include "common.h"

#include <nusys.h>
#include <n_libaudio_sc.h>
#include <n_libaudio_sn_sc.h>
#include <nualstl_n.h>

typedef struct {
    ALLink	node;		
    int	startAddr;	
    u32	lastFrame;	
    char*	ptr;		
} NUDMABuffer;

typedef struct {
    u8		initialized;  
    NUDMABuffer*  firstUsed;    
    NUDMABuffer*  firstFree;    
} NUDMAState;

extern OSThread	auMgrThread;
extern u64		auMgrStack[NU_AU_STACK_SIZE];

extern Acmd*    auCmdList_ptr[2]; 
extern NUScTask	auTask[2];		
extern s16*     auBuffer_ptr[3];	


extern OSMesgQueue	auDmaMesgQ;
extern OSMesg		auDmaMesgBuf[NU_AU_DMA_QUEUE_NUM];
extern OSIoMesg		auDmaIOMesgBuf[NU_AU_DMA_QUEUE_NUM];
extern NUDMAState	auDmaState;
extern NUDMABuffer	auDmaBuf[NU_AU_DMA_BUFFER_NUM];
extern s32			auDmaNext;

static ALDMAproc auDmaNew(NUDMAState** state);
static void nuAuMgr(void* arg);

u32			    nuAuFrameCounter = 0;	
ALHeap		    nuAuHeap;			
N_ALGlobals		auGlobal;
NUAuSeqPlayer	nuAuSeqPlayer[2];
ALBankFile*     nuAuSeqBank_ptr;
extern ALSeqFile*      nuAuSeqFile_ptr;
extern ALSndPlayer		nuAuSndPlayer;
ALBankFile*     nuAuSndBank_ptr = NULL;
ALSndId*        nuAuSndId = NULL;
u8			    nuAuTaskStop = 1;
u8			    nuAuPreNMI = 0;
NUAuPreNMIFunc	nuAuPreNMIFunc = NULL;

static s32 auDmaCallBack(s32 addr, s32 len, void *state) {
    
    void*	freeBuffer;
    int		delta;
    NUDMABuffer* dmaPtr;
    NUDMABuffer* lastDmaPtr;
    s32		addrEnd,buffEnd;

#ifdef USE_EPI
    OSPiHandle*	cartHandle;
    OSIoMesg*	dmaIoMesgBufPtr;
#endif 

    lastDmaPtr = 0;
    dmaPtr = auDmaState.firstUsed;
    addrEnd = addr+len;

    while (dmaPtr) {
	
        buffEnd = dmaPtr->startAddr + NU_AU_DMA_BUFFER_SIZE;

        
        if (dmaPtr->startAddr > addr) {	
        
            break;                   
        
        } else if (addrEnd <= buffEnd) {      
                    
            dmaPtr->lastFrame = nuAuFrameCounter;
            freeBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;
            
            return (int) osVirtualToPhysical(freeBuffer);

        }

            lastDmaPtr = dmaPtr;
            dmaPtr = (NUDMABuffer*)dmaPtr->node.next;

    }

    dmaPtr = auDmaState.firstFree;

    if (dmaPtr == NULL) return(int)OS_K0_TO_PHYSICAL(auDmaState.firstUsed->ptr);
    
    auDmaState.firstFree = (NUDMABuffer*)dmaPtr->node.next;
    alUnlink((ALLink*)dmaPtr);
    
    if (lastDmaPtr) {
	
	
	    alLink((ALLink*)dmaPtr,(ALLink*)lastDmaPtr);
	
    } else if (auDmaState.firstUsed) {
	
	
        lastDmaPtr = auDmaState.firstUsed;
        auDmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = (ALLink*)lastDmaPtr;
        dmaPtr->node.prev = 0;
        lastDmaPtr->node.prev = (ALLink*)dmaPtr;
        
    } else {
	
        
        auDmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = 0;
        dmaPtr->node.prev = 0;
	
    }

#ifdef USE_EPI
    
    cartHandle = osCartRomInit();
    dmaIoMesgBufPtr = &auDmaIOMesgBuf[auDmaNext];
    auDmaNext++;
    
    freeBuffer = dmaPtr->ptr;
    delta = addr & 0x1;
    addr -= delta;            
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = nuAuFrameCounter;
    
    dmaIoMesgBufPtr->hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBufPtr->hdr.retQueue = &auDmaMesgQ;
    dmaIoMesgBufPtr->dramAddr     = freeBuffer;
    dmaIoMesgBufPtr->devAddr      = (u32)addr;
    dmaIoMesgBufPtr->size         = NU_AU_DMA_BUFFER_SIZE;
    osEPiStartDma(cartHandle, dmaIoMesgBufPtr, OS_READ);

#else
    
    freeBuffer = dmaPtr->ptr;
    delta = addr & 0x1;
    addr -= delta;            
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = nuAuFrameCounter;  

    
    osPiStartDma(&auDmaIOMesgBuf[auDmaNext++], OS_MESG_PRI_NORMAL, OS_READ,
		 (u32)addr, freeBuffer, NU_AU_DMA_BUFFER_SIZE,&auDmaMesgQ);
#endif 
    
    return (int) osVirtualToPhysical(freeBuffer) + delta;

}

static ALDMAproc auDmaNew(NUDMAState **state) {

    int         i;
    
    if (!auDmaState.initialized) {
	
        auDmaState.firstFree = &auDmaBuf[0];	
        auDmaState.firstUsed = NULL;              
        auDmaBuf[0].node.prev = NULL;
        auDmaBuf[0].node.next = NULL;
	
	
        for (i = 0; i < NU_AU_DMA_BUFFER_NUM - 1; i++) {
            alLink((ALLink*)&auDmaBuf[i+1], (ALLink*)&auDmaBuf[i]);
            auDmaBuf[i].ptr = alHeapAlloc(&nuAuHeap, 1, NU_AU_DMA_BUFFER_SIZE);
        }
        
        auDmaBuf[i].ptr = alHeapAlloc(&nuAuHeap, 1, NU_AU_DMA_BUFFER_SIZE);
        
        auDmaState.initialized++;
    }

    auDmaNext = 0;
    *state = &auDmaState;  
    
    return auDmaCallBack;

}

void auCleanDMABuffers(void) {
    
    NUDMABuffer *dmaPtr,*nextPtr;

    dmaPtr = auDmaState.firstUsed;
    
    while (dmaPtr) {
	
	nextPtr = (NUDMABuffer*)dmaPtr->node.next;
	
  	if (dmaPtr->lastFrame + NU_AU_BUFFER_USE_FRAME  <nuAuFrameCounter) {

	    if (auDmaState.firstUsed == dmaPtr) {
		    auDmaState.firstUsed = (NUDMABuffer*)dmaPtr->node.next;
	    }
        alUnlink((ALLink*)dmaPtr);

        if (auDmaState.firstFree) {
            alLink((ALLink*)dmaPtr,(ALLink*)auDmaState.firstFree);

        } else {

            auDmaState.firstFree = dmaPtr;
            dmaPtr->node.next = 0;
            dmaPtr->node.prev = 0;
      
        }
	}

	dmaPtr = nextPtr;
    
    }
    
    auDmaNext=0;
    nuAuFrameCounter++;

}

INCLUDE_ASM("asm/nonmatchings/lib/nusys-1/nuaustlmgr", nuAuStlMgrInit);

INCLUDE_ASM("asm/nonmatchings/lib/nusys-1/nuaustlmgr", nuAuMgr);
