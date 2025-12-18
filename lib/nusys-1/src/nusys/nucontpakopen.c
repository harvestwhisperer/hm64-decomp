#include <nusys.h>

void nuContPakOpen(NUContPakFile *file, u32 contNo) {
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakOpenMesg	openMesg;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakOpen: no contpak manager!!\n");
#endif 
	    return;
    }
	
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    file->pfs = &nuContPfs[contNo];
    file->pfs->channel = contNo;

    openMesg.mesg     = NU_CONT_PAK_OPEN_MSG;
    openMesg.file     = file;
    openMesg.rtnMesgQ = &rtnMesgQ;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&openMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);
    
}
