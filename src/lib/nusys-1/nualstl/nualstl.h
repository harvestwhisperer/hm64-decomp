#ifndef _NUALSTL_H_
#define _NUALSTL_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#define  NU_AU_SYN_VVOICE_MAX	32 
#define  NU_AU_SYN_PVOICE_MAX	32  
#define  NU_AU_SYN_UPDATE_MAX	96  
#define  NU_AU_SYN_HEAP_SIZE	(144+NU_AU_SYN_PVOICE_MAX*432+32*NU_AU_SYN_UPDATE_MAX+0x2000)

#define NU_AU_SEQ_VOICE_MAX	24     
#define NU_AU_SEQ_EVENT_MAX	64     
#define NU_AU_SEQ_CHANNEL_MAX	16      
#define	NU_AU_SEQ_HEAP_SIZE	(NU_AU_SEQ_CHANNEL_MAX*16+NU_AU_SEQ_EVENT_MAX*32)
#define NU_AU_SEQ_PLAYER0	0
#define NU_AU_SEQ_PLAYER1	1
#define	NU_AU_SEQ_MODE_FADEOUT	0x0001
#define	NU_AU_SEQ_MODE_PLAYPENDING	0x0002


#define NU_AU_SND_SOUND_MAX	8     			 
#define NU_AU_SND_EVENT_MAX	(NU_AU_SND_SOUND_MAX*2)   
#define NU_AU_SND_HEAP_SIZE	(NU_AU_SND_SOUND_MAX*48+NU_AU_SND_EVENT_MAX*64)

#define	NU_AU_DMA_BUFFER_NUM	64	  
#define	NU_AU_DMA_BUFFER_SIZE	1024  
#define NU_AU_DMA_QUEUE_NUM	NU_AU_DMA_BUFFER_NUM
#define NU_AU_BUFFER_USE_FRAME  2	

#define	NU_AU_MGR_THREAD_PRI	70
#define	NU_AU_MGR_THREAD_ID	6
#define 	NU_AU_STACK_SIZE	0x2000
#define	NU_AU_CLIST_SIZE	0x800   	
#define	NU_AU_BUFFER_MAX	0x2000  	
#define	NU_AU_OUTPUT_RATE	32000   	
#define	NU_AU_MESG_MAX		8
#define	NU_AU_HEAP_MIN_SIZE (NU_AU_SND_HEAP_SIZE+NU_AU_SEQ_HEAP_SIZE+NU_AU_SYN_HEAP_SIZE+NU_AU_DMA_BUFFER_NUM*NU_AU_DMA_BUFFER_SIZE+NU_AU_CLIST_SIZE*2)
#define 	NU_AU_HEAP_SIZE	0x80000	
#define 	NU_AU_HEAP_ADDR	(NU_GFX_FRAMEBUFFER_ADDR - NU_AU_HEAP_SIZE)

#define	NU_AU_AUDIO_SAMPLES	16	
#define 	NU_AU_AUDIO_EXTRASMP	0	

#define 	NU_AU_TASK_STOP		0	
#define 	NU_AU_TASK_RUN		1	

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)
#include "ultra64.h"
#include "PR/libaudio.h"    

#include "libmus/libmus.h"
#include "libmus/libmus_data.h"


typedef struct st_Seqence{
    ALCSPlayer	player;     	
    ALCSeq		sequence;
    u8*		data_ptr;       	
    s32		seqno;          	
    u32		mode;		
    u16		counter;
    u16		speed;
} NUAuSeqPlayer;

typedef struct st_SndState {
    ALSndId*		Id;
    u8			flag;
} NUAuSndid;

typedef void (*NUAuPreNMIFunc)(NUScMsg,u32);	

extern u32			nuAuFrameCounter;	
extern ALHeap		nuAuHeap;		
extern NUAuSeqPlayer	nuAuSeqPlayer[2];	
extern u8*			nuAuSeqData;
extern ALBankFile*	nuAuSeqBank_ptr;
extern ALSeqFile*		nuAuSeqFile_ptr;
extern ALSndPlayer	nuAuSndPlayer;
extern ALBankFile*	nuAuSndBank_ptr;
extern ALSndId*       	nuAuSndId;
extern ALSynConfig	nuAuSynConfig;
extern ALSeqpConfig	nuAuSeqpConfig;
extern ALSndpConfig	nuAnSndpConfig;
extern u8			nuAuTaskStop;
extern u8			nuAuPreNMI;
extern NUAuPreNMIFunc	nuAuPreNMIFunc;

extern void nuAuStlInit(musConfig *config);
extern void nuAuStlMgrInit(void* heap_ptr, u32 size, ALSynConfig* synconfig);
extern void nuAuStlBankSet(u8 *pbk_addr, u32 pbk_size, u8 *wbk_addr);
extern void nuAuStlPlayerInit(musConfig *config, u32 size);
extern void nuAuStlSeqPlayerSetData(u8 *seq_addr, u32 seq_size);
extern u32 nuAuStlSeqPlayerPlay(void);
extern u32 nuAuStlSndPlayerPlay(u32 sndNo);
extern u32 nuAuStlSndPlayerPlay2(u32 sndNo, s32 volume, s32 pan, s32 restartflag, s32 priority);
extern void nuAuPreNMIFuncSet(NUAuPreNMIFunc func);
extern void nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter);

#define nuAuPreNMIFuncRemove() nuAuPreNMIFuncSet(NULL)

#define nuAuStlSeqPlayerGetState()	MusAsk(MUSFLAG_SONGS)

#define nuAuStlSeqPlayerStop(speed)	MusStop(MUSFLAG_SONGS, speed)

#define nuAuStlSeqPlayerSetMasterVol(vol) MusSetMasterVolume(MUSFLAG_SONGS, vol)

#define nuAuStlSeqPlayerGetSeqState(handle)	MusHandleAsk(handle)

#define nuAuStlSeqPlayerSeqStop(handle, speed)MusHandleStop(handle, speed)

#define nuAuStlSeqPlayerSetSeqTempo(handle, tempo) MusHandleSetTempo(handle, tempo)

#define nuAuStlSeqPlayerSetSeqVol(handle, vol) MusHandleSetVolume(handle, vol)

#define nuAuStlSeqPlayerSetSeqPan(handle, pan) MusHandleSetPan(handle, pan )

#define nuAuStlSndPlayerGetState() MusAsk(MUSFLAG_EFFECTS)

#define nuAuStlSndPlayerStop(speed) MusStop(MUSFLAG_EFFECTS, speed)

#define nuAuStlSndPlayerSetMasterVol(vol) MusSetMasterVolume(MUSFLAG_EFFECTS, vol)

#define nuAuStlSndPlayerGetSndState(handle)	MusHandleAsk(handle)

#define nuAuStlSndPlayerSndStop(handle, speed) MusHandleStop(handle, speed)

#define nuAuStlSndPlayerSetSndVol(handle, vol) MusHandleSetVolume(handle, vol)

#define nuAuStlSndPlayerSetSndPan(handle, pan) MusHandleSetPan(handle, pan )

#define nuAuStlSndPlayerSetSndPitch(handle, pitch) MusHandleSetFreqOffset(handle, pitch)

  
#endif  
#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif 
