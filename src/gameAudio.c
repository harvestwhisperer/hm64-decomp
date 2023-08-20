#include "common.h"
#include "system/audio.h"

extern Sfx gSfx[4];
extern SongInfo gSongs[4];

// rodata: 0xF16F0 
extern WaveTableInfo waveTableAddresses[1];
extern u8 *songRomAddresses[0x40][2];
extern u8 *sfxRomAddresses[0x80][2];
extern s32 volumesTable[];

// rodata: 0xF1CFC
// size 0x60 or 0x180? 0x60 set elements, but takes up 0x180 bytes in rodata
extern u8 audioTypeTable[0x60];


//INCLUDE_ASM(const s32, "gameAudio", initializeWaveTable);

void initializeWaveTable(u16 waveTableIndex) {
    setSongBank(waveTableAddresses[waveTableIndex].pbk_addr, waveTableAddresses[waveTableIndex].pbk_end, waveTableAddresses[waveTableIndex].wbk_addr);
}

//INCLUDE_ASM(const s32, "gameAudio", func_800ACB04);

void func_800ACB04(u16 songIndex) {

    if (songIndex < TOTAL_SONGS) {
        setSong(0, songRomAddresses[songIndex][0], songRomAddresses[songIndex][1]);
    }
    
    setSongVolumes(0, 0, 0);
}

//INCLUDE_ASM(const s32, "gameAudio", func_800ACB5C);

void func_800ACB5C(u16 songIndex) {
    if (songIndex < TOTAL_SONGS) {
        setSongSpeed(0, 32);
    }
}

//INCLUDE_ASM(const s32, "gameAudio", func_800ACB8C);

void func_800ACB8C(u16 songIndex) {
    if (songIndex  < TOTAL_SONGS) {
        stopSong(0);
    }
}

//INCLUDE_ASM(const s32, "gameAudio", func_800ACBB8);

// check if first song is open/not set
u8 func_800ACBB8(u16 songIndex) {
    
    u8 result = 0;
    
    if (songIndex < TOTAL_SONGS) {
        result = gSongs[0].flags == 0;
    }

    if (songIndex == 0xFF) {
        result = 1;
    }
    
    return result;
}


//INCLUDE_ASM(const s32, "gameAudio", func_800ACBEC);

void func_800ACBEC(u16 songIndex, u32 arg1) {
    if (songIndex < TOTAL_SONGS) {
        setSongVolumes(0, arg1, 32);
    }
}

//INCLUDE_ASM(const s32, "gameAudio", func_800ACC1C);

void func_800ACC1C(u16 songIndex) {
    if (songIndex < TOTAL_SONGS) {
        setSongVolumes(0, 0, 32);
    }
}

//INCLUDE_ASM(const s32, "gameAudio", func_800ACC50);

void func_800ACC50(u16 index) {
    
  if (index < TOTAL_SFX) {
      
    if (audioTypeTable[index] == SFX) {
      setSfx(index + 1);
      setSfxVolume(index + 1, volumesTable[index]);
      return;
    }
      
    setSong(audioTypeTable[index], sfxRomAddresses[index][0], sfxRomAddresses[index][1]);
    setSongVolumes(audioTypeTable[index], volumesTable[index], volumesTable[index]);
  }
}

//INCLUDE_ASM(const s32, "gameAudio", checkAllSfxInactive);

// check if all sfx objects are inactive
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

