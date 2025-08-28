#include <nusys.h>

void nuContQueryRead(void) {

    NUContQueryMesg	queryMesg;
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;

    if (nuContCallBack.func == NULL) {
#ifdef NU_DEBUG    
	    osSyncPrintf("nuContQueryRead: no controller manager!!\n");
#endif 
	    return;
    }

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);

    queryMesg.mesg	= NU_CONT_QUERY_MSG;
    queryMesg.rtnMesgQ  = &rtnMesgQ;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&queryMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
    
}
    
