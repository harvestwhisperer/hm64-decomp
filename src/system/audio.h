#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "common.h"

#define SONG 1
#define SFX 0xFF
#define MAX_ACTIVE_SONGS 4
#define MAX_ACTIVE_SFX 4
#define TOTAL_SONGS 64
#define TOTAL_SFX 128

u8 setSong(u16 songIndex, u8 *songAddrStart, u8 *songAddrEnd);
u32 setSongSpeed(u16 index, u32 speed);
u32 stopSong(u16 songIndex);
u32 setSongVolumes(u16 index, s32 maxVolume, s16 arg2);
void setSongBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart);
u32 setSfx(u32 sfxIndex);
u32 func_8003D6A8(u32 sfxIndex);                     
u32 setSfxVolume(u32 sfxIndex, s32 volume);  
u32 setSfxFrequency(u32 sfxIndex, s32 frequency);                   
u32 setSfxPan(u32 sfxIndex, s32 arg1);
void stopSfx(void);

typedef struct {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
    s32 mainVolume;
    s32 maxVolume;
} Volume;

typedef struct {
    u8 *currentSongRomAddrStart;
    u8 *currentSongRomAddrEnd;
    Volume volumes;
    musHandle handle;
    s32 unk_1C;
    s32 unk_20;
    u32 speed;
    u8 numChannelsPlaying;
    u16 flags;
} SongInfo;

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


#endif