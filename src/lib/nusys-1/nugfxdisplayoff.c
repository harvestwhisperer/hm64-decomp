#include <nusys.h>

void nuGfxDisplayOff(void) {
    nuGfxDisplay = NU_GFX_DISPLAY_OFF;
    osViBlack(TRUE);
}
