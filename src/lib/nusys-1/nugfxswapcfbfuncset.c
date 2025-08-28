#include <nusys.h>

void nuGfxSwapCfbFuncSet(NUGfxSwapCfbFunc func) {
    
    OSIntMask mask;
    
    mask = osSetIntMask(OS_IM_NONE);
    nuGfxSwapCfbFunc = func;
    osSetIntMask(mask);

}

