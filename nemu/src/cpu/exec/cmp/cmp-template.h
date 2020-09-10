#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    printf("start to deal with cmp%d\n", DATA_BYTE);
    printf("dest:%x,src:%x\n", op_dest->val, op_src->val);
    int res = carry_flag(op_dest->val, -op_src->val);
    parity_flag(res);
    adjust_flag(op_dest->val, -op_src->val);
    zero_flag(res);
    printf("zflag %d\n", reg_eflags(ZF));
    sign_flag(res);
    overflow_flag(op_dest->val, -op_src->val);
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(si2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"