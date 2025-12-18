#define ALVoice                N_ALVoice
#define ALSynth                N_ALSynth
#define ALGlobals              N_ALGlobals

#define alSynAddPlayer( a, b)         n_alSynAddPlayer( b)
#define alSynAllocFX( a, b, c, d)     n_alSynAllocFX( b, c, d)
#define alSynAllocVoice( a, b, c)     n_alSynAllocVoice( b, c)
#define alSynDelete( a)               n_alSynDelete()
#define alSynFreeVoice( a, b)         n_alSynFreeVoice( b)
#define alSynGetFXRef( a, b, c)       n_alSynGetFXRef( b, c)
#define alSynGetPriority( a, b)       n_alSynGetPriority( b)
#define alSynRemovePlayer( a, b)      n_alSynRemovePlayer( b)
#define alSynSetFXMix( a, b, c)       n_alSynSetFXMix( b, c)
#define alSynSetFXParam( a, b, c, d)  n_alSynSetFXParam( b, c, d)
#define alSynSetFXtype( a, b, c, d)   n_alSynSetFXtype( b, c, d)
#define alSynSetPan( a, b, c)         n_alSynSetPan( b, c)
#define alSynSetPitch( a, b, c)       n_alSynSetPitch( b, c)
#define alSynSetPriority( a, b, c)    n_alSynSetPriority( b, c)
#define alSynSetVol( a, b, c, d)      n_alSynSetVol( b, c, d)
#define alSynStartVoice( a, b, c)     n_alSynStartVoice( b, c)
#define alSynStartVoiceParams( a, b, c, d, e, f, g, h) \
                                      n_alSynStartVoiceParams( b, c, d, e, f, g, h)
#define alSynStopVoice( a, b)         n_alSynStopVoice( b)
#define alSynNew( a, b)               n_alSynNew( b)

#define alInit                        n_alInit
#define alClose( a )                  n_alClose()
#define alAudioFrame                  n_alAudioFrame
