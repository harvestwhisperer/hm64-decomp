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
/* Internal name: "ﾎﾞｸｼﾞｮｳﾓﾉｶﾞﾀﾘ2" + padding (half-width katakana) */
.byte 0xce, 0xde, 0xb8, 0xbc, 0xde, 0xae, 0xb3, 0xd3, 0xc9, 0xb6, 0xde, 0xc0, 0xd8
.ascii "2      "                /* '2' + padding to 20 bytes */
.word 0                         /* Unknown 3 */
.word 0x0000004E
.ascii "YW"                     /* Cartridge ID */
.ascii "J"                      /* Country code (Japan) */
.byte 0x0                       /* Version */
