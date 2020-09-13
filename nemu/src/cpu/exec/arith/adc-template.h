#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
    int res = carry_flag3(op_dest->val, op_src->val);
    int st = 0xffff;
    if (DATA_BYTE == 1) st = 0xff;
    if (DATA_BYTE != 4) res &= st;
    parity_flag(res);
    adjust_flag3(op_dest->val, op_src->val);
    zero_flag(res);
    sign_flag(res);
    overflow_flag3(op_dest->val, op_src->val);
    concat(write_operand_, SUFFIX)(op_dest, res);
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#if DATA_BYTE != 1
make_instr_helper(sib2rm)
#endif

#include "cpu/exec/template-end.h"