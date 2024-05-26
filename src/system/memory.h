#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "common.h"

#define SRAM_START_ADDR		0x08000000
#define SRAM_SIZE		0x8000
#define SRAM_LATENCY		0x5
#define SRAM_PULSE		0x0c
#define SRAM_PAGE_SIZE		0xd
#define SRAM_REL_DURATION	0x2

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    u16 unk_6;
    u16 unk_8;
    u16 unk_A;
    u16 unk_C;
    u16 unk_E;
} Unaligned32;

typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_C;
} Aligned32;

typedef union {
    Unaligned32 unaligned_32;
    Aligned32 aligned_32;
} MemoryRead_32;

static inline void readMemory(u32 addr1, u32 addr2, u32 size) {

    MemoryRead_32 *ptr = (MemoryRead_32*)addr2;
    MemoryRead_32 *ptr2 = (MemoryRead_32*)addr1;

    if (((u32)addr1 | (u32)addr2) % 4) { 
        do {
            *(Unaligned32*)ptr++ = *(Unaligned32*)ptr2++;
        } while (ptr2 != (addr1 + size));        
    } else {
        do {
            *(Aligned32*)ptr++ = *(Aligned32*)ptr2++;
        } while (ptr2 != (addr1 + size));       
    }

}

void func_8004DD7C(void *dramAddr, void *devAddr, u32 size);

#endif