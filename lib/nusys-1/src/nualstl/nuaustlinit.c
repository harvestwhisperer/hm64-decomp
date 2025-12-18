#include <nusys.h>
#include <nualstl_n.h>

ALSynConfig	auSynConfig = {
    NU_AU_SYN_VVOICE_MAX,	
    NU_AU_SYN_PVOICE_MAX,	
    NU_AU_SYN_UPDATE_MAX,	
    0,
    NULL,				
    NULL,				
    NU_AU_OUTPUT_RATE,		
    AL_FX_BIGROOM,		
    0,				
};

ALSeqpConfig auSeqpConfig = {
	NU_AU_SEQ_VOICE_MAX,
	NU_AU_SEQ_EVENT_MAX,
	NU_AU_SEQ_CHANNEL_MAX,
	0,
	NULL,
	NULL,
	NULL,
	NULL
};

ALSndpConfig anSndpConfig = {
    NU_AU_SND_SOUND_MAX,
    NU_AU_SND_EVENT_MAX,
    NULL
};

void nuAuStlInit(musConfig *c) {
    
    unsigned char *addr;
    unsigned long length;

    auSynConfig.outputRate = NU_AU_OUTPUT_RATE;
    c->channels = NU_AU_SYN_PVOICE_MAX;
    c->thread_priority = 12;

    addr = (void*)NU_AU_HEAP_ADDR;
    length = NU_AU_HEAP_SIZE;
    
    while (length--) *addr++ = 0;
    
    nuAuStlMgrInit((void*)NU_AU_HEAP_ADDR, NU_AU_HEAP_SIZE, &auSynConfig);
    nuAuStlPlayerInit(c, 0x8000);
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

}
