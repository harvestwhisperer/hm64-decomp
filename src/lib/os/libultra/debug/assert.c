#include "PR/os.h"

//INCLUDE_ASM("lib/os/libultra/debug/assert", __assert);

void __assert(const char* exp, const char* filename, int line) {
    osSyncPrintf("\nASSERTION FAULT: %s, %d: \"%s\"\n", filename, line, exp);
}