#include <nusys.h>

void nuGfxPreNMIFuncSet(NUGfxPreNMIFunc func) {
    
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);
    nuGfxPreNMIFunc = func;
    osSetIntMask(mask);

}
