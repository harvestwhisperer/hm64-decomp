#include "common.h"

// > version 1.0, but < version 3

INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", __MusIntMain);

// __MusIntGetNewNote
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F5F50);

// __MusIntFlushPending
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F6544);

// __MusIntSetVolumeAndPan
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F65EC);

// __MusIntSetPitch
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F67DC);

// __MusIntInitEnvelope
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F69C0);

// __MusIntProcessEnvelope
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F6A8C);

// __MustIntInitSweep
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F6DD4);

// __MusIntProcessSweep
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F6E20);

// __MusIntProcessWobble
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F6F84);

// __MusIntProcessVibrato
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F7038);

// __MusIntProcessContinuousVolume
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F70F4);

// __MusIntProcessContinuousPitchBend
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F725C);

// __MusIntPowerOf2
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F7440);

// __MusIntRemapPtrBank
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F7690);

// __MusIntRandom
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F7A0C);

// __MusIntInitialiseChannel
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F7B1C);

// __MusIntFindChannel
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F7D5C);

// __MusIntMemSet
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F8000);

// not in version 1.0
// __MusIntRemapPtrs
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F8070);

// __MusIntStartEffect
INCLUDE_ASM(const s32, "lib/libmus/libsrc/player", func_800F8134);
