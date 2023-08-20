#ifndef __R4300_H__
#define __R4300_H__

#define	K0_TO_K1(x)	((u32)(x)|0xA0000000)
#define	K1_TO_K0(x)	((u32)(x)&0x9FFFFFFF)
#define	K0_TO_PHYS(x)	((u32)(x)&0x1FFFFFFF)
#define	K1_TO_PHYS(x)	((u32)(x)&0x1FFFFFFF)
#define	KDM_TO_PHYS(x)	((u32)(x)&0x1FFFFFFF)
#define	PHYS_TO_K0(x)	((u32)(x)|0x80000000)
#define	PHYS_TO_K1(x)	((u32)(x)|0xA0000000)

#endif