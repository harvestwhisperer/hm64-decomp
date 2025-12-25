#include <nusys.h>
#include <nualstl_n.h>

void nuAuStlPlayerInit(musConfig *c, u32 size)
{
    MusInitialize(c);

    nuAuSeqPlayer[NU_AU_SEQ_PLAYER0].data_ptr = alHeapAlloc(&nuAuHeap, 1, size);
    // added in this version of nusys
    nuAuSeqPlayer[NU_AU_SEQ_PLAYER1].data_ptr = alHeapAlloc(&nuAuHeap, 1, size);
    nuAuSeqPlayer[2].data_ptr = alHeapAlloc(&nuAuHeap, 1, size); 
    nuAuSeqPlayer[3].data_ptr = alHeapAlloc(&nuAuHeap, 1, size); 

}