#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)){
    int r = MEM_R(reg_l(R_EDI));
    int l = REG(R_EAX);
    // int res = sozapc(l, -r, DATA_BYTE);
    int res = carry_flag(l, -r);
    int st = 0xffff;
    if (DATA_BYTE == 1) st = 0xff;
    if (DATA_BYTE != 4) res &= st;
    parity_flag(res);
    adjust_flag(l, -r);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(l, -r);
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    reg_l(R_EDI) = reg_l(R_EDI) + incDec;

    #ifdef DEBUG
	    snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
        snprintf(op_dest->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EDI));
    #endif
    print_asm_template2();
    return 1;
}

#include "cpu/exec/template-end.h"