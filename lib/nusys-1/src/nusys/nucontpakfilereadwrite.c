#include <nusys.h>

void nuContPakFileReadWrite(NUContPakFile *file, s32 offset, s32 size, u8* buf, u32 mode) {
    OSMesgQueue	rtnMesgQ;
    OSMesg rtnMesgBuf;
    NUContPakFileRWMesg rwMesg;

#ifdef 

    if (size & 0x1f) {
        osSyncPrintf("nuContPakFileReadWrite: data size(%X) must be  multiple 32.\n", size);
        return;
    }

    if (offset & 0x1f) {
        osSyncPrintf("nuContPakFileReadWrite: data offset(%X) must be multiple 32.\n");
        return;
    }

#endif 
    if (nuContPakCallBack.func == NULL) {
#ifdef NU_DEBUG
	    osSyncPrintf("nuContPakFileReadWrite: no contpak manager!!\n");
#endif 
	    return;
    }

    osCreateMesgQueue(&rtnMesgQ, &rtnMesgBuf, 1);

    rwMesg.mesg     = NU_CONT_PAK_READWRITE_MSG;
    rwMesg.file     = file;
    rwMesg.rtnMesgQ = &rtnMesgQ;
    rwMesg.offset   = offset;
    rwMesg.size     = size;
    rwMesg.mode     = mode;
    rwMesg.buf      = buf;

    osSendMesg(&nuSiMgrMesgQ, (OSMesg*)&rwMesg, OS_MESG_BLOCK);
    osRecvMesg(&rtnMesgQ, NULL, OS_MESG_BLOCK);

}
