#include <nusys.h>

#define TASK_YIELD	1		
#define TASK_YIELDED	2		

#define VIDEO_MSG		666		
#define RSP_DONE_MSG	667		
#define RDP_DONE_MSG	668		
#define PRE_NMI_MSG	669		

static void nuScEventHandler(void);
static void nuScEventBroadcast(NUScMsg *msg);
static void nuScExecuteAudio(void);
static void nuScExecuteGraphics(void);
static void nuScWaitTaskReady(NUScTask *task);

static u64 nuScStack[NU_SC_STACK_SIZE/8];	  
static u64 nuScAudioStack[NU_SC_STACK_SIZE/8];	  
static u64 nuScGraphicsStack[NU_SC_STACK_SIZE/8];

NUSched nusched;	
NUScPreNMIFunc nuScPreNMIFunc = NULL;	

u32 nuScRetraceCounter; 

#ifdef NU_DEBUG

NUDebTaskPerf nuDebTaskPerf[3];
NUDebTaskPerf* nuDebTaskPerfPtr;
volatile u32 nuDebTaskPerfCnt = 0;
volatile u32 nuDebTaskPerfEnd = 1;
static NUDebTaskPerf* debTaskPerfPtr;

#endif 

void nuScCreateScheduler(u8 videoMode, u8 numFields) {
	
	nusched.curGraphicsTask = NULL;
	nusched.curAudioTask = NULL;
	nusched.graphicsTaskSuspended = NULL;
	nusched.clientList   = NULL;
	nusched.retraceMsg   = NU_SC_RETRACE_MSG;
	nusched.prenmiMsg    = NU_SC_PRENMI_MSG;
	nusched.retraceCount	  = numFields;

#ifdef	NU_DEBUG
	debTaskPerfPtr = &nuDebTaskPerf[0];
	debTaskPerfPtr->retraceTime = 0;
	debTaskPerfPtr->auTaskCnt = 0;
	debTaskPerfPtr->gfxTaskCnt = 0;
	nuDebTaskPerfPtr = debTaskPerfPtr;
	osDpSetStatus(DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR | DPC_CLR_CLOCK_CTR);
#endif 

	osCreateMesgQueue(&nusched.retraceMQ, nusched.retraceMsgBuf, NU_SC_MAX_MESGS);
	osCreateMesgQueue(&nusched.rspMQ, nusched.rspMsgBuf, NU_SC_MAX_MESGS);
	osCreateMesgQueue(&nusched.rdpMQ, nusched.rdpMsgBuf, NU_SC_MAX_MESGS);
	osCreateMesgQueue(&nusched.graphicsRequestMQ, nusched.graphicsRequestBuf, NU_SC_MAX_MESGS);

	osCreateMesgQueue(&nusched.audioRequestMQ, nusched.audioRequestBuf, NU_SC_MAX_MESGS);
	osCreateMesgQueue(&nusched.waitMQ, nusched.waitMsgBuf, NU_SC_MAX_MESGS);

	osCreateViManager(OS_PRIORITY_VIMGR);
	osViSetMode(&osViModeTable[videoMode]);
	osViBlack(TRUE);

	osViSetEvent(&nusched.retraceMQ, (OSMesg)VIDEO_MSG, numFields); 
	osSetEventMesg(OS_EVENT_SP, &nusched.rspMQ, (OSMesg)RSP_DONE_MSG);
	osSetEventMesg(OS_EVENT_DP, &nusched.rdpMQ, (OSMesg)RDP_DONE_MSG);
	osSetEventMesg(OS_EVENT_PRENMI, &nusched.retraceMQ, (OSMesg)PRE_NMI_MSG);   

	osCreateThread(&nusched.schedulerThread, 19,
		(void(*)(void*))nuScEventHandler,
		(void *)&nusched, nuScStack+NU_SC_STACK_SIZE/sizeof(u64),
		NU_SC_HANDLER_PRI);
	osStartThread(&nusched.schedulerThread);

	osCreateThread(&nusched.audioThread, 18,
		(void(*)(void *))nuScExecuteAudio,
		(void *)&nusched, nuScAudioStack+NU_SC_STACK_SIZE/sizeof(u64),
		NU_SC_AUDIO_PRI);
	osStartThread(&nusched.audioThread);

	osCreateThread(&nusched.graphicsThread, 17,
		(void(*)(void*))nuScExecuteGraphics,
		(void *)&nusched, nuScGraphicsStack+NU_SC_STACK_SIZE/sizeof(u64),
		NU_SC_GRAPHICS_PRI);
	osStartThread(&nusched.graphicsThread);

}

OSMesgQueue *nuScGetAudioMQ(void) {
  return( &nusched.audioRequestMQ );
}

OSMesgQueue *nuScGetGfxMQ(void) {
  return( &nusched.graphicsRequestMQ );
}

static void nuScEventHandler(void) {

 OSMesg	msg;
 nuScRetraceCounter = 0;
 
 while (1) {
	
		osRecvMesg(&nusched.retraceMQ, &msg, OS_MESG_BLOCK);
		
		switch ((int)msg) {
			
			case VIDEO_MSG:		

				nuScRetraceCounter++;

		#ifdef NU_DEBUG
				if (nuDebTaskPerfEnd) {
					debTaskPerfPtr->retraceTime = OS_CYCLES_TO_USEC(osGetTime());
					nuDebTaskPerf[nuDebTaskPerfCnt].auTaskCnt = 0;
					nuDebTaskPerfEnd = 0;
				}
		#endif 
				
				nuScEventBroadcast(&nusched.retraceMsg );
				
				break;

			case PRE_NMI_MSG:		
				
				nuScEventBroadcast(&nusched.prenmiMsg );

				if (nuScPreNMIFunc != NULL) {
					(*nuScPreNMIFunc)();
				}

				break;

		}
 }

}

void nuScAddClient(NUScClient *client, OSMesgQueue *msgQ, NUScMsg msgType) {

	OSIntMask	mask;

	mask = osSetIntMask(OS_IM_NONE);

	client->msgQ = msgQ;
	client->next = nusched.clientList;
	client->msgType = msgType;
	nusched.clientList = client;

	osSetIntMask(mask);

}

void nuScRemoveClient(NUScClient *c) {

	NUScClient*	client	= nusched.clientList; 
	NUScClient*	prev	= 0;
	OSIntMask	mask;

	mask = osSetIntMask(OS_IM_NONE);

	while (client != 0) {

		if (client == c) {
		
			if (prev) {
				prev->next = c->next;
			} else {
				nusched.clientList = c->next;
			}
		
			break;
		
		}

		prev   = client;
		client = client->next;

	}

	osSetIntMask(mask);

}

static void nuScEventBroadcast(NUScMsg *msg) {

	NUScClient *client;

	for (client = nusched.clientList; client != 0; client = client->next) {

		if (client->msgType & *msg) {
			osSendMesg(client->msgQ, (OSMesg *)msg, OS_MESG_NOBLOCK);
		}
		
	}

}

static void nuScExecuteAudio(void) {

	NUScTask* gfxTask;
	NUScTask* audioTask;
	OSMesg	msg;
	u32 yieldFlag;
#ifdef NU_DEBUG
	OSIntMask mask;
#endif 
 
 	while (1) {
	
		osRecvMesg(&nusched.audioRequestMQ, (OSMesg*)&audioTask, OS_MESG_BLOCK);
		osWritebackDCacheAll();	
		
		yieldFlag = 0;
		gfxTask = nusched.curGraphicsTask;
		
		if ( gfxTask ) {
			
			osSpTaskYield();		
			osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);

			if (osSpTaskYielded(&gfxTask->list)) {
				yieldFlag = TASK_YIELD;
			} else {
				yieldFlag = TASK_YIELDED;
			}

		}

	#ifdef NU_DEBUG
		
		mask = osSetIntMask(OS_IM_NONE);
		
		if (debTaskPerfPtr->auTaskCnt < NU_DEB_PERF_AUTASK_CNT) {
			debTaskPerfPtr->auTaskTime[debTaskPerfPtr->auTaskCnt].rspStart =
			OS_CYCLES_TO_USEC(osGetTime());
		}
		
		osSetIntMask(mask);

	#endif 
		
		nusched.curAudioTask = audioTask;
		osSpTaskStart(&audioTask->list);
		
		osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
		nusched.curAudioTask = (NUScTask *)NULL;

	#ifdef NU_DEBUG

		mask = osSetIntMask(OS_IM_NONE);

		if (debTaskPerfPtr->auTaskCnt < NU_DEB_PERF_AUTASK_CNT) {
			debTaskPerfPtr->auTaskTime[debTaskPerfPtr->auTaskCnt].rspEnd = OS_CYCLES_TO_USEC(osGetTime());
			debTaskPerfPtr->auTaskCnt++;
		}

		osSetIntMask(mask);

	#endif 

		if (nusched.graphicsTaskSuspended) osSendMesg( &nusched.waitMQ, &msg, OS_MESG_BLOCK );
		
		if (yieldFlag == TASK_YIELD) {
			osSpTaskStart(&gfxTask->list); 
		} else if ( yieldFlag == TASK_YIELDED ) {
			osSendMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);
		}

		osSendMesg(audioTask->msgQ, audioTask->msg, OS_MESG_BLOCK);
 
	}

}

static void nuScExecuteGraphics(void) {

	OSMesg msg;
	NUScTask* gfxTask;
	OSIntMask mask;

	while (1) {

		osRecvMesg(&nusched.graphicsRequestMQ, (OSMesg *)&gfxTask, OS_MESG_BLOCK);
		
		nuScWaitTaskReady(gfxTask);

		mask = osSetIntMask(OS_IM_NONE);

		if (nusched.curAudioTask) {

			nusched.graphicsTaskSuspended = gfxTask;
			
			osSetIntMask(mask);
			osRecvMesg( &nusched.waitMQ, &msg, OS_MESG_BLOCK );
			
			mask = osSetIntMask(OS_IM_NONE);
			
			nusched.graphicsTaskSuspended = (NUScTask *)NULL;

		}

		osSetIntMask(mask);

	#ifdef NU_DEBUG
		
		mask = osSetIntMask(OS_IM_NONE);
		
		if (debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT) {
			debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspStart =
			OS_CYCLES_TO_USEC(osGetTime());
		}
		
		osSetIntMask(mask);

	#endif 
			
		mask = osSetIntMask(OS_IM_NONE);
		nusched.curGraphicsTask = gfxTask;
		osSetIntMask(mask);
		
		osSpTaskStart(&gfxTask->list);  
		osRecvMesg(&nusched.rspMQ, &msg, OS_MESG_BLOCK);

		mask = osSetIntMask(OS_IM_NONE);
		nusched.curGraphicsTask = (NUScTask *)NULL;

		osSetIntMask(mask);

	#ifdef NU_DEBUG
		
		mask = osSetIntMask(OS_IM_NONE);
		
		if (debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT) {
			debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspEnd = OS_CYCLES_TO_USEC(osGetTime());
		}
		
		osSetIntMask(mask);

	#endif 

		if (!(gfxTask->flags & NU_SC_NORDP)) { 
			osRecvMesg(&nusched.rdpMQ, &msg, OS_MESG_BLOCK);
		}

	#ifdef NU_DEBUG
		
		mask = osSetIntMask(OS_IM_NONE);
		
		if (debTaskPerfPtr->gfxTaskCnt < NU_DEB_PERF_GFXTASK_CNT) {
			
			if (gfxTask->flags & NU_SC_NORDP) {
			
				debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rdpEnd = debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rspStart;
				debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].dpCnt[0] = 0;
			
			} else {
			
				debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].rdpEnd = OS_CYCLES_TO_USEC(osGetTime());

				osDpGetCounters(debTaskPerfPtr->gfxTaskTime[debTaskPerfPtr->gfxTaskCnt].dpCnt);
				osDpSetStatus(DPC_CLR_TMEM_CTR | DPC_CLR_PIPE_CTR | DPC_CLR_CMD_CTR | DPC_CLR_CLOCK_CTR);
			
			}
			
			debTaskPerfPtr->gfxTaskCnt++;

		}

		if (gfxTask->flags & NU_SC_SWAPBUFFER) {
		
			nuDebTaskPerfPtr = debTaskPerfPtr;
			nuDebTaskPerfCnt++;
			nuDebTaskPerfCnt %= 3;
		
			nuDebTaskPerf[nuDebTaskPerfCnt].gfxTaskCnt = 0;
			nuDebTaskPerf[nuDebTaskPerfCnt].retraceTime = 0;
		
			debTaskPerfPtr = &nuDebTaskPerf[nuDebTaskPerfCnt];
			nuDebTaskPerfEnd++;
		
		}
		
		osSetIntMask(mask);

	#endif 

		osSendMesg(gfxTask->msgQ, (OSMesg*)gfxTask, OS_MESG_BLOCK);
 
	}

}

void nuScWaitTaskReady(NUScTask *task) {

	NUScClient	client;
	void*	fb	= task->framebuffer;

	nuScAddClient(&client, &nusched.waitMQ , NU_SC_RETRACE_MSG);  

	while (osViGetCurrentFramebuffer() == fb || osViGetNextFramebuffer() == fb) {
		osRecvMesg( &nusched.waitMQ, NULL, OS_MESG_BLOCK);
	}
 
	nuScRemoveClient(&client);

}
