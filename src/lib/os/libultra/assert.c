#include "PR/os.h"

//INCLUDE_ASM(const s32, "lib/os/libultra/assert", __assert);

void __assert(const char* exp, const char* filename, int line) {
    osSyncPrintf("\nASSERTION FAULT: %s, %d: \"%s\"\n", filename, line, exp);
}