#include "common.h"
#include "nusys.h"

//INCLUDE_ASM()

void nuGfxTaskAllEndWait(void) {
    while (nuGfxTaskSpool);
}
