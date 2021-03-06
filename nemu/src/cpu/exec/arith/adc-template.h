#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
    op_src->val += cpu.CF;
    concat(write_operand_, SUFFIX)(op_dest, op_src->val + op_dest->val);

    cf_add(op_dest->val, op_src->val);
    DATA_TYPE res = op_dest->val + op_src->val;
    parity_flag(res);
    adjust_flag(op_dest->val, op_src->val);
    zero_flag(res);
    sign_flag(res);
    // overflow_flag(op_dest->val, op_src->val);
    int s1 = sign_bit32(op_dest->val);
    int s2 = sign_bit32(op_src->val);
    cpu.OF=(s1 == s2 && s1 != cpu.SF) ;
    
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