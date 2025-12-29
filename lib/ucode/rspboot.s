#include "hasm.h"

.section .text, "ax"

LEAF(rspbootTextStart)
.incbin "lib/ucode/rspboot.textbin.bin"
END(rspbootTextStart)
EXPORT(rspbootTextEnd)
