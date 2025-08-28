#ifndef __N_LIBAUDIO__
#define __N_LIBAUDIO__

#include <PR/libaudio.h>

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/ultratypes.h>
#include <PR/mbi.h>
    
typedef struct N_ALVoice_s {
    ALLink              node;
    struct N_PVoice_s     *pvoice;
    ALWaveTable         *table;
    void                *clientPrivate;
    s16                 state;
    s16                 priority;
    s16                 fxBus;
    s16                 unityPitch;
} N_ALVoice;

typedef struct {
    ALPlayer    *head;          /* client list head                     */
    ALLink      pFreeList;      /* list of free physical voices         */
    ALLink      pAllocList;     /* list of allocated physical voices    */
    ALLink      pLameList;      /* list of voices ready to be freed     */
    s32         paramSamples;
    s32         curSamples;     /* samples from start of game           */
    ALDMANew    dma;
    ALHeap      *heap;
    struct ALParam_s    *paramList;
    struct N_ALMainBus_s  *mainBus;
    struct N_ALAuxBus_s   *auxBus; 
    s32                 numPVoices;
    s32                 maxAuxBusses;
    s32                 outputRate; 
    s32                 maxOutSamples;
    s32	       		sv_dramout;
    s32                 sv_first;
} N_ALSynth;

/*************************************************************/
void     n_alSynAddPlayer(ALPlayer *client);
ALFxRef  n_alSynAllocFX(s16 bus, ALSynConfig *c, ALHeap *hp);
s32      n_alSynAllocVoice(N_ALVoice *voice, ALVoiceConfig *vc);
void     n_alSynDelete(void);
void     n_alSynFreeVoice(N_ALVoice *voice);
ALFxRef  n_alSynGetFXRef(s16 bus, s16 index);
s16      n_alSynGetPriority(N_ALVoice *voice);
void     n_alSynRemovePlayer(ALPlayer *client);
void     n_alSynSetFXMix(N_ALVoice *v, u8 fxmix);
void     n_alSynSetFXParam(ALFxRef fx, s16 paramID, void *param);
void     n_alSynSetFXtype(s16 bus, ALFxId fxid, s32 rate);
void     n_alSynSetPan(N_ALVoice *v, u8 pan);
void     n_alSynSetPitch(N_ALVoice *v, f32 pitch);
void     n_alSynSetPriority(N_ALVoice *voice, s16 priority);
void     n_alSynSetVol(N_ALVoice *v, s16 volume, ALMicroTime t);
void     n_alSynStartVoice(N_ALVoice *v, ALWaveTable *table);
void     n_alSynStartVoiceParams(N_ALVoice *v, ALWaveTable *w,f32 pitch,
			     s16 vol, ALPan pan, u8 fxmix, ALMicroTime t);
void     n_alSynStopVoice(N_ALVoice *v);
void     n_alSynNew(ALSynConfig *c);

/*************************************************************/

/***********************************************************************
 * Audio Library (AL) stuff
 ***********************************************************************/
typedef struct {
    N_ALSynth     drvr;
} N_ALGlobals;

extern N_ALGlobals *n_alGlobals;
extern N_ALSynth   *n_syn;

void  n_alInit(N_ALGlobals *g, ALSynConfig *c);
void  n_alClose(void);
Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen);

/*
 *  for n_audio micro code
 */
extern long long int    n_aspMainTextStart[], n_aspMainTextEnd[];
extern long long int    n_aspMainDataStart[], n_aspMainDataEnd[];

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* !__N_LIBAUDIO__ */



