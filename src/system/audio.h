#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "common.h"
#include "system/volume.h"

#define SONG 1
#define SFX 0xFF
#define MAX_ACTIVE_SONGS 4
#define MAX_ACTIVE_SFX 4
#define TOTAL_SONGS 64
#define TOTAL_SFX 128

#define CLOSE 1
#define MOVE_CURSOR 2
#define STORE_ITEM 4
#define SELECT 7
#define WHISTLE 10
#define SICKLE_SFX 11
#define AX_SFX 17
#define SEEDS 29
#define MILKER_SFX 30
#define ROOSTER 63
#define CHICKEN_CLUCK 64
#define BIRD_CHIRP 66
#define CAT_MEOW 67
#define BIRD_CHIRP_2 70
#define DIALOGUE_BEEP 87
#define RUMBLE 89

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

extern void initializeAudio(musConfig*);      
extern void mainLoopAudioHandler();
extern bool setSong(u16 songIndex, u8 *songAddrStart, u8 *songAddrEnd);
extern bool setSongSpeed(u16 index, u32 speed);
extern bool stopSong(u16 songIndex);
extern bool setSongVolumes(u16 index, s32 maxVolume, s16 arg2);
extern void setSongBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart);
extern bool setSfx(u32 sfxIndex);
extern bool func_8003D6A8(u32 sfxIndex);                     
extern bool setSfxVolume(u32 sfxIndex, s32 volume);  
extern bool setSfxFrequency(u32 sfxIndex, s32 frequency);                   
extern bool setSfxPan(u32 sfxIndex, s32 arg1);
extern void stopSfx(void);

extern u16 gCurrentSongIndex;
extern s32 gSongVolume;

// might be gameAudio.c
extern Sfx gSfx[4];
extern SongInfo gSongs[4];

#endif