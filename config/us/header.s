#include "nusys.h"

.section .data

.word 0x80371240                /* PI BSB Domain 1 register */
.word 15                        /* Clockrate setting */
.word 0x80025c00                /* Entrypoint address */
.short 0
.byte 20                        /* Revision */  
.ascii "I"                      /* Libultra version */
.word 0                         /* Checksum 1 */
.word 0                         /* Checksum 2 */
.word 0                         /* Unknown 1 */
.word 0                         /* Unknown 2 */
.ascii "HARVESTMOON64       "   /* Internal name */
.word 0                         /* Unknown 3 */
.word 0x0000004E                
.ascii "YW"                     /* Cartridge ID */
.ascii "E"                      /* Country code */
.byte 0x0                       /* Version */
