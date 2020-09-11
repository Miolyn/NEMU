#include "cpu/exec/template-start.h"

#define instr lods

#if DATA_BYTE != 1
make_helper(concat(lods_m_, SUFFIX)){
    MEM_W(REG(R_ESI), REG(R_EAX));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    REG(R_ESI) = REG(R_ESI) + incDec;
    return 1;
}
#endif

#if DATA_BYTE == 1
make_helper(concat(lods_m_, SUFFIX)){
    MEM_W(reg_w(R_ESI), REG(R_EAX));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    reg_w(R_ESI) = reg_w(R_ESI) + incDec;
    return 1;
}
#endif
#include "cpu/exec/template-end.h"