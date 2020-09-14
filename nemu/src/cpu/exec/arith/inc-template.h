#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	printf("edx0x%x,\n", reg_l(R_EDX));
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	// panic("please implement me");
	int res = carry_flag(op_src->val, 1);
    parity_flag(res);
    adjust_flag(op_src->val, 1);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_src->val, 1);
	printf("dest type %d\n", op_dest->type);
	if(op_src->reg < 8){
		printf("src reg:%s, val:%x\n", REG_NAME(op_src->reg), reg_l(op_src->reg));
	}
	if(op_dest->reg < 8){
		printf("dest reg:%s, val%x\n", REG_NAME(op_dest->reg), reg_l(op_src->reg));
	}
    // concat(write_operand_, SUFFIX)(op_dest, result);
	concat(write_operand_, SUFFIX)(op_src, result);
	printf("edx0x%x,\n", reg_l(R_EDX));
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
