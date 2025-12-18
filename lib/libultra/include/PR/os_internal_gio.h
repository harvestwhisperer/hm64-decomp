#ifndef _OS_INTERNAL_GIO_H_
#define	_OS_INTERNAL_GIO_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/os.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/* Development board functions */

extern void		__osGIOInit(s32);
extern void		__osGIOInterrupt(s32);
extern void		__osGIORawInterrupt(s32);


#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_INTERNAL_GIO_H */
