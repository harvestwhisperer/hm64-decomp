#include <nusys.h>

void nuContPakFileDeleteJis(NUContPakFile *file, u8* noteName, u8* extName) {
    
    OSMesgQueue		rtnMesgQ;
    OSMesg		    rtnMesgBuf;
    NUContPakFileOpenMesg fopenMesg;
    u8			noteNameN64[32];
    u8			extNameN64[4];
    u32			len;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakFileDeleteJis: no contpak manager!!\n");
#endif 
    	return;
    }
    
    bzero(noteNameN64, 32);
    bzero(extNameN64, 4);

    len = 0;

    while (noteName[len]) len++;

    nuContPakJisToN64(noteName, noteNameN64, len);
    nuContPakJisToN64(extName, extNameN64, 1);

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);

    fopenMesg.mesg     = NU_CONT_PAK_FILEDELETE_MSG;
    fopenMesg.file     = file;
    fopenMesg.rtnMesgQ = &rtnMesgQ;
    fopenMesg.noteName = noteNameN64;
    fopenMesg.extName  = extNameN64;
    
    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&fopenMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

}
