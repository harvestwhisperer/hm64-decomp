#include <nusys.h>

void nuGfxSwapCfb(void* gfxTask) {
    
    NUScTask*	gfxTaskPtr;
    gfxTaskPtr = (NUScTask*)gfxTask;
    
    osViSwapBuffer(gfxTaskPtr->framebuffer);

}
