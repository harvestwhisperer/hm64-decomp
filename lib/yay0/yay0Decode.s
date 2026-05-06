.set noreorder
.set noat

.text
.globl yay0Decode
.ent yay0Decode

# void yay0Decode(const u8 *src, u8 *dst)
#   $a0 = src (also used as flagPtr after init), $a1 = dst (advances)
#
# All state lives in caller-saved $a/$t regs so the function is leaf-like:
# no stack frame, no prologue/epilogue. State is kept narrow enough that
# the byte-at-a-time copy loop plus 32-bit flag-word refills outruns a
# variant that calls out to libultra bcopy on long non-overlapping runs
# (benched on farmSpringMap, ~3% faster than the bcopy-fast-path version).
#
#
#   $a0 = flagPtr (advances by 4 per flag word read)
#   $a1 = dst (advances)
#   $a2 = flagBits remaining (32 -> 0, reload at 0)
#   $a3 = linkPtr
#   $t0 = flagWord (top bit = current flag; shifted left each iter)
#   $t8 = dstEnd
#   $t9 = chunkPtr
#   $t1-$t3 scratch

yay0Decode:
    lw      $t8, 4($a0)         # decompressedSize
    lw      $a3, 8($a0)         # linkOffset
    lw      $t9, 0xC($a0)       # chunkOffset
    move    $a2, $zero          # flagBits = 0
    addu    $t8, $t8, $a1       # dstEnd = dst + size
    addu    $a3, $a3, $a0       # linkPtr = src + linkOffset
    addu    $t9, $t9, $a0       # chunkPtr = src + chunkOffset
    addiu   $a0, $a0, 0x10      # flagPtr = src + 16

main_loop:
    bnez    $a2, have_bits
    nop
    lw      $t0, 0($a0)         # load next 32 flag bits
    li      $a2, 0x20
    addiu   $a0, $a0, 4

have_bits:
    bltz    $t0, literal_path   # top bit set -> literal
    nop

# --- Back-reference ---
    lhu     $t2, 0($a3)         # 16-bit link (native BE on VR4300)
    addiu   $a3, $a3, 2
    srl     $t3, $t2, 12        # raw len bits (0 = extended)
    andi    $t2, $t2, 0xFFF     # raw dist
    beqz    $t3, extended_len
    subu    $t1, $a1, $t2       # from = dst - raw_dist  [delay slot]
    addiu   $t3, $t3, 2         # short_len = raw_len + 2

byte_loop:
    lb      $t2, -1($t1)
    addiu   $t3, $t3, -1
    addiu   $t1, $t1, 1
    sb      $t2, 0($a1)
    bnez    $t3, byte_loop
    addiu   $a1, $a1, 1

advance:
    sll     $t0, $t0, 1
    bne     $a1, $t8, main_loop
    addiu   $a2, $a2, -1
    jr      $ra
    nop

# --- Extended back-ref length ---
extended_len:
    lbu     $t3, 0($t9)
    addiu   $t9, $t9, 1
    b       byte_loop
    addiu   $t3, $t3, 0x12      # ext_len = byte + 18

# --- Literal ---
literal_path:
    lb      $t2, 0($t9)
    addiu   $t9, $t9, 1
    sb      $t2, 0($a1)
    b       advance
    addiu   $a1, $a1, 1

.end yay0Decode
