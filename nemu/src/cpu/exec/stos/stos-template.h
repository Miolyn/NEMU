#include "cpu/exec/template-start.h"

#define instr stos

#if DATA_BYTE != 1
make_helper(concat(stos_m_, SUFFIX)){
    MEM_W(REG(R_EDI), REG(R_EAX));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    REG(R_EDI) = REG(R_EDI) + incDec;
    return 1;
}
#endif

#if DATA_BYTE == 1
make_helper(concat(stos_m_, SUFFIX)){
    MEM_W(reg_w(R_EDI), REG(R_EAX));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    reg_w(R_EDI) = reg_w(R_EDI) + incDec;
    return 1;
}
#endif

#include "cpu/exec/template-end.h"