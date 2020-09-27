#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_m_, SUFFIX)){
    MEM_W(reg_l(R_EDI), MEM_R(reg_l(R_ESI), R_DS), R_ES);
    // swaddr_write(reg_l(R_EDI), 4, swaddr_read(reg_l(R_ESI), DATA_BYTE));
    // zero_flag(MEM_R(reg_l(R_ESI)));
    int incDec = cpu.DF ? -DATA_BYTE : DATA_BYTE;
    reg_l(R_ESI) = reg_l(R_ESI) + incDec;
    reg_l(R_EDI) = reg_l(R_EDI) + incDec;
    print_asm_template5();
    return 1;
}

#include "cpu/exec/template-end.h"