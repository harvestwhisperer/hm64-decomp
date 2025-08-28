#ifndef _SPTASK_H_
#define	_SPTASK_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/ultratypes.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

typedef struct {
	u32	type;
	u32	flags;

	u64	*ucode_boot;
	u32	ucode_boot_size;

	u64	*ucode;
	u32	ucode_size;

	u64	*ucode_data;
	u32	ucode_data_size;

	u64	*dram_stack;
	u32	dram_stack_size;

	u64	*output_buff;
	u64	*output_buff_size;

	u64	*data_ptr;
	u32	data_size;

	u64	*yield_data_ptr;
	u32	yield_data_size;

} OSTask_t;

typedef union {
    OSTask_t		t;
    long long int	force_structure_alignment;
} OSTask;

typedef u32 OSYieldResult;

#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_ASSEMBLY

/*
 * For the RSP ucode:
 *	offsets into the task structure
 */

#include <PR/sptaskoff.h>

#endif

/*
 * Task Flags field
 */
#define OS_TASK_YIELDED			0x0001
#define OS_TASK_DP_WAIT			0x0002
#define	OS_TASK_LOADABLE		0x0004
#define	OS_TASK_SP_ONLY			0x0008
#define OS_TASK_USR0			0x0010
#define OS_TASK_USR1			0x0020
#define OS_TASK_USR2			0x0040
#define OS_TASK_USR3			0x0080

/*
 * Size of Yield buffer.  The taskHdrPtr->t.yield_data_ptr must point to a 
 * buffer of this size.  (The size is in bytes).  ONLY If the task will NEVER
 * yield it may be a null pointer.  The buffer must be aligned to a 64 bit
 * boundary.  The taskHdrPtr->t.yield_data_ptr must be set to point to the
 * buffer BEFORE the task is started.
 */
#if	(defined(F3DEX_GBI)||defined(F3DLP_GBI)||defined(F3DEX_GBI_2))
#define	OS_YIELD_DATA_SIZE		0xc00
#else
#define OS_YIELD_DATA_SIZE		0x900
#endif
#define OS_YIELD_AUDIO_SIZE		0x400

/**************************************************************************
 *
 * Global definitions
 *
 */



#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/**************************************************************************
 *
 * Macro definitions
 *
 */

/*
 * this macro simulates atomic action.
 */
#define	osSpTaskStart(tp)	\
    {				\
        osSpTaskLoad((tp));	\
        osSpTaskStartGo((tp));	\
    }


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

/*
 * break this up into two steps for debugging.
 */
extern void		osSpTaskLoad(OSTask *tp);
extern void		osSpTaskStartGo(OSTask *tp);

extern void		osSpTaskYield(void);
extern OSYieldResult	osSpTaskYielded(OSTask *tp);

#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !_SPTASK_H */
