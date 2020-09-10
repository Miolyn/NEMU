#include "cpu/exec/template-start.h"

#define instr add

static void do_execute(){
    int res = carry_flag(op_dest->val, op_src->val);
    parity_flag(res);
    adjust_flag(op_dest->val, op_src->val);
    zero_flag(res);
    // imm?
    sign_flag(res);
    overflow_flag(op_dest->val, op_src->val);
    concat(write_operand_, SUFFIX)(op_dest, res);
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#if DATA_BYTE != 1 
make_instr_helper(si2rm)

#endif



#include "cpu/exec/template-end.h"