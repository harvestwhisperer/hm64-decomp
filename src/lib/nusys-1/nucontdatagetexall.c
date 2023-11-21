#include "nusys.h"

//INCLUDE_ASM(const s32, "lib/nusys-1/nucontdatagetexall", nuContDataGetExAll);

void nuContDataGetExAll(NUContData *contdata) {

    u16	button;
    u32	cnt;
    
    nuContDataClose(); 
 
    for (cnt = 0; cnt < NU_CONT_MAXCONTROLLERS; cnt++) {
 
        button = contdata[cnt].button;
        _bcopy(&nuContData[cnt], &contdata[cnt], sizeof(OSContPad));
        
        contdata[cnt].trigger = nuContData[cnt].button & (~button);
    
    }
    
    nuContDataOpen();	

}
