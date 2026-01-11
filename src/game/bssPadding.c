#include "common.h"

// this is a hack to get the original total bss size for the main code segment
// bss addresses are assigned partially via their alphabetical order (z = to ensure last in this case)
u8 z[0xFAC20];