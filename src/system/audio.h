#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "common.h"

#include "system/interpolation.h"

#define SONG_TYPE 1
#define SFX_TYPE 0xFF
#define MAX_ACTIVE_SEQUENCES 4
#define MAX_ACTIVE_SFX 4
#define TOTAL_SEQUENCES 64
#define TOTAL_SFX 128

#define SFX_VOLUME 128
#define SEQUENCE_VOLUME 128

#define MAX_VOLUME 256

#define AUDIO_ACTIVE 1
#define AUDIO_START 2
#define AUDIO_STOP 4

typedef struct {
    u8 *currentAudioSequenceRomAddrStart;
    u8 *currentAudioSequenceRomAddrEnd;
    Interpolator volumes;
    musHandle handle;
    s32 unused1; // seem to be volumes 
    s32 unused2;
    u32 speed;
    u8 numChannelsPlaying;
    u16 flags;
} SequenceInfo;

typedef struct {
	u32 sfxIndex;
	musHandle handle;
    s32 frequency;
	s32 pan;
	s32 volume;
	u8 numSfxPlaying;
    u16 flags;
} Sfx;

typedef struct {
    u8 *pbk_addr;
    u8 *pbk_end;
    u8 *wbk_addr;
} WaveTableInfo;

extern void initializeAudio(musConfig*);      
extern void updateAudio();
extern bool setAudioSequence(u16 sequenceIndex, u8 *sequenceAddrStart, u8 *sequenceAddrEnd);
extern bool stopAudioSequenceWithFadeOut(u16 index, u32 speed);
extern bool stopAudioSequence(u16 sequenceIndex);
extern bool setAudioSequenceVolumes(u16 index, s32 maxVolume, s16 arg2);
extern void setAudioSequenceBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart);
extern bool setSfx(u32 sfxIndex);
extern bool func_8003D6A8(u32 sfxIndex);                     
extern bool setSfxVolume(u32 sfxIndex, s32 volume);  
extern bool setSfxFrequency(u32 sfxIndex, s32 frequency);                   
extern bool setSfxPan(u32 sfxIndex, s32 arg1);
extern void stopSfx(void);

extern u16 gCurrentAudioSequenceIndex;
extern s32 gAudioSequenceVolume;

extern Sfx gSfx[4];
extern SequenceInfo gAudioSequences[4];

#endif