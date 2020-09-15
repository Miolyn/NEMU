#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
#if DATA_BYTE == 1
    op_dest->val &= 0xff; op_src->val &= 0xff;
#endif
#if DATA_BYTE == 2
    op_dest->val &= 0xffff; op_src->val &= 0xff;
#endif
    printf("l:0x%x,r0x%x\n", op_dest->val, op_src->val);
    uint32_t res = carry_flag(op_dest->val, -op_src->val);
    parity_flag(res);
    adjust_flag(op_dest->val, -op_src->val);
    zero_flag(res);
    sign_flag(res);
    overflow_flag(op_dest->val, -op_src->val);
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