#include "common.h"

// the virtual address for this gets specified in the spec file
/*
beginseg
	name "sramBuffer"
	flags OBJECT
	address 0x8030D000
	include "sramBuffer.o"
endseg
*/

// TODO: actually a struct (stores different types and accessed contiguously)
u8 D_8030D000[0x1000];