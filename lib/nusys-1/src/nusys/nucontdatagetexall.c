#include <nusys.h>

void nuContDataGetExAll(NUContData *contdata) {

    u16	button;
    u32	cnt;
    
    nuContDataClose();	    
 
    for(cnt =0; cnt < NU_CONT_MAXCONTROLLERS; cnt++) {
        button = contdata[cnt].button;
        bcopy(&nuContData[cnt], &contdata[cnt], sizeof(OSContPad));
        
        
        contdata[cnt].trigger = nuContData[cnt].button & (~button);
        
    }
    
    nuContDataOpen();	

}
