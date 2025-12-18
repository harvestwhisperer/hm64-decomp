#include <nusys.h>

void nuContPakCodeSet(u8* companyCode, u8* gameCode) {
    bcopy(companyCode, (u8*)&nuContPakCompanyCode, 2);
    bcopy(gameCode, (u8*)&nuContPakGameCode, 4);
}
