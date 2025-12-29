#ifndef _NUSYS_H_
#define _NUSYS_H_

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif  /* F3DEX_GBI */

#include <ultra64.h>
#include <PR/gs2dex.h>
#endif

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	/*	DEFINE										*/
	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
#define NU_VERSION		"1.0"

/*--------------------------------------*/
/* NuSystem spec define		 	    */
/*--------------------------------------*/
#define NU_SPEC_BOOT_STACK_SIZE	0x2000
#define NU_SPEC_BOOT_ADDR		0x80025c00
#define NU_SPEC_BOOT_STACK		nuMainStack + NU_SPEC_BOOT_STACK_SIZE

/*--------------------------------------*/
/* Thread ID for DEBUG (To make sure)   */
/*--------------------------------------*/
#define NU_IDLE_THREAD_ID	1
#define NU_ROMN_THERAD_ID	2	/* no use */
#define NU_MAIN_THREAD_ID	3


/*--------------------------------------*/
/* NuSYS Thread Priority		    */
/*--------------------------------------*/
#define NU_IDLE_THREAD_PRI	OS_PRIORITY_IDLE	/* Idle thread	*/
#define NU_RMON_THREAD_PRI	OS_PRIORITY_RMON	/* no use	*/
#define NU_MAIN_THREAD_PRI	10			/* main thread	*/

/*--------------------------------------*/
/* NUSYS STACK SIZE			    */
/*--------------------------------------*/
#define NU_IDLE_STACK_SIZE	0x2000			/* Idle thread */
#define NU_MAIN_STACK_SIZE	NU_SPEC_BOOT_STACK_SIZE	/* Main thread */


/*--------------------------------------*/
/* MESSAGE QUEUE				    */
/*--------------------------------------*/
#define NU_PI_MESG_NUM		50		/* PI Message buffer size */


/*----------------------------------------------------------------------*/
/* SCHEDULER DEFINE								 	*/
/*----------------------------------------------------------------------*/
#define NU_SC_STACK_SIZE	0x2000		/* The thread stack size */
#define NU_SC_SWAPBUFFER	0x0001		/* Switching frame buffers */
#define NU_SC_NOSWAPBUFFER	0x0000		/* No switching */
#define NU_SC_NORDP		0x0002		/* Don't want RDP to end */
#define NU_SC_RETRACE_MSG	0x0001		/* The retrace message */
#define NU_SC_PRENMI_MSG	0x0002		/* The NMI message */
#define NU_SC_SWAPBUFFER_MSG	0x0004		/* Switching frame buffers  */
						/* The message */
#define NU_SC_GTASKEND_MSG	0x0008		/* The task end message */
#define NU_SC_MAX_MESGS		8			/* The message buffer size */

#define NU_SC_HANDLER_PRI	120	/* EVENT HANDLER THREAD PRORITY */
#define NU_SC_AUDIO_PRI		110	/* AUDIO DISPATCHER THREAD PRORITY */
#define NU_SC_GRAPHICS_PRI	100	/* GFX DISPATCHER THREAD PRORITY */

/*----------------------------------------------------------------------*/
/* GRAFIC THREAD DEFINE									*/
/*----------------------------------------------------------------------*/
#define NU_GFX_THREAD_ID		4	
#define NU_GFX_TASKMGR_THREAD_ID 	5
#define NU_GFX_STACK_SIZE		0x2000	/* The thread stack size */
#define	NU_GFX_MESGS		8		/* The GFX message queue */
#define NU_GFX_TASKMGR_STACK_SIZE 	0x2000	/* The stack size */
#define	NU_GFX_TASKMGR_MESGS	8	/* The queue of the task manager */
#define NU_GFX_THREAD_PRI		50	/* Priority of the GFX thread */
#define NU_GFX_TASKMGR_THREAD_PRI 	60	/* Priority of the task manager */

/*----------------------------------------------------------------------*/
/* GRAFIC MANEGER DEFINE								*/
/*----------------------------------------------------------------------*/
#define	NU_GFX_UCODE_F3DEX2	0		/* F3DEX microcode  */
#define	NU_GFX_UCODE_F3DEX2_NON	1		/* F3DEX.NoN microcode  */
#define	NU_GFX_UCODE_F3DEX2_REJ	2		/* F3DEX.NoN microcode  */
#define	NU_GFX_UCODE_F3DLX2_REJ	3		/* F3DEX.ReJ microcode  */
#define	NU_GFX_UCODE_L3DEX2	4		/* L3DEX microcode  */
#define	NU_GFX_UCODE_S2DEX2	5		/* S2DEX microcode  */

#define	NU_GFX_UCODE_F3DEX	0		/*  For compatibility		*/
#define	NU_GFX_UCODE_F3DEX_NON	1		/*  For compatibility		*/
#define	NU_GFX_UCODE_F3DLX	0		/*  For compatibility		*/
#define	NU_GFX_UCODE_F3DLX_NON	1		/*  For compatibility		*/
#define	NU_GFX_UCODE_F3DLX_REJ	4		/*  For compatibility		*/
#define	NU_GFX_UCODE_F3DLP_REJ	3		/*  For compatibility		*/
#define	NU_GFX_UCODE_L3DEX	4		/*  For compatibility		*/
#define	NU_GFX_UCODE_S2DEX	5		/*  For compatibility		*/
#define	NU_GFX_UCODE_S2DEX_D	5		/*  For compatibility		*/


// #define	NU_GFX_UCODE_L3DEX	6		/* L3DEX micro code */
// #define	NU_GFX_UCODE_S2DEX	7		/* S2DEX micro code */
// #define	NU_GFX_UCODE_S2DEX_D	8		/* S2DEX micro code */

/*--------------------------------------*/
/* The number of graphic task structure */
/* should be message buffer size of     */
/* scheduler or more. 			    */
/* Otherwise, it's possible to use task */
/* structure which is being used. 	    */
/*--------------------------------------*/
#define NU_GFX_TASK_NUM			10		/* number of graphic task structures */
#define NU_GFX_RDP_OUTPUTBUFF_SIZE	0x20000	/* fifo buffer size */

/*--------------------------------------*/
/* Default frame buffer 		    */
/* In default, frame buffer is located  */
/* on the highest of RDRAM. 		    */
/* Set to 320x240, 16-bit triple buffer */
/*--------------------------------------*/
#define NU_GFX_FRAMEBUFFER_NUM	3
#define NU_GFX_FRAMEBUFFER_ADDR	(0x80400000-320*240*2*3)
#define NU_GFX_FRAMEBUFFER0_ADDR	(NU_GFX_FRAMEBUFFER_ADDR)
#define NU_GFX_FRAMEBUFFER1_ADDR	(NU_GFX_FRAMEBUFFER0_ADDR+320*240*2)
#define NU_GFX_FRAMEBUFFER2_ADDR	(NU_GFX_FRAMEBUFFER1_ADDR+320*240*2)

#define NU_GFX_INIT_SCREEN_WD		320
#define NU_GFX_INIT_SCREEN_HT		240

/*--------------------------------------*/
/* In default, the Z-buffer is located  */
/*  on the lowest in memory 		    */
/*--------------------------------------*/
#define NU_GFX_ZBUFFER_ADDR		0x80000400

#define	NU_GFX_DISPLAY_ON		1	/* display is ON */
#define	NU_GFX_DISPLAY_OFF	0	/* display is OFF */


/*----------------------------------------------------------------------*/
/* CONTROLLER MANEGER DEFINE								*/
/*----------------------------------------------------------------------*/
#define NU_CONT_MAXCONTROLLERS	MAXCONTROLLERS
#define NU_CONT_STACK_SIZE		0x2000
#define NU_CONT_MESG_MAX		8
#define NU_CONT_THREAD_ID		5
#define NU_CONT_THREAD_PRI		115
#define NU_CONT_DATA_UNLOCK		0
#define NU_CONT_DATA_LOCK		1

/* Define the message to the controller manager */
#define NU_CONT_READ_MSG		0x1000	/* Read synchronization */
#define NU_CONT_READ_NW_MSG		0x1001	/* Read asynchronization */
#define NU_CONT_QUERY_MSG		0x1002	/* Get state of controller */
#define NU_CONT_PAK_OPEN_MSG		0x2001	/* Open device */
#define NU_CONT_PAK_FREE_MSG		0x2002	/* Get space area */
#define NU_CONT_PAK_FOPEN_MSG		0x2003	/* Open fill */
#define NU_CONT_PAK_READWRITE_MSG	0x2004	/* Read/Write fill */
#define NU_CONT_PAK_FILEDELETE_MSG	0x2005	/* Delete fill */
#define NU_CONT_PAK_FILESTATE_MSG	0x2006	/* Get state of fill */
#define NU_CONT_PAK_FILENUM_MSG	0x2007	/* Get note information of pack */
#define NU_CONT_PAK_REPAIRID_MSG	0x2008	/* Repair pack */
#define NU_CONT_PAK_RESIZE_MSG	0x2009	/* Change file size */


#define NU_CONT_PAK_MODE_NOCREATE	0
#define NU_CONT_PAK_MODE_CREATE	1
#define NU_CONT_PAK_TYPE_NONE		0
#define NU_CONT_PAK_TYPE_PAK		1		/* controller pack */
#define NU_CONT_PAK_TYPE_RUMBLE	2		/* oscillating pack */
#define NU_CONT_PAK_READ		PFS_READ
#define NU_CONT_PAK_WRITE		PFS_WRITE

/* �U���p�b�N���� */
#define NU_CONT_RMB_STATE_STOP		0x00
#define NU_CONT_RMB_STATE_STOPPING		0x01
#define NU_CONT_RMB_STATE_STOPPED		0x02
#define NU_CONT_RMB_STATE_RUN			0x03
#define NU_CONT_RMB_STATE_FORCESTOP		0x04
#define NU_CONT_RMB_MODE_DISABLE		0x00
#define NU_CONT_RMB_MODE_ENABLE		0x01
#define NU_CONT_RMB_MODE_AUTORUN		0x02
#define NU_CONT_RMB_MODE_PAUSE		0x80
#define NU_CONT_RMB_AUTO_SEARCH		0x00
#define NU_CONT_RMB_AUTO_FIND			0x01
#define NU_CONT_RMB_AUTO_SEARCHTIME		(60*5)
#define NU_CONT_RMB_CHECK_MSG			0x3000
#define NU_CONT_RMB_START_MSG			0x3001
#define NU_CONT_RMB_FORCESTOP_MSG		0x3002
#define NU_CONT_RMB_FORCESTOPEND_MSG	0x3003
/*----------------------------------------------------------------------*/
/* EEPROM											*/
/*----------------------------------------------------------------------*/
#define NU_EEPROM_CHECK_MSG		0x4000
#define NU_EEPROM_READ_MSG		0x4001
#define NU_EEPROM_WRITE_MSG		0x4002

/*----------------------------------------------------------------------*/
/* SI MANAGER DEFINE									*/
/*----------------------------------------------------------------------*/
#define NU_SI_STACK_SIZE		NU_CONT_STACK_SIZE
#define NU_SI_THREAD_ID			NU_CONT_THREAD_ID
#define NU_SI_THREAD_PRI		NU_CONT_THREAD_PRI
#define NU_SI_MESG_MAX			NU_CONT_MESG_MAX
#define NU_SI_CALLBACK_END		1
#define NU_SI_CALLBACK_CONTINUE	0

/*----------------------------------------------------------------------*/
/* DEBUG 											*/
/*----------------------------------------------------------------------*/
#define NU_DEB_PERF_GFXTASK_CNT		8	/* measurement number of graphic task */
#define NU_DEB_PERF_AUTASK_CNT		4	/* measurement number of  audio task */
#define NU_DEB_DP_CLOCK_CTR			0	/* RDP internal counter	*/
#define NU_DEB_DP_CMD_CTR			1	/* CMD counter */
#define NU_DEB_DP_PIPE_CTR			2	/* PIPE counter */
#define NU_DEB_DP_TMEM_CTR			3	/* TMEM counter */
#define NU_DEB_BAR_FRAME1			0	/* maximum number of displaying */
#define NU_DEB_BAR_FRAME2			1	/* performance bar */
#define NU_DEB_BAR_FRAME3			2
#define NU_DEB_BAR_FRAME4			3
#define NU_DEB_BAR_FRAME5			4
#define NU_DEB_BAR_FRAME6			5
#define NU_DEB_BAR_FRAME7			6
#define NU_DEB_CON_ROW_MAX			40	/* row number of console */
#define NU_DEB_CON_COLUMN_MAX			30 	/* column number of console */
#define NU_DEB_CON_TEXT_SIZE			(NU_DEB_CON_ROW_MAX*NU_DEB_CON_COLUMN_MAX)
#define NU_DEB_CON_WIDTH			320
#define NU_DEB_CON_WINDOW_NUM			4
#define NU_DEB_CON_WINDOW0			0
#define NU_DEB_CON_WINDOW1			1
#define NU_DEB_CON_WINDOW2			2
#define NU_DEB_CON_WINDOW3			3
#define NU_DEB_CON_TEXT_BLACK			0	/* Black */
#define NU_DEB_CON_TEXT_BLUE			1	/* Blue */
#define NU_DEB_CON_TEXT_GREEN			2	/* Green */
#define NU_DEB_CON_TEXT_CYAN			3	/* Cyan */
#define NU_DEB_CON_TEXT_RED			4	/* Red */
#define NU_DEB_CON_TEXT_MAGENTA		5	/* Magenta */
#define NU_DEB_CON_TEXT_YELLOW		6	/* Yellow */
#define NU_DEB_CON_TEXT_WHITE			7	/* White */
#define NU_DEB_CON_TEXT_LIGHTBLACK		8	/* Gray */
#define NU_DEB_CON_TEXT_LIGTHBLUE		9	/* Light blue */
#define NU_DEB_CON_TEXT_LIGHTGREEN		10	/* Light green */
#define NU_DEB_CON_TEXT_LIGHTCYAN		11	/* Light cyan */
#define NU_DEB_CON_TEXT_LIGHTRED		12	/* Light red */
#define NU_DEB_CON_TEXT_LIGHTMAGENTA	13	/* Light magenta */
#define NU_DEB_CON_TEXT_LIGHTYELLOW		14	/* Light yellow */
#define NU_DEB_CON_TEXT_LIGHTWHITE		15	/* White */
#define NU_DEB_CON_ATTR_NORMAL		0x0	/* No attributes */
#define NU_DEB_CON_ATTR_BLINK			0x1	/* Blinking */
#define NU_DEB_CON_SCROLL_ON			1
#define NU_DEB_CON_SCROLL_OFF			0


#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* structer define 									*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*--------------------------------------*/
/* Scheduler sturcter			    */
/*--------------------------------------*/
	typedef short	NUScMsg;		/* The type of the scheduler message */

	typedef struct	st_SCClient {	/* The client list structure */
		struct st_SCClient* next;		/* The pointer for next client */
		OSMesgQueue* msgQ;		/* The message sent to the client */
		NUScMsg			msgType;	/* The type of the message */
		/* NU_SC_RETRACE_MSG or	*/
			/* NU_SC_PRENMI_MSG */
	} NUScClient;

	typedef struct st_SCTask {		/* The task structure */
		struct st_SCTask* next;
		u32		state;
		u32		flags;
		void* framebuffer;		/* For the graphics task */
		OSTask		list;
		OSMesgQueue* msgQ;
		OSMesg		msg;
	} NUScTask;

	typedef struct st_Sched { /* Define the scheduller structure */

		/* The message */
		NUScMsg	retraceMsg;
		NUScMsg	prenmiMsg;

		/* Define the task request queue */
		OSMesgQueue	audioRequestMQ;
		OSMesg		audioRequestBuf[NU_SC_MAX_MESGS];
		OSMesgQueue graphicsRequestMQ;
		OSMesg		graphicsRequestBuf[NU_SC_MAX_MESGS];

		/* Define the message queue */
		OSMesgQueue	retraceMQ;
		OSMesg		retraceMsgBuf[NU_SC_MAX_MESGS];
		OSMesgQueue	rspMQ;
		OSMesg     	 rspMsgBuf[NU_SC_MAX_MESGS];
		OSMesgQueue 	rdpMQ;
		OSMesg		rdpMsgBuf[NU_SC_MAX_MESGS];

		/* Used for waiting for next retrace signal */
		OSMesgQueue	waitMQ;
		OSMesg		waitMsgBuf[NU_SC_MAX_MESGS];

		/* Define the thread */
		OSThread	schedulerThread;		/* The main thread */
		OSThread	audioThread;		/* Audio */
		OSThread	graphicsThread;		/* Graphics */

		/* The client list */
		NUScClient* clientList;

		/* The graphics task which is being executed */
		NUScTask* curGraphicsTask;
		NUScTask* curAudioTask;
		NUScTask* graphicsTaskSuspended;

		u32		retraceCount;


	} NUSched;


	/*--------------------------------------*/
	/* ucode sturcter				    */
	/*--------------------------------------*/
	typedef struct  st_Ucode {
		u64* ucode;
		u64* ucode_data;
	} NUUcode;

	/*--------------------------------------*/
	/* CALL BACK Function	typedef	    */
	/*--------------------------------------*/
	typedef void (*NUIdleFunc)(void);		/* pointer of the idle call-back function */
	typedef void (*NUScPreNMIFunc)(void);	/* pointer of the PRENMI call-back function */
	typedef void (*NUGfxFunc)(u32);		/* pointer of the GFX retrace call-back function */
	typedef void (*NUGfxPreNMIFunc)(void);	/* pointer of the GFX PRENMI call-back function */

	typedef void (*NUGfxSwapCfbFunc)(void*);	/* pointer of the SWAPBUF call-back function */
	typedef void (*NUGfxTaskEndFunc)(void*);	/* pointer of the TASKEND call-back function */
	typedef void (*NUContReadFunc)(u32);	/* pointer of the call-back function which */
	/* ends reading the controller */
	typedef void (*NUContPakFunc)(void*);	/* Call-back the controller-pack control-function */
	typedef void (*NUContRmbFunc)(void*);	/* Call-back the oscillating-pack control-function */
	typedef s32(*NUCallBackFunc)(void*);	/* Call-back function */

	/*--------------------------------------*/
	/* controller typedef			    */
	/*--------------------------------------*/
	typedef struct st_ContRead {
		NUScMsg	mesg;			/* The message type */
		OSMesgQueue* rtnMesgQ;
		OSContPad* pad;
	} NUContReadMesg;

	typedef struct st_ContData {
		u16	button;		/* A,B,C,Z,L,R,START,PAD ,button 	*/
		s8	stick_x;		/* real-use range  -61 <= stick X <= 61 */
		s8	stick_y;		/* real-use range  -63 <= stick Y <= 63 */
		u8	errno;		/* error*/
		u16	trigger;		/* Trigger data of the button */
	} NUContData;

	typedef struct st_ContQuery {
		NUScMsg	mesg;
		OSMesgQueue* rtnMesgQ;
	} NUContQueryMesg;

	typedef struct st_ContPakFile {
		OSPfs* pfs;
		s32		file_no;
		s32		error;
		u32		type;
	} NUContPakFile;


	typedef struct st_ConPaktMesg {	/* basic class of controller-pack access */
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
	} NUContPakMesg;

	typedef struct st_ContPakOpenMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
		s32			error;
	} NUContPakOpenMesg;

	typedef struct st_ContPakFreeMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
		s32			size;
	} NUContPakFreeMesg;

	typedef struct st_ContPakFileOpenMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
		u8* noteName;
		u8* extName;
		s32			size;
		s32			mode;
	} NUContPakFileOpenMesg;

	typedef struct st_ContPakFileRWMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
		s32			offset;
		s32			size;
		u8			mode;
		u8* buf;
	} NUContPakFileRWMesg;

	typedef struct st_ContPakFileStateMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
		OSPfsState* state;
	} NUContPakFileStateMesg;

	typedef struct st_ContPakFileNumMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		NUContPakFile* file;
		s32* max_files;
		s32* used_files;
	} NUContPakFileNumMesg;

	typedef struct st_ContPakIsPlugMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		s32			error;
		u8			bitPattern;
	} NUContPakIsPlugMesg;

	typedef struct st_ContRmbCtl {
		u16			freq;		/* oscillating frequency */
		u16			frame;
		u16			counter;
		u8			state;
		u8			mode;
		u8			autorun;
		u8			type;
	} NUContRmbCtl;

	typedef struct st_ContRmbMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		u8			contNo;
		s32			error;
		void* data;
	} NUContRmbMesg;

	typedef struct st_CallBackList {
		struct st_CallBackList* next;
		NUCallBackFunc		func;
	} NUCallBackList;

	typedef struct st_EepromMesg {
		NUScMsg			mesg;
		OSMesgQueue* rtnMesgQ;
		void* dataPtr;
		s32			error;
		u8			address;
		u16			dataLen;
	} NUEepromMesg;

	/*--------------------------------------*/
	/* Debug					    */
	/*--------------------------------------*/
	typedef struct st_GfxTaskTime {
		s64		rspStart;		/* start time of the RSP task */
		s64		rspEnd;		/* end time of the RSP task */
		s64		rdpEnd;		/* end time of RDP */
		u32		dpCnt[4];		/* RDP internal counter */
	} NUDebTaskTime;

	typedef struct st_DebTaskPerf {
		s64		retraceTime;	/* retrace event time */
		u8		gfxTaskCnt;		/* number of graphics tasks */
		u8		auTaskCnt;		/* number of audio tasks */
		u8		gfxTaskStart;
		NUDebTaskTime	gfxTaskTime[NU_DEB_PERF_GFXTASK_CNT];
		NUDebTaskTime	auTaskTime[NU_DEB_PERF_AUTASK_CNT];
	} NUDebTaskPerf;

	/* console window structure */
	typedef struct st_DebConWindow {
		u8	windowFlag;	/* ON/OFF flag of the console window */
		u16	scroll;	/* scroll value */
		u8	scrollFlag;	/* Scroll ON/OFF */
		u8	textColor;	/* Character color */
		u8	attribute;	/* attribute */
		u16	posX;		/* digit position of writing characters */
		u16	posY;		/* line position of writing characters */
		u16	index;	/* position of writing the character buffer */
		u16	winX;		/* X-coordinate on the display of the console window */
		u16	winY;		/* Y-coordinate on the display of the console window */
		u16	winW;		/* number of digits of the console display */
		u16	winH;		/* number of digits of the console display */
		u16	text[NU_DEB_CON_TEXT_SIZE];	/* The character buffer */
	} NUDebConWindow;

	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	/* extern variables 									*/
	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/

	extern u8		nuRDPOutputBuf[NU_GFX_RDP_OUTPUTBUFF_SIZE];
	extern u8		nuDramStack[SP_DRAM_STACK_SIZE8];
	extern u8		nuYieldBuf[OS_YIELD_DATA_SIZE];
	extern NUSched		nusched;		/* scheduller structure */
	extern OSMesgQueue 	nuGfxMesgQ;		/* graphic thread queue */
	extern u32		nuScRetraceCounter;     /* retrace counter */
	/*--------------------------------------*/
	/*  graphics variables 			    */
	/*--------------------------------------*/
	extern NUUcode* nuGfxUcode;		/* pointer of the GFX micro code structure */
	extern u16** nuGfxCfb;		/* pointer of the pointer array of frame buffer */
	extern u16* nuGfxCfb_ptr;	/* pointer of the frame buffer */
	extern u32			nuGfxCfbNum;	/* number of frame buffers */
	extern u16* nuGfxZBuffer;	/* pointer of the Z-buffer */
	extern volatile u32	nuGfxTaskSpool;	/* number of tasks in the queue */
	extern u32			nuGfxDisplay;	/* ON/OFF flag of the screen */
	extern u32			nuGfxCfbCounter;	/* switching frame buffers */

	/*--------------------------------------*/
	/*  controller manager variables 	*/
	/*--------------------------------------*/
	extern OSContStatus	nuContStatus[NU_CONT_MAXCONTROLLERS];
	extern OSContPad		nuContData[NU_CONT_MAXCONTROLLERS];
	extern u32			nuContNum;			/* number of connected controllers */
	extern u32			nuContDataLockKey; 	/* Lock the controller data */
	extern OSMesgQueue	nuContWaitMesgQ;		/* Wait for the controller read */
	extern OSPfs		nuContPfs[4];
	extern NUCallBackList	nuContCallBack;
	extern u16			nuContPakCompanyCode;	/* company code */
	extern u32			nuContPakGameCode;	/* game code */
	extern NUCallBackList	nuContPakCallBack;

	/*--------------------------------------*/
	/*  RUMBUL manager variables 		    */
	/*--------------------------------------*/
	extern NUContRmbCtl	nuContRmbCtl[4];
	extern u32			nuContRmbSearchTime;
	extern NUCallBackList	nuContRmbCallBack;
	/*--------------------------------------*/
	/*  EEPROM manager variables 		    */
	/*--------------------------------------*/
	extern NUCallBackList	nuEepromCallBack;

	/*--------------------------------------*/
	/*  si variables 				    */
	/*--------------------------------------*/
	extern OSMesgQueue	nuSiMesgQ;		/* SI event message queue */
	extern OSMesgQueue	nuSiMgrMesgQ;	/* SI manager queue */
	extern NUCallBackList	nuSiCallBackList;	/* Call-back function list */

	/*--------------------------------------*/
	/* CALL BACK Function pointer 	    */
	/*--------------------------------------*/
	extern NUIdleFunc		nuIdleFunc;		/* callback func pointer */
	extern NUScPreNMIFunc	nuScPreNMIFunc;	/* PRE NMI callback pointer */
	extern NUGfxSwapCfbFunc	nuGfxSwapCfbFunc; /* swapbuf callback pointer */
	extern NUGfxFunc		nuGfxFunc;		/* callback pointer */
	extern NUGfxPreNMIFunc	nuGfxPreNMIFunc;	/* callback pointer */
	extern NUGfxTaskEndFunc	nuGfxTaskEndFunc; /* task end callback ptr */
	extern NUContReadFunc	nuContReadFunc;	/* call-back function at the end of */
	/* controller read */
/*--------------------------------------*/
/* Debug 		 			    */
/*--------------------------------------*/
	extern NUDebTaskPerf* nuDebTaskPerfPtr;
	extern NUDebConWindow	nuDebConWin[];

	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	/* Global Function 									*/
	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	/*--------------------------------------*/
	/* Boot function				    */
	/*--------------------------------------*/
	extern void nuIdleFuncSet(void (*func)(void));
	extern void nuPreNMIFuncSet(NUScPreNMIFunc func);
	/*--------------------------------------*/
	/* Scheduler function			    */
	/*--------------------------------------*/
	extern void nuScCreateScheduler(u8 videoMode, u8 numFields);
	extern void nuScAddClient(NUScClient* c, OSMesgQueue* mq, NUScMsg msgType);
	extern void nuScRemoveClient(NUScClient* client);
	extern void nuPreNMIFuncSet(NUScPreNMIFunc func);
	extern OSMesgQueue* nuScGetGfxMQ(void);
	extern OSMesgQueue* nuScGetAudioMQ(void);


	/*--------------------------------------*/
	/* graphic(GFX) manager function	    */
	/*--------------------------------------*/
	extern void nuGfxInit(void);
	extern void nuGfxThreadStart(void);
	extern void nuGfxTaskMgrInit(void);
	extern void nuGfxTaskStart(Gfx* gfxList_ptr, u32 gfxListSize, u32 ucode, u32 flag);
	extern void nuGfxFuncSet(NUGfxFunc func);
	extern void nuGfxPreNMIFuncSet(NUGfxPreNMIFunc func);
	extern void nuGfxSwapCfbFuncSet(NUGfxSwapCfbFunc func);
	extern void nuGfxSetCfb(u16** framebuf, u32 framebufnum);
	extern void nuGfxSwapCfb(void* framebuffer);

	extern void nuGfxTaskEndFuncSet(NUGfxTaskEndFunc func);
	extern void nuGfxTaskAllEndWait(void);
	extern void nuGfxRetraceWait(u32 retrace_num);
	extern void nuGfxDisplayOff(void);
	extern void nuGfxDisplayOn(void);
	/*--------------------------------------*/
	/* controller manager function	    */
	/*--------------------------------------*/
	extern u8   nuContInit(void);
	extern s32  nuContMgr(void* mesg);
	extern u8   nuContMgrInit(void);
	extern void nuContMgrRemove(void);
	extern void nuContDataLock(void);
	extern void nuContDataUnLock(void);
	extern void nuContDataRead(OSContPad* pad);
	extern void nuContDataReadWait(void);
	extern void nuContDataReadStart(void);
	extern void nuContDataGet(OSContPad* contdata, u32 padno);
	extern void nuContDataGetAll(OSContPad* contdata);
	extern void nuContDataGetEx(NUContData* contdata, u32 padno);
	extern void nuContDataGetExAll(NUContData* contdata);
	extern void nuContReadFuncSet(NUContReadFunc func);
	extern void nuContQueryRead(void);
	extern void nuContDataClose(void);
	extern void nuContDataOpen(void);

	/*--------------------------------------*/
	/* controller pak function		    */
	/*--------------------------------------*/
	extern s32  nuContPakMgr(void* mesg_type);
	extern void nuContPakMgrInit(void);
	extern void nuContPakMgrRemove(void);
	extern void nuContPakJisToN64(u8* src, u8* dest, u32 len);
	extern void nuContPakN64ToJis(u8* src, u8* dest, u32 len);
	extern void nuContPakOpen(NUContPakFile* file, u32 contNo);
	extern s32  nuContPakGetFree(NUContPakFile* file);
	extern void nuContPakCodeSet(u8* companyCode, u8* gameCode);
	extern void nuContPakRepairId(NUContPakFile* file);
	extern void nuContPakFileOpen(NUContPakFile* file,
		u8* noteName, u8* extName, u32 mode, s32 size);
	extern void nuContPakFileOpenJis(NUContPakFile* file,
		u8* noteName, u8* extName, u32 mode, s32 size);
	extern void nuContPakFileReadWrite(NUContPakFile* file,
		s32 offset, s32 size, u8* buf, u32 mode);
	extern void nuContPakFileDelete(NUContPakFile* file, u8* noteName, u8* extName);
	extern void nuContPakFileDeleteJis(NUContPakFile* file, u8* noteName, u8* extName);
	extern void nuContPakFileState(NUContPakFile* file, OSPfsState* state);
	extern void nuContPakFileNum(NUContPakFile* file,
		s32* max_files, s32* used_files);

	/*--------------------------------------*/
	/* rumbler pak function			    */
	/*--------------------------------------*/
	extern void nuContRmbMgrInit(void);
	extern s32  nuContRmbMgr(void* mesg);
	extern void nuContRmbMgrRemove(void);
	extern void nuContRmbStart(u32 contNo, u16 freq, u16 frame);
	extern s32  nuContRmbCheck(u32 contNo);
	extern void nuContRmbModeSet(u32 contNo, u8 mode);
	extern void nuContRmbForceStop(void);
	extern void nuContRmbForceStopEnd(void);
	extern void nuContRmbSearchTimeSet(u32 frame);

	/*--------------------------------------*/
	/* eeprom function			    */
	/*--------------------------------------*/
	extern void nuEepromMgrInit(void);
	extern s32  nuEepromMgr(void* mesg);
	extern s32  nuEepromCheck(void);
	extern void nuEepromMgrRemove(void);
	extern s32 nuEepromRead(u8 address, u8* buffer, u16 len);
	extern s32 nuEepromWrite(u8 address, u8* buffer, u16 len);

	/*--------------------------------------*/
	/* pi function				    */
	/*--------------------------------------*/
	extern void nuPiInit(void);
	extern void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size);

	/*--------------------------------------*/
	/* si function				    */
	/*--------------------------------------*/
	extern u8 nuSiMgrInit(void);
	extern void nuSiCallBackAdd(NUCallBackList* list, NUCallBackFunc func);
	extern void nuSiCallBackRemove(NUCallBackList* list);

	/*--------------------------------------*/
	/* dubug function				    */
	/*--------------------------------------*/
#ifdef NDEBUG
#define nuDebTaskPerfBar0(EX0 ,EX1 ,EX2)       ((void)0)
#define nuDebTaskPerfBar1(EX0 ,EX1 ,EX2)       ((void)0)
#else
	extern void nuDebTaskPerfBar0(u32 frameNum, u32 y, u32 flag);
	extern void nuDebTaskPerfBar1(u32 frameNum, u32 y, u32 flag);
#endif /* NDEBUG */

	extern void nuDebConDisp(u32 flag);
	extern void nuDebConCPuts(u32 wndNo, const char* s);
	extern void nuDebConInc(NUDebConWindow* conWin);
	extern void nuDebConRtn(NUDebConWindow* conWin);
	extern void nuDebConEsc(NUDebConWindow* conWin, char esc);
	extern void nuDebConWindowSet(u32 wndNo, u32 winX, u32 winY, u32 width, u32 height);
	extern void nuDebConWindowSize(u32 wndNo, u32 width, u32 height);
	extern void nuDebConWindowPos(u32 wndNo, u32 winX, u32 winY);
	extern void nuDebConTextColor(u32 wndNo, u32 color);
	extern void nuDebConTextAttr(u32 wndNo, u32 attr);
	extern void nuDebConTextPos(u32 wndNo, u32 posX, u32 posY);
	extern void nuDebConScroll(u32 wndNo, u32 flag);
	extern void nuDebConWindowShow(u32 wndNo, u32 flag);
	extern void nuDebConClear(u32 wndNo);
	extern void nuDebConPutc(u32 wndNo, u32  c);
	extern void nuDebTaskPerfLoad(void);
	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/
	/* MACRO	 										*/
	/*----------------------------------------------------------------------*/
	/*----------------------------------------------------------------------*/

	/*----------------------------------------------------------------------*/
	/*	Register the graphics micro code 						*/
	/*	IN:	The pointer of the NUUcode array of the graphics micro code */
	/*----------------------------------------------------------------------*/
#define nuGfxSetUcode(ucode)						\
{									\
    nuGfxUcode = ucode;							\
}
/*----------------------------------------------------------------------*/
/*	Setting the Z-buffer 								*/
/*	IN:	The pointer of the Z-buffer 						*/
/*----------------------------------------------------------------------*/
#define nuGfxSetZBuffer(ZBuf_p)						\
{									\
    nuGfxZBuffer = ZBuf_p;						\
}
/*----------------------------------------------------------------------*/
/*	Delete the call-back function 						*/
/*	IN:	None 										*/
/*----------------------------------------------------------------------*/
#define	nuPreNmiFuncRemove()		nuPreNmiFuncSet(NULL)
#define 	nuGfxFuncRemove()			nuGfxFuncSet(NULL)
#define 	nuGfxPreNMIFuncRemove()		nuGfxPreNMIFuncSet(NULL)
#define	nuGfxSwapCfbFuncRemove()	nuGfxSwapCfbFuncSet(NULL)
#define	nuGfxTaskEndFuncRemove()	nuGfxTaskEndFuncSet(NULL)
#define 	nuContReadFuncRemove()		nuContReadFuncSet(NULL)

/*----------------------------------------------------------------------*/
/*	nuContDataGet - Get the controller data 					*/
/*	Get the read data from the controller manager 				*/
/*												*/
/*	IN:	*contpad The pointer of the controller structure 		*/
/* 		cont_no  The controller number 					*/
/*----------------------------------------------------------------------*/
#if 0
#define	nuContDataGet(contpad, cont_no)					\
{									\
    bcopy(&nuContData[cont_no], contpad, sizeof(OSContPad));		\
}

/*----------------------------------------------------------------------*/
/*	nuContDataGetAll - Get the controller data 				*/
/*	Get the read data from the controller manager 				*/
/*	Copy all four buffers 								*/
/*	IN:	*contpad The pointer of the controller structure 		*/
/* 		cont_no  The controller number 					*/
/*----------------------------------------------------------------------*/
#define nuContDataGetAll(contpad)					\
{									\
    bcopy(&nuContData[0], contpad, sizeof(OSContPad)*MAXCONTROLLERS);\
}
#endif
/*----------------------------------------------------------------------*/
/*	nuContPakFileFind - Retrieve the node 					*/
/*----------------------------------------------------------------------*/
#define nuContPakFileFind(file, nodeName, extName)			\
{									\
    nuContPakFileOpen(file, nodeNname, extName, NU_CONT_PAK_NOCREAT, 0);\
}

/*----------------------------------------------------------------------*/
/*	nuContPakFileFindJis - Retrieve the note 					*/
/*----------------------------------------------------------------------*/
#define nuContPakFileFindJis(file, nodeName, extName)			\
{									\
    nuContPakFileOpenJis(file, nodeNname, extName, NU_CONT_PAK_NOCREAT, 0);\
}
/*----------------------------------------------------------------------*/
/*	nuContPakFileRead - Read the note 						*/
/*----------------------------------------------------------------------*/
#define nuContPakFileRead(file, offset, size, buf)			\
{									\
    nuContPakFileReadWrite(file, offset, size, buf, PFS_READ);		\
}

/*----------------------------------------------------------------------*/
/*	nuContPakFileWrite - Write the note 					*/
/*----------------------------------------------------------------------*/
#define nuContPakFileWrite(file, offset, size, buf)			\
{									\
    nuContPakFileReadWrite(file, offset, size, buf, PFS_WRITE);		\
}

/*----------------------------------------------------------------------*/
/*	nuDebConPuts - Output the character string (with the line feed)	*/
/*----------------------------------------------------------------------*/
#define  nuDebConPuts(wndNo, s)						\
{									\
    nuDebConCPuts(wndNo, s);						\
    nuDebConRtn(&nuDebConWin[wndNo]);					\
}
/*----------------------------------------------------------------------*/


#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */
#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif /* _NUSYS_H_ */


