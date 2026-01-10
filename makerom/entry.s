.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

glabel entrypoint
    lui     $t0, %hi(_codeSegmentBssStart)
    addiu   $t0, $t0, %lo(_codeSegmentBssStart)
    lui     $t1, %hi(_codeSegmentBssSize)
    addiu   $t1, $t1, %lo(_codeSegmentBssSize)
.clear_bss:
    sw      $zero, 0x0($t0)
    sw      $zero, 0x4($t0)
    addi    $t0, $t0, 0x8
    addi    $t1, $t1, -0x8
    bnez    $t1, .clear_bss
    nop
    lui     $t2, %hi(nuBoot)
    addiu   $t2, $t2, %lo(nuBoot)
    lui     $sp, %hi(nuMainStack + 0x2000)
    jr      $t2
    addiu  $sp, $sp, %lo(nuMainStack + 0x2000)

.fill 0x50 - (. - entrypoint)