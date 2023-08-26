#include "common.h"
#include "nusys.h"

//INCLUDE_ASM(const s32, "lib/nusys-1/nugfxtaskallendwait", nuGfxTaskAllEndWait);

void nuGfxTaskAllEndWait(void) {
    while (nuGfxTaskSpool);
}
