#include "cpu/exec/template-start.h"

#define instr push


// #define do_execute concat4(do_, instr, _, SUFFIX)
static void do_execute() {
#if DATA_BYTE == 1
	if(sign_bit8(op_src->val)) op_src->val |= 0xffffff00;
#endif
	PUSH_STACK(op_src->val);
	
	print_asm_template2();
}



#if DATA_BYTE == 2 || DATA_BYTE == 4

make_instr_helper(r)
make_instr_helper(rm)

#endif

make_instr_helper(i)

#if DATA_BYTE != -1

make_helper(concat(pusha_, SUFFIX)){
	uint32_t tmp = REG(R_ESP);
	PUSH_STACK(REG(R_EAX));
	PUSH_STACK(REG(R_ECX));
	PUSH_STACK(REG(R_EDX));
	PUSH_STACK(REG(R_EBX));
	PUSH_STACK(tmp);
	PUSH_STACK(REG(R_EBP));
	PUSH_STACK(REG(R_ESI));
	PUSH_STACK(REG(R_EDI));
	return 1;
}
#endif

#include "cpu/exec/template-end.h"