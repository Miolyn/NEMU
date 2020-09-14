#include "cpu/exec/template-start.h"

#define instr movs

#if DATA_BYTE != 1
make_helper(concat(movs_m_, SUFFIX)){
    MEM_W(REG(R_EDI), MEM_R(REG(R_ESI)));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    REG(R_ESI) = REG(R_ESI) + incDec;
    REG(R_EDI) = REG(R_EDI) + incDec;
    
    return 1;
}
#endif

#if DATA_BYTE == 1
make_helper(concat(movs_m_, SUFFIX)){
    MEM_W(reg_w(R_DI), MEM_R(reg_w(R_SI)));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    reg_w(R_ESI) = reg_w(R_ESI) + incDec;
    reg_w(R_EDI) = reg_w(R_EDI) + incDec;
    return 1;
}
#endif
#include "cpu/exec/template-end.h"