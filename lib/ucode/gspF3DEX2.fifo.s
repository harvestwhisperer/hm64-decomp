#include "hasm.h"

.section .text, "ax"

LEAF(gspF3DEX2_fifoTextStart)
.incbin "lib/ucode/gspF3DEX2.fifo.textbin.bin"
END(gspF3DEX2_fifoTextStart)
EXPORT(gspF3DEX2_fifoTextEnd)

.section .data, "wa"

SYM_START(gspF3DEX2_fifoDataStart)
.incbin "lib/ucode/gspF3DEX2.fifo.databin.bin"
SYM_END(gspF3DEX2_fifoDataStart)
EXPORT(gspF3DEX2_fifoDataEnd)