#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    printf("src:0x%x,dest:0x%x\n", op_src->val, op_dest->val);
    int res = op_src->val & op_dest->val;
    reset_eflags(OF);
    reset_eflags(CF);
    sign_flag(res);
    zero_flag(res);
    parity_flag(res);
    print_asm_template2();
}

// make_instr_helper(ai)
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"