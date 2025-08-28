#include <nusys.h>
#include "nupakmenu.h"
#include "pakmenu.h"

#ifdef	LANG_JAPANESE
#include "fontbmp.h"
#endif

typedef	struct{
    u8	code;
    u8	number;
} FTBL;

FTBL	canvas_ftbl[] = {
    { FONT_CODE_A-FONT_CODE_TOP, 26 },	/* A-Z			*/
    { FONT_CODE_0-FONT_CODE_TOP, 10 },	/* 0-9			*/
    { FONT_CODE_i-FONT_CODE_TOP, 14 },	/* !# '*+,-./:=?@// 	*/
    
#ifdef	LANG_JAPANESE
    { FONT_CODE_o-FONT_CODE_TOP, 58 },	/* KANA			*/
#endif	/* LANG_JAPANESE */
};

#ifdef LANG_JAPANESE
s8 font_kana_dot[] = {	/* font pattern of DAKUTEN,HANDAKUTEN	*/
/* 0123456789012345
   -------------XO-
   -----------XOXXO
   -----------XXOXX
   ------------XX-- */
  0x02,  0x09,  0x04,  0x00,
 ~0x04, ~0x16, ~0x1b, ~0x0c,

/* 0123456789012345
   ------------XOOO
   ------------XOXO
   ------------XOOO
   ------------XXXX */
  0x07,  0x05,  0x07,  0x00,
 ~0x08, ~0x0a, ~0x08, ~0x0f,
};
#endif /* LANG_JAPANESE */

u16	*__font_buf;

void nuPakMenuLoadFont(u16 *font_buf) {

    register u16	*fpix;
    register u32	*farc = (u32 *)FONTARC_TOP+1;
    register u32	bitmap = *(farc-1);
    register u32	position = 0;
    register int	i;
    register int	j;
    int	k;

    __font_buf = font_buf;
     
    for (i = 0; i < FONT_HT * (FONT_CODE_BTM - FONT_CODE_TOP + 2); i++) {
		*font_buf++ = 0x0000;
    }
  
    for (i = 0; i < sizeof(canvas_ftbl)/sizeof(FTBL); i++) {

#ifdef LANG_JAPANESE
		if (i == 3) {
			farc = (u32 *)font_bitmaps;
			position  = 0;
			bitmap = *farc++;
		}
#endif
	
		fpix = __font_buf + (canvas_ftbl[i].code * FONT_HT + FONT_HOFS);

		for (j = 0; j < canvas_ftbl[i].number; j++) {
		
			for (k = 0; k < FONTARC_HT; k++) {

				position += FONTARC_WD;
				
				if (position & ~0x1f) {

					position &= 0x1f;
					*fpix = (u16)(bitmap << position)  & 0x1fff;
					bitmap = *farc++;
					
					if (!position) {
						goto nosLoadFont_00;
					}

				}

				*fpix |= (u16)(bitmap >> (32 - position)) & 0x1fff;
			
nosLoadFont_00:
				fpix++;
				
		#if DEBUG_FONT
				{
					int n;

					osSyncPrintf("j=%3d k=%3d position=%3d  ", j, k, position);
					
					for (n = 15; n >= 0; n--) {
						if (*(fpix-1) & (1 << n)) osSyncPrintf("O");
						else osSyncPrintf("-");
					}

					osSyncPrintf("\n");

				}
		#endif
				
			}

			fpix += (FONT_HT-FONTARC_HT);
			position += 2;

		}

    }
    
#if	LANG_JAPANESE

	   for (i = F_ADRS(KA); i < F_ADRS(MA); i++) {
	       (__font_buf+(F_ADRS(GA)-F_ADRS(KA)))[i] = __font_buf[i];
	    }

		for (i = F_ADRS(HA); i < F_ADRS(MA); i++) {
			(__font_buf+(F_ADRS(BA)-F_ADRS(HA)))[i] = __font_buf[i];
		}  
    
		k = 0;
		fpix = __font_buf + F_ADRS(GA);
		
		for (i = FONT_CODE_GA; i < FONT_CODE_PA+5; i++) {

			if (i == FONT_CODE_PA) k = 8;
			for (j = 0; j < 4; j++) {
				fpix[j] = (fpix[j] | font_kana_dot[j+k]) & font_kana_dot[j+k+4];
			}
			
#if	DEBUG_FONT
	{
	    int	n;

	    for (j = 0; j < FONT_HT; j++) {
			
			for (n = 15; n >= 0; n--) {
				if (fpix[j] & (1 << n)) osSyncPrintf("O");
				else osSyncPrintf("-");
			}

			osSyncPrintf("\n");
	    }

	    osSyncPrintf("\n");
	}
#endif

	fpix += FONT_HT;

    }

#endif
}

void canvas_u8_to_str(u16 n, u8 *str) {

    if (n > 999) {
		n = 999;
    }
    
#if 1
    str[0] = n / 100 + 0x0f + (n >= 100);
    str[1] = (n % 100) / 10 + 0x0f + (n >= 10);
    str[2] = n % 10 + 0x10;
#else
    static  u8  ofs[] = { 0x0f, 0x0f, 0x10, 0x10, 0x10 };
    s32	i;

    if      (n >= 100) i = 2;
    else if (n >= 10)  i = 1;
    else               i = 0;
    
    str[0] = n / 100 + ofs[i+0];
    n %= 100;  
    str[1] = n / 10  + ofs[i+1];
    str[2] = n % 10  + ofs[i+2];
#endif

}  

u16	*__cursor;

void canvas_putc(int c, u16 color_fg, u16 color_bg) {

    register int	i;
    register int	j;
    register u16	*fpix;
    register u16	*curp;

    if (c == FONT_CODE_QSKIP) {
		__cursor += FONT_WSKIP/3;
    } else {
	
	if (c < FONT_CODE_TOP || FONT_CODE_BTM < c) {
	    c = FONT_CODE_NON;
	}

	fpix = __font_buf + (c - FONT_CODE_TOP)*FONT_HT;
	curp = __cursor;
	
	for (i = 0; i < FONT_HT; i++) {
	
	    for (j = FONT_WD - 1; j >= 0; j--) {
	
			if ((*fpix >> j) & 1) {
				*(curp)             = color_fg;
				*(curp+1+SCREEN_WD) = color_bg;
			}

			curp++;
	    
		}
	    
		curp += (SCREEN_WD - FONT_WD);
	    fpix++;
	
	}
	
	__cursor += FONT_WSKIP;

    }

}

void canvas_puts(u8 *str, int n, u16 color_fg, u16 color_bg) {
    while (n-- && *str) canvas_putc(*str++, color_fg, color_bg);
}
