#include "common.h"
#include "nusys.h"


#define TASK_YIELD	1
#define TASK_YIELDED	2

#define VIDEO_MSG		666
#define RSP_DONE_MSG	667
#define RDP_DONE_MSG	668
#define PRE_NMI_MSG	    669

// FIXME: uncomment
// static u64	nuScStack[NU_SC_STACK_SIZE/8];
// static u64	nuScAudioStack[NU_SC_STACK_SIZE/8];
// static u64	nuScGraphicsStack[NU_SC_STACK_SIZE/8];

NUSched		nusched;

// rodata
//NUScPreNMIFunc	nuScPreNMIFunc = NULL;

u32		nuScRetraceCounter; 

extern void *nuScStack;
extern void *nuScAudioStack;
extern void *nuScGraphicsStack;

void nuScEventHandler(void);
void nuScExecuteAudio(void);
void nuScExecuteGraphics(void);

//INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScCreateScheduler);

void nuScCreateScheduler(u8 videoMode, u8 numFields) {

  nusched.curGraphicsTask = NULL;
  nusched.curAudioTask    = NULL;
  nusched.graphicsTaskSuspended = NULL;
  nusched.clientList      = NULL;
  nusched.retraceMsg      = NU_SC_RETRACE_MSG;
  nusched.prenmiMsg       = NU_SC_PRENMI_MSG;
  nusched.retraceCount	  = numFields;
    
  osCreateMesgQueue(&nusched.retraceMQ, nusched.retraceMsgBuf,
		    NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.rspMQ, nusched.rspMsgBuf, NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.rdpMQ, nusched.rdpMsgBuf, NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.graphicsRequestMQ, nusched.graphicsRequestBuf,
		    NU_SC_MAX_MESGS);

  osCreateMesgQueue(&nusched.audioRequestMQ, nusched.audioRequestBuf,
		    NU_SC_MAX_MESGS);
  osCreateMesgQueue(&nusched.waitMQ, nusched.waitMsgBuf, NU_SC_MAX_MESGS);

  osCreateViManager(OS_PRIORITY_VIMGR);
  osViSetMode(&osViModeTable[videoMode]);
  osViBlack(TRUE);

  osViSetEvent(&nusched.retraceMQ, (OSMesg)VIDEO_MSG, numFields);    
  osSetEventMesg(OS_EVENT_SP    , &nusched.rspMQ,     (OSMesg)RSP_DONE_MSG);
  osSetEventMesg(OS_EVENT_DP    , &nusched.rdpMQ,     (OSMesg)RDP_DONE_MSG);
  osSetEventMesg(OS_EVENT_PRENMI, &nusched.retraceMQ, (OSMesg)PRE_NMI_MSG);   

  osCreateThread(&nusched.schedulerThread, 19,
		 (void(*)(void*))nuScEventHandler,
		 (void *)&nusched, &nuScStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_HANDLER_PRI);
  osStartThread(&nusched.schedulerThread);

  osCreateThread(&nusched.audioThread, 18,
		 (void(*)(void *))nuScExecuteAudio,
		 (void *)&nusched, &nuScAudioStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_AUDIO_PRI);
  osStartThread(&nusched.audioThread);

  osCreateThread(&nusched.graphicsThread, 17,
		 (void(*)(void*))nuScExecuteGraphics,
		 (void *)&nusched, &nuScGraphicsStack+NU_SC_STACK_SIZE/sizeof(u64),
		 NU_SC_GRAPHICS_PRI);
  osStartThread(&nusched.graphicsThread);

}

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB354);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB360);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScEventHandler);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB428);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB48C);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB524);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScExecuteAudio);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", nuScExecuteGraphics);

INCLUDE_ASM(const s32, "lib/nusys-1/nusched", func_800FB800);
