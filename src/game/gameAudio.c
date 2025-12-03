#include "common.h"

#include "system/audio.h"

#include "game/gameAudio.h"

// bss or system/audio.c
extern Sfx gSfx[4];
extern SequenceInfo gAudioSequences[4];

// data
extern WaveTableInfo waveTableAddresses[1];
extern Addresses sequenceRomAddresses[0x40];
extern Addresses sfxRomAddresses[0x80];
extern s32 volumesTable[];
// 0x60 set elements, but takes up 0x180 bytes in rodata
extern u8 audioTypeTable[0x60];


//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", initializeWaveTable);

void initializeWaveTable(u16 waveTableIndex) {
    setAudioSequenceBank(waveTableAddresses[waveTableIndex].pbk_addr, waveTableAddresses[waveTableIndex].pbk_end, waveTableAddresses[waveTableIndex].wbk_addr);
}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", setCurrentAudioSequence);

void setCurrentAudioSequence(u16 sequenceIndex) {

    if (sequenceIndex < TOTAL_SEQUENCES) {
        setAudioSequence(0, (u8*)sequenceRomAddresses[sequenceIndex].romAddrStart, (u8*)sequenceRomAddresses[sequenceIndex].romAddrEnd);
    }
    
    setAudioSequenceVolumes(0, 0, 0);

}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", stopAudioSequenceWithDefaultFadeOut);

void stopAudioSequenceWithDefaultFadeOut(u16 sequenceIndex) {
    if (sequenceIndex < TOTAL_SEQUENCES) {
        stopAudioSequenceWithFadeOut(0, 32);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", stopCurrentAudioSequence);

void stopCurrentAudioSequence(u16 sequenceIndex) {
    if (sequenceIndex  < TOTAL_SEQUENCES) {
        stopAudioSequence(0);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", checkDefaultSequenceChannelOpen);

// check if first sequence slot is open/not set
bool checkDefaultSequenceChannelOpen(u16 sequenceIndex) {
    
    u8 result = FALSE;
    
    if (sequenceIndex < TOTAL_SEQUENCES) {
        result = gAudioSequences[0].flags == 0;
    }

    if (sequenceIndex == 0xFF) {
        result = TRUE;
    }
    
    return result;

}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", setAudioSequenceVolume);

void setAudioSequenceVolume(u16 sequenceIndex, u32 targetVolume) {
    if (sequenceIndex < TOTAL_SEQUENCES) {
        setAudioSequenceVolumes(0, targetVolume, 32);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", func_800ACC1C);

void func_800ACC1C(u16 sequenceIndex) {
    if (sequenceIndex < TOTAL_SEQUENCES) {
        setAudioSequenceVolumes(0, 0, 32);
    }
}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", setSfx);

// only used for sfx
void playSfx(u16 index) {
    
  if (index < TOTAL_SFX) {
      
    if (audioTypeTable[index] == SFX) {
      
      setSfx(index + 1);
      setSfxVolume(index + 1, volumesTable[index]);

    } else {

        setAudioSequence(audioTypeTable[index], (u8*)sfxRomAddresses[index].romAddrStart, (u8*)sfxRomAddresses[index].romAddrEnd);
        setAudioSequenceVolumes(audioTypeTable[index], volumesTable[index], volumesTable[index]);
    
    }
  }
  
}

//INCLUDE_ASM("asm/nonmatchings/game/gameAudio", checkAllSfxInactive);

u8 checkAllSfxInactive(void) {

    u8 i;
    u8 count;

    count = 0;

    for (i = 0; i < MAX_ACTIVE_SFX; i++) {
        if (!gSfx[i].flags) {
            count++;
        }
    }
    
    return count == 4;

}