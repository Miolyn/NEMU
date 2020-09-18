#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    // printf("l:0x%x,r:0x%x\n", op_dest->val, op_src->val);
    cf_sub(op_dest->val, op_src->val);
    DATA_TYPE res = op_dest->val - op_src->val;
    parity_flag(res);
    adjust_flag(op_dest->val, -op_src->val);
    zero_flag(res);
    sign_flag(res);
    // overflow_flag(op_dest->val, -op_src->val);
    int s1 = sign_bit32(op_dest->val);
    int s2 = sign_bit32(op_src->val);
    cpu.OF=(s1 != s2 && s2 == cpu.SF) ;
    print_asm_template2();
}
/*
#define make_instr_helper(type) \
	make_helper(concat5(instr, _, type, _, SUFFIX)) { \
		return idex(eip, concat4(decode_, type, _, SUFFIX), do_execute); \
	}
*/
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE != 1
make_instr_helper(sib2rm)
#endif

#include "cpu/exec/template-end.h"