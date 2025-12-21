#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include "common.h"

#include "system/audio.h"

extern void initializeWaveTable(u16 waveTableIndex);
extern void setCurrentAudioSequence(u16 sequenceIndex);
extern void stopCurrentAudioSequence(u16 sequenceIndex);
extern void stopAudioSequenceWithDefaultFadeOut(u16 sequenceIndex);
extern u8 checkDefaultSequenceChannelOpen(u16 sequenceIndex);
extern void setAudioSequenceVolume(u16 sequenceIndex, u32 arg1);
extern void func_800ACC1C(u16 sequenceIndex);
extern void playSfx(u16 index);
extern u8 checkAllSfxInactive(void);

#endif