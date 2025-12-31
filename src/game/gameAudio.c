#include "common.h"

#include "system/audio.h"

#include "game/gameAudio.h"

#include "assetIndices/sequences.h"

#include "ld_symbols.h"

// data
WaveTableInfo waveTableAddresses[1] = {
    { (u8*)&_waveTablePointerSegmentRomStart, (u8*)&_waveTablePointerSegmentRomEnd, (u8*)&_waveTableDataSegmentRomStart },
};

Addresses sequenceRomAddresses[] = {
    { &_openingSegmentRomStart, &_openingSegmentRomEnd },
    { &_titleSegmentRomStart, &_titleSegmentRomEnd },
    { &_sportsFestival1SegmentRomStart, &_sportsFestival1SegmentRomEnd },
    { &_goodbyeSegmentRomStart, &_goodbyeSegmentRomEnd },
    { &_springSegmentRomStart, &_springSegmentRomEnd },
    { &_summerSegmentRomStart, &_summerSegmentRomEnd },
    { &_autumnSegmentRomStart, &_autumnSegmentRomEnd },
    { &_winterSegmentRomStart, &_winterSegmentRomEnd },
    { &_namingScreenSegmentRomStart, &_namingScreenSegmentRomEnd },
    { &_festivalThemeSegmentRomStart, &_festivalThemeSegmentRomEnd },
    { &_sportsFestival2SegmentRomStart, &_sportsFestival2SegmentRomEnd },
    { &_spiritFestivalThemeSegmentRomStart, &_spiritFestivalThemeSegmentRomEnd },
    { &_dogRaceThemeSegmentRomStart, &_dogRaceThemeSegmentRomEnd },
    { &_racingFestivalSegmentRomStart, &_racingFestivalSegmentRomEnd },
    { &_festivalDanceSegmentRomStart, &_festivalDanceSegmentRomEnd },
    { &_mountainSegmentRomStart, &_mountainSegmentRomEnd },
    { &_villageSegmentRomStart, &_villageSegmentRomEnd },
    { &_pondSegmentRomStart, &_pondSegmentRomEnd },
    { &_churchSegmentRomStart, &_churchSegmentRomEnd },
    { &_tavernSegmentRomStart, &_tavernSegmentRomEnd },
    { &_weddingSegmentRomStart, &_weddingSegmentRomEnd },
    { &_newYearSunriseSegmentRomStart, &_newYearSunriseSegmentRomEnd },
    { &_creditsSegmentRomStart, &_creditsSegmentRomEnd },
    { &_brokenMusicBoxSegmentRomStart, &_brokenMusicBoxSegmentRomEnd },
    { &_musicBoxSegmentRomStart, &_musicBoxSegmentRomEnd },
    { &_flutePerformanceSegmentRomStart, &_flutePerformanceSegmentRomEnd },
    { &_organPerformanceSegmentRomStart, &_organPerformanceSegmentRomEnd },
    { &_ocarinaPerformanceSegmentRomStart, &_ocarinaPerformanceSegmentRomEnd },
    { &_sleighBellsSegmentRomStart, &_sleighBellsSegmentRomEnd },
    { &_badEndingSegmentRomStart, &_badEndingSegmentRomEnd },
    { &_vocalPerformanceSegmentRomStart, &_vocalPerformanceSegmentRomEnd },
    { &_ensemblePerformanceSegmentRomStart, &_ensemblePerformanceSegmentRomEnd },
    { &_drumPerformanceSegmentRomStart, &_drumPerformanceSegmentRomEnd },
    { &_typhoonAmbience1SegmentRomStart, &_typhoonAmbience1SegmentRomEnd },
    { &_typhoonAmbience2SegmentRomStart, &_typhoonAmbience2SegmentRomEnd },
    { &_nightAmbienceSpringSegmentRomStart, &_nightAmbienceSpringSegmentRomEnd },
    { &_nightAmbienceSummerSegmentRomStart, &_nightAmbienceSummerSegmentRomEnd },
    { &_nightAmbienceAutumnSegmentRomStart, &_nightAmbienceAutumnSegmentRomEnd },
    { &_starryNightFestivalSegmentRomStart, &_starryNightFestivalSegmentRomEnd },
    { &_beachAmbience1SegmentRomStart, &_beachAmbience1SegmentRomEnd },
    { &_beachAmbience2SegmentRomStart, &_beachAmbience2SegmentRomEnd },
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
    { &_horseGallop2SegmentRomStart, &_horseGallop2SegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { &_sickleSegmentRomStart, &_sickleSegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { &_hoeSegmentRomStart, &_hoeSegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { &_seq59SegmentRomStart, &_seq59SegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { &_horseGallop3SegmentRomStart, &_horseGallop3SegmentRomEnd },
    { &_water1SegmentRomStart, &_water1SegmentRomEnd },
    { &_water2SegmentRomStart, &_water2SegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { &_seedsSegmentRomStart, &_seedsSegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { &_cowBellSegmentRomStart, &_cowBellSegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { &_shimmerSegmentRomStart, &_shimmerSegmentRomEnd },
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
    { &_seq60SegmentRomStart, &_seq60SegmentRomEnd },
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
    { &_bird1SegmentRomStart, &_bird1SegmentRomEnd },
    { &_bird2SegmentRomStart, &_bird2SegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { &_cicadaSegmentRomStart, &_cicadaSegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { &_seq47SegmentRomStart, &_seq47SegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { &_fanfareSegmentRomStart, &_fanfareSegmentRomEnd },
    { &_fireworksSfxSegmentRomStart, &_fireworksSfxSegmentRomEnd },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { &_horseGallopFastSegmentRomStart, &_horseGallopFastSegmentRomEnd },
    { 0, 0 },
    { &_seq64SegmentRomStart, &_seq64SegmentRomEnd },
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