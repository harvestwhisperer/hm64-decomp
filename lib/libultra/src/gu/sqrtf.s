#include <sys/asm.h>
#include <sys/regdef.h>

LEAF(sqrtf)
    sqrt.s fv0, fa0
    j ra
END(sqrtf)
