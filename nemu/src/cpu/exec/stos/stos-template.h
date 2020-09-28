#include "cpu/exec/template-start.h"

#define instr stos
make_helper(concat(stos_m_, SUFFIX)){
    MEM_W(reg_l(R_EDI), REG(R_EAX), R_ES);
    printf("after stos 0x%x\n", swaddr_read(reg_l(R_EDI), 4, R_ES));
    int incDec = cpu.DF ? -DATA_BYTE : DATA_BYTE;
    reg_l(R_EDI) = reg_l(R_EDI) + incDec;
    print_asm_template5();
    return 1;
}

#include "cpu/exec/template-end.h"