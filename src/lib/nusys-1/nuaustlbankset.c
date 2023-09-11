#include <nusys.h>
#include "nualstl/nualstl.h"

extern ptr_bank_t	*mus_init_bank;
extern ptr_bank_t	*mus_default_bank;

//INCLUDE_ASM(const s32, "lib/nusys-1/nuaustlbankset", nuAuStlBankSet);

void nuAuStlBankSet(u8 *pbk_addr, u32 pbk_size, u8 *wbk_addr) {
    nuAuSeqBank_ptr = alHeapAlloc(&nuAuHeap, 1, pbk_size);
    nuPiReadRom((u32)pbk_addr, nuAuSeqBank_ptr, pbk_size);
    MusBankInitialize(nuAuSeqBank_ptr, wbk_addr);
    mus_init_bank = mus_default_bank = (ptr_bank_t *)nuAuSeqBank_ptr;
}