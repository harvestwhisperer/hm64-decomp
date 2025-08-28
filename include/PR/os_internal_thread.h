#ifndef _OS_INTERNAL_THREAD_H_
#define	_OS_INTERNAL_THREAD_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/os.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/* For debugger use */

extern OSThread *	__osGetActiveQueue(void);


#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_INTERNAL_THREAD_H */
