#ifdef __ISA_X86__
#define _JBLEN 10
#endif

#ifdef __ISA_MIPS32__
#define _JBLEN 36
#endif

#include <stdint.h>

typedef intptr_t jmp_buf[_JBLEN];
