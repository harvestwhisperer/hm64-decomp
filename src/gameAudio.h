#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include "common.h"
#include "system/audio.h"

#define TITLE_THEME 1
#define RAIN_SFX 32
#define TYPHOON_SFX 34

void initializeWaveTable(u16 waveTableIndex);
void func_800ACB04(u16 songIndex);
void setSongWithDefaultSpeed(u16 songIndex);
u8 checkDefaultSongChannelOpen(u16 songIndex);
void setSongWithVolume(u16 songIndex, u32 arg1);
void func_800ACC1C(u16 songIndex);
void setAudio(u16 index);
u8 checkAllSfxInactive(void);

extern int musPriorityList[];
extern u8 *sfxList[];

#endif