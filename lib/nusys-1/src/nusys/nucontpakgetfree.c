#include <nusys.h>

s32 nuContPakGetFree(NUContPakFile *file) {
    
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFreeMesg	freeMesg;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakGetFree: no contpak manager!!\n");
#endif 
	    return 0;
    }
	
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    freeMesg.mesg = NU_CONT_PAK_FREE_MSG;
    freeMesg.file = file;
    freeMesg.rtnMesgQ = &rtnMesgQ;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&freeMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

    return freeMesg.size;

}
