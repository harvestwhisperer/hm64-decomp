#include "common.h"

#include "system/audio.h"

#include "game/gameAudio.h"

#include "assetIndices/sequences.h"
#include "assetIndices/waveTable.h"

// data
WaveTableInfo waveTableAddresses[1] = {
    { (u8*)WAVE_TABLE_PTR_ROM_ADDRESS_START, (u8*)WAVE_TABLE_PTR_ROM_ADDRESS_END, (u8*)WAVE_TABLE_ROM_ADDRESS },
};

Addresses sequenceRomAddresses[] = {
    { SEQUENCE_0_ROM_START, SEQUENCE_0_ROM_END },
    { SEQUENCE_1_ROM_START, SEQUENCE_1_ROM_END },
    { SEQUENCE_2_ROM_START, SEQUENCE_2_ROM_END },
    { SEQUENCE_3_ROM_START, SEQUENCE_3_ROM_END },
    { SEQUENCE_4_ROM_START, SEQUENCE_4_ROM_END },
    { SEQUENCE_5_ROM_START, SEQUENCE_5_ROM_END },
    { SEQUENCE_6_ROM_START, SEQUENCE_6_ROM_END },
    { SEQUENCE_7_ROM_START, SEQUENCE_7_ROM_END },
    { SEQUENCE_8_ROM_START, SEQUENCE_8_ROM_END },
    { SEQUENCE_9_ROM_START, SEQUENCE_9_ROM_END },
    { SEQUENCE_10_ROM_START, SEQUENCE_10_ROM_END },
    { SEQUENCE_11_ROM_START, SEQUENCE_11_ROM_END },
    { SEQUENCE_12_ROM_START, SEQUENCE_12_ROM_END },
    { SEQUENCE_13_ROM_START, SEQUENCE_13_ROM_END },
    { SEQUENCE_14_ROM_START, SEQUENCE_14_ROM_END },
    { SEQUENCE_15_ROM_START, SEQUENCE_15_ROM_END },
    { SEQUENCE_16_ROM_START, SEQUENCE_16_ROM_END },
    { SEQUENCE_17_ROM_START, SEQUENCE_17_ROM_END },
    { SEQUENCE_18_ROM_START, SEQUENCE_18_ROM_END },
    { SEQUENCE_19_ROM_START, SEQUENCE_19_ROM_END },
    { SEQUENCE_20_ROM_START, SEQUENCE_20_ROM_END },
    { SEQUENCE_21_ROM_START, SEQUENCE_21_ROM_END },
    { SEQUENCE_22_ROM_START, SEQUENCE_22_ROM_END },
    { SEQUENCE_23_ROM_START, SEQUENCE_23_ROM_END },
    { SEQUENCE_24_ROM_START, SEQUENCE_24_ROM_END },
    { SEQUENCE_25_ROM_START, SEQUENCE_25_ROM_END },
    { SEQUENCE_26_ROM_START, SEQUENCE_26_ROM_END },
    { SEQUENCE_27_ROM_START, SEQUENCE_27_ROM_END },
    { SEQUENCE_28_ROM_START, SEQUENCE_28_ROM_END },
    { SEQUENCE_29_ROM_START, SEQUENCE_29_ROM_END },
    { SEQUENCE_30_ROM_START, SEQUENCE_30_ROM_END },
    { SEQUENCE_31_ROM_START, SEQUENCE_31_ROM_END },
    { SEQUENCE_32_ROM_START, SEQUENCE_32_ROM_END },
    { SEQUENCE_33_ROM_START, SEQUENCE_33_ROM_END },
    { SEQUENCE_34_ROM_START, SEQUENCE_34_ROM_END },
    { SEQUENCE_35_ROM_START, SEQUENCE_35_ROM_END },
    { SEQUENCE_36_ROM_START, SEQUENCE_36_ROM_END },
    { SEQUENCE_37_ROM_START, SEQUENCE_37_ROM_END },
    { SEQUENCE_38_ROM_START, SEQUENCE_38_ROM_END },
    { SEQUENCE_39_ROM_START, SEQUENCE_39_ROM_END },
    { SEQUENCE_40_ROM_START, SEQUENCE_40_ROM_END },
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
    { SEQUENCE_47_ROM_START, SEQUENCE_47_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_57_ROM_START, SEQUENCE_57_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_58_ROM_START, SEQUENCE_58_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_51_ROM_START, SEQUENCE_51_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_48_ROM_START, SEQUENCE_48_ROM_END },             
    { SEQUENCE_49_ROM_START, SEQUENCE_49_ROM_END },             
    { SEQUENCE_50_ROM_START, SEQUENCE_50_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_54_ROM_START, SEQUENCE_54_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_55_ROM_START, SEQUENCE_55_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_53_ROM_START, SEQUENCE_53_ROM_END },             
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
    { SEQUENCE_52_ROM_START, SEQUENCE_52_ROM_END },             
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
    { SEQUENCE_41_ROM_START, SEQUENCE_41_ROM_END },             
    { SEQUENCE_42_ROM_START, SEQUENCE_42_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_43_ROM_START, SEQUENCE_43_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_44_ROM_START, SEQUENCE_44_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_59_ROM_START, SEQUENCE_59_ROM_END },             
    { SEQUENCE_45_ROM_START, SEQUENCE_45_ROM_END },             
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { 0, 0 },  
    { SEQUENCE_46_ROM_START, SEQUENCE_46_ROM_END },             
    { 0, 0 },  
    { SEQUENCE_56_ROM_START, SEQUENCE_56_ROM_END },             
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

// for sfx; sfx can either be raw audio data from `/data/audio/sfx.c` or sequences that use the game's wavetable
u8 audioTypeTable[] = {
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,SFX_TYPE, 
    SFX_TYPE, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,
    SEQUENCE_TYPE, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,
    SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SEQUENCE_TYPE, SEQUENCE_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SEQUENCE_TYPE, SFX_TYPE, SFX_TYPE, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, 
    SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    0x03, SEQUENCE_TYPE,SFX_TYPE, SFX_TYPE, SFX_TYPE, SFX_TYPE, 
    SEQUENCE_TYPE, SFX_TYPE, SEQUENCE_TYPE, 0x00, 0x00, 0x00, 0x00, 
    
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