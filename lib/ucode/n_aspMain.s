#include "hasm.h"

.section .text, "ax"

LEAF(aspMainTextStart)
.incbin "lib/ucode/n_aspMain.textbin.bin"
END(aspMainTextStart)
EXPORT(aspMainTextEnd)

.section .data, "wa"

SYM_START(aspMainDataStart)
.incbin "lib/ucode/n_aspMain.databin.bin"
SYM_END(aspMainDataStart)
EXPORT(aspMainDataEnd)