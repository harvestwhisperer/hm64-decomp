#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", alFxPull);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", alFxParam);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", alFxParamHdl);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", _loadOutputBuffer);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", _loadBuffer);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", _saveBuffer);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", _filterBuffer);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/reverb", _doModFunc);
