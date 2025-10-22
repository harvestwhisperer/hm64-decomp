#include "common.h"

#include "system/audio.h"

#include "game/gameAudio.h"

// rodata
#define MIN_SFX_FREQUENCY (double)-6
#define MAX_SFX_FREQUENCY (double)6

//INCLUDE_ASM("asm/nonmatchings/system/audio", initializeAudio);

void initializeAudio(musConfig *config) {

    u16 i;
    u16 j;

    for (i = 0; i < MAX_ACTIVE_SONGS; i++) {
        
        gSongs[i].flags = 0;
        gSongs[i].handle = 0;
        gSongs[i].numChannelsPlaying = 0;
        gSongs[i].unused1 = 0;
        gSongs[i].unused2 = 0;
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

//INCLUDE_ASM("asm/nonmatchings/system/audio", updateAudio);

void updateAudio(void) {

    u16 i, j;
    u8 current;

    for (i = 0; i < MAX_ACTIVE_SONGS; i++) {

        if (gSongs[i].flags & AUDIO_ACTIVE) {

            if (gSongs[i].flags & AUDIO_START) {
                current = i;
                nuAuStlSeqPlayerDataSet(current, gSongs[i].currentSongRomAddrStart, gSongs[i].currentSongRomAddrEnd - gSongs[i].currentSongRomAddrStart);
                gSongs[i].handle = nuAuStlSeqPlayerPlay2(current);
                gSongs[i].flags &= ~AUDIO_START;
            }
            
            if (gSongs[i].flags & AUDIO_STOP) {
                MusHandleStop(gSongs[i].handle, gSongs[i].speed);
                gSongs[i].flags &= ~AUDIO_STOP;
            }
            
            interpolateToTarget(&gSongs[i].volumes);
            MusHandleSetVolume(gSongs[i].handle, gSongs[i].volumes.currentValue);
            
            gSongs[i].numChannelsPlaying = MusHandleAsk(gSongs[i].handle);
            
            if (!gSongs[i].numChannelsPlaying) {
                gSongs[i].flags = 0;
                MusHandleStop(gSongs[i].handle, 1);
            }

        }   
        
    }

    for (j = 0; j < MAX_ACTIVE_SFX; j++) {

        if (gSfx[j].flags & AUDIO_ACTIVE) {

            if (gSfx[j].flags & 2) {
                gSfx[j].handle = nuAuStlSndPlayerPlay(gSfx[j].sfxIndex);
                gSfx[j].flags &= ~2;
            }
            
            if (gSfx[j].flags & AUDIO_STOP) {
                MusHandleStop(gSfx[j].handle, 0);
                gSfx[j].flags &= ~AUDIO_STOP;
            }
            
            MusHandleSetFreqOffset(gSfx[j].handle, gSfx[j].frequency);
            MusHandleSetPan(gSfx[j].handle, gSfx[j].pan);
            MusHandleSetVolume(gSfx[j].handle, gSfx[j].volume);
            
            gSfx[j].numSfxPlaying = MusHandleAsk(gSfx[j].handle);
            
            if (!(gSfx[j].numSfxPlaying)) {
                gSfx[j].flags = 0;
            }

        }
    }
}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSongBank);

void setSongBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart) {
    nuAuStlBankSet(pBankStart, pBankEnd - pBankStart, wBankStart);
}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSong);

bool setSong(u16 songIndex, u8 *songAddrStart, u8 *songAddrEnd) {
    
    bool result = FALSE;
    
    if (songIndex < MAX_ACTIVE_SONGS) {

        if (!(gSongs[songIndex].flags & AUDIO_ACTIVE)) {

            func_800266C0(&gSongs[songIndex].volumes, 0, 0, 0, 0);

            gSongs[songIndex].unused1 = 128;
            gSongs[songIndex].unused2 = 128;
            gSongs[songIndex].currentSongRomAddrStart = songAddrStart;
            gSongs[songIndex].currentSongRomAddrEnd = songAddrEnd;
            gSongs[songIndex].flags = (AUDIO_ACTIVE | AUDIO_START);

            result = TRUE;
            
        }
    } 

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", stopSongWithFadeOut);

bool stopSongWithFadeOut(u16 index, u32 speed) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SONGS) {
        
        if (gSongs[index].flags & AUDIO_ACTIVE) {
            
            gSongs[index].speed = speed;
            gSongs[index].flags |= AUDIO_STOP;
            
            result = TRUE;

        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", stopSong);

bool stopSong(u16 songIndex) {

    bool result = FALSE;

    if (songIndex < MAX_ACTIVE_SONGS) {
    
        if (gSongs[songIndex].flags & AUDIO_ACTIVE) {
            
            MusHandleStop(gSongs[songIndex].handle, 0);
            gSongs[songIndex].flags = 0;
            
            result = TRUE;

        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSongVolumes);

bool setSongVolumes(u16 index, s32 targetVolume, s16 fadeInRate) {
    
    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SONGS) {
         
        if (gSongs[index].flags & AUDIO_ACTIVE) {

            targetVolume &= (~targetVolume >> 0x1F);
            
            if (targetVolume > MAX_VOLUME) {
                targetVolume = MAX_VOLUME;
            }
            
            gSongs[index].volumes.targetValue = targetVolume;
            
            initializeInterpolator(&gSongs[index].volumes, fadeInRate, targetVolume);
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", func_8003D4E4);

// unused
bool func_8003D4E4(u16 songIndex, s32 arg1) {
    
    bool result = FALSE;

    if (songIndex < MAX_ACTIVE_SONGS) {
        
        if (gSongs[songIndex].flags & AUDIO_ACTIVE) {
        
            gSongs[songIndex].unused1 = arg1;
        
            if (arg1 < 0) {
                 gSongs[songIndex].unused1 = 0;
            }
        
            result = TRUE;
        
            if (gSongs[songIndex].unused1 >= 257) {
                 gSongs[songIndex].unused1 = 256;
            }
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", func_8003D570);

// unused
bool func_8003D570(u16 songIndex, s32 arg1) {
    
    bool result = FALSE;

    if (songIndex < MAX_ACTIVE_SONGS) {
        
        if (gSongs[songIndex].flags & AUDIO_ACTIVE) {
            
            gSongs[songIndex].unused2 = arg1;
            
            if (arg1 < 0) {
                 gSongs[songIndex].unused2 = 0;
            }
            
            if (gSongs[songIndex].unused2 >= 257) {
                 gSongs[songIndex].unused2 = 256;
            }

            result = TRUE;
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", stopMusic);

void stopMusic(int speed) {
    MusStop(MUSFLAG_SONGS, speed);
}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSfx);

bool setSfx(u32 sfxIndex) {

    u16 i = 0;

    bool result = FALSE;

    u32 current;

    while (TRUE) {
        
        current = i;

        if (!(gSfx[current].flags & AUDIO_ACTIVE)) {
            
            i = MAX_ACTIVE_SFX;
            
            gSfx[current].sfxIndex = sfxIndex;
            gSfx[current].frequency = 0;
            gSfx[current].pan = 128;
            gSfx[current].flags = (AUDIO_ACTIVE | AUDIO_START);

            result = TRUE;

        } else {
            i++;
        }

        if (i >= MAX_ACTIVE_SFX) break;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", findAndStopSfx);

// unused
bool findAndStopSfx(u32 sfxIndex) {
    
    u16 i = 0;
    bool result = FALSE;
    
    for (i = 0; i < MAX_ACTIVE_SFX; i++) {

        if (gSfx[i].flags & AUDIO_ACTIVE) {

            if (gSfx[i].sfxIndex == sfxIndex) {

                gSfx[i].flags |= AUDIO_STOP;

                result = TRUE;

            }
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSfxVolume);

bool setSfxVolume(u32 sfxIndex, s32 volume) {

    bool result;
    u16 i = 0;
    
    result = FALSE;
    
    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        
        if (gSfx[i].flags & AUDIO_ACTIVE && gSfx[i].sfxIndex == sfxIndex) {

            gSfx[i].volume = volume;
            
            if (volume < 0) {
                gSfx[i].volume = 0;
            }
            
            if (gSfx[i].volume > MAX_VOLUME) {
                gSfx[i].volume = MAX_VOLUME;
            }
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSfxFrequency);

bool setSfxFrequency(u32 sfxIndex, s32 frequency) {

    u16 i = 0;
    bool result = FALSE;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {

        if (gSfx[i].flags & AUDIO_ACTIVE) {

            if (gSfx[i].sfxIndex == sfxIndex) {

                gSfx[i].frequency = frequency;

                if (frequency < MIN_SFX_FREQUENCY) {
                    gSfx[i].frequency = -6;
                }


                if (gSfx[i].frequency > MAX_SFX_FREQUENCY) {
                    gSfx[i].frequency = 6;
                }

                result = TRUE;

            }
        }
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setSfxPan);

bool setSfxPan(u32 sfxIndex, s32 arg1) {

    u16 i = 0;
    bool result = FALSE;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {

        if (gSfx[i].flags & AUDIO_ACTIVE) {

            if (gSfx[i].sfxIndex == sfxIndex) {

                gSfx[i].pan = arg1;
                
                if (arg1 < 0) {
                    gSfx[i].pan = 0;
                }
                
                result = TRUE;
                
                if (gSfx[i].pan >= 257) {
                    gSfx[i].pan = 256;
                }
            }
        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", stopSoundEffect);

void stopSfx(void) {
    MusStop(MUSFLAG_EFFECTS, 0);
}