#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "common.h"

#define SRAM_START_ADDR		0x08000000
#define SRAM_SIZE		0x8000
#define SRAM_LATENCY		0x5
#define SRAM_PULSE		0x0c
#define SRAM_PAGE_SIZE		0xd
#define SRAM_REL_DURATION	0x2

extern void sramWrite(void *devAddr, void *dramAddr, u32 size);
extern void sramLoad(void* devAddr, void* dramAddr, u32 size);

#endif