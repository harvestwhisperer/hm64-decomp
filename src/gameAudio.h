#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include "common.h"
#include "system/audio.h"

void initializeWaveTable(u16 waveTableIndex);
void func_800ACB04(u16 songIndex);
void func_800ACB5C(u16 songIndex);
u8 func_800ACBB8(u16 songIndex);
void func_800ACBEC(u16 songIndex, u32 arg1);
void func_800ACC1C(u16 songIndex);
void setAudio(u16 index);
u8 checkAllSfxInactive(void);

extern int musPriorityList[];
extern u8 *sfxList[];

#endif