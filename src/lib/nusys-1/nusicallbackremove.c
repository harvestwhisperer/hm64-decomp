#include <nusys.h>

void nuSiCallBackRemove(NUCallBackList* list) {

    OSIntMask		mask;
    NUCallBackList*	siCallBackListPtr;
    NUCallBackList*	forwardPtr;

    siCallBackListPtr = nuSiCallBackList.next;
    forwardPtr = &nuSiCallBackList;

    while (siCallBackListPtr) {

		if (siCallBackListPtr->func == list->func) {
			
			mask = osSetIntMask(OS_IM_NONE);
			
			forwardPtr->next = list->next;
			list->next = NULL;
			list->func = NULL;
			
			osSetIntMask(mask);
			
			break;

		}
		
		forwardPtr = siCallBackListPtr;
		siCallBackListPtr = siCallBackListPtr->next;

    }

}
