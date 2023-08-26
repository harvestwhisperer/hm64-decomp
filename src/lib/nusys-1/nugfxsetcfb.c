#include "nusys.h"

//INCLUDE_ASM(const s32, "lib/nusys-1/nugfxsetcfb", nuGfxSetCfb);

void nuGfxSetCfb(u16** framebuf, u32 framebufnum) {
    nuGfxCfb = framebuf;
    nuGfxCfbNum = framebufnum;
    nuGfxCfbCounter = 0;
    nuGfxCfb_ptr = nuGfxCfb[0];
}
