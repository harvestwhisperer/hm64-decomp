#include <nusys.h>
#include <nualstl_n.h>

void nuAuStlSeqPlayerSetData(u8 *seq_addr, u32 seq_size) {
    
    if (seq_size & 0x00000001) seq_size++;

    nuPiReadRom((u32)seq_addr, nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr, seq_size);

}

void nuAuStlSeqPlayerDataSet(u8 player_no, u8* seq_addr, u32 seq_size) {
	
	if (seq_size & 0x00000001) seq_size++;

	nuPiReadRom((u32)seq_addr, (void*)nuAuSeqPlayer[player_no].data_ptr, seq_size);

}