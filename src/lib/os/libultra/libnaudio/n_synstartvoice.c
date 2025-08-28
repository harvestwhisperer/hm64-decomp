#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"

void n_alSynStartVoice(N_ALVoice *v, ALWaveTable *table)
{
    ALStartParam  *update;
    
    if (v->pvoice) {
        
        update = (ALStartParam *)__n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        /*
         * send the start message to the motion control filter
         */
#ifdef SAMPLE_ROUND
	update->delta  = SAMPLE184( n_syn->paramSamples + v->pvoice->offset);
#else
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
#endif

        update->type   = AL_FILTER_START_VOICE;
        update->wave   = table;
        update->next   = 0;
        update->unity  = v->unityPitch;

	n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);        
    }
}

