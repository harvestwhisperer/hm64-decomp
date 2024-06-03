#include "common.h"

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", n_alSynNew);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", n_alAudioFrame);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", _n_allocParam);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", _n_freeParam);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", _n_collectPVoices);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", _n_freePVoice);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", _n_timeToSamplesNoRound);

INCLUDE_ASM(const s32, "lib/os/libultra/libnaudio/n_synthesizer", _n_timeToSamples);
