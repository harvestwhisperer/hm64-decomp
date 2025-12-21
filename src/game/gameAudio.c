#include "common.h"

#include "system/audio.h"

#include "game/gameAudio.h"

// shared bss (system/audio.c)
extern Sfx gSfx[4];
extern SequenceInfo gAudioSequences[4];

// data
WaveTableInfo waveTableAddresses[1] = {
    { (u8*)0xE93080, (u8*)0xE99910, (u8*)0xE99910 },
};

Addresses sequenceRomAddresses[] = {
    { 0xFD8510, 0xFDAD00 },
    { 0xFDAD00, 0xFDD6C0 },
    { 0xFDD6C0, 0xFDFB50 },
    { 0xFDFB50, 0xFE12F0 },
    { 0xFE12F0, 0xFE27B0 },
    { 0xFE27B0, 0xFE4D20 },
    { 0xFE4D20, 0xFE5E50 },
    { 0xFE5E50, 0xFE6E60 },
    { 0xFE6E60, 0xFE7E00 },
    { 0xFE7E00, 0xFE9040 },
    { 0xFE9040, 0xFEAB70 },
    { 0xFEAB70, 0xFEB8F0 },
    { 0xFEB8F0, 0xFED5F0 },
    { 0xFED5F0, 0xFEEA40 },
    { 0xFEEA40, 0xFEF9F0 },
    { 0xFEF9F0, 0xFF1CD0 },
    { 0xFF1CD0, 0xFF3760 },
    { 0xFF3760, 0xFF4160 },
    { 0xFF4160, 0xFF4750 },
    { 0xFF4750, 0xFF6300 },
    { 0xFF6300, 0xFF6ED0 },
    { 0xFF6ED0, 0xFF7D80 },
    { 0xFF7D80, 0xFF9370 },
    { 0xFF9370, 0xFF95A0 },
    { 0xFF95A0, 0xFF9840 },
    { 0xFF9840, 0xFF99F0 },
    { 0xFF99F0, 0xFF9BC0 },
    { 0xFF9BC0, 0xFF9DC0 },
    { 0xFF9DC0, 0xFF9F40 },
    { 0xFF9F40, 0xFFA7B0 },
    { 0xFFA7B0, 0xFFA950 },
    { 0xFFA950, 0xFFAF80 },
    { 0xFFAF80, 0xFFB090 },
    { 0xFFB090, 0xFFB700 },
    { 0xFFB700, 0xFFC530 },
    { 0xFFC530, 0xFFC9F0 },
    { 0xFFC9F0, 0xFFCC40 },
    { 0xFFCC40, 0xFFCE10 },
    { 0xFFCE10, 0xFFD490 },
    { 0xFFD490, 0xFFD680 },
    { 0xFFD680, 0xFFD820 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
};

Addresses sfxRomAddresses[] = {
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFE620, 0xFFE720 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFF350, 0xFFF470 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFF470, 0xFFF580 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFEC30, 0xFFED40 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFE720, 0xFFE820 },
    { 0xFFE820, 0xFFE920 },
    { 0xFFE920, 0xFFEA30 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFEF50, 0xFFF070 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFF070, 0xFFF180 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFEE40, 0xFFEF50 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFED40, 0xFFEE40 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFD820, 0xFFD920 },
    { 0xFFD920, 0xFFDA20 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFDA30, 0xFFDB80 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFDBA0, 0xFFDCD0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFF580, 0xFFF700 },
    { 0xFFDCF0, 0xFFDEE0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0xFFDEF0, 0xFFE520 },
    { 0, 0 },
    { 0xFFF180, 0xFFF350 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
};

u8 audioTypeTable[] = {
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SONG_TYPE,SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SONG_TYPE,SFX_TYPE, SFX_TYPE, SONG_TYPE,SFX_TYPE, 
    SFX_TYPE, SONG_TYPE,SFX_TYPE, SFX_TYPE, SFX_TYPE, SONG_TYPE,
    SONG_TYPE, SONG_TYPE,SFX_TYPE, SFX_TYPE, SFX_TYPE, SONG_TYPE,
    SFX_TYPE, SFX_TYPE, SONG_TYPE,SFX_TYPE, SFX_TYPE, SONG_TYPE,
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SONG_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SONG_TYPE, SONG_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SONG_TYPE, SFX_TYPE, SFX_TYPE, SONG_TYPE,SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    0x03, SONG_TYPE,SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SONG_TYPE, SFX_TYPE, SONG_TYPE, 0x00, 0x00, 0x00, 0x00, 
    
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // this is probably separate unused array 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

s32 volumesTable[] = {
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};


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

void playSfx(u16 index) {
    
  if (index < TOTAL_SFX) {
      
    if (audioTypeTable[index] == SFX_TYPE) {
      
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