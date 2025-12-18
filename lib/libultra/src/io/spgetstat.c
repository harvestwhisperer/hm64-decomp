#include <os_internal.h>
#include <rcp.h>

// TODO: this comes from a header
#ident "$Revision: 1.17 $"

u32 __osSpGetStatus() {
	return IO_READ(SP_STATUS_REG);
}
