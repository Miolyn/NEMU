#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
    panic("hello");
    uint32_t cfv = reg_eflags(CF);
    int res = carry_flag(op_dest->val, op_src->val + cfv);
    int st = 0xffff;
    if (DATA_BYTE == 1) st = 0xff;
    if (DATA_BYTE != 4) res &= st;
    parity_flag(res);
    adjust_flag(op_dest->val, op_src->val + cfv);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_dest->val, op_src->val + cfv);
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