#include <nusys.h>
#include <nualstl_n.h>

u32 nuAuStlSeqPlayerPlay(void) {
    return( MusStartSong(nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr) );
}

musHandle nuAuStlSeqPlayerPlay2(u8 player_no) {
    return(MusStartSong(nuAuSeqPlayer[player_no].data_ptr));
}
