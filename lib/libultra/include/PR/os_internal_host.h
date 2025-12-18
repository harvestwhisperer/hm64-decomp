#ifndef _OS_INTERNAL_HOST_H_
#define	_OS_INTERNAL_HOST_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/os.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/* routine for rdb port */
extern u32             __osRdbSend(u8 *buf, u32 size, u32 type);


#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_OS_INTERNAL_HOST_H */
