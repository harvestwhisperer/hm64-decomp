#ifndef _NUPAKMENU_H_
#define	_NUPAKMENU_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#define	NU_PAKMENU_NOPAK		0
#define	NU_PAKMENU_ERROR		1
#define	NU_PAKMENU_NOT_START		2
#define	NU_PAKMENU_SELECT_END		3
#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)
extern s32	nuPakMenu(void* frameBuffer, void* fontBuffer);
extern void	nuPakMenuLoadFont(u16 *font_buf);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */


#endif /* _NUPAKMENU_H_ */
