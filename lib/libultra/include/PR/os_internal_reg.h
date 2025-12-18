#ifndef _OS_INTERNAL_REG_H_
#define _OS_INTERNAL_REG_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include "os.h"

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/* Routines to get/fetch coprocessor 0 registers */
extern u32  __osGetCause(void);
extern void __osSetCause(u32);
extern u32  __osGetCompare(void);
extern void __osSetCompare(u32);
extern u32  __osGetConfig(void);
extern void __osSetConfig(u32);
extern void __osSetCount(u32);
extern u32  __osGetSR(void);
extern void __osSetSR(u32);
extern u32  __osDisableInt(void);
extern void __osRestoreInt(u32);
extern u32  __osGetWatchLo(void);
extern void __osSetWatchLo(u32);

/* Routines to get/set floating-point control and status register */
extern u32  __osSetFpcCsr(u32);
extern u32  __osGetFpcCsr(void);

#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_INTERNAL_REG_H */
