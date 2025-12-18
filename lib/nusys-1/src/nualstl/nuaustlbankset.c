#include <nusys.h>
#include <nualstl_n.h>

extern ptr_bank_t	*mus_init_bank;
extern ptr_bank_t	*mus_default_bank;

void nuAuStlBankSet(u8 *pbk_addr, u32 pbk_size, u8 *wbk_addr) {

    nuAuSeqBank_ptr = alHeapAlloc(&nuAuHeap, 1, pbk_size);
    nuPiReadRom((u32)pbk_addr, nuAuSeqBank_ptr, pbk_size);

    MusBankInitialize(nuAuSeqBank_ptr, wbk_addr);
    mus_default_bank = mus_init_bank = (ptr_bank_t *)nuAuSeqBank_ptr;
    
}
