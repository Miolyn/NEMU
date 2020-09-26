#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)){
    uint32_t r = MEM_R(reg_l(R_EDI), R_DS);
    uint32_t l = REG(R_EAX);
    op_dest->val = l; op_src->val = r;

    cf_sub(l, r);
    uint32_t res = l - r;
    parity_flag(res);
    adjust_flag(l, -r);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(l, -r);


    int incDec = cpu.DF ? -DATA_BYTE : DATA_BYTE;
    reg_l(R_EDI) = reg_l(R_EDI) + incDec;

    #ifdef DEBUG
	    snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
        snprintf(op_dest->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EDI));
    #endif
    print_asm_template2();
    return 1;
}

#include "cpu/exec/template-end.h"