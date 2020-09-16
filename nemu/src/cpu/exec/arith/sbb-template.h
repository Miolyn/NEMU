#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute(){
    // uint32_t cfv = reg_eflags(CF);
    // int res = carry_flag(op_dest->val, -(op_src->val + cfv));
    op_src->val += reg_eflags(CF);
    int res = carry_flag(op_dest->val, -op_src->val);
    // int res = sozapc(op_dest->val, -(op_src->val + reg_eflags(CF)), DATA_BYTE);
// #if DATA_BYTE == 1
//     res &= 0xff;
// #endif
// #if DATA_BYTE == 2
//     res &= 0xffff;
// #endif
    parity_flag(res);
    // adjust_flag(op_dest->val, -(op_src->val + cfv));
    adjust_flag(op_dest->val, -op_src->val);
    zero_flag(res);
    sign_flag(res);
    // overflow_flag(op_dest->val, -(op_src->val + cfv));
    overflow_flag(op_dest->val, -op_src->val);
    concat(write_operand_, SUFFIX)(op_dest, res);
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