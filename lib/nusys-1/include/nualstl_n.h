#ifndef _NUALSTL_H_
#define _NUALSTL_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

/*----------------------------------------------------------------------*/    
/*----------------------------------------------------------------------*/
/*	DEFINE										*/
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* AUDIO MANEGER DEFINE										*/
/*----------------------------------------------------------------------------*/
/*	The AUDIO HEAP computational exprssion 						*/
/*													*/
/*  (1)The Heap size of alInit 								*/
/* AUIDO HEAP calls the alHeapAlloc function and reserves heap memory  		*/
/* when each driver of the synthesizer, sequence and sound is initialized. 	*/
/* The following is expressing how much of the alHeap ara is needed then: 	*/
/*													*/
/*  144 + maxPVoices * 432 + 32 * maxUPdates + FxType					*/
/*													*/
/* FxType varies depending on effect types and takes the following values: 	*/
/* AL_FX_NONE		0									*/
/* AL_FX_SMALLROOM	8192									*/
/* AL_FX_BIGROOM		8224									*/
/* AL_FX_ECHO		1640									*/
/* AL_FX_CHORUS		304									*/
/* AL_FX_FLANGE		304									*/
/*													*/
/* (2)The Heap size of alSeqpNew and alCSPNew 						*/
/* 													*/
/* maxChannels * 16 + maxVoices * 64 + maxEvents * 32					*/
/*													*/
/* (3)The Heap size of alSndpNew 								*/
/*													*/
/* maxSounds * 48 + maxEvents * 32								*/
/*													*/
/*----------------------------------------------------------------------------*/
/*--------------------------------------*/
/* SYNTHE DEFINE				    */
/*--------------------------------------*/
#define NU_AU_SYN_VVOICE_MAX	32      /* The maximum number of virtual voices (Unused, as a matter of fact)  */
#define NU_AU_SYN_PVOICE_MAX	32      /* The maximum number of physical voices  */
#define NU_AU_SYN_UPDATE_MAX	96      /*The maximum number of updating parameters of the synthesizer  */
#define NU_AU_SYN_HEAP_SIZE	(144+NU_AU_SYN_PVOICE_MAX*432+32*NU_AU_SYN_UPDATE_MAX+0x2000)

/*--------------------------------------*/
/* SEQUENCE DEFINE			    */
/*--------------------------------------*/
#define NU_AU_SEQ_VOICE_MAX	24      /* The maximum number of voices  */
#define NU_AU_SEQ_EVENT_MAX	64      /* The number of events */
#define NU_AU_SEQ_CHANNEL_MAX	16      /* The number of MIDI channels (16, normally) */
#define NU_AU_SEQ_HEAP_SIZE	(NU_AU_SEQ_CHANNEL_MAX*16+NU_AU_SEQ_EVENT_MAX*32)
#define NU_AU_SEQ_PLAYER0	0
#define NU_AU_SEQ_PLAYER1	1
#define NU_AU_SEQ_MODE_FADEOUT	0x0001
#define NU_AU_SEQ_MODE_PLAYPENDING	0x0002


/*--------------------------------------*/
/* SOUND DEFINE				    */
/*--------------------------------------*/
#define NU_AU_SND_SOUND_MAX	8     			/* The maximum number of sounds  */  
#define NU_AU_SND_EVENT_MAX	(NU_AU_SND_SOUND_MAX*2) /* The maximum number of events  */
#define NU_AU_SND_HEAP_SIZE	(NU_AU_SND_SOUND_MAX*48+NU_AU_SND_EVENT_MAX*64)

/*--------------------------------------*/
/* AUDIO DMA DEFINE			    */
/*--------------------------------------*/
#define NU_AU_DMA_BUFFER_NUM	64		/* The number of DMA transfer buffers */
#define NU_AU_DMA_BUFFER_SIZE	1024  	/* The size of the DMA buffer */
#define NU_AU_DMA_QUEUE_NUM	NU_AU_DMA_BUFFER_NUM
#define NU_AU_BUFFER_USE_FRAME  2		/* The frame space for clearing the DMA Buffer */

/*--------------------------------------*/
/* SOUND DEFINE				    */
/*--------------------------------------*/
#define NU_AU_MGR_THREAD_PRI	70
#define NU_AU_MGR_THREAD_ID	6
#define NU_AU_STACK_SIZE	0x2000
#define NU_AU_CLIST_SIZE	0x400   	/* The buffer size of the command list  */
#define NU_AU_BUFFER_MAX	0x2000  	/* The buffer size for the synthesizer  */
#define NU_AU_OUTPUT_RATE	32000   	/* Setting the frequency  */
#define NU_AU_MESG_MAX		8
#define NU_AU_HEAP_MIN_SIZE	(NU_AU_SND_HEAP_SIZE+NU_AU_SEQ_HEAP_SIZE+NU_AU_SYN_HEAP_SIZE+NU_AU_DMA_BUFFER_NUM*NU_AU_DMA_BUFFER_SIZE+NU_AU_CLIST_SIZE*2)
#define NU_AU_HEAP_SIZE		0x80000 	/* The HEAP size of default  */
#define NU_AU_HEAP_ADDR		(NU_GFX_FRAMEBUFFER_ADDR - NU_AU_HEAP_SIZE)

#define NU_AU_NAUDIO_SAMPLES	184	/* The sample number of NAUDIO is a multiple of 184  */
#define NU_AU_NAUDIO_EXTRASMP	0	/* The expansion of the sample number 		*/

#define NU_AU_TASK_STOP		0	/* Stop running the audio task  */
#define NU_AU_TASK_RUN		1	/* Possible to run the audio task    */

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)
#include <ultra64.h>
/*
#include <n_libaudio_sc.h>
#include <n_libaudio_sn_sc.h>
*/
#include <libmus.h>
#include <libmus_data.h>

/*--------------------------------------*/
/* audio typedef				    */
/*--------------------------------------*/
/* The sequence player structure  */
typedef struct st_Seqence{
    ALCSPlayer	player;     	/* The sequence player structure   */
    ALCSeq		sequence;
    u8*		data_ptr;       	/* Sequence data  */
    s32		seqno;          	/* Sequence No. */
    u32		mode;			/* Expansion of control  */
    u16		counter;
    u16		speed;
} NUAuSeqPlayer;

typedef struct st_SndState {
    ALSndId*	Id;
    u8		flag;
} NUAuSndid;

/*--------------------------------------*/
/* CALL BACK Function	typedef	    */
/*--------------------------------------*/
typedef void (*NUAuPreNMIFunc)(NUScMsg,u32);	/* The PRENMI call-back function  */

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* extern variables 									*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/    
/*--------------------------------------*/
/*  audio variables 			    */
/*--------------------------------------*/
extern u32			nuAuFrameCounter;	/* The frame counter  */
extern ALHeap		nuAuHeap;		/* The Heap structure  */
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

/*----------------------------------------------------------------------*/    
/*----------------------------------------------------------------------*/
/* Global Function 									*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*--------------------------------------*/
/* audio manager function		    */
/*--------------------------------------*/
extern void nuAuStlInit(musConfig *config);
extern void nuAuStlMgrInit(void* heap_ptr, u32 size, ALSynConfig* synconfig);
extern void nuAuStlBankSet(u8 *pbk_addr, u32 pbk_size, u8 *wbk_addr);
extern void nuAuStlPlayerInit(musConfig *config, u32 size);
extern void nuAuStlSeqPlayerSetData(u8 *seq_addr, u32 seq_size);
extern u32 	nuAuStlSeqPlayerPlay(void);
extern u32 	nuAuStlSndPlayerPlay(u32 sndNo);
extern u32 	nuAuStlSndPlayerPlay2(u32 sndNo, s32 volume, s32 pan, s32 restartflag, s32 priority);
extern void nuAuPreNMIFuncSet(NUAuPreNMIFunc func);
extern void nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter);

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* MACRO	 										*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

/* Delete the call-back function  */
#define nuAuPreNMIFuncRemove() nuAuPreNMIFuncSet(NULL)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerGetState - Get the state of the sequence player 	*/
/*	IN:	None 										*/
/*	RET:	The channel number that the sequence player is using 		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerGetState()					\
	MusAsk(MUSFLAG_SONGS)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerStop - Stop the performance of all sequence 		*/
/*	IN:	speed		The frame number before the stop 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerStop(speed)					\
	MusStop(MUSFLAG_SONGS, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetMasterVol - Setting volume of the whole sequence 	*/
/*	IN:	vol		Volume  (0 - 0x7fff)					*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetMasterVol(vol)				\
	MusSetMasterVolume(MUSFLAG_SONGS, vol)


/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerGetSeqState - Get the state of sequence 			*/
/*	IN:	handle		The sequence handler 				*/
/*	RET:	The channel number which is currently playing 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerGetSeqState(handle)				\
	MusHandleAsk(handle)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSeqStop - Stop the performance of sequence 		*/
/*	IN:	handle		The sequence handler 				*/
/*		speed		The frame number before the stop 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSeqStop(handle, speed)				\
	MusHandleStop(handle, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetSeqTempo - Setting tempo of sequence 			*/
/*	IN:	handle		The sequence handler 				*/
/*		tempo		Tempo  (0 - 0x100)					*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetSeqTempo(handle, tempo)			\
	MusHandleSetTempo(handle, tempo)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetSeqVol - Setting volume of sequence 			*/
/*	IN:	handle		The sequence handler 				*/
/*		vol		The volume scale  (0 - 0x100)				*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetSeqVol(handle, vol)				\
	MusHandleSetVolume(handle, vol)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetSeqPan - Setting the pan position of sequence 	*/
/*	IN:	handle		The sequence handler 				*/
/*		pan		The pan scale 						*/
/*				0=Left 0x80=The center  0x100=Right 		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetSeqPan(handle, pan)				\
	MusHandleSetPan(handle, pan )


/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerGetState - Get the state of the sound player 		*/
/*	IN:	None 										*/
/*	RET:	The channel number which is currently playing 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerGetState()					\
	MusAsk(MUSFLAG_EFFECTS)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerStop - Stop playback of all sound 				*/
/*	IN:	speed		The frame number before the stop 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerStop(speed)					\
	MusStop(MUSFLAG_EFFECTS, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetMasterVol - Setting volume of the whole sound 	*/
/*	IN:	vol		Volume  (0 - 0x7fff)					*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetMasterVol(vol)				\
	MusSetMasterVolume(MUSFLAG_EFFECTS, vol)


/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerGetSndState - Get the state of sound 			*/
/*	IN:	handle		The sound handler 				*/
/*		None 										*/
/*	RET:	The channel number which is currently playing 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerGetSndState(handle)				\
	MusHandleAsk(handle)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSndStop - Stop the sound performance 			*/
/*	IN:	handle		The sound handler 				*/
/*		speed		The frame number before the stop 			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSndStop(handle, speed)				\
	MusHandleStop(handle, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetSndVol - Setting volume of sound 			*/
/*	IN:	handle		The sound handler 				*/
/*		vol		The volume scale  (0 - 0x100)				*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetSndVol(handle, vol)				\
	MusHandleSetVolume(handle, vol)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetSndPan - Setting the pan position of sound 		*/
/*	IN:	handle		The sound handler 				*/
/*		pan		The pan scale 						*/
/*				0=Left  0x80=The center  0x100=Right 		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetSndPan(handle, pan)				\
	MusHandleSetPan(handle, pan )

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetSndPitch - Setting sound pitch 			*/
/*	IN:	handle		The sound handler 			*/
/*		pitch		The pitch value  (-6.0 - +6.0)			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetSndPitch(handle, pitch)			\
	MusHandleSetFreqOffset(handle, pitch)


/*----------------------------------------------------------------------*/    
#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */
#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif /* _NUALSTL_H__ */
