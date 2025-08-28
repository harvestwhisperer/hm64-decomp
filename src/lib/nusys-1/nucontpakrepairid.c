#include <nusys.h>

void nuContPakRepairId(NUContPakFile *file) {

    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakMesg 	repairMesg;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakRepairId: no contpak manager!!\n");
#endif 
	    return;
    }
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    repairMesg.mesg       = NU_CONT_PAK_REPAIRID_MSG;
    repairMesg.file       = file;
    repairMesg.rtnMesgQ   = &rtnMesgQ;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&repairMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

}
