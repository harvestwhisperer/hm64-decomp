#include <nusys.h>
#include "nupakmenu.h"
#include "pakmenu.h"

extern	void	canvas_putc(int, u16, u16);
extern	void	canvas_puts(u8*, int, u16, u16);
extern	u16*	__cursor;
extern	void	canvas_u8_to_str(u16, u8*);

static	u16	cursor_color[] = {
    MENU_COLOR_TARGET_7, MENU_COLOR_TARGET_8, MENU_COLOR_TARGET_9,
    MENU_COLOR_TARGET_0, MENU_COLOR_TARGET_1, MENU_COLOR_TARGET_2,
    MENU_COLOR_TARGET_3, MENU_COLOR_TARGET_4, MENU_COLOR_TARGET_5,
    MENU_COLOR_TARGET_6,
};

OSPfsState	pfs_stat[16];
s32		pfs_stat_ret[16];

void __display_term(int sel, int n, u16 fg1, u16 fg2, u16 bg) {

    u8	ibuf[4];
    int	cy;

    static u8 *cmd_str[] = {
        STR_CMD_ERASE, STR_CMD_OK, STR_CMD_QUIT,
    };

    static u16 cmd_str_len[] = {
        sizeof(STR_CMD_ERASE)-1, sizeof(STR_CMD_OK)-1, sizeof(STR_CMD_QUIT)-1,
    };

    static s16 cmd_str_yofs[] = {
        8, MENU_IS_OK_YOFS*SCREEN_WD+8, MENU_IS_NO_YOFS*SCREEN_WD+8,
    };

    cy = n * FONT_HSKIP + MENU_FILE_Y;

    if (n < 16) {

        if (!pfs_stat_ret[n]) {
      
            canvas_cursor(MENU_FILE_X, cy);
            canvas_puts(pfs_stat[n].game_name, 16, fg1, bg);

            if (pfs_stat[n].ext_name[0]) {
                canvas_putc(0x3c, fg1, bg);
                __cursor -= MENU_EXT_X;
                canvas_putc(pfs_stat[n].ext_name[0], fg1, bg);
            }

            canvas_cursor(MENU_BLOCK_X, cy);
            canvas_u8_to_str((u16)((pfs_stat[n].file_size+255) >> 8), ibuf);
            canvas_puts(ibuf, 3, fg1, bg); 

            __cursor += cmd_str_yofs[sel];
            canvas_puts(cmd_str[sel], cmd_str_len[sel], fg2, bg);
    
        }
    
        if (n < 9) {
            canvas_cursor(MENU_NUM_X + 3, cy);
            canvas_putc  (n+0x11, fg1, bg);
        } else {
            canvas_cursor(MENU_NUM_X - 4, cy);
            canvas_putc(0x11, fg1, bg);
            canvas_cursor(MENU_NUM_X + 4, cy);
            canvas_putc(n+7, fg1, bg);
        }

        canvas_cursor(MENU_NUM_X + 15, cy);
        canvas_putc(0x3e, fg1, bg);
    
  } else {

    canvas_cursor(MENU_NUM_X-2, cy+4);
    canvas_putstr(STR_CMD_END, fg1, bg);
  
  }
    
}

void __display_free_block(u16 free_blocks) {
    
  u8	ibuf[4];
  int	i;
  int	j;
  int	k;
  
  canvas_u8_to_str(free_blocks, ibuf);
  canvas_cursor(MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2);

  for (i = 0; i < FONT_HT; i ++) {
    for (j = 0; j < FONT_WSKIP * 3; j ++) {
      k = i * SCREEN_WD + j;
      ((u16 *)FBUF_ADRS(MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2))[k] = (u16)BGCOLOR;
    }
  }

  canvas_puts(ibuf, 3, MENU_COLOR_FILE, MENU_COLOR_SHADOW);

}

s32 nuPakMenu(void* frameBuffer, void* fontBuffer) {

    NUContData contData;
    NUContPakFile pakFile;
    NUGfxSwapCfbFunc funcBack;
    u16* cfbBack;
    u16 fg;
    u16 repeat_trig, repeat_cnt; 
    u16 companyCodeBackup;
    u32 gameCodeBackup;
    s32 file_num, file_num_max, free_blocks;
    s32 mode, save_mode;
    s32 target;
    s32 save_target;
    s32 i;
    s32 flash_cnt;
    s32 screen_type;
    s32 rtn = 0;

    nuGfxDisplayOff();

    nuContQueryRead();

    if ((nuContNum == 0) || nuContStatus[0].errno) {
	    return NU_PAKMENU_NOPAK;
    }

    if (!(nuContStatus[0].status & CONT_CARD_ON)) {
    	return NU_PAKMENU_NOPAK;
    }
    
    nuContDataReadStart();		
    nuContDataReadWait();		
    nuContDataGetEx(&contData, 0);	

    if (!(contData.button & START_BUTTON)) {
    	return NU_PAKMENU_NOT_START;
    }
    
    nuContPakOpen(&pakFile, 0);
    mode = pakFile.error;
    
    if ((mode != 0) && (mode != PFS_ERR_ID_FATAL)) {
	    return NU_PAKMENU_NOPAK;
    }

    nuPakMenuLoadFont((u16*)fontBuffer);
    
    bzero(frameBuffer, FBUF_SIZE);
    bzero(NU_GFX_FRAMEBUFFER0_ADDR, 0x70800);

    nuGfxRetraceWait(1);

    funcBack = nuGfxSwapCfbFunc;	
    nuGfxSwapCfbFuncRemove();

    cfbBack = nuGfxCfb_ptr;
    nuGfxCfb_ptr = (u16*)frameBuffer;
    osViSwapBuffer((void*)nuGfxCfb_ptr);
    nuGfxRetraceWait(1);
    
    osViSetMode(&osViModeTable[OS_VI_NTSC_HPF1]); 
    nuGfxDisplayOn();	

    companyCodeBackup = nuContPakCompanyCode;
    gameCodeBackup = nuContPakGameCode;
    
    if (mode == PFS_ERR_ID_FATAL) {

        canvas_cursor(MENU_IDERR_X, MENU_IDERR_Y);
        canvas_putstr(STR_PAK_IDERR, MENU_COLOR_IDERR, MENU_COLOR_TITLE);
        canvas_cursor(MENU_IDERR_X1, MENU_IDERR_Y1);
        canvas_putstr(STR_PAK_IDERR1, MENU_COLOR_TITLE, MENU_COLOR_TITLE);
        canvas_cursor(MENU_IDERR_X2, MENU_IDERR_Y2);
        canvas_putstr(STR_PAK_IDERR2, MENU_COLOR_TITLE, MENU_COLOR_TITLE);

        osWritebackDCacheAll();

        nuGfxRetraceWait(1);
        
        do {

            nuContDataGetEx(&contData, 0);
            
            if (contData.button == 0) {
                continue;
            } else if (contData.button == (L_TRIG|R_TRIG)) {
                nuContPakRepairId(&pakFile);
            }
            
            nuContPakOpen(&pakFile, 0);
            mode = pakFile.error;
        
        } while (mode != 0);

    }

    nuContPakFileNum(&pakFile, &file_num_max, &file_num);

    if (pakFile.error) {
        rtn = NU_PAKMENU_NOPAK;
        goto FUNC_END;
    }

    for (i = 0; i < 16; i++) {
        pakFile.file_no = i;
        nuContPakFileState(&pakFile, pfs_stat + i);
        pfs_stat_ret[i] = pakFile.error;
    }

    free_blocks =  nuContPakGetFree(&pakFile);
    
    if (pakFile.error) {
        rtn =  NU_PAKMENU_NOPAK;
    	goto FUNC_END;
    }

    free_blocks >>= 8;

    screen_type = STYPE_MENU;
    
display_screen:

    bzero(nuGfxCfb_ptr, FBUF_SIZE);	

    switch(screen_type) {

        case STYPE_MENU:

            canvas_cursor(MENU_TITLE_X, MENU_TITLE_Y);
            canvas_putstr(STR_MENU_TITLE, MENU_COLOR_TITLE, MENU_COLOR_SHADOW);

            canvas_cursor(MENU_NUM_X, MENU_FILE_Y-FONT_HSKIP);
            canvas_putstr(STR_MENU_ITEMS, MENU_COLOR_ITEM, MENU_COLOR_SHADOW);
            canvas_cursor(0xF2, MENU_FREE_BLK_Y1);
            canvas_putstr("\x1F\x2B\x1E\x1E\x01\x29\x1A\x20\x1E\x2C\x3F" , MENU_COLOR_ITEM, MENU_COLOR_SHADOW);
            __display_free_block((u16)(free_blocks));
            
            for (i = 0; i < 17; i ++) {
                __display_term(0, i, MENU_COLOR_FILE,  MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
            }

            break;
            
    
        case STYPE_NODEL:

            canvas_cursor(MENU_WARN_X, MENU_WARN_Y);
            canvas_putstr(STR_PAK_NODEL, MENU_COLOR_TITLE, MENU_COLOR_TITLE);
            file_num = 0;
            
            break;
        
        default:
            break;
    
    }

    
    osWritebackDCacheAll();    
    
    nuGfxRetraceWait(1);

    repeat_cnt = 0;
    repeat_trig = 0;
    target = 16;
    flash_cnt = 0;
    mode = 0;
    
    while (1) {

        save_target = target;
        save_mode   = mode;

        switch (mode) {

            case 0:

                if (contData.trigger & A_BUTTON) {    
                
                    if (target == 16) {
                        if (screen_type == STYPE_NODEL) {
                        rtn =  NU_PAKMENU_ERROR;
                        goto FUNC_END;
                        } else {
                        rtn =  NU_PAKMENU_SELECT_END;
                        goto FUNC_END;
                        }
                    }
                
                    
                    for (i = 0; i < 17; i ++) {
                        __display_term(0, i, MENU_COLOR_NOTSEL,
                                MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
                    }
                    mode = 2;
                    
                } else if ((contData.trigger | repeat_trig) & U_JPAD) {
                
                    while (pfs_stat_ret[--target] && target >= 0);

                    if (target == -1) target = 16;
                
                } else if ((contData.trigger | repeat_trig) & D_JPAD) {
            
next_target:
                    if (target == 16) target = -1;
                    while (pfs_stat_ret[++target] && target <= 15);
                    
                } else if (contData.trigger & B_BUTTON) {
                
                    target = 16;
                
                }
                
                break;

            case 1:

                if (contData.trigger & D_JPAD) mode = 2;
                if (contData.trigger & B_BUTTON) mode = 0;

                if (contData.trigger & A_BUTTON) {

                    nuContPakCodeSet((u8*)&pfs_stat[target].company_code,
                        (u8*)&pfs_stat[target].game_code);
                
                    nuContPakFileDelete(&pakFile,
                        pfs_stat[target].game_name,
                        pfs_stat[target].ext_name);
                    
                    if (pakFile.error) {
                        screen_type = STYPE_NODEL;
                        goto display_screen;
                    }
                    
                    __display_term(1, target, MENU_COLOR_EMPTY,
                        MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
                    __display_term(2, target, MENU_COLOR_EMPTY,
                        MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
                    
                    mode = 0;
                    
                    pfs_stat_ret[target] = -1;
                    
                    free_blocks += (u8)((pfs_stat[target].file_size+255) >> 8);
                    __display_free_block((u16)(free_blocks));
                
                    goto next_target;
                
                }      

                break;
                
            case 2:
                
                if (contData.trigger & U_JPAD) mode = 1;
                if (contData.trigger & (A_BUTTON|B_BUTTON)) mode = 0;
                
                break;

            default:
                break;

	    }
    
        if (!file_num) target = 16;
        
        if (mode != save_mode) {

            fg = (u16)MENU_COLOR_FILE;
            
            if (mode == 0) {

                for (i = 0; i < 17; i ++) {
                    __display_term(0, i, MENU_COLOR_FILE,
                        MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
                }
                fg = (u16)MENU_COLOR_SHADOW;
                
            }

            __display_term(1, save_target, MENU_COLOR_FILE, fg, MENU_COLOR_SHADOW);
            __display_term(2, save_target, MENU_COLOR_FILE, fg, MENU_COLOR_SHADOW);
            
            flash_cnt = 0;
        
        } else if (target != save_target) {
            
            __display_term(0, save_target,
                MENU_COLOR_FILE, MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
            flash_cnt = 0;
        
        }
        
        __display_term(mode, target,
                cursor_color[flash_cnt/3],
                cursor_color[flash_cnt/3], cursor_color[flash_cnt/3]);

        if ((++ flash_cnt) == 30) flash_cnt = 0;

        osWritebackDCacheAll();
        nuGfxRetraceWait(1);
        nuContDataGetEx(&contData, 0);	
        
        repeat_trig = contData.button & contData.trigger;
        
        if (repeat_trig) {
        
            repeat_cnt ++;

            if ((repeat_cnt>>5)==0 || (repeat_cnt&7)!=0) repeat_trig = 0;
        
        } else {
            repeat_cnt = 0;
        }

    }

FUNC_END:
    
    nuGfxDisplayOff();
    bzero(frameBuffer, FBUF_SIZE);

    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);

    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON
        | OS_VI_GAMMA_OFF
        | OS_VI_GAMMA_DITHER_OFF
        | OS_VI_DIVOT_ON);
    
    nuGfxDisplayOff();
    nuGfxCfb_ptr = cfbBack;
    nuGfxSwapCfbFuncSet(funcBack);

    nuContPakCompanyCode = companyCodeBackup;
    nuContPakGameCode = gameCodeBackup;

    return rtn;

}
