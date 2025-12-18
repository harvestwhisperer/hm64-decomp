#include <nusys.h>

OSMesgQueue nuContWaitMesgQ;	 		
static OSMesg nuContWaitMesgBuf;
OSMesgQueue	nuContDataMutexQ;
static OSMesg nuContDataMutexBuf;		
OSContStatus nuContStatus[NU_CONT_MAXCONTROLLERS];
OSContPad nuContData[NU_CONT_MAXCONTROLLERS];
u32 nuContNum;
u32	nuContDataLockKey;

NUContReadFunc nuContReadFunc = NULL;	
										
OSPfs nuContPfs[4];		
NUCallBackList nuContCallBack;

u8 nuContMgrInit(void) {

    int	cnt;
    u8	pattern;
    u8	bitmask;

    if (nuContCallBack.func != NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContMgrinit: controller manager already entry!!\n");
#endif 
    	return 0;
    }

    nuContDataUnLock();
    
    osCreateMesgQueue(&nuContWaitMesgQ, &nuContWaitMesgBuf, 1);
    osCreateMesgQueue(&nuContDataMutexQ, &nuContDataMutexBuf, 1);
    
    nuSiCallBackAdd(&nuContCallBack, nuContMgr);

    nuContQueryRead();

    nuContNum = 0;
    bitmask = 1;
    pattern = 0;

    for (cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++) {
	
        if (nuContStatus[cnt].errno) continue;

        if ((nuContStatus[cnt].type & (CONT_ABSOLUTE | CONT_JOYPORT)) == (CONT_ABSOLUTE | CONT_JOYPORT)) {
            nuContNum++;
            pattern |= bitmask;
        }

        bitmask <<= 1;

    }

    return pattern;

}

void nuContMgrRemove(void) {
    nuSiCallBackRemove(&nuContCallBack);
}

void nuContDataClose(void) {
    osSendMesg(&nuContDataMutexQ, NULL, OS_MESG_BLOCK);
}

void nuContDataOpen(void) {
    osRecvMesg(&nuContDataMutexQ, NULL, OS_MESG_BLOCK);
}
	
static void contReadData(OSContPad *pad,u32 lockflag) {

    osContStartReadData(&nuSiMesgQ);
    
    osRecvMesg(&nuSiMesgQ, NULL, OS_MESG_BLOCK);

    if (lockflag & nuContDataLockKey) return;
    
    nuContDataClose();
    osContGetReadData(pad);
    nuContDataOpen();

}

static void contQuery(NUContQueryMesg* mesg) {

    osContStartQuery(&nuSiMesgQ);
    osRecvMesg(&nuSiMesgQ, NULL, OS_MESG_BLOCK);
    osContGetQuery(nuContStatus);
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_NOBLOCK);

}

s32 nuContMgr(void* mesg) {
    
    NUContReadMesg*	readMesg;
    NUScMsg*		mesg_type;

    mesg_type = (NUScMsg*)mesg;
    
    
    switch(*mesg_type) {

        case NU_SC_RETRACE_MSG:	
        
            if (nuContDataLockKey) break;
            
            osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
            
            contReadData(nuContData, 1);

            if (nuContReadFunc != NULL) {
                (*nuContReadFunc)(*mesg_type);
            }
            
            osSendMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
            
            return NU_SI_CALLBACK_CONTINUE;	
	
        case NU_CONT_READ_MSG:	

            readMesg = (NUContReadMesg*) mesg_type;
            contReadData(readMesg->pad, 0);
            osSendMesg(readMesg->rtnMesgQ, NULL, OS_MESG_NOBLOCK);
        
            break;
	
        case NU_CONT_READ_NW_MSG:  
				   	 
            osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
            
            contReadData(nuContData, 0);
            
            if (nuContReadFunc != NULL) {
                (*nuContReadFunc)(*mesg_type);
            }

            osSendMesg(&nuContWaitMesgQ, NULL, OS_MESG_NOBLOCK);
            
            break;

        case NU_CONT_QUERY_MSG:		
            contQuery((NUContQueryMesg*)mesg_type);
            break;
 
       default:
	        return NU_SI_CALLBACK_CONTINUE;

    }
    
    return NU_SI_CALLBACK_END;

}
