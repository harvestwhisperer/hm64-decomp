#include "common.h"

// the virtual address for this gets specified in the spec file
/*
beginseg
	name "gamePakBuffer"
	flags OBJECT
	address 0x8030E000
	include "gamePakBuffer.o"
endseg
*/

// TODO: actually a struct (stores different types and accessed contiguously)
u8 D_8030E000[0x100];