#include <nusys.h>

void nuContPakFileDelete(NUContPakFile *file, u8* noteName, u8* extName) {
    
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileOpenMesg fopenMesg;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakFileDelete: no contpak manager!!\n");
#endif 
	    return;
    }
	
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    fopenMesg.mesg     = NU_CONT_PAK_FILEDELETE_MSG;
    fopenMesg.file     = file;
    fopenMesg.rtnMesgQ = &rtnMesgQ;
    fopenMesg.noteName = noteName;
    fopenMesg.extName  = extName;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&fopenMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

}
