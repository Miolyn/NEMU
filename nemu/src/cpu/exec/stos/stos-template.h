#include "cpu/exec/template-start.h"

#define instr stos
make_helper(concat(stos_m_, SUFFIX)){
    // printf("stos\n");
    MEM_W(reg_l(R_EDI), REG(R_EAX));
    // printf("addr:0x%x, eax:0x%x, mem:0x%x\n", reg_l(R_EDI), REG(R_EAX), swaddr_read(reg_l(R_EDI), 1));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    reg_l(R_EDI) = reg_l(R_EDI) + incDec;
    print_asm_template5();
    return 1;
}

#include "cpu/exec/template-end.h"