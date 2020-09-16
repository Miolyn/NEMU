#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	// panic("please implement me");

	// int len = (DATA_BYTE << 3) - 1;
	// cpu.SF=result >> len;
	// int s1,s2;
	// s1=op_src->val>>len;
	// s2=0;
	// cpu.OF=(s1 != s2 && s2 == cpu.SF) ;
	// cpu.ZF=!result;
	// result ^= result >>4;
	// result ^= result >>2;
	// result ^= result >>1;
	// cpu.PF=!(result & 1);

	uint32_t res = result;
    parity_flag(res);
    adjust_flag(op_src->val, -1);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_src->val, -1);
	
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
