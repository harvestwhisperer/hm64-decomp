#ifndef _COMMON_H_
#define _COMMON_H_

#include "include_asm.h"

#include "ultra64.h"
#include "nusys.h"
#include "libmus/libmus.h"

#include "structs.h"

typedef u8 bool;

// FIXM: signature from ultra64.h; not getting picked up by sprite.c
void __assert(const char* exp, const char* filename, int line);     

#define TRUE 1
#define FALSE 0

#define MAX_UNSIGNED_SHORT 65535

#define NAME_LENGTH 6

#endif