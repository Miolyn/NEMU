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

#include "cpu/exec/template-end.h"