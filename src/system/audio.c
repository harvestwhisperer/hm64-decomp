#include "common.h"

#include "nualstl_n.h"

#include "system/audio.h"

#include "game/gameAudio.h"

extern void nuAuStlSeqPlayerDataSet(u8 player_no, u8* seq_addr, u32 seq_size);
extern musHandle nuAuStlSeqPlayerPlay2(u8 player_no);

// rodata
#define MIN_SFX_FREQUENCY (double)-6
#define MAX_SFX_FREQUENCY (double)6

// bss
u16 gCurrentAudioSequenceIndex;
s32 gAudioSequenceVolume;

// shared bss (gameAudio.c)
Sfx gSfx[4];
SequenceInfo gAudioSequences[4];

void initializeAudio(musConfig *config) {

    u16 i;
    u16 j;

    for (i = 0; i < MAX_ACTIVE_SEQUENCES; i++) {
        SequenceInfo *seq = &gAudioSequences[i];
        Sfx *sfx = &gSfx[i];

        seq->flags = 0;
        seq->handle = 0;
        seq->numChannelsPlaying = 0;
        seq->unused1 = 0;
        seq->unused2 = 0;
        sfx->volume = 0;
        seq->speed = 0;

        setupInterpolator(&seq->volumes, 0, 0, 0, 0);

    }

    for (j = 0; j < MAX_ACTIVE_SFX; j++) {
        Sfx *sfx = &gSfx[j];
        sfx->flags = 0;
        sfx->sfxIndex = 0;
        sfx->handle = 0;
        sfx->numSfxPlaying = 0;
        sfx->frequency = 0;
        sfx->pan = 0;
        sfx->volume = 0;
    }
    
    nuAuStlInit(config);
    MusSetMasterVolume(MUSFLAG_SONGS, 0x7FFF);
    MusSetMasterVolume(MUSFLAG_EFFECTS, 0x7FFF);
    
}

void updateAudio(void) {

    u16 i, j;
    u8 current;

    for (i = 0; i < MAX_ACTIVE_SEQUENCES; i++) {
        SequenceInfo *seq = &gAudioSequences[i];

        if (seq->flags & AUDIO_ACTIVE) {

            if (seq->flags & AUDIO_START) {
                current = i;
                nuAuStlSeqPlayerDataSet(current, seq->currentAudioSequenceRomAddrStart, seq->currentAudioSequenceRomAddrEnd - seq->currentAudioSequenceRomAddrStart);
                seq->handle = nuAuStlSeqPlayerPlay2(current);
                seq->flags &= ~AUDIO_START;
            }

            if (seq->flags & AUDIO_STOP) {
                MusHandleStop(seq->handle, seq->speed);
                seq->flags &= ~AUDIO_STOP;
            }

            interpolateToTarget(&seq->volumes);
            MusHandleSetVolume(seq->handle, seq->volumes.currentValue);

            seq->numChannelsPlaying = MusHandleAsk(seq->handle);

            if (!seq->numChannelsPlaying) {
                seq->flags = 0;
                MusHandleStop(seq->handle, 1);
            }

        }

    }

    for (j = 0; j < MAX_ACTIVE_SFX; j++) {
        Sfx *sfx = &gSfx[j];

        if (sfx->flags & AUDIO_ACTIVE) {

            if (sfx->flags & 2) {
                sfx->handle = nuAuStlSndPlayerPlay(sfx->sfxIndex);
                sfx->flags &= ~2;
            }

            if (sfx->flags & AUDIO_STOP) {
                MusHandleStop(sfx->handle, 0);
                sfx->flags &= ~AUDIO_STOP;
            }

            MusHandleSetFreqOffset(sfx->handle, sfx->frequency);
            MusHandleSetPan(sfx->handle, sfx->pan);
            MusHandleSetVolume(sfx->handle, sfx->volume);

            sfx->numSfxPlaying = MusHandleAsk(sfx->handle);

            if (!(sfx->numSfxPlaying)) {
                sfx->flags = 0;
            }

        }
    }
}

void setAudioSequenceBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart) {
    nuAuStlBankSet(pBankStart, pBankEnd - pBankStart, wBankStart);
}

bool setAudioSequence(u16 sequenceIndex, u8 *sequenceAddrStart, u8 *sequenceAddrEnd) {

    SequenceInfo *seq = &gAudioSequences[sequenceIndex];
    
    bool result = FALSE;
    
    if (sequenceIndex < MAX_ACTIVE_SEQUENCES) {

        if (!(seq->flags & AUDIO_ACTIVE)) {

            setupInterpolator(&seq->volumes, 0, 0, 0, 0);

            seq->unused1 = 128;
            seq->unused2 = 128;
            seq->currentAudioSequenceRomAddrStart = sequenceAddrStart;
            seq->currentAudioSequenceRomAddrEnd = sequenceAddrEnd;
            seq->flags = (AUDIO_ACTIVE | AUDIO_START);

            result = TRUE;
            
        }
    } 

    return result;

}

bool stopAudioSequenceWithFadeOut(u16 index, u32 speed) {

    SequenceInfo *seq = &gAudioSequences[index];

    bool result = FALSE;

    if (index < MAX_ACTIVE_SEQUENCES) {
        
        if (seq->flags & AUDIO_ACTIVE) {
            
            seq->speed = speed;
            seq->flags |= AUDIO_STOP;
            
            result = TRUE;

        }

    }
    
    return result;

}

bool stopAudioSequence(u16 sequenceIndex) {

    SequenceInfo *seq = &gAudioSequences[sequenceIndex];

    bool result = FALSE;

    if (sequenceIndex < MAX_ACTIVE_SEQUENCES) {
    
        if (seq->flags & AUDIO_ACTIVE) {
            
            MusHandleStop(seq->handle, 0);
            seq->flags = 0;
            
            result = TRUE;

        }

    }
    
    return result;

}

bool setAudioSequenceVolumes(u16 index, s32 targetVolume, s16 fadeInRate) {

    SequenceInfo *seq = &gAudioSequences[index];
    
    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SEQUENCES) {
         
        if (seq->flags & AUDIO_ACTIVE) {

            targetVolume &= (~targetVolume >> 0x1F);
            
            if (targetVolume > MAX_VOLUME) {
                targetVolume = MAX_VOLUME;
            }
            
            seq->volumes.targetValue = targetVolume;
            
            initializeInterpolator(&seq->volumes, fadeInRate, targetVolume);
            
            result = TRUE;

        }
    }

    return result;

}

// unused
// bool func_8003D4E4(u16 sequenceIndex, s32 arg1) {
    
//     bool result = FALSE;

//     if (sequenceIndex < MAX_ACTIVE_SEQUENCES) {
        
//         if (gAudioSequences[sequenceIndex].flags & AUDIO_ACTIVE) {
        
//             gAudioSequences[sequenceIndex].unused1 = arg1;
        
//             if (arg1 < 0) {
//                  gAudioSequences[sequenceIndex].unused1 = 0;
//             }
        
//             result = TRUE;
        
//             if (gAudioSequences[sequenceIndex].unused1 >= 257) {
//                  gAudioSequences[sequenceIndex].unused1 = 256;
//             }
//         }
//     }

//     return result;

// }

// unused
// bool func_8003D570(u16 sequenceIndex, s32 arg1) {
    
//     bool result = FALSE;

//     if (sequenceIndex < MAX_ACTIVE_SEQUENCES) {
        
//         if (gAudioSequences[sequenceIndex].flags & AUDIO_ACTIVE) {
            
//             gAudioSequences[sequenceIndex].unused2 = arg1;
            
//             if (arg1 < 0) {
//                  gAudioSequences[sequenceIndex].unused2 = 0;
//             }
            
//             if (gAudioSequences[sequenceIndex].unused2 >= 257) {
//                  gAudioSequences[sequenceIndex].unused2 = 256;
//             }

//             result = TRUE;
//         }
//     }

//     return result;

// }

void stopMusic(int speed) {
    MusStop(MUSFLAG_SONGS, speed);
}

bool setSfx(u32 sfxIndex) {

    u16 i = 0;

    bool result = FALSE;

    u32 current;

    while (TRUE) {
        Sfx *sfx;

        current = i;
        sfx = &gSfx[current];

        if (!(sfx->flags & AUDIO_ACTIVE)) {

            i = MAX_ACTIVE_SFX;

            sfx->sfxIndex = sfxIndex;
            sfx->frequency = 0;
            sfx->pan = 128;
            sfx->flags = (AUDIO_ACTIVE | AUDIO_START);

            result = TRUE;

        } else {
            i++;
        }

        if (i >= MAX_ACTIVE_SFX) break;
    }
    
    return result;

}

// unused
// bool findAndStopSfx(u32 sfxIndex) {
    
//     u16 i = 0;
//     bool result = FALSE;
    
//     for (i = 0; i < MAX_ACTIVE_SFX; i++) {

//         if (gSfx[i].flags & AUDIO_ACTIVE) {

//             if (gSfx[i].sfxIndex == sfxIndex) {

//                 gSfx[i].flags |= AUDIO_STOP;

//                 result = TRUE;

//             }
//         }
//     }
    
//     return result;

// }

bool setSfxVolume(u32 sfxIndex, s32 volume) {

    bool result;
    u16 i = 0;
    
    result = FALSE;
    
    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        Sfx *sfx = &gSfx[i];

        if (sfx->flags & AUDIO_ACTIVE && sfx->sfxIndex == sfxIndex) {

            sfx->volume = volume;

            if (volume < 0) {
                sfx->volume = 0;
            }

            if (sfx->volume > MAX_VOLUME) {
                sfx->volume = MAX_VOLUME;
            }

            result = TRUE;

        }
    }

    return result;

}

bool setSfxFrequency(u32 sfxIndex, s32 frequency) {

    u16 i = 0;
    bool result = FALSE;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        Sfx *sfx = &gSfx[i];

        if (sfx->flags & AUDIO_ACTIVE) {

            if (sfx->sfxIndex == sfxIndex) {

                sfx->frequency = frequency;

                if (frequency < MIN_SFX_FREQUENCY) {
                    sfx->frequency = -6;
                }

                if (sfx->frequency > MAX_SFX_FREQUENCY) {
                    sfx->frequency = 6;
                }

                result = TRUE;

            }
        }
    }
    
    return result;

}

bool setSfxPan(u32 sfxIndex, s32 arg1) {

    u16 i = 0;
    bool result = FALSE;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        Sfx *sfx = &gSfx[i];

        if (sfx->flags & AUDIO_ACTIVE) {

            if (sfx->sfxIndex == sfxIndex) {

                sfx->pan = arg1;

                if (arg1 < 0) {
                    sfx->pan = 0;
                }

                result = TRUE;

                if (sfx->pan >= 257) {
                    sfx->pan = 256;
                }
            }
        }
    }

    return result;

}

void stopSfx(void) {
    MusStop(MUSFLAG_EFFECTS, 0);
}