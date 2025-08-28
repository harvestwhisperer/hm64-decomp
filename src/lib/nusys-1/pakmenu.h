#ifndef	__pakmenu_h__
#define	__pakmenu_h__

#define	nPrintf		osSyncPrintf

#define	NUM_SI_MSGS	1
#define	DEBUG		0
#define	DEBUG_FONT	0

/*
#define	LANG_JAPANESE	0
*/

#define	PORT_ID		0

#define	SCREEN_LOW	0
#define	SCREEN_HIGH	!SCREEN_LOW

#define	STYPE_MENU	0
#define	STYPE_NODEL	1

#if	LANG_JAPANESE
/***********************************************************************/
/* Game note                  page */

#define STR_MENU_ITEMS	\
	"\x7f\x3b\x70\x68\x3b\x63\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x01\x93\x3b\x82"

#define STR_MENU_TITLE	"\x59\x4f\x63\x7a\x3b\x76\x90\x4a\x57\x01\x71\x65\x4c\x3b"
#define	STR_MENU_VER	"\x2f\x1e\x2b\x3c"

/***********************************************************************/
/*  "Delete . . ."	*/
# define STR_CMD_ERASE			"\x3b\x01\x58\x5b\x6e\x5c"
/*
//  Delete data
//# define STR_CMD_ERASE		"\x89\x3b\x5f\x01\x4e\x01\x58\x5c"
//  Delete
//# define STR_CMD_ERASE		"\x01\x58\x01\x5c\x01"
//  "Delete . . . "
//# define STR_CMD_ERASE		"\x4e\x0f\x58\x5b\x6e\x5c"
//  "Delete"
//# define STR_CMD_ERASE		"\x58\x5b\x6e\x5c"
//  "Delete this: . . . "
//# define STR_CMD_ERASE		"\x3b\x59\x79\x4e\x01\x58\x5c"
//  "Delete . . . "
//# define STR_CMD_ERASE		"\x01\x4e\x0f\x58\x5c\x42"
*/

/***********************************************************************/
/* Actually delete	*/
# define STR_CMD_OK		"\x6d\x4f\x63\x52\x65\x01\x58\x5c"
/*
// OK
//# define STR_CMD_OK		"\x28\x24"
*/

/***********************************************************************/
/* Quit after all	*/
# define STR_CMD_QUIT		"\x73\x4a\x90\x77\x01\x73\x71\x78"
/*
// Quit	
//# define STR_CMD_QUIT		"\x73\x71\x78"
*/

/***********************************************************************/
/* Exit	*/
# define STR_CMD_END		"\x54\x7b\x78"

/***********************************************************************/
/* Remainder  = 	*/
# define STR_BLK_LEFT		"\x68\x59\x77\x3f"
/*
//  Block
//# define STR_BLK_LEFT1		"\x8d\x7a\x4a\x57"
//  Remainder 
//# define STR_BLK_LEFT2		"\x68\x59\x77"
*/

/***********************************************************************/
/* Do not delete data.	*/
# define STR_PAK_NODEL		"\x58\x5d\x6e\x5d\x4f\x89\x5b\x5f"

/***********************************************************************/
/* Controller Pak error	*/
# define STR_PAK_IDERR		"\x59\x4f\x63\x7a\x3b\x76\x90\x4a\x57\x0f\x53\x76\x3b\x34\x34"
/*Z button:  try pressing again. */
# define STR_PAK_IDERR1		"\x33\x8f\x5f\x4f\x3e\x5a\x5b\x64\x54\x5b\x62\x6f\x78"
/* L+R buttons: restore */
# define STR_PAK_IDERR2		"\x25\x39\x2b\x8f\x5f\x4f\x3e\x5b\x4c\x52\x6b\x57\x5c\x78"


#else		/* English */

/***********************************************************************/
/* GAME NOTES            PAGES	*/
# define STR_MENU_ITEMS		"\x20\x1a\x26\x1e\x01\x27\x28\x2d\x1e\x2c\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x0f\x29\x1a\x20\x1e\x2c"
#define STR_MENU_TITLE	"\x1c\x28\x27\x2d\x2b\x28\x25\x25\x1e\x2b\x01\x29\x1a\x24\x01\x26\x1e\x27\x2e"
#define	STR_MENU_VER	"\x2f\x1e\x2b\x3c"
	
/***********************************************************************/
/* - ERASE	*/
# define STR_CMD_ERASE	\
"\x3b\x01\x1e\x2b\x1a\x2c\x1e"

/***********************************************************************/
/*  ERASE OK	*/
# define STR_CMD_OK	\
"\x1e\x2b\x1a\x2c\x1e\x01\x28\x24"

/***********************************************************************/
/*  CANCEL	*/
# define STR_CMD_QUIT	\
"\x1c\x1a\x27\x1c\x1e\x25"

/***********************************************************************/
/* QUIT		*/
# define STR_CMD_END	\
"\x2a\x2e\x22\x2d"

/***********************************************************************/
/*  NOT USED =	*/
# define STR_BLK_LEFT	\
"\x27\x28\x2d\x01\x2e\x2c\x1e\x1d\x3f"

/***********************************************************************/
/* NO DATA TO ERASE	*/
# define STR_PAK_NODEL	\
"\x27\x28\x0f\x1d\x1a\x2d\x1a\x0f\x2d\x28\x0f\x1e\x2b\x1a\x2c\x1e"

/***********************************************************************/
/* Controller-Pak ERROR */
# define STR_PAK_IDERR		"\x1c\x28\x27\x2d\x2b\x28\x25\x25\x1e\x2b\x3b\x29\x1a\x24\x0f\x1e\x2b\x2b\x28\x2b"
/*   Z button : pull and set again */
# define STR_PAK_IDERR1		"\x33\x0f\x1b\x2e\x2d\x2d\x28\x27\x3e\x29\x2e\x25\x25\x0f\x1a\x27\x1d\x0f\x2c\x1e\x2d\x0f\x1a\x20\x1a\x22\x27"
/* L+R button : Auto-REPAIR */
# define STR_PAK_IDERR2		"\x25\x39\x2b\x0f\x1b\x2e\x2d\x2d\x28\x27\x3e\x1a\x2e\x2d\x28\x3b\x2b\x1e\x29\x1a\x22\x2b"
#endif

#define	MENU_COLOR_TITLE	GPACK_RGBA5551( 80, 80,255,1)
#define	MENU_COLOR_ITEM		GPACK_RGBA5551(255,255,255,1)
#define	MENU_COLOR_FILE		GPACK_RGBA5551(255,255,255,1)
#define	MENU_COLOR_TARGET_0	GPACK_RGBA5551(255,255,  0,1)
#define	MENU_COLOR_TARGET_1	GPACK_RGBA5551(240,240,  5,1)
#define	MENU_COLOR_TARGET_2	GPACK_RGBA5551(224,224, 10,1)
#define	MENU_COLOR_TARGET_3	GPACK_RGBA5551(192,192, 21,1)
#define	MENU_COLOR_TARGET_4	GPACK_RGBA5551(128,128, 42,1)
#define	MENU_COLOR_TARGET_5	GPACK_RGBA5551( 64, 64, 64,1)
#define	MENU_COLOR_TARGET_6	GPACK_RGBA5551(128,128, 42,1)
#define	MENU_COLOR_TARGET_7	GPACK_RGBA5551(192,192, 21,1)
#define	MENU_COLOR_TARGET_8	GPACK_RGBA5551(224,224, 10,1)
#define	MENU_COLOR_TARGET_9	GPACK_RGBA5551(240,240,  5,1)

/*
#define	MENU_COLOR_EMPTY	GPACK_RGBA5551( 24, 24, 24,1)
#define	MENU_COLOR_SHADOW	GPACK_RGBA5551( 24, 24, 24,1)
*/

#define	MENU_COLOR_EMPTY	GPACK_RGBA5551( 0, 0, 0, 1)
#define	MENU_COLOR_NOTSEL	GPACK_RGBA5551(15,15,15, 1)
#define	MENU_COLOR_SHADOW	GPACK_RGBA5551( 0, 0, 0, 1)

#define	MENU_COLOR_IDERR	GPACK_RGBA5551(255, 0,0,1)

#if SCREEN_LOW
#define	SCREEN_WD	320
#define	SCREEN_HT	240
#else
#define	SCREEN_WD	640
#define	SCREEN_HT	480
#endif

#define	SCREEN_X0	56
#define	SCREEN_X1	(SCREEN_WD-70)
#define	SCREEN_Y0	40
#define	SCREEN_Y1	(SCREEN_HT-48)

#define	MENU_TITLE_X	MENU_NUM_X
#define	MENU_TITLE_Y	MENU_FILE_Y-FONT_HSKIP*2-8
#define	MENU_NUM_X	(144-32 - FONT_WSKIP)
#define	MENU_FILE_X	(MENU_NUM_X + 30)
#define	MENU_PART_X	(MENU_FILE_X + FONT_WSKIP * 16 + 4)
#define	MENU_BLOCK_X	(MENU_FILE_X + FONT_WSKIP * 17 + 8)
#define	MENU_FILE_Y	(36+20+28)
#define MENU_EXT_X	8
/*
#define	MENU_FREE_BLK_X	(MENU_BLOCK_X-FONT_WSKIP*8)
#define	MENU_FREE_BLK_Y (MENU_FILE_Y+FONT_HSKIP*16)
*/

#if	LANG_JAPANESE
# define MENU_FREE_BLK_X1 (MENU_FREE_BLK_X2-FONT_WSKIP*4)
#else
# define MENU_FREE_BLK_X1 (MENU_FREE_BLK_X2-FONT_WSKIP*8-4)
#endif
#define	MENU_FREE_BLK_Y1 (MENU_FREE_BLK_Y2)
#define	MENU_FREE_BLK_X2 (MENU_BLOCK_X)
#define	MENU_FREE_BLK_Y2 (MENU_FILE_Y+FONT_HSKIP*16+4)

#define	MENU_IS_OK_X	(MENU_BLOCK_X+FONT_WSKIP*4-8)
#define	MENU_IS_OK_YOFS	(-FONT_HSKIP/2)
#define	MENU_IS_NO_X	(MENU_IS_OK_X)
#define	MENU_IS_NO_YOFS	(+FONT_HSKIP/2)

#define	MENU_CMD_X	(SCREEN_X1-FONT_WSKIP*4)
#define	MENU_CMD_Y	(MENU_FILE_Y+FONT_HSKIP*4)

#define	MENU_WARN_X	(FONT_WSKIP*10)
#define	MENU_WARN_Y	(FONT_HSKIP*10)

#if	LANG_JAPANESE
#define	MENU_IDERR_X	(FONT_WSKIP*12)
#define	MENU_IDERR_Y	(FONT_HSKIP*7)
#define	MENU_IDERR_X1	(MENU_IDERR_X + FONT_WSKIP*2)
#define	MENU_IDERR_Y1	(MENU_IDERR_Y + FONT_HSKIP*6)
#define	MENU_IDERR_X2	(MENU_IDERR_X + FONT_WSKIP*0)
#define	MENU_IDERR_Y2	(MENU_IDERR_Y1 + FONT_HSKIP)
#else
#define	MENU_IDERR_X	(FONT_WSKIP*10)
#define	MENU_IDERR_Y	(FONT_HSKIP*7)
#define	MENU_IDERR_X1	(MENU_IDERR_X - FONT_WSKIP*2)
#define	MENU_IDERR_Y1	(MENU_IDERR_Y + FONT_HSKIP*6)
#define	MENU_IDERR_X2	(MENU_IDERR_X - FONT_WSKIP*4)
#define	MENU_IDERR_Y2	(MENU_IDERR_Y1 + FONT_HSKIP)
#endif

#if 0
#define	BGCOLOR		GPACK_RGBA5551(15,15,15,1)
#else
#define	BGCOLOR		GPACK_RGBA5551(0, 0, 0, 0)
#endif

#define	nWaitMesg(n)	osRecvMesg(n##MessageQ, &dummyMesg, OS_MESG_BLOCK)

#define	FBUF_SIZE	(SCREEN_WD*SCREEN_HT*2)
/*#define	FBUF_TOP	0x80300000*/
#define	FBUF_TOP	((u8*)nuGfxCfb_ptr)
#define	FONTARC_WD	13
#define	FONTARC_HT	14
#define	FONTARC_TOP	0xb0000b70
#define	FONTARC_BTM	0xb0000fee 
#define	FONTARC_SIZE	(FONTARC_BTM-FONTARC_TOP)

#define	FONT_WD		16
#define	FONT_HT		16
#define	FONT_WSKIP	16
#define	FONT_HSKIP	20
#define	FONT_PIX	(FONT_WD * FONT_HT)
#define	FONT_WOFS	0
#define	FONT_HOFS	2
#define	FONT_BUFSZ	(FONT_HT*2*(256-32))
#define	FONT_BUF	0x80200000

#define	SAVE_BITMAP	0x80280000

#define	F_ADRS(n)	((FONT_CODE_##n##-FONT_CODE_TOP) * FONT_HT)

#define FONT_CODE_QSKIP	0x01
#define FONT_CODE_TOP	0x0f
#define FONT_CODE_A	0x1a
#define FONT_CODE_0	0x10
#define FONT_CODE_i	0x34
#define FONT_CODE_o	0x42
#define FONT_CODE_NON	0x3b

#if	LANG_JAPANESE
# define FONT_CODE_BTM	0x94
# define FONT_CODE_KA	0x55		// code "�J"(KA)
# define FONT_CODE_NA	0x64		// code "�i"(NA)
# define FONT_CODE_HA	0x69		// code "�n"(HA)
# define FONT_CODE_MA	0x6e		// code "�}"(HA)
# define FONT_CODE_GA	0x7c		// code "�K"(GA)
# define FONT_CODE_BA	0x8b		// code "�o"(BA)
# define FONT_CODE_PA	0x90		// code "�p"(PA)
#else
# define FONT_CODE_BTM	0x41
#endif

#define	FBUF_ADRS(x,y)		(FBUF_TOP+((x)+(y)*SCREEN_WD)*2)
#define	canvas_cursor(x,y)	__cursor = (u16 *)FBUF_ADRS((x),(y))
#define	canvas_center(x,y)	canvas_cursor((SCREEN_WD-(x)*FONT_WSKIP)/2,(y))
#define	canvas_wskip(x)		__cursor += (x);
#define	canvas_putstr(n,fg,bg)	canvas_puts((n), sizeof(n)-1, (fg), (bg))

#define	ERR_OSPFSNUMFILES	1

#define check255(x)		( ((x) > 255) ? 255 : x)

#endif
