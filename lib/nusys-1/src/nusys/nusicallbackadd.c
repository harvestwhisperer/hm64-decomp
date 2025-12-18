#include <nusys.h>

void nuSiCallBackAdd(NUCallBackList* list, NUCallBackFunc func) {

    OSIntMask		mask;
    NUCallBackList*	siCallBackListPtr;
    NUCallBackList*	LastPtr;

    siCallBackListPtr = &nuSiCallBackList;
    
    while (siCallBackListPtr) {

        if (siCallBackListPtr->func == func) {
    #ifdef NU_DEBUG
            osSyncPrintf("nuSiCallBackAdd: CallBack is already added!!\n");
    #endif 
            return;
        }

        LastPtr = siCallBackListPtr;
        siCallBackListPtr = siCallBackListPtr->next;
    }
    
    mask = osSetIntMask(OS_IM_NONE);
    list->next = LastPtr->next;
    list->func = func;
    LastPtr->next = list;

    osSetIntMask(mask);

}
