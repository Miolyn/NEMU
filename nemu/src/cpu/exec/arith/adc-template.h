#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
    op_src->val += cpu.CF;
    concat(write_operand_, SUFFIX)(op_dest, op_src->val + op_dest->val);
    
	DATA_TYPE result = op_dest->val + op_src->val;
	int len = (DATA_BYTE << 3) - 1;
	int s1,s2;
	cpu.CF=(result < op_dest->val);
	cpu.SF=result >> len;
	s1=op_dest->val>>len;
	s2=op_src->val>>len;
    	cpu.OF=(s1 == s2 && s1 != cpu.SF) ;
	cpu.ZF=!result;
	result ^= result >>4;
	result ^= result >>2;
	result ^= result >>1;
	cpu.PF=!(result & 1);

    // cf_add(op_dest->val, op_src->val);
    // uint32_t res = op_dest->val + op_src->val;
    // parity_flag(res);
    // adjust_flag(op_dest->val, op_src->val);
    // zero_flag(res);
    // sign_flag(res);
    // overflow_flag(op_dest->val, op_src->val);
    
    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#if DATA_BYTE != 1
make_instr_helper(sib2rm)
#endif

#include "cpu/exec/template-end.h"