#include "cpu/exec/template-start.h"

#define instr mov

// #define do_execute concat4(do_, instr, _, SUFFIX)
static void do_execute() {
	// #define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)
	// printf("dest:%x,src:%x\n", op_dest->val, op_src->val);
	// if(op_dest->type == OP_TYPE_MEM){
	// 	printf("mem: %x\n", MEM_R(op_dest->addr));
	// }
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

/*
#define make_instr_helper(type) \
	make_helper(concat5(instr, _, type, _, SUFFIX)) { \
		return idex(eip, concat4(decode_, type, _, SUFFIX), do_execute); \
	}
*/
make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

// #define SUFFIX b/w/l : three times of include means three conditions
make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
