#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;

	/* TODO: Update EFLAGS. */
	// panic("please implement me");
	uint32_t res = result;
    parity_flag(res);
    adjust_flag(op_src->val, 1);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_src->val, 1);
	OPERAND_W(op_src, result);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
