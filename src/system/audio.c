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


//INCLUDE_ASM("asm/nonmatchings/system/audio", initializeAudio);

void initializeAudio(musConfig *config) {

    u16 i;
    u16 j;

    for (i = 0; i < MAX_ACTIVE_SEQUENCES; i++) {
        
        gAudioSequences[i].flags = 0;
        gAudioSequences[i].handle = 0;
        gAudioSequences[i].numChannelsPlaying = 0;
        gAudioSequences[i].unused1 = 0;
        gAudioSequences[i].unused2 = 0;
        gSfx[i].volume = 0;
        gAudioSequences[i].speed = 0;
        
        setupInterpolator(&gAudioSequences[i].volumes, 0, 0, 0, 0);
        
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

    for (i = 0; i < MAX_ACTIVE_SEQUENCES; i++) {

        if (gAudioSequences[i].flags & AUDIO_ACTIVE) {

            if (gAudioSequences[i].flags & AUDIO_START) {
                current = i;
                nuAuStlSeqPlayerDataSet(current, gAudioSequences[i].currentAudioSequenceRomAddrStart, gAudioSequences[i].currentAudioSequenceRomAddrEnd - gAudioSequences[i].currentAudioSequenceRomAddrStart);
                gAudioSequences[i].handle = nuAuStlSeqPlayerPlay2(current);
                gAudioSequences[i].flags &= ~AUDIO_START;
            }
            
            if (gAudioSequences[i].flags & AUDIO_STOP) {
                MusHandleStop(gAudioSequences[i].handle, gAudioSequences[i].speed);
                gAudioSequences[i].flags &= ~AUDIO_STOP;
            }
            
            interpolateToTarget(&gAudioSequences[i].volumes);
            MusHandleSetVolume(gAudioSequences[i].handle, gAudioSequences[i].volumes.currentValue);
            
            gAudioSequences[i].numChannelsPlaying = MusHandleAsk(gAudioSequences[i].handle);
            
            if (!gAudioSequences[i].numChannelsPlaying) {
                gAudioSequences[i].flags = 0;
                MusHandleStop(gAudioSequences[i].handle, 1);
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

//INCLUDE_ASM("asm/nonmatchings/system/audio", setAudioSequenceBank);

void setAudioSequenceBank(u8 *pBankStart, u8 *pBankEnd, u8 *wBankStart) {
    nuAuStlBankSet(pBankStart, pBankEnd - pBankStart, wBankStart);
}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setAudioSequence);

bool setAudioSequence(u16 sequenceIndex, u8 *sequenceAddrStart, u8 *sequenceAddrEnd) {
    
    bool result = FALSE;
    
    if (sequenceIndex < MAX_ACTIVE_SEQUENCES) {

        if (!(gAudioSequences[sequenceIndex].flags & AUDIO_ACTIVE)) {

            setupInterpolator(&gAudioSequences[sequenceIndex].volumes, 0, 0, 0, 0);

            gAudioSequences[sequenceIndex].unused1 = 128;
            gAudioSequences[sequenceIndex].unused2 = 128;
            gAudioSequences[sequenceIndex].currentAudioSequenceRomAddrStart = sequenceAddrStart;
            gAudioSequences[sequenceIndex].currentAudioSequenceRomAddrEnd = sequenceAddrEnd;
            gAudioSequences[sequenceIndex].flags = (AUDIO_ACTIVE | AUDIO_START);

            result = TRUE;
            
        }
    } 

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", stopAudioSequenceWithFadeOut);

bool stopAudioSequenceWithFadeOut(u16 index, u32 speed) {

    bool result = FALSE;

    if (index < MAX_ACTIVE_SEQUENCES) {
        
        if (gAudioSequences[index].flags & AUDIO_ACTIVE) {
            
            gAudioSequences[index].speed = speed;
            gAudioSequences[index].flags |= AUDIO_STOP;
            
            result = TRUE;

        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", stopAudioSequence);

bool stopAudioSequence(u16 sequenceIndex) {

    bool result = FALSE;

    if (sequenceIndex < MAX_ACTIVE_SEQUENCES) {
    
        if (gAudioSequences[sequenceIndex].flags & AUDIO_ACTIVE) {
            
            MusHandleStop(gAudioSequences[sequenceIndex].handle, 0);
            gAudioSequences[sequenceIndex].flags = 0;
            
            result = TRUE;

        }

    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", setAudioSequenceVolumes);

bool setAudioSequenceVolumes(u16 index, s32 targetVolume, s16 fadeInRate) {
    
    bool result = FALSE;
    
    if (index < MAX_ACTIVE_SEQUENCES) {
         
        if (gAudioSequences[index].flags & AUDIO_ACTIVE) {

            targetVolume &= (~targetVolume >> 0x1F);
            
            if (targetVolume > MAX_VOLUME) {
                targetVolume = MAX_VOLUME;
            }
            
            gAudioSequences[index].volumes.targetValue = targetVolume;
            
            initializeInterpolator(&gAudioSequences[index].volumes, fadeInRate, targetVolume);
            
            result = TRUE;

        }
    }

    return result;

}

//INCLUDE_ASM("asm/nonmatchings/system/audio", func_8003D4E4);

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

//INCLUDE_ASM("asm/nonmatchings/system/audio", func_8003D570);

// unused
// s

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