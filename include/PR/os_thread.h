#ifndef _OS_THREAD_H_
#define	_OS_THREAD_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/ultratypes.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/**************************************************************************
 *
 * Type definitions
 *
 */

typedef s32 OSPri;
typedef s32 OSId;

typedef union {
    struct {
        f32 f_odd;
        f32 f_even;
    } f;
    f64 d;
} __OSfp;

typedef struct {
    u64 at, v0, v1, a0, a1, a2, a3;
    u64 t0, t1, t2, t3, t4, t5, t6, t7;
    u64 s0, s1, s2, s3, s4, s5, s6, s7;
    u64 t8, t9;
    u64 gp, sp, s8, ra;
    u64 lo, hi;
    u32 sr, pc, cause, badvaddr, rcp;
    u32 fpcsr;
    __OSfp  fp0,  fp2,  fp4,  fp6,  fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;

typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;

typedef struct OSThread_s {
    struct OSThread_s    *next;       /* run/mesg queue link */
    OSPri                 priority;   /* run/mesg queue priority */
    struct OSThread_s   **queue;      /* queue thread is on */
    struct OSThread_s    *tlnext;     /* all threads queue link */
    u16                   state;      /* OS_STATE_* */
    u16                   flags;      /* flags for rmon */
    OSId                  id;         /* id for debugging */
    int                   fp;         /* thread has used fp unit */
    __OSThreadprofile_s  *thprof;     /* workarea for thread profiler */
    __OSThreadContext     context;    /* register/interrupt mask */
} OSThread;


#endif /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */

/**************************************************************************
 *
 * Global definitions
 *
 */

/* Thread states */

#define OS_STATE_STOPPED    (1 << 0)
#define OS_STATE_RUNNABLE   (1 << 1)
#define OS_STATE_RUNNING    (1 << 2)
#define OS_STATE_WAITING    (1 << 3)

/* Recommended thread priorities for the system threads */

#define OS_PRIORITY_MAX         255
#define OS_PRIORITY_VIMGR       254
#define OS_PRIORITY_RMON        250
#define OS_PRIORITY_RMONSPIN    200
#define OS_PRIORITY_PIMGR       150
#define OS_PRIORITY_SIMGR       140
#define OS_PRIORITY_APPMAX      127
#define OS_PRIORITY_IDLE          0 /* Must be 0 */

/* For thread profiler */
#define THPROF_IDMAX            64
#define THPROF_STACKSIZE        256

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/**************************************************************************
 *
 * Macro definitions
 *
 */


/**************************************************************************
 *
 * Extern variables
 *
 */


/**************************************************************************
 *
 * Function prototypes
 *
 */

/* Thread operations */

extern void     osCreateThread(OSThread *, OSId, void (*)(void *), void *, void *, OSPri);
extern void     osDestroyThread(OSThread *);
extern void     osYieldThread(void);
extern void     osStartThread(OSThread *);
extern void     osStopThread(OSThread *);
extern OSId     osGetThreadId(OSThread *);
extern void     osSetThreadPri(OSThread *, OSPri);
extern OSPri    osGetThreadPri(OSThread *);


#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_THREAD_H_ */
