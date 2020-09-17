#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)){
    uint32_t r = MEM_R(reg_l(R_EDI));
    uint32_t l = REG(R_EAX);
    op_dest->val = l; op_src->val = r;
    
	// DATA_TYPE result = op_dest->val - op_src->val;
	// int len = (DATA_BYTE << 3) - 1;
	// cpu.CF = op_dest->val < op_src->val;
	// cpu.SF=result >> len;
    // 	int s1,s2;
	// s1=op_dest->val>>len;
	// s2=op_src->val>>len;
    // 	cpu.OF=(s1 != s2 && s2 == cpu.SF) ;
    // 	cpu.ZF=!result;
	// result ^= result >>4;
	// result ^= result >>2;
	// result ^= result >>1;
	// cpu.PF=!(result & 1);

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