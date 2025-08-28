#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


void n_alSynStopVoice(N_ALVoice *v)
{
    ALParam  *update;
    ALFilter *f;
    
    if (v->pvoice) {
        
        update = __n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

#ifdef SAMPLE_ROUND
	update->delta  = SAMPLE184( n_syn->paramSamples + v->pvoice->offset);
#else
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
#endif
        update->type   = AL_FILTER_STOP_VOICE;
        update->next   = 0;

	n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);        
    }
}

