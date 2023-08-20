#ifndef __INCLUDE_ASM_H__
#define __INCLUDE_ASM_H__

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(TYPE, FOLDER, NAME, ARGS...) __asm__( "\t.section .text\n" "\t.align\t2\n" "\t.globl\t"#NAME"\n" "\t.type "#NAME", @function\n" "\t.ent\t"#NAME"\n" "\t"#NAME ":\n" "\t.include \"asm/nonmatchings/"FOLDER"/"#NAME".s\"\n" "\t.set reorder\n" "\t.set at\n" "\t.end\t"#NAME"\n" ".end"#NAME":\n" "\t.size\t"#NAME",.end"#NAME"-"#NAME );
#endif

#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(TYPE, FOLDER, NAME, ARGS...) __asm__( "\t.include \"asm/nonmatchings/"FOLDER"/"#NAME".s\"\n");
#endif

__asm__("\t.include \"include/macro.inc\"\n");

#endif
