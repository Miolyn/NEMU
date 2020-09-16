#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
    int res = carry_flag(op_dest->val, -op_src->val);
// #if DATA_BYTE == 1
//     res &= 0xff;
// #endif
// #if DATA_BYTE == 2
//     res &= 0xffff;
// #endif
    parity_flag(res);
    adjust_flag(op_dest->val, -op_src->val);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_dest->val, -op_src->val);
    concat(write_operand_, SUFFIX)(op_dest, res);
    print_temp1();
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