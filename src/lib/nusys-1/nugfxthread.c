#include <nusys.h>

NUGfxFunc nuGfxFunc = NULL;
NUGfxPreNMIFunc	nuGfxPreNMIFunc = NULL;
OSMesgQueue nuGfxMesgQ;

static OSMesg nuGfxMesgBuf[NU_GFX_MESGS];
static OSThread	GfxThread;
static u64 GfxStack[NU_GFX_STACK_SIZE/8];

static void nuGfxThread(void *arg) {

    u8 nuGfxpending;
    NUScClient gfx_client;
    NUScMsg* mesg_type;

    nuGfxpending = 0;
    nuGfxFunc = NULL;
    nuGfxPreNMIFunc = NULL;
    
    osCreateMesgQueue(&nuGfxMesgQ, nuGfxMesgBuf, NU_GFX_MESGS);

    nuScAddClient(&gfx_client, &nuGfxMesgQ, NU_SC_RETRACE_MSG | NU_SC_PRENMI_MSG);

    while (1) {
    
    	(void)osRecvMesg(&nuGfxMesgQ, (OSMesg*)&mesg_type, OS_MESG_BLOCK);
    
        switch(*mesg_type) {

            case NU_SC_RETRACE_MSG:	{

                if (!(nuGfxpending & 1)) {

                    if (nuGfxFunc != NULL) {
                        (*nuGfxFunc)(nuGfxTaskSpool);
                    }

                    nuGfxpending = 0;

                }
                
                nuGfxpending++;
            
                break;

            }

            case NU_SC_PRENMI_MSG: {

                if (nuGfxPreNMIFunc != NULL) {
                    (*nuGfxPreNMIFunc)();
                    
                }
            
                break;

            }
    
            default:
                break;

        }
    }
}

void nuGfxThreadStart(void) {
    
    osCreateThread(&GfxThread, NU_GFX_THREAD_ID, nuGfxThread, (void*)NULL,
		           (GfxStack + NU_GFX_STACK_SIZE/8), NU_GFX_THREAD_PRI);
    osStartThread(&GfxThread);

}
