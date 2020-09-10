#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    printf("start test\n");
    int res = op_src->val & op_dest->val;
    reset_eflags(OF);
    reset_eflags(CF);
    // SF ZF PF
    sign_flag(res);
    zero_flag(res);
    parity_flag(res);
}

// make_instr_helper(ai)
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"