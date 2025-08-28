#include <nusys.h>

NUContRmbCtl nuContRmbCtl[4];
u32 nuContRmbSearchTime = NU_CONT_RMB_AUTO_SEARCHTIME;

NUCallBackList	nuContRmbCallBack;

void nuContRmbMgrInit(void) {
    
	u32 cnt;

    if (nuContRmbCallBack.func != NULL) {
#ifdef NU_DEBUG
		osSyncPrintf("nuContRmbMgrInit: RUMBLE manager already entry!!\n");
#endif 
		return;
    }
    
    for(cnt = 0; cnt <NU_CONT_MAXCONTROLLERS; cnt++) {
		nuContRmbCtl[cnt].state = NU_CONT_RMB_STATE_STOPPED;
		nuContRmbCtl[cnt].mode	= NU_CONT_RMB_MODE_DISABLE;
		nuContRmbCtl[cnt].counter = cnt;
    }

    nuSiCallBackAdd(&nuContRmbCallBack, nuContRmbMgr);

}

void nuContRmbMgrRemove(void) {
    nuSiCallBackRemove(&nuContRmbCallBack);
}

static int contRmbControl(NUContRmbCtl* rmbCtl, u32 contNo) {

    s32	rtn = 0;
    u32	integer;

    switch(rmbCtl->state) {

		case NU_CONT_RMB_STATE_STOPPED:	
			break;

		case NU_CONT_RMB_STATE_STOPPING:

			if (rmbCtl->counter > 0) {
				rtn = osMotorStop(&nuContPfs[contNo]);
			} else {
				rmbCtl->state = NU_CONT_RMB_STATE_STOPPED;
			}

			rmbCtl->counter--;

			break;

		case NU_CONT_RMB_STATE_RUN:	

			if (rmbCtl->frame >0) {
				
				rmbCtl->counter += rmbCtl->freq;
				integer = rmbCtl->counter >> 8;
				rmbCtl->counter &= 0x00ff;

				if ( integer > 0) {
					rtn = osMotorStart(&nuContPfs[contNo]);
				} else {
					rtn = osMotorStop(&nuContPfs[contNo]);
				}

			} else {
			
				rtn = osMotorStop(&nuContPfs[contNo]);

				rmbCtl->state = NU_CONT_RMB_STATE_STOPPING;
				rmbCtl->counter = 2;
			
			}

			rmbCtl->frame--;
			
			break;

		case NU_CONT_RMB_STATE_FORCESTOP:	

			rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[contNo], contNo);
		
			if (!rtn) {
				osMotorStop(&nuContPfs[contNo]);
			}
		
			rmbCtl->state = NU_CONT_RMB_STATE_STOPPING;
			rmbCtl->counter = 2;
		
			break;

    }

    return rtn;

}

void contRmbRetrace(void) {

    u32		cnt;
    s32		rtn;
    NUContRmbCtl*  rmbCtl;
    u32 	counter;
    
    counter = nuScRetraceCounter  % nuContRmbSearchTime;

    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++) {
	
		rmbCtl = &nuContRmbCtl[cnt];
		
		switch(rmbCtl->mode) {

			case NU_CONT_RMB_MODE_DISABLE:	
				break;
				
			case NU_CONT_RMB_MODE_ENABLE:	
			
				rtn = contRmbControl(rmbCtl, cnt);	
				
				if (rtn) {
					rmbCtl->mode = NU_CONT_RMB_MODE_DISABLE;
				}
	
				break;
				
			case NU_CONT_RMB_MODE_AUTORUN:  

				if (rmbCtl->autorun == NU_CONT_RMB_AUTO_SEARCH) {			
				
					counter =  rmbCtl->counter % nuContRmbSearchTime;

					if (!counter) {
						
						rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[cnt], cnt);

						if (!rtn) {
							rmbCtl->autorun = NU_CONT_RMB_AUTO_FIND;
							rmbCtl->type = NU_CONT_PAK_TYPE_RUMBLE;
						}

					}
					
					rmbCtl->counter++;

				} else {

					rtn = contRmbControl(rmbCtl, cnt);

					if (rtn) {
						rmbCtl->counter = cnt;
						rmbCtl->autorun = NU_CONT_RMB_AUTO_SEARCH;
						rmbCtl->type = NU_CONT_PAK_TYPE_NONE;
					}

				}

				break;

			case (NU_CONT_RMB_MODE_ENABLE | NU_CONT_RMB_MODE_PAUSE):
			case (NU_CONT_RMB_MODE_AUTORUN | NU_CONT_RMB_MODE_PAUSE):

				if (rmbCtl->type == NU_CONT_PAK_TYPE_RUMBLE) {
					rtn = contRmbControl(rmbCtl, cnt);
				}

				break;

		}
    }

}

static void contRmbCheckMesg(NUContRmbMesg* mesg) {

    s32		rtn;
    
    rtn = osMotorInit(&nuSiMesgQ, &nuContPfs[mesg->contNo], mesg->contNo);
    mesg->error = rtn;
    
	osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contRmbForceStopMesg(NUContRmbMesg* mesg) {

    u32 cnt;

    for(cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++) {
		nuContRmbCtl[cnt].state = NU_CONT_RMB_STATE_FORCESTOP;
		nuContRmbCtl[cnt].mode |= NU_CONT_RMB_MODE_PAUSE;
    }

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contRmbForceStopEndMesg(NUContRmbMesg* mesg) {

    u32 cnt;

    for (cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++) {
		nuContRmbCtl[cnt].mode &= 0x7f;
    }
    
	osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contRmbStartMesg(NUContRmbMesg* mesg) {

    NUContRmbCtl*	rmbCtlData;
    
    rmbCtlData = mesg->data;

    nuContRmbCtl[mesg->contNo].state = rmbCtlData->state;
    nuContRmbCtl[mesg->contNo].frame = rmbCtlData->frame;
    nuContRmbCtl[mesg->contNo].freq  = rmbCtlData->freq;
    nuContRmbCtl[mesg->contNo].counter = 0;

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

s32 nuContRmbMgr(void* mesg) {

    NUScMsg*	mesg_type;
    mesg_type = (NUScMsg*)mesg;

    switch(*mesg_type) {

		case NU_SC_RETRACE_MSG:		
			contRmbRetrace();		
			return NU_SI_CALLBACK_CONTINUE;
		
		case NU_CONT_RMB_CHECK_MSG:		
			contRmbCheckMesg((NUContRmbMesg*)mesg);
			break;

		case NU_CONT_RMB_START_MSG:		
			contRmbStartMesg((NUContRmbMesg*)mesg);
			break;

		case NU_SC_PRENMI_MSG:		
		case NU_CONT_RMB_FORCESTOP_MSG:	
			contRmbForceStopMesg((NUContRmbMesg*)mesg);
			break;
		
		case NU_CONT_RMB_FORCESTOPEND_MSG:	
			contRmbForceStopEndMesg((NUContRmbMesg*)mesg);
			break;
		
		default:
			return NU_SI_CALLBACK_CONTINUE;

    }
    
    return NU_SI_CALLBACK_END;
    
}
