#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    // printf("start to deal with cmp%d\n", DATA_BYTE);
    // if (DATA_BYTE != 4 && (op_src->val >> (DATA_BYTE * 8 - 1)) & 1){
	// 	int st = 0xFFFF0000;
	// 	if (DATA_BYTE == 1) st = 0xFFFFFF00;
	// 	op_src->simm |= st;
	// }
    printf("dest:%x,src:%x\n", op_dest->val, op_src->val);

    int res = carry_flag(op_dest->val, -op_src->val);
    // int res = sozapc(op_dest->val, -op_src->val, DATA_BYTE);
    parity_flag(res);
    adjust_flag(op_dest->val, -op_src->val);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_dest->val, -op_src->val);
    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#if DATA_BYTE != 1
make_instr_helper(sib2rm)
#endif
#include "cpu/exec/template-end.h"