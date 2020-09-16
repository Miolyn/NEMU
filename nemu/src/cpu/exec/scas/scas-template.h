#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)){
    uint32_t r = MEM_R(reg_l(R_EDI));
    uint32_t l = REG(R_EAX);
    op_dest->val = l; op_src->val = r;
    printf("l:0x%x,r:0x%x => 0x%x\n", l, r, l - r);
    // cpu.CF = dest < src
    cf_sub(l, r);
    if(l == 0) cpu.CF = 0;
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