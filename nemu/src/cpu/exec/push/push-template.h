#include "cpu/exec/template-start.h"

#define instr push

#if DATA_BYTE == 2 || DATA_BYTE == 4

// #define do_execute concat4(do_, instr, _, SUFFIX)
static void do_execute() {
	// #define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)
	OPERAND_W(op_dest, op_src->val);
    REG(R_SP) = op_src->val;
	print_asm_template2();
}

make_instr_helper(r)

#endif



#include "cpu/exec/template-end.h"