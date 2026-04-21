.set noreorder
.set noat

.text
.globl yay0Decode
.ent yay0Decode

# void yay0Decode(const u8 *src, u8 *dst)
#   $a0 = src, $a1 = dst
#
# Register map across the main loop:
#   $s1 = dst (advances)
#   $s2 = flagPtr
#   $s3 = linkPtr
#   $s4 = chunkPtr
#   $s5 = dstEnd
#   $s6 = flagByte (next flag bit in MSB; shifted left each iter)
#   $s7 = flagBits remaining (counts 8 -> 0)
#   $t0-$t5 scratch

yay0Decode:
    addiu   $sp, $sp, -40
    sw      $ra, 36($sp)
    sw      $s0, 0($sp)
    sw      $s1, 4($sp)
    sw      $s2, 8($sp)
    sw      $s3, 12($sp)
    sw      $s4, 16($sp)
    sw      $s5, 20($sp)
    sw      $s6, 24($sp)
    sw      $s7, 28($sp)

    lw      $s5, 4($a0)         # decompressedSize
    lw      $t0, 8($a0)         # linkOffset
    lw      $t1, 0xC($a0)       # chunkOffset
    addiu   $s2, $a0, 0x10      # flagPtr = src + 16
    addu    $s3, $a0, $t0       # linkPtr
    addu    $s4, $a0, $t1       # chunkPtr
    addu    $s5, $a1, $s5       # dstEnd = dst + size

    move    $s1, $a1            # dst (advances)
    move    $s6, $zero          # flagByte
    move    $s7, $zero          # flagBits

main_loop:
    bnez    $s7, have_bits
     nop
    lbu     $t0, 0($s2)
    addiu   $s2, $s2, 1
    sll     $s6, $t0, 24        # flag byte -> MSB
    li      $s7, 8

have_bits:
    bltz    $s6, literal_path   # top bit set -> literal
     nop

# --- Back-reference ---
    lhu     $t0, 0($s3)         # 16-bit link (native BE on VR4300)
    addiu   $s3, $s3, 2
    srl     $t1, $t0, 12        # raw len bits (0 = extended)
    andi    $t2, $t0, 0xFFF
    bnez    $t1, short_len
     addiu  $t2, $t2, 1         # dist = (link & 0xFFF) + 1   [delay slot]

    # len bits == 0: read extended-length byte
    lbu     $t1, 0($s4)
    addiu   $s4, $s4, 1
    j       have_len
     addiu  $t1, $t1, 0x12      # len = byte + 18            [delay slot]

short_len:
    addiu   $t1, $t1, 2         # len = raw + 2

have_len:
    subu    $t3, $s1, $t2       # from = dst - dist

    # Fast path via libultra bcopy when back-ref is non-overlapping and
    # long enough that the call overhead pays back. bcopy is 64-bit-aware
    # hand-tuned asm and handles unaligned endpoints.
    sltu    $t4, $t1, $t2       # t4 = (len < dist)   -> non-overlapping
    beqz    $t4, byte_loop_start
     slti   $t5, $t1, 4         # t5 = (len < 4)      -> too short
    bnez    $t5, byte_loop_start
     nop

    # bcopy(from, dst, len); advance dst in delay slot (s1 is callee-saved)
    move    $a0, $t3
    move    $a1, $s1
    move    $a2, $t1
    jal     bcopy
     addu   $s1, $s1, $t1
    j       advance
     nop

byte_loop_start:
byte_loop:
    lb      $t4, 0($t3)
    addiu   $t3, $t3, 1
    addiu   $t1, $t1, -1
    sb      $t4, 0($s1)
    bnez    $t1, byte_loop
     addiu  $s1, $s1, 1
    j       advance
     nop

# --- Literal ---
literal_path:
    lbu     $t4, 0($s4)
    addiu   $s4, $s4, 1
    sb      $t4, 0($s1)
    addiu   $s1, $s1, 1

advance:
    sll     $s6, $s6, 1
    bne     $s1, $s5, main_loop
     addiu  $s7, $s7, -1

    # Epilogue
    lw      $ra, 36($sp)
    lw      $s0, 0($sp)
    lw      $s1, 4($sp)
    lw      $s2, 8($sp)
    lw      $s3, 12($sp)
    lw      $s4, 16($sp)
    lw      $s5, 20($sp)
    lw      $s6, 24($sp)
    lw      $s7, 28($sp)
    jr      $ra
     addiu  $sp, $sp, 40

.end yay0Decode
