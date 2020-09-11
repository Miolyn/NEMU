#include "cpu/exec/template-start.h"

#define instr push


// #define do_execute concat4(do_, instr, _, SUFFIX)
static void do_execute() {
	printf("start to deal with push%d\n", DATA_BYTE);
	// #define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)
	// OPERAND_W(op_dest, op_src->val);
	// REG(R_SP) = REG(R_SP) - DATA_BYTE;
	// swaddr_write(REG(R_SP), op_src->val, DATA_BYTE);
	printf("opcode %x", instr_fetch(cpu.eip, 1));
	printf("before %x push esp %x\n", DATA_BYTE, reg_l(R_ESP));
	PUSH_STACK(op_src->val);
	printf("after push esp %x\n", reg_l(R_ESP));
	print_asm_template2();
}



#if DATA_BYTE == 2 || DATA_BYTE == 4

make_instr_helper(r)
make_instr_helper(m)

#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"