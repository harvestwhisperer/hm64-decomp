#include <nusys.h>

void nuContPakFileState(NUContPakFile *file, OSPfsState* state) {

    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileStateMesg stateMesg;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakFileState: no contpak manager!!\n");
#endif 
	    return;
    }
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    stateMesg.mesg     = NU_CONT_PAK_FILESTATE_MSG;
    stateMesg.file     = file;
    stateMesg.rtnMesgQ = &rtnMesgQ;
    stateMesg.state    = state;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&stateMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
    
}
