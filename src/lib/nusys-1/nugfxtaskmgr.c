#include <nusys.h>

static NUScTask* nuGfxTask_ptr;

static short taskDoneMsg;
static short swapBufMsg;
static OSThread	GfxTaskMgrThread;		
static s32 D_801317B8[2];
static u64 GfxTaskMgrStack[NU_GFX_TASKMGR_STACK_SIZE/8];
static OSMesg nuGfxTaskMgrMesgBuf[NU_GFX_TASKMGR_MESGS];

NUUcode* nuGfxUcode;
NUScTask nuGfxTask[NU_GFX_TASK_NUM];	
volatile u32 nuGfxTaskSpool;		
OSMesgQueue	nuGfxTaskMgrMesgQ;

u16** nuGfxCfb;		
u32 nuGfxCfbNum = 1;	
u16* nuGfxCfb_ptr;	
u16* nuGfxZBuffer;	
u32 nuGfxDisplay;	
u32 nuGfxCfbCounter;

NUGfxSwapCfbFunc nuGfxSwapCfbFunc = NULL; 
NUGfxTaskEndFunc nuGfxTaskEndFunc = NULL; 

void nuGfxTaskMgr(void *arg) {

    NUScMsg*	mesg_type;
    NUScTask*	gfxTask;
    OSIntMask	mask;
    
    osCreateMesgQueue(&nuGfxTaskMgrMesgQ, nuGfxTaskMgrMesgBuf, NU_GFX_MESGS);

    while (1) {

		(void)osRecvMesg(&nuGfxTaskMgrMesgQ,(OSMesg*)&gfxTask, OS_MESG_BLOCK);
		
		mesg_type = gfxTask->msg;

		switch(*mesg_type) {
	
			case NU_SC_SWAPBUFFER_MSG:
				
				if (nuGfxSwapCfbFunc != NULL) {
					(*nuGfxSwapCfbFunc)((void*)gfxTask);
				}

				if (nuGfxDisplay == NU_GFX_DISPLAY_ON) {
					osViBlack(FALSE);
				}

				mask = osSetIntMask(OS_IM_NONE);
				nuGfxTaskSpool--;
				osSetIntMask(mask);
				
				break;
				
			case NU_SC_GTASKEND_MSG:	

				if (nuGfxTaskEndFunc != NULL) {
					(*nuGfxTaskEndFunc)((void*)gfxTask);
				}
	
				mask = osSetIntMask(OS_IM_NONE);
				nuGfxTaskSpool--;
				osSetIntMask(mask);
	
				break;
	
			}

    }
}

void nuGfxTaskMgrInit(void) {

    u32	cnt;

    taskDoneMsg	= NU_SC_GTASKEND_MSG;
    swapBufMsg	= NU_SC_SWAPBUFFER_MSG;
    nuGfxTaskSpool = 0;
    nuGfxDisplayOff();		
    
    osCreateThread(&GfxTaskMgrThread, NU_GFX_TASKMGR_THREAD_ID, nuGfxTaskMgr,
		   (void*)NULL,
		   (GfxTaskMgrStack + NU_GFX_TASKMGR_STACK_SIZE/8),
		   NU_GFX_TASKMGR_THREAD_PRI );

    osStartThread(&GfxTaskMgrThread);
    
    for (cnt = 0; cnt < NU_GFX_TASK_NUM; cnt++) {
	 
		nuGfxTask[cnt].next = &nuGfxTask[cnt+1];
		nuGfxTask[cnt].msgQ	= &nuGfxTaskMgrMesgQ;
		nuGfxTask[cnt].list.t.type = M_GFXTASK;
		nuGfxTask[cnt].list.t.flags = 0x00;
		nuGfxTask[cnt].list.t.ucode_boot = (u64*)rspbootTextStart;
		nuGfxTask[cnt].list.t.ucode_boot_size = ((s32) rspbootTextEnd - (s32) rspbootTextStart);
		nuGfxTask[cnt].list.t.ucode_size = SP_UCODE_SIZE;
		nuGfxTask[cnt].list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
		nuGfxTask[cnt].list.t.dram_stack = (u64*) nuDramStack;
		nuGfxTask[cnt].list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
		nuGfxTask[cnt].list.t.output_buff = (u64*)&nuRDPOutputBuf[0];
		nuGfxTask[cnt].list.t.output_buff_size = (u64*)(nuRDPOutputBuf + NU_GFX_RDP_OUTPUTBUFF_SIZE);
		nuGfxTask[cnt].list.t.yield_data_ptr = (u64*) nuYieldBuf;
		nuGfxTask[cnt].list.t.yield_data_size = OS_YIELD_DATA_SIZE;
		nuGfxTask[cnt].msgQ	= &nuGfxTaskMgrMesgQ;	
		
     }

     nuGfxTask[NU_GFX_TASK_NUM-1].next = &nuGfxTask[0];
     nuGfxTask_ptr	= &nuGfxTask[0];

 }

 void nuGfxTaskStart(Gfx *gfxList_ptr, u32 gfxListSize, u32 ucode, u32 flag) {

     u32	framebuf_no;
     OSIntMask	mask;
     
     nuGfxTask_ptr->list.t.data_ptr	= (u64*)gfxList_ptr;
     nuGfxTask_ptr->list.t.data_size = gfxListSize;
     nuGfxTask_ptr->list.t.ucode = nuGfxUcode[ucode].ucode;
     nuGfxTask_ptr->list.t.ucode_data = nuGfxUcode[ucode].ucode_data;
     nuGfxTask_ptr->flags = flag;
     nuGfxTask_ptr->framebuffer = (u16*)nuGfxCfb_ptr;
     
     if (flag & NU_SC_SWAPBUFFER) {
		
		nuGfxTask_ptr->msg = (OSMesg)&swapBufMsg;
		
		nuGfxCfbCounter++;
		framebuf_no = nuGfxCfbCounter % nuGfxCfbNum;
		nuGfxCfb_ptr = nuGfxCfb[framebuf_no];
     
	 } else {	 
	 	nuGfxTask_ptr->msg = (OSMesg)&taskDoneMsg;
	 }

     mask = osSetIntMask(OS_IM_NONE);
     nuGfxTaskSpool++;
     osSetIntMask(mask);
     
     osWritebackDCacheAll();
     osSendMesg(&nusched.graphicsRequestMQ, (OSMesg*)nuGfxTask_ptr, OS_MESG_BLOCK);
     
     nuGfxTask_ptr = nuGfxTask_ptr->next;
     
}
