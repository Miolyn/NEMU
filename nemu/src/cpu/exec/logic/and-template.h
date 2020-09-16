#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
// #if DATA_BYTE == 2
// 	op_src->val &= 0xffff;
// #endif
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	cpu.CF = 0;
	cpu.OF = 0;
	parity_flag(result);
	sign_flag(result);
	zero_flag(result);
	// panic("please implement me");

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
// make_instr_helper(si2rm)
make_instr_helper(sib2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
