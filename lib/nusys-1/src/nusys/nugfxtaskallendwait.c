#include <nusys.h>

void nuGfxTaskAllEndWait(void) {
    while (nuGfxTaskSpool);
}
