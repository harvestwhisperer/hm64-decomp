#include <nusys.h>

static OSMesgQueue PiMesgQ;
static OSMesg PiMesgBuf[NU_PI_MESG_NUM];

void nuPiInit(void) {
    osCreatePiManager((OSPri)OS_PRIORITY_PIMGR, &PiMesgQ, PiMesgBuf, NU_PI_MESG_NUM);
}
