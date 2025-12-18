#ifndef _OS_INTERNAL_RSP_H_
#define	_OS_INTERNAL_RSP_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/os.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/* Signal processor interface (Sp) */

extern u32 		__osSpGetStatus(void);
extern void		__osSpSetStatus(u32);
extern s32		__osSpSetPc(u32);
extern s32		__osSpRawWriteIo(u32, u32);
extern s32		__osSpRawReadIo(u32, u32 *);
extern s32		__osSpRawStartDma(s32, u32, void *, u32);


#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_INTERNAL_RSP_H */
