#include <nusys.h>
u16		nuContPakCompanyCode = 0;
u32		nuContPakGameCode = 0;

NUCallBackList	nuContPakCallBack = {NULL, NULL};

void nuContPakMgrInit(void) {

    if (nuContPakCallBack.func != NULL) {
#ifdef NU_DEBUG
        osSyncPrintf("nuContPakMgrInit: controller pak manager already entry!!\n");
        return;
#endif 
    }

    nuSiCallBackAdd(&nuContPakCallBack, nuContPakMgr);

}

void nuContPakMgrRemove(void) {
    nuSiCallBackRemove(&nuContPakCallBack);
}

static void contPakOpen(NUContPakOpenMesg* mesg) {

    s32			rtn;
    NUContPakFile*	file;
    
    file = mesg->file;
    file->type = NU_CONT_PAK_TYPE_NONE;
  
    rtn = osPfsInitPak(&nuSiMesgQ, file->pfs, file->pfs->channel);
    
    if (rtn == 0) {
    	file->type = NU_CONT_PAK_TYPE_PAK;
    }

    file->error = rtn;

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakFree(NUContPakFreeMesg* mesg) {
    
    s32 rtn;
    mesg->size = 0;
    rtn = osPfsFreeBlocks(mesg->file->pfs, &mesg->size);
    mesg->file->error = rtn;

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakFileReadWrite(NUContPakFileRWMesg* mesg) {

    NUContPakFile*	file;
    s32			rtn;

    file = mesg->file;

    rtn = osPfsReadWriteFile(file->pfs, file->file_no, mesg->mode,
			     mesg->offset, mesg->size, mesg->buf);
    file->error = rtn;
    
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakFileOpen(NUContPakFileOpenMesg* mesg) {

    s32 rtn;
    NUContPakFile*file;

    file = mesg->file;

    rtn = osPfsFindFile(file->pfs, nuContPakCompanyCode, nuContPakGameCode,
        mesg->noteName, mesg->extName, &file->file_no);

    if ((rtn == PFS_ERR_INVALID) && (mesg->mode == NU_CONT_PAK_MODE_CREATE)) {
	
        rtn = osPfsAllocateFile(file->pfs,
            nuContPakCompanyCode, nuContPakGameCode,
            mesg->noteName, mesg->extName, mesg->size,
            &file->file_no);

    }

    file->error = rtn;	

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakFileDelete(NUContPakFileOpenMesg* mesg) {

    s32 rtn;
    NUContPakFile* file;

    file = mesg->file;
    
    rtn = osPfsDeleteFile(file->pfs,
        nuContPakCompanyCode, nuContPakGameCode,
        mesg->noteName, mesg->extName);
    file->error = rtn;	

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakFileState(NUContPakFileStateMesg* mesg) {

    s32 rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsFileState(file->pfs, file->file_no, mesg->state);
    file->error = rtn;	

    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakFileNum(NUContPakFileNumMesg* mesg) {
    
    s32	rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsNumFiles(file->pfs, mesg->max_files, mesg->used_files);
    file->error = rtn;	
    
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

static void contPakRepairId(NUContPakMesg* mesg) {

    s32			rtn;
    NUContPakFile*	file;

    file = mesg->file;
    rtn = osPfsRepairId(file->pfs);
    file->error = rtn;	
    
    osSendMesg(mesg->rtnMesgQ, NULL, OS_MESG_BLOCK);

}

s32 nuContPakMgr(void* mesg) {

    NUScMsg*	mesg_type;
    mesg_type = (NUScMsg*)mesg;
    
    switch(*mesg_type) {

        case NU_CONT_PAK_OPEN_MSG:	
            contPakOpen((NUContPakOpenMesg*)mesg_type);
            break;

        case NU_CONT_PAK_FREE_MSG:	
            contPakFree((NUContPakFreeMesg*)mesg_type);
            break;
        
        case NU_CONT_PAK_FOPEN_MSG:	
            contPakFileOpen((NUContPakFileOpenMesg*)mesg_type);
            break;

        case NU_CONT_PAK_READWRITE_MSG: 
            contPakFileReadWrite((NUContPakFileRWMesg*)mesg_type);
            break;
        
        case NU_CONT_PAK_FILEDELETE_MSG: 
            contPakFileDelete((NUContPakFileOpenMesg*)mesg_type);
            break;

        case NU_CONT_PAK_FILESTATE_MSG:	
            contPakFileState((NUContPakFileStateMesg*)mesg_type);
            break;

        case NU_CONT_PAK_FILENUM_MSG:	
            contPakFileNum((NUContPakFileNumMesg*)mesg_type);
            break;

        case NU_CONT_PAK_REPAIRID_MSG:	
            contPakRepairId((NUContPakMesg*)mesg_type);
            break;
#if 0
        case NU_CONT_PAK_RESIZE_MSG:	
            contPakReSize((NUContPakFileOpenMesg*)mesg_type);
            break;
#endif
        default:
	        return NU_SI_CALLBACK_CONTINUE;

    }
    
    return NU_SI_CALLBACK_END;

}
