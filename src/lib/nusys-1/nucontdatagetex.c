#include <nusys.h>

void nuContDataGetEx(NUContData *contdata, u32 padno) {
    
    u16	button;

    button = contdata->button;
    nuContDataClose();	    
    bcopy(&nuContData[padno], contdata, sizeof(OSContPad));

    
    contdata->trigger = nuContData[padno].button & (~button);
    nuContDataOpen();	

}
