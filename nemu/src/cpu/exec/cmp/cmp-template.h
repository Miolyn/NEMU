#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    printf("l:0x%x,r0x%x\n", op_dest->val, op_src->val);

	DATA_TYPE result = op_dest->val - op_src->val;
	int len = (DATA_BYTE << 3) - 1;
	cpu.CF = op_dest->val < op_src->val;
	cpu.SF=result >> len;
    	int s1,s2;
	s1=op_dest->val>>len;
	s2=op_src->val>>len;
    	cpu.OF=(s1 != s2 && s2 == cpu.SF) ;
    	cpu.ZF=!result;
	result ^= result >>4;
	result ^= result >>2;
	result ^= result >>1;
	cpu.PF=!(result & 1);

    // cpu.tmp = true;
    // cf_sub(op_dest->val, op_src->val);
    // DATA_TYPE res = op_dest->val - op_src->val;
    // res ^= res >>4;
	// res ^= res >>2;
	// res ^= res >>1;
	// cpu.PF=!(res & 1);
    // // parity_flag(res);
    // adjust_flag(op_dest->val, -op_src->val);
    // cpu.ZF = !res;
    // zero_flag(res);
    // cpu.SF = sign_bit32(res);
    // // sign_flag(res);
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