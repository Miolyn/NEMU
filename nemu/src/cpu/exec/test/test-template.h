#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    printf("start to deal with test%d\n", DATA_BYTE);
    int res = op_src->val & op_dest->val;
    printf("test src %x , dest %x\n", op_src->val, op_dest->val);
    reset_eflags(OF);
    reset_eflags(CF);
    sign_flag(res);
    zero_flag(res);
    parity_flag(res);
    printf("zf :%d\n", reg_eflags(ZF));
    print_asm_template2();
}

// make_instr_helper(ai)
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"