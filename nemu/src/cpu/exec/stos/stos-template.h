#include "cpu/exec/template-start.h"

#define instr stos
make_helper(concat(stos_m_, SUFFIX)){
    MEM_W(reg_l(R_EDI), REG(R_EAX));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    reg_l(R_EDI) = reg_l(R_EDI) + incDec;
    return 1;
}

#include "cpu/exec/template-end.h"