#ifndef _STDARG_H
#define _STDARG_H

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef char * __gnuc_va_list;
#endif

#if defined (_STDARG_H) || defined (_VARARGS_H)

#if __GNUC__ > 1
#define __va_ellipsis ...
#else
#define __va_ellipsis
#endif

#if __mips>=3
#define __va_rounded_size(__TYPE)  \
  (((sizeof (__TYPE) + 8 - 1) / 8) * 8)
#else
#define __va_rounded_size(__TYPE)  \
  (((sizeof (__TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))
#endif

#ifdef _MIPS_SIM
#include <sgidefs.h>
#endif

#ifdef _STDARG_H
#if defined(_MIPS_SIM) && (_MIPS_SIM == _MIPS_SIM_ABI64)
#define va_start(__AP, __LASTARG)					\
  (__AP = __builtin_next_arg (__LASTARG) - 64				\
   + (__builtin_args_info (2) > 8 ? 64 : __builtin_args_info(2) * 8))
#else
#define va_start(__AP, __LASTARG) \
  (__AP = (__gnuc_va_list) __builtin_next_arg (__LASTARG))
#endif

#else
#define va_alist  __builtin_va_alist
#if __mips>=3
#define va_dcl    long long int __builtin_va_alist; __va_ellipsis
#else
#define va_dcl    int __builtin_va_alist; __va_ellipsis
#endif
#if defined(_MIPS_SIM) && (_MIPS_SIM == _MIPS_SIM_ABI64)
#define va_start(__AP)							\
  (__AP = __builtin_next_arg () - 64					\
   + (__builtin_args_info (2) > 8 ? 64 : __builtin_args_info(2) * 8))
#else
#define va_start(__AP)  __AP = (char *) &__builtin_va_alist
#endif
#endif

#ifndef va_end
void va_end (__gnuc_va_list);
#endif
#define va_end(__AP)	((void)0)

#if __mips>=3
#ifdef __MIPSEB__
#define va_arg(__AP, __type)                                    \
  ((__type *) (void *) (__AP = (char *) ((((__PTRDIFF_TYPE__)__AP + 8 - 1) & -8) \
					 + __va_rounded_size (__type))))[-1]
#else
#define va_arg(__AP, __type)                                    \
  ((__AP = (char *) ((((__PTRDIFF_TYPE__)__AP + 8 - 1) & -8)	\
		     + __va_rounded_size (__type))),		\
   *(__type *) (void *) (__AP - __va_rounded_size (__type)))
#endif

#else 

#ifdef __MIPSEB__
#define va_arg(__AP, __type)					\
  ((__AP = (char *) ((__alignof__ (__type) > 4			\
		      ? ((int)__AP + 8 - 1) & -8		\
		      : ((int)__AP + 4 - 1) & -4)		\
		     + __va_rounded_size (__type))),		\
   *(__type *) (void *) (__AP - __va_rounded_size (__type)))
#else
#define va_arg(__AP, __type)						    \
  ((__type *) (void *) (__AP = (char *) ((__alignof__(__type) > 4	    \
					  ? ((int)__AP + 8 - 1) & -8	    \
					  : ((int)__AP + 4 - 1) & -4)	    \
					 + __va_rounded_size(__type))))[-1]
#endif
#endif

typedef __gnuc_va_list va_list;

#endif /* defined (_STDARG_H) || defined (_VARARGS_H) */
#endif
