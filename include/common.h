#ifndef _COMMON_H_
#define _COMMON_H_

#include "include_asm.h"

#include <ultra64.h>
#include <nusys.h>
#include <libmus.h>

#include "structs.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TRUE 1
#define FALSE 0

typedef u32 Addr;
typedef u8 bool;

// FIXME: signature from ultra64.h; not getting picked up by sprite.c
void __assert(const char* exp, const char* filename, int line);     

#endif