#include <nusys.h>
#include <nualstl_n.h>

extern void* D_8018965C;
extern void* D_801897E0;

void nuAuStlPlayerInit(musConfig *c, u32 size) {

    MusInitialize(c);
    
    nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr = alHeapAlloc(&nuAuHeap, 1, size);
    nuAuSeqPlayer[NU_AU_SEQ_PLAYER1].data_ptr = alHeapAlloc(&nuAuHeap, 1, size);

	D_8018965C = alHeapAlloc(&nuAuHeap, 1, size);
	D_801897E0 = alHeapAlloc(&nuAuHeap, 1, size);

}
