#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", alEnvmixerPull);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", alEnvmixerParam);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _pullSubFrame);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _frexpf);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _ldexpf);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _getRate);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/env", _getVol);

INCLUDE_RODATA(const s32, "lib/os/libultra/audio/env", env_data_0000);
