#include <nusys.h>

OSMesgQueue	nuSiMesgQ;			
static OSMesg	nuSiMesgBuf[8];	
static OSThread siMgrThread;
static u64	siMgrStack[NU_SI_STACK_SIZE/sizeof(u64)];
OSMesgQueue	nuSiMgrMesgQ;		
NUCallBackList	nuSiCallBackList;	

static void nuSiMgrThread(void* arg);

u8 nuSiMgrInit(void) {

    u8 pattern;
    OSContStatus status[NU_CONT_MAXCONTROLLERS];
    
    nuSiCallBackList.next = NULL;
    nuSiCallBackList.func = NULL;
        
    osCreateMesgQueue(&nuSiMesgQ, nuSiMesgBuf, 8);
    osSetEventMesg(OS_EVENT_SI, &nuSiMesgQ, (OSMesg)0);

    osContInit(&nuSiMesgQ, &pattern, status);
    
    osCreateThread(&siMgrThread, NU_SI_THREAD_ID, nuSiMgrThread,
		   (void*)NULL,
		   (siMgrStack + NU_SI_STACK_SIZE/8), NU_SI_THREAD_PRI);
    osStartThread(&siMgrThread);
    
    return pattern;

}

static void nuSiMgrThread(void* arg) {

    NUScClient siClient;
    OSMesg siMgrMesgBuf[NU_SI_MESG_MAX];
    NUScMsg* mesgType;
    NUCallBackList*	siCallBackListPtr;
	s32 ret;
	
	osCreateMesgQueue(&nuSiMgrMesgQ, siMgrMesgBuf, NU_SI_MESG_MAX);

    nuScAddClient(&siClient, &nuSiMgrMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);
	
    while (1) {

        (void)osRecvMesg(&nuSiMgrMesgQ,(OSMesg*)&mesgType, OS_MESG_BLOCK);
        
        siCallBackListPtr = nuSiCallBackList.next;

        while (siCallBackListPtr) {

            if (siCallBackListPtr->func) {
            
                ret = (*(siCallBackListPtr->func))((void*)mesgType);
                if (ret) break;
            
            }

            siCallBackListPtr = siCallBackListPtr->next;
        
        }
        
    }

}
