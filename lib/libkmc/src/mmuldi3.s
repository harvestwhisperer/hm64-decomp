#include "sys/asm.h"


.section .text


/*
    (long long) * (long long)

    A .. $a1:$a0
    B .. $a3:$a2

    A.H..Upper 32 bit of A
    A.L..Lower 32 bit of A
    B.H..Upper 32 bit of B
    B.L..Lower 32 bit of B

    (A.H+A.L)*(B.H+B.L) = A.H*B.H+A.L*B.H+A.H*B.L+A.L*B.L

    The calculation is deleted because of the site error of  A.H*B.H
*/
LEAF(__muldi3)
    nop
    multu   $a1, $a2
    mflo    $t0
    nop
    nop
    multu   $a0, $a3
    mflo    $v1
    addu    $t0, $t0, $v1
    nop
    multu   $a0, $a2
    mflo    $v1
    mfhi    $v0
    addu    $v0, $v0, $t0
    j       $ra
END(__muldi3)



/*
    (long long) / (long long) support routine

    A .. $a0:$a1(divisor)
    B .. $a2:$a3(dividend)

    return

    $v1     .. quotient
    $a0:$a1 .. remainder

*/
LEAF(div64_64)
    xor     $v1, $v1, $v1           /* c[$v1] = 0 */
    sltu    $t9, $a2, $a0
    bnez    $t9, .div64_64_2         /* A.H[$a0]>B.H[$a2] */
    bne     $a0, $a2, .div64_64_1    /* A.H[$a0]<B.H[$a2] */

    sltu    $t9, $a3, $a1
    bnez    $t9, .div64_64_2         /* A.L[$a1]>B.L[$a3] */
    bne     $a1, $a3, .div64_64_1    /* A.L[$a1]<B.L[$a3] */

/* A == B */
    xor     $a1, $a1, $a1
    xor     $a0, $a0, $a0
    li      $v1, 0x1
    xor     $v0, $v0, $v0
    jr      $ra

.div64_64_1:
/* A < B */
    xor     $v0, $v0, $v0
    jr      $ra

.div64_64_2:
    li      $v0, 0x1                /* bit[$v0] = 1 */
    li      $t0, 0x80000000         /* msk[$t0] = 0x80000000 */

.div64_64_3:
    and     $t9, $a2, $t0
    bnez    $t9, .div64_64_7

    srl     $t9, $a3, 31
    sll     $a2, $a2, 1
    or      $a2, $a2, $t9
    sll     $a3, $a3, 1             /* B[$a2:$a3] <<= 1 */
    sltu    $t9, $a2, $a0
    bnez    $t9, .div64_64_4         /* A.H[$a0]>B.H[$a2] */
    bne     $a0, $a2, .div64_64_5    /* A.H[$a0]<B.H[$a2] */

    sltu    $t9, $a3, $a1
    beqz    $t9, .div64_64_5         /* A.L[$a1]<=B.L[$a3] */

.div64_64_4:
    sll     $v0, $v0, 1             /* bit[$v0] <<= 1 */
    j       .div64_64_3

.div64_64_5:
    sll     $t9, $a2, 31
    srl     $a3, $a3, 1
    or      $a3, $a3, $t9
    srl     $a2, $a2, 1             /* B[$a2:$a3] >>= 1 */

.div64_64_7:
    sltu    $t9, $a2, $a0
    bnez    $t9, .div64_64_8         /* A.H[$a0]>B.H[$a2] */
    bne     $a0, $a2, .div64_64_9    /* A.H[$a0]<B.H[$a2] */
    sltu    $t9, $a1, $a3
    bnez    $t9, .div64_64_9         /* B.L[$a3]>A.L[$a1] */

.div64_64_8:
    sltu    $t9, $a1, $a3
    subu    $a1, $a1, $a3
    subu    $a0, $a0, $a2
    subu    $a0, $a0, $t9
    or      $v1, $v1, $v0           /* c[$v1] |= bit[$v0] */
.div64_64_9:
    sll     $t9, $a2, 31
    srl     $a3, $a3, 1
    or      $a3, $a3, $t9
    srl     $a2, $a2, 1             /* B[$a2:$a3] >>= 1 */
    srl     $v0, $v0, 1             /* bit[$v0] >>= 1 */
    bnez    $v0, .div64_64_7

    xor     $v0, $v0, $v0
    jr      $ra
END(div64_64)


/*
    (long long) / (long long) support routine

    A .. $a0:$a1(divisor)
    B .. $a3(dividend)


    return

    $v0:$v1 .. quotient
    $t1     .. remainder

*/
LEAF(div64_32)
    divu_ds $t2,$a0,$a3
    mfhi    $a0
    xor     $t0, $t0, $t0

.div64_32_0:
    beqz    $a0, .div64_32_2
    move    $v1, $a1                /* a1[$v0:$v1] = a[$a0:$a1] */
    move    $v0, $a0
    move    $t1, $a3                /* b1[edp] = b[$a3] */

.div64_32_1:
    sll     $t9, $v0, 31
    srl     $v1, $v1, 1
    or      $v1, $v1, $t9
    srl     $v0, $v0, 1             /* a1[$v0:$v1] >>= 1 */
    srl     $t1, $t1, 1             /* b1[$t1] >>= 1 */
    bnez    $v0, .div64_32_1

    addiu   $t1, $t1, 0x1
    divu_ds $v1, $v1, $t1           /* $v1 = a1[$v0:$v1]/(b1[$t1]+1) */

    xor     $v0, $v0, $v0
    addu    $t0, $t0, $v1
    sltu    $t9, $t0, $v1
    addu    $t2, $t2, $v0
    addu    $t2, $t2, $t9           /* c1[$t2:$t0] += 0:$v1 */

    nop
    multu   $v1, $a3

    mflo    $v1
    mfhi    $v0
    sltu    $t9, $a1, $v1
    subu    $a1, $a1, $v1
    subu    $a0, $a0, $v0
    subu    $a0, $a0, $t9
    j       .div64_32_0

.div64_32_2:
    divu_ds $v1, $a1, $a3           /* $v1 = a1[bx]/b[$a3] */
    mfhi    $t1
    xor     $v0, $v0, $v0
    addu    $v1, $v1, $t0
    sltu    $t9, $v1, $t0
    addu    $v0, $v0, $t2
    addu    $v0, $v0, $t9
    j       $ra
END(div64_32)


/*
    (long long) / (long long)

    A .. $a0:$a1
    B .. $a2:$a3
*/
.align    4
LEAF(__divdi3)
    li      $t0, 0x80000000
    move    $v1, $a0
    xor     $v1, $v1, $a2
    and     $t9, $t0, $a0
    beqz    $t9, .divdi1
    li      $t1, -0x1
    xor     $a1, $a1, $t1
    xor     $a0, $a0, $t1
    addiu   $a1, $a1, 0x1
    sltiu   $t9, $a1, 0x1
    addu    $a0, $a0, $t9

.divdi1:
    and     $t9, $t0, $a2
    beqz    $t9, .divdi2
    li      $t1, -0x1
    xor     $a3, $a3, $t1
    xor     $a2, $a2, $t1
    addiu   $a3, $a3, 0x1
    sltiu   $t9, $a3, 0x1
    addu    $a2, $a2, $t9

.divdi2:
    and     $v1, $v1, $t0 
    beqz    $v1, .div_com
    bnez    $a2, .divdi64
    move    $t5, $ra

    jal     div64_32
    move    $ra, $t5
    j       .divdi_ret

.divdi64:
    move    $t5, $ra
    jal     div64_64

    move    $ra, $t5
.divdi_ret:
    addiu   $a3, $zero, -0x1
    xor     $v1, $v1, $a3
    xor     $v0, $v0, $a3
    addiu   $v1, $v1, 0x1
    sltiu   $t9, $v1, 0x1
    addu    $v0, $v0, $t9
    j       $ra
END(__divdi3)


/*
    (long long) % (long long)

    A .. $a0:$a1
    B .. $a2:$a3
*/

.align    4
LEAF(__moddi3)
    li      $t0, 0x80000000
    move    $v1, $a0
    and     $t9, $t0, $a0
    beqz    $t9, .moddi1

    li      $t1, -0x1
    xor     $a1, $a1, $t1
    xor     $a0, $a0, $t1
    addiu   $a1, $a1, 0x1
    sltiu   $t9, $a1, 0x1
    addu    $a0, $a0, $t9

.moddi1:
    and     $t9, $t0, $a2
    beqz    $t9, .moddi2

    li      $t1, -0x1
    xor     $a3, $a3, $t1
    xor     $a2, $a2, $t1
    addu    $a3, $a3, 0x1
    sltiu   $t9, $a3, 0x1
    addu    $a2, $a2, $t9

.moddi2:
    and     $v1, $v1, $t0
    beqz    $v1, mod_com

    bnez    $a2, .moddi64
    move    $t5, $ra
    jal     div64_32

    move    $ra, $t5
    move    $v1, $t1
    xor     $v0, $v0, $v0
    j       .moddi_ret

.moddi64:
    move    $t5, $ra
    jal     div64_64

    move    $ra, $t5
    move    $v1, $a1
    move    $v0, $a0

.moddi_ret:
    li      $a3, -0x1
    xor     $v1, $v1, $a3
    xor     $v0, $v0, $a3
    addiu   $v1, $v1, 0x1
    sltiu   $t9, $v1, 0x1
    addu    $v0, $v0, $t9
    jr      $ra
END(__moddi3)

.align    4
LEAF(__udivdi3)
.div_com:
    bnez    $a2, .udivdi64
    j       div64_32
.udivdi64:
    j       div64_64
END(__udivdi3)

.align 4
LEAF(__umoddi3)
mod_com:
    bnez    $a2, .umoddi64
    move    $t5, $ra
    jal     div64_32
    move    $ra, $t5
    move    $v1, $t1
    xor     $v0, $v0, $v0
    j       $ra

.umoddi64:
    move    $t5, $ra
    jal     div64_64
    move    $ra, $t5
    move    $v1, $a1
    move    $v0, $a0
    j       $ra
END(__umoddi3)
