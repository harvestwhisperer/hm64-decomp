#include <nusys.h>

void nuContDataReadStart(void) {
    
    static NUScMsg	mesg =  NU_CONT_READ_NW_MSG;

    if (nuContCallBack.func == NULL) {
#ifdef NU_DEBUG    
	    osSyncPrintf("nuContDataReadStart: no controller manager!!\n");
#endif
	    return;
    }

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&mesg, OS_MESG_BLOCK);

}
