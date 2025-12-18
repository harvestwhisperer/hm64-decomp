#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"

void n_alSynSetFXMix(N_ALVoice *v, u8 fxmix)
{
    ALParam  *update;

    if (v->pvoice) {
        /*
         * get new update struct from the free list
         */
        update = __n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        /*
         * set offset and fxmix data
         */
#ifdef SAMPLE_ROUND
	update->delta  = SAMPLE184( n_syn->paramSamples + v->pvoice->offset);
#else
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
#endif
        update->type   = AL_FILTER_SET_FXAMT;
	if (fxmix > 127)
	    fxmix = 127;
	update->data.i = fxmix;
        update->next   = 0;

	n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
    }
}

