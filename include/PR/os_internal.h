#ifndef _OS_INTERNAL_H_
#define	_OS_INTERNAL_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include "os.h"

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

extern u32		__osGetCause(void);
extern void		__osSetCause(u32);
extern u32		__osGetCompare(void);
extern void		__osSetCompare(u32);
extern u32		__osGetConfig(void);
extern void		__osSetConfig(u32);
extern void		__osSetCount(u32);
extern u32		__osGetSR(void);
extern void		__osSetSR(u32);
extern u32		__osDisableInt(void);
extern void		__osRestoreInt(u32);

extern u32              __osSetFpcCsr(u32);
extern u32              __osGetFpcCsr(void);

extern void		__osSetHWIntrRoutine(OSHWIntr, s32 (*handler)(void));

extern void		__osSetGlobalIntMask(OSHWIntr);
extern void		__osResetGlobalIntMask(OSHWIntr);

extern s32		__osLeoInterrupt(void);

extern u32		__osGetTLBASID(void);
extern u32		__osGetTLBPageMask(s32);
extern u32		__osGetTLBHi(s32);
extern u32		__osGetTLBLo0(s32);
extern u32		__osGetTLBLo1(s32);

extern u32 		__osSiGetStatus(void);
extern s32		__osSiRawWriteIo(u32, u32);
extern s32		__osSiRawReadIo(u32, u32 *);
extern s32		__osSiRawStartDma(s32, void *);

extern u32 		__osSpGetStatus(void);
extern void		__osSpSetStatus(u32);
extern s32		__osSpSetPc(u32);
extern s32		__osSpRawWriteIo(u32, u32);
extern s32		__osSpRawReadIo(u32, u32 *);
extern s32		__osSpRawStartDma(s32, u32, void *, u32);

extern void		__osError(s16, s16, ...);
extern OSThread *	__osGetCurrFaultedThread(void);
extern OSThread *	__osGetNextFaultedThread(OSThread *);

extern void		__osGIOInit(s32);
extern void		__osGIOInterrupt(s32);
extern void		__osGIORawInterrupt(s32);

extern OSThread *	__osGetActiveQueue(void);

extern void		__osSyncPutChars(int, int, const char *);
extern int		__osSyncGetChars(char *);
extern void		__osAsyncPutChars(int, int, const char *);
extern int		__osAsyncGetChars(char *);
extern int		__osAtomicInc(unsigned int *p);
extern int		__osAtomicDec(unsigned int *p);

extern u32             __osRdbSend(u8 *buf, u32 size, u32 type);


#endif

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif