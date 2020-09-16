#include "cpu/exec/template-start.h"

#define instr add

static void do_execute(){
    concat(write_operand_, SUFFIX)(op_dest, op_dest->val + op_src->val);


    cf_add(op_dest->val, op_src->val);
    DATA_TYPE res = op_dest->val + op_src->val;
    parity_flag(res);
    adjust_flag(op_dest->val, op_src->val);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_dest->val, op_src->val);
    
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