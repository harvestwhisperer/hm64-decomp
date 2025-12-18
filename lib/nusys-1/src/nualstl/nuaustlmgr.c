#include <nusys.h>
#include <n_libaudio_sc.h>
#include <n_libaudio_sn_sc.h>
#include <nualstl_n.h>

/*--------------------------------------*/
/* DMA BUFFER STRUCT		  	    */
/*--------------------------------------*/
typedef struct {
    ALLink	node;		/* link list pointer */
    int	startAddr;	/* address of data in ROM */
    u32	lastFrame;	/* value of last frame counter used */
    char*	ptr;		/* pointer value for buffer */
} NUDMABuffer;
/*----------------------------------------------------*/
/* DMA callback for sequence player				*/
/*   DMA status to be used in routine			*/
/* Structure to store status of use of DMABuffer	*/
/*----------------------------------------------------*/
typedef struct {
    u8		initialized;  /* flag to show if has already been intialised */
    NUDMABuffer*  firstUsed;    /* head of DMABuffer link list in use */
    NUDMABuffer*  firstFree;    /* head of free DMABuffer link list */
} NUDMAState;

static OSThread	auMgrThread;
static u64		auMgrStack[NU_AU_STACK_SIZE/sizeof(u64)];

static Acmd*    auCmdList_ptr[2]; /* pointer of command list */
static NUScTask	auTask[2];		/* audio task buffer */
static s16*     auBuffer_ptr[3];	/* audio buffer */

// unused padding; probably indicates different struct layout
static s32         D_801287F4; 
static OSMesgQueue	auDmaMesgQ;

static OSMesg		auDmaMesgBuf[NU_AU_DMA_QUEUE_NUM];
static OSIoMesg		auDmaIOMesgBuf[NU_AU_DMA_QUEUE_NUM];

static NUDMAState	auDmaState;
// unused padding; probably indicates different struct layout
static s32           D_80128F1C;

static NUDMABuffer	auDmaBuf[NU_AU_DMA_BUFFER_NUM];
static s32			auDmaNext;
// unused padding; probably indicates different struct layout
static s32            D_80129424; 

static ALDMAproc auDmaNew(NUDMAState** state);
static void nuAuMgr(void);

u32			    nuAuFrameCounter = 0;	/* flame counter */
ALHeap		    nuAuHeap;			/* Heap structure */
N_ALGlobals		auGlobal;
NUAuSeqPlayer	nuAuSeqPlayer[2];
ALBankFile*     nuAuSeqBank_ptr;
// unused
// ALSeqFile*      nuAuSeqFile_ptr;
// ALSndPlayer		nuAuSndPlayer;
ALBankFile*     nuAuSndBank_ptr = NULL;
ALSndId*        nuAuSndId = NULL;
u8			    nuAuTaskStop = 1;
u8			    nuAuPreNMI = 0;
NUAuPreNMIFunc	nuAuPreNMIFunc = NULL;

/*----------------------------------------------------------------------*/
/*	DMA CallBack 									*/
/*												*/
/*   DMA callback routine for synthesis driver.  If ther is necessary	*/
/*   value for DMABuffer, return the pointer of the buffer, and if	*/
/*   not, DMA transfer the data and head in the buffer.  By this 		*/
/*   mechanism, the number of DMA calls is reduced.				*/
/*   Unused portion is deleted by 2 frames.					*/
/* IN	addr	ROM address with data							*/
/* 	len	data size									*/
/*----------------------------------------------------------------------*/
static s32 auDmaCallBack(s32 addr, s32 len, void *state)
{
    
    void*	freeBuffer;
    int		delta;
    NUDMABuffer* dmaPtr;
    NUDMABuffer* lastDmaPtr;
    s32		addrEnd,buffEnd;

#ifdef USE_EPI
    OSPiHandle*	cartHandle;
    OSIoMesg*	dmaIoMesgBufPtr;
#endif /* USE_EPI */

    lastDmaPtr = 0;
    dmaPtr = auDmaState.firstUsed;
    addrEnd = addr+len;


	/* Check if desired data is in buffer */    
    while(dmaPtr) {
	buffEnd = dmaPtr->startAddr + NU_AU_DMA_BUFFER_SIZE;

	/*----------------------------------------*/
	/*  As startAddr is aligned in ascending 	*/
	/*  order in DMABuffer, if small value	*/
	/*  should come, Buffer doesn't need data	*/
	/*----------------------------------------*/
	if(dmaPtr->startAddr > addr) {	/* No data in buffer */
	    break;                   
	} else if(addrEnd <= buffEnd){      /* Data is in buffer */
	    
	    /* Set value of current frame counter */	    
	    dmaPtr->lastFrame = nuAuFrameCounter;
	    
	    /* Calculate the address which has necessary data */
	    freeBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;
	    
	    /* Return physical address of buffer */
	    return (int) osVirtualToPhysical(freeBuffer);
	}
	lastDmaPtr = dmaPtr;
	dmaPtr = (NUDMABuffer*)dmaPtr->node.next;
    }
    
    /*------------------------------------------------------------------*/
    /* As DMABuffer doesn't have desired data, set data to be 		*/
    /* transferred by DMA								*/
    /*------------------------------------------------------------------*/
    
    /* Take one empty buffer out of DMABuffer */
    dmaPtr = auDmaState.firstFree;

    
    /* If there's no empty buffer, NULL will be taken. 	*/
    /* Return head pointer of used buffer for now, as it  	*/
    /* is better than stopping					*/
    if(dmaPtr == NULL) return(int)OS_K0_TO_PHYSICAL(auDmaState.firstUsed->ptr);
    
    auDmaState.firstFree = (NUDMABuffer*)dmaPtr->node.next;
    alUnlink((ALLink*)dmaPtr);
    
    /* 
       Insert the aquired DMABuffer into used link
       list so that startAddr is in ascending order
       */
    if(lastDmaPtr) {
	
	/* Insert into link list as usual */
	alLink((ALLink*)dmaPtr,(ALLink*)lastDmaPtr);
	
    } else if(auDmaState.firstUsed) {
	
	/* Insert into head of link list */
	lastDmaPtr = auDmaState.firstUsed;
	auDmaState.firstUsed = dmaPtr;
	dmaPtr->node.next = (ALLink*)lastDmaPtr;
	dmaPtr->node.prev = 0;
	lastDmaPtr->node.prev = (ALLink*)dmaPtr;
	
    } else {
	
	/* If DMABuffer in use doesn't exsist */
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
    addr -= delta;            /* address location is placed in 2 byte boundary of DMA */
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = nuAuFrameCounter;/* set current frame counter */
    
    dmaIoMesgBufPtr->hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIoMesgBufPtr->hdr.retQueue = &auDmaMesgQ;
    dmaIoMesgBufPtr->dramAddr     = freeBuffer;
    dmaIoMesgBufPtr->devAddr      = (u32)addr;
    dmaIoMesgBufPtr->size         = NU_AU_DMA_BUFFER_SIZE;
    osEPiStartDma(cartHandle, dmaIoMesgBufPtr, OS_READ);

#else
    
    freeBuffer = dmaPtr->ptr;
    delta = addr & 0x1;
    addr -= delta;            /* address location is placed in 2 byte boundary of DMA */
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = nuAuFrameCounter;  /* set current frame counter */

    /* DMA transfer */
    osPiStartDma(&auDmaIOMesgBuf[auDmaNext++], OS_MESG_PRI_NORMAL, OS_READ,
		 (u32)addr, freeBuffer, NU_AU_DMA_BUFFER_SIZE,&auDmaMesgQ);
#endif /* USE_EPI */
    
    return (int) osVirtualToPhysical(freeBuffer) + delta;
}


/*----------------------------------------------------------------------*/ 
/*  auDmaNew -  sequence player DMA callback initialize			*/
/*												*/
/*  callback routine which is called once for initialization in 		*/
/*  sequence player.  Next time, callback routine which was returned	*/
/*  by "return" will be called.							*/
/* IN **state	status pointer							*/
/*	        	If pointer is set here, it will be passed to state	*/
/*			of DMA callback.							*/
/*----------------------------------------------------------------------*/ 
static ALDMAproc auDmaNew(NUDMAState **state)
{
    int         i;
    
    /* Check if it is initialized.  If not, initialize */
    if(!auDmaState.initialized) {
	
	auDmaState.firstFree = &auDmaBuf[0];	/* register head of empty DMABuffer list */
	auDmaState.firstUsed = NULL;              /* no DMABuffer in use */
	auDmaBuf[0].node.prev = NULL;
	auDmaBuf[0].node.next = NULL;
	
	/* Create pointer link */
	for (i = 0; i < NU_AU_DMA_BUFFER_NUM - 1; i++){
	    alLink((ALLink*)&auDmaBuf[i+1], (ALLink*)&auDmaBuf[i]);
	    auDmaBuf[i].ptr = alHeapAlloc(&nuAuHeap, 1, NU_AU_DMA_BUFFER_SIZE);
	}
	auDmaBuf[i].ptr = alHeapAlloc(&nuAuHeap, 1, NU_AU_DMA_BUFFER_SIZE);
	/* Set initialize flag */
	auDmaState.initialized++;
    }
    auDmaNext = 0;
    *state = &auDmaState;  /* set pointer of nuDmaState structure in state */
    
    return auDmaCallBack;
}


/*----------------------------------------------------------------------*/ 
/*  auCleanDMABuffers - Clear DMA buffer list					*/
/*												*/
/*  Search DMABuffer and add buffer whose frames aren't used for		*/
/*  more than 2 frames to a link list of empty buffer.			*/
/*												*/
/*	IN 	None										*/
/* 	RTN	None										*/
/*----------------------------------------------------------------------*/ 
void auCleanDMABuffers(void)
{
    
    NUDMABuffer *dmaPtr,*nextPtr;

    dmaPtr = auDmaState.firstUsed;
    
    while(dmaPtr) {
	
	nextPtr = (NUDMABuffer*)dmaPtr->node.next;
	
	/*----------------------------------------------------------------*/
	/*  Add buffer which has passed NU_AU_BUFFER_USE_FRAME to a link	*/
	/*  list of empty buffer.  To increase NU_AU_BUFFER_USE_FRAME,	*/
	/*  it is necessary to increase buffer.  If decreased, the 		*/
	/*  number of DMA use will increase.					*/
	/*----------------------------------------------------------------*/	      

  	if(dmaPtr->lastFrame + NU_AU_BUFFER_USE_FRAME  <nuAuFrameCounter){
	    if(auDmaState.firstUsed == dmaPtr){
		auDmaState.firstUsed = (NUDMABuffer*)dmaPtr->node.next;
	    }
	    alUnlink((ALLink*)dmaPtr);
	    if(auDmaState.firstFree){
		alLink((ALLink*)dmaPtr,(ALLink*)auDmaState.firstFree);
	    }else{
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

void nuAuStlMgrInit(void* heap_ptr, u32 size, ALSynConfig* synConfig)
{
    u32 cnt;

    nuAuTaskStop = NU_AU_TASK_RUN;	/* able to run task */

    /* Initialize audio heap */
    alHeapInit(&nuAuHeap, heap_ptr, size);

    /* Reserve command list buffer */
    auCmdList_ptr[0] = alHeapAlloc(&nuAuHeap, 1, NU_AU_CLIST_SIZE*sizeof(Acmd));
    auCmdList_ptr[1] = alHeapAlloc(&nuAuHeap, 1, NU_AU_CLIST_SIZE*sizeof(Acmd));

    /* Reserve audio buffer */
    auBuffer_ptr[0] = alHeapAlloc(&nuAuHeap, 1, sizeof(s32)*NU_AU_BUFFER_MAX);
    auBuffer_ptr[1] = alHeapAlloc(&nuAuHeap, 1, sizeof(s32)*NU_AU_BUFFER_MAX);
    auBuffer_ptr[2] = alHeapAlloc(&nuAuHeap, 1, sizeof(s32)*NU_AU_BUFFER_MAX);

    /* Create message queue for DMA callback */
    osCreateMesgQueue(&auDmaMesgQ, &auDmaMesgBuf[0], NU_AU_DMA_QUEUE_NUM);


    /* Initialize synthesis driver	*/
    synConfig->dmaproc    = auDmaNew;
    synConfig->heap	  = &nuAuHeap;
    synConfig->outputRate = osAiSetFrequency(synConfig->outputRate);
    alInit(&auGlobal.drvr.pFreeList, synConfig);
    
    /* Initialize the task structure */
    for(cnt = 0; cnt < 2; cnt++){
	auTask[cnt].msg                      = 0;     /* reply with this message */
	auTask[cnt].list.t.type		         = M_AUDTASK;
	auTask[cnt].list.t.ucode_boot	     = (u64*)rspbootTextStart;
	auTask[cnt].list.t.ucode_boot_size   = ((int) rspbootTextEnd - (int) rspbootTextStart);
	auTask[cnt].list.t.ucode	         = (u64*) aspMainTextStart;
	auTask[cnt].list.t.ucode_data        = (u64 *) aspMainDataStart;
	auTask[cnt].list.t.ucode_data_size   = SP_UCODE_DATA_SIZE;
	auTask[cnt].list.t.dram_stack	     = (u64 *) NULL;
	auTask[cnt].list.t.dram_stack_size   = 0;
	auTask[cnt].list.t.output_buff	     = (u64 *) NULL;
	auTask[cnt].list.t.output_buff_size  = 0;
	auTask[cnt].list.t.yield_data_ptr    = NULL;
	auTask[cnt].list.t.yield_data_size   = 0;

	nuAuSeqPlayer[cnt].mode	= 0;
    }

    /* Start audio manager */
    osCreateThread(&auMgrThread, NU_AU_MGR_THREAD_ID, nuAuMgr, (void*)NULL,
		   (auMgrStack + NU_AU_STACK_SIZE/sizeof(u64)),
		   NU_AU_MGR_THREAD_PRI);
    osStartThread(&auMgrThread);

}

static void nuAuMgr(void)
{
    NUScClient	auClient;
    NUScMsg*	mesg_type;
    NUScTask*	auTask_ptr;
    OSMesgQueue	auMesgQ;
    OSMesg		auMsgBuf[NU_AU_MESG_MAX];
    OSMesgQueue auRtnMesgQ;
    OSMesg		auRtnMesgBuf;
    s32		FrameSize, FrameSizeAjst;
    s32		samplesLeft, Samples = 0;    
    u32		auBufCnt, auTaskCnt;
    Acmd*		cmdList_ptr;
    Acmd*		cmdListAfter_ptr;    
    s32		cmdList_len;
    u32		ajstFrame;
    s16*		Buffer_ptr;
    u32		status;
    
    s32 temp;
    
    osCreateMesgQueue(&auMesgQ, auMsgBuf, NU_AU_MESG_MAX);
    
    osCreateMesgQueue(&auRtnMesgQ, &auRtnMesgBuf, 1);
    
    nuScAddClient(&auClient, &auMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG );
    
    temp = (nusched.retraceCount * auGlobal.drvr.maxOutSamples + 59) / 60;
    
    FrameSize = ((s32)temp / NU_AU_NAUDIO_SAMPLES + 1) * NU_AU_NAUDIO_SAMPLES;
    
    temp = FrameSize - temp; 
    temp = (s32)temp < 10;
    
    if(temp)
    {
        FrameSize = FrameSize + NU_AU_NAUDIO_SAMPLES;
    }
    
    FrameSizeAjst = FrameSize - NU_AU_NAUDIO_SAMPLES;
    
    auBufCnt  = 0;
    auTaskCnt = 0;
    cmdList_len  = 0;
    auTask_ptr   = &auTask[0];
    cmdList_ptr  = auCmdList_ptr[0];
    Buffer_ptr	 = auBuffer_ptr[0];
    
    while(1){
	(void)osRecvMesg(&auMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
    
	switch(*mesg_type){
        case NU_SC_RETRACE_MSG:
        status = osAiGetStatus();
        
        if(status & AI_STATUS_FIFO_FULL) {
            ajstFrame = 0;
            continue;
        }
        
        samplesLeft = (u32)osAiGetLength() >> 2;
        
        if(cmdList_len && nuAuTaskStop == NU_AU_TASK_RUN){
            
	    osAiSetNextBuffer(Buffer_ptr, Samples << 2);
        
	    auTask_ptr  = &auTask[auTaskCnt];
	    cmdList_ptr = auCmdList_ptr[auTaskCnt];
	    Buffer_ptr	 = auBuffer_ptr[auBufCnt];	    
	}
	
    
	if( (samplesLeft < (0xC2 + NU_AU_NAUDIO_EXTRASMP)) || ajstFrame )
    {
	    Samples = FrameSize;
	    ajstFrame = 0;
	} else {
	    Samples = FrameSizeAjst;
    ajstFrame = 1;
}

cmdListAfter_ptr = n_alAudioFrame(cmdList_ptr, &cmdList_len,
(s16*)osVirtualToPhysical(Buffer_ptr),
Samples);

if(cmdList_len && nuAuTaskStop == NU_AU_TASK_RUN)
{
    auTask_ptr->list.t.data_ptr  = (u64 *)cmdList_ptr;
    auTask_ptr->list.t.data_size = (cmdListAfter_ptr - cmdList_ptr) * sizeof(Acmd);
    auTask_ptr->msgQ		     = &auRtnMesgQ;
    osSendMesg(&nusched.audioRequestMQ, (OSMesg*)auTask_ptr, OS_MESG_BLOCK);
    auCleanDMABuffers();
    
    osRecvMesg(&auRtnMesgQ, NULL, OS_MESG_BLOCK);
    
    auBufCnt    = (auBufCnt+1) % 3;
    auTaskCnt   ^=1;	    
}

if((u32)nuAuPreNMIFunc && nuAuPreNMI)
{
    (*nuAuPreNMIFunc)(NU_SC_RETRACE_MSG, nuAuPreNMI);
    nuAuPreNMI++;
}
break;
case NU_SC_PRENMI_MSG:
if(nuAuPreNMIFunc)
{
    (*nuAuPreNMIFunc)(NU_SC_PRENMI_MSG, nuAuPreNMI);
}
nuAuPreNMI++;
break;
default:
break;
}
}
}
