#ifndef _LIBMUS_DATA_H_
#define _LIBMUS_DATA_H_

#include "PR/libaudio.h"

typedef struct
{
  unsigned char	header_name[16];  
  unsigned long flags;		  
  unsigned long wbk_name[3];     
  int 		count;           
  unsigned char	*basenote;	  
  float		*detune;        
  ALWaveTable	**wave_list;     
} ptr_bank_t;


#define   qv		24
#define   sq		12
#define   dcr		72
#define   ddcr		84
#define   dsq		6
#define   dqv		36
#define   cr		48
#define   mn		96
#define   crtr		16
#define   ddqv		42
#define   hdsqtr	1
#define   sqtr		4
#define   qvtr		8
#define   dtsq		18
#define   hdsq		3
#define   sb		192
#define   dmn		144

#define   cs0		1
#define   d0		2
#define   ds0		3
#define   e0		4
#define   f0		5
#define   fs0		6
#define   g0		7
#define   gs0		8
#define   a0		9
#define   as0		10
#define   b0		11
#define   c1		12
#define   cs1		13
#define   d1		14
#define   ds1		15
#define   e1		16
#define   f1		17
#define   fs1		18
#define   g1		19
#define   gs1		20
#define   a1		21
#define   as1		22
#define   b1		23
#define   c2		24
#define   cs2		25
#define   d2		26
#define   ds2		27
#define   e2		28
#define   f2		29
#define   fs2		30
#define   g2		31
#define   gs2		32
#define   a2		33
#define   as2		34
#define   b2		35
#define   c3		36
#define   cs3		37
#define   d3		38
#define   ds3		39
#define   e3		40
#define   f3		41
#define   fs3		42
#define   g3		43
#define   gs3		44
#define   a3		45
#define   as3		46
#define   b3		47
#define   c4		48
#define   cs4		49
#define   d4		50
#define   ds4		51
#define   e4		52
#define   f4		53
#define   fs4		54
#define   g4		55
#define   gs4		56
#define   a4		57
#define   as4		58
#define   b4		59
#define   c5		60
#define   cs5		61
#define   d5		62
#define   ds5		63
#define   e5		64
#define   f5		65
#define   fs5		66
#define   g5		67
#define   gs5		68
#define   a5		69
#define   as5		70
#define   b5		71
#define   c6		72
#define   cs6		73
#define   d6		74
#define   ds6		75
#define   e6		76
#define   f6		77
#define   fs6		78
#define   g6		79
#define   gs6		80
#define   a6		81
#define   as6		82
#define   b6		83
#define   c7		84
#define   cs7		85
#define   d7		86
#define   ds7		87
#define   e7		88
#define   f7		89
#define   fs7		90
#define   g7		91
#define   gs7		92
#define   a7		93
#define   as7		94
#define   b7		95

#define   Cstop         0x80
#define   Cwave	        0x81
#define   Cport	        0x82	
#define	  Cportoff      0x83
#define   Cdefa	        0x84	
#define   Ctempo        0x85	
#define   Ccutoff       0x86
#define   Cendit        0x87
#define   Cvibup	0x88
#define   Cvibdown	0x89
#define	  Cviboff	0x8a
#define	  Clength	0x8b
#define	  Cignore	0x8c
#define	  Ctrans	0x8d
#define	  Cignoretrans  0x8e
#define	  Cdistort      0x8f
#define   Cadsr		0x90
#define   Cenvoff	0x91
#define   Cenvon	0x92
#define   Ctroff	0x93
#define	  Ctron		0x94
#define   Cfor		0x95
#define	  Cnext		0x96
#define	  Cwobble	0x97
#define   Cwobbleoff	0x98
#define   Cvelon	0x99
#define	  Cveloff	0x9a
#define   Cvelocity	0x9b
#define   Cpan		0x9c
#define   Cstereo	0x9d
#define	  Cdrums	0x9e
#define	  Cdrumsoff	0x9f
#define	  Cprint	0xa0
#define	  Cgoto		0xa1
#define	  Creverb	0xa2
#define	  Crandnote	0xa3
#define	  Crandvolume	0xa4
#define	  Crandpan	0xa5
#define	  Cvolume	0xa6
#define	  Cstartfx     	0xa7
#define	  Cbendrange	0xa8
#define	  Csweep	0xa9
#define	  Clast		0xaa


#endif