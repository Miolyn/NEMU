#include "cpu/exec/template-start.h"

#define instr lods


make_helper(concat(lods_m_, SUFFIX)){
    printf("addr:%x\n", reg_l(R_ESI));
    printf("esi:%x,edi:%x\n", MEM_R(reg_l(R_ESI)), MEM_R(reg_l(R_EDI)));
    REG(R_EAX) = MEM_R(reg_l(R_ESI));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    printf("esi before %x\n", reg_l(R_ESI));
    reg_l(R_ESI) = reg_l(R_ESI) + incDec;
    printf("esi after %x\n", reg_w(R_ESI));
    #ifdef DEBUG
	    snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
        snprintf(op_dest->str, OP_STR_SIZE, "%%%s", REG_NAME(R_ESI));
    #endif
    print_asm_template2();

    return 1;
}

#include "cpu/exec/template-end.h"