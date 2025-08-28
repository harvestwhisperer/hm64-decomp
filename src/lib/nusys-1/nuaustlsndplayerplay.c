#include <nusys.h>
#include <nualstl_n.h>

u32 nuAuStlSndPlayerPlay(u32 sndNo) {
    return( MusStartEffect(sndNo) );
}

u32 nuAuStlSndPlayerPlay2(u32 sndNo, s32 volume, s32 pan, s32 restartflag, s32 priority) {
    return( MusStartEffect2(sndNo, volume, pan , restartflag, priority) );
}
