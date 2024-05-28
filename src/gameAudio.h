#ifndef _GAME_AUDIO_H_
#define _GAME_AUDIO_H_

#include "common.h"
#include "system/audio.h"

// TODO: finish adding songs
#define TITLE_THEME 1
#define NAMING_SCREEN_THEME 8

// TODO: finish adding sfx
#define CLOSE 1
#define MOVE_CURSOR 2
#define STORE_ITEM 4
#define SELECT 7
#define WHISTLE 10
#define SICKLE_SFX 11
#define AX_SFX 17
#define SEEDS 29
#define MILKER_SFX 30
#define RAIN_SFX 32
#define TYPHOON_SFX 34
#define ROOSTER 63
#define CHICKEN_CLUCK 64
#define BIRD_CHIRP 66
#define CAT_MEOW 67
#define BIRD_CHIRP_2 70
#define DIALOGUE_BEEP 87
#define RUMBLE 89


void initializeWaveTable(u16 waveTableIndex);
void func_800ACB04(u16 songIndex);
void func_800ACB8C(u16 songIndex);
void setSongWithDefaultSpeed(u16 songIndex);
u8 checkDefaultSongChannelOpen(u16 songIndex);
void setSongWithVolume(u16 songIndex, u32 arg1);
void func_800ACC1C(u16 songIndex);
void setAudio(u16 index);
u8 checkAllSfxInactive(void);

extern int musPriorityList[];
extern u8 *sfxList[];

#endif