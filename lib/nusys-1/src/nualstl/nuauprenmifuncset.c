#include <nusys.h>
#include <nualstl_n.h>

void nuAuPreNMIFuncSet(NUAuPreNMIFunc func) {
    
    OSIntMask	mask;
    
    mask = osSetIntMask(OS_IM_NONE);		
    nuAuPreNMIFunc = func;
    osSetIntMask(mask);

}
