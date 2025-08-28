#include "n_synthInternals.h"

N_ALGlobals* n_alGlobals = 0;
N_ALSynth* n_syn = 0;

void n_alInit(N_ALGlobals* g, ALSynConfig* c)
{
    if (!n_alGlobals) { /* already initialized? */
        n_alGlobals = g;
        if (!n_syn) {
            n_syn = &n_alGlobals->drvr;
            n_alSynNew(c);
        }
    }
}

void n_alClose(void)
{
    if (n_alGlobals) {
        n_alSynDelete();
        n_alGlobals = 0;
        n_syn = 0;
    }
}
