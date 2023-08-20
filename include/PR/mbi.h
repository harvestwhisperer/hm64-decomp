#ifndef _MBI_H_
#define	_MBI_H_

#define _SHIFTL(v, s, w)	\
    ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define _SHIFTR(v, s, w)	\
    ((unsigned int)(((unsigned int)(v) >> (s)) & ((0x01 << (w)) - 1)))

#define _SHIFT _SHIFTL	/* old, for compatibility only */

#define G_ON	(1)
#define G_OFF	(0)

/**************************************************************************
 *
 * Graphics Binary Interface
 *
 **************************************************************************/

#include "PR/gbi.h"

/**************************************************************************
 *
 * Audio Binary Interface
 *
 **************************************************************************/

#include "PR/abi.h"

/**************************************************************************
 *
 * Task list
 *
 **************************************************************************/

#define	M_GFXTASK	1
#define	M_AUDTASK	2
#define	M_VIDTASK	3

/**************************************************************************
 *
 * Segment macros and definitions
 *
 **************************************************************************/

#define	NUM_SEGMENTS		(16)
#define	SEGMENT_OFFSET(a)	((unsigned int)(a) & 0x00ffffff)
#define	SEGMENT_NUMBER(a)	((unsigned int)(a) >> 24)
#define	SEGMENT_ADDR(num, off)	(((num) << 24) + (off))

#ifndef NULL
#define NULL 0
#endif

#endif /* !_MBI_H_ */
