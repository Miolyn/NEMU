#include "cpu/exec/template-start.h"

#define instr push

#if DATA_BYTE == 2 || DATA_BYTE == 4

// #define do_execute concat4(do_, instr, _, SUFFIX)
static void do_execute() {
	printf("start to deal with push%d\n", DATA_BYTE);
	printf("start push reg_name:%s, push_val%x\n", REG_NAME(op_src->reg), op_src->val);
	// #define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)
	// OPERAND_W(op_dest, op_src->val);
	// REG(R_SP) = REG(R_SP) - DATA_BYTE;
	// swaddr_write(REG(R_SP), op_src->val, DATA_BYTE);
	PUSH_STACK(op_src->val);
	print_asm_template2();
}

make_instr_helper(r)

#endif



#include "cpu/exec/template-end.h"