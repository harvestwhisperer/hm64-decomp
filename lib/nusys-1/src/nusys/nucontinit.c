#include <nusys.h>

u8 nuContInit(void) {

    u8 pattern;

    pattern = nuSiMgrInit();

    nuContMgrInit();
    nuContPakMgrInit();
    nuContRmbMgrInit();

    return pattern;

}
