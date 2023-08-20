#ifndef _OS_H_
#define	_OS_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include "ultratypes.h"

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

typedef s32	OSPri;
typedef s32	OSId;
typedef union	{ struct { f32 f_odd; f32 f_even; } f; f64 d; }	__OSfp;

typedef struct {
	u64	at, v0, v1, a0, a1, a2, a3;
	u64	t0, t1, t2, t3, t4, t5, t6, t7;
	u64	s0, s1, s2, s3, s4, s5, s6, s7;
	u64	t8, t9,         gp, sp, s8, ra;
	u64	lo, hi;
	u32	sr, pc, cause, badvaddr, rcp;
	u32	fpcsr;
	__OSfp	 fp0,  fp2,  fp4,  fp6,  fp8, fp10, fp12, fp14;
	__OSfp	fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;

typedef struct OSThread_s {
	struct OSThread_s	*next;		
	OSPri			priority;	
	struct OSThread_s	**queue;	
	struct OSThread_s	*tlnext;	
	u16			state;	
	u16			flags;		
	OSId			id;		
	int			fp;		
	__OSThreadContext	context;	
} OSThread;

typedef u32 OSEvent;
typedef u32 OSIntMask;
typedef u32 OSPageMask;
typedef u32 OSHWIntr;

typedef void *	OSMesg;

typedef struct OSMesgQueue_s {
	OSThread	*mtqueue;	
	OSThread	*fullqueue;	
	s32		validCount;
	s32		first;	
	s32		msgCount;
	OSMesg		*msg;	
} OSMesgQueue;

typedef struct {
	u32		errStatus;
        void     	*dramAddr;    
	void		*C2Addr;
	u32		sectorSize;	
	u32		C1ErrNum;	
	u32		C1ErrSector[4];	
} __OSBlockInfo;

typedef struct {
	u32     	cmdType;       	
	u16     	transferMode;   
	u16		blockNum;	
	s32     	sectorNum;    
	u32     	devAddr;     
	u32		bmCtlShadow;	
	u32		seqCtlShadow;
	__OSBlockInfo	block[2];	
} __OSTranxInfo;


typedef struct OSPiHandle_s {
        struct OSPiHandle_s     *next;  
        u8                      type;   
        u8                      latency;       
        u8                      pageSize;     
        u8                      relDuration;   
        u8                      pulse;        
		u8						domain;		
        u32                     baseAddress;    
        u32                     speed;        
        __OSTranxInfo           transferInfo;	
} OSPiHandle;

typedef struct {
        u8      type;
        u32     address;
} OSPiInfo;

typedef struct {
        u16 		type;	
        u8 		pri;		
        u8		status;		
	OSMesgQueue	*retQueue;	
} OSIoMesgHdr;

typedef struct {
	OSIoMesgHdr	hdr;		
	void *		dramAddr;
	u32		devAddr;
	u32 		size;		
	OSPiHandle	*piHandle;	
} OSIoMesg;

typedef struct {
        s32             active;		
	OSThread	*thread;	
        OSMesgQueue  	*cmdQueue;	
        OSMesgQueue  	*evtQueue;
        OSMesgQueue  	*acsQueue;
        s32             (*dma)(s32, u32, void *, u32);
        s32             (*edma)(OSPiHandle *, s32, u32, void *, u32);
} OSDevMgr;

typedef struct {
    u32	ctrl;
    u32	width;
    u32	burst;
    u32	vSync;
    u32	hSync;
    u32	leap;
    u32	hStart;
    u32	xScale;
    u32	vCurrent;
} OSViCommonRegs;


typedef struct {
    u32	origin;
    u32	yScale;
    u32	vStart;	
    u32	vBurst;
    u32	vIntr;
} OSViFieldRegs;

typedef struct {
    u8			type;		
    OSViCommonRegs	comRegs;	
    OSViFieldRegs	fldRegs[2];	
} OSViMode;

typedef u64	OSTime;

typedef struct OSTimer_s {
	struct OSTimer_s	*next;	
	struct OSTimer_s	*prev;	
	OSTime			interval;	
	OSTime			value;	
	OSMesgQueue		*mq;		
	OSMesg			msg;		
} OSTimer;

typedef struct {
	u16     type;                
	u8      status;               
	u8	errno;
}OSContStatus;

typedef struct {
	u16     button;
	s8      stick_x;		/* -80 <= stick_x <= 80 */
	s8      stick_y;		/* -80 <= stick_y <= 80 */
	u8	errno;
} OSContPad;

typedef struct {
	void    *address;             
	u8      databuffer[32];         
        u8      addressCrc;          
	u8      dataCrc;                
	u8	errno;
} OSContRamIo;

typedef struct {
	int		status;
	OSMesgQueue 	*queue;
	int		channel;
	u8		id[32];
	u8		label[32];
	int		version;
	int		dir_size;
	int		inode_table;	
	int		minode_table;		
	int		dir_table;		
	int		inode_start_page;	
	u8		banks;
	u8		activebank;
} OSPfs;

typedef struct {
	u32	file_size;	
  	u32 	game_code;
  	u16 	company_code;
  	char  	ext_name[4];
  	char 	game_name[16];
} OSPfsState;

typedef struct {
	u16	*histo_base;	
	u32	histo_size;		
	u32	*text_start;		
	u32	*text_end;		
} OSProf;

#endif 

#define OS_STATE_STOPPED	1
#define OS_STATE_RUNNABLE	2
#define OS_STATE_RUNNING	4
#define OS_STATE_WAITING	8

#ifdef _FINALROM
#define OS_NUM_EVENTS           15
#else
#define OS_NUM_EVENTS           23
#endif

#define OS_EVENT_SW1              0    
#define OS_EVENT_SW2              1     
#define OS_EVENT_CART             2    
#define OS_EVENT_COUNTER          3    
#define OS_EVENT_SP               4    
#define OS_EVENT_SI               5     
#define OS_EVENT_AI               6    
#define OS_EVENT_VI               7    
#define OS_EVENT_PI               8     
#define OS_EVENT_DP               9     
#define OS_EVENT_CPU_BREAK        10  
#define OS_EVENT_SP_BREAK         11   
#define OS_EVENT_FAULT            12  
#define OS_EVENT_THREADSTATUS     13    
#define OS_EVENT_PRENMI           14  
#ifndef _FINALROM
#define OS_EVENT_RDB_READ_DONE    15  
#define OS_EVENT_RDB_LOG_DONE     16   
#define OS_EVENT_RDB_DATA_DONE    17  
#define OS_EVENT_RDB_REQ_RAMROM   18   
#define OS_EVENT_RDB_FREE_RAMROM  19   
#define OS_EVENT_RDB_DBG_DONE     20
#define OS_EVENT_RDB_FLUSH_PROF   21
#define OS_EVENT_RDB_ACK_PROF     22
#endif

#define	OS_FLAG_CPU_BREAK	1	
#define	OS_FLAG_FAULT		2	

#define	OS_IM_NONE	0x00000001
#define	OS_IM_SW1	0x00000501
#define	OS_IM_SW2	0x00000601
#define	OS_IM_CART	0x00000c01
#define	OS_IM_PRENMI	0x00001401
#define OS_IM_RDBWRITE	0x00002401
#define OS_IM_RDBREAD	0x00004401
#define	OS_IM_COUNTER	0x00008401
#define	OS_IM_CPU	0x0000ff01
#define	OS_IM_SP	0x00010401
#define	OS_IM_SI	0x00020401
#define	OS_IM_AI	0x00040401
#define	OS_IM_VI	0x00080401
#define	OS_IM_PI	0x00100401
#define	OS_IM_DP	0x00200401
#define	OS_IM_ALL	0x003fff01
#define	RCP_IMASK	0x003f0000
#define	RCP_IMASKSHIFT	16

#define OS_PRIORITY_MAX		255
#define OS_PRIORITY_VIMGR	254
#define OS_PRIORITY_RMON	250
#define OS_PRIORITY_RMONSPIN	200
#define OS_PRIORITY_PIMGR	150
#define OS_PRIORITY_SIMGR	140
#define	OS_PRIORITY_APPMAX	127
#define OS_PRIORITY_IDLE	  0	

#define	OS_MESG_NOBLOCK		0
#define	OS_MESG_BLOCK		1

#define	OS_READ			0		
#define	OS_WRITE		1		
#define	OS_OTHERS		2		

#define OS_MESG_TYPE_BASE	(10)
#define OS_MESG_TYPE_LOOPBACK	(OS_MESG_TYPE_BASE+0)
#define OS_MESG_TYPE_DMAREAD	(OS_MESG_TYPE_BASE+1)
#define OS_MESG_TYPE_DMAWRITE	(OS_MESG_TYPE_BASE+2)
#define OS_MESG_TYPE_VRETRACE	(OS_MESG_TYPE_BASE+3)
#define OS_MESG_TYPE_COUNTER	(OS_MESG_TYPE_BASE+4)
#define OS_MESG_TYPE_EDMAREAD	(OS_MESG_TYPE_BASE+5)
#define OS_MESG_TYPE_EDMAWRITE	(OS_MESG_TYPE_BASE+6)

#define OS_MESG_PRI_NORMAL	0
#define OS_MESG_PRI_HIGH	1

#define OS_PM_4K	0x0000000
#define OS_PM_16K	0x0006000
#define OS_PM_64K	0x001e000
#define OS_PM_256K	0x007e000
#define OS_PM_1M	0x01fe000
#define OS_PM_4M	0x07fe000
#define OS_PM_16M	0x1ffe000

#define OS_PIM_STACKSIZE	4096
#define OS_VIM_STACKSIZE	4096
#define OS_SIM_STACKSIZE	4096

#define	OS_MIN_STACKSIZE	72

#define	OS_TV_PAL		0
#define	OS_TV_NTSC		1
#define	OS_TV_MPAL		2

#define OS_VI_NTSC_LPN1		0	
#define OS_VI_NTSC_LPF1		1
#define OS_VI_NTSC_LAN1		2
#define OS_VI_NTSC_LAF1		3
#define OS_VI_NTSC_LPN2		4
#define OS_VI_NTSC_LPF2		5
#define OS_VI_NTSC_LAN2		6
#define OS_VI_NTSC_LAF2		7
#define OS_VI_NTSC_HPN1		8
#define OS_VI_NTSC_HPF1		9
#define OS_VI_NTSC_HAN1		10
#define OS_VI_NTSC_HAF1		11
#define OS_VI_NTSC_HPN2		12
#define OS_VI_NTSC_HPF2		13

#define OS_VI_PAL_LPN1		14	
#define OS_VI_PAL_LPF1		15
#define OS_VI_PAL_LAN1		16
#define OS_VI_PAL_LAF1		17
#define OS_VI_PAL_LPN2		18
#define OS_VI_PAL_LPF2		19
#define OS_VI_PAL_LAN2		20
#define OS_VI_PAL_LAF2		21
#define OS_VI_PAL_HPN1		22
#define OS_VI_PAL_HPF1		23
#define OS_VI_PAL_HAN1		24
#define OS_VI_PAL_HAF1		25
#define OS_VI_PAL_HPN2		26
#define OS_VI_PAL_HPF2		27

#define OS_VI_MPAL_LPN1		28	
#define OS_VI_MPAL_LPF1		29
#define OS_VI_MPAL_LAN1		30
#define OS_VI_MPAL_LAF1		31
#define OS_VI_MPAL_LPN2		32
#define OS_VI_MPAL_LPF2		33
#define OS_VI_MPAL_LAN2		34
#define OS_VI_MPAL_LAF2		35
#define OS_VI_MPAL_HPN1		36
#define OS_VI_MPAL_HPF1		37
#define OS_VI_MPAL_HAN1		38
#define OS_VI_MPAL_HAF1		39
#define OS_VI_MPAL_HPN2		40
#define OS_VI_MPAL_HPF2		41

#define	OS_VI_GAMMA_ON			0x0001
#define	OS_VI_GAMMA_OFF			0x0002
#define	OS_VI_GAMMA_DITHER_ON		0x0004
#define	OS_VI_GAMMA_DITHER_OFF		0x0008
#define	OS_VI_DIVOT_ON			0x0010
#define	OS_VI_DIVOT_OFF			0x0020
#define	OS_VI_DITHER_FILTER_ON		0x0040
#define	OS_VI_DITHER_FILTER_OFF		0x0080

#define OS_VI_BIT_NONINTERLACE		0x0001        
#define OS_VI_BIT_INTERLACE		0x0002       
#define OS_VI_BIT_NORMALINTERLACE	0x0004        
#define OS_VI_BIT_DEFLICKINTERLACE	0x0008        
#define OS_VI_BIT_ANTIALIAS		0x0010
#define OS_VI_BIT_POINTSAMPLE		0x0020
#define OS_VI_BIT_16PIXEL		0x0040
#define OS_VI_BIT_32PIXEL		0x0080
#define OS_VI_BIT_LORES			0x0100
#define OS_VI_BIT_HIRES			0x0200
#define OS_VI_BIT_NTSC			0x0400
#define OS_VI_BIT_PAL			0x0800

#define LEO_BLOCK_MODE	1
#define LEO_TRACK_MODE	2
#define LEO_SECTOR_MODE	3

#ifndef _HW_VERSION_1
#define MAXCONTROLLERS  4
#else
#define MAXCONTROLLERS  6
#endif

#define CONT_NO_RESPONSE_ERROR          0x8
#define CONT_OVERRUN_ERROR              0x4
#ifdef _HW_VERSION_1
#define CONT_FRAME_ERROR                0x2
#define CONT_COLLISION_ERROR            0x1
#endif 

#define CONT_ABSOLUTE           0x0001
#define CONT_RELATIVE           0x0002
#define CONT_JOYPORT            0x0004
#define CONT_EEPROM		0x8000


#define CONT_CARD_ON            0x01
#define CONT_CARD_PULL          0x02
#define CONT_ADDR_CRC_ER        0x04
#define CONT_EEPROM_BUSY	0x80

#define CONT_A      0x8000
#define CONT_B      0x4000
#define CONT_G	    0x2000
#define CONT_START  0x1000
#define CONT_UP     0x0800
#define CONT_DOWN   0x0400
#define CONT_LEFT   0x0200
#define CONT_RIGHT  0x0100
#define CONT_L      0x0020
#define CONT_R      0x0010
#define CONT_E      0x0008
#define CONT_D      0x0004
#define CONT_C      0x0002
#define CONT_F      0x0001

/* Nintendo's official button names */

#define A_BUTTON	CONT_A
#define B_BUTTON	CONT_B
#define L_TRIG		CONT_L
#define R_TRIG		CONT_R
#define Z_TRIG		CONT_G
#define START_BUTTON	CONT_START
#define U_JPAD		CONT_UP
#define L_JPAD		CONT_LEFT
#define R_JPAD		CONT_RIGHT
#define D_JPAD		CONT_DOWN
#define U_CBUTTONS	CONT_E
#define L_CBUTTONS	CONT_C
#define R_CBUTTONS	CONT_F
#define D_CBUTTONS	CONT_D

#define PFS_FILE_NAME_LEN       16
#define PFS_FILE_EXT_LEN        4
#define BLOCKSIZE		32		
#define PFS_ONE_PAGE            8		
#define PFS_MAX_BANKS		62

#define PFS_READ                0
#define PFS_WRITE               1
#define PFS_CREATE              2

#define PFS_INITIALIZED		0x1
#define PFS_CORRUPTED		0x2		


#define PFS_ERR_NOPACK		1	
#define PFS_ERR_NEW_PACK        2	
#define PFS_ERR_INCONSISTENT    3	
#define PFS_ERR_CONTRFAIL	CONT_OVERRUN_ERROR              
#define PFS_ERR_INVALID         5	
#define PFS_ERR_BAD_DATA        6      
#define PFS_DATA_FULL           7   
#define PFS_DIR_FULL            8	
#define PFS_ERR_EXIST		9	
#define PFS_ERR_ID_FATAL	10	
#define PFS_ERR_DEVICE		11	

#define EEPROM_MAXBLOCKS	64
#define EEPROM_BLOCK_SIZE	8

#define PROF_MIN_INTERVAL	50

#define	BOOT_ADDRESS_ULTRA	0x80000400
#define	BOOT_ADDRESS_COSIM	0x80002000
#define	BOOT_ADDRESS_EMU	0x20010000
#define	BOOT_ADDRESS_INDY 	0x88100000

#define OS_APP_NMI_BUFSIZE	64

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

#ifdef PTN64
#define osReadHost osReadHost_pt
#define osWriteHost osWriteHost_pt
#endif

#define MQ_GET_COUNT(mq)        ((mq)->validCount)

#define MQ_IS_EMPTY(mq)		(MQ_GET_COUNT(mq) == 0)
#define MQ_IS_FULL(mq)		(MQ_GET_COUNT(mq) >= (mq)->msgCount)

extern u64 osClockRate;

#define OS_NSEC_TO_CYCLES(n)	(((u64)(n)*(osClockRate))/1000000000LL)
#define OS_USEC_TO_CYCLES(n)	(((u64)(n)*(osClockRate))/1000000LL)
#define OS_CYCLES_TO_NSEC(c)	(((u64)(c)*1000000000LL)/(osClockRate))
#define OS_CYCLES_TO_USEC(c)	(((u64)(c)*1000000LL)/(osClockRate))



extern OSViMode	osViModeTable[];	

extern OSViMode	osViModeNtscLpn1;	
extern OSViMode	osViModeNtscLpf1;
extern OSViMode	osViModeNtscLan1;
extern OSViMode	osViModeNtscLaf1;
extern OSViMode	osViModeNtscLpn2;
extern OSViMode	osViModeNtscLpf2;
extern OSViMode	osViModeNtscLan2;
extern OSViMode	osViModeNtscLaf2;
extern OSViMode	osViModeNtscHpn1;
extern OSViMode	osViModeNtscHpf1;
extern OSViMode	osViModeNtscHan1;
extern OSViMode	osViModeNtscHaf1;
extern OSViMode	osViModeNtscHpn2;
extern OSViMode	osViModeNtscHpf2;

extern OSViMode	osViModePalLpn1;	
extern OSViMode	osViModePalLpf1;
extern OSViMode	osViModePalLan1;
extern OSViMode	osViModePalLaf1;
extern OSViMode	osViModePalLpn2;
extern OSViMode	osViModePalLpf2;
extern OSViMode	osViModePalLan2;
extern OSViMode	osViModePalLaf2;
extern OSViMode	osViModePalHpn1;
extern OSViMode	osViModePalHpf1;
extern OSViMode	osViModePalHan1;
extern OSViMode	osViModePalHaf1;
extern OSViMode	osViModePalHpn2;
extern OSViMode	osViModePalHpf2;

extern OSViMode	osViModeMpalLpn1;	
extern OSViMode	osViModeMpalLpf1;
extern OSViMode	osViModeMpalLan1;
extern OSViMode	osViModeMpalLaf1;
extern OSViMode	osViModeMpalLpn2;
extern OSViMode	osViModeMpalLpf2;
extern OSViMode	osViModeMpalLan2;
extern OSViMode	osViModeMpalLaf2;
extern OSViMode	osViModeMpalHpn1;
extern OSViMode	osViModeMpalHpf1;
extern OSViMode	osViModeMpalHan1;
extern OSViMode	osViModeMpalHaf1;
extern OSViMode	osViModeMpalHpn2;
extern OSViMode	osViModeMpalHpf2;

extern s32 	osRomType;	
extern void 	*osRomBase;	
extern s32 	osTvType;
extern s32 	osResetType;	
extern s32 	osCicId;
extern s32 	osVersion;
extern u32	osMemSize;	
extern s32	osAppNMIBuffer[];

extern OSIntMask __OSGlobalIntMask;	
extern OSPiHandle      *__osPiTable;    
extern OSPiHandle      *__osDiskHandle;


extern void		osCreateThread(OSThread *, OSId, void (*)(void *),
				       void *, void *, OSPri);
extern void		osDestroyThread(OSThread *);
extern void		osYieldThread(void);
extern void		osStartThread(OSThread *);
extern void		osStopThread(OSThread *);
extern OSId		osGetThreadId(OSThread *);
extern void		osSetThreadPri(OSThread *, OSPri);
extern OSPri		osGetThreadPri(OSThread *);

extern void		osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32		osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32		osJamMesg(OSMesgQueue *, OSMesg, s32);
extern s32		osRecvMesg(OSMesgQueue *, OSMesg *, s32);

extern void		osSetEventMesg(OSEvent, OSMesgQueue *, OSMesg);

extern OSIntMask	osGetIntMask(void);
extern OSIntMask	osSetIntMask(OSIntMask);

extern void             osInitRdb(u8 *sendBuf, u32 sendSize);

extern void		osInvalDCache(void *, s32);
extern void		osInvalICache(void *, s32);
extern void		osWritebackDCache(void *, s32);
extern void		osWritebackDCacheAll(void);

#define	OS_DCACHE_ROUNDUP_ADDR(x)	(void *)(((((u32)(x)+0xf)/0x10)*0x10))
#define	OS_DCACHE_ROUNDUP_SIZE(x)	(u32)(((((u32)(x)+0xf)/0x10)*0x10))

extern void		osMapTLB(s32, OSPageMask, void *, u32, u32, s32);
extern void		osMapTLBRdb(void);
extern void		osUnmapTLB(s32);
extern void		osUnmapTLBAll(void);
extern void		osSetTLBASID(s32);

extern u32		 osVirtualToPhysical(void *);
extern void *		 osPhysicalToVirtual(u32);

#define	OS_K0_TO_PHYSICAL(x)	(u32)(((char *)(x)-0x80000000))
#define	OS_K1_TO_PHYSICAL(x)	(u32)(((char *)(x)-0xa0000000))

#define	OS_PHYSICAL_TO_K0(x)	(void *)(((u32)(x)+0x80000000))
#define	OS_PHYSICAL_TO_K1(x)	(void *)(((u32)(x)+0xa0000000))

extern u32 		osAiGetStatus(void);
extern u32 		osAiGetLength(void);
extern s32		osAiSetFrequency(u32);
extern s32		osAiSetNextBuffer(void *, u32);

extern u32 		osDpGetStatus(void);
extern void		osDpSetStatus(u32);
extern void 		osDpGetCounters(u32 *);
extern s32		osDpSetNextBuffer(void *, u64);

extern u32 		osPiGetStatus(void);
extern s32		osPiGetDeviceType(void);
extern s32		osPiRawWriteIo(u32, u32);
extern s32		osPiRawReadIo(u32, u32 *);
extern s32		osPiRawStartDma(s32, u32, void *, u32);
extern s32		osPiWriteIo(u32, u32);
extern s32		osPiReadIo(u32, u32 *);
extern s32		osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32,
				     OSMesgQueue *);
extern void		osCreatePiManager(OSPri, OSMesgQueue *, OSMesg *, s32);

extern u32		osViGetStatus(void);
extern u32		osViGetCurrentMode(void);
extern u32		osViGetCurrentLine(void);
extern u32		osViGetCurrentField(void);
extern void		*osViGetCurrentFramebuffer(void);
extern void		*osViGetNextFramebuffer(void);
extern void		osViSetXScale(f32);
extern void		osViSetYScale(f32);
extern void		osViSetSpecialFeatures(u32);
extern void		osViSetMode(OSViMode *);
extern void		osViSetEvent(OSMesgQueue *, OSMesg, u32);
extern void		osViSwapBuffer(void *);
extern void		osViBlack(u8);
extern void		osViFade(u8, u16);
extern void		osRepeatLine(u8);
extern void		osCreateViManager(OSPri);

extern OSTime		osGetTime(void);
extern void		osSetTime(OSTime);
extern int		osSetTimer(OSTimer *, OSTime, OSTime,
				   OSMesgQueue *, OSMesg);
extern int		osStopTimer(OSTimer *);

extern s32		osContInit(OSMesgQueue *, u8 *, OSContStatus *);
extern s32		osContReset(OSMesgQueue *, OSContStatus *);
extern s32		osContStartQuery(OSMesgQueue *);
extern s32		osContStartReadData(OSMesgQueue *);
#ifndef _HW_VERSION_1
extern s32		osContSetCh(u8);
#endif
extern void		osContGetQuery(OSContStatus *);
extern void		osContGetReadData(OSContPad *);

extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsRepairId(OSPfs *);
extern s32 osPfsInit(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsReFormat(OSPfs *, OSMesgQueue *, int);
extern s32 osPfsChecker(OSPfs *);
extern s32 osPfsAllocateFile(OSPfs *, u16, u32, u8 *, u8 *, int, s32 *);
extern s32 osPfsFindFile(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 osPfsDeleteFile(OSPfs *, u16, u32, u8 *, u8 *);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 osPfsGetLabel(OSPfs *, u8 *, int *);
extern s32 osPfsSetLabel(OSPfs *, u8 *);
extern s32 osPfsIsPlug(OSMesgQueue *, u8 *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);
extern s32 osPfsReSizeFile(OSPfs *, u16 , u32 , u8 *, u8 *, int);

extern s32 osEepromProbe(OSMesgQueue *);
extern s32 osEepromRead(OSMesgQueue *, u8, u8 *);
extern s32 osEepromWrite(OSMesgQueue *, u8, u8 *);
extern s32 osEepromLongRead(OSMesgQueue *, u8, u8 *, int);
extern s32 osEepromLongWrite(OSMesgQueue *, u8, u8 *, int);

extern s32 osMotorInit(OSMesgQueue *, OSPfs *, int);
extern s32 osMotorStop(OSPfs *);
extern s32 osMotorStart(OSPfs *);

extern OSPiHandle *osCartRomInit(void);
extern OSPiHandle *osLeoDiskInit(void);
extern OSPiHandle *osDriveRomInit(void);

extern s32 osEPiDeviceType(OSPiHandle *, OSPiInfo *);
extern s32 osEPiRawWriteIo(OSPiHandle *, u32 , u32);
extern s32 osEPiRawReadIo(OSPiHandle *, u32 , u32 *);
extern s32 osEPiRawStartDma(OSPiHandle *, s32 , u32 , void *, u32 );
extern s32 osEPiWriteIo(OSPiHandle *, u32 , u32 );
extern s32 osEPiReadIo(OSPiHandle *, u32 , u32 *);
extern s32 osEPiStartDma(OSPiHandle *, OSIoMesg *, s32);

extern void		osProfileInit(OSProf *, u32 profcnt);
extern void		osProfileStart(u32);
extern void		osProfileFlush(void);
extern void		osProfileStop(void);

extern s32		osTestHost(void);
extern void		osReadHost(void *, u32);
extern void		osWriteHost(void *, u32);
extern void		osAckRamromRead(void);
extern void		osAckRamromWrite(void);

extern void     bcopy(const void *, void *, int);
extern int      bcmp(const void *, const void *, int);
extern void     bzero(void *, int);

extern void		osInitialize(void);
extern u32		osGetCount(void);
extern void		osExit(void);

extern int		sprintf(char *s, const char *fmt, ...);
extern void		osSyncPrintf(const char *fmt, ...);
extern void		osAsyncPrintf(const char *fmt, ...);
extern int		osSyncGetChars(char *buf);
extern int		osAsyncGetChars(char *buf);

#endif 

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif