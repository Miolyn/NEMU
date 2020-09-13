#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)){
    int r = 0;
    if (DATA_BYTE == 1){
        r = MEM_R(reg_w(R_EDI));
    } else{
        r = MEM_R(REG(R_EDI));
    }
    int l = REG(R_EAX);
    // int res = oszapc(l, -r, DATA_BYTE);
    int res = carry_flag(l, -r);
    parity_flag(res);
    adjust_flag(l, -r);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(l, -r);
    printf("l%x,r%x\n", l, r);
    printf("res:%x\n", res);
    printf("zf%d\n", reg_eflags(ZF));
    int incDec = reg_eflags(DF) ? -DATA_BYTE : DATA_BYTE;
    if (DATA_BYTE == 1){
        printf("edi before %x\n", reg_w(R_EDI));
        reg_w(R_EDI) = reg_w(R_EDI) + incDec;
        printf("edi after %x\n", reg_w(R_EDI));
    } else{
        REG(R_EDI) = REG(R_EDI) + incDec;
    }
    #ifdef DEBUG
	    snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EAX));
        snprintf(op_dest->str, OP_STR_SIZE, "%%%s", REG_NAME(R_EDI));
    #endif
    print_asm_template2();
    return 1;
}

#include "cpu/exec/template-end.h"