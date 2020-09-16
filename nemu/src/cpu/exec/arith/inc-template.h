#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	OPERAND_W(op_src, op_src->val + 1);

	DATA_TYPE res = op_src->val + 1;
    parity_flag(res);
    adjust_flag(op_src->val, 1);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_src->val, 1);
	

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
