#include <nusys.h>

static OSThread	IdleThread;		
static OSThread	MainThread;		

u64		    nuMainStack[NU_MAIN_STACK_SIZE/sizeof(u64)];
static u64	IdleStack[NU_IDLE_STACK_SIZE/sizeof(u64)];

void (*nuIdleFunc)(void);		

static void idle(void *arg);		
extern void mainproc(void *arg);		

void nuBoot(void) {

    osInitialize();	
    
    osCreateThread(&IdleThread,NU_IDLE_THREAD_ID, idle, (void*)NULL,
		   (IdleStack + NU_IDLE_STACK_SIZE/sizeof(u64)), NU_MAIN_THREAD_PRI);
    osStartThread(&IdleThread);

}

static void idle(void *arg) {
    
    nuIdleFunc = NULL;

    nuPiInit();
    nuScCreateScheduler(OS_VI_NTSC_LAN1, 1);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON
			   | OS_VI_GAMMA_OFF
			   | OS_VI_GAMMA_DITHER_OFF
			   | OS_VI_DIVOT_ON);
                   
    osCreateThread(&MainThread, NU_MAIN_THREAD_ID, mainproc, (void*)NULL,
		   (nuMainStack + NU_MAIN_STACK_SIZE/sizeof(u64)), NU_MAIN_THREAD_PRI);
    osStartThread(&MainThread);

    osSetThreadPri(&IdleThread, NU_IDLE_THREAD_PRI);

    while (1) {
        if (nuIdleFunc != NULL) {
            
            (*nuIdleFunc)();
        }
    }

}
