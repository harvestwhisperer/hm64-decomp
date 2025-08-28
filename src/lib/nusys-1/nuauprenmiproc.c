#include <nusys.h>
#include <nualstl_n.h>

void nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter) {

    switch(mesg_type) {
		
		case NU_SC_PRENMI_MSG:	
		
			nuAuStlSeqPlayerStop(28);
			nuAuStlSndPlayerStop(28);
		#ifdef NU_DEBUG	
			osSyncPrintf("nuAuPreNMIProc: Get PRNMI MSG\n");
		#endif 
			break;

		case NU_SC_RETRACE_MSG:	
		
			if (frameCounter == 28) {
				nuAuTaskStop = NU_AU_TASK_STOP;
			}

			break;

    }

}
