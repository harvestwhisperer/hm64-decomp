#include <nusys.h>

void nuGfxFuncSet(NUGfxFunc func) {

    OSIntMask mask;
    nuGfxTaskAllEndWait();	    

    mask = osSetIntMask(OS_IM_NONE);
    nuGfxFunc = func;

    osSetIntMask(mask);

}
