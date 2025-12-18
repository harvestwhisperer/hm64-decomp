#ifndef _COMMON_H_
#define _COMMON_H_

#include "include_asm.h"

#include <ultra64.h>
#include <nusys.h>
#include <libmus.h>

#include "structs.h"

typedef u8 bool;

#ifndef NULL
#define NULL ((void *)0)
#endif

// FIXME: signature from ultra64.h; not getting picked up by sprite.c
void __assert(const char* exp, const char* filename, int line);     

#define TRUE 1
#define FALSE 0

typedef u32 Addr;

#endif