#include "common.h"
#include "yay0.h"

// Placed at 0x8030F800, taking over the slot vacated by unknownBuffer.
u8 yay0Scratch[YAY0_SCRATCH_SIZE] __attribute__((aligned(16)));
