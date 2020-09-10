#include "cpu/exec/template-start.h"

#define instr ret

#if DATA_BYTE == 2
make_helper(concat(ret_rel_, SUFFIX)){
    printf("start to deal with ret%d\n", DATA_BYTE);
    printf("ret%d read %x\n", DATA_BYTE, swaddr_read(REG(R_SP), DATA_BYTE) & 0xFFFF);
    cpu.eip = swaddr_read(REG(R_SP), DATA_BYTE) & 0xFFFF;
    REG(R_SP) = REG(R_SP) + DATA_BYTE;
    return -1;
}
#endif

#if DATA_BYTE == 4
make_helper(concat(ret_rel_, SUFFIX)){
    printf("start to deal with ret%d\n", DATA_BYTE);
    printf("ret%d read %x\n", DATA_BYTE, swaddr_read(REG(R_SP), DATA_BYTE));
    cpu.eip = swaddr_read(REG(R_SP), DATA_BYTE);
    REG(R_SP) = REG(R_SP) + DATA_BYTE;
    return 0;
}
#endif

#include "cpu/exec/template-end.h"