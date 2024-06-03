#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", alSynNew);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", alAudioFrame);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", __allocParam);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", __freeParam);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", _collectPVoices);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", _freePVoice);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", _timeToSamplesNoRound);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", _timeToSamples);

INCLUDE_ASM(const s32, "lib/os/libultra/audio/synthesizer", __nextSampleTime);
