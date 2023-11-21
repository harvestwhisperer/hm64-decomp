#ifndef KMCLIB_H
#define KMCLIB_H

#define LIB_DBG		0
#define FAST_SPEED	1

#define ENDIAN_BIG	1
#define FLOAT_S		0
#define CPU_MIPS	1
#define CPU_SH7		0
#define CPU_68K		0
#define CPU_V800	0
#define CPU_386		0

#define ET		1

#define	FLOAT_LLONG_ON	1	/* when supporting float, double and long long in printf 
						   and scanf, the value is 1; 0 when not supporting      */

#if CPU_MIPS
#define BUS_ERR_ALLIGN	3	/* Bud error due to out of alignment
						3=32bit , 1=16bit , 0=none */

#define BYTE	char
#define UBYTE	unsigned char
#define WORD 	short int
#define UWORD	unsigned short
#define DWORD	long int
#define UDWORD	unsigned long

#define REG1	register
#define REG2	register
#define REG3	register
#define REG4	register
#define REG5	register
#define REG6	register
#endif


#ifndef NULL
#define NULL ((void *)0)
#endif
#ifndef EOF
#define EOF (-1)
#endif

/******  CONIO ************/
int _pt_key(void);
void _pt_dsp(int c);
extern unsigned char _keybuf;	/* key buffer */

#endif