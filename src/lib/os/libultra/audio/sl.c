#include <libaudio.h>

ALGlobals* alGlobals = 0;

void alInit(ALGlobals* g, ALSynConfig* c)
{
    if (!alGlobals) { /* already initialized? */
        alGlobals = g;
        alSynNew(&alGlobals->drvr, c);
    }
}

void alClose(ALGlobals* glob)
{
    if (alGlobals) {
        alSynDelete(&glob->drvr);
        alGlobals = 0;
    }
}

/* might want to make these macros */
void alLink(ALLink* ln, ALLink* to)
{
    ln->next = to->next;
    ln->prev = to;
    if (to->next)
        to->next->prev = ln;
    to->next = ln;
}

void alUnlink(ALLink* ln)
{
    if (ln->next)
        ln->next->prev = ln->prev;
    if (ln->prev)
        ln->prev->next = ln->next;
}





