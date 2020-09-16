#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute(){
    op_src->val += cpu.CF;
    printf("d:0x%x,s:0x%x\n", op_dest->val, op_src->val);
    concat(write_operand_, SUFFIX)(op_dest, op_dest->val - op_src->val);

    DATA_TYPE src = op_src->val;
	DATA_TYPE result = op_dest->val - src;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = op_dest->val < src;
	cpu.SF=result >> len;
    	int s1,s2;
	s1=op_dest->val>>len;
	s2=src>>len;
    	cpu.OF=(s1 != s2 && s2 == cpu.SF) ;
    	cpu.ZF=!result;
	result ^= result >>4;
	result ^= result >>2;
	result ^= result >>1;
	cpu.PF=!(result & 1);

    // uint32_t res = op_dest->val - op_src->val;
    // cf_sub(op_dest->val, op_src->val);
    // // cpu.CF = op_dest->val < op_src->val;
    // parity_flag(res);
    // adjust_flag(op_dest->val, -op_src->val);
    // zero_flag(res);
    // sign_flag(res);
    // overflow_flag(op_dest->val, -op_src->val);
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