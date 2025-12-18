#include <nusys.h>

void nuContPakFileNum(NUContPakFile *file, s32 *max_files, s32 *used_files) {
    
    OSMesgQueue		rtnMesgQ;
    OSMesg		rtnMesgBuf;
    NUContPakFileNumMesg numMesg;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakFileNum: no contpak manager!!\n");
#endif 
    	return;
    }
    
    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    numMesg.mesg       = NU_CONT_PAK_FILENUM_MSG;
    numMesg.file       = file;
    numMesg.rtnMesgQ   = &rtnMesgQ;
    numMesg.max_files  = max_files;
    numMesg.used_files = used_files;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&numMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

}
