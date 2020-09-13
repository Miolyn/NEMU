#include "cpu/exec/template-start.h"

#define instr lods

#if DATA_BYTE != 1
make_helper(concat(lods_m_, SUFFIX)){
    REG(R_EAX) = MEM_R(REG(R_ESI));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    REG(R_ESI) = REG(R_ESI) + incDec;
    #ifdef DEBUG
	    snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
        snprintf(op_dest->str, OP_STR_SIZE, "%%%s", REG_NAME(R_ESI));
    #endif
    print_asm_template2();
    return 1;
}
#endif

#if DATA_BYTE == 1
make_helper(concat(lods_m_, SUFFIX)){
    printf("addr:%x\n", reg_l(R_ESI));
    printf("esi:%x,edi:%x\n", MEM_R(reg_l(R_ESI)), MEM_R(reg_l(R_EDI)));
    // MEM_W(reg_w(R_ESI), REG(R_EAX));
    REG(R_EAX) = MEM_R(reg_l(R_ESI));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    printf("esi before %x\n", reg_w(R_ESI));
    reg_w(R_ESI) = reg_w(R_ESI) + incDec;
    printf("esi after %x\n", reg_w(R_ESI));
    #ifdef DEBUG
	    snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
        snprintf(op_dest->str, OP_STR_SIZE, "%%%s", REG_NAME(R_ESI));
    #endif
    print_asm_template2();

    return 1;
}
#endif
#include "cpu/exec/template-end.h"