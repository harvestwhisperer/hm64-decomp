#ifndef HASM_H
#define HASM_H

#ifdef _LANGUAGE_ASSEMBLY

#define LEAF(x)                 \
    x:                         ;\
    .globl x                   ;\
    .type x, @function         ;\
        .ent x, 0              ;\
        .frame $29, 0, $31

#define END(x)                  \
    .size x, . - x             ;\
    .end x

#define SYM_START(x)            \
    .align 2                   ;\
    .globl x                   ;\
    .type	 x, @object        ;\
    x:

#define SYM_END(x)              \
    .size x, . - x

#define EXPORT(x)               \
    .globl  x                  ;\
    x:

#define ABS(x, y)               \
    .globl  x                  ;\
    x = y

#endif

#endif
