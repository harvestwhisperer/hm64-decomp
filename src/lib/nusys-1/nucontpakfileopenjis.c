#include <nusys.h>

void nuContPakFileOpenJis(NUContPakFile *file, u8* noteName, u8* extName, u32 mode, s32 size) {

    OSMesgQueue rtnMesgQ;
    OSMesg rtnMesgBuf;
    NUContPakFileOpenMesg fopenMesg;
    u8 noteNameN64[32];
    u8 extNameN64[4];
    u32 len;

    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakFileOpenJis: no contpak manager!!\n");
#endif 
	    return;
    }

    if (file->type == NU_CONT_PAK_TYPE_NONE) {
        file->error == PFS_ERR_NOPACK;
        return;
    }
    
    bzero(noteNameN64, 32);
    bzero(extNameN64, 4);

    len = 0;

    while (noteName[len]) len++;

#ifdef NU_DEBUG
    if (len > 16) {
	    osSyncPrintf("nuContPakFileOpenJis: Gamenote name(%d) is too long.(max 16 byte)\n", len);
	    return;
    }
#endif 
    
    nuContPakJisToN64(noteName, noteNameN64, len);
    
    nuContPakJisToN64(extName, extNameN64, 1);

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);
    
    fopenMesg.mesg     = NU_CONT_PAK_FOPEN_MSG;
    fopenMesg.file     = file;
    fopenMesg.rtnMesgQ = &rtnMesgQ;
    fopenMesg.noteName = noteNameN64;
    fopenMesg.extName  = extNameN64;
    fopenMesg.size     = size;
    fopenMesg.mode     = mode;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&fopenMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

}
