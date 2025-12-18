#include <os.h>

void __assert(const char* exp, const char* filename, int line) {
    osSyncPrintf("\nASSERTION FAULT: %s, %d: \"%s\"\n", filename, line, exp);
}