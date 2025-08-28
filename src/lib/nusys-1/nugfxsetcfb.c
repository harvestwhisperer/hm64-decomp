#include <nusys.h>

void nuGfxSetCfb(u16** framebuf, u32 framebufnum) {
    
    nuGfxCfb	= framebuf;
    nuGfxCfbNum	= framebufnum;
    nuGfxCfbCounter = 0;
    nuGfxCfb_ptr = nuGfxCfb[0];

}
