#include "common.h"

// version 2.00

// player_commands.c

// FStop
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3220);

// FWave
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3274);

// Fport
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3308);

// FPortoff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F336C);

// Fdefa
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F33A0);

// Ftempo
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F35B8);

// Fendit
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3724);

// Fcutoff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3770);

// Fvibup
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F37E4);

// Fvibdown
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3878);

// Fviboff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3910);

// Flength
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F394C);

// Fignore
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F39EC);

// Ftrans
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3A24);

// Fignore_trans
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3A68);

// Fdistort
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3AA0);

// Fenvelope
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3B60);

// Fenvoff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3C18);

// Fenvon
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3C50);

// Ftroff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3C84);

// Ftron
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3CBC);

// Ffor
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3CF0);

// Fnext
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F3E94);

// Fwobble
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4070);

// Fwobbleoff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F40E4);

// Fvelon
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4118);

// Fveloff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4150);

// Fvelocity
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4184);

// Fpan
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F41D0);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", Fstereo);

// Fdrums
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F424C);

// Fdrumsoff
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F42C0);

// Fprint
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F42F4);

// Fgoto
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4330);

// Freverb
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4450);

// FrandNote
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4494);

// FrandVolume
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F450C);

// FrandPand
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4584);

// Fvolume
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F45FC);

// Fstartfx
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4640);

// Fbendrange
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F47C0);

// Fsweep
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4840);


// version 2.00

// FIXME: split this somehow
// player_api.c

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusInitialize);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusSetMasterVolume);

// MusStartSong
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4BC4);

// MusStartEffect
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4DB8);

// MusStartEffect2
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F4F18);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusStop);

// MusAsk
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F5228);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleStop);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleAsk);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleSetVolume);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandlePan);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusHandleSetFreqOffset);

// MusHandleSetTempo
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F573C);

// MusHandleSetReverb
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F5858);

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", MusBankInitialize);

// MusBankStartSong
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F5994);

// MusBankStartEffect
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F5A1C);

// MusBankStartEffect2
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F5AA4);

// MusHandleGetPtrAddr
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player_commands", func_800F5B48);
