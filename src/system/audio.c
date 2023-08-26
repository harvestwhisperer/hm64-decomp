#include "common.h"
#include "system/audio.h"


void func_800266C0(Volume*, u16, u16, u16, u16); 
u8 func_800266F8(u16*);
void func_800267A4(Volume*, s16, s16);   

extern SongInfo gSongs[4];
extern Sfx gSfx[4];

// rodata
extern f64 minSfxFrequency;
extern f64 maxSfxFrequency;


//INCLUDE_ASM(const s32, "system/audio", initializeAudio);

void initializeAudio(musConfig *config) {

    u16 i;
    u16 j;

    for (i = 0; i < MAX_ACTIVE_SONGS; i++) {
        
        gSongs[i].flags = 0;
        gSongs[i].handle = 0;
        gSongs[i].numChannelsPlaying = 0;
        gSongs[i].unk_1C = 0;
        gSongs[i].unk_20 = 0;
        gSfx[i].volume = 0;
        gSongs[i].speed = 0;
        
        func_800266C0(&gSongs[i].volumes, 0, 0, 0, 0);
        
    }

    for (j = 0; j < MAX_ACTIVE_SFX; j++) {
        gSfx[j].flags = 0;
        gSfx[j].sfxIndex = 0;
        gSfx[j].handle = 0;
        gSfx[j].numSfxPlaying = 0;
        gSfx[j].frequency = 0;
        gSfx[j].pan = 0;
        gSfx[j].volume = 0;
    }
    
    nuAuStlInit(config);
    MusSetMasterVolume(MUSFLAG_SONGS, 0x7FFF);
    MusSetMasterVolume(MUSFLAG_EFFECTS, 0x7FFF);
    
}

//INCLUDE_ASM(const s32, "system/audio", mainLoopAudioHandler);

void mainLoopAudioHandler(void) {

    u16 i;
    u16 j;
    u8 current;

    for (i = 0; i < MAX_ACTIVE_SONGS; i++) {
        if (gSongs[i].flags & 1) {
            if (gSongs[i].flags & 2) {
                current = i;
                nuAuStlSeqPlayerDataSet(current, gSongs[i].currentSongRomAddrStart, gSongs[i].currentSongRomAddrEnd - gSongs[i].currentSongRomAddrStart);
                gSongs[i].handle = nuAuStlSeqPlayerPlay2(current);
                gSongs[i].flags &= 0xFFFD;
            }
            
            if (gSongs[i].flags & 4) {
                MusHandleStop(gSongs[i].handle, gSongs[i].speed);
                gSongs[i].flags &= 0xFFFB;
            }
            
            func_800266F8(&gSongs[i].volumes);
            MusHandleSetVolume(gSongs[i].handle, gSongs[i].volumes.mainVolume);
            
            gSongs[i].numChannelsPlaying = MusHandleAsk(gSongs[i].handle);
            
            if (!gSongs[i].numChannelsPlaying) {
                gSongs[i].flags = 0;
                MusHandleStop(gSongs[i].handle, 1);
            }
        }   
    }

    for (j = 0; j < MAX_ACTIVE_SFX; j++) {
        if (gSfx[j].flags & 1) {
            if (gSfx[j].flags & 2) {
                gSfx[j].handle = nuAuStlSndPlayerPlay(gSfx[j].sfxIndex);
                gSfx[j].flags &= 0xFFFD;
            }
            
            if (gSfx[j].flags & 4) {
                MusHandleStop(gSfx[j].handle, 0);
                gSfx[j].flags &= 0xFFFB;
            }
            
            MusHandleSetFreqOffset(gSfx[j].handle, gSfx[j].frequency);
            MusHandlePan(gSfx[j].handle, gSfx[j].pan);
            MusHandleSetVolume(gSfx[j].handle, gSfx[j].volume);
            
            gSfx[j].numSfxPlaying = MusHandleAsk(gSfx[j].handle);
            
            if (!(gSfx[j].numSfxPlaying)) {
                gSfx[j].flags = 0;
            }
        }
    }
}

//INCLUDE_ASM(const s32, "system/audio", setSongBank);

// wrapper for nuAuStlBankSet
void setSongBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart) {
    nuAuStlBankSet(pBankStart, pBankEnd - pBankStart, wBankStart);
}

//INCLUDE_ASM(const s32, "system/audio", setSong);

u8 setSong(u16 songIndex, u8 *songAddrStart, u8 *songAddrEnd) {
    
    u8 result = 0;
    
    if (songIndex < MAX_ACTIVE_SONGS) {
        if (!(gSongs[songIndex].flags & 1)) {
            // set volumes
            func_800266C0(&gSongs[songIndex].volumes, 0, 0, 0, 0);
            result = 1;
            gSongs[songIndex].unk_1C = 128;
            gSongs[songIndex].unk_20 = 128;
            gSongs[songIndex].currentSongRomAddrStart = songAddrStart;
            gSongs[songIndex].currentSongRomAddrEnd = songAddrEnd;
            gSongs[songIndex].flags = 3;
        }
    } 

    return result;
}

//INCLUDE_ASM(const s32, "system/audio", setSongSpeed);

u32 setSongSpeed(u16 index, u32 speed) {

    u32 result = 0;

    if (index < MAX_ACTIVE_SONGS) {
        if (gSongs[index].flags & 1) {
            result = 1;
            gSongs[index].speed = speed;
            gSongs[index].flags |= 4;
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/audio", stopSong);

u32 stopSong(u16 songIndex) {

    u32 result = 0;

    if (songIndex < MAX_ACTIVE_SONGS) {
    
        if (gSongs[songIndex].flags & 1) {
            MusHandleStop(gSongs[songIndex].handle, 0);
            result = 1;
            gSongs[songIndex].flags = 0;
        }

    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/audio", setSongVolumes);

u32 setSongVolumes(u16 index, s32 maxVolume, s16 arg2) {
    
    u32 result = 0;
    
    if (index < MAX_ACTIVE_SONGS) {
        
        if (gSongs[index].flags & 1) {
            
            // unpack to short?
            maxVolume &= (~maxVolume >> 0x1F);
            
            if (maxVolume > 256) {
                maxVolume = 256;
            }
            
            gSongs[index].volumes.maxVolume = maxVolume;
            
            // set volume substruct
            func_800267A4(&gSongs[index].volumes, arg2, maxVolume);
            
            result = 1;
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/audio", func_8003D4E4);

u32 func_8003D4E4(u16 songIndex, s32 arg1) {
    
    u32 result = 0;

    if (songIndex < MAX_ACTIVE_SONGS) {
        
        if (gSongs[songIndex].flags & 1) {
        
            gSongs[songIndex].unk_1C = arg1;
        
            if (arg1 < 0) {
                 gSongs[songIndex].unk_1C = 0;
            }
        
            result = 1;
        
            if (gSongs[songIndex].unk_1C >= 257) {
                 gSongs[songIndex].unk_1C = 256;
            }
        }
    }

    return result;

}

//INCLUDE_ASM(const s32, "system/audio", func_8003D570);

u32 func_8003D570(u16 songIndex, s32 arg1) {
    
    u32 result = 0;

    if (songIndex < MAX_ACTIVE_SONGS) {
        
        if (gSongs[songIndex].flags & 1) {
            
            gSongs[songIndex].unk_20 = arg1;
            
            if (arg1 < 0) {
                 gSongs[songIndex].unk_20 = 0;
            }

            result = 1;
            
            if (gSongs[songIndex].unk_20 >= 257) {
                 gSongs[songIndex].unk_20 = 256;
            }
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/audio", stopMusic);

void stopMusic(int speed) {
    MusStop(MUSFLAG_SONGS, speed);
}

//INCLUDE_ASM(const s32, "system/audio", setSfx);

u32 setSfx(u32 sfxIndex) {

    u16 i = 0;
    u32 result = 0;
    u32 current;

    while (TRUE) {
        
        current = i;

        if (!(gSfx[current].flags & 1)) {
            
            i = MAX_ACTIVE_SFX;
            gSfx[current].sfxIndex = sfxIndex;
            gSfx[current].frequency = 0;
            gSfx[current].pan = 128;
            gSfx[current].flags = 3;
            result = 1;

        } else {
            i++;
        }

        if (i >= MAX_ACTIVE_SFX) break;
    }
    
    return result;

}

//INCLUDE_ASM(const s32, "system/audio", func_8003D6A8);

u32 func_8003D6A8(u32 sfxIndex) {
    
    u16 i = 0;
    u32 result = 0;
    
    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        if (gSfx[i].flags & 1) {
            if (gSfx[i].sfxIndex == sfxIndex) {
                gSfx[i].flags |= 4;
                result = 1;
            }
        }
    }
    
    return result;
}

//INCLUDE_ASM(const s32, "system/audio", setSfxVolume);

u32 setSfxVolume(u32 sfxIndex, s32 volume) {

    u32 result;
    u16 i = 0;
    
    result = 0;
    
    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        
        if (gSfx[i].flags & 1 && gSfx[i].sfxIndex == sfxIndex) {

            gSfx[i].volume = volume;
            
            if (volume < 0) {
                gSfx[i].volume = 0;
            }
            
            result = 1;
            
            if (gSfx[i].volume >= 257) {
                gSfx[i].volume = 256;
            }
        }
    }

    return result;    
}

INCLUDE_ASM(const s32, "system/audio", setSfxFrequency);

/*
u32 setSfxFrequency(u32 sfxIndex, s32 frequency) {

    static const double minSfxFrequency = -6;
    static const double maxSfxFrequency = 6;

//     dlabel minSfxFrequency
//  FA200 8011EE00 C018000000000000 .double -6

// dlabel maxSfxFrequency
//  FA208 8011EE08 4018000000000000 .double 6

    u16 i = 0;
    u32 result = 0;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {

        if (gSfx[i].flags & 1) {
            if (gSfx[i].sfxIndex == sfxIndex) {
                gSfx[i].frequency = frequency;
                if (frequency < minSfxFrequency) {
                    gSfx[i].frequency = -6;
                }
                result = 1;
                if (gSfx[i].frequency > maxSfxFrequency) {
                    gSfx[i].frequency = 6;
                }
            }
        }
        
    }
    
    return result;
}
*/

//INCLUDE_ASM(const s32, "system/audio", setSfxPan);

u32 setSfxPan(u32 sfxIndex, s32 arg1) {

    u16 i = 0;
    u32 result = 0;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        if (gSfx[i].flags & 1) {
            if (gSfx[i].sfxIndex == sfxIndex) {
                gSfx[i].pan = arg1;
                if (arg1 < 0) {
                    gSfx[i].pan = 0;
                }
                result = 1;
                if (gSfx[i].pan >= 257) {
                    gSfx[i].pan = 256;
                }
            }
        }
    }

    return result;
}

//INCLUDE_ASM(const s32, "system/audio", stopSoundEffect);

void stopSfx(void) {
    MusStop(MUSFLAG_EFFECTS, 0);
}