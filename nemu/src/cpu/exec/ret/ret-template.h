#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE == 2
make_helper(concat(ret_rel_, SUFFIX)){
    printf("ret%d read %x", DATA_BYTE, swaddr_read(REG(R_SP), DATA_BYTE) & 0xFFFF);
    cpu.eip = swaddr_read(REG(R_SP), DATA_BYTE) & 0xFFFF;
    REG(R_SP) = REG(R_SP) + DATA_BYTE;
    return 1;
}
#endif

#if DATA_BYTE == 4
make_helper(concat(ret_rel_, SUFFIX)){
    printf("ret%d read %x", DATA_BYTE, swaddr_read(REG(R_SP), DATA_BYTE) & 0xFFFF);
    cpu.eip = swaddr_read(REG(R_SP), DATA_BYTE);
    REG(R_SP) = REG(R_SP) + DATA_BYTE;
    return 1;
}
#endif

#include "cpu/exec/template-end.h"