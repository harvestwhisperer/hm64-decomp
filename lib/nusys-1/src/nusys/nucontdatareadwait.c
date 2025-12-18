#include <nusys.h>

void nuContDataReadWait(void) {
    osRecvMesg(&nuContWaitMesgQ, NULL, OS_MESG_BLOCK);
}
